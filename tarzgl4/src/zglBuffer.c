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
    This code unit manages the buffer management subsystem in SIGMA GL/2, focusing on the lifecycle, synchronization mechanisms, 
    and OpenGL state management of avxBuffer objects. Buffer management encompasses object construction, device instantiation, 
    binding/synchronization through DpuBindAndSyncBuf, and destruction.

    The avxBuffer object extends the platform-agnostic buffer interface with OpenGL-specific state and handles.

    The DpuBindAndSyncBuf function serves as the central buffer synchronization and binding mechanism. It is invoked before every buffer 
    operation to ensure the OpenGL handle exists and is optionally bound to a target.
    The function uses Direct State Access (DSA) when available for better performance at.

    Buffer destruction implements a safe deferred deletion pattern to prevent use-after-free errors when GPU commands are in flight.
    The destructor enqueues the OpenGL handle for deletion rather than deleting immediately.
    Before enqueuing deletion the destructor ensures the buffer is unmapped.
    This prevents leaking mapped memory regions that would persist after the OpenGL handle is deleted.

    Many buffer operations temporarily bind buffers to specialized OpenGL targets for specific purposes. 
    The choice between specialized targets and the buffer's natural target is controlled by preprocessor flags.

    Buffer management integrates with the command processing system through worker thread operations that handle mapping and synchronization requests.

    The buffer operation subsystem provides several categories of data transfer functionality.
    All operations follow the DSA (Direct State Access) vs legacy fallback pattern, selecting the appropriate OpenGL path based on available extensions.

    DpuUpdateBuffer uploads data from CPU memory to a GPU buffer. It handles both linear and strided transfers.
    DpuDumpBuffer downloads data from a GPU buffer to CPU memory. It uses a similar pattern to DpuUpdateBuffer but in reverse.
    DpuCopyBuffer performs GPU-side buffer-to-buffer copies without CPU involvement.
    DpuFillBuffer fills a buffer range with a constant 32-bit value. It attempts three strategies in order.

    The system provides stream-based variants for buffer I/O operations that work with afxStream objects instead of raw memory pointers.
    _DpuUploadBuffer reads data from an afxStream and writes it to a GPU buffer.
    _DpuDownloadBuffer reads data from a GPU buffer and writes it to an afxStream.

    _DpuRemapBuf provides direct CPU access to GPU buffer memory through mapping.
    The mapping flags (glMapRangeAccess) are determined during buffer construction.
*/

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

//#define _ALWAYS_UNBIND_GL_BUF TRUE

_ZGL afxError DpuBindAndSyncBuf(zglDpu* dpu, GLenum glTarget, avxBuffer buf, afxBool keepBound)
{
    //AfxEntry("buf=%p", buf);
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    if (!buf)
    {
        if (keepBound)
        {
            gl->BindBuffer(glTarget, 0); _ZglThrowErrorOccuried();
        }
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        zglUpdateFlags devUpdReq = (buf->updFlags & ZGL_UPD_FLAG_DEVICE);
        GLuint glHandle = buf->glHandle;

        if (glHandle && !(devUpdReq & ZGL_UPD_FLAG_DEVICE_INST))
        {
            if (keepBound)
            {
                gl->BindBuffer(glTarget, glHandle); _ZglThrowErrorOccuried();

                if (buf->glTarget == GL_TEXTURE_BUFFER)
                {
                    AFX_ASSERT(buf->m.usage & avxBufferUsage_FETCH);
                    // Como selecionar a texture unit?
                    //gl->BindTexture(GL_TEXTURE_BUFFER, buf->glTexHandle); _ZglThrowErrorOccuried();
                }
            }
        }
        else
        {
            if (glHandle)
            {
                gl->DeleteBuffers(1, &(glHandle)); _ZglThrowErrorOccuried();
                buf->glHandle = NIL;
                glHandle = NIL;
#if 0
                if (buf->glTexHandle)
                {
                    AFX_ASSERT(buf->glTarget == GL_TEXTURE_BUFFER);
                    AFX_ASSERT(buf->m.usage & avxBufferUsage_FETCH);
                    gl->DeleteTextures(1, &buf->glTexHandle); _ZglThrowErrorOccuried();
                    buf->glTexHandle = NIL;
                }
#endif
            }

            _ZglThrowErrorOccuried();
            afxBool bound = FALSE;

            if ((!keepBound) && (glTarget == buf->glTarget) && gl->CreateBuffers && gl->NamedBufferStorage)
            {
                gl->CreateBuffers(1, &glHandle); _ZglThrowErrorOccuried();
                gl->NamedBufferStorage(glHandle, buf->m.reqSiz, NIL, buf->glGenAccess); _ZglThrowErrorOccuried();
            }
            else
            {
                gl->GenBuffers(1, &glHandle); _ZglThrowErrorOccuried();
                gl->BindBuffer(buf->glTarget, glHandle); _ZglThrowErrorOccuried();
                gl->BufferStorage(buf->glTarget, buf->m.reqSiz, NIL, buf->glGenAccess); _ZglThrowErrorOccuried();
                bound = TRUE;
            }

            AFX_ASSERT(gl->IsBuffer(glHandle));
            if (buf->m.tag.len)
            {
                gl->ObjectLabel(GL_BUFFER, glHandle, buf->m.tag.len, (GLchar const*)buf->m.tag.start); _ZglThrowErrorOccuried();
            }
            buf->glHandle = glHandle;
            buf->updFlags &= ~(ZGL_UPD_FLAG_DEVICE);
            //AfxReportMessage("GPU buffer %p ready. %u, %u, %x", buf, buf->glTarget, glHandle, buf->m.usage);

            if (!keepBound)
            {
                if (bound)
                {
                    // if bound and is to be kept bound, unbind it.
                    gl->BindBuffer(buf->glTarget, 0); _ZglThrowErrorOccuried();
                    bound = FALSE;
                }
            }
            else
            {
                if (glTarget != buf->glTarget)
                {
                    if (bound)
                    {
                        // if bound at creation target and is NOT to be kept bound at creation target, unbind it.
                        gl->BindBuffer(buf->glTarget, 0); _ZglThrowErrorOccuried();
                    }
                    // if it is to be kept bound but has not been bound yet correct target, bind it.
                    gl->BindBuffer(glTarget, glHandle); _ZglThrowErrorOccuried();
                }
                else
                {
                    if (!bound)
                    {
                        // if not bound and is to be kept bound, bind it.
                        gl->BindBuffer(glTarget, glHandle); _ZglThrowErrorOccuried();
                        bound = TRUE;
                    }
                }
            }

#if 0
            if (buf->glTarget == GL_TEXTURE_BUFFER)
            {
                AFX_ASSERT(buf->m.usage & avxBufferUsage_FETCH);
                afxBool texBound = FALSE;

                if ((!keepBound) && gl->CreateTextures && gl->TextureBuffer)
                {
                    gl->CreateTextures(GL_TEXTURE_BUFFER, 1, &buf->glTexHandle); _ZglThrowErrorOccuried();
                    gl->TextureBuffer(buf->glTexHandle, buf->glTexIntFmt, glHandle); _ZglThrowErrorOccuried();
                }
                else
                {
                    gl->ActiveTexture(GL_TEXTURE0 + ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT); _ZglThrowErrorOccuried();
                    gl->GenTextures(1, &buf->glTexHandle); _ZglThrowErrorOccuried();
                    gl->BindTexture(GL_TEXTURE_BUFFER, buf->glTexHandle); _ZglThrowErrorOccuried();
                    gl->TexBuffer(GL_TEXTURE_BUFFER, buf->glTexIntFmt, glHandle); _ZglThrowErrorOccuried();
                    //texBound = TRUE;
                    gl->BindTexture(GL_TEXTURE_BUFFER, 0); _ZglThrowErrorOccuried();
                }
            }
#endif
        }
    }
    return err;
}

