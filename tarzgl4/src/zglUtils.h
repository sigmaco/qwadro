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

#ifndef AVX_TO_GL_H
#define AVX_TO_GL_H

#include "zglDefs.h"

ZGL GLenum AfxToGlFrontFace(avxFrontFace ff);
ZGL GLenum AfxToGlCullMode(avxCullMode cm);
ZGL GLenum AfxToGlFillMode(avxFillMode pm);
ZGL GLenum ZglToGlCompareOp(avxCompareOp co);
ZGL GLuint AfxToGlShaderStage(avxShaderType s);
ZGL GLuint AfxToGlShaderStageBit(avxShaderType s);
ZGL GLenum AfxToGlTopology(avxTopology pm);
ZGL GLenum AfxToGlBufferUsage(afxInt bt);
ZGL GLenum AfxToGlStencilOp(avxStencilOp so);
ZGL GLenum AfxToGlBlendOp(avxBlendOp f);
ZGL GLenum AfxToGlBlendFactor(avxBlendFactor f);
#if 0
ZGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxUnit32* stride);
#endif
ZGL GLenum AfxToGlLogicOp(avxLogicOp f);
//ZGL void ZglToGlFormat(avxFormat pf, GLint *fmt, GLenum *layout, GLenum *type);
ZGL GLenum ZglToGlImageDimensions(afxUnit d);
ZGL GLenum ZglToGlTexelFilterMode(avxTexelFilter tfm);
ZGL GLenum ZglToGlTexelFilterModeMipmapped(avxTexelFilter min, avxTexelFilter mip);
ZGL GLenum ZglToGlTexWrapMode(avxTexelWrap twm);
ZGL GLenum ZglToGlColorSwizzle(avxColorSwizzle swizzle);
ZGL void ZglToGlColorSwizzling(avxSwizzling const *swizzling, GLenum *arrayedSwizzles);
ZGL void ZglDetermineGlTargetInternalFormatType(avxRaster tex, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type);

ZGL void ZglToGlFormat2(avxFormat pf, GLint *fmt, GLenum *layout, GLenum *type);

// TEXTURES

ZGL afxError zglDeleteTextures(glVmt const* gl, GLsizei n, GLuint *textures);
ZGL afxError zglCreateTextures(glVmt const* gl, GLenum target, GLsizei n, GLuint *textures);

ZGL afxError zglBindTextureUnit(glVmt const* gl, GLuint unit, GLenum target, GLuint texture);
ZGL afxError zglBindTextures(glVmt const* gl, GLuint first, GLsizei count, const GLenum* targets, const GLuint *textures);

ZGL afxError zglMakeTextureStorage(glVmt const* gl, GLenum target, GLuint texture, GLsizei levelsOrSamples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

ZGL afxError zglUpdateTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
ZGL afxError zglUpdateTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);

ZGL afxError zglUnpackTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLuint buffer, GLsizei offset);
ZGL afxError zglUnpackTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLuint buffer, GLsizei offset);

ZGL afxError zglDumpTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
ZGL afxError zglDumpTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);

ZGL afxError zglPackTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLuint buffer, GLsizei offset);
ZGL afxError zglPackTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLuint buffer, GLsizei offset);

ZGL afxError zglBindSamplers(glVmt const* gl, GLuint first, GLsizei count, const GLuint *samplers);

// FRAMEBUFFER AND RENDERBUFFERS

ZGL afxError zglCreateRenderbuffers(glVmt const* gl, GLsizei n, GLuint *renderbuffers);
ZGL afxError zglMakeRenderbufferStorage(glVmt const* gl, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

ZGL afxError zglBlitFramebuffer(glVmt const* gl, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

// BUFFERS

ZGL afxError zglDeleteBuffers(glVmt const* gl, GLsizei n, GLuint *buffers);
ZGL afxError zglCreateBuffers(glVmt const* gl, GLenum target, GLsizei n, GLuint *buffers);

ZGL afxError zglBindBufferBase(glVmt const* gl, GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
ZGL afxError zglBindBufferRange(glVmt const* gl, GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);

ZGL afxError zglMakeBufferStorage(glVmt const* gl, GLenum target, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
ZGL afxError zglUpdateBufferSubData(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);

ZGL afxError zglClearBufferData(glVmt const* gl, GLenum target, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
ZGL afxError zglClearBufferSubData(glVmt const* gl, GLenum target, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
ZGL afxError zglCopyNamedBufferSubData(glVmt const* gl, GLenum readTarget, GLuint readBuffer, GLenum writeTarget, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

ZGL afxError zglMapBuffer(glVmt const* gl, GLenum target, GLuint buffer, GLenum access, void** pp);
ZGL afxError zglMapBufferRange(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access, void** pp);
ZGL afxError zglUnmapBuffer(glVmt const* gl, GLenum target, GLuint buffer);
ZGL afxError zglFlushBufferRange(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr length);

ZGL afxError zglTextureBuffer(glVmt const* gl, GLuint texture, GLenum internalformat, GLuint buffer);
ZGL afxError zglTextureBufferRange(glVmt const* gl, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);

#endif//AVX_TO_GL_H
