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

/*
    This code unit manages the shader compilation, GLSL generation, program linking, and caching mechanisms within the SIGMA GL/2 system. 
    It explains how shader source code (stored in avxShader objects) is transformed into OpenGL shader and program objects, 
    including the dynamic GLSL generation system that adds version directives, standard defines, and platform-specific workarounds.

    The shader compilation process is triggered during pipeline creation or first use, coordinated by the _DpuFlushPipelineState function 
    when a pipeline lacking an OpenGL program handle is bound. The primary compilation entry point is _DpuCreateShaders, which processes 
    all shader stages defined in a pipeline configuration.

    The compilation process iterates through each shader stage defined in the pipeline configuration (pip->m.stages), compiling individual 
    shaders and storing their OpenGL handles in pip->stagesExt[i].glShaderHandle. After all stages are compiled, an OpenGL program object is 
    created, shaders are attached, the program is linked, and ligature (resource binding) resolution is performed before the shaders are 
    detached and the program handle is cached.

    The system dynamically generates GLSL source code by combining multiple components: version directives, standard defines/macros, 
    stage-specific defines, platform workarounds, and the actual shader code. This approach allows shader source to be written in a more portable, 
    AFX-friendly syntax that is transformed into valid GLSL at compilation time.

    The GLSL version directive is constructed based on the avxShader program's specified version or system defaults.
    The system injects standard type aliases and convenience macros to make GLSL code more compatible with AFX conventions:

    The compilation process injects defines that identify the current shader stage, allowing shared shader code to conditionally compile stage-specific logic.

    The system provides a workaround for OpenGL implementations lacking ARB_shader_draw_parameters extension support.

    GLSL requires the shader entry point to be named main, but the system allows specification of alternative entry point names in the pipeline 
    stage configuration (pip->m.stages[stageIdx].fn). When a non-empty entry point name is provided, a preprocessor define is injected to remap it.
    This allows multiple entry points to be defined in a single shader source file, with the active one selected at compilation time.

    Shader objects are represented by the avxShader class, which stores shader source code and metadata. The lifecycle follows the system's 
    standard resource management pattern with deferred GPU instantiation and deletion.
    When an avxShader is destroyed, its OpenGL shader and program handles are not immediately deleted. Instead, they are enqueued for deferred 
    deletion using _ZglDsysEnqueueDeletion, which ensures that GPU resources are not freed while they might still be referenced by in-flight commands.

    After linking, the system must resolve resource bindings (uniforms, uniform buffers, storage buffers, samplers) between the shader's declared 
    resources and the ligature (descriptor set) schema. This is performed by _DpuBindAndResolveLiga, which queries the linked program for active 
    uniforms and uniform blocks, mapping them to ligature binding points.

    This resolution step is critical for the later resource binding process to correctly bind buffers and textures to the locations expected by the shader.

    After linking completes, the individual shader objects are detached from the program using glDetachShader. This is safe because the compiled 
    shader code is embedded in the program object. The shader handles remain stored in pip->stagesExt[i].glShaderHandle for potential reuse if 
    the pipeline is recreated.

    The system implements a multi-level caching strategy to avoid redundant shader compilation and program linking.
    Compiled shader objects are cached in the pipeline's stagesExt array. When _DpuCreateShaders is called, it first checks if 
    pip->stagesExt[stageIdx].glShaderHandle already exists. If so, that stage's compilation is skipped. This allows shader stages to be reused 
    across multiple pipeline recreations or updates.


*/

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"



afxChar const stdIncVs[] =
"#extension ARB_shader_draw_parameters : enable\n";