_ZGL afxError DpuCopyBuffer(zglDpu* dpu, avxBuffer src, avxBuffer dst, afxUnit opCnt, avxBufferCopy const ops[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

#ifdef _USE_RW_SLOTS_ON_BUFFER_COPY
#ifdef _USE_IO_SLOTS_ON_BUFFER_COPY
    GLenum glTargetSrc = GL_PIXEL_UNPACK_BUFFER;
    GLenum glTargetDst = GL_PIXEL_PACK_BUFFER;
#else
    GLenum glTargetSrc = GL_COPY_READ_BUFFER;
    GLenum glTargetDst = GL_COPY_WRITE_BUFFER;
#endif
#else
    GLenum glTargetSrc = src->glTarget;
    GLenum glTargetDst = dst->glTarget;
#endif

    if (gl->CopyNamedBufferSubData)
    {
        DpuBindAndSyncBuf(dpu, glTargetSrc, src, FALSE); // sync
        DpuBindAndSyncBuf(dpu, glTargetDst, dst, FALSE); // sync
        
        for (afxUnit i = 0; i < opCnt; i++)
        {
            gl->CopyNamedBufferSubData(src->glHandle, dst->glHandle, ops[i].srcOffset, ops[i].dstOffset, ops[i].range); _ZglThrowErrorOccuried();
        }
    }
    else
    {
        DpuBindAndSyncBuf(dpu, glTargetSrc, src, TRUE); // bind
        DpuBindAndSyncBuf(dpu, glTargetDst, dst, TRUE); // bind

        for (afxUnit i = 0; i < opCnt; i++)
        {
            gl->CopyBufferSubData(glTargetSrc, glTargetDst, ops[i].srcOffset, ops[i].dstOffset, ops[i].range); _ZglThrowErrorOccuried();
        }
#if _ALWAYS_UNBIND_GL_BUF
        // unbind buffers
        DpuBindAndSyncBuf(dpu, glTargetSrc, NIL, TRUE); // unbind
        DpuBindAndSyncBuf(dpu, glTargetDst, NIL, TRUE); // unbind
#endif
    }
    return err;
}

_ZGL void DpuFillBuffer(zglDpu* dpu, avxBuffer buf, afxUnit offset, afxUnit range, afxUnit data)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
#ifdef _USE_RW_SLOTS_ON_BUFFER_FILL
#ifdef _USE_IO_SLOTS_ON_BUFFER_FILL
    GLenum glTarget = GL_PIXEL_PACK_BUFFER;
#else
    GLenum glTarget = GL_COPY_WRITE_BUFFER;
#endif
#else
    GLenum glTarget = buf->glTarget;
#endif

    if (gl->ClearNamedBufferSubData)
    {
        DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
        gl->ClearNamedBufferSubData(buf->glHandle, GL_R32UI, offset, range, GL_R32UI, GL_UNSIGNED_INT, &data); _ZglThrowErrorOccuried();
    }
    else if (gl->ClearBufferSubData)
    {
        DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
        gl->ClearBufferSubData(glTarget, GL_R32UI, offset, range, GL_R32UI, GL_UNSIGNED_INT, &data); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
    }
    else if (gl->MapNamedBufferRange)
    {
        DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync

        AFX_ASSERT(buf->glGenAccess & GL_MAP_WRITE_BIT);
        void* dst = gl->MapNamedBufferRange(buf->glHandle, offset, range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
        AFX_ASSERT(dst);
        AfxStream(range / sizeof(data), 0, sizeof(data), &data, dst);
        gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
        gl->FlushMappedNamedBufferRange(buf->glHandle, offset, range); _ZglThrowErrorOccuried();
    }
    else
    {
        DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind

        AFX_ASSERT(buf->glGenAccess & GL_MAP_WRITE_BIT);
        void* dst = gl->MapBufferRange(glTarget, offset, range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
        AFX_ASSERT(dst);
        AfxStream(range / sizeof(data), 0, sizeof(data), &data, dst);
        gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
        gl->FlushMappedBufferRange(glTarget, offset, range); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
    }
}

_ZGL afxError DpuDumpBuffer(zglDpu* dpu, avxBuffer buf, afxByte* dst, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
#ifdef _USE_RW_SLOTS_ON_BUFFER_DUMP
#ifdef _USE_IO_SLOTS_ON_BUFFER_DUMP
    GLenum glTarget = GL_PIXEL_UNPACK_BUFFER;
#else
    GLenum glTarget = GL_COPY_READ_BUFFER;
#endif
#else
    GLenum glTarget = buf->glTarget;
#endif
    afxBool bufSynced = FALSE;
    afxBool bufBound = FALSE;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];

        if (op->srcStride == op->dstStride)
        {
            // LINEAR VERSION
            afxUnit bufRange = AFX_MIN(op->rowCnt * op->srcStride, AvxGetBufferCapacity(buf, op->srcOffset));

            if (gl->GetNamedBufferSubData)
            {
                if (!bufSynced)
                {
                    DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE);
                    bufSynced = TRUE;
                }
                gl->GetNamedBufferSubData(buf->glHandle, op->srcOffset, bufRange, &dst[op->dstOffset]); _ZglThrowErrorOccuried();
            }
            else
            {
                if (!bufBound)
                {
                    DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE);
                    bufBound = TRUE;
                    bufSynced = TRUE;
                }
                gl->GetBufferSubData(glTarget, op->srcOffset, bufRange, &dst[op->dstOffset]); _ZglThrowErrorOccuried();
            }
        }
        else
        {
            // STRIDED VERSION
            afxUnit bufRange = AFX_MIN(op->rowCnt * op->srcStride, AvxGetBufferCapacity(buf, op->srcOffset));

            if (gl->MapNamedBufferRange)
            {
                if (!bufSynced)
                {
                    DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE);
                    bufSynced = TRUE;
                }

                afxByte const* src = gl->MapNamedBufferRange(buf->glHandle, op->srcOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

                if (!src)
                {
                    AfxThrowError();
                    continue;
                }

                AfxStream2(op->rowCnt, src, op->srcStride, &dst[op->dstOffset], op->dstStride);
                gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
            }
            else
            {
                if (!bufBound)
                {
                    DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE);
                    bufBound = TRUE;
                    bufSynced = TRUE;
                }

                afxByte const* src = gl->MapBufferRange(glTarget, op->srcOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

                if (!src)
                {
                    AfxThrowError();
                    continue;
                }

                AfxStream2(op->rowCnt, src, op->srcStride, &dst[op->dstOffset], op->dstStride);
                gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
            }
        }
    }

