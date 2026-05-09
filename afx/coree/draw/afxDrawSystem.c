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

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_INPUT_C
#define _AVX_BUFFER_C
#define _AVX_RASTER_C
#include "avxIcd.h"
#define _AUX_UX_C
#include "../ux/auxIcd.h"

_AVX _avxDdiDsys const* _AvxDsysGetDdi(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys->ddi;
}

_AVX afxMask _AvxDsysGetIoExuMask(afxDrawSystem dsys, afxMask* dedIoExuMask)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (dedIoExuMask) *dedIoExuMask = dsys->dedIoExuMask;
    return dsys->ioExuMask;
}

_AVX avxFeatures const* _AvxDsysGetReqFeatures(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return &dsys->requirements;
}

_AVX avxLimits const* _AvxDsysGetLimits(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys->limits;
}

_AVX afxClass const* _AvxDsysSW_GetDctxClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetDexuClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->dexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DEXU);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetFencClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->fencCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetQrypClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->qrypCls;
    AFX_ASSERT_CLASS(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetVinClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->vinCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetRasClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->rasCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetBufClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->bufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetSampClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->sampCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SAMP);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetPipClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->pipCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetCanvClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->canvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetShdClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->shadCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetLigaClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass *cls = &dsys->ligaCls;
    AFX_ASSERT_CLASS(cls, afxFcc_LIGA);
    return cls;
}

_AVX afxClass const* _AvxDsysSW_GetTxdClassCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxClass const* cls = &dsys->txdCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TXD);
    return cls;
}

