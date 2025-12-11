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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _ASX_STEP_C
#define _ASX_BUFFER_C
#include "asxIcd.h"

extern unsigned char rawData[1095920];

_ASX afxStepSystem AsxGetBufferHost(asxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    afxStepSystem ssys = AfxGetHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    return ssys;
}

_ASX void* AsxGetBufferUdd(asxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    return buf->udd;
}

_ASX afxSize AsxGetBufferAddress(asxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    return buf->storage[0].hostedAlloc.addr + buf->storage[0].offset;
}

_ASX afxSize AsxGetBufferCapacity(asxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    return AFX_MIN(buf->reqSiz, buf->reqSiz - from);
}

_ASX asxBufferUsage AsxGetBufferUsage(asxBuffer buf, asxBufferUsage mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    return (!mask) ? buf->usage : (buf->usage & mask);
}

_ASX asxBufferFlags AsxGetBufferFlags(asxBuffer buf, asxBufferFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    return (!mask) ? buf->flags : (buf->flags & mask);
}

////////////////////////////////////////////////////////////////////////////////

_ASX void* AsxGetBufferMap(asxBuffer sbuf, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    afxSize mappedOffset = sbuf->storage[0].mapOffset;
    afxSize mappedRange = sbuf->storage[0].mapRange;

    // Validate if the requested region is within the currently mapped region
    if ((offset < mappedOffset) || ((offset + range) > (mappedOffset + mappedRange)))
        return NIL;

    // If the region is valid, set the placeholder to the corresponding address
    AFX_ASSERT(mappedOffset + mappedRange >= offset + range);
    AFX_ASSERT(mappedOffset <= offset);
    afxSize offDiff = offset - mappedOffset;

    AFX_ASSERT(sbuf->storage[0].mapPtr);
    return &sbuf->storage[0].mapPtr[offDiff];
}

_ASX afxError AsxUnmapBuffer(asxBuffer buf, afxBool wait)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    AFX_ASSERT(AsxGetBufferFlags(buf, asxBufferFlag_RW));

    afxStepSystem ssys = AsxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    asxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = buf->storage[0].mapOffset;
    map.range = buf->storage[0].mapRange;
    map.flags = buf->storage[0].mapFlags;

    if (AsxUnmapBuffers(ssys, 1, &map))
        AfxThrowError();

    return err;
}

_ASX afxError AsxMapBuffer(asxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    AFX_ASSERT(AsxGetBufferFlags(buf, asxBufferFlag_RW));
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, ASX_BUFFER_ALIGNMENT));

    asxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxStepSystem ssys = AsxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    void* holder;
    if (AsxMapBuffers(ssys, 1, &map, (void**[]) { &holder }))
        AfxThrowError();

    // TODO(?): If no placeholder is specified, do not wait for completion.

    AFX_ASSERT(placeholder);
    *placeholder = holder;

    return err;
}

_ASX afxError AsxCohereMappedBuffer(asxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, afxBool discard)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, ASX_BUFFER_ALIGNMENT));

    asxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxStepSystem ssys = AsxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    if (AsxCohereMappedBuffers(ssys, discard, 1, &map))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_ASX afxError _AsxSbufDtorCb(asxBuffer sbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    if (!(sbuf->flags & asxBufferFlag_F))
    {
        if (sbuf->storage[0].hostedAlloc.bytemap && (!sbuf->base || (sbuf->base != sbuf)))
        {
            AfxDeallocate((void**)&sbuf->storage[0].hostedAlloc.bytemap, AfxHere());
        }
    }
    return err;
}

