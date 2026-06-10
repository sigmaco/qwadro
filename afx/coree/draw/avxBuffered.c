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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#include "avxIcd.h"

_AVX afxError AvxMakeBufferedRing(avxBufferedRing* rng, avxBuffer buf, afxSize bufBase, afxUnit bufRange, afxUnit blockSiz, afxUnit blockAlign, afxUnit rounds)
{
    // Small buffered ring utility.
    // Reuse memory across frames (no new buffer creation).
    // Efficient for dynamic scenes and many objects.
    // Prevents CPU-GPU sync stalls and fragmentation.

    afxError err = { 0 };
    AFX_ASSERT(rng);
    *rng = (avxBufferedRing) { 0 };

    rng->buf = buf;
    rng->bufBase = bufBase;
    rng->bufRange = bufRange;

    if (buf)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        rng->basePtr = AvxGetBufferMap(buf, bufBase, bufRange);
        
        if (!rng->basePtr)
        {
            AfxReportError("avxBuffer %p [ %u, %u ] not mapped", buf, bufBase, bufRange);
            AfxThrowError();
        }
    }
    else
    {
        rng->basePtr = (void*)bufBase;

        if (!rng->basePtr)
        {
            AfxReportError("avxBuffer %p [ %u, %u ] not mapped", buf, bufBase, bufRange);
            AfxThrowError();
        }
    }

    rng->currOffset = 0;

    rng->blockAlign = AFX_ALIGN_SIZE(AFX_MIN(AVX_BUFFER_ALIGNMENT, blockAlign), AVX_BUFFER_ALIGNMENT);
    rng->blockSiz = AFX_MAX(rng->blockAlign, AVX_ALIGN_BUFFERED(blockSiz));
    rng->blockCnt = rng->bufRange / rng->blockSiz;

    // Use triple buffering (rounds = 3) to rotate through buffer regions.
    rng->rounds = AFX_MAX(1, rounds);

    return err;
}

_AVX afxSize AvxCycleBufferedRing(avxBufferedRing* rng)
{
    afxError err = { 0 };
    AFX_ASSERT(rng);

    rng->currOffset = rng->bufBase + (rng->currOffset + rng->blockSiz * rng->blockCnt / rng->rounds) % rng->bufRange;

    return rng->currOffset;
}

_AVX void* AvxAdvanceBufferedRing(avxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange)
{
    afxError err = { 0 };
    AFX_ASSERT(rng);

    reqSiz = AFX_ALIGN_SIZE(reqSiz, rng->blockSiz);

    if (rng->currOffset + reqSiz > rng->bufRange)
    {
        // Wrap around
        rng->currOffset = 0;
    }

    void* blockPtr = &rng->basePtr[rng->currOffset];

    // Bind for shader access
    // You now write directly into blockPtr and the shader sees the bound block via layout(std140, binding = %).
    if (pOffset)
        *pOffset = rng->currOffset;

    if (pRange)
        *pRange = reqSiz;

    rng->currOffset += reqSiz;

    return blockPtr;
}


// Align and advance the allocation pointer
static void* aligned_alloc_buffer(avxBufferedPumpStash* chunk, afxSize size, afxUnit align, afxSize* out_offset)
{
    afxSize aligned_offset = AFX_ALIGN_SIZE(chunk->used, align);
    if (aligned_offset + size > chunk->capacity)
        return NULL;

    *out_offset = aligned_offset;
    chunk->used = aligned_offset + size;
    return (char*)chunk->mapped_ptr + aligned_offset;
}

// Create a new chunk, fully initialized
static avxBufferedPumpStash create_new_chunk(avxBufferedPump* pump, afxSize size)
{
    afxError err = { 0 };
    avxBufferedPumpStash chunk = { 0 };
    chunk.capacity = AFX_ALIGN_SIZE(size, pump->blockAlign);
    chunk.used = 0;
    chunk.frame_in_use = pump->current_frame;

    avxBuffer buf;
    avxBufferInfo bufi = { 0 };
    bufi.usage = pump->bufUsage;
    bufi.flags = pump->bufFlags;
    bufi.size = chunk.capacity;
    AvxAcquireBuffers(pump->dsys, 1, &bufi, &buf);
    chunk.buffer = buf;

    AvxMapBuffer(buf, 0, bufi.size, NIL, &chunk.mapped_ptr);

    if (!chunk.mapped_ptr)
    {
        AfxThrowError();
    }

    return chunk;
}

