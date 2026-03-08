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

#include "zglCommands.h"
#include "zglObjects.h"
#include "zglUtils.h"

#define _ZGL_UNBIND_ANY_VIN_AT_END 1
#define _ZGL_UNBIND_ANY_PIP_AT_END 1
#define _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_END 1
#define _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD 1
#define _ZGL_UNBIND_ANY_INDIRECT_DISPATCH_BUF_AT_END 1
#define _ZGL_UNBIND_ANY_INDIRECT_DISPATCH_BUF_AT_CMD 1
#define _ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_END 1
#define _ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD 1
//#define _FLUSH_ON_IOREQ_EXECUTION TRUE
//#define _FINISH_AFTER_ALL_IOREQ_EXECUTIONS TRUE
//#define _YIELD_AFTER_ALL_IOREQ_EXECUTIONS TRUE
#define _YIELD_WITH_SLEEP_AFTER_ALL_IOREQ_EXECUTIONS TRUE
#define _YIELD_WITH_SLEEP_AFTER_ALL_IOREQ_EXECUTIONS_TIMEOUT 1
#define _FLUSH_BEFORE_YIELD_AFTER_ALL_IOREQ_EXECUTIONS TRUE
#define _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC TRUE
#define _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC_TIMEOUT 1
#define _USE_REAL_GL_FENCES 1
//#define _USE_SINGLE_GL_FENCES 1

#define COHERENT_PUSHABLES

static const GLenum idxSizGl[] =
{
    0,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    0,
    GL_UNSIGNED_INT
};

_ZGL void DpuMarkDebugMilestone(zglDpu* dpu, avxColor const color, afxString const* label)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    gl->DebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, AvxGetColorRgba8(color), GL_DEBUG_SEVERITY_NOTIFICATION, label->len, label->start); _ZglThrowErrorOccuried();
}

_ZGL void DpuCommenceDebugScope(zglDpu* dpu, avxColor const color, afxString const* label)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    gl->PushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, AvxGetColorRgba8(color), label->len, label->start); _ZglThrowErrorOccuried();
    ++dpu->dbgScopeStackTop;
}

_ZGL void DpuConcludeDebugScope(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    
    if (!dpu->dbgScopeStackTop) AfxThrowError();
    else
    {
        gl->PopDebugGroup(); _ZglThrowErrorOccuried();
        --dpu->dbgScopeStackTop;
    }
}

// RENDERING SCOPE

_ZGL void _DpuPlacePipelineBarrier(zglDpu* dpu, avxBusStage dstStage, avxBusAccess dstAcc)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    GLbitfield glMask = NIL;

    // Because GL pretend to be synchronous in execution model, it only expects a destination stage and access.
    // This design makes sense for OpenGL, where GL expects barriers to declare where data will be consumed, not where it was produced.
    // OpenGL only really cares about what's about to be read or written, not what it came from.

#if 0
    if (dstStage & avxBusStage_VTX_ATTR)
    {
        glMask |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
    }
    if (dstStage & avxBusStage_VTX_IDX)
    {
        glMask |= GL_ELEMENT_ARRAY_BARRIER_BIT;
    }
    if (dstStage & avxBusStage_DRAW_IND)
    {
        glMask |= GL_COMMAND_BARRIER_BIT;
    }
#endif

    avxBusAccess access = dstAcc;
    
    if (access & avxBusAccess_VERTEX_R)
    {
        glMask |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
        // If set, vertex data sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier.
        // The set of buffer objects affected by this bit is derived from the buffer object bindings used for generic vertex attributes derived from the GL_VERTEX_ATTRIB_ARRAY_BUFFER bindings.
    }
    
    if (access & avxBusAccess_INDEX_R)
    {
        glMask |= GL_ELEMENT_ARRAY_BARRIER_BIT;
        // If set, vertex array indices sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier.
        // The buffer objects affected by this bit are derived from the GL_ELEMENT_ARRAY_BUFFER binding.
    }
    
    if (access & avxBusAccess_UNIFORM_R)
    {
        glMask |= GL_UNIFORM_BARRIER_BIT;
        // Shader uniforms sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier.
    }
    
    if (access & avxBusAccess_SHADER_R)
    {
        glMask |= GL_TEXTURE_FETCH_BARRIER_BIT;
        // Texture fetches from shaders, including fetches from buffer object memory via buffer textures, after the barrier will reflect data written by shaders prior to the barrier.
    }
    
    if (access & avxBusAccess_SHADER_W)
    {
        glMask |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
        // Memory accesses using shader image load, store, and atomic built - in functions issued after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, image stores and atomics issued after the barrier will not execute until all memory accesses(e.g., loads, stores, texture fetches, vertex fetches) initiated prior to the barrier complete.
    }
    
    if (access & avxBusAccess_INDIRECT_R)
    {
        glMask |= GL_COMMAND_BARRIER_BIT;
        // Command data sourced from buffer objects by Draw*Indirect commands after the barrier will reflect data written by shaders prior to the barrier.
        // The buffer objects affected by this bit are derived from the GL_DRAW_INDIRECT_BUFFER binding.
    }
    
    if (access & (avxBusAccess_TRANSFER))
    {
        glMask |= GL_PIXEL_BUFFER_BARRIER_BIT;
        // Reads and writes of buffer objects via the GL_PIXEL_PACK_BUFFER and GL_PIXEL_UNPACK_BUFFER bindings(via glReadPixels, glTexSubImage1D, etc.) after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, buffer object writes issued after the barrier will wait on the completion of all shader writes initiated prior to the barrier.
    }
    
    if (access & (avxBusAccess_TRANSFER))
    {
        glMask |= GL_TEXTURE_UPDATE_BARRIER_BIT;
        // Writes to a texture via glTex(Sub)Image*, glCopyTex(Sub)Image*, glCompressedTex(Sub)Image*, and reads via glGetTexImage after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, texture writes from these commands issued after the barrier will not execute until all shader writes initiated prior to the barrier complete.
    }
    
    if (access & (avxBusAccess_TRANSFER))
    {
        glMask |= GL_BUFFER_UPDATE_BARRIER_BIT;
        // Reads or writes via glBufferSubData, glCopyBufferSubData, or glGetBufferSubData, or to buffer object memory mapped by glMapBuffer or glMapBufferRange after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, writes via these commands issued after the barrier will wait on the completion of any shader writes to the same memory initiated prior to the barrier.
    }
    
    if (access & (avxBusAccess_TRANSFER))
    {
        glMask |= GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT;
        // Access by the client to persistent mapped regions of buffer objects will reflect data written by shaders prior to the barrier.
        // Note that this may cause additional synchronization operations.
    }
    
    if (access & (avxBusAccess_COLOR_OUT | avxBusAccess_DEPTH_OUT))
    {
        glMask |= GL_FRAMEBUFFER_BARRIER_BIT;
        // Reads and writes via framebuffer object attachments after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, framebuffer writes issued after the barrier will wait on the completion of all shader writes issued prior to the barrier.
    }
    
    if (access & avxBusAccess_SHADER_W)
    {
        glMask |= GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
        // Writes via transform feedback bindings after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, transform feedback writes issued after the barrier will wait on the completion of all shader writes issued prior to the barrier.
    }
    
    if (access & avxBusAccess_SHADER_W)
    {
        glMask |= GL_ATOMIC_COUNTER_BARRIER_BIT;
        // Accesses to atomic counters after the barrier will reflect writes prior to the barrier.
    }

    if (access & avxBusAccess_SHADER_W)
    {
        glMask |= GL_SHADER_STORAGE_BARRIER_BIT;
        // Accesses to shader storage blocks after the barrier will reflect writes prior to the barrier.
    }

    if (access & avxBusAccess_SHADER_W)
    {
        glMask |= GL_QUERY_BUFFER_BARRIER_BIT;
        // Writes of buffer objects via the GL_QUERY_BUFFER binding after the barrier will reflect data written by shaders prior to the barrier.
        // Additionally, buffer object writes issued after the barrier will wait on the completion of all shader writes initiated prior to the barrier.
    }

    /*
        glMemoryBarrier defines a barrier ordering the memory transactions issued prior to the command relative to those issued after the barrier. 
        For the purposes of this ordering, memory transactions performed by shaders are considered to be issued by the rendering command that triggered the execution of the shader. 
        barriers is a bitfield indicating the set of operations that are synchronized with shader stores; the bits used in barriers are as follows:

        GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
        If set, vertex data sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier. 
        The set of buffer objects affected by this bit is derived from the buffer object bindings used for generic vertex attributes derived from the GL_VERTEX_ATTRIB_ARRAY_BUFFER bindings.

        GL_ELEMENT_ARRAY_BARRIER_BIT
        If set, vertex array indices sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier. 
        The buffer objects affected by this bit are derived from the GL_ELEMENT_ARRAY_BUFFER binding.

        GL_UNIFORM_BARRIER_BIT
        Shader uniforms sourced from buffer objects after the barrier will reflect data written by shaders prior to the barrier.

        GL_TEXTURE_FETCH_BARRIER_BIT
        Texture fetches from shaders, including fetches from buffer object memory via buffer textures, after the barrier will reflect data written by shaders prior to the barrier.

        GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
        Memory accesses using shader image load, store, and atomic built-in functions issued after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, image stores and atomics issued after the barrier will not execute until all memory accesses (e.g., loads, stores, texture fetches, vertex fetches) initiated prior to the barrier complete.

        GL_COMMAND_BARRIER_BIT
        Command data sourced from buffer objects by Draw*Indirect commands after the barrier will reflect data written by shaders prior to the barrier. 
        The buffer objects affected by this bit are derived from the GL_DRAW_INDIRECT_BUFFER binding.

        GL_PIXEL_BUFFER_BARRIER_BIT
        Reads and writes of buffer objects via the GL_PIXEL_PACK_BUFFER and GL_PIXEL_UNPACK_BUFFER bindings (via glReadPixels, glTexSubImage1D, etc.) after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, buffer object writes issued after the barrier will wait on the completion of all shader writes initiated prior to the barrier.

        GL_TEXTURE_UPDATE_BARRIER_BIT
        Writes to a texture via glTex(Sub)Image*, glCopyTex(Sub)Image*, glCompressedTex(Sub)Image*, and reads via glGetTexImage after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, texture writes from these commands issued after the barrier will not execute until all shader writes initiated prior to the barrier complete.

        GL_BUFFER_UPDATE_BARRIER_BIT
        Reads or writes via glBufferSubData, glCopyBufferSubData, or glGetBufferSubData, or to buffer object memory mapped by glMapBuffer or glMapBufferRange after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, writes via these commands issued after the barrier will wait on the completion of any shader writes to the same memory initiated prior to the barrier.

        GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
        Access by the client to persistent mapped regions of buffer objects will reflect data written by shaders prior to the barrier. 
        Note that this may cause additional synchronization operations.

        GL_FRAMEBUFFER_BARRIER_BIT
        Reads and writes via framebuffer object attachments after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, framebuffer writes issued after the barrier will wait on the completion of all shader writes issued prior to the barrier.

        GL_TRANSFORM_FEEDBACK_BARRIER_BIT
        Writes via transform feedback bindings after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, transform feedback writes issued after the barrier will wait on the completion of all shader writes issued prior to the barrier.

        GL_ATOMIC_COUNTER_BARRIER_BIT
        Accesses to atomic counters after the barrier will reflect writes prior to the barrier.

        GL_SHADER_STORAGE_BARRIER_BIT
        Accesses to shader storage blocks after the barrier will reflect writes prior to the barrier.

        GL_QUERY_BUFFER_BARRIER_BIT
        Writes of buffer objects via the GL_QUERY_BUFFER binding after the barrier will reflect data written by shaders prior to the barrier. 
        Additionally, buffer object writes issued after the barrier will wait on the completion of all shader writes initiated prior to the barrier.

        If barriers is GL_ALL_BARRIER_BITS, shader memory accesses will be synchronized relative to all the operations described above.

        // -----
        glMemoryBarrierByRegion behaves as described above for glMemoryBarrier, with two differences:

        First, it narrows the region under consideration so that only reads and writes of prior fragment shaders that are invoked for a smaller region of the framebuffer will be completed/reflected prior to subsequent reads and writes of following fragment shaders. 
        The size of the region is implementation-dependent and may be as small as one framebuffer pixel.

        Second, it only applies to memory transactions that may be read by or written by a fragment shader. 
        Therefore, only the barrier bits

        GL_ATOMIC_COUNTER_BARRIER_BIT

        GL_FRAMEBUFFER_BARRIER_BIT

        GL_SHADER_IMAGE_ACCESS_BARRIER_BIT

        GL_SHADER_STORAGE_BARRIER_BIT

        GL_TEXTURE_FETCH_BARRIER_BIT

        GL_UNIFORM_BARRIER_BIT

        are supported.

        When barriers is GL_ALL_BARRIER_BITS, shader memory accesses will be synchronized relative to all these barrier bits, 
        but not to other barrier bits specific to glMemoryBarrier. This implies that reads and writes for scatter/gather-like algorithms may or may not be completed/reflected after a glMemoryBarrierByRegion command. 
        However, for uses such as deferred shading, where a linked list of visible surfaces with the head at a framebuffer address may be constructed, and the entirety of the list is only dependent on previous executions at that framebuffer address, glMemoryBarrierByRegion may be significantly more efficient than glMemoryBarrier.
    */

    GLbitfield flags = NIL;
    gl->MemoryBarrier(glMask ? glMask : GL_ALL_BARRIER_BITS);
}

