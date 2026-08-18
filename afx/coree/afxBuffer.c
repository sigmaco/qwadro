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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of SIGMA Future Storage.

#define _AFX_CORE_C
#define _AFX_BUFFER_C
#include "afxSystemDDK.h"

#define _AFX_BUFFER_HOSTSIDE_ALWAYS_FULLY_MAPPED TRUE

_AFX afxIommu AfxGetBufferHost(afxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    afxIommu iom = AfxGetHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    return iom;
}

_AFX afxSize AfxGetBufferAddress(afxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return buf->storage[0].host.addr + buf->storageOffset;
}

_AFX afxSize AfxGetBufferCapacity(afxBuffer buf, afxSize from)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return AFX_MIN(buf->reqSiz, buf->reqSiz - from);
}

_AFX afxBufferUsage AfxGetBufferUsage(afxBuffer buf, afxBufferUsage mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (!mask) ? buf->usage : (buf->usage & mask);
}

_AFX afxBufferFlags AfxGetBufferFlags(afxBuffer buf, afxBufferFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    return (!mask) ? buf->flags : (buf->flags & mask);
}

_AFXINL void _AfxSanitizeBufferCopy(afxBuffer buf, afxBuffer src, afxUnit cnt, afxBufferCopy const raw[], afxBufferCopy san[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize bufCap = buf->reqSiz;
    afxSize srcBufCap = src->reqSiz;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferCopy* s = &san[i];
        afxBufferCopy const* r = &raw[i];
        s->srcOffset = AFX_MIN(r->srcOffset, srcBufCap - 1);
        s->dstOffset = AFX_MIN(r->dstOffset, bufCap - 1);
        s->range = AFX_CLAMP(r->range, 1, AFX_MIN(srcBufCap - s->srcOffset, bufCap - s->dstOffset));
    }
}

_AFXINL void _AfxSanitizeBufferIo(afxBuffer buf, afxBuffer src, afxUnit cnt, afxBufferIo const raw[], afxBufferIo san[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &src);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize bufCap = buf->reqSiz;
    afxSize srcBufCap = src->reqSiz;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferIo* s = &san[i];
        afxBufferIo const* r = &raw[i];
        s->srcOffset = AFX_MIN(r->srcOffset, srcBufCap - 1);
        s->dstOffset = AFX_MIN(r->dstOffset, bufCap - 1);
        s->srcStride = AFX_MIN(r->srcStride, srcBufCap - s->srcOffset);
        s->dstStride = AFX_MIN(r->dstStride, bufCap - s->dstOffset);
        s->rowCnt = AFX_CLAMP(r->rowCnt, 1, AFX_MIN(r->rowCnt - srcBufCap / s->srcStride, r->rowCnt - bufCap / s->dstStride));
    }
}

////////////////////////////////////////////////////////////////////////////////

_AFX void* AfxGetBufferMap(afxBuffer buf, afxSize offset, afxUnit range)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxSize mappedOffset = buf->storage[0].mapOffset;
    afxSize mappedRange = buf->storage[0].mapRange;

    // Validate if the requested region is within the currently mapped region
    if ((offset < mappedOffset) || ((offset + range) > (mappedOffset + mappedRange)))
        return NIL;

    // If the region is valid, set the placeholder to the corresponding address
    AFX_ASSERT(mappedOffset + mappedRange >= offset + range);
    AFX_ASSERT(mappedOffset <= offset);    
    afxSize offDiff = offset - mappedOffset;

    AFX_ASSERT(buf->storage[0].mapPtr);
    return &buf->storage[0].mapPtr[offDiff];
}

_AFX afxError AfxUnmapBuffer(afxBuffer buf, afxBool wait)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_RW));

    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    afxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = buf->storage[0].mapOffset;
    map.range = buf->storage[0].mapRange;
    map.flags = buf->storage[0].mapFlags;

    if (AfxUnmapBuffers(iom, 1, &map))
        AfxThrowError();

    return err;
}