_AVX afxError _AvxDsysSW_TransferCb(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask dsysIoExuMask = dsys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = dsys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (dsysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(dsysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && dsys->dedIoExuMask && (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (dsys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & dsys->dedIoExuMask))
                    {
                        if (!(dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (dsys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxDrawBridge dexu;
            if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AvxDexuTransferVideoMemory(dexu, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        // Try again
        if (err == afxError_TIMEOUT || err == afxError_BUSY)
            continue; // while

        if (err || queued)
            break; // while --- find bridges
    }
    return err;
}

_AVX afxError _AvxDsysSW_RemapBuffersCb(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_AVX afxError _AvxDsysSW_CohereMappedBuffersCb(afxDrawSystem dsys, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AvxDsysGetIoExuMask(dsys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxDrawBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, avxService_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AvxDexuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

AFX_DEFINE_STRUCT(avxMemoryReq)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

AFX_DEFINE_STRUCT(avxBufferStorage)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

AFX_DEFINE_STRUCT(avxRasterStorage)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

_AVX void _AvxGetRasterMemoryRequirements(afxDrawSystem dsys, avxRaster ras, avxMemoryReq* req)
{
    avxMemoryReq req2;
    req2.align = ras->reqAlign;
    req2.size = ras->reqSiz;
    req2.memType = ras->reqMemType;
    *req = req2;
}

_AVX void _AvxGetBufferMemoryRequirements(afxDrawSystem dsys, avxBuffer buf, avxMemoryReq* req)
{
    avxMemoryReq req2 = { 0 };
    req2.align = buf->reqAlign;
    req2.size = buf->reqSiz;
    req2.memType = buf->reqMemType;
}

_AVX void _GetImageSubresourceLayout(afxDrawSystem dsys, avxRaster ras, afxUnit lodIdx, afxUnit layerIdx, avxRasterArrangement* layout)
{
    avxFormat fmt = ras->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    avxRasterArrangement lay2 = { 0 };
    afxWarp whd = { ras->whd.w, ras->whd.h, ras->whd.d };
    afxBool is3d = !!AvxGetRasterFlags(ras, avxRasterFlag_3D);

    if (AvxGetRasterFlags(ras, avxRasterFlag_MULTISAMP))
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AFX_MAX(1, whd.w * 2);
            whd.h = AFX_MAX(1, whd.h * 2);
            if (is3d) whd.d = AFX_MAX(1, whd.d * 2);
        }

        afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);

        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
    else
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AFX_MAX(1, whd.w >> 1);
            whd.h = AFX_MAX(1, whd.h >> 1);
            if (is3d) whd.d = AFX_MAX(1, whd.d >> 1);
        }

        afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);

        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
}

_AVX afxError _AvxCommitBuffers(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, avxBuffer buffers[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];

        if (!buf->storage[0].host.addr)
        {
            AfxAllocate(buf->reqSiz, AVX_BUFFER_ALIGNMENT, AfxHere(), (void**)&buf->storage[0].host.addr);
        }
    }
}

_AVX afxError _AvxCommitRasters(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, avxRaster rasters[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRaster ras = rasters[i];

        if (!ras->storage[0].host.addr)
        {
            AfxAllocate(ras->reqSiz, ras->reqAlign, AfxHere(), (void**)&ras->storage[0].host.addr);
        }
    }
}

_AVX afxCmdId AvxCmdPageInRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);
_AVX afxCmdId AvxCmdPageOutRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);

_AVX afxCmdId _AvxCmdRegenerateMipmapsSIGMA(afxDrawContext dctx, afxFlags flags, afxUnit rasCnt, avxRaster rasters[])
{
    for (afxUnit rasIt = 0; rasIt < rasCnt; rasIt++)
    {
        avxRaster ras = rasters[rasIt];
        afxUnit mipLevels = ras->mipCnt;
        afxUnit mipWidth = ras->whd.w;
        afxUnit mipHeight = ras->whd.h;

        for (afxUnit i = 1; i < mipLevels; i++)
        {
            AvxCmdDeclareBarrier(dctx, avxBusStage_TRANSFER, NIL);

            // Set up blit
            avxRasterBlit blit =
            {
                .src.lodIdx = i - 1,
                .src.whd = { mipWidth, mipHeight, 1 },
                .dst.lodIdx = i,
                .dst.whd = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }
            };
            AvxCmdBlitRaster(dctx, ras, 1, &blit, ras, avxTexelFilter_LINEAR);

            // Update dimensions for next level
            mipWidth = mipWidth > 1 ? mipWidth / 2 : 1;
            mipHeight = mipHeight > 1 ? mipHeight / 2 : 1;
        }
    }
}

AFX_DEFINE_STRUCT(avxRasterAllocation)
{
    avxRaster   ras;
    afxUnit     iommu;
    afxSize     offset;
    afxFlags    flags;
    afxError    rslt;
};

AFX_DEFINE_STRUCT(avxBufferAllocation)
{
    avxBuffer   buf;
    afxUnit     iommu;
    afxSize     offset;
    afxFlags    flags;
    afxError    rslt;
};

_AVXINL afxError _AvxDsysSW_DeallocateRastersCb(afxDrawSystem dsys, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRaster ras = rasters[i];
        _avxRasStorage* bufs = &ras->storage[0];

        if (ras->flags & avxRasterFlag_FOREIGN)
        {
            bufs->host.bytemap = NIL;
            bufs->size = 0;
        }
        else
        {
            if (bufs->host.bytemap)
            {
                if (AfxDeallocate((void**)&bufs->host.bytemap, AfxHere()))
                {
                    AfxThrowError();
                }
            }
            bufs->size = 0;
        }
    }
    return err;
}

_AVXINL afxError _AvxDsysSW_AllocateRastersCb(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const infos[], avxRaster rasters[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterInfo const* info = &infos[i];
        avxRaster ras = rasters[i];
        _avxRasStorage* bufs = &ras->storage[0];

        if (ras->flags & avxRasterFlag_FOREIGN)
        {
            avxExorasterInfo const* info2 = (void*)info;
            //bufs->offset = info->from;
            AFX_ASSERT((!info2->resrvdS) || (info2->resrvdS && ras->reqSiz));
            bufs->host.bytemap = (afxByte*)info2->resrvdA;
            bufs->host.external = TRUE;
            bufs->size = info2->resrvdS;
        }
        else
        {
            if (AfxAllocate(ras->reqSiz, ras->reqAlign, AfxHere(), (void**)&bufs->host.bytemap))
            {
                AfxThrowError();
            }
            bufs->host.external = FALSE;
            bufs->size = ras->reqSiz;
        }
    }
    return err;
}