_ZGL void DpuDraw(zglDpu* dpu, avxDrawIndirect const* data)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    afxUnit cnt;

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    //if (dpu->nextVtxInStreamUpdMask || dpu->nextVtxInAttribUpdMask || dpu->flushIbb)
        //_ZglFlushVertexInputStateChanges(dpu, FALSE);

    //AFX_ASSERT(!dpu->nextVinBindingsMask);
    //AFX_ASSERT(!dpu->nextVtxInAttribUpdMask);
    //AFX_ASSERT(!dpu->flushIbb);

    AFX_ASSERT(dpu->activePip);
    afxUnit32 vtxCnt = data->vtxCnt;
    AFX_ASSERT(vtxCnt);
    //AFX_ASSERT(cmd->instCnt);
    GLenum top = AfxToGlTopology(dpu->primTop);

    afxUnit firstInst = data->baseInst;
    afxUnit firstVtx = data->baseVtx;
    afxUnit instCnt = data->instCnt;

    AFX_ASSERT(!dpu->boundDrawIndBuf);

#if 0//FORCE_GL_GENERIC_FUNCS
    AFX_ASSERT(gl->DrawArraysInstancedBaseInstance);
    gl->DrawArraysInstancedBaseInstance(top, firstVtx, vtxCnt, instCnt, firstInst); _ZglThrowErrorOccuried();
#else
#if 0 // not a GL 1.1+ function
    if (0 == instCnt)
    {
        gl->DrawArrays(top, firstVtx, vtxCnt); _ZglThrowErrorOccuried();
    }
    else
#endif
    {
#if 0
        if (firstInst)
#else
        if (firstInst > 1)
#endif
        {
            AFX_ASSERT(gl->DrawArraysInstancedBaseInstance);
            gl->DrawArraysInstancedBaseInstance(top, firstVtx, vtxCnt, instCnt, firstInst); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->DrawArraysInstanced(top, firstVtx, vtxCnt, instCnt); _ZglThrowErrorOccuried();
        }
    }
#endif
    dpu->m.numOfFedVertices += vtxCnt;
    dpu->m.numOfFedInstances += instCnt;

    //AfxReportMessage("Geometry drawn. vtxCnt %u, instCnt %u, firstVtx %u, firstInst %u", vtxCnt, instCnt, firstVtx, firstInst);
}

_ZGL void DpuDrawIndexed(zglDpu* dpu, avxDrawIndexedIndirect const* data)
{
    /*
        When the command is executed, primitives are assembled using the current primitive topology and indexCount vertices whose indices are retrieved from the index buffer.
        The index buffer is treated as an array of tightly packed unsigned integers of size defined by the vkCmdBindIndexStream::indexType parameter with which the buffer was bound.

        The first vertex index is at an offset of firstIndex × indexSize + offset within the bound index buffer, where offset is the offset specified by CmdBindIndexStream and indexSize is the byte size of the type specified by indexType.
        Subsequent index values are retrieved from consecutive locations in the index buffer.
        Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the indexType is VK_INDEX_TYPE_UINT8_EXT or VK_INDEX_TYPE_UINT16) and have vertexOffset added to them, before being supplied as the vertexIndex value.

        The primitives are drawn instanceCount times with instanceIndex starting with firstInstance and increasing sequentially for each instance.
        The assembled primitives execute the bound graphics pipeline.
    */

    afxError err = { 0 };

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    afxUnit idxCnt = data->idxCnt;

    avxVertexInput vin = dpu->activeVin;
    afxUnit vaoHandleIdx = dpu->dpuIterIdx % _ZGL_VAO_SWAPS;
    zglVertexInputState* bindings = &vin->perDpu[dpu->m.exuIdx][vaoHandleIdx].bindings;
    afxSize idxBufBaseOff = bindings->idxSrcOff;
    afxSize idxSiz = bindings->idxSrcSiz;
#if !0 // DEBUG
    avxBuffer idxSrcBuf = bindings->idxSrcBuf;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &idxSrcBuf);
    afxSize idxSrcBufSiz = AvxGetBufferCapacity(idxSrcBuf, 0);
    afxSize idxSrcRange = bindings->idxSrcRange;
    // Should be a NOP, not be an error. But we throw error here anyway to help debugging.
    AFX_ASSERT(idxCnt);
    AFX_ASSERT_RANGE(idxSrcBufSiz, idxBufBaseOff, idxSiz * idxCnt);
    AFX_ASSERT_RANGE(idxSrcRange, 0, idxSiz * idxCnt);
