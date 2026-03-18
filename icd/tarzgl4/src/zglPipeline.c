/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

 // OpenGL/Vulkan Continuous Integration

/*
    This code unit manages the avxPipeline object lifecycle, structure, and management within the SIGMA GL/2 system. 
    It covers pipeline construction and destruction, per-DPU OpenGL program handle management, update flag synchronization, 
    and binding operations.

    The avxPipeline object encapsulates a complete graphics pipeline state, including shader programs, rasterization configuration, 
    blending, depth/stencil settings, and resource binding schemas. It acts as a reusable state object similar to Vulkan's VkPipeline 
    or Direct3D 12's ID3D12PipelineState.

    Pipeline objects maintain separate OpenGL program handles for each execution unit (DPU) and swap iteration. This design enables:
     - Concurrent Recording: Multiple threads can record commands using the same pipeline simultaneously
     - Triple Buffering: The _ZGL_PSO_SWAPS constant (typically 3) enables triple-buffered pipeline state
     - Resource Isolation: Each DPU has independent GL state, preventing cross-context conflicts

    Pipeline construction occurs when the application requests a new pipeline through the draw system. When a pipeline is first bound to a DPU, 
    the system detects the missing OpenGL handle and performs lazy instantiation. After initial instantiation, binding the pipeline simply 
    reuses the cached OpenGL program handle.

    Pipeline destruction uses deferred deletion to ensure OpenGL resources aren't freed while GPU commands are in-flight.

    The DpuBindPipeline function transitions a pipeline to active status and propagates its configuration to the DPU's "next" state fields.

    When binding a pipeline, DpuBindPipeline copies pipeline configuration to the DPU's "next" state fields. 
    This implements a deferred state update pattern.

    The per-DPU handle array architecture supports concurrent command recording across multiple execution units.
    A modulo operation implements circular buffering with _ZGL_PSO_SWAPS (typically 3) slots, enabling triple-buffered pipeline state per DPU.

    After DpuBindPipeline sets the nextPip field, the actual OpenGL state changes occur during _DpuFlushPipelineState.

    Pipeline instantiation delegates shader compilation to _DpuCreateShaders.
    After linking the GL program, _DpuBindAndResolveLiga resolves resource bindings.
*/

#include "zglCommands.h"
#include "zglObjects.h"
#include "zglUtils.h"

#define _FLUSH_ON_PROGRAM_SWITCH TRUE
//#define _FLUSH_ON_PROGRAM_SWITCH_FOR_DRAW_SCOPE TRUE