_AVXINL afxError _AvxDsysSW_DeallocateBuffersCb(afxDrawSystem dsys, afxUnit cnt, avxBuffer buffers[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];
        _avxBufStorage* bufs = &buf->storage[0];

        if (buf->flags & avxBufferFlag_F)
        {
            bufs->host.bytemap = NIL;
            bufs->size = 0;
        }
        else
        {
            if (bufs->host.bytemap)
            {
                if (AfxDeallocate((void**)&bufs->host.bytemap, AfxHere()))
                {
                    AfxThrowError();
                }
            }
            bufs->size = 0;
        }
    }
    return err;
}

_AVXINL afxError _AvxDsysSW_AllocateBuffersCb(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferInfo const* info = &infos[i];
        avxBuffer buf = buffers[i];
        _avxBufStorage* bufs = &buf->storage[0];

        if (buf->flags & avxBufferFlag_F)
        {
            //bufs->offset = info->from;
            AFX_ASSERT((!info->dataSiz) || (info->dataSiz && buf->reqSiz));
            bufs->host.bytemap = info->data;
            bufs->host.external = TRUE;
            bufs->size = info->dataSiz;
        }
        else
        {
            if (AfxAllocate(buf->reqSiz, buf->reqAlign, AfxHere(), (void**)&bufs->host.bytemap))
            {
                AfxThrowError();
            }
            bufs->host.external = FALSE;
            bufs->size = buf->reqSiz;
        }
    }
    return err;
}

_AVXINL void AvxCommitRasterStorage()
{

}

AFX_DEFINE_STRUCT(avxBufferPaging)
{
    avxBuffer       buf;
    afxSize         base;
    afxSize         range;
    //afxUnit         iommu;
    //afxSize         offset;
    afxFlags        flags;
    afxError        rslt;
};

AFX_DEFINE_STRUCT(avxRasterPaging)
{
    avxRaster       ras;
    avxRasterRegion rgn;
    //afxUnit         iommu;
    //afxSize         offset;
    afxFlags        flags;
    afxError        rslt;
};

/*
    void NamedBufferPageCommitmentEXT(uint buffer,
                                          intptr offset,
                                          sizeiptr size,
                                          boolean commit);
    void TexturePageCommitmentEXT(uint texture,
                                      int level,
                                      int xoffset,
                                      int yoffset,
                                      int zoffset,
                                      sizei width,
                                      sizei height,
                                      sizei depth,
                                      boolean commit);
*/

_AVX _avxDdiDsys const _AVX_DDI_DSYS =
{
    .fencCls = _AvxDsysSW_GetFencClassCb,
    .dexuCls = _AvxDsysSW_GetDexuClassCb,
    .qrypCls = _AvxDsysSW_GetQrypClassCb,
    .vtxdCls = _AvxDsysSW_GetVinClassCb,
    .rasCls = _AvxDsysSW_GetRasClassCb,
    .bufCls = _AvxDsysSW_GetBufClassCb,
    .sampCls = _AvxDsysSW_GetSampClassCb,
    .pipCls = _AvxDsysSW_GetPipClassCb,
    .canvCls = _AvxDsysSW_GetCanvClassCb,
    .shadCls = _AvxDsysSW_GetShdClassCb,
    .ligaCls = _AvxDsysSW_GetLigaClassCb,

    .txdCls = _AvxDsysSW_GetTxdClassCb,

    .getQrypRslt = _AvxDsysSW_GetQrypRsltCb,
    .resetQryp = _AvxDsysSW_ResetQrypCb,

    .transferCb = _AvxDsysSW_TransferCb,
    .cohereCb = _AvxDsysSW_CohereMappedBuffersCb,
    .remapCb = _AvxDsysSW_RemapBuffersCb,

    .allocRasCb = _AvxDsysSW_AllocateRastersCb,
    .deallocRasCb = _AvxDsysSW_DeallocateRastersCb,
    .allocBufCb = _AvxDsysSW_AllocateBuffersCb,
    .deallocBufCb = _AvxDsysSW_DeallocateBuffersCb,

    .waitFencCb = _AvxDsysSW_WaitForFencesCb,
};