#endif

    GLint vtxOff2 = data->vtxOffset;
    afxUnit32 firstIdx = data->baseIdx;
    afxUnit32 instCnt = data->instCnt;
    afxUnit32 firstInst = data->baseInst;
    afxSize dataOff = (idxBufBaseOff + (idxSiz * firstIdx));

    GLenum top = AfxToGlTopology(dpu->primTop);

    glVmt const* gl = dpu->gl;

    AFX_ASSERT(!dpu->boundDrawIndBuf);

    // If there any error occuried by calling the Draw functions refering to NULL pointers,
    // some previous operations probably have unbind the IBO or the VAO.

#if 0//FORCE_GL_GENERIC_FUNCS
    AFX_ASSERT(gl->DrawElementsInstancedBaseVertexBaseInstance);
    gl->DrawElementsInstancedBaseVertexBaseInstance(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff, instCnt, vtxOff2, firstInst); _ZglThrowErrorOccuried();
#else
#if 0
    if (instCnt)
#else
    if (instCnt > 1)
#endif
    {
        if (firstInst)
        {
            AFX_ASSERT(gl->DrawElementsInstancedBaseVertexBaseInstance);
            gl->DrawElementsInstancedBaseVertexBaseInstance(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff, instCnt, vtxOff2, firstInst); _ZglThrowErrorOccuried();
        }
        else
        {
            if (0 == vtxOff2)
            {
                gl->DrawElementsInstanced(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff, instCnt); _ZglThrowErrorOccuried();
            }
            else
            {
                gl->DrawElementsInstancedBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff, instCnt, vtxOff2); _ZglThrowErrorOccuried();
            }
        }
    }
    else
    {
#if 0 // not a newer than GL 1.1 function
        if (0 == vtxOff2)
        {
            gl->DrawElements(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff); _ZglThrowErrorOccuried(); 
        }
        else
#endif
        {
            gl->DrawElementsBaseVertex(top, idxCnt, idxSizGl[idxSiz], (void const*)dataOff, vtxOff2); _ZglThrowErrorOccuried();
        }
    }
#endif

    //dpu->numOfFedVertices += cmd->idxCnt;
    dpu->m.numOfFedIndices += idxCnt;
    dpu->m.numOfFedInstances += instCnt;

    //AfxReportMessage("Indexed geometry drawn. idxCnt %u, instCnt %u, baseIdx %u, vtxOff %u, baseInst %u", idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_ZGL void DpuDrawIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = { 0 };

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    glVmt const* gl = dpu->gl;

    GLenum top = AfxToGlTopology(dpu->primTop);
    void* offPtr = (void*)offset;

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, buf, TRUE);
    dpu->boundDrawIndBuf = buf;
    dpu->boundDrawIndBufGpuHandle = buf->glHandle;

#if 0//FORCE_GL_GENERIC_FUNCS
    AFX_ASSERT(gl->MultiDrawArraysIndirect);
    gl->MultiDrawArraysIndirect(top, offPtr, drawCnt, stride); _ZglThrowErrorOccuried();
#else
    if (1 >= drawCnt)
    {
        gl->DrawArraysIndirect(top, offPtr); _ZglThrowErrorOccuried();
    }
    else
    {
        AFX_ASSERT(gl->MultiDrawArraysIndirect);
        gl->MultiDrawArraysIndirect(top, offPtr, drawCnt, stride); _ZglThrowErrorOccuried();
    }
#endif

#ifdef _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, NIL, TRUE);
    dpu->boundDrawIndBuf = NIL;
    dpu->boundDrawIndBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
}

_ZGL void DpuDrawIndirect2(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = { 0 };

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    glVmt const* gl = dpu->gl;

    GLenum top = AfxToGlTopology(dpu->primTop);
    void* offPtr = (void*)offset;

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, buf, TRUE);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);
    DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, cntBuf, TRUE);
    dpu->boundDrawIndBuf = buf;
    dpu->boundDrawIndBufGpuHandle = buf->glHandle;
    dpu->boundParamBuf = cntBuf;
    dpu->boundParamBufGpuHandle = cntBuf->glHandle;

    /*
    The command void MultiDrawArraysIndirectCount( enum mode, const void *indirect, intptr drawcount, intptr maxdrawcount, sizei stride );
    behaves similarly to MultiDrawArraysIndirect, except that drawcount defines an offset (in bytes) into the buffer object bound to the PARAMETER_BUFFER
    binding point at which a single sizei typed value is stored, which contains the draw count.
    maxdrawcount specifies the maximum number of draws that are expected to be stored in the buffer. If the value stored at drawcount into the buffer is greater
    than maxdrawcount, the implementation stops processing draws after maxdrawcount parameter sets. drawcount must be a multiple of four.

    Errors
    In addition to errors that would be generated by MultiDrawArraysIndirect:
    An INVALID_OPERATION error is generated if no buffer is bound to the PARAMETER_BUFFER binding point.
    An INVALID_VALUE error is generated if drawcount is not a multiple of four.
    An INVALID_OPERATION error is generated if reading a sizei typed value from the buffer bound to the PARAMETER_BUFFER target at the offset specified by drawcount would result in an out-of-bounds access.
    */

    AFX_ASSERT(gl->MultiDrawArraysIndirectCount);
    gl->MultiDrawArraysIndirectCount(top, offPtr, cntBufOff, maxDrawCnt, stride); _ZglThrowErrorOccuried();

#ifdef _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, NIL, TRUE);
    dpu->boundDrawIndBuf = NIL;
    dpu->boundDrawIndBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
#ifdef _ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, NIL, TRUE);
    dpu->boundParamBuf = NIL;
    dpu->boundParamBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD
}

_ZGL void DpuDrawIndexedIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = { 0 };

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    glVmt const* gl = dpu->gl;

    avxVertexInput vin = dpu->activeVin;
    afxUnit vaoHandleIdx = dpu->dpuIterIdx % _ZGL_VAO_SWAPS;
    afxSize idxSiz = vin->perDpu[dpu->m.exuIdx][vaoHandleIdx].bindings.idxSrcSiz;
    GLenum top = AfxToGlTopology(dpu->primTop);
    void* offPtr = (void*)offset;

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, buf, TRUE);
    dpu->boundDrawIndBuf = buf;
    dpu->boundDrawIndBufGpuHandle = buf->glHandle;

#if 0//FORCE_GL_GENERIC_FUNCS
    AFX_ASSERT(gl->MultiDrawElementsIndirect);
    gl->MultiDrawElementsIndirect(top, idxSizGl[idxSiz], offPtr, drawCnt, stride); _ZglThrowErrorOccuried();
#else
    if (1 >= drawCnt)
    {
        gl->DrawElementsIndirect(top, idxSizGl[idxSiz], offPtr); _ZglThrowErrorOccuried();
    }
    else
    {
        AFX_ASSERT(gl->MultiDrawElementsIndirect);
        gl->MultiDrawElementsIndirect(top, idxSizGl[idxSiz], offPtr, drawCnt, stride); _ZglThrowErrorOccuried();
    }
#endif

#ifdef _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, NIL, TRUE);
    dpu->boundDrawIndBuf = NIL;
    dpu->boundDrawIndBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
}