// Initialize the buffered pump
_AVX afxError AvxDeployBufferedPump(avxBufferedPump* pump, avxBufferUsage usage, avxBufferFlags flags, afxUnit minChunkSiz, afxUnit blockAlign, afxUnit rounds, afxDrawSystem dsys)
{
    afxError err = { 0 };

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    pump->dsys = dsys;

    pump->capacity_chunks = 4;
    pump->num_chunks = 0;
    pump->current_frame = 0;
    pump->last = NULL;
    pump->chunks = NIL;

    if (AfxAllocate(AfxHere(), sizeof(avxBufferedPumpStash) * pump->capacity_chunks, NIL, (void**)&pump->chunks))
    {
        AfxThrowError();
    }

    AFX_ASSERT(32 >= rounds);
    pump->rounds = rounds ? rounds : 3; // triple buffering
    pump->bufUsage = usage ? usage : avxBufferUsage_UNIFORM;
    pump->bufFlags = flags ? flags : avxBufferFlag_WX;
    AFX_ASSERT_ALIGNMENT(minChunkSiz, AVX_BUFFER_ALIGNMENT);
    pump->minChunkSiz = minChunkSiz ? AVX_ALIGN_BUFFERED(minChunkSiz) : 2 * 1024 * 1024; // 2MB
    AFX_ASSERT_ALIGNMENT(blockAlign, AVX_BUFFER_ALIGNMENT);
    pump->blockAlign = blockAlign ? AVX_ALIGN_BUFFERED(blockAlign) : 256; // uniform buffer alignment

    return err;
}

// Find a chunk with enough space or create a new one
static avxBufferedPumpStash* find_available_chunk(avxBufferedPump* pump, afxSize size_needed, afxSize* out_offset)
{
    afxError err = { 0 };

    // Try last-used chunk first
    if (pump->last)
    {
        avxBufferedPumpStash* chunk = pump->last;
        if (chunk->frame_in_use <= pump->current_frame - pump->rounds)
        {
            chunk->used = 0;
            chunk->frame_in_use = pump->current_frame;
        }

        void* ptr = aligned_alloc_buffer(chunk, size_needed, pump->blockAlign, out_offset);
        if (ptr)
            return chunk;
    }

    // Search all other chunks
    for (afxSize i = 0; i < pump->num_chunks; ++i)
    {
        avxBufferedPumpStash* chunk = &pump->chunks[i];
        if (chunk->frame_in_use <= pump->current_frame - pump->rounds)
        {
            chunk->used = 0;
            chunk->frame_in_use = pump->current_frame;
        }

        void* ptr = aligned_alloc_buffer(chunk, size_needed, pump->blockAlign, out_offset);
        if (ptr)
            return chunk;
    }

    // Need to allocate a new chunk
    if (pump->num_chunks >= pump->capacity_chunks)
    {
        afxSize new_capacity = pump->capacity_chunks * 2;

        if (AfxReallocate(AfxHere(), sizeof(avxBufferedPumpStash) * new_capacity, 0, (void**)&pump->chunks))
        {
            AfxThrowError(); // or handle OOM gracefully
        }
        else
        {
            AFX_ASSERT(pump->chunks);
            pump->capacity_chunks = new_capacity;
        }
    }

    afxSize chunk_size = size_needed > pump->minChunkSiz ? size_needed : pump->minChunkSiz;
    avxBufferedPumpStash new_chunk = create_new_chunk(pump, chunk_size);
    pump->chunks[pump->num_chunks++] = new_chunk;
    *out_offset = 0;
    return &pump->chunks[pump->num_chunks - 1];
}

// Allocate from buffered pump
_AVX void* AvxRequestBufferedPump(avxBufferedPump* pump, afxSize size, avxBuffer* out_buffer, afxSize* out_offset)
{
    avxBufferedPumpStash* chunk = find_available_chunk(pump, size, out_offset);
    pump->last = chunk;
    *out_buffer = chunk->buffer;
    return (char*)chunk->mapped_ptr + *out_offset;
}

// Advance to next frame
_AVX afxError AvxAdvanceBufferedPump(avxBufferedPump* pump)
{
    afxError err = { 0 };
    pump->current_frame += 1;
    return err;
}

// Cleanup all buffers
_AVX afxError AvxDismantleBufferedPump(avxBufferedPump* pump)
{
    afxError err = { 0 };

    for (afxSize i = 0; i < pump->num_chunks; ++i)
    {
        AvxUnmapBuffer(pump->chunks[i].buffer, FALSE);
        AfxDisposeObjects(1, &pump->chunks[i].buffer);
    }

    pump->num_chunks = 0;
    pump->last = NULL;

    if (pump->chunks)
    {
        if (AfxDeallocate(AfxHere(), (void**)&pump->chunks))
            AfxThrowError();

        pump->chunks = NULL;
    }

    pump->capacity_chunks = 0;

    return err;
}
