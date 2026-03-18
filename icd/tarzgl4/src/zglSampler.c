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
    This code unit manages the sampler object system in SIGMA GL/2, which provides texture sampling configuration separate from texture objects 
    themselves. Samplers define how textures are filtered, addressed at boundaries, and compared during shader operations.

    The sampler system (avxSampler) implements OpenGL sampler objects that encapsulate texture sampling state.
    Samplers are identified by the object class afxFcc_SAMP and maintain OpenGL sampler handles per execution unit.
    Per-DPU Handle Management: Like other resources, samplers maintain separate OpenGL handles for each execution unit to 
    support concurrent command recording across multiple threads.

    Samplers are bound through the ligature system using a deferred binding pattern. The binding process stages changes in "next" state that are later synchronized to "active" state.

    Some buggy drivers incorrectly bind samplers per-program rather than globally. To work around this, the system provides explicit unbinding.
    This workaround ensures samplers are re-bound after pipeline changes on drivers that don't maintain sampler state correctly.

    The actual OpenGL sampler binding occurs during _ZglFlushLigatureState, which is called before draw commands to synchronize pending state changes.

    When a pipeline is linked, sampler uniforms in the shader program must be resolved to texture unit indices.

    The _DpuBindAndSyncSamp function ensures the sampler object exists on the GPU and is ready for use.


*/

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

//#define FORCE_GL_SAMPLER_SETTINGS 1

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_ZGL afxError _DpuBindAndSyncSamp(zglDpu* dpu, afxUnit glUnit, avxSampler samp)
{
    //AfxEntry("smp=%p", smp);
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Despite samplers being data-like, these are not always reliably shared between contexts.
    */

    if (!samp)
    {
        gl->BindSampler(glUnit, 0); _ZglThrowErrorOccuried();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
        zglUpdateFlags devUpdReq = (samp->updFlags & ZGL_UPD_FLAG_DEVICE);
        GLuint glHandle = samp->glHandle;
        afxBool bound = FALSE;

        if ((!glHandle) || (devUpdReq & ZGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _ZglThrowErrorOccuried();
                samp->glHandle = NIL;
                glHandle = NIL;
            }

            gl->GenSamplers(1, &(glHandle)); _ZglThrowErrorOccuried();
            gl->BindSampler(glUnit, glHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(gl->IsSampler(glHandle));
            samp->glHandle = glHandle;
            bound = TRUE;

            if (samp->m.tag.len)
            {
                gl->ObjectLabel(GL_SAMPLER, glHandle, samp->m.tag.len, (GLchar const*)samp->m.tag.start); _ZglThrowErrorOccuried();
            }
            GLenum magF = ZglToGlTexelFilterMode(samp->m.cfg.magnify);
            GLenum minF = ZglToGlTexelFilterModeMipmapped(samp->m.cfg.minify, samp->m.cfg.mipFlt);
            GLint wrapS = ZglToGlTexWrapMode(samp->m.cfg.uvw[0]);
            GLint wrapT = ZglToGlTexWrapMode(samp->m.cfg.uvw[1]);
            GLint wrapR = ZglToGlTexWrapMode(samp->m.cfg.uvw[2]);
            GLint cop = ZglToGlCompareOp(samp->m.cfg.compareOp);

            gl->SamplerParameteri(glHandle, GL_TEXTURE_MAG_FILTER, magF); _ZglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_MIN_FILTER, minF); _ZglThrowErrorOccuried();

            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_S, wrapS); _ZglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_T, wrapT); _ZglThrowErrorOccuried();
            gl->SamplerParameteri(glHandle, GL_TEXTURE_WRAP_R, wrapR); _ZglThrowErrorOccuried();

#ifndef FORCE_GL_SAMPLER_SETTINGS
            if (samp->m.cfg.anisotropyEnabled)
#endif
            {
                gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_ANISOTROPY, samp->m.cfg.anisotropyMaxDegree); _ZglThrowErrorOccuried();
            }

            gl->SamplerParameterf(glHandle, GL_TEXTURE_LOD_BIAS, samp->m.cfg.lodBias); _ZglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MIN_LOD, samp->m.cfg.minLod); _ZglThrowErrorOccuried();
            gl->SamplerParameterf(glHandle, GL_TEXTURE_MAX_LOD, samp->m.cfg.maxLod); _ZglThrowErrorOccuried();

#ifndef FORCE_GL_SAMPLER_SETTINGS
            if (!samp->m.cfg.compareEnabled)
            {
                gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _ZglThrowErrorOccuried();
            }
            else
#endif
            {
                // what about GL_TEXTURE_COMPARE_MODE?
                gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); _ZglThrowErrorOccuried();
                gl->SamplerParameteri(glHandle, GL_TEXTURE_COMPARE_FUNC, cop); _ZglThrowErrorOccuried();
            }

#ifndef FORCE_GL_SAMPLER_SETTINGS
            if ((samp->m.cfg.uvw[0] == avxTexelWrap_BORDER) || 
                (samp->m.cfg.uvw[1] == avxTexelWrap_BORDER) || 
                (samp->m.cfg.uvw[2] == avxTexelWrap_BORDER))
#endif
            {
                /*
                    The GL_CLAMP_TO_BORDER requires a color that edge texels are blended when texture coordinates fall outside 
                    of the valid area of the texture. When this this edge mode is used, a border color must be set by setting 
                    the GL_TEXTURE_BORDER_COLOR parameter.
                */
                gl->SamplerParameterfv(glHandle, GL_TEXTURE_BORDER_COLOR, (void*)samp->m.cfg.borderColor.rgba); _ZglThrowErrorOccuried();
            }

#if 0
            /*
                As an OpenGL extension, seamless cubemap filtering can be manipulated on a per-texture or per-sampler object basis. 
                Note that if you use the global version, it will still globally force seamless behavior on all cubemaps.

                This is governed by a simple boolean sampler parameter, GL_TEXTURE_CUBE_MAP_SEAMLESS. Setting it to 0 turns it off 
                for that texture/sampler; setting it to any other value causes accesses to the cubemap to be seamless.
            */
            // In Vulkan, there is a extension to do the opposite of it; to disable it.
            gl->SamplerParameteri(glHandle, GL_TEXTURE_CUBE_MAP_SEAMLESS, TRUE); _ZglThrowErrorOccuried();
#endif

            //AfxReportMessage("Hardware-side sampler %p ready.", samp);
            samp->updFlags &= ~(ZGL_UPD_FLAG_DEVICE);
        }
        else
        {
            gl->BindSampler(glUnit, glHandle); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError _ZglSampDtor(avxSampler samp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    if (samp->glHandle)
    {
        afxDrawSystem dsys = AvxGetSamplerHost(samp);
        _ZglDsysEnqueueDeletion(dsys, 0, GL_SAMPLER, (afxSize)samp->glHandle);
        samp->glHandle = 0;
    }

    if (_AVX_CLASS_CONFIG_SAMP.dtor(samp))
        AfxThrowError();

    return err;
}

_ZGL afxError _ZglSampCtor(avxSampler samp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);

    if (_AVX_CLASS_CONFIG_SAMP.ctor(samp, args, invokeNo)) AfxThrowError();
    else
    {
        samp->glHandle = 0;
        samp->updFlags = ZGL_UPD_FLAG_DEVICE_INST;

        if (err && _AVX_CLASS_CONFIG_SAMP.dtor(samp))
            AfxThrowError();
    }
    return err;
}