afxChar const stdShaderDefines[] =
" \
\n#define afxInt int \
\n#define afxUnit uint \
\n#define afxReal float \
\n#define afxV2d vec2 \
\n#define afxV3d vec3 \
\n#define afxV4d vec4 \
\n#define afxM2d mat2 \
\n#define afxM3d mat3 \
\n#define afxM4d mat4 \
\n#define float2 vec2 \
\n#define float3 vec3 \
\n#define float4 vec4 \
\n \
\n#define IN(_at_, _type_, _name_) layout(location = _at_) in _type_ _name_ \
\n#define OUT(_at_, _type_, _name_) layout(location = _at_) out _type_ _name_ \
// enforce early tests \
\n#define IN_EARLY_FRAGMENT_TESTS() layout(early_fragment_tests) in \
\n#define IN_CANVAS_COORD() in vec4 gl_FragCoord \
\n#define IN_WINDOW_COORD() layout(origin_upper_left) in vec4 gl_FragCoord \
\n \
// ubo = std140, ssbo = std430 \
\n#define UNIFORM(_set_, _binding_, _layout_, _name_) layout(_layout_) uniform _name_ \
\n#define BUFFER(_set_, _binding_, _layout_, _access_, _name_) layout(_layout_) _access_ buffer _name_ \
\n#define TEXTURE(_set_, _binding_, _type_, _name_) uniform _type_ _name_ \
\n#define PUSH(_name_) layout(std140, location = 1023) uniform /*_name_*/ pushable \
\n#define SPECIALIZED(_specid_, _type_, _name_) layout(_specid_) const _type_ _name_ \
\n#define FETCH(_set_, _binding_, _name_) uniform usamplerBuffer _name_ \
\n \
\n#define fetch(_bufName_, _elemIdx_) texelFetch(_bufName_, _elemIdx_) \
\n \
\n#define VTX_ID gl_VertexID \
\n#define INST_ID gl_InstanceID \
\n \
\n const float PI = 3.14159265359;\
";

/*
    in int gl_VertexID;
    in int gl_InstanceID;
    in int gl_DrawID; // Requires GLSL 4.60 or ARB_shader_draw_parameters
    in int gl_BaseVertex; // Requires GLSL 4.60 or ARB_shader_draw_parameters
    in int gl_BaseInstance; // Requires GLSL 4.60 or ARB_shader_draw_parameters

    gl_VertexID
    the index of the vertex currently being processed. When using non-indexed rendering, it is the effective index of the current vertex
    (the number of vertices processed + the first value). For indexed rendering, it is the index used to fetch this vertex from the buffer.
    Note: gl_VertexID will have the baseVertex parameter added to the index, if there was such a parameter in the rendering command.

    gl_InstanceID
    the index of the current instance when doing some form of instanced rendering. The instance count always starts at 0,
    even when using base instance calls. When not using instanced rendering, this value will be 0.
    Warning: This value does not follow the baseInstance provided by some instanced rendering functions.
    gl_InstanceID always falls on the half-open range [0, instancecount).
    If you have GLSL 4.60, you may use gl_BaseInstance to compute the proper instance index.

    gl_DrawID
    the index of the drawing command within multi-draw rendering commands (including indirect multi-draw commands).
    The first draw command has an ID of 0, increasing by one as the renderer passes through drawing commands.
    This value will always be a Dynamically Uniform Expression.

    gl_BaseVertex
    the value of the baseVertex parameter of the rendering command. If the rendering command did not include that parameter,
    the value of this input will be 0.

    gl_BaseInstance
    the value of the baseInstance parameter of the instanced rendering command. If the rendering command did not include this parameter,
    the value of this input will be 0.
*/

afxChar const incDrawParamsEmulated[] =
"\
\n#define DRAW_ID zgl_DrawID  \
\n#define BASE_VTX zgl_BaseVertex  \
\n#define BASE_INST zgl_BaseInstance \
";

afxChar const incDrawParams[] =
"\
\n#define DRAW_ID gl_DrawID  \
\n#define BASE_VTX gl_BaseVertex  \
\n#define BASE_INST gl_BaseInstance \
";