_ZGL void DpuDrawIndexedIndirect2(zglDpu* dpu, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = { 0 };

    _DpuFlushPipelineState(dpu);
    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);
    _ZglFlushVertexInputState(dpu);

    glVmt const* gl = dpu->gl;

    avxVertexInput vin = dpu->activeVin;
    afxUnit vaoHandleIdx = dpu->dpuIterIdx % _ZGL_VAO_SWAPS;
    afxSize idxSiz = vin->perDpu[dpu->m.exuIdx][vaoHandleIdx].bindings.idxSrcSiz;
    GLenum top = AfxToGlTopology(dpu->primTop);
    void* offPtr = (void*)offset;

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, buf, TRUE);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);
    DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, cntBuf, TRUE);
    dpu->boundDrawIndBuf = buf;
    dpu->boundDrawIndBufGpuHandle = buf->glHandle;
    dpu->boundParamBuf = cntBuf;
    dpu->boundParamBufGpuHandle = cntBuf->glHandle;

    /*
    The command void MultiDrawElementsIndirectCount( enum mode, enum type, const void *indirect, intptr drawcount, sizei maxdrawcount, sizei stride );
    behaves similarly to MultiDrawElementsIndirect, except that drawcount defines an offset (in bytes) into the buffer object bound to the PARAMETER_BUFFER
    binding point at which a single sizei typed value is stored, which contains the draw count. 
    maxdrawcount specifies the maximum number of draws that are expected to be stored in the buffer. 
    If the value stored at drawcount into the buffer is greater than maxdrawcount, the implementation stops processing draws after maxdrawcount parameter sets. 
    drawcount must be a multiple of four.
        
    Errors
    In addition to errors that would be generated by MultiDrawElementsIndirect:
    An INVALID_OPERATION error is generated if no buffer is bound to the PARAMETER_BUFFER binding point.
    An INVALID_VALUE error is generated if drawcount is not a multiple of four.
    An INVALID_VALUE error is generated if maxdrawcount is negative.
    An INVALID_OPERATION error is generated if reading a sizei typed value
    from the buffer bound to the PARAMETER_BUFFER target at the offset specified by drawcount would result in an out-of-bounds access.
    */

    AFX_ASSERT(gl->MultiDrawElementsIndirectCount);
    gl->MultiDrawElementsIndirectCount(top, idxSizGl[idxSiz], offPtr, cntBufOff, maxDrawCnt, stride); _ZglThrowErrorOccuried();

#ifdef _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, NIL, TRUE);
    dpu->boundDrawIndBuf = NIL;
    dpu->boundDrawIndBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD
#ifdef _ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, NIL, TRUE);
    dpu->boundParamBuf = NIL;
    dpu->boundParamBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_CMD
}

_ZGL void DpuDispatch(zglDpu* dpu, afxUnit grpCntX, afxUnit grpCntY, afxUnit grpCntZ)
{
    afxError err = { 0 };

    //_DpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
    //dpu->activePip = dpu->nextPip;

    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);

    glVmt const* gl = dpu->gl;

    AFX_ASSERT(gl->DispatchCompute);
    gl->DispatchCompute(grpCntX, grpCntY, grpCntZ); _ZglThrowErrorOccuried();
}

_ZGL void DpuDispatchIndirect(zglDpu* dpu, avxBuffer buf, afxUnit32 offset)
{
    afxError err = { 0 };

    //_DpuBindAndSyncPip(dpu, (dpu->activePip != dpu->nextPip), TRUE, dpu->nextPip);
    //dpu->activePip = dpu->nextPip;

    if (!dpu->activePip)
    {
        AfxThrowError();
        return;
    }

    _ZglFlushLigatureState(dpu);

    glVmt const* gl = dpu->gl;

    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    DpuBindAndSyncBuf(dpu, GL_DISPATCH_INDIRECT_BUFFER, buf, TRUE);
    dpu->boundDispatchIndBuf = buf;
    dpu->boundDispatchIndBufGpuHandle = buf->glHandle;

    AFX_ASSERT(gl->DispatchComputeIndirect);
    gl->DispatchComputeIndirect(offset); _ZglThrowErrorOccuried();

#ifdef _ZGL_UNBIND_ANY_INDIRECT_DISPATCH_BUF_AT_CMD
    DpuBindAndSyncBuf(dpu, GL_DISPATCH_INDIRECT_BUFFER, NIL, TRUE);
    dpu->boundDispatchIndBuf = NIL;
    dpu->boundDispatchIndBufGpuHandle = 0;
#endif//_ZGL_UNBIND_ANY_INDIRECT_DISPATCH_BUF_AT_CMD
}

#if 0
_ZGL void _DpuCmdUpdateUniformVector(zglDpu* dpu, _zglCmdUniformVectorEXT const* cmd)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    gl->Uniform4fv(loc, cmd->cnt, cmd->v);
}

_ZGL void _DpuCmdUpdateUniformMatrix(zglDpu* dpu, _zglCmdUniformMatrixEXT const* cmd)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    GLuint loc = gl->GetUniformLocation(dpu->activePip->glHandle, cmd->name);
    //gl->UniformMatrix4fv(loc, cmd->cnt, GL_FALSE, cmd->m);
    AfxThrowError();
}
#endif

_ZGL void _DpuEndOfCommands(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

}

// QUEUE STUFF