_AVX afxModule AvxGetSystemIcd(afxDrawSystem dsys)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxModule icd = AfxGetHost(dsys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));
    return icd;
}

_AVX void AvxGetEnabledSystemFeatures(afxDrawSystem dsys, avxFeatures* features)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(features);
    *features = dsys->requirements;

    return;
}

_AVX avxClipSpaceDepth AvxGetSystemClipSpaceDepth(afxDrawSystem dsys, afxReal* rangeEpsilon)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (rangeEpsilon) *rangeEpsilon = dsys->clipSpaceDepthRangeEpsilon;
    return dsys->clipSpaceDepth;
}

_AVX afxUnit AvxTestForEnabledSystemExtensions(afxDrawSystem dsys, afxUnit cnt, afxString const* exts, afxBool enabled[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(exts);
    afxUnit rslt = 0;

    if (enabled)
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++, enabled[i] = TRUE;
            else
                enabled[i] = FALSE;
        }
    }
    else
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++;
        }
    }

    return rslt;
}

_AVX afxUnit AvxResolveSystemSymbols(afxDrawSystem dsys, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(names);
    // @addresses must be a recipient array for returned function pointers.
    AFX_ASSERT(addresses);
    afxUnit rslt = 0;

    AFX_ASSERT(dsys->ddi->getProcCb);
    rslt = dsys->ddi->getProcCb(dsys, cnt, names, addresses);

    return rslt;
}

_AVX afxUnit AvxGetDrawBridges(afxDrawSystem dsys, afxUnit baseIdx, afxUnit cnt, afxDrawBridge bridges[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit bridgeCnt = dsys->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxDrawBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxDrawBridge dexu = dsys->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        bridges[rslt++] = dexu;
    }
    return rslt;
}