_ZGL afxError _DpuFlushPipelineState(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    afxUnit psoHandleIdx = dpu->dpuIterIdx % _ZGL_PSO_SWAPS;
    avxPipeline pip = dpu->nextPip;
    
    if (pip != dpu->activePip)
    {
        //dpu->nextPip = NIL;
        dpu->activePip = pip;

        if (!pip)
        {
            gl->UseProgram(0); _ZglThrowErrorOccuried();
            //gl->BindProgramPipeline(0); _ZglThrowErrorOccuried();
            //gl->BindVertexArray(0); _ZglThrowErrorOccuried();
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

            GLuint glHandle = pip->perDpu[dpu->m.exuIdx][psoHandleIdx].glHandle;
            zglUpdateFlags devUpdReq = (pip->updFlags & ZGL_UPD_FLAG_DEVICE);
            afxBool bound = FALSE;

            if (glHandle && !(devUpdReq & ZGL_UPD_FLAG_DEVICE_INST))
            {
                AFX_ASSERT(gl->IsProgram(glHandle));
                gl->UseProgram(glHandle); _ZglThrowErrorOccuried();
                bound = TRUE;
            }
            else
            {
                //afxArray code;
                afxChar errStr[1024];
                //afxUnit tmpShdGlHandleCnt = 0;
                //GLuint tmpShdGlHandles[8];
                //AfxMakeArray(&code, sizeof(afxChar), 2048, NIL, 0);

                avxShader codb = pip->m.codb;
                AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);

                //if (_DpuCreateShaders(dpu, codb, pip->m.stageCnt, pip->m.stages, &tmpShdGlHandleCnt, tmpShdGlHandles))
                    //AfxThrowError();

                if (_DpuCreateShaders(dpu, codb, pip))
                    AfxThrowError();

                if (!err)
                {
                    if (glHandle)
                    {
                        gl->DeleteProgram(glHandle), glHandle = NIL; _ZglThrowErrorOccuried();
                    }

                    if (!(glHandle = gl->CreateProgram()))
                    {
                        _ZglThrowErrorOccuried();
                    }
                    else
                    {
                        afxBool linked;

                        if (pip->m.tag.len)
                        {
                            gl->ObjectLabel(GL_PROGRAM, glHandle, pip->m.tag.len, (GLchar const*)pip->m.tag.start); _ZglThrowErrorOccuried();
                        }

                        for (afxUnit i = 0; i < pip->m.progCnt; i++)
                        {
                            gl->AttachShader(glHandle, pip->progsExt[i].glShaderHandle); _ZglThrowErrorOccuried();
                        }

                        gl->LinkProgram(glHandle); _ZglThrowErrorOccuried();
                        gl->GetProgramiv(glHandle, GL_LINK_STATUS, &linked); _ZglThrowErrorOccuried();

                        if (linked == GL_FALSE)
                        {
                            AfxThrowError();
                            gl->GetProgramInfoLog(glHandle, sizeof(errStr), NIL, (GLchar*)errStr); _ZglThrowErrorOccuried();
                            AfxReportError(errStr);
                        }

                        // required bind due to issue with Intel Graphics Drivers no allowing retrieve of uniform locations only after assembling.
                        AFX_ASSERT(gl->IsProgram(glHandle));
                        gl->UseProgram(glHandle); _ZglThrowErrorOccuried();
                        bound = TRUE;

                        //AvxGetPipelineLigature(&pip, &liga);

                        if (_DpuBindAndResolveLiga(dpu, pip->m.liga, glHandle))
                            AfxThrowError();

                        for (afxUnit i = pip->m.progCnt; i-- > 0;)
                        {
                            gl->DetachShader(glHandle, pip->progsExt[i].glShaderHandle); _ZglThrowErrorOccuried();
                        }

                        if (err)
                        {
                            gl->DeleteProgram(glHandle); _ZglThrowErrorOccuried();
                            glHandle = NIL;
                        }
                    }

                    for (afxUnit i = pip->m.progCnt; i-- > 0;)
                    {
                        //gl->DeleteShader(pip->m.stages[i].glShaderHandle); _ZglThrowErrorOccuried();
                        //pip->m.stages[i].glShaderHandle = NIL;
                    }

                    if (!err)
                    {
                        //AfxReportMessage("avxPipeline %p hardware-side data instanced.", pip);
                        pip->updFlags &= ~(ZGL_UPD_FLAG_DEVICE);
                    }

                    pip->perDpu[dpu->m.exuIdx][psoHandleIdx].glHandle = glHandle;
                }
            }
        }

#ifdef _FLUSH_ON_PROGRAM_SWITCH
        // Draw scope already flushes explicitly on conclusion or implictly on FBO switch.
#ifndef _FLUSH_ON_PROGRAM_SWITCH_FOR_DRAW_SCOPE
        if (!dpu->inDrawScope)
#endif
        {
            gl->Flush();
        }
#endif
    }

    _ZglFlushTsChanges(dpu);
    _ZglFlushRsChanges(dpu);
}

_ZGL afxError DpuBindShadersEXT(zglDpu* dpu, avxShaderType stage, avxShader shd)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    GLuint glStage = AfxToGlShaderStageBit(stage);

    gl->UseProgramStages(dpu->activeProgPipGlHandle, glStage, shd->glProgHandle);
}

