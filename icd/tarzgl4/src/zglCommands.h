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

#ifndef ZGL_COMMANDS_H
#define ZGL_COMMANDS_H

#include "zglDevices.h"

// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

ZGL afxError _DpuBindAndSyncSamp(zglDpu* dpu, afxUnit glUnit, avxSampler samp);
ZGL afxError _DpuSyncShd(zglDpu* dpu, avxShader shd, avxShaderType stage);
ZGL afxError _DpuCreateShaders(zglDpu* dpu, avxShader codb, avxPipeline pip);
ZGL afxError _DpuSurfSync(zglDpu* dpu, _avxCanvasBin surf, glVmt const* gl); // must be used before texUpdate
ZGL afxError DpuBindAndSyncRas(zglDpu* dpu, afxUnit glUnit, avxRaster tex, afxBool keepBound);
ZGL afxError _ZglTexSubImage(glVmt const* gl, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src);
ZGL afxError _ZglCompressedTexSubImage(glVmt const* gl, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxUnit compressedSiz, afxAddress const src);


ZGL void DpuBindVertexInput(zglDpu* dpu, avxVertexInput vin);
ZGL afxError DpuBindPipeline(zglDpu* dpu, avxPipeline pip, avxVertexInput vin, afxFlags dynamics);
ZGL afxError DpuBindShadersEXT(zglDpu* dpu, avxShaderType stage, avxShader shd);
ZGL afxError _DpuFlushPipelineState(zglDpu* dpu);
ZGL afxError _DpuBindAndResolveLiga(zglDpu* dpu, avxLigature legt, GLuint glHandle);
ZGL afxError _DpuBindAndSyncCanv(zglDpu* dpu, GLenum glTarget, avxCanvas canv, afxBool keepBound);


ZGL afxError DpuBindAndSyncBuf(zglDpu* dpu, GLenum glTarget, avxBuffer buf, afxBool keepBound);
ZGL afxError _DpuRemapBuf(zglDpu* dpu, avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder);
ZGL afxError DpuDumpBuffer(zglDpu* dpu, avxBuffer buf, afxByte* dst, afxUnit opCnt, avxBufferIo const* ops);
ZGL afxError DpuUpdateBuffer(zglDpu* dpu, avxBuffer buf, afxByte const* src, afxUnit opCnt, avxBufferIo const* ops);
ZGL afxError DpuCopyBuffer(zglDpu* dpu, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[]);
ZGL void DpuFillBuffer(zglDpu* dpu, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data);
ZGL afxError _DpuDownloadBuffer(zglDpu* dpu, avxBuffer buf, afxStream out, afxUnit opCnt, avxBufferIo const* ops);
ZGL afxError _DpuUploadBuffer(zglDpu* dpu, avxBuffer buf, afxStream in, afxUnit opCnt, avxBufferIo const* ops);

ZGL afxError _DpuUpdateRaster(zglDpu* dpu, avxRaster ras, afxByte const* src, afxUnit opCnt, avxRasterIo const* ops);
ZGL afxError _DpuDumpRaster(zglDpu* dpu, avxRaster ras, afxByte* dst, afxUnit opCnt, avxRasterIo const* ops);
ZGL afxError _DpuUploadRaster(zglDpu* dpu, avxRaster ras, afxStream in, afxUnit opCnt, avxRasterIo const* ops);
ZGL afxError _DpuDownloadRaster(zglDpu* dpu, avxRaster ras, afxStream out, afxUnit opCnt, avxRasterIo const* ops);
ZGL afxError DpuPackRaster(zglDpu* dpu, avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const* ops);
ZGL afxError DpuUnpackRaster(zglDpu* dpu, avxRaster ras, avxBuffer buf, afxUnit opCnt, avxRasterIo const* ops);

ZGL afxError DpuCopyRaster(zglDpu* dpu, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[]);
ZGL afxError _ZglDpuResolveRaster(zglDpu* dpu, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterCopy const ops[]);
ZGL afxError _ZglDpuBlitRaster(zglDpu* dpu, avxRaster src, avxRaster dst, afxUnit opCnt, avxRasterBlit const ops[], avxTexelFilter flt);
ZGL afxError _ZglDpuClearRaster(zglDpu* dpu, avxRaster ras, avxClearValue const* clearVal, afxUnit baseLodIdx, afxUnit lodCnt, afxUnit baseLayer, afxUnit layerCnt);