afxChar const incCommFuncs[] =
"float saturate(float x) { \n \
    return clamp(x, 0.0, 1.0);\n \
}\n \
\n \
vec2 saturate(vec2 x) { \n \
    return clamp(x, 0.0, 1.0);\n \
}\n \
\n \
vec3 saturate(vec3 x) {\n \
    return clamp(x, 0.0, 1.0);\n \
}\n \
\n \
vec4 saturate(vec4 x) {\n \
    return clamp(x, 0.0, 1.0);\n \
}\n";

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_ZGL afxError _DpuCreateShaders(zglDpu* dpu, avxShader codb, avxPipeline pip)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);

    afxChar errStr[1024];
    afxUnit tmpShdGlHandleCnt = 0;

    // transient strings
    afxString4096 tmps;
    AfxMakeString4096(&tmps, NIL);

    afxArray code;
    AfxMakeArray(&code, sizeof(afxByte), 4096, NIL, 0);

    for (afxUnit stageIdx = 0; stageIdx < pip->m.progCnt; stageIdx++)
    {
        if (pip->progsExt[stageIdx].glShaderHandle)
            continue;

        AfxEmptyArray(&code, TRUE, FALSE);
        afxUnit progId = pip->m.progs[stageIdx].progId;
        avxShaderType stage = pip->m.progs[stageIdx].stage;

        _avxCodeBlock* prog;
        if (!AvxGetShaderCrate(codb, progId, (void**)&prog))
        {
            AfxThrowError();
            continue;
        }

        // meta includes #version, #extension, etc; things required to be at topmost part of code.
        afxString4096 meta;
        AfxMakeString4096(&meta, NIL);
        AfxFormatString(&meta.s, "#version %d%d0 // Qwadro \n", AFX_MAX(ZGL_DEFAULT_CTX_VER_MAJOR, prog->verMajor), prog->verMajor ? prog->verMinor : AFX_MAX(ZGL_DEFAULT_CTX_VER_MINOR, prog->verMinor), prog->extended ? " " : " core");

        // defines includes types mirrored from AFX (to ease #include reuse), convenient macroes to get rid of some GLSL pollution.
        afxString4096 defines;
        AfxMakeString4096(&defines, NIL);
        AfxCatenateString(&defines.s, stdShaderDefines, sizeof(stdShaderDefines));

        afxUnit arrel;
        void* room;
#if 0 // now cospecified at glShaderSource
        room = AfxPushArrayUnits(&code, sizeof(stdInc) - 1, &arrel, NIL);
        AfxCopy(room, stdInc, sizeof(stdInc) - 1);
#endif
        // Workaround to emulate GL Core 4.6 draw parameters.
        if (dpu->emulatedDrawParams)
        {
            AfxCatenateString(&defines.s, incDrawParamsEmulated, sizeof(incDrawParamsEmulated) - 1);
        }
        else
        {
            AfxCatenateString(&meta.s, stdIncVs, sizeof(stdIncVs) - 1);
            AfxCatenateString(&defines.s, incDrawParams, sizeof(incDrawParams) - 1);
        }

        // Workaround to select an shader entry-point function in GLSL.
        if (!AfxIsStringEmpty(&pip->m.progs[stageIdx].fn.s))
        {
            AfxFormatString(&tmps.s, "\n#define main %.*s \n", AfxPushString(&pip->m.progs[stageIdx].fn.s));
            void* room = AfxPushArrayUnits(&code, tmps.s.len, &arrel, NIL, 0);
            AfxDumpString(&tmps.s, 0, tmps.s.len, room);
        }

        // Some #define's to make shader aware of its environment.
        switch (stage)
        {
        case avxShaderType_VERTEX:
        {
            AfxFormatString(&tmps.s, "\n#define _VERTEX_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        case avxShaderType_FRAGMENT:
        {
            AfxFormatString(&tmps.s, "\n#define _FRAGMENT_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        case avxShaderType_COMPUTE:
        {
            AfxFormatString(&tmps.s, "\n#define _COMPUTE_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        case avxShaderType_PRIMITIVE:
        {
            AfxFormatString(&tmps.s, "\n#define _PRIMITIVE_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        case avxShaderType_TESS_EVAL:
        {
            AfxFormatString(&tmps.s, "\n#define _TESS_EVAL_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        case avxShaderType_TESS_CTRL:
        {
            AfxFormatString(&tmps.s, "\n#define _TESS_CTRL_ ");
            AfxCatenateString(&defines.s, tmps.buf, tmps.s.len);
            break;
        }
        default: AfxThrowError(); break;
        }
#if 0
        AfxCatenateString(&defines.s, incCommFuncs, sizeof(incCommFuncs));
        void* room2 = AfxPushArrayUnits(&code, sizeof(incCommFuncs), &arrel, NIL, 0);
        AfxCopy(room2, incCommFuncs, sizeof(incCommFuncs));
#endif
        afxUnit nullTermArrel;
        //AvxDumpShaderCode(codb, progId, &code);
        {
            _avxCodeBlock* slot;
            if (!AvxGetShaderCrate(codb, progId, (void**)&slot))
                return afxError_NOT_FOUND;

            if (slot->codeLen)
            {
                afxString cbs;
                AfxMakeString(&cbs, 0, slot->code, slot->codeLen);
                _AvxConvertToGlsl(&cbs, &code);
            }
        }
        AfxPushArrayUnits(&code, 1, &nullTermArrel, (afxChar[]) { 0 }, 0);

        GLuint shader;

        if (!(shader = gl->CreateShader(AfxToGlShaderStage(stage))))
        {
            _ZglThrowErrorOccuried();
        }
        else
        {
            GLint compiled = 0;

            if (codb->m.tag.len)
            {
                gl->ObjectLabel(GL_SHADER, shader, codb->m.tag.len, (GLchar const*)codb->m.tag.start); _ZglThrowErrorOccuried();
            }

            //gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _ZglThrowErrorOccuried();
#if !0
            //if (echo)
            {
                AfxReportComment("%.*s", code.pop, code.bytemap);
            }
#endif
            gl->ShaderSource(shader, 3, (GLchar const*const[]) { (void*)meta.buf, (void*)defines.buf, (void*)code.bytemap }, (GLint const[]) { meta.s.len, defines.s.len, code.pop }); _ZglThrowErrorOccuried();
            gl->CompileShader(shader); _ZglThrowErrorOccuried();
            gl->GetShaderiv(shader, GL_COMPILE_STATUS, &compiled); _ZglThrowErrorOccuried();

            if (compiled == GL_FALSE)
            {
                AfxThrowError();
                gl->GetShaderInfoLog(shader, sizeof(errStr), NIL, (GLchar*)errStr); _ZglThrowErrorOccuried();
                AfxReportError(errStr);
                gl->DeleteShader(shader); _ZglThrowErrorOccuried();
                AfxReportError((afxChar const*)code.bytemap);
            }
            else
            {
                pip->progsExt[stageIdx].glShaderHandle = shader;
                //glShaders[tmpShdGlHandleCnt] = shader;
                tmpShdGlHandleCnt++;
            }
        }

        if (err)
        {
            for (afxUnit i = tmpShdGlHandleCnt; i-- > 0;)
            {
                //gl->DeleteShader(glShaders[tmpShdGlHandleCnt]); _ZglThrowErrorOccuried();
                //glShaders[tmpShdGlHandleCnt] = NIL;
                gl->DeleteShader(pip->progsExt[stageIdx].glShaderHandle); _ZglThrowErrorOccuried();
                pip->progsExt[stageIdx].glShaderHandle = NIL;
            }
            break;
        }
    }

    AfxEmptyArray(&code, FALSE, FALSE);

    //*glShaderCnt = tmpShdGlHandleCnt;

    return err;
}

#if 0
_ZGL afxError _DpuSyncShd(zglDpu* dpu, avxShader shd, avxShaderType stage)
{
    //AfxEntry("shd=%p", shd);
    afxError err = { 0 };
    glVmt const* gl = &dpu->gl;

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if ((shd->updFlags & ZGL_UPD_FLAG_DEVICE))
    {
        if (!shd->glHandle || (shd->updFlags & ZGL_UPD_FLAG_DEVICE_INST))
        {
            if (shd->glHandle)
            {
                gl->DeleteShader(shd->glHandle); _ZglThrowErrorOccuried();
                shd->glHandle = NIL;
            }

            AFX_ASSERT(NIL == shd->glHandle);

            if (!(shd->glHandle = gl->CreateShader(AfxToGlShaderStage(stage))))
            {
                AfxThrowError();
                _ZglThrowErrorOccuried();
            }
            else
            {
                AFX_ASSERT(shd);
                AFX_ASSERT(gl->IsShader(shd->glHandle));
                const GLint codeLens[] = { shd->m.codeLen };
                GLchar const* const codes[] = { (GLchar const*)shd->m.code };
                gl->ShaderSource(shd->glHandle, 1, codes, codeLens); _ZglThrowErrorOccuried();
                gl->CompileShader(shd->glHandle); _ZglThrowErrorOccuried();
                GLint status = 0;
                gl->GetShaderiv(shd->glHandle, GL_COMPILE_STATUS, &status); _ZglThrowErrorOccuried();

                if (status == GL_FALSE)
                {
                    AfxThrowError();
                    afxChar str[1024];
                    gl->GetShaderInfoLog(shd->glHandle, sizeof(str), NIL, (GLchar*)str); _ZglThrowErrorOccuried();
                    AfxReportError(str);
                    gl->DeleteShader(shd->glHandle); _ZglThrowErrorOccuried();
                    shd->glHandle = NIL;
                }
                else
                {
                    shd->glProgHandle = 0;
                    shd->compiled = TRUE;
                    shd->updFlags &= ~(ZGL_UPD_FLAG_DEVICE_INST | ZGL_UPD_FLAG_DEVICE_FLUSH);
                    AfxReportMessage("avxShader %p hardware-side data instanced.", shd);
                }
            }
        }
        else if ((shd->updFlags & ZGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AFX_ASSERT(shd->glHandle);
            
            AfxThrowError(); // can't be modified
        }
    }
    return err;
}
#endif

_ZGL afxError _ZglShdDtor(avxShader shd)
{
    afxError err = { 0 };

    afxDrawSystem dsys = AvxGetShaderHost(shd);
    
    if (shd->glHandle)
    {
        _ZglDsysEnqueueDeletion(dsys, 0, GL_SHADER, (afxSize)shd->glHandle);
        shd->glHandle = 0;
    }

    if (shd->glProgHandle)
    {
        _ZglDsysEnqueueDeletion(dsys, 0, GL_PROGRAM, (afxSize)shd->glProgHandle);
        shd->glProgHandle = 0;
    }

    if (_AVX_CLASS_CONFIG_CODB.dtor(shd))
        AfxThrowError();

    return err;
}

_ZGL afxError _ZglShdCtor(avxShader shd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if (_AVX_CLASS_CONFIG_CODB.ctor(shd, args, invokeNo)) AfxThrowError();
    else
    {
        shd->glHandle = NIL;
        shd->glProgHandle = NIL;
        shd->updFlags = ZGL_UPD_FLAG_DEVICE_INST;
        shd->compiled = FALSE;

        afxDrawSystem dsys = AvxGetShaderHost(shd);
        shd->shdUniqueId = ++dsys->shdUniqueId;

        if (err && _AVX_CLASS_CONFIG_CODB.dtor(shd))
            AfxThrowError();
    }
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    return err;
}