_AVX afxUnit AvxChooseDrawBridges(afxDrawSystem dsys, afxUnit ddevId, avxService caps, afxMask exuMask, afxUnit first, afxUnit maxCnt, afxDrawBridge bridges[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((ddevId == AFX_INVALID_INDEX) && (!caps) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, dsys->bridgeCnt - first) : AFX_MIN(dsys->bridgeCnt, dsys->bridgeCnt - first);

    afxUnit bridgeCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < bridgeCnt; exuIdx++)
    {
        // skip if a mask is specified and this EXU is not bitmapped in it.
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxDrawBridge dexu = dsys->bridges[exuIdx];
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        if ((ddevId != AFX_INVALID_INDEX) && (ddevId != AfxGetObjectId(ddev)))
            continue;

        if (caps)
        {
            avxDeviceInfo capsi;
            AvxQueryDrawCapabilities(ddev, &capsi);
            
            if ((capsi.capabilities & caps) != caps)
                continue;
        }

        if (maxCnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = dexu;
            }

            if (maxCnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_AVX afxError AvxWaitForDrawQueue(afxDrawSystem dsys, afxUnit64 timeout, afxUnit exuIdx, afxUnit queId)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
    afxDrawBridge dexu;
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(dsys->bridgeCnt, exuIdx, 1);
    if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

    afxDrawQueue dque;
    if (!AvxGetDrawQueues(dexu, queId, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);

    if (AvxWaitForEmptyDrawQueue(dque, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError AvxWaitForDrawBridges(afxDrawSystem dsys, afxUnit64 timeout, afxMask exuMask)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxUnit exuCnt = dsys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(AFX_TEST_BIT_SET(exuMask, exuIdx)))
            continue;

        afxDrawBridge dexu;
        if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
        AvxWaitForIdleDrawBridge(dexu, timeout);
    }
    return err;
}

_AVX afxError AvxWaitForDrawSystem(afxDrawSystem dsys, afxUnit64 timeout)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (!dsys->ddi->waitCb)
    {
        afxUnit exuCnt = dsys->bridgeCnt;
        for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
        {
            afxDrawBridge dexu;
            if (!AvxGetDrawBridges(dsys, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                return err;
            }
            AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
            AvxWaitForIdleDrawBridge(dexu, timeout);
        }
    }
    else if (dsys->ddi->waitCb(dsys, timeout))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDsysDtorCb(afxDrawSystem dsys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);
    //AvxWaitForDrawSystem(dsys, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = dsys->bridgeCnt;

    AfxDeregisterChainedClasses(&dsys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
#if 0
    AfxAssertObjects(dsys->exuCnt, dsys->exus, afxFcc_DEXU);

    for (afxUnit j = dsys->exuCnt; j-- > 0;)
        while (!AfxDisposeObjects(1, &dsys->exus[j]));
#endif

    if (dsys->bridges)
    {
#if 0
        AFX_TRY_ASSERT_OBJECTS(afxFcc_DEXU, bridgeCnt, dsys->bridges);

        for (afxUnit i = bridgeCnt; i-- > 0;)
        {
            AfxDisposeObjects(1, &dsys->bridges[i]);
        }
#endif
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(dsys->bridges[0]),
            .var = (void**)&dsys->bridges
        }
    };

    if (AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDsysCtorCb(afxDrawSystem dsys, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(args[1]);
    _avxDsysAcq const* cfg = AFX_CAST(_avxDsysAcq const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    _avxDexuAcq* bridgeCfgs = cfg->bridgeCfg;

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    dsys->ddi = &_AVX_DDI_DSYS;
    dsys->idd = NIL;
    dsys->udd = cfg->cfg.udd;
    dsys->tag = cfg->cfg.tag;

    dsys->running = FALSE;

    dsys->limits = _AvxDdevGetLimits(bridgeCfgs[0].ddev);
    dsys->requirements = cfg->cfg.features;
    dsys->nonRhcs = cfg->cfg.nonRhcs;//ddev->leftHandedSpace;
    dsys->clipSpaceDepth = cfg->cfg.clipSpcDepth ? cfg->cfg.clipSpcDepth : avxClipSpaceDepth_ZERO_TO_ONE;//ddev->clipSpaceDepth;
    dsys->clipSpaceDepthRangeEpsilon = AFX_EPSILON;

    

    {
        afxChain* classes = &dsys->ctx.classes;
        AfxMakeChain(classes, (void*)dsys);

        // Must be first to be disponible at disposition of DSYS' child objects.
        afxClassConfig dexuClsCfg = cfg->dexuClsCfg ? *cfg->dexuClsCfg : _AVX_CLASS_CONFIG_DEXU;
        AFX_ASSERT(dexuClsCfg.fcc == afxFcc_DEXU);
        AfxMountClass(&dsys->dexuCls, NIL, classes, &dexuClsCfg);

        afxClassConfig dctxClsCfg = cfg->dctxClsCfg ? *cfg->dctxClsCfg : _AVX_CLASS_CONFIG_DCTX;
        AFX_ASSERT(dctxClsCfg.fcc == afxFcc_DCTX);
        AfxMountClass(&dsys->dctxCls, NIL, classes, &dctxClsCfg);

        afxClassConfig bufClsCfg = cfg->bufClsCfg ? *cfg->bufClsCfg : _AVX_CLASS_CONFIG_BUF;
        AFX_ASSERT(bufClsCfg.fcc == afxFcc_BUF);
        AfxMountClass(&dsys->bufCls, NIL, classes, &bufClsCfg);

        afxClassConfig rasClsCfg = cfg->rasClsCfg ? *cfg->rasClsCfg : _AVX_CLASS_CONFIG_RAS;
        AFX_ASSERT(rasClsCfg.fcc == afxFcc_RAS);
        AfxMountClass(&dsys->rasCls, NIL, classes, &rasClsCfg);

        afxClassConfig canvClsCfg = cfg->canvClsCfg ? *cfg->canvClsCfg : _AVX_CLASS_CONFIG_CANV;
        AFX_ASSERT(canvClsCfg.fcc == afxFcc_CANV);
        AfxMountClass(&dsys->canvCls, NIL, classes, &canvClsCfg); // req RAS

        afxClassConfig sampClsCfg = cfg->sampClsCfg ? *cfg->sampClsCfg : _AVX_CLASS_CONFIG_SAMP;
        AFX_ASSERT(sampClsCfg.fcc == afxFcc_SAMP);
        AfxMountClass(&dsys->sampCls, NIL, classes, &sampClsCfg);

        afxClassConfig ligaClsCfg = cfg->ligaClsCfg ? *cfg->ligaClsCfg : _AVX_CLASS_CONFIG_LIGA;
        AFX_ASSERT(ligaClsCfg.fcc == afxFcc_LIGA);
        AfxMountClass(&dsys->ligaCls, NIL, classes, &ligaClsCfg); // req BUF, RAS, SAMP

        afxClassConfig vtxdClsCfg = cfg->vtxdClsCfg ? *cfg->vtxdClsCfg : _AVX_CLASS_CONFIG_VIN;
        AFX_ASSERT(vtxdClsCfg.fcc == afxFcc_VIN);
        AfxMountClass(&dsys->vinCls, NIL, classes, &vtxdClsCfg);

        afxClassConfig shdClsCfg = cfg->shdClsCfg ? *cfg->shdClsCfg : _AVX_CLASS_CONFIG_CODB;
        AFX_ASSERT(shdClsCfg.fcc == afxFcc_SHD);
        AfxMountClass(&dsys->shadCls, NIL, classes, &shdClsCfg);

        afxClassConfig pipClsCfg = cfg->pipClsCfg ? *cfg->pipClsCfg : _AVX_CLASS_CONFIG_PIP;
        AFX_ASSERT(pipClsCfg.fcc == afxFcc_PIP);
        AfxMountClass(&dsys->pipCls, NIL, classes, &pipClsCfg); // req SHD, LIGA, VTXD

        afxClassConfig qrypClsCfg = cfg->qrypClsCfg ? *cfg->qrypClsCfg : _AVX_CLASS_CONFIG_QRYP;
        AFX_ASSERT(qrypClsCfg.fcc == afxFcc_QRYP);
        AfxMountClass(&dsys->qrypCls, NIL, classes, &qrypClsCfg);

        afxClassConfig txdClsCfg = cfg->txdClsCfg ? *cfg->txdClsCfg : _AVX_CLASS_CONFIG_TXD;
        AFX_ASSERT(txdClsCfg.fcc == afxFcc_TXD);
        AfxMountClass(&dsys->txdCls, NIL, classes, &txdClsCfg); // req RAS
#if 0
        afxClassConfig doutClsCfg;
        if (cfg->doutClsCfg) doutClsCfg = *cfg->doutClsCfg;
        else
        {
            doutClsCfg = _AVX_CLASS_CONFIG_DOUT;
            if (_AuxIcdGetInteropDoutClass(dsys, &AFX_STRING(""), &doutClsCfg))
            {
                doutClsCfg = _AVX_CLASS_CONFIG_DOUT;
            }
        }
        AFX_ASSERT(doutClsCfg.fcc == afxFcc_DOUT);
        AfxMountClass(&dsys->doutCls, NIL, classes, &doutClsCfg); // req RAS, CANV
#endif
    }

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->cfg.exuCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    dsys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(dsys->bridges[0]),
            .var = (void**)&dsys->bridges
        }
    };

    if (AfxAllocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
        return err;
    }

    if (_AvxDsysBridgeDevices(dsys, dsys->bridgeCnt, bridgeCfgs, dsys->bridges))
    {
        AfxThrowError();

        AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, dsys->bridgeCnt, dsys->bridges);

    dsys->ioExuMask = NIL;
    dsys->dedIoExuMask = NIL;
    dsys->gfxExuMask = NIL;
    dsys->cfxExuMask = NIL;
    dsys->dedCfxExuMask = NIL;
    dsys->videoExuMask = NIL;

    for (afxUnit i = 0; i < dsys->bridgeCnt; i++)
    {
        afxDrawBridge dexu;
        AvxGetDrawBridges(dsys, i, 1, &dexu);
        AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

        avxDeviceInfo capsi;
        AvxQueryDrawCapabilities(ddev, &capsi);

        if ((capsi.capabilities & avxService_DMA) == avxService_DMA)
            dsys->ioExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (avxService_DMA | avxService_PCX | avxService_GFX)) == avxService_DMA)
            dsys->dedIoExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & avxService_PCX) == avxService_PCX)
            dsys->cfxExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (avxService_PCX | avxService_GFX)) == avxService_PCX)
            dsys->dedCfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & avxService_GFX) == avxService_GFX)
            dsys->gfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & avxService_PRESENT) == avxService_PRESENT)
            dsys->videoExuMask |= AFX_BITMASK(i);
    }

    if (!err)
    {
        afxDrawDevice ddev = AvxGetBridgedDrawDevice(dsys->bridges[0], NIL);

        AfxCallDevice((afxDevice)ddev, 3, dsys);
        AfxCallDevice((afxDevice)ddev, 5, dsys);

        if (AfxDoDeviceService((afxDevice)ddev))
            AfxThrowError(); // let the device build its DPUs.
            
        if (!err)
        {
            {
                dsys->running = TRUE;
            }

        }
    }

    if (err)
    {
        AfxDeallocateInstanceData(dsys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&dsys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&dsys->ctx.classes));
    }

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DSYS =
{
    .fcc = afxFcc_DSYS,
    .name = "DrawSystem",
    .desc = "Draw I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawSystem)),
    .ctor = (void*)_AvxDsysCtorCb,
    .dtor = (void*)_AvxDsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxIcdConfigureDsysSW(afxModule avxIcd, avxSystemConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX));
    AFX_ASSERT(cfg);

    avxService caps = cfg->caps;
    afxAcceleration accel = cfg->accel;

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AVX_MAX_BRIDGES; i++)
        {
            afxUnit ddevId = i;

            afxDrawDevice ddev;
            if (AvxEnumerateDrawDevices(avxIcd, ddevId, 1, &ddev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

                avxDeviceInfo capsi;
                AvxQueryDrawCapabilities(ddev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].ddevId = ddevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
        }
    }
    else
    {
        afxUnit exuCnt = cfg->exuCnt;
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < exuCnt; i++)
        {
            avxDeviceInfo capsi = { 0 };
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;
            capsi.clipSpaceDepth = cfg->clipSpcDepth;
            capsi.nonRhcs = FALSE;

            afxUnit ddevId;
            if (AvxChooseDrawDevices(avxIcd, &capsi, NIL, NIL, 1, &ddevId))
            {
                afxDrawDevice ddev;
                AvxEnumerateDrawDevices(avxIcd, ddevId, 1, &ddev);
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

                AvxQueryDrawCapabilities(ddev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].ddevId = ddevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
#if 0
            else
            {
                cfg->exus[i].capabilities = caps.capabilities;
                cfg->exus[i].acceleration = caps.acceleration;
                cfg->exus[i].minQueCnt = caps.minQueCnt;
                cfg->exus[i].ddevId = NIL;
            }
#endif
        }
    }

    return err;
}