ZGL afxError _DpuPresentDout(zglDpu* dpu, afxSurface dout, afxUnit outBufIdx);

ZGL afxError _ZglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLuint fbo, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint z, afxBool multilayered);

ZGL afxError _ZglWaitFenc(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[]);
ZGL afxError _ZglWaitFencOnHost(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[]);
ZGL afxError _ZglResetFenc(afxUnit cnt, avxFence const fences[]);

ZGL void _ZglCopyTexSubImage(zglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxUnit opCnt, avxRasterCopy const ops[]);
ZGL void _ZglUnpackTexSubImage(zglDpu* dpu, GLenum glTarget, afxUnit bpp, GLenum glFmt, GLenum glType, afxUnit opCnt, avxRasterIo const ops[]);

ZGL void DpuBindIndexBuffer(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, afxUnit32 range, afxUnit32 stride);
ZGL void DpuBindVertexBuffers(zglDpu* dpu, afxUnit first, afxUnit cnt, avxBufferedStream const src[]);
ZGL void DpuSetViewports(zglDpu* dpu, afxUnit first, afxUnit cnt, avxViewport const vp[]);

ZGL void _ZglFlushTsChanges(zglDpu* dpu);
ZGL void _ZglFlushRsChanges(zglDpu* dpu);
ZGL void _ZglFlushLigatureState(zglDpu* dpu);
ZGL void _ZglFlushVertexInputState(zglDpu* dpu);

ZGL void DpuCommenceDrawScope(zglDpu* dpu, avxDrawScopeFlags flags, avxCanvas canv, afxLayeredRect const* bounds, afxUnit cCnt, avxDrawTarget const* c, avxDrawTarget const* d, avxDrawTarget const* s, afxString const* dbgTag, afxBool defFbo);
ZGL afxError _ZglDpuClearCanvas(zglDpu* dpu, afxUnit bufCnt, afxUnit const bins[], avxClearValue const values[], afxUnit areaCnt, afxLayeredRect const areas[]);
ZGL void DpuConcludeDrawScope(zglDpu* dpu);
ZGL void DpuNextPass(zglDpu* dpu);

ZGL void DpuMarkDebugMilestone(zglDpu* dpu, avxColor const color, afxString const* label);
ZGL void DpuCommenceDebugScope(zglDpu* dpu, avxColor const color, afxString const* label);
ZGL void DpuConcludeDebugScope(zglDpu* dpu);

ZGL void DpuDraw(zglDpu* dpu, avxDrawIndirect const* data);
ZGL void DpuDrawIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride);
ZGL void DpuDrawIndirect2(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride);
ZGL void DpuDrawIndexed(zglDpu* dpu, avxDrawIndexedIndirect const* data);
ZGL void DpuDrawIndexedIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride);
ZGL void DpuDrawIndexedIndirect2(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride);
ZGL void DpuDispatch(zglDpu* dpu, afxUnit grpCntX, afxUnit grpCntY, afxUnit grpCntZ);
ZGL void DpuDispatchIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset);

ZGL void DpuBindRasters(zglDpu* dpu, afxUnit set, afxUnit first, afxUnit cnt, avxRaster const rasters[]);
ZGL void DpuBindSamplers(zglDpu* dpu, afxUnit set, afxUnit first, afxUnit cnt, avxSampler const samplers[]);
ZGL void DpuBindBuffers(zglDpu* dpu, afxUnit set, afxUnit first, afxUnit cnt, avxBufferedMap const maps[]);

ZGL void DpuPushConstants(zglDpu* dpu, afxUnit32 offset, afxUnit32 siz, void const* data);

ZGL void _DpuPlacePipelineBarrier(zglDpu* dpu, avxBusStage dstStage, avxBusAccess dstAcc);

ZGL _avxCmdLut const cmdDevmt;

ZGL void _DpuEndOfCommands(zglDpu* dpu);

#endif//ZGL_COMMANDS_H
