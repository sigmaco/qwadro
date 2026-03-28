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

// This file is part of Advanced RenderWare Extensions.

#define _ARX_C
#define _ARX_BUFFER_C
#include "arxIcd.h"

_ARX arxScenario ArxGetBufferHost(arxBuffer rbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    arxScenario scio = AfxGetHost(rbuf);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio;
}

_ARX void* ArxGetBufferUdd(arxBuffer rbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    return rbuf->udd;
}

_ARX afxUnit ArxGetBufferCapacity(arxBuffer rbuf, afxUnit from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    return AFX_MIN(rbuf->reqSiz, rbuf->reqSiz - from);
}

_ARX arxBufferUsage ArxGetBufferUsage(arxBuffer rbuf, arxBufferUsage usage)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    return usage ? (rbuf->usage & usage) : rbuf->usage;
}

_ARX arxBufferFlags ArxGetBufferFlags(arxBuffer rbuf, arxBufferFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    return mask ? (rbuf->flags & mask) : rbuf->flags;
}

_ARXINL void _ArxSanitizeBufferCopy(arxBuffer src, arxBuffer dst, arxBufferCopy const* raw, arxBufferCopy* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &dst);

    san->srcOffset = AFX_MIN(raw->srcOffset, src->reqSiz - 1);
    san->dstOffset = AFX_MIN(raw->dstOffset, dst->reqSiz - 1);
    san->range = AFX_CLAMP(raw->range, 1, AFX_MIN(src->reqSiz - san->srcOffset, dst->reqSiz - san->dstOffset));
}

_ARXINL void _ArxSanitizeBufferIo(arxBuffer src, arxBuffer dst, arxBufferIo const* raw, arxBufferIo* san)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &dst);

    san->srcOffset = AFX_MIN(raw->srcOffset, src->reqSiz - 1);
    san->dstOffset = AFX_MIN(raw->dstOffset, dst->reqSiz - 1);
    san->srcStride = AFX_MIN(raw->srcStride, src->reqSiz - san->srcOffset);
    san->dstStride = AFX_MIN(raw->dstStride, dst->reqSiz - san->dstOffset);
    san->rowCnt = AFX_CLAMP(raw->rowCnt, 1, AFX_MIN(raw->rowCnt - src->reqSiz / san->srcStride, raw->rowCnt - dst->reqSiz / san->dstStride));
}

_ARX void* ArxGetBufferData(arxBuffer rbuf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);
    return &rbuf->mem.hostedAlloc.bytemap[from];
}

////////////////////////////////////////////////////////////////////////////////

_ARX afxError _ArxRbufDtorCb(arxBuffer rbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);

    while (rbuf->mem.mapPtr)
    {
        //ArxUnmapBuffer(rbuf, TRUE);
        AfxYield();
        //AFX_ASSERT(!buf->bytemap);
    }

    if (rbuf->mem.hostedAlloc.bytemap)
    {
        AfxDeallocate((void**)&rbuf->mem.hostedAlloc.bytemap, AfxHere());
    }

    if (rbuf->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf->base);
        AfxDisposeObjects(1, &rbuf->base);
    }

    return err;
}

_ARX afxError _ArxRbufCtorCb(arxBuffer rbuf, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &rbuf);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxBufferInfo const* bufi = ((arxBufferInfo const*)args[1]) + invokeNo;
    AFX_ASSERT(bufi);

    AFX_ASSERT(bufi && bufi->size && bufi->usage);

    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    rbuf->tag = bufi->tag;
    rbuf->udd = bufi->udd;

    arxBuffer base = bufi->base;
    afxSize from = bufi->from;
    afxUnit size = bufi->size;

    if (!AFX_TEST_ALIGNMENT(from, AVX_BUFFER_ALIGNMENT))
    {
        AFX_ASSERT_ALIGNMENT(from, AVX_BUFFER_ALIGNMENT);
        AfxThrowError();
        err = afxError_OUT_OF_RANGE;
        return err;
    }

    if (!base)
    {
        if (!size)
        {
            // If there is not a @base buffer, we need a size here.
            AfxThrowError();
            return err;
        }
        // If there is not a @base buffer, @from is useless and should be zero.
        AFX_ASSERT(from == 0);
        from = 0;
        // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
        // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
        size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &base);

        if (base->base)
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxUnit srcCap = ArxGetBufferCapacity(base, 0);

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (size == 0)
            size = srcCap - from;
        else
            size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);

        AFX_ASSERT_RANGE(srcCap, from, size);

        if ((from >= srcCap) ||
            (size > srcCap) ||
            (size > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }
        AfxReacquireObjects(1, &base);
    }

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    rbuf->reqSiz = size;
    rbuf->from = from;
    rbuf->base = base;

    if (err) return err;
#ifdef AVX_VALIDATION_ENABLED
    if (!rbuf->base)
    {
        AFX_ASSERT(rbuf->from == 0);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_RBUF, 1, &base);
        AFX_ASSERT_ALIGNMENT(rbuf->from, AVX_BUFFER_ALIGNMENT);
    }
#endif

    rbuf->usage = bufi->usage;
    rbuf->flags = bufi->flags;
    rbuf->fmt = bufi->fmt;

    afxUnit exuCnt = 16; // TODO Get it from DSYS
    rbuf->sharingMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        rbuf->sharingMask |= bufi->sharingMask & AFX_BITMASK(i);
    }

    // STORAGE
    rbuf->reqMemType = NIL;
    rbuf->reqAlign = AFX_SIMD_ALIGNMENT;
    rbuf->memBase = 0;

    _arxMemory* smem = &rbuf->mem;
    smem->size = rbuf->reqSiz;
    smem->alignment = AFX_SIMD_ALIGNMENT;

    // binding
    smem->mmu = 0;
    //rbuf->storage[0].offset = 0;
    smem->hostedAlloc.addr = NIL;

    smem->mapPtr = NIL;
    smem->mapOffset = 0;
    smem->mapRange = 0;
    smem->mapFlags = NIL;

    smem->pendingRemap = 0;
    smem->permanentlyMapped = !!bufi->mapped;

    AfxAllocate(smem->size, smem->alignment, AfxHere(), (void**)&smem->hostedAlloc.bytemap);

    smem->mapPtr = smem->hostedAlloc.bytemap;
    smem->mapRange = smem->size;
    smem->permanentlyMapped = TRUE;
    

    return err;
}

_ARX afxClassConfig const _ARX_RBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_RBUF,
    .name = "Buffer",
    .desc = "Synergy Buffer", // ARX Buffer
    .fixedSiz = sizeof(AFX_OBJECT(arxBuffer)),
    .ctor = (void*)_ArxRbufCtorCb,
    .dtor = (void*)_ArxRbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireBuffers(arxScenario scio, afxUnit cnt, arxBufferInfo const info[], arxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetRbufClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_RBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { scio, info }))
    {
        AfxThrowError();
    }
    AFX_ASSERT_OBJECTS(afxFcc_RBUF, cnt, buffers);

    return err;
}