_ASX afxError _AsxSbufCtorCb(asxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &buf);

    afxStepSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    asxBufferInfo const* bufi = args[1] ? ((asxBufferInfo const *)args[1]) + invokeNo : NIL;
    asxMetabufferInfo const* subi = args[2] ? ((asxMetabufferInfo const *)args[2]) + invokeNo : NIL;
    
    if (subi)
    {
        asxBuffer base = subi->base;

        if (!base)
        {
            AFX_ASSERT(!base);
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &base);

        if (base->base)
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxUnit srcCap = AsxGetBufferCapacity(base, 0);
        afxUnit size = subi->range;
        afxSize from = subi->from;

        if (!AFX_TEST_ALIGNMENT(from, ASX_BUFFER_ALIGNMENT))
        {
            AFX_ASSERT_ALIGNMENT(from, ASX_BUFFER_ALIGNMENT);
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        AFX_ASSERT_RANGE(srcCap, from, size);

        if ((from >= srcCap) ||
            (size > srcCap) ||
            (size > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (size == 0)
            size = srcCap - from;
        else
            size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);

        buf->from = from;
        buf->base = base;
        buf->reqSiz = size;


        afxUnit bps = 0;
        switch (buf->fmt)
        {
        case asxFormat_M32f: bps = 32; break;
        case asxFormat_S32f: bps = 32; break;
        case asxFormat_M32i: bps = 32; break;
        case asxFormat_S32i: bps = 32; break;
        case asxFormat_M24i: bps = 24; break;
        case asxFormat_S24i: bps = 24; break;
        case asxFormat_M16i: bps = 16; break;
        case asxFormat_S16i: bps = 16; break;
        case asxFormat_M8i: bps = 8; break;
        case asxFormat_S8i: bps = 8; break;
        default: AfxThrowError();  break;
        }
        buf->fmt = subi->fmt;
        buf->fmtBps = bps;
        buf->fmtStride = bps / AFX_BYTE_SIZE;

        buf->usage = base->usage;
        buf->flags = base->flags | subi->flags;
        buf->sharingMask = base->sharingMask;

        buf->reqMemType = base->reqMemType;

        buf->tag = subi->tag;
        buf->udd = subi->udd;

        buf->storage[0] = base->storage[0];

        AfxReacquireObjects(1, &base);

        return err;
    }

    AFX_ASSERT(bufi && bufi->size && bufi->usage);
    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    buf->tag = bufi->tag;
    buf->udd = bufi->udd;

    afxUnit size = bufi->size;

    if (!size)
    {
        // If there is not a @base buffer, we need a size here.
        AfxThrowError();
        return err;
    }

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
    size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);

    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    buf->reqAlign = AFX_SIMD_ALIGNMENT;
    buf->reqSiz = size;
    buf->from = 0;
    buf->base = NIL;

    if (err) return err;
#ifdef ASX_VALIDATION_ENABLED
    if (!buf->base)
    {
        AFX_ASSERT(buf->from == 0);
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &base);
        AFX_ASSERT_ALIGNMENT(buf->from, ASX_BUFFER_ALIGNMENT);
    }
#endif

    buf->usage = bufi->usage;
    buf->flags = bufi->flags;
    

    afxUnit bps = 0;
    switch (bufi->fmt)
    {
    case asxFormat_M32f: bps = 32; break;
    case asxFormat_S32f: bps = 32; break;
    case asxFormat_M32i: bps = 32; break;
    case asxFormat_S32i: bps = 32; break;
    case asxFormat_M24i: bps = 24; break;
    case asxFormat_S24i: bps = 24; break;
    case asxFormat_M16i: bps = 16; break;
    case asxFormat_S16i: bps = 16; break;
    case asxFormat_M8i: bps = 8; break;
    case asxFormat_S8i: bps = 8; break;
    default: /*AfxThrowError();*/  break;
    }
    buf->fmt = bufi->fmt;
    buf->fmtBps = bps;
    buf->fmtStride = bps / AFX_BYTE_SIZE;


    afxUnit exuCnt = 16; // TODO Get it from SSYS
    buf->sharingMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        buf->sharingMask |= bufi->sharingMask & AFX_BITMASK(i);
    }

    // STORAGE
    buf->reqMemType = NIL;

    // binding
    buf->storage[0].mmu = 0;
    buf->storage[0].offset = 0;
    buf->storage[0].hostedAlloc.addr = NIL;

    buf->storage[0].mapPtr = NIL;
    buf->storage[0].mapOffset = 0;
    buf->storage[0].mapRange = 0;
    buf->storage[0].mapFlags = NIL;

    buf->storage[0].pendingRemap = 0;
    buf->storage[0].permanentlyMapped = !!bufi->mapped;

    if (buf->flags & asxBufferFlag_F)
    {
        if (!bufi->data) AfxThrowError();
        else
        {
            buf->storage[0].hostedAlloc.bytemap = bufi->data;
            buf->storage[0].mapPtr = buf->storage[0].hostedAlloc.bytemap;
            buf->storage[0].mapRange = buf->reqSiz;
        }
    }
    else
    {
        if (AfxAllocate(buf->reqSiz, buf->reqAlign, AfxHere(), (void**)&buf->storage[0].hostedAlloc.bytemap))
        {
            AfxThrowError();
        }
        else
        {
            buf->storage[0].mapPtr = buf->storage[0].hostedAlloc.bytemap;
            buf->storage[0].mapRange = buf->reqSiz;

            if (!bufi->data)
                AfxCopy(buf->storage[0].mapPtr, &rawData[44], AFX_MIN(sizeof(rawData), buf->reqSiz));
            else
                AfxCopy(buf->storage[0].mapPtr, bufi->data, bufi->dataSiz);
        }
    }
    return err;
}

_ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_SBUF,
    .name = "StepBuffer",
    .desc = "Step Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(asxBuffer)),
    .ctor = (void*)_AsxSbufCtorCb,
    .dtor = (void*)_AsxSbufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireBuffers(afxStepSystem ssys, afxUnit cnt, asxBufferInfo const infos[], asxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(buffers);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxSsysGetBufClass(ssys);
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { ssys, (void*)infos, NIL }))
        AfxThrowError();

    return err;
}