#if _ALWAYS_UNBIND_GL_BUF
    if (bufBound)
    {
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE);
    }
#endif
    return err;
}

_ZGL afxError DpuUpdateBuffer(zglDpu* dpu, avxBuffer buf, afxByte const* src, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
#ifdef _USE_RW_SLOTS_ON_BUFFER_UPDATE
#ifdef _USE_IO_SLOTS_ON_BUFFER_UPDATE
    GLenum glTarget = GL_PIXEL_PACK_BUFFER;
#else
    GLenum glTarget = GL_COPY_WRITE_BUFFER;
#endif
#else
    GLenum glTarget = buf->glTarget;
#endif
    afxBool bufSynced = FALSE;
    afxBool bufBound = FALSE;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];

        /*
            If your project is crashing here, maybe it is because you are not waiting for completion before deallocating the source.
        */

        if (op->srcStride == op->dstStride)
        {
            // LINEAR VERSION
            afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));

            if (gl->NamedBufferSubData)
            {
                if (!bufSynced)
                {
                    DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE);
                    bufSynced = TRUE;
                }
                gl->NamedBufferSubData(buf->glHandle, op->dstOffset, bufRange, &src[op->srcOffset]); _ZglThrowErrorOccuried();
            }
            else
            {
                if (!bufBound)
                {
                    DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE);
                    bufBound = TRUE;
                    bufSynced = TRUE;
                }
                gl->BufferSubData(glTarget, op->dstOffset, bufRange, &src[op->srcOffset]); _ZglThrowErrorOccuried();
            }
        }
        else
        {
            // STRIDED VERSION
            afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));

            if (gl->MapNamedBufferRange)
            {
                if (!bufSynced)
                {
                    DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE);
                    bufSynced = TRUE;
                }

                afxByte* dstAt = gl->MapNamedBufferRange(buf->glHandle, op->dstOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

                if (!dstAt)
                {
                    AfxThrowError();
                    continue;
                }

                AfxStream2(op->rowCnt, &src[op->srcOffset], op->srcStride, dstAt, op->dstStride);
                gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
            }
            else
            {
                if (!bufBound)
                {
                    DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE);
                    bufBound = TRUE;
                    bufSynced = TRUE;
                }

                afxByte* dstAt = gl->MapBufferRange(glTarget, op->dstOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

                if (!dstAt)
                {
                    AfxThrowError();
                    continue;
                }

                AfxStream2(op->rowCnt, &src[op->srcOffset], op->srcStride, dstAt, op->dstStride);
                gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
            }
        }
    }

#if _ALWAYS_UNBIND_GL_BUF
    if (bufBound)
    {
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE);
    }
#endif
    return err;
}