_AVX afxError AvxConfigureDrawSystem(afxModule avxIcd, avxSystemConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);

    if (!AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return err;
    }

    AFX_ASSERT(cfg);
    if (AfxFailed(_AvxGetDdi(avxIcd)->cfgDsysCb(avxIcd, cfg)))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AVX afxError _AvxIcdEstablishDsysSW(afxModule avxIcd, avxSystemConfig const* cfg, afxDrawSystem* system)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX));
    AFX_ASSERT(system);
    AFX_ASSERT(cfg);


    // Acquire bridges and queues
    afxUnit totalDqueCnt = 0;
    afxUnit baseQueIdx[AVX_MAX_BRIDGES] = { 0 };
    _avxDexuAcq bridgeCfg[AVX_MAX_BRIDGES] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(AVX_MAX_BRIDGES, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        avxBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable it to allow multibridge to same device port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalDqueCnt;
                totalDqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxDrawDevice ddev;
        if (!AvxEnumerateDrawDevices(avxIcd, exuCfg->ddevId, 1, &ddev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

            if (AfxCallDevice((afxDevice)ddev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].ddev = ddev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, AVX_MAX_QUEUES_PER_BRIDGE);
        avxDeviceInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].dqueClsCfg = &_AVX_CLASS_CONFIG_DQUE;
        //bridgeCfg[bridgeCnt].dctxClsCfg = &_AVX_CLASS_CONFIG_DCTX;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    _avxDsysAcq cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.cfg.clipSpcDepth = cfg->clipSpcDepth;
    cfg2.cfg.nonRhcs = cfg->nonRhcs;
    cfg2.cfg.exuCnt = bridgeCnt;
    cfg2.cfg.extCnt = cfg->extCnt;
    cfg2.cfg.exts = cfg->exts;
    cfg2.cfg.features = cfg->features;
    cfg2.cfg.udd = cfg->udd;
    cfg2.cfg.tag = cfg->tag;

    cfg2.bridgeCfg = &bridgeCfg[0];

    afxClass* dsysCls = (afxClass*)_AvxIcdGetDsysClass(avxIcd);
    AFX_ASSERT_CLASS(dsysCls, afxFcc_DSYS);

    afxDrawSystem dsys;
    if (AfxAcquireObjects(dsysCls, 1, (afxObject*)&dsys, (void const*[]) { avxIcd, &cfg2 }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    *system = dsys;

    return err;
}

_AVX afxError AvxEstablishDrawSystem(afxModule avxIcd, avxSystemConfig const* cfg, afxDrawSystem* system)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(system);
    AFX_ASSERT(cfg);

    if (!AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return err;
    }

    afxDrawSystem dsys = NIL;
    if (AfxFailed(_AvxGetDdi(avxIcd)->acqDsysCb(avxIcd, cfg, &dsys)))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    *system = dsys;

    return err;
}

_AVX afxUnit AvxInvokeDrawSystems(afxModule avxIcd, afxUnit first, void *udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    if (!AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return rslt;
    }

    afxClass const* cls = _AvxIcdGetDsysClass(avxIcd);
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);

    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_AVX afxUnit AvxEvokeDrawSystems(afxModule avxIcd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(systems);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    if (!AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return rslt;
    }

    afxClass const* cls = _AvxIcdGetDsysClass(avxIcd);
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);

    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, rslt, systems);

    return rslt;
}

_AVX afxUnit AvxEnumerateDrawSystems(afxModule avxIcd, afxUnit first, afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &avxIcd);
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    if (!AfxTestModule(avxIcd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return rslt;
    }

    afxClass const* cls = _AvxIcdGetDsysClass(avxIcd);
    AFX_ASSERT_CLASS(cls, afxFcc_DSYS);

    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, rslt, systems);

    return rslt;
}
