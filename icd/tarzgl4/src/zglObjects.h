/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ZGL_OBJECTS_H
#define ZGL_OBJECTS_H

#include "zglDevices.h"

// Most OpenGL objects are sharable, including Sync Objects and GLSL Objects. 
// Container Objects are not sharable, nor are Query Objects.
#define ZGL_MAX_DPUS 32
#define ZGL_MAX_QO_HANDLES 32
#define _ZGL_PSO_SWAPS 3
#define _ZGL_FENC_SET_POP 3

AFX_OBJECT(avxPipeline)
{
    AFX_OBJECT(_avxPipeline) m;

    afxUnit pipUniqueId;
    zglUpdateFlags  updFlags;
    struct
    {
        GLuint          glHandle;
        //zglVertexInput  vertexInput;
    } perDpu[ZGL_MAX_DPUS][_ZGL_PSO_SWAPS];
    struct
    {
        GLuint glShaderHandle;
    }* progsExt;
};

AFX_OBJECT(avxVertexInput)
{
    AFX_OBJECT(_avxVertexInput) m;

    afxUnit vdeclUniqueId;
    zglUpdateFlags  updFlags;
    struct
    {
        GLuint          glHandle;
        zglVertexInputState bindings; // TODO: per DPU
    } perDpu[ZGL_MAX_DPUS][_ZGL_VAO_SWAPS]; // the second set is to avoid contention in dynamic binding.
};

AFX_OBJECT(avxQueryPool)
{
    AFX_OBJECT(_avxQueryPool) m;

    afxUnit qrypUniqueId;
    zglUpdateFlags  updFlags;
    GLenum          glTarget;
    struct
    {
        GLuint         *glHandle; // ZGL_MAX_QO_HANDLES * cap
    } perDpu[ZGL_MAX_DPUS];
};

AFX_OBJECT(avxLigature)
{
    AFX_OBJECT(_avxLigature) m;
    struct
    {
        GLuint  texBufGlHandle[8];
    } perDpu[ZGL_MAX_DPUS][_ZGL_PSO_SWAPS];
    afxUnit tboCnt;
};

AFX_OBJECT(avxShader)
{
    AFX_OBJECT(_avxShader) m;

    afxUnit shdUniqueId;
    zglUpdateFlags      updFlags;
    afxUnit              glHandle;
    afxUnit              glProgHandle;
    afxBool             compiled;
};

AFX_OBJECT(avxSampler)
{
    AFX_OBJECT(_avxSampler) m;

    afxUnit smpUniqueId;
    zglUpdateFlags  updFlags;
    afxUnit          glHandle;
};

AFX_OBJECT(avxFence)
{
    AFX_OBJECT(_avxFence) m;

    afxUnit         fencUniqueId;
    zglUpdateFlags  updFlags;
    union
    {
        afxAtomPtr  glHandleAtom;
        GLsync      glHandle;
    };
    HANDLE          hEventW32;
    afxUnit64       nextValueToSignal;
    afxLink         onSignalChain;
    afxLink         onWaitChain;
};

AFX_OBJECT(afxSemaphore)
{
    AFX_OBJECT(_afxSemaphore) m;
};

AFX_OBJECT(avxBuffer)
{
    AFX_OBJECT(_avxBuffer) m;
    // idd

    afxUnit bufUniqueId;
    zglUpdateFlags  updFlags;
    afxUnit          glHandle;
    //afxUnit          glTexHandle; // if GL_TEXTURE_BUFFER, it would be a GL texture handle used to bind the buffer.
    // Since Vulkan doesn't share OpenGL's notion that this is some kind of texture, the descriptor's data is a VkBufferView, not a VkImage or VkImageView. Specifically, VkWriteDescriptorSet::pTexelBufferView.
    GLenum          glTexIntFmt;
    GLenum          glTarget;
    //GLenum          glUsage;
    GLbitfield      glGenAccess;
    GLbitfield      glMapRangeAccess;
    GLbitfield      glMapWholeAccess;
    //GLintptr        glMappedOff;
    //GLsizeiptr      glMappedSiz;
};

AFX_OBJECT(avxRaster)
{
    AFX_OBJECT(_avxRaster) m;

    afxUnit rasUniqueId;
    zglUpdateFlags  updFlags;
    GLuint          glHandle; // when avxRasterUsage_DRAW only, it is a GL_RENDERBUFFER
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
    GLenum          glAttachment;
};

AFX_OBJECT(avxCanvas)
{
    AFX_OBJECT(_avxCanvas) m;

    afxUnit             canvUniqueId;
    zglUpdateFlags      updFlags;
    struct
    {
        afxUnit         glHandle;
        afxMask         storeBitmask;
    } perDpu[ZGL_MAX_DPUS][_ZGL_FBO_SWAPS];
};

ZGL void _ZglDsysEnqueueDeletion(afxDrawSystem dsys, afxUnit exuIdx, afxUnit type, afxSize gpuHandle);


ZGL afxError _ZglWaitFenc(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[]);
ZGL afxError _ZglResetFenc(afxUnit cnt, avxFence const fences[]);

ZGL afxClassConfig const _ZglFencMgrCfg;
ZGL afxClassConfig const _ZglSemMgrCfg;

ZGL afxError _ZglPipCtor(avxPipeline pip, void** args, afxUnit invokeNo);
ZGL afxError _ZglPipDtor(avxPipeline pip);
ZGL afxError _ZglShdCtor(avxShader shd, void** args, afxUnit invokeNo);
ZGL afxError _ZglShdDtor(avxShader shd);
ZGL afxError _ZglVinCtor(avxVertexInput vin, void** args, afxUnit invokeNo);
ZGL afxError _ZglVinDtor(avxVertexInput vin);
ZGL afxError _ZglCanvCtor(avxCanvas canv, void** args, afxUnit invokeNo);
ZGL afxError _ZglCanvDtor(avxCanvas canv);
ZGL afxError _ZglRasCtor(avxRaster ras, void** args, afxUnit invokeNo);
ZGL afxError _ZglRasDtor(avxRaster ras);
ZGL afxError _BufCtorCb(avxBuffer buf, void** args, afxUnit invokeNo);
ZGL afxError _BufDtorCb(avxBuffer buf);
ZGL afxError _ZglSampCtor(avxSampler samp, void** args, afxUnit invokeNo);
ZGL afxError _ZglSampDtor(avxSampler samp);
ZGL afxError _ZglLigaCtor(avxLigature liga, void** args, afxUnit invokeNo);
ZGL afxError _ZglLigaDtor(avxLigature liga);
ZGL afxError _ZglQrypCtor(avxQueryPool qryp, void** args, afxUnit invokeNo);
ZGL afxError _ZglQrypDtor(avxQueryPool qryp);

ZGL afxError _ZglCmdbCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo);
ZGL afxError _ZglCmdbDtorCb(afxDrawContext dctx);



#endif//ZGL_OBJECTS_H