_ZGL afxError DpuBindPipeline(zglDpu* dpu, avxPipeline pip, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    if (!pip)
    {
        dpu->nextPip = NIL;
        dpu->nextLiga = NIL;
        DpuBindVertexInput(dpu, vin);
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    dpu->nextPip = pip;
    AvxGetPipelineLigature(pip, &dpu->nextLiga);
    DpuBindVertexInput(dpu, vin ? vin : pip->m.vin);

    afxBool applyStates = FALSE;
    applyStates = TRUE;
    
#if 0
    if (applyStates)
#endif
    {
        avxPipelineInfo config;
        AvxDescribePipeline(pip, &config);

        dpu->nextVpCnt = pip->m.vpCnt;

        dpu->nextPrimTop = pip->m.primTop;
        dpu->nextPrimRestartEnabled = pip->m.primRestartEnabled;
        dpu->nextDepthClampEnabled = pip->m.depthClampEnabled;

        if ((dpu->nextCullMode = pip->m.cullMode))
        {
            dpu->nextFrontFaceCw = pip->m.frontFacingInverted;
        }

        // RASTERIZATION

        dpu->nextRasterizationDisabled = pip->m.rasterizationDisabled;

        if ((dpu->nextOutCnt = pip->m.outCnt))
        {
            AfxCopy(dpu->nextOuts, pip->m.outs, sizeof(pip->m.outs[0]) * pip->m.outCnt);

            AvxCopyColor(dpu->nextBlendConstants, pip->m.blendConstants);
            //dpu->nextBlendConstUpd = 1;
        }

        if ((dpu->nextDepthBiasEnabled = pip->m.depthBiasEnabled))
        {
            dpu->nextDepthBiasClamp = pip->m.depthBiasClamp;
            dpu->nextDepthBiasConstFactor = pip->m.depthBiasConstFactor;
            dpu->nextDepthBiasSlopeScale = pip->m.depthBiasSlopeScale;
        }

        if ((dpu->nextDepthBoundsTestEnabled = pip->m.depthBoundsTestEnabled))
            AfxV2dCopy(dpu->nextDepthBounds, pip->m.depthBounds);

        if ((dpu->nextDepthTestEnabled = pip->m.depthTestEnabled))
        {
            dpu->nextDepthCompareOp = pip->m.depthCompareOp;
        }

        dpu->nextDepthWriteDisabled = pip->m.depthWriteDisabled;

        dpu->nextFillMode = pip->m.fillMode;
        dpu->nextLineWidth = pip->m.lineWidth;

        if ((dpu->nextMinSampleShadingValue = pip->m.minSampleShadingValue))
        {
            dpu->nextSampleShadingEnabled = pip->m.sampleShadingEnabled;
        }

        dpu->nextMsEnabled = pip->m.msEnabled;

        dpu->nextAlphaToCoverageEnabled = pip->m.alphaToCoverageEnabled;
        dpu->nextAlphaToOneEnabled = pip->m.alphaToOneEnabled;

        if ((dpu->nextLogicOpEnabled = pip->m.logicOpEnabled))
        {
            dpu->nextLogicOp = pip->m.logicOp;
        }

        if ((dpu->nextSampleLvl = pip->m.sampleLvl))
        {
            AfxCopy(dpu->nextSampleMasks, pip->m.sampleMasks, sizeof(pip->m.sampleMasks) * pip->m.sampleLvl);
        }

        if ((dpu->nextStencilTestEnabled = pip->m.stencilTestEnabled))
        {
            dpu->nextStencilBack = pip->m.stencilBack;
            dpu->nextStencilFront = pip->m.stencilFront;
        }
    }

    return err;
}

#if 0
_ZGL afxResult _AfxRegisterOpenGlResourcesToQwadroDrawPipeline(avxPipeline pip)
{
    glVmt* gl = &(((afxDrawSystem)AfxGetHost(&pip->obj))->vmt);
#if 0
    avxShaderType                      stages;
    avxShaderParam              type;
    avxPipelineSubresType           baseType;
    afxUnit32                            set,
        binding,
        loc,
        inputAttachIdx,
        vecSiz,
        columns,
        arrSiz,
        off,
        siz;
    afxEmbeddedString(name, 16);
    avxPipelineSubresource const    *subresources;
#endif
    _DrawPipelineResource res, *resptr;
    GLuint gpuHandle = pip->gpuHandle;

    afxInt cnt = 0;
    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORM_BLOCKS, &cnt); _ZglThrowErrorOccuried();

    for (afxInt i = 0; i < cnt; i++)
    {
        AFX_ZERO(&res);

        GLint binding, dataSiz, nameLen, subresCnt, subresIndices[1], refByVsh, refByFsh, refByGsh;
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_BINDING, &binding);  _ZglThrowErrorOccuried();// The current block binding, as set either within the shader or from gl->UniformBlockBinding.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSiz);  _ZglThrowErrorOccuried();// The buffer object storage size needed for this block.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);  _ZglThrowErrorOccuried();// The length of this block's name.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &subresCnt);  _ZglThrowErrorOccuried();// The number of active uniforms within this block.
        //glGetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &subresIndices); // params​ will be filled in with the uniform indices of all uniforms that are stored in this block.It will receive GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS number of uniforms.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &refByVsh);  _ZglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, &refByFsh);  _ZglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER, &refByGsh);  _ZglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.


        res.binding = binding;
        res.resType = avxShaderParam_UNIFORM;
        res.set = 0;
        res.location = i;
        res.arraySize = 0;
        res.vecSize = 0;
        res.inputAttachmentIndex = 0;
        res.size = dataSiz;
        res.stages = 0;
        gl->GetActiveUniformBlockName(gpuHandle, i, sizeof(res.name), NIL, res.name); _ZglThrowErrorOccuried();
        res.name[15] = '\0';
        //res.loc = gl->GetUniformLocation(gpuHandle, res.nameBuf); _ZglThrowErrorOccuried();

        afxResult j = AfxPipeline.RegisterResource(pip, &res);
        AfxPipeline.GetResource(pip, j, &res);
        AFX_ASSERT(i == j);
        AfxReportMessage("%d %d %d %d", res.loc, res.binding, i, j);
    }

    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORMS, &cnt);

    for (GLuint i = 0; i < (GLuint)cnt; i++)
    {
        AFX_ZERO(&res);
        AfxDeployString(&res.name, res.nameBuf);

        afxBool valid = FALSE;

        GLint type, blockIdx;
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_TYPE, &type); _ZglThrowErrorOccuried();
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIdx); _ZglThrowErrorOccuried();

        if (blockIdx == -1)
        {
            switch (type)
            {
            case GL_SAMPLER_1D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _ZglThrowErrorOccuried();
                res.resType = avxShaderParam_TEXTURE;
                valid = TRUE;
                break;
            case GL_SAMPLER_2D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _ZglThrowErrorOccuried();
                res.resType = avxShaderParam_TEXTURE;
                valid = TRUE;
                break;
            case GL_SAMPLER_3D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _ZglThrowErrorOccuried();
                res.resType = avxShaderParam_TEXTURE;
                valid = TRUE;
                break;
            case GL_SAMPLER_CUBE:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _ZglThrowErrorOccuried();
                res.resType = avxShaderParam_TEXTURE;
                valid = TRUE;
                break;
            default:
                AfxReportError("%X is a unhandled type", type);
                valid = FALSE;
                break;
            }

            if (valid)
            {
                res.location = gl->GetUniformLocation(gpuHandle, res.name); _ZglThrowErrorOccuried();
                //res.loc = i;
                afxSupplyDescriptor entry[1];
                afxResult j = AfxPipeline.FindEntries(pip, 1, &res.name, entry);
                //AfxPipeline.GetResource(pip, j, &res);

                if (j == 1)
                {
                    AFX_ASSERT(i == j);
                    gl->BindUniform(gpuHandle, );
                    AfxReportMessage("%d %d %d %d", res.location, res.binding, i, j);
                }
            }
        }
    }
    return 0;
}
#endif