_ZGL afxError _DpuDownloadBuffer(zglDpu* dpu, avxBuffer buf, afxStream out, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

#ifdef _USE_RW_SLOTS_ON_BUFFER_DOWNLOAD
#ifdef _USE_IO_SLOTS_ON_BUFFER_DOWNLOAD
    GLenum glTarget = GL_PIXEL_UNPACK_BUFFER;
#else
    GLenum glTarget = GL_COPY_READ_BUFFER;
#endif
#else
    GLenum glTarget = buf->glTarget;
#endif
    afxBool bufSynced = FALSE;
    afxBool bufBound = FALSE;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->srcStride, AvxGetBufferCapacity(buf, op->srcOffset));

        if (gl->MapNamedBufferRange)
        {
            if (!bufSynced)
            {
                DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE);
                bufSynced = TRUE;
            }

            afxByte const* srcAt = gl->MapNamedBufferRange(buf->glHandle, op->srcOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

            if (!srcAt)
            {
                AfxThrowError();
                continue;
            }

            if (op->dstStride != op->srcStride)
                AfxWriteStreamAt2(out, op->dstOffset, bufRange, op->dstStride, srcAt, op->srcStride);
            else
                AfxWriteStreamAt(out, op->dstOffset, bufRange, 0, srcAt);

            gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
        }
        else
        {
            if (!bufBound)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE);
                bufBound = TRUE;
                bufSynced = TRUE;
            }

            afxByte const* srcAt = gl->MapBufferRange(glTarget, op->srcOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

            if (!srcAt)
            {
                AfxThrowError();
                continue;
            }

            if (op->dstStride != op->srcStride)
                AfxWriteStreamAt2(out, op->dstOffset, bufRange, op->dstStride, srcAt, op->srcStride);
            else
                AfxWriteStreamAt(out, op->dstOffset, bufRange, 0, srcAt);

            gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
        }
    }

#if _ALWAYS_UNBIND_GL_BUF
    if (bufBound)
    {
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
    }
#endif
    return err;
}

_ZGL afxError _DpuUploadBuffer(zglDpu* dpu, avxBuffer buf, afxStream in, afxUnit opCnt, avxBufferIo const* ops)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

#ifdef _USE_RW_SLOTS_ON_BUFFER_UPLOAD
#ifdef _USE_IO_SLOTS_ON_BUFFER_UPLOAD
    GLenum glTarget = GL_PIXEL_PACK_BUFFER;
#else
    GLenum glTarget = GL_COPY_WRITE_BUFFER;
#endif
#else
    GLenum glTarget = buf->glTarget;
#endif
    afxBool bufSynced = FALSE;
    afxBool bufBound = FALSE;

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxBufferIo const* op = &ops[i];
        afxUnit bufRange = AFX_MIN(op->rowCnt * op->dstStride, AvxGetBufferCapacity(buf, op->dstOffset));
        
        if (gl->MapNamedBufferRange)
        {
            if (!bufSynced)
            {
                DpuBindAndSyncBuf(dpu, buf->glTarget, buf, FALSE); // sync
                bufSynced = TRUE;
            }

            afxByte* dstAt = gl->MapNamedBufferRange(buf->glHandle, op->dstOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

            if (!dstAt)
            {
                AfxThrowError();
                continue;
            }

            if (op->dstStride != op->srcStride)
                AfxReadStreamAt2(in, op->srcOffset, bufRange, op->dstStride, dstAt, op->srcStride);
            else
                AfxReadStreamAt(in, op->srcOffset, bufRange, 0, dstAt);

            gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
        }
        else
        {
            if (!bufBound)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
                bufBound = TRUE;
                bufSynced = TRUE;
            }

            afxByte* dstAt = gl->MapBufferRange(glTarget, op->dstOffset, bufRange, buf->glMapRangeAccess); _ZglThrowErrorOccuried();

            if (!dstAt)
            {
                AfxThrowError();
                continue;
            }

            if (op->dstStride != op->srcStride)
                AfxReadStreamAt2(in, op->srcOffset, bufRange, op->srcStride, dstAt, op->dstStride);
            else
                AfxReadStreamAt(in, op->srcOffset, bufRange, 0, dstAt);

            gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
            //gl->FlushMappedBufferRange(glTarget, op->dstOffset, op->range); _ZglThrowErrorOccuried();
        }
    }

#if _ALWAYS_UNBIND_GL_BUF
    if (bufBound)
    {
        DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
    }
#endif
    return err;
}

_ZGL afxError _DpuRemapBuf(zglDpu* dpu, avxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = { 0 };
    //AfxAssertObject(dexu, afxFcc_DEXU);
    glVmt const* gl = dpu->gl;

#ifdef _USE_RW_SLOTS_ON_BUFFER_MAPPING
    GLenum glTarget = NIL;

    if (buf->m.flags & avxBufferFlag_W)
    {
#ifdef _USE_IO_SLOTS_ON_BUFFER_MAPPING
        glTarget = GL_PIXEL_PACK_BUFFER;
#else
        glTarget = GL_COPY_WRITE_BUFFER;
#endif
    }
    else
    {
#ifdef _USE_IO_SLOTS_ON_BUFFER_MAPPING
        glTarget = GL_PIXEL_UNPACK_BUFFER;
#else
        glTarget = GL_COPY_READ_BUFFER;
#endif
    }
#else
    GLenum glTarget = buf->glTarget;
#endif

    if (range)
    {
        GLenum glAccess = NIL;
        AFX_ASSERT(!buf->m.storage[0].mapPtr);

#if 0 // disable to force usage of flags by glMapBufferRange()
        if (!offset && range == AvxGetBufferCapacity(buf, 0))
        {
            if (!(buf->m.flags & avxBufferFlag_R))
            {
                if (buf->m.flags & avxBufferFlag_W)
                    glAccess = GL_WRITE_ONLY;
            }
            else
            {
                if (buf->m.flags & avxBufferFlag_W)
                    glAccess = GL_READ_WRITE;
                else
                    glAccess = GL_READ_ONLY;
            }

            buf->m.bytemap = gl->MapBuffer(glTarget, glAccess); _ZglThrowErrorOccuried();
        }
        else
#endif
        {

            if (gl->MapNamedBufferRange)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
                buf->m.storage[0].mapPtr = gl->MapNamedBufferRange(buf->glHandle, offset, range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
            }
            else
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
                buf->m.storage[0].mapPtr = gl->MapBufferRange(glTarget, offset, range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
                DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif

            }
        }

        buf->m.storage[0].mapOffset = offset;
        buf->m.storage[0].mapRange = range;
        buf->m.storage[0].mapFlags = flags;
        AFX_ASSERT(buf->m.storage[0].mapPtr);

        //if (placeholder)
        *placeholder = buf->m.storage[0].mapPtr;
        AFX_ASSERT(*placeholder);
    }
    else
    {
        AFX_ASSERT(buf->m.storage[0].mapPtr);

        if (gl->UnmapNamedBuffer)
        {
            DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
            gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
        }
        else
        {
            DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
            gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
            DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
        }
        buf->m.storage[0].mapPtr = NIL;

        //gl->FlushMappedBufferRange(glTarget, subm->buf->glMappedOff, subm->buf->glMappedSiz); _ZglThrowErrorOccuried();
        buf->m.storage[0].mapOffset = 0;
        buf->m.storage[0].mapRange = 0;
        buf->m.storage[0].mapFlags = NIL;
    }
    //AFX_ASSERT(!AfxLoadAtom32(&buf->m.pendingRemap));

    return err;
}