_ZGL afxError _DpuWork_ExecuteCb(zglDpu* dpu, _avxIoReqPacket* work)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    afxUnit cnt = work->Execute.cmdbCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (work->Execute.cmdbs[i].wait)
        {
            _DpuWaitForFence(dpu, work->Execute.cmdbs[i].wait, work->Execute.cmdbs[i].waitValue);
            // Require flush.
        }

        afxDrawContext dctx = work->Execute.cmdbs[i].dctx;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        _AvxDpuRollContext(&dpu->m, dctx);

        // Must be disposed because AvxSubmitDrawCommands() reacquires it.
        AfxDisposeObjects(1, &dctx);

        if (work->Execute.cmdbs[i].signal)
        {
            //_ZglSignalFence(dpu, iorp->Execute.signal);
            _DpuSignalFence(dpu, work->Execute.cmdbs[i].signal, work->Execute.cmdbs[i].signalValue);
        }
        else
        {
            gl->Flush(); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError _DpuWork_Transfer(zglDpu* dpu, _avxIoReqPacket* subm)
{
    afxError err = { 0 };
    //AfxAssertObject(dexu, afxFcc_DEXU);

    if (subm->Transfer.wait)
    {
        _DpuWaitForFence(dpu, subm->Transfer.wait, subm->Transfer.waitValue);
        // Require flush.
    }

    switch (subm->Transfer.srcFcc)
    {
    case NIL:
    {
        switch (subm->Transfer.dstFcc)
        {
        case afxFcc_BUF: // raw to buf
        {
            avxBuffer buf = subm->Transfer.dst.buf;
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

            if (DpuUpdateBuffer(dpu, subm->Transfer.dst.buf, subm->Transfer.src.src, subm->Transfer.opCnt, subm->Transfer.bufOps))
                AfxThrowError();

            AfxDisposeObjects(1, &buf);
            break;
        }
        case afxFcc_RAS: // raw to ras
        {
            if (_DpuUpdateRaster(dpu, subm->Transfer.dst.ras, subm->Transfer.src.src, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            avxRaster ras = subm->Transfer.dst.ras;
            AfxDisposeObjects(1, &ras);
            break;
        }
        case NIL:
        {
            for (afxUnit i = 0; i < subm->Transfer.opCnt; i++)
            {
                afxByte* dstAt = (afxByte*)(subm->Transfer.dst.dst) + subm->Transfer.bufOps[i].dstOffset;
                afxByte const* srcAt = (afxByte const*)(subm->Transfer.src.src) + subm->Transfer.bufOps[i].srcOffset;
                AfxStream2(subm->Transfer.bufOps[i].rowCnt, srcAt, subm->Transfer.bufOps[i].srcStride, dstAt, subm->Transfer.bufOps[i].dstStride);
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_BUF:
    {
        switch (subm->Transfer.dstFcc)
        {
        case afxFcc_BUF: // buf to buf
        {
            if (DpuCopyBuffer(dpu, subm->Transfer.src.buf, subm->Transfer.dst.dst, subm->Transfer.opCnt, subm->Transfer.bufCpyOps))
                AfxThrowError();

            avxBuffer src = subm->Transfer.src.buf;
            AfxDisposeObjects(1, &src);
            avxBuffer dst = subm->Transfer.dst.buf;
            AfxDisposeObjects(1, &dst);
            break;
        }
        case afxFcc_RAS:
        {
            if (DpuUnpackRaster(dpu, subm->Transfer.dst.ras, subm->Transfer.src.buf, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            avxBuffer buf = subm->Transfer.src.buf;
            avxRaster ras = subm->Transfer.dst.ras;
            AfxDisposeObjects(1, &ras);
            AfxDisposeObjects(1, &buf);
            break;
        }
        case NIL: // buf to raw
        {
            if (DpuDumpBuffer(dpu, subm->Transfer.src.buf, subm->Transfer.dst.dst, subm->Transfer.opCnt, subm->Transfer.bufOps))
                AfxThrowError();

            avxBuffer buf = subm->Transfer.src.buf;
            AfxDisposeObjects(1, &buf);
            break;
        }
        case afxFcc_IOB: // buf to iob
        {
            if (_DpuDownloadBuffer(dpu, subm->Transfer.src.buf, subm->Transfer.dst.iob, subm->Transfer.opCnt, subm->Transfer.bufOps))
                AfxThrowError();

            afxStream iob = subm->Transfer.dst.iob;
            avxBuffer buf = subm->Transfer.src.buf;
            AfxDisposeObjects(1, &buf);
            AfxDisposeObjects(1, &iob);
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_RAS:
    {
        switch (subm->Transfer.dstFcc)
        {
        case NIL: // ras to raw
        {
            if (_DpuDumpRaster(dpu, subm->Transfer.src.ras, subm->Transfer.dst.dst, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            avxRaster ras = subm->Transfer.src.ras;
            AfxDisposeObjects(1, &ras);
            break;
        }
        case afxFcc_BUF:
        {
            if (DpuPackRaster(dpu, subm->Transfer.src.ras, subm->Transfer.dst.buf, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            avxBuffer buf = subm->Transfer.dst.buf;
            avxRaster ras = subm->Transfer.src.ras;
            AfxDisposeObjects(1, &ras);
            AfxDisposeObjects(1, &buf);
            break;
        }
        case afxFcc_RAS:
        {
            if (DpuCopyRaster(dpu, subm->Transfer.src.ras, subm->Transfer.dst.ras, subm->Transfer.opCnt, subm->Transfer.rasCpyOps))
                AfxThrowError();

            avxRaster dst = subm->Transfer.dst.ras;
            avxRaster src = subm->Transfer.src.ras;
            AfxDisposeObjects(1, &dst);
            AfxDisposeObjects(1, &src);
            break;
        }
        case afxFcc_IOB: // ras to iob
        {
            if (_DpuDownloadRaster(dpu, subm->Transfer.src.ras, subm->Transfer.dst.iob, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            afxStream iob = subm->Transfer.dst.iob;
            avxRaster ras = subm->Transfer.src.ras;
            AfxDisposeObjects(1, &ras);
            AfxDisposeObjects(1, &iob);
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_IOB:
    {
        switch (subm->Transfer.dstFcc)
        {
        case afxFcc_BUF: // iob to buf
        {
            if (_DpuUploadBuffer(dpu, subm->Transfer.dst.buf, subm->Transfer.src.iob, subm->Transfer.opCnt, subm->Transfer.bufOps))
                AfxThrowError();

            afxStream iob = subm->Transfer.src.iob;
            avxBuffer buf = subm->Transfer.dst.buf;
            AfxDisposeObjects(1, &buf);
            AfxDisposeObjects(1, &iob);
            break;
        }
        case afxFcc_RAS: // iob to ras
        {
            if (_DpuUploadRaster(dpu, subm->Transfer.dst.ras, subm->Transfer.src.iob, subm->Transfer.opCnt, subm->Transfer.rasOps))
                AfxThrowError();

            avxRaster ras = subm->Transfer.dst.ras;
            afxStream iob = subm->Transfer.src.iob;
            AfxDisposeObjects(1, &ras);
            AfxDisposeObjects(1, &iob);
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    default: AfxThrowError(); break;
    }

    if (subm->Transfer.signal)
    {
        //_ZglSignalFence(dpu, iorp->Execute.signal);
        _DpuSignalFence(dpu, subm->Transfer.signal, subm->Transfer.signalValue);
    }

    return err;
}

#if 0
_ZGL afxError _DpuWork_Stamp(zglDpu* dpu, _avxIoReqPacket* subm)
{
    afxError err = { 0 };

    for (afxUnit itemIdx = 0; itemIdx < subm->itemCnt; itemIdx++)
    {
        afxSurface dout = subm->items[itemIdx].dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        afxUnit outBufIdx = subm->items[itemIdx].bufIdx;

        afxDrawSystem dsys = dpu->m.dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

        avxRange whd;
        avxCanvas canv;
        AvxGetSurfaceCanvas(dout, outBufIdx, &canv);
        whd = AvxGetCanvasExtent(canv, (avxOrigin) { 0 });

        afxRect area = AFX_RECT(0, 0, whd.w, whd.h);
        avxDrawTarget const dt = { .loadOp = avxLoadOp_LOAD,.storeOp = avxStoreOp_STORE };
        DpuCommenceDrawScope(dpu, canv, &area, 0, 1, 1, &dt, NIL, NIL, NIL, FALSE);

        avxViewport vp = AVX_VIEWPORT(0, 0, whd.w, whd.h, 0, 1);
        DpuSetViewports(dpu, 0, 1, &vp);

        DpuBindPipeline(dpu, dsys->fntRazr, NIL, NIL);

        arxViewConstants vc;
        AfxM4dReset(vc.v);
        AfxComputeOffcenterOrthographicMatrix(vc.p, 0, vp.extent[0], 0, vp.extent[1], -1.f, 1.f, avxClipSpaceDepth_NEG_ONE_TO_ONE);
        //AfxComputeBasicOrthographicMatrix(vc.p, vp.extent[0] / vp.extent[1], 1.0, 3.0, &AVX_CLIP_SPACE_OPENGL);
        DpuUpdateBuffer(dpu, dsys->fntUnifBuf, &vc, 1, &AVX_BUFFER_IO(0, 0, 1, 1, sizeof(vc)));
        
        DpuBindBuffers(dpu, 0, 0, 1, &dsys->fntUnifBuf, (afxUnit const[]) {0}, (afxUnit const[]) { 0 });

        DpuBindSamplers(dpu, 0, 1, 1, &dsys->fntSamp);
        DpuBindRasters(dpu, 0, 1, 1, &dsys->fntRas, NIL);

        afxReal x = subm->Stamp.origin.x;
        afxReal y = subm->Stamp.origin.y;
        afxUnit numchar = subm->Stamp.caption.s.len;
        afxReal r = 1, g = 1, b = 1;
        afxReal x2 = x;

        afxReal* verts;
        _DpuRemapBuf(dpu, dsys->fntDataBuf, 0, AvxGetBufferCapacity(dsys->fntDataBuf, 0), NIL, (void**)&verts);
        //afxReal* verts = (void*)dsys->fntDataBuf->m.bytemap;// AvxMapBuffer(dsys->fntDataBuf, 0, 2048, avxBufferFlag_W, TRUE); // TODO map directly
        void const* bufStart = verts;

        for (char const *ptr = subm->Stamp.caption.s.start, i = 0; *ptr != '\0'; ptr++)
        {
            // Decrement 'y' for any CR's
            if (*ptr == '\n')
            {
                x = x2;
                y -= 16;
                continue;
            }

#if !0
            // Just advance spaces instead of rendering empty quads
            if (*ptr == ' ')
            {
                x += 16;
                numchar--;
                continue;
            }
#endif
            // Emit position, atlas offset, and color for this character
            *verts++ = x;
            *verts++ = y;
            *verts++ = (float)(*ptr % 16)*0.0625f; // *verts++ = (float)(*ptr % 16)*0.0625f;
            *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f; // *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f;
            //*verts++ = r;
            //*verts++ = g;
            //*verts++ = b;
            *verts++ = 1.f;

            // Advance one character
            x += 16;
        }

        _DpuRemapBuf(dpu, dsys->fntDataBuf, 0, 0, NIL, NIL);
        //AvxUnmapBuffer(dsys->fntDataBuf, FALSE);

        avxBufferedStream bufi = { 0 };
        bufi.buf = dsys->fntDataBuf;
        bufi.stride = 20;
        DpuBindVertexBuffers(dpu, 0, 1, &bufi);
        
        avxDrawIndirect dic;
        dic.vtxCnt = 4;
        dic.baseInst = 0;
        dic.baseVtx = 0;
        dic.instCnt = numchar;
        DpuDraw(dpu, &dic);

        DpuConcludeDrawScope(dpu);
        AfxDecAtom32(&dout->m.submCnt);
    }

    return err;
}
#endif

_ZGL afxError _DpuCleanUpResourceUnbinding(zglDpu* dpu)
{
    afxError err = { 0 };

#if !0
    // At draw scope conclusion, it should not have a bound canvas. May be it was suspended rather than concluded.
    AFX_ASSERT(!dpu->canv);

#ifdef _ZGL_UNBIND_ANY_VIN_AT_END
    // unbind any bound vin and potentially its bound buffers.
    if (dpu->activeVin)
    {
        DpuBindVertexInput(dpu, NIL);
        _ZglFlushVertexInputState(dpu);
        AFX_ASSERT(!dpu->activeVin);
    }
#endif
#ifdef _ZGL_UNBIND_ANY_PIP_AT_END
    // unbind any bound pip
    if (dpu->activePip)
    {
        DpuBindPipeline(dpu, NIL, NIL, NIL);
        _DpuFlushPipelineState(dpu);
        AFX_ASSERT(!dpu->activePip);
    }
#endif
#ifdef _ZGL_UNBIND_ANY_INDIRECT_DRAW_BUF_AT_END
    // unbind any indirect draw buffer
    if (dpu->boundDrawIndBuf)
    {
        DpuBindAndSyncBuf(dpu, GL_DRAW_INDIRECT_BUFFER, NIL, TRUE);
        dpu->boundDrawIndBuf = NIL;
        dpu->boundDrawIndBufGpuHandle = 0;
    }
#endif
#ifdef _ZGL_UNBIND_ANY_INDIRECT_DISPATCH_BUF_AT_END
    // unbind any indirect dispatch buffer
    if (dpu->boundDispatchIndBuf)
    {
        DpuBindAndSyncBuf(dpu, GL_DISPATCH_INDIRECT_BUFFER, NIL, TRUE);
        dpu->boundDispatchIndBuf = NIL;
        dpu->boundDispatchIndBufGpuHandle = 0;
    }
#endif
#ifdef _ZGL_UNBIND_ANY_INDIRECT_PARAM_BUF_AT_END
    // unbind any indirect draw/dispatch parameter buffer
    if (dpu->boundParamBuf)
    {
        DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, NIL, TRUE);
        dpu->boundParamBuf = NIL;
        dpu->boundParamBufGpuHandle = 0;
    }
#endif
#endif
}

_ZGL afxInt ZGL_DPU_THR_PROC(afxDrawBridge dexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == dexu->m.worker);

    afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);

    zglDpu* dpu = &dexu->dpu;
    glVmt const* gl = dpu->gl;

    if (!wglMakeCurrentWIN(dexu->hDC, dexu->hRC))
        AfxThrowError();

    AfxMakeString(&dexu->subsysName, 0, (afxChar const*)gl->GetString(GL_RENDERER), 0); _ZglThrowErrorOccuried();
    AfxMakeString(&dexu->subsysVer, 0, (afxChar const*)gl->GetString(GL_VERSION), 0); _ZglThrowErrorOccuried();

    AfxReportf(6, NIL, "Draw Thread for DPU %.03u\n\t%.*s %.*s", dexu->m.exuIdx, AfxPushString(&dexu->subsysName), AfxPushString(&dexu->subsysVer));

#if 0
    AfxReportHint("%s", dpu->wgl.GetExtensionsStringARB(dpu->dc));
#endif

    dpu->m.exuIdx = dexu->m.exuIdx;
    dpu->m.portId = dexu->m.exuIdx;
    dpu->m.dexu = dexu;
    dpu->m.dsys = AvxGetBridgedDrawSystem(dexu, NIL);
    dpu->m.thr = thr;
    dexu->m.dpu = (void*)dpu;

#if _AFX_DEBUG
    gl->Enable(GL_DEBUG_OUTPUT);
    gl->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    gl->DebugMessageCallback(_glDbgMsgCb, NIL);
    //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

    // RESET DEFAULT STATE TO OPENGL DEFAULTS

    dpu->frontFaceCw = FALSE;
    dpu->cullMode = avxCullMode_NONE;// avxCullMode_BACK;
    dpu->fillMode = avxFillMode_FACE;

    dpu->depthBiasConstFactor = 0.f;
    dpu->depthBiasSlopeScale = 0.f;

    dpu->lineWidth = 1.f;
    
    dpu->depthTestEnabled = FALSE;
    dpu->depthCompareOp = avxCompareOp_LESS;
    dpu->depthWriteDisabled = FALSE;
    dpu->depthBounds[0] = 0.f;
    dpu->depthBounds[1] = 1.f;
    
    dpu->stencilTestEnabled = FALSE;
    dpu->stencilFront.compareOp = avxCompareOp_ALWAYS;
    dpu->stencilFront.reference = 0;
    dpu->stencilFront.compareMask = (dpu->stencilFront.writeMask = 0xFFFFFFFF);
    dpu->stencilFront.failOp = (dpu->stencilFront.passOp = (dpu->stencilFront.depthFailOp = avxStencilOp_KEEP));
    dpu->stencilBack.compareOp = avxCompareOp_ALWAYS;
    dpu->stencilBack.reference = 0;
    dpu->stencilBack.compareMask = (dpu->stencilBack.writeMask = 0xFFFFFFFF);
    dpu->stencilBack.failOp = (dpu->stencilBack.passOp = (dpu->stencilBack.depthFailOp = avxStencilOp_KEEP));
    
    dpu->anyBlendEnabled = FALSE;

    for (afxUnit i = 0; i < 8; i++)
    {
        dpu->outs[i].blendConfig.aBlendOp = avxBlendOp_ADD;
        dpu->outs[i].blendConfig.rgbBlendOp = avxBlendOp_ADD;
        dpu->outs[i].blendConfig.aSrcFactor = avxBlendFactor_ONE;
        dpu->outs[i].blendConfig.rgbSrcFactor = avxBlendFactor_ONE;
        dpu->outs[i].blendConfig.aDstFactor = avxBlendFactor_ZERO;
        dpu->outs[i].blendConfig.rgbDstFactor = avxBlendFactor_ZERO;
        dpu->outs[i].discardMask = NIL;
    }

    // In Qwadro specification, seamless cubemap sampling is ALWAYS enabled by default.
    // As SIGMA GL/2 is only bound to Qwadro, actually there is not way of disable it.
    gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS); _ZglThrowErrorOccuried();
    //gl->ClipControl(/*GL_LOWER_LEFT_EXT*/0x8CA1, /*GL_ZERO_TO_ONE_EXT*/0x935F); _ZglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]

    // Scissor test is ALWAYS enabled.
    // We need it to crop the specified canvas rect, for example.
    gl->Enable(GL_SCISSOR_TEST); _ZglThrowErrorOccuried();

    // empty VAO used when a vertex input is not provided to avoid crash on vertex fetch.
    gl->GenVertexArrays(_ZGL_VAO_SWAPS, dpu->emptyVaos); _ZglThrowErrorOccuried();
    for (afxUnit i = 0; i < _ZGL_VAO_SWAPS; i++)
    {
        gl->BindVertexArray(dpu->emptyVaos[i]); _ZglThrowErrorOccuried();
    }
    gl->BindVertexArray(0); _ZglThrowErrorOccuried();

    // FBOs used to blit rasters.
    gl->GenFramebuffers(1, &dpu->fboOpSrc); _ZglThrowErrorOccuried();
    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc); _ZglThrowErrorOccuried();
    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, 0); _ZglThrowErrorOccuried();
    gl->GenFramebuffers(1, &dpu->fboOpDst); _ZglThrowErrorOccuried();
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, dpu->fboOpDst); _ZglThrowErrorOccuried();
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); _ZglThrowErrorOccuried();
    dpu->fboOpSrcAnnex = NIL;
    dpu->fboOpDstAnnex = NIL;

    GLint datai;
    gl->GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &datai); _ZglThrowErrorOccuried();
    dpu->texPackUnitIdx = datai - 1;
    dpu->texUnpackUnitIdx = datai - 2;
    dpu->srcPboGpuHandle = NIL;
    dpu->dstPboGpuHandle = NIL;

    // GL_MAX_COMBINED_UNIFORM_BLOCKS --- the maximum number of uniform blocks per program.The value must be at least 70.
    gl->GetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &datai); _ZglThrowErrorOccuried();
    dpu->activePushUboReservedBindPoint = datai - 1; // the last uniform block

    GLuint pushUbos[_ZGL_PUSH_SWAPS];
    gl->GenBuffers(_ZGL_PUSH_SWAPS, pushUbos); _ZglThrowErrorOccuried();

    for (afxUnit i = 0; i < _ZGL_PUSH_SWAPS; i++)
    {
        dpu->pushSets[i].pushConstUbo = pushUbos[i];
        gl->BindBuffer(GL_UNIFORM_BUFFER, dpu->pushSets[i].pushConstUbo); _ZglThrowErrorOccuried();
#ifdef _AFX_DEBUG
        gl->ObjectLabel(GL_BUFFER, dpu->pushSets[i].pushConstUbo, 13, "pushConstUbo"); _ZglThrowErrorOccuried();
#endif
#ifdef COHERENT_PUSHABLES
        GLbitfield pushUboCreateFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT;
        GLbitfield pushUboAccessFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
#else
        GLbitfield pushUboCreateFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT;
        GLbitfield pushUboAccessFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
#endif
        gl->BufferStorage(GL_UNIFORM_BUFFER, ddev->m.limits.maxPushConstsSiz, NIL, pushUboCreateFlags); _ZglThrowErrorOccuried();
        dpu->pushSets[i].pushConstMappedMem = gl->MapBufferRange(GL_UNIFORM_BUFFER, 0, ddev->m.limits.maxPushConstsSiz, pushUboAccessFlags); _ZglThrowErrorOccuried();
        gl->BindBuffer(GL_UNIFORM_BUFFER, 0); _ZglThrowErrorOccuried();
        gl->BindBufferBase(GL_UNIFORM_BUFFER, dpu->activePushUboReservedBindPoint, dpu->pushSets[i].pushConstUbo); _ZglThrowErrorOccuried();
    }
    dpu->pushSetIdx = 0;

    dpu->emulatedDrawParams = !glHasExtensionSIG(gl, "ARB_shader_draw_parameters");

    dpu->m.instanced = TRUE;
    dpu->m.running = TRUE;
    
    GLint64 maxSrvWaitTimeout;
    gl->GetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &maxSrvWaitTimeout);
    dpu->maxSrvWaitTimeout = maxSrvWaitTimeout;

    AfxMakeChain(&dpu->fenceSignalChain, dpu);

    dpu->dpuIterIdx = 0;

    while (1)
    {
#if !0
            AfxLockMutex(&dexu->m.schedCndMtx);

            while (!dexu->m.schedCnt)
                AfxWaitCondition(&dexu->m.schedCnd, &dexu->m.schedCndMtx);

            dexu->m.schedCnt = 0;
            AfxUnlockMutex(&dexu->m.schedCndMtx);
#endif

        afxBool ctxEntered = FALSE;

        if (!ctxEntered)
        {
            HGLRC rc = wglGetCurrentContextWIN();

            if (rc == dexu->hRC) ctxEntered = TRUE;
            else
            {
                if (!(ctxEntered = !!wglMakeCurrentWIN(dexu->hDC, dexu->hRC)))
                {
                    AfxThrowError();
                    AfxReportError("DPU %u failed to take active context.", dexu->m.exuIdx);
                }
            }

            if (ctxEntered)
                dpu->m.dsys = AvxGetBridgedDrawSystem(dexu, NIL);
        }

        if (ctxEntered)
        {
            dpu->m.numOfFedVertices = 0;
            dpu->m.numOfFedIndices = 0;
            dpu->m.numOfFedInstances = 0;
        }

        gl->BindBufferBase(GL_UNIFORM_BUFFER, dpu->activePushUboReservedBindPoint, dpu->pushSets[dpu->dpuIterIdx % _ZGL_PUSH_SWAPS].pushConstUbo); _ZglThrowErrorOccuried();

        afxBool shouldYield = 0;
        do
        {
            _DpuProcessFenceSignalChain(dpu);
            _ZglProcessDeletionQueue(gl, &dexu->deletionQueue);
            dexu->m.procCb(&dpu->m);
            _DpuProcessFenceSignalChain(dpu);
        } while (0);

        _DpuCleanUpResourceUnbinding(dpu);

#ifdef _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC
        AfxSleep(_YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC_TIMEOUT);
#else
        AfxYield();
#endif//_YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC

        if (AfxShouldThreadBeInterrupted())
            break;

        ++dpu->dpuIterIdx;
    }

    gl->DeleteVertexArrays(_ZGL_VAO_SWAPS, dpu->emptyVaos); _ZglThrowErrorOccuried();
    gl->DeleteFramebuffers(1, &dpu->fboOpSrc); _ZglThrowErrorOccuried();
    gl->DeleteFramebuffers(1, &dpu->fboOpDst); _ZglThrowErrorOccuried();

    for (afxUnit i = 0; i < _ZGL_PUSH_SWAPS; i++)
    {
        gl->DeleteBuffers(1, &dpu->pushSets[i].pushConstUbo); _ZglThrowErrorOccuried();
    }

    wglMakeCurrentWIN(NIL, NIL);

    return 0;
}

_ZGL afxBool _Dpu_ProcCb(zglDpu* dpu)
{
    afxError err = { 0 };
    AFX_ASSERT(dpu);

    glVmt const* gl = dpu->gl;
    afxDrawBridge dexu = dpu->m.dexu;
    _avxIoReqLut const* iorpVmt = dexu->m.iorpVmt;

    afxClass const* dqueCls = _AvxDexuGetDqueClass(dexu);
    afxUnit totalQueCnt = dqueCls->instCnt;

    afxBool mustWaitCnt = 0;

    while (1) // while mustWaitCnt to avoid thread sleep
    {
        for (afxUnit queIdx = 0; queIdx < totalQueCnt; queIdx++)
        {
            afxDrawQueue dque;
            if (!AvxGetDrawQueues(dexu, queIdx, 1, &dque) || !dque)
            {
                AfxThrowError();
                continue;
            }
            AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

            if (dque->m.exuIdx != dexu->m.exuIdx)
                continue;

            if (AfxTryLockMutex(&dque->m.iorpChnMtx))
            {
#if 0
                do
#endif
                {
#if _USE_REAL_GL_FENCES
#if _USE_SINGLE_GL_FENCES
                    GLsync singleGlFence = NIL;
                    afxBool singleGlFenceCompleted = FALSE;
#endif
#endif
                    //afxUnit iterCnt = 3; // in case we want to do a second fast iteration for resolve fences still holding the mutex.
                    //afxUnit iterCnt = 1;
                    /*
                        2 better fit iterCnt once it can iterate again after a Flush and/or Yield and mainly after a Finish
                        without unlocking and relocking the mutex.
                    */
                    afxUnit iterCnt = 2;

                    while (iterCnt--)
                    {
                        _avxIoReqPacket* iorp;
                        AFX_ITERATE_CHAIN_B2F(iorp, hdr.chain, &dque->m.iorpChn)
                        {
                            AFX_ASSERT(dque->m.iorpChn.cnt);

                            if (iorp->hdr.pulled == 0)
                            {
                                iorp->hdr.pulled = 1;
                                AfxGetClock(&iorp->hdr.pullTime);
                                iorp->hdr.dpuId = dpu->m.portId;

                                AFX_ASSERT(iorpVmt->f[iorp->hdr.id]);
                                if (afxError_TIMEOUT == iorpVmt->f[iorp->hdr.id](dpu, iorp))
                                    continue;

#ifdef _FLUSH_ON_IOREQ_EXECUTION
                                gl->Flush();
#endif//_FLUSH_ON_IOREQ_EXECUTION

                                //if (iorp->hdr.completionFence)
                                if (0)
                                {
                                    //_ZglResetFence(dpu, iorp->hdr.completionFence);
                                    ++mustWaitCnt;
#ifdef _USE_REAL_GL_FENCES
#ifndef _USE_SINGLE_GL_FENCES
                                    iorp->hdr.syncIdd0 = gl->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); _ZglThrowErrorOccuried();
#else
                                    iorp->hdr.syncIdd0 = NIL;
#endif//_USE_SINGLE_GL_FENCES
#else
                                    iorp->hdr.syncIdd0 = NIL;
#endif//_USE_REAL_GL_FENCES
                                }
                                else
                                {
                                    iorp->hdr.syncIdd0 = NIL;
                                    AfxGetClock(&iorp->hdr.complTime);
                                    iorp->hdr.completed = 1;
                                    // if we have not to wait, delete it right now.
                                    _AvxDquePopIoReqPacket(dque, iorp);
                                }
                            }
                            // if already pulled, test if it has been this DPU.
                            else if (iorp->hdr.dpuId == dpu->m.portId)
                            {
                                if (iorp->hdr.completed)
                                {
                                    // delete completed workload
                                    _AvxDquePopIoReqPacket(dque, iorp);
                                }
                                else
                                {
#ifdef _USE_REAL_GL_FENCES
#ifndef _USE_SINGLE_GL_FENCES
                                    GLsync glFence = iorp->hdr.syncIdd0;
#else
                                    GLsync glFence = singleGlFence;
#endif//_USE_SINGLE_GL_FENCES

#ifdef _USE_SINGLE_GL_FENCES
                                    GLenum rslt = GL_CONDITION_SATISFIED;
                                    if (!singleGlFenceCompleted && glFence)
                                    {
                                        // To block all CPU operations until a sync object is signaled, you call this function:
                                        rslt = gl->ClientWaitSync(glFence, GL_SYNC_FLUSH_COMMANDS_BIT, /*timeout*/0); _ZglThrowErrorOccuried();
                                    }
#else
                                    // To block all CPU operations until a sync object is signaled, you call this function:
                                    GLenum rslt = gl->ClientWaitSync(glFence, GL_SYNC_FLUSH_COMMANDS_BIT, /*timeout*/0); _ZglThrowErrorOccuried();
#endif//_USE_SINGLE_GL_FENCES
                                    /*
                                        This function will not return until one of two things happens: the sync​ object parameter becomes signaled, or a number of nanoseconds greater than or equal to the timeout​ parameter passes.
                                        If timeout​ is zero, the function will simply check to see if the sync object is signaled and return immediately.
                                        Note that the fact that timeout​ is in nanoseconds does not imply that this function has true nanosecond granularity in its timeout; you are only guaranteed that at least that much time will pass.
                                    */

                                    switch (rslt)
                                    {
                                    case GL_TIMEOUT_EXPIRED: // the sync object has not been signaled within the given timeout period.
                                        //AfxYield(); // once we have locked inside this loop, give way.
                                        break; // switch; wait for next iteration
                                    case GL_ALREADY_SIGNALED: // the sync object was signaled before the function was called.
                                    case GL_CONDITION_SATISFIED: // the sync object was signaled within the given timeout period.
                                    {
                                        iorp->hdr.completed = 1;
                                        AfxGetClock(&iorp->hdr.complTime);
                                        //avxFence fenc = iorp->hdr.completionFence;
                                        avxFence fenc = NIL;

                                        if (fenc)
                                        {
                                            //_ZglSignalFence(dpu, fenc);
                                            _ZglFencSignalOnHostCb(fenc, 1);
                                        }
#ifndef _USE_SINGLE_GL_FENCES
                                        gl->DeleteSync(glFence); _ZglThrowErrorOccuried();
                                        iorp->hdr.syncIdd0 = NIL;
#else
                                        singleGlFenceCompleted = TRUE;
#endif//_USE_SINGLE_GL_FENCES
                                        _AvxDquePopIoReqPacket(dque, iorp);
                                        --mustWaitCnt;
                                        break;
                                    }
                                    case GL_WAIT_FAILED: // If an OpenGL Error occurred, then GL_WAIT_FAILED will be returned in addition to raising an error.
                                    default:
#ifndef _USE_SINGLE_GL_FENCES
                                        gl->DeleteSync(glFence); _ZglThrowErrorOccuried();
                                        iorp->hdr.syncIdd0 = NIL;
#else
                                        singleGlFenceCompleted = TRUE;
#endif//_USE_SINGLE_GL_FENCES
                                        iorp->hdr.completed = 1;
                                        AfxGetClock(&iorp->hdr.complTime);
                                        _AvxDquePopIoReqPacket(dque, iorp);
                                        --mustWaitCnt;
                                        break;
                                    }
#else
                                    _ZglSignalFence(dpu, iorp->hdr.completionFence);
#endif//_USE_REAL_GL_FENCES
                                }
                            }
                        }
#ifdef _USE_SINGLE_GL_FENCES
                        if (!singleGlFence && mustWaitCnt)
                        {
                            AFX_ASSERT(!singleGlFenceCompleted);
                            singleGlFence = gl->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); _ZglThrowErrorOccuried();
                        }
                        else
#ifdef _USE_SINGLE_GL_FENCES
                        {
                            if (!singleGlFenceCompleted)
                            {
                                if (iterCnt == 0 && mustWaitCnt)
                                {
                                    iterCnt = 1;
                                    AfxYield();
                                    AfxSleep(0);
                                }
                            }
                            else
                            {
                                AFX_ASSERT(singleGlFenceCompleted);
                                AFX_ASSERT(singleGlFence);
                                gl->DeleteSync(singleGlFence); _ZglThrowErrorOccuried();
                                singleGlFence = NIL;
                            }
#endif//_USE_SINGLE_GL_FENCES
                        }
#endif//_USE_SINGLE_GL_FENCES
#ifdef _FINISH_AFTER_ALL_IOREQ_EXECUTIONS
                        gl->Finish();
#else
                        // Finish does already wait for completion, so we don't need to yield or flush.
#ifdef _YIELD_AFTER_ALL_IOREQ_EXECUTIONS
#ifdef _FLUSH_BEFORE_YIELD_AFTER_ALL_IOREQ_EXECUTIONS
                    // Flush only if we are not flush after each batch.
#ifndef _FLUSH_ON_IOREQ_EXECUTION
                        gl->Flush();
#endif//_FLUSH_ON_IOREQ_EXECUTION
#endif//_FLUSH_BEFORE_YIELD_AFTER_ALL_IOREQ_EXECUTIONS
#ifdef _YIELD_WITH_SLEEP_AFTER_ALL_IOREQ_EXECUTIONS
                        AfxSleep(_YIELD_WITH_SLEEP_AFTER_ALL_IOREQ_EXECUTIONS_TIMEOUT);
#else
                        AfxYield();
#endif//_YIELD_WITH_SLEEP_AFTER_ALL_IOREQ_EXECUTIONS
#endif//_YIELD_AFTER_ALL_IOREQ_EXECUTIONS
#endif//_FINISH_AFTER_ALL_IOREQ_EXECUTIONS
                    }
                }
#if 0
                while (mustWaitCnt);
#endif
                AfxSignalCondition(&dque->m.idleCnd);
                AfxUnlockMutex(&dque->m.iorpChnMtx);
                //AfxYield();
            }
#if !0
            else
            {
                afxDrawBridge dexu = AfxGetHost(dque);
                AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
                _AvxDexu_PingCb(dexu, 0);
            }
#else
            dexu->m.schedCnt += 1;
#endif
        }

        if (mustWaitCnt == 0)
            break; // quit while 
    }
    return TRUE;
}

_ZGL afxError _ZglDexuDtorCb(afxDrawBridge dexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = AvxGetBridgedDrawSystem(dexu, NIL);

    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE); // yes, two times.

    if (_AVX_CLASS_CONFIG_DEXU.dtor(dexu))
        AfxThrowError();

    wglDeleteContextWIN(dexu->hRC);
    ReleaseDC(dexu->hWnd, dexu->hDC);
    DestroyWindow(dexu->hWnd);

    AfxExhaustInterlockedQueue(&dexu->deletionQueue);

    return err;
}

_ZGL afxError _ZglDexuCtorCb(afxDrawBridge dexu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    _avxDexuAcq const* cfg = AFX_CAST(_avxDexuAcq const*, args[1]) + invokeNo;
    
    afxDrawDevice ddev = cfg->ddev;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    if (wglCreateSurfaceSIGMA(0, 0, &dexu->hWnd, &dexu->hDC, &dexu->dcPxlFmt, &dexu->dcPfd))
    {
        AfxThrowError();
        return err;
    }

    HDC bkpHdc = wglGetCurrentDCWIN();
    HGLRC bkpGlrc = wglGetCurrentContextWIN();
    HGLRC hShareRC = dsys->hPrimeRC;

    AfxZero(&dexu->dpu, sizeof(dexu->dpu));
    if (wglCreateContextSIGMA(dexu->hDC, hShareRC, dsys->glVerMaj, dsys->glVerMin, FALSE, dsys->robustCtx, FALSE, &dexu->hRC, NIL, FALSE))
    {
        AfxThrowError();
        ReleaseDC(dexu->hWnd, dexu->hDC), DestroyWindow(dexu->hWnd);
        return err;
    }

    dexu->dpu.gl = &dsys->gl;

    if (err)
    {
        wglDeleteContextWIN(dexu->hRC);
        ReleaseDC(dexu->hWnd, dexu->hDC), DestroyWindow(dexu->hWnd);
    }

#if 0
    afxBool shared = FALSE;
    afxDrawBridge primeExu = NIL;
    afxUnit primeExuIdx = 0;
    for (afxUnit i = 0; i < dsys->m.exuCnt; i++)
    {
        if (dsys->m.exus[i] && (ddev == AfxGetHost(dsys->m.exus[i])));
        {
            primeExuIdx = i;
            primeExu = dsys->m.exus[primeExuIdx];
            shared = TRUE;
        }
    }
#endif

#if 0
    dexu->vinExt.extId = afxFcc_DEXU;
    AfxInstallClassExtension(_AvxDsysGetVinClassCb_SW(dsys), &dexu->vinExt);

    dexu->pipExt.extId = afxFcc_DEXU;
    AfxInstallClassExtension(_AvxDsysGetPipClassCb_SW(dsys), &dexu->pipExt);

    dexu->canvExt.extId = afxFcc_DEXU;
    AfxInstallClassExtension(_AvxDsysGetCanvClassCb_SW(dsys), &dexu->canvExt);
#endif

    if (_AVX_CLASS_CONFIG_DEXU.ctor(dexu, (void*[]) { dsys, (void*)cfg }, 0))
    {
        AfxThrowError();
        wglDeleteContextWIN(dexu->hRC);
        ReleaseDC(dexu->hWnd, dexu->hDC), DestroyWindow(dexu->hWnd);
        return err;
    }

    AfxMakeInterlockedQueue(&dexu->deletionQueue, sizeof(_zglDeleteGlRes), 32);

    static _avxIoReqLut const dpuIorpVmt =
    {
        .Callback = _AvxDpuWork_CallbackCb,
        .Execute = _DpuWork_ExecuteCb,
        .Transfer = _DpuWork_Transfer,
        .Remap = _DpuWork_Remap,
        .SyncMaps = _DpuWork_SyncMaps
    };

    dexu->m.cmdVmt = &cmdDevmt;
    dexu->m.iorpVmt = &dpuIorpVmt;
    dexu->m.workerProc = ZGL_DPU_THR_PROC;
    dexu->m.procCb = (void*)_Dpu_ProcCb;

    if (err)
    {
        _AVX_CLASS_CONFIG_DEXU.dtor(dexu);
        wglDeleteContextWIN(dexu->hRC);
        ReleaseDC(dexu->hWnd, dexu->hDC), DestroyWindow(dexu->hWnd);
    }
    return err;
}