_ZGL afxError _ZglPipDtor(avxPipeline pip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxDrawSystem dsys = AvxGetPipelineHost(pip);

    for (afxUnit i = 0; i < ZGL_MAX_DPUS; i++)
    {
        for (afxUnit j = 0; j < _ZGL_PSO_SWAPS; j++)
        {
            if (pip->perDpu[i][j].glHandle)
            {
                _ZglDsysEnqueueDeletion(dsys, i, GL_PROGRAM, (afxSize)pip->perDpu[i][j].glHandle);
                pip->perDpu[i][j].glHandle = 0;
            }
        }
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = pip->m.progCnt,
            .siz = sizeof(pip->progsExt[0]),
            .var = (void*)&pip->progsExt
        }
    };

    if (AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    if (_AVX_CLASS_CONFIG_PIP.dtor(pip))
        AfxThrowError();

    return err;
}

_ZGL afxError _ZglPipCtor(avxPipeline pip, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxDrawSystem dsys = args[0];
    avxPipelineConfig const *pipb = ((avxPipelineConfig const*)args[1]) + invokeNo;
    //AfxAssertType(pipb, afxFcc_PIPB);

    if (_AVX_CLASS_CONFIG_PIP.ctor(pip, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = pip->m.progCnt,
            .siz = sizeof(pip->progsExt[0]),
            .var = (void*)&pip->progsExt
        }
    };

    if (AfxAllocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        _AVX_CLASS_CONFIG_PIP.dtor(pip);
        return err;
    }

    for (afxUnit i = 0; i < pip->m.progCnt; i++)
        pip->progsExt[i].glShaderHandle = NIL;

    AfxZero(pip->perDpu, sizeof(pip->perDpu));
    pip->updFlags = ZGL_UPD_FLAG_DEVICE_INST;

    pip->pipUniqueId = ++dsys->pipUniqueId;

    if (err && _AVX_CLASS_CONFIG_PIP.dtor(pip))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    return err;
}