#if 0
_ZGL afxError _BufRemapCb(avxBuffer buf, afxUnit offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->m.cap, offset, range);

    afxDrawSystem dsys = AvxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxDrawQueue dque;
    afxDrawBridge dexu;
    afxUnit portId = 0;
    AvxGetDrawBridges(dsys, portId, 1, &dexu);
    AvxGetDrawQueues(dexu, 0, 1, &dque);
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    afxBool queued = FALSE;

    if (!range) // to unmap
    {
        void* ptr = NIL;
        if (_AvxDqueRemapBuffers(dque, buf, 0, 0, 0, &ptr))
        {
            AfxThrowError();
        }
        else
        {
            queued = TRUE;
            afxBool wait = !!flags;

            if (wait && AvxWaitForEmptyDrawQueue(dque, AFX_TIMEOUT_INFINITE))
                AfxThrowError();
        }

        if (!queued)
            AfxThrowError();
    }
    else // to map
    {
        if (buf->m.mappedRange) AfxThrowError();
        else
        {
            void* ptr = NIL;
            if (_AvxDqueRemapBuffers(dque, buf, offset, range, flags, &ptr))
            {
                AfxThrowError();
            }
            else
            {
                queued = TRUE;

                if (AvxWaitForEmptyDrawQueue(dque, AFX_TIMEOUT_INFINITE))
                    AfxThrowError();

                AFX_ASSERT(ptr);
                AFX_ASSERT(buf->m.bytemap);
            }
            
            *placeholder = ptr;

            if (!queued)
                AfxThrowError();
        }
    }
    return err;
}
#endif

_ZGL afxError _BufDtorCb(avxBuffer buf)
{
    afxError err = { 0 };

    afxDrawSystem dsys = AvxGetBufferHost(buf);

    if (buf->m.storage[0].mapRange)
    {
        AvxUnmapBuffer(buf, TRUE);
        AFX_ASSERT(!buf->m.storage[0].mapRange);
    }

    if (buf->glHandle)
    {
        _ZglDsysEnqueueDeletion(dsys, 0, GL_BUFFER, (afxSize)buf->glHandle);
        buf->glHandle = 0;
    }

#if 0
    if (buf->glTexHandle)
    {
        _ZglDsysEnqueueDeletion(dsys, 0, GL_TEXTURE, (afxSize)buf->glTexHandle);
        buf->glTexHandle = 0;
    }
#endif
    if (_AVX_CLASS_CONFIG_BUF.dtor(buf))
        AfxThrowError();

    return err;
}