_AFX afxError AfxMapBuffer(afxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, void** placeholder)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AfxGetBufferFlags(buf, afxBufferFlag_RW));
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AFX_BUFFER_ALIGNMENT));

    afxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    void* holder;
    if (AfxMapBuffers(iom, 1, &map, (void**[]) { &holder }))
        AfxThrowError();

    // TODO(?): If no placeholder is specified, do not wait for completion.

    AFX_ASSERT(placeholder);
    *placeholder = holder;

    return err;
}

_AFX afxError AfxCohereMappedBuffer(afxBuffer buf, afxSize offset, afxUnit range, afxFlags flags, afxBool invalidate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(buf->reqSiz, offset, range);
    AFX_ASSERT(AFX_TEST_ALIGNMENT(offset, AFX_BUFFER_ALIGNMENT));

    afxBufferedMap map = { 0 };
    map.buf = buf;
    map.offset = offset;
    map.range = range;
    map.flags = flags;

    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    if (AfxCohereMappedBuffers(iom, invalidate, 1, &map))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxBufDtorCb(afxBuffer buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    while (buf->storage[0].mapPtr)
    {
        AfxUnmapBuffer(buf, TRUE);
        AfxYield();
        //AFX_ASSERT(!buf->bytemap);
    }

    if (buf->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf->base);
        AfxDisposeObjects(1, &buf->base);
    }

    //afxIommu iom = AfxGetBufferHost(buf);
    if (_AfxDsysGetDdi(iom)->deallocBufCb(iom, 1, &buf))
    {
        AfxThrowError();
    }

    return err;
}