_ZGL afxError _BufCtorCb(avxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    avxBufferInfo const *spec = ((avxBufferInfo const *)args[1]) + invokeNo;

    if (_AVX_CLASS_CONFIG_BUF.ctor(buf, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }

    afxDrawSystem dsys = AvxGetBufferHost(buf);
    buf->bufUniqueId = ++dsys->bufUniqueId;

    buf->glHandle = NIL;
#if 0
    buf->glTexHandle = NIL;
#endif
    buf->glTarget = NIL;
    buf->updFlags = ZGL_UPD_FLAG_DEVICE_INST;

    avxBufferUsage usage = buf->m.usage;
    avxBufferFlags access = buf->m.flags;
    GLbitfield glGenAccess = NIL;
    GLbitfield glMapRangeAccess = NIL;
    GLbitfield glMapWholeAccess = NIL;

    if (usage & avxBufferUsage_STORAGE)
        buf->glTarget = GL_SHADER_STORAGE_BUFFER;
    else if (usage & avxBufferUsage_UNIFORM)
        buf->glTarget = GL_UNIFORM_BUFFER;
    else if (usage & avxBufferUsage_INDIRECT) // before vbo because baseInst can be passed in as vtx attr as a fallback.
        buf->glTarget = GL_DRAW_INDIRECT_BUFFER; // defer to opt for GL_DISPATCH_INDIRECT_BUFFER?
    else if (usage & avxBufferUsage_INDEX) // first in case, it is also a vertex buffer.
        buf->glTarget = GL_ELEMENT_ARRAY_BUFFER;
    else if (usage & avxBufferUsage_VERTEX)
        buf->glTarget = GL_ARRAY_BUFFER; // behind because any buffer can be bound as vertex source
    else if (usage & avxBufferUsage_QUERY)
        buf->glTarget = GL_QUERY_BUFFER;
    else if (usage & avxBufferUsage_TENSOR) // pull buffer or drag buffer
        buf->glTarget = GL_TEXTURE_BUFFER;
    else if (usage & avxBufferUsage_FETCH)
        buf->glTarget = GL_TEXTURE_BUFFER;
    else if (usage & avxBufferUsage_DOWNLOAD) // last because it can be combined with other types.
        buf->glTarget = GL_PIXEL_PACK_BUFFER;
    else if (usage & avxBufferUsage_UPLOAD) // last because it can be combined with other types.
        buf->glTarget = GL_PIXEL_UNPACK_BUFFER;
    else if (usage & avxBufferUsage_FEEDBACK)
        buf->glTarget = GL_TRANSFORM_FEEDBACK_BUFFER;
    else
        AfxThrowError();

    GLint glIntFmt;
    GLenum glLayout;
    GLenum glType;
    ZglToGlFormat2(buf->m.fmt, &glIntFmt, &glLayout, &glType);
    buf->glTexIntFmt = glIntFmt; // tex buf needs

    if ((access & avxBufferFlag_R) == avxBufferFlag_R)
    {
        /*
            GL_MAP_READ_BIT
            The data store may be mapped by the client for read access and a pointer 
            in the client's address space obtained that may be read from.
        */
        glGenAccess |= GL_MAP_READ_BIT;
        glMapRangeAccess |= GL_MAP_READ_BIT;
    }

    if ((access & avxBufferFlag_W) == avxBufferFlag_W)
    {
        /*
            GL_DYNAMIC_STORAGE_BIT
            The contents of the data store may be updated after creation through calls to glBufferSubData. 
            If this bit is not set, the buffer content may not be directly updated by the client. 
            The data argument may be used to specify the initial content of the buffer's data store regardless 
            of the presence of the GL_DYNAMIC_STORAGE_BIT. Regardless of the presence of this bit, 
            buffers may always be updated with server-side calls such as glCopyBufferSubData and glClearBufferSubData.

            GL_MAP_WRITE_BIT
            The data store may be mapped by the client for write access and a pointer in the client's 
            address space obtained that may be written through.
        */
        glGenAccess |= GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT;
        glMapRangeAccess |= GL_MAP_WRITE_BIT;
    }

    if ((access & avxBufferFlag_H) == avxBufferFlag_H)
    {
        /*
            GL_CLIENT_STORAGE_BIT
            When all other criteria for the buffer storage allocation are met, 
            this bit may be used by an implementation to determine whether to use storage that is local 
            to the server or to the client to serve as the backing store for the buffer.
        */
        glGenAccess |= GL_CLIENT_STORAGE_BIT;
    }

    if (access & avxBufferFlag_RW) // only if host accesible/visible.
    {
        if ((access & avxBufferFlag_W) == avxBufferFlag_W) // only if writeable
        {
            if ((access & avxBufferFlag_R) != avxBufferFlag_R) // only if write-only
            {
                /*
                    glMapBufferRange()
                    GL_INVALID_OPERATION is generated for any of the following conditions:
                        Neither GL_MAP_READ_BIT nor GL_MAP_WRITE_BIT is set.
                        GL_MAP_READ_BIT is set and any of GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT or GL_MAP_UNSYNCHRONIZED_BIT is set.
                        GL_MAP_FLUSH_EXPLICIT_BIT is set and GL_MAP_WRITE_BIT is not set.
                        Any of GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_PERSISTENT_BIT, or GL_MAP_COHERENT_BIT are set, but the same bit is not included in the buffer's storage flags.
                */

                glMapWholeAccess = GL_WRITE_ONLY;

                /*
                    GL_MAP_INVALIDATE_RANGE_BIT indicates that the previous contents of the specified range may be discarded.
                    Data within this range are undefined with the exception of subsequently written data.
                    No GL error is generated if subsequent GL operations access unwritten data,
                    but the result is undefined and system errors (possibly including program termination) may occur.
                    This flag may not be used in combination with GL_MAP_READ_BIT.

                    GL_MAP_INVALIDATE_BUFFER_BIT indicates that the previous contents of the entire buffer may be discarded.
                    Data within the entire buffer are undefined with the exception of subsequently written data.
                    No GL error is generated if subsequent GL operations access unwritten data,
                    but the result is undefined and system errors (possibly including program termination) may occur.
                    This flag may not be used in combination with GL_MAP_READ_BIT.
                */
                glMapRangeAccess |= GL_MAP_INVALIDATE_RANGE_BIT;

            }
            else
            {
                glMapWholeAccess = GL_READ_WRITE;
            }
        }
        else
        {
            glMapWholeAccess = GL_READ_ONLY;
        }

        if ((access & avxBufferFlag_X) == avxBufferFlag_X) // only if executable/persistent.
        {
            glGenAccess |= GL_MAP_PERSISTENT_BIT;
            glMapRangeAccess |= GL_MAP_PERSISTENT_BIT;
        }

        if ((access & avxBufferFlag_C) == avxBufferFlag_C)
        {
            glGenAccess |= GL_MAP_COHERENT_BIT;
            glMapRangeAccess |= GL_MAP_COHERENT_BIT;
        }
        else
        {
            // If mapping is write-only
            if ((access & avxBufferFlag_RW) == avxBufferFlag_W)
            {
                /*
                    GL_MAP_FLUSH_EXPLICIT_BIT indicates that one or more discrete subranges of the mapping may be modified.
                    When this flag is set, modifications to each subrange must be explicitly flushed by calling glFlushMappedBufferRange.
                    No GL error is set if a subrange of the mapping is modified and not flushed,
                    but data within the corresponding subrange of the buffer are undefined.
                    This flag may only be used in conjunction with GL_MAP_WRITE_BIT. When this option is selected,
                    flushing is strictly limited to regions that are explicitly indicated with calls to glFlushMappedBufferRange prior to unmap;
                    if this option is not selected glUnmapBuffer will automatically flush the entire mapped range when called.

                    If a buffer range is mapped with both GL_MAP_PERSISTENT_BIT and GL_MAP_FLUSH_EXPLICIT_BIT set,
                    then these commands may be called to ensure that data written by the client into the flushed region
                    becomes visible to the server. Data written to a coherent store will always become visible to the
                    server after an unspecified period of time.
                */
                glMapRangeAccess |= GL_MAP_FLUSH_EXPLICIT_BIT;
            }
        }

        /*
            GL_MAP_UNSYNCHRONIZED_BIT indicates that the GL should not attempt to synchronize
            pending operations on the buffer prior to returning from glMapBufferRange or glMapNamedBufferRange.
            No GL error is generated if pending operations which source or modify the buffer overlap the mapped region,
            but the result of such previous and any subsequent operations is undefined.
        */
        glMapRangeAccess |= GL_MAP_UNSYNCHRONIZED_BIT;
        // TODO force to be always unsynchronized like Vulkan.
    }

    /*
        NOTE:
        Most drivers do treat standard write mappings as coherent, unless you 
        specifically ask for non-coherent behavior (i.e., GL_MAP_FLUSH_EXPLICIT_BIT).

        On desktop GPUs, glMapBufferRange is often coherent by default, especially with persistent mapping.
        On mobile GPUs (like OpenGL ES on Android), coherency can vary, and flushing may be more important.
        If you use GL_MAP_PERSISTENT_BIT and GL_MAP_COHERENT_BIT (OpenGL 4.4+), you explicitly ask for coherence.

        OpenGL vs Vulkan Memory Mapping: Overview
        Concept	                        OpenGL	                                    Vulkan
        Buffer mapping	                glMapBufferRange()	                        vkMapMemory()
        Coherent memory	                GL_MAP_COHERENT_BIT                         (OpenGL 4.4+) or no flush needed	VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        Non-coherent memory	            GL_MAP_FLUSH_EXPLICIT_BIT	                No HOST_COHERENT — requires vkFlushMappedMemoryRanges()
        Persistent mapping	            GL_MAP_PERSISTENT_BIT	                    Long-lived vkMapMemory() (no unmap between frames)
        Unsynchronized access	        GL_MAP_UNSYNCHRONIZED_BIT	                Vulkan always gives you unsynchronized access — you manage all sync
        Invalidate buffer before read	GL_MAP_INVALIDATE_RANGE_BIT/...BUFFER_BIT   vkInvalidateMappedMemoryRanges()
        Explicit flush	                glFlushMappedBufferRange()	                vkFlushMappedMemoryRanges()

        GL_MAP_UNSYNCHRONIZED_BIT -> Vulkan-style access by default
        OpenGL uses this to tell the driver we will take care of synchronization.
        In Vulkan, all access is unsynchronized by default - you must use barriers, fences, and memory dependencies to manage visibility and hazard prevention.
        So in a way, Vulkan operates as if GL_MAP_UNSYNCHRONIZED_BIT is always enabled - it's up to you to keep things safe.

        GL_MAP_PERSISTENT_BIT -> Persistent vkMapMemory()
        Vulkan encourages persistent mapping - you're expected to map once and reuse it.
        No need to unmap each frame, similar to OpenGL persistent buffers.
        But Vulkan doesn't need a flag for this - you just map once and manage everything.

        GL_MAP_FLUSH_EXPLICIT_BIT -> Vulkan without HOST_COHERENT_BIT
        The app is responsible for flushing memory ranges using vkFlushMappedMemoryRanges() (CPU -> GPU)
        Also responsible for invalidating memory using vkInvalidateMappedMemoryRanges() (GPU -> CPU)
        This matches non-coherent memory in Vulkan.

        GL_MAP_COHERENT_BIT -> VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        In Vulkan, if the memory has the HOST_COHERENT_BIT, then:
        CPU writes are automatically visible to the GPU (no vkFlushMappedMemoryRanges() needed).
        GPU writes are automatically visible to the CPU (no vkInvalidateMappedMemoryRanges() needed).
        This mimics the behavior of coherent buffer mappings in OpenGL.

        We are open to hear about what cursed use case could have 'MAP_WRITE_BIT | MAP_PERSISTENT_BIT' lacking both 'MAP_COHERENT_BIT' and 'MAP_FLUSH_EXPLICIT_BIT'.
    */

    /*
        GL_MAP_READ_BIT
        GL_MAP_WRITE_BIT
        GL_MAP_INVALIDATE_RANGE_BIT
        GL_MAP_INVALIDATE_BUFFER_BIT
        GL_MAP_FLUSH_EXPLICIT_BIT
        GL_MAP_UNSYNCHRONIZED_BIT
    */

    buf->glGenAccess = glGenAccess;
    buf->glMapRangeAccess = glMapRangeAccess;
    buf->glMapWholeAccess = glMapWholeAccess;

    //buf->m.remap = _BufRemapCb;

    if ((access & avxBufferFlag_X) &&
        (!(glMapRangeAccess & GL_MAP_PERSISTENT_BIT) || !(glGenAccess & GL_MAP_PERSISTENT_BIT)))
        AfxThrowError();

    if ((access & avxBufferFlag_C) &&
        (!(glMapRangeAccess & GL_MAP_COHERENT_BIT) || !(glGenAccess & GL_MAP_COHERENT_BIT)))
        AfxThrowError();

    if (err && _AVX_CLASS_CONFIG_BUF.dtor(buf))
        AfxThrowError();

    // Indirect Command Buffers (e.g., via glMultiDrawIndirect) are not reliably shareable because they are attached to execution context-specific stuff.

    return err;
}

//////////////////////////////////////////////////////////

_ZGL afxError _DpuWork_SyncMaps(zglDpu* dpu, _avxIoReqPacket* subm)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    if (subm->SyncMaps.flushCnt)
    {
        afxUnit flushCnt = subm->SyncMaps.flushCnt;
        for (afxUnit i = 0; i < flushCnt; i++)
        {
            avxBufferedMap const* map = &subm->SyncMaps.ops[subm->SyncMaps.baseFlushIdx + i];

            avxBuffer buf = map->buf;
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
#ifdef _USE_RW_SLOTS_ON_BUFFER_MAPPING
            GLenum glTarget = (buf->m.flags & avxBufferFlag_W) ? GL_COPY_WRITE_BUFFER : GL_COPY_READ_BUFFER;
#else
            GLenum glTarget = buf->glTarget;
#endif
            if (gl->FlushMappedNamedBufferRange)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
                gl->FlushMappedNamedBufferRange(buf->glHandle, map->offset, map->range); _ZglThrowErrorOccuried();
            }
            else
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
                gl->FlushMappedBufferRange(glTarget, map->offset, map->range); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
                // unbind and dispose buffer.
                DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
            }
            AfxDisposeObjects(1, &buf);
        }
    }

    if (subm->SyncMaps.fetchCnt)
    {
        afxUnit fetchCnt = subm->SyncMaps.fetchCnt;
        for (afxUnit i = 0; i < fetchCnt; i++)
        {
            avxBufferedMap const* map = &subm->SyncMaps.ops[subm->SyncMaps.baseFetchIdx + i];

            avxBuffer buf = map->buf;
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
#ifdef _USE_RW_SLOTS_ON_BUFFER_MAPPING
            GLenum glTarget = (buf->m.flags & avxBufferFlag_W) ? GL_COPY_WRITE_BUFFER : GL_COPY_READ_BUFFER;
#else
            GLenum glTarget = buf->glTarget;
#endif
            if (gl->InvalidateBufferSubData)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind

                AFX_ASSERT(gl->InvalidateBufferSubData); // GL Core 4.3
                gl->InvalidateBufferSubData(glTarget, map->offset, map->range); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
                // unbind and dispose buffer.
                DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
            }
            AfxDisposeObjects(1, &buf);
        }
    }

    return err;
}

_ZGL afxError _DpuWork_Remap(zglDpu* dpu, _avxIoReqPacket* subm)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    if (subm->Remap.unmapCnt)
    {
        afxUnit unmapCnt = subm->Remap.unmapCnt;
        for (afxUnit i = 0; i < unmapCnt; i++)
        {
            _avxBufferRemapping const* unmap = &subm->Remap.unmapOps[subm->Remap.firstUnmapIdx + i];
            avxBuffer buf = unmap->buf;
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
#ifdef _USE_RW_SLOTS_ON_BUFFER_MAPPING
            GLenum glTarget = (buf->m.flags & avxBufferFlag_W) ? GL_COPY_WRITE_BUFFER : GL_COPY_READ_BUFFER;
#else
            GLenum glTarget = buf->glTarget;
#endif
            if (gl->UnmapNamedBuffer)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
                gl->UnmapNamedBuffer(buf->glHandle); _ZglThrowErrorOccuried();
            }
            else
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind
                gl->UnmapBuffer(glTarget); _ZglThrowErrorOccuried();
#if _ALWAYS_UNBIND_GL_BUF
                // unbind and dispose buffer.
                DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
            }

            buf->m.storage[0].mapOffset = 0;
            buf->m.storage[0].mapRange = 0;
            buf->m.storage[0].mapFlags = NIL;
            AFX_ASSERT(buf->m.storage[0].mapPtr);
            buf->m.storage[0].mapPtr = NIL;

            AFX_ASSERT(buf->m.storage[0].mapRefCnt == 1);
            --buf->m.storage[0].mapRefCnt;

            AfxDecAtom32(&buf->m.storage[0].pendingRemap);
            AfxDisposeObjects(1, &buf);
        }
    }

    if (subm->Remap.mapCnt)
    {
        afxUnit mapCnt = subm->Remap.mapCnt;
        for (afxUnit i = 0; i < mapCnt; i++)
        {
            _avxBufferRemapping const* map = &subm->Remap.mapOps[subm->Remap.firstMapIdx + i];

            avxBuffer buf = map->buf;
#ifdef _USE_RW_SLOTS_ON_BUFFER_MAPPING
            GLenum glTarget = (buf->m.flags & avxBufferFlag_W) ? GL_COPY_WRITE_BUFFER : GL_COPY_READ_BUFFER;
#else
            GLenum glTarget = buf->glTarget;
#endif
            void* ptr = NIL;

            if (gl->MapNamedBufferRange)
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, FALSE); // sync
                ptr = gl->MapNamedBufferRange(buf->glHandle, map->offset, map->range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
            }
            else
            {
                DpuBindAndSyncBuf(dpu, glTarget, buf, TRUE); // bind

#if 0 // disable to force usage of flags by glMapBufferRange()
                if (!map->offset && map->range == AvxGetBufferCapacity(buf, 0))
                {
                    GLenum glAccess = NIL; // glMapBuffer() uses different access flags;

                    if ((buf->m.flags & avxBufferFlag_RW) == avxBufferFlag_RW) glAccess = GL_READ_WRITE;
                    else if ((buf->m.flags & avxBufferFlag_W) == avxBufferFlag_W) glAccess = GL_WRITE_ONLY;
                    else if ((buf->m.flags & avxBufferFlag_R) == avxBufferFlag_R) glAccess = GL_READ_ONLY;
                    else
                    {
                        AfxThrowError();
                        continue;
                    }
                    buf->m.bytemap = gl->MapBuffer(glTarget, glAccess); _ZglThrowErrorOccuried();
                }
                else
#endif
                {
                    ptr = gl->MapBufferRange(glTarget, map->offset, map->range, buf->glMapRangeAccess); _ZglThrowErrorOccuried();
                }
#if _ALWAYS_UNBIND_GL_BUF
                // unbind and dispose buffer.
                DpuBindAndSyncBuf(dpu, glTarget, NIL, TRUE); // unbind
#endif
            }

            buf->m.storage[0].mapPtr = ptr;
            AFX_ASSERT(buf->m.storage[0].mapPtr);
            buf->m.storage[0].mapOffset = map->offset;
            buf->m.storage[0].mapRange = map->range;
            buf->m.storage[0].mapFlags = map->flags;
            
            AFX_ASSERT(map->placeholder);
            *map->placeholder = buf->m.storage[0].mapPtr;
            AFX_ASSERT(*map->placeholder);

            AFX_ASSERT(buf->m.storage[0].mapRefCnt == 0);
            ++buf->m.storage[0].mapRefCnt;

            AfxDecAtom32(&buf->m.storage[0].pendingRemap);
            AfxDisposeObjects(1, &buf);
        }
    }

    return err;
}