_AFX afxError _AfxBufCtorCb(afxBuffer buf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxIommu iom = AfxGetBufferHost(buf);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxBufferInfo const* bufi = args[1] ? ((afxBufferInfo const*)args[1]) + invokeNo : NIL;
    AFX_ASSERT(bufi && bufi->size && bufi->usage);
    afxMetabufferInfo const* sub = args[2] ? ((afxMetabufferInfo const*)args[2]) + invokeNo : NIL;

    buf->tag = bufi->tag;
    buf->udd = bufi->udd;

    if (sub)
    {
        afxBuffer base = sub->buf;
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &base);

        if (base->base && (base->base != base))
        {
            // Base buffer must be a storage buffer, not a buffer view.
            AFX_ASSERT(!base->base);
            AfxThrowError();
            return err;
        }

        afxSize from = sub->bufBase;
        afxSize range = sub->bufRange;
        afxSize srcCap = AfxGetBufferCapacity(base, 0);
        AFX_ASSERT_RANGE(srcCap, from, range);

        if (!AFX_TEST_ALIGNMENT(from, AFX_BUFFER_ALIGNMENT))
        {
            AFX_ASSERT_ALIGNMENT(from, AFX_BUFFER_ALIGNMENT);
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        // If a capacity is not specified, the new buffer inherits the full capacity of the base buffer, 
        // excluding the portion displaced by @from.
        if (range == 0)
            range = srcCap - from;
        else
            range = AFX_ALIGN_SIZE(range, AFX_SIMD_ALIGNMENT);

        // As every buffer capacity is a power of AFX_SIMD_ALIGNMENT, it should already be aligned here.
        AFX_ASSERT_ALIGNMENT(range, AFX_SIMD_ALIGNMENT);

        if ((from >= srcCap) ||
            (range > srcCap) ||
            (range > srcCap - from))
        {
            AfxThrowError();
            err = afxError_OUT_OF_RANGE;
            return err;
        }

        buf->reqSiz = range;
        buf->fmt = sub->fmt;

        buf->usage = base->usage;
        buf->flags = base->flags;
        buf->exuMask = base->exuMask;

        buf->storage[0] = base->storage[0];
        buf->storageOffset = base->storageOffset;

        if (buf->usage & afxBufferUsage_FETCH)
        {
            if (!buf->fmt)
            {
                AFX_ASSERT(buf->fmt);
                AfxThrowError();
                return err;
            }
        }

        AfxReacquireObjects(1, &base);

        return err;
    }

    buf->base = NIL;
    buf->from = 0;
    buf->fmt = bufi->fmt;

    if (!bufi) AfxThrowError();
    else if (!bufi->usage)
        AfxThrowError();

    buf->usage = bufi->usage;
    buf->flags = bufi->flags;

    if (buf->usage & afxBufferUsage_FETCH)
    {
        if (!buf->fmt)
        {
            AFX_ASSERT(buf->fmt);
            AfxThrowError();
            return err;
        }
    }

    afxSize size = bufi->size;

    if (!size)
    {
        // If there is not a @base buffer, we need a size here.
        AfxThrowError();
        return err;
    }
    // If there is not a @base buffer, @from is useless and should be zero.
    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    // All buffer mapping requires ranges aligned to AFX_SIMD_ALIGNMENT. This alignment is ensured at AFX level.
    size = AFX_ALIGN_SIZE(size, AFX_SIMD_ALIGNMENT);
    // Buffer capacity must be always aligned to AFX_SIMD_ALIGNMENT for a correct mapping behavior.
    AFX_ASSERT_ALIGNMENT(size, AFX_SIMD_ALIGNMENT);
    buf->reqSiz = size;

    if (err) return err;

    afxUnit exuCnt = 16; // TODO Get it from IOM
    buf->exuMask = NIL;
    for (afxUnit i = 0; i < exuCnt; i++)
    {
        buf->exuMask |= bufi->exuMask & AFX_BITMASK(i);
    }

    // STORAGE
    buf->reqMemType = NIL;
    buf->reqAlign = AFX_BUFFER_ALIGNMENT;

    // binding
    buf->storage[0].mmu = 0;
    buf->storageOffset = 0;
    buf->storage[0].host.addr = NIL;

    buf->storage[0].mapPtr = NIL;
    buf->storage[0].mapOffset = 0;
    buf->storage[0].mapRange = 0;
    buf->storage[0].mapFlags = NIL;
    buf->storage[0].mapRefCnt = 0;

    buf->storage[0].pendingRemap = 0;
    buf->storage[0].permanentlyMapped = !!bufi->mapped;

    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_BUF =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .desc = "I/O Memory Buffer", // AFX Buffer
    .fixedSiz = sizeof(AFX_OBJECT(afxBuffer)),
    .ctor = (void*)_AfxBufCtorCb,
    .dtor = (void*)_AfxBufDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireBuffers(afxIommu iom, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    AFX_ASSERT(cnt);
    AFX_ASSERT(infos);
    AFX_ASSERT(buffers);
    if (!cnt || !infos || !buffers)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferInfo const* info = &infos[i];

        if (!info->usage)
        {
            AFX_ASSERT(info->usage);
            AfxThrowError();
        }

        if (!info->size)
        {
            AFX_ASSERT(!info->size);
            AfxThrowError();
        }
    }
    
    if (err) return err;

    afxClass* cls = (afxClass*)_AfxIomGetDdi(iom)->bufCls(iom);
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { iom, (void*)infos, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);

    if (_AfxDsysGetDdi(iom)->allocBufCb(iom, cnt, infos, buffers))
    {
        AfxDisposeObjects(cnt, buffers);
        AfxThrowError();
        return err;
    }

    if (err) return err;

#ifdef AFX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        afxBufferInfo const* bufi = &infos[i];

        AFX_ASSERT(buf->reqSiz >= bufi->size);
        AFX_ASSERT((buf->flags & bufi->flags) == bufi->flags);
        AFX_ASSERT(buf->exuMask == bufi->exuMask);
        AFX_ASSERT(buf->udd == bufi->udd);
        AFX_ASSERT((buf->usage & bufi->usage) == bufi->usage);
        AFX_ASSERT(buf->tag.start == bufi->tag.start);
    }
#endif

    // Proceed to permanently map and/or upload initial data.
    // We can't do it in constructor callback because ICD callbacks are called after the AFX one,
    // so it would be never ready to be mapped at device side.

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        afxBufferInfo const* bufi = &infos[i];

        if (bufi->mapped)
        {
            buf->storage[0].permanentlyMapped = TRUE;

            void* ptr;
            if (AfxMapBuffer(buf, 0, buf->reqSiz, NIL, &ptr))
            {
                AfxThrowError();
                break;
            }
            AFX_ASSERT(ptr == buf->storage[0].mapPtr);

            if (bufi->dataSiz)
            {
                AFX_ASSERT(bufi->data);
                AFX_ASSERT(ptr == buf->storage[0].mapPtr);
                AfxCopy(ptr, bufi->data, bufi->dataSiz);

                // Unmapping should do nothing in a persistently mapped buffer.
                // But we will keep it here for good didactic reasons.
                AfxUnmapBuffer(buf, 0);
            }
        }
        else
        {
            if (bufi->dataSiz)
            {
                AFX_ASSERT(bufi->data);
                afxBufferIo iop = { 0 };
                iop.srcStride = 1;
                iop.dstStride = 1;
                iop.rowCnt = bufi->dataSiz;
                if (AfxUpdateBuffer(buf, 1, &iop, bufi->data, 0, NIL))
                {
                    AfxThrowError();
                    break;
                }
            }
        }
    }

    if (err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);
        AfxDisposeObjects(cnt, buffers);
    }

    return err;
}

_AFX afxError AfxReacquireBuffers(afxIommu iom, afxUnit cnt, afxMetabufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    AFX_ASSERT(cnt);
    AFX_ASSERT(infos);
    AFX_ASSERT(buffers);
    if (!cnt || !infos || !buffers)
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMetabufferInfo const* info = &infos[i];

        if (!info->buf)
        {
            AFX_ASSERT(info->buf);
            AfxThrowError();
        }
    }

    if (err) return err;

    afxClass* cls = (afxClass*)_AfxIomGetDdi(iom)->bufCls(iom);
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)buffers, (void const*[]) { iom, NIL, (void*)infos }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_BUF, cnt, buffers);

#ifdef AFX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        afxMetabufferInfo const* bufi = &infos[i];

        AFX_ASSERT(buf->base == bufi->buf);
        AFX_ASSERT(buf->reqSiz >= bufi->bufRange);
        AFX_ASSERT((buf->flags & bufi->flags) == bufi->flags);
        AFX_ASSERT(buf->fmt == bufi->fmt);
        AFX_ASSERT(buf->from == bufi->bufBase);
        AFX_ASSERT(buf->udd == bufi->udd);
        AFX_ASSERT(buf->tag.start == bufi->tag.start);
    }
#endif

    return err;
}

_AFXINL afxError _AfxIomSW_DeallocateBuffersCb(afxIommu iom, afxUnit cnt, afxBuffer buffers[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBuffer buf = buffers[i];
        _afxBufMem* bufs = &buf->storage[0];

        if (buf->flags & afxBufferFlag_F)
        {
            bufs->host.bytemap = NIL;
            bufs->size = 0;
        }
        else
        {
            if (bufs->host.bytemap)
            {
                if (AfxDeallocate(AfxHere(), (void**)&bufs->host.bytemap))
                {
                    AfxThrowError();
                }
            }
            bufs->size = 0;
        }
    }
    return err;
}

_AFXINL afxError _AfxIomSW_AllocateBuffersCb(afxIommu iom, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxBufferInfo const* info = &infos[i];
        afxBuffer buf = buffers[i];
        _afxBufMem* bufs = &buf->storage[0];

        if (buf->flags & afxBufferFlag_F)
        {
            //bufs->offset = info->from;
            AFX_ASSERT((!info->dataSiz) || (info->dataSiz && buf->reqSiz));
            bufs->host.bytemap = info->data;
            bufs->host.external = TRUE;
            bufs->size = info->dataSiz;
        }
        else
        {
            if (AfxAllocate(AfxHere(), buf->reqSiz, buf->reqAlign, (void**)&bufs->host.bytemap))
            {
                AfxThrowError();
            }
            bufs->host.external = FALSE;
            bufs->size = buf->reqSiz;
        }
    }
    return err;
}
