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

#define _AVX_DRAW_C
#define _AVX_CANVAS_C
#include "../icd/avxIcd.h"

_AVX afxDrawSystem AvxGetCanvasHost(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxDrawSystem dsys = AfxGetHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX avxCanvasFlags AvxGetCanvasFlags(avxCanvas canv, avxCanvasFlags mask)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    return (!mask) ? canv->flags : (canv->flags & mask);
}

_AVX afxUnit AvxGetCanvasExtent(avxCanvas canv, avxOrigin const* origin, afxLayeredRect* extent)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    avxExtent extent2 = canv->extent;
    avxOrigin origin2 = (avxOrigin) { 0 };

    if (origin)
    {
        origin2 = *origin;
        origin2 = AvxClampOrigin(origin2, AVX_RANGE(extent2.w - 1, extent2.h - 1, extent2.d - 1));
        extent2 = AvxSubtractRange(extent2, AVX_RANGE(origin2.x, origin2.y, origin2.z));
    }
    
    AFX_ASSERT(extent);
    *extent = AFX_LAYERED_RECT(origin2.x, origin2.y, origin2.z, extent2.w, extent2.h, extent2.d);
    return extent2.w * extent2.h;
}

_AVX afxUnit AvxClipCanvas_(avxCanvas canv, afxRect const* rc, afxBool flipY, avxViewport* vp)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(rc);

    avxExtent extent = canv->extent;
    afxRect rcClip =
    {
        .x = AFX_MIN(rc->x, (afxInt)extent.w - 1),
        .y = AFX_MIN(rc->y, (afxInt)extent.h - 1),
        .w = rc->w ? AFX_MIN(rc->w, (afxInt)extent.w - rcClip.x) : (afxInt)extent.w - rcClip.x,
        .h = rc->h ? AFX_MIN(rc->h, (afxInt)extent.h - rcClip.y) : (afxInt)extent.h - rcClip.y
    };

    if (flipY)
        rcClip = AfxGetFlippedRect(&rcClip, extent.h);

    vp->origin[0] = AFX_MIN(rc->x, (afxInt)extent.w - 1);
    vp->origin[1] = AFX_MIN(rc->y, (afxInt)extent.h - 1);
    vp->extent[0] = AFX_MAX(rc->w, AFX_MIN(rc->x, (afxInt)extent.w - 1));
    vp->extent[1] = AFX_MAX(rc->h, AFX_MIN(rc->y, (afxInt)extent.h - 1));
    vp->minDepth = 0;
    vp->maxDepth = 1;
    return (vp->extent[0] * vp->extent[1]);
}

_AVX afxUnit AvxClipCanvas2_(avxCanvas canv, afxRect const* rc, afxBool flipY, avxViewport* vp)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(rc);

    avxExtent extent = canv->extent;
    afxRect rcClip =
    {
        .x = AFX_MIN(rc->x, (afxInt)extent.w - 1),
        .y = AFX_MIN(rc->y, (afxInt)extent.h - 1),
        .w = rc->w ? AFX_MIN(rc->w, extent.w - rcClip.x) : (afxInt)extent.w - rcClip.x,
        .h = rc->h ? AFX_MIN(rc->h, extent.h - rcClip.y) : (afxInt)extent.h - rcClip.y
    };

    if (flipY)
        rcClip = AfxGetFlippedRect(&rcClip, extent.h);

    vp->origin[0] = AFX_MIN(rc->x, (afxInt)extent.w - 1);
    vp->origin[1] = AFX_MIN(rc->y, (afxInt)extent.h - 1);
    vp->extent[0] = AFX_MAX(rc->w, AFX_MIN(rc->x, (afxInt)extent.w - 1));
    vp->extent[1] = AFX_MAX(rc->h, AFX_MIN(rc->y, (afxInt)extent.h - 1));
    vp->minDepth = 0;
    vp->maxDepth = 1;
    return (vp->extent[0] * vp->extent[1]);
}

_AVX avxExtent AvxGetCanvasExtentNdc(avxCanvas canv, afxV2d const origin, afxV2d const extent)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxV2d at, ran;
    AfxV2dNdc(at, origin, AFX_V2D(canv->extent.w, canv->extent.h));
    AfxV2dNdc(ran, extent, AFX_V2D(canv->extent.w, canv->extent.h));
    return AVX_RANGE(ran[0], ran[1], canv->extent.d);
}

_AVX afxUnit AvxQueryCanvasRigs(avxCanvas canv, afxUnit* colRigCnt, afxUnit* dRigIdx, afxUnit* sRigIdx)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    if (colRigCnt)
        *colRigCnt = canv->colorCnt;

    if (dRigIdx)
        *dRigIdx = canv->dsRigIdx[0];

    if (sRigIdx)
        *sRigIdx = canv->dsRigIdx[1];

    return canv->rigCnt;
}

_AVX afxUnit AvxGetCanvasBuffers(avxCanvas canv, afxUnit baseRigIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->rigCnt, baseRigIdx, cnt);
    if (baseRigIdx >= canv->rigCnt) return rslt;
    //baseRigIdx = AFX_MIN(baseRigIdx, canv->rigCnt - 1);
    cnt = AFX_MIN(cnt, canv->rigCnt - baseRigIdx);

    AFX_ASSERT(rasters);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit rigIdx = baseRigIdx + i;
        avxRaster ras = canv->rigs[rigIdx].ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            ++rslt;
        }
        rasters[i] = ras;
    }
    return rslt;
}

_AVX afxUnit AvxGetColorBuffers(avxCanvas canv, afxUnit baseRigIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    afxUnit rslt = 0;

    AFX_ASSERT_RANGE(canv->colorCnt, baseRigIdx, cnt);
    if (baseRigIdx >= canv->colorCnt) return rslt;
    //baseRigIdx = AFX_MIN(baseRigIdx, canv->colorCnt - 1);
    cnt = AFX_MIN(cnt, canv->colorCnt - baseRigIdx);

    rslt = AvxGetCanvasBuffers(canv, baseRigIdx, cnt, rasters);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, rslt, rasters);

    return rslt;
}

_AVX afxUnit AvxGetDepthBuffers(avxCanvas canv, avxRaster* depth, avxRaster* stencil)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT(depth || stencil);
    afxBool rslt = 0;

    afxUnit dRigIdx = AFX_INVALID_INDEX;
    afxUnit sRigIdx = AFX_INVALID_INDEX;    
    if (!AvxQueryCanvasRigs(canv, NIL, &dRigIdx, &sRigIdx))
        return rslt;

    if (depth)
    {
        avxRaster d = NIL;
        if ((dRigIdx != AFX_INVALID_INDEX) && AvxGetCanvasBuffers(canv, dRigIdx, 1, &d))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &d);
            *depth = d;
            ++rslt;
        }
        else *depth = NIL;
    }

    if (stencil)
    {
        avxRaster s = NIL;
        if ((sRigIdx != AFX_INVALID_INDEX) && AvxGetCanvasBuffers(canv, sRigIdx, 1, &s))
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &s);
            *stencil = s;
            ++rslt;
        }
        else *stencil = NIL;
    }
    return rslt;
}

_AVX afxError _AvxCanvRelinkDrawBuffersCb_SW(avxCanvas canv, afxBool regen, afxUnit baseRig, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->rigCnt, baseRig, cnt);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxDrawSystem dsys = AvxGetCanvasHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    for (afxUnit idx = 0; idx < cnt; idx++)
    {
        afxUnit rigIdx = baseRig + idx;
        _avxCanvasRig* rig = &canv->rigs[rigIdx];
        avxRaster curr = rig->ras;
        avxRaster ras = rasters ? rasters[idx] : NIL;

        // If current is NIL, we must proceed to managed generation.
        if ((ras == curr) && (curr)) continue;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            rig->ras = NIL;
            --canv->linkedCnt;
        }
        else if (regen)
        {
            //canv->rigs[rigIdx].managed = TRUE;

            avxRasterInfo rasi = { 0 };
            rasi.usage = rig->usage | avxRasterUsage_DRAW;
            rasi.flags = rig->flags;
            rasi.fmt = rig->fmt;
            rasi.extent.w = AFX_ALIGN_SIZE(canv->extent.w, 16); // aligned to tile
            rasi.extent.h = AFX_ALIGN_SIZE(canv->extent.h, 16); // aligned to tile
            rasi.extent.d = canv->extent.d;

            if (1 < (rasi.lodCnt = rig->lodCnt))
                rasi.flags |= avxRasterFlag_MULTISAMP;

            if (AvxAcquireRasters(dsys, 1, &rasi, &ras))
            {
                AfxThrowError();
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            }
        }

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            if (rig->usage != AvxGetRasterUsage(ras, rig->usage))
            {
                AfxThrowError();
                continue;
            }

            if (rig->fmt != AvxGetRasterFormat(ras))
            {
                AfxThrowError();
                continue;
            }

            avxExtent ext = AvxGetRasterExtent(ras, 0);
            // Each buffer must be at least as large as the canvas dimensions.
            if ((ext.w < canv->extent.w) && (ext.h < canv->extent.h) && (ext.d < canv->extent.d))
            {
                AfxThrowError();
                continue;
            }

            if (!regen)
            {
                AfxReacquireObjects(1, &ras);
                //rig->managed = !!managed;
            }
            ++canv->linkedCnt;
        }
        rig->ras = ras;
    }
    return err;
}

_AVX afxError _AvxRigdDrawBuffers(avxCanvas canv, afxUnit baseRig, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);
    AFX_ASSERT_RANGE(canv->rigCnt, baseRig, cnt);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    if (!canv->ddi->relink)
    {
        // canv->pimpl->bind(canv, FASE, baseRig, cnt, rasters);
        if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, baseRig, cnt, rasters))
            AfxThrowError();
    }
    else if (canv->ddi->relink(canv, TRUE, baseRig, cnt, rasters))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxRigdDepthBuffer(avxCanvas canv, avxRaster depth)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &depth);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit rigIdx;
    if (!AvxQueryCanvasRigs(canv, NIL, &rigIdx, NIL) || (rigIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxRigdDrawBuffers(canv, rigIdx, 1, &depth))
        AfxThrowError();

    return err;
}

_AVXINL afxError _AvxRigdStencilBuffer(avxCanvas canv, avxRaster stencil)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &stencil);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    afxUnit rigIdx;
    if (!AvxQueryCanvasRigs(canv, NIL, NIL, &rigIdx) || (rigIdx == AFX_INVALID_INDEX)) AfxThrowError();
    else if (_AvxRigdDrawBuffers(canv, rigIdx, 1, &stencil))
        AfxThrowError();

    return err;
}

_AVX afxError AvxPrintCanvasBuffer(avxCanvas canv, afxUnit rigIdx, avxRasterIo const* op, afxUri const* uri, afxMask exuMask)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    AFX_ASSERT_RANGE(canv->rigCnt, rigIdx, 1);

    if (canv->flags & avxCanvasFlag_VOID)
    {
        AfxThrowError();
        return err;
    }

    avxRaster ras;
    if (!AvxGetCanvasBuffers(canv, rigIdx, 1, &ras))
    {
        AfxThrowError();
        return err;
    }

    if (op)
    {
        AFX_ASSERT_RANGE((afxInt)canv->extent.w, op->rgn.origin.x, 1);
        AFX_ASSERT_RANGE((afxInt)canv->extent.h, op->rgn.origin.y, 1);
        AFX_ASSERT_RANGE((afxInt)canv->extent.d, op->rgn.origin.z, 1);
        AFX_ASSERT2(canv->extent.w >= op->rgn.extent.w, op->rgn.extent.w);
        AFX_ASSERT2(canv->extent.h >= op->rgn.extent.h, op->rgn.extent.h);
        AFX_ASSERT2(canv->extent.d >= op->rgn.extent.d, op->rgn.extent.d);
            
        if (AvxPrintRaster(ras, op, 1, uri, exuMask))
            AfxThrowError();
    }
    else
    {
        afxLayeredRect bounds;
        avxRasterIo op2 = { 0 };
        AvxGetCanvasExtent(canv, NIL, &bounds);
        op2.rgn.origin = (avxOrigin) { bounds.area.x, bounds.area.y, bounds.baseLayer };
        op2.rgn.extent = (avxExtent){ bounds.area.w, bounds.area.h, bounds.layerCnt };

        if (AvxPrintRaster(ras, &op2, 1, uri, exuMask))
            AfxThrowError();
    }
    return err;
}

#if 0
_AVX afxError _AvxRedoDrawBuffers(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxDrawSystem dsys = AvxGetCanvasHost(canv);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    avxRasterInfo rasi = { 0 };
    rasi.lodCnt = 1;
    rasi.extent = AvxGetCanvasExtent(canv, AVX_ORIGIN_ZERO);

    for (afxUnit i = 0; i < canv->rigCnt; i++)
    {
        _avxCanvasRig* surf = &canv->rigs[i];

        if (!surf->managed)
            continue;

        if (surf->ras)
            AfxRigdDrawBuffers(canv, i, 1, NIL);

        rasi.fmt = surf->fmt;
        rasi.lodCnt = surf->sampleCnt;
        rasi.usage = surf->usage | avxRasterUsage_DRAW;
        avxRaster ras;

        if (AvxAcquireRasters(dsys, 1, &rasi, &ras))
        {
            AfxThrowError();
            continue;
        }

        if (!canv->relink)
        {
            if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, i, 1, &ras))
                AfxThrowError();
        }
        else if (canv->relink(canv, TRUE, i, 1, &ras))
            AfxThrowError();

        AfxDisposeObjects(1, &ras);
    }
    return err;
}
#endif

_AVX afxError _AvxCanvDtorCb(avxCanvas canv)
{
    afxError err = { 0 };
    // @canv must be a valid avxCanvas handle.
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    for (afxUnit i = 0; i < canv->rigCnt; i++)
    {
        _avxCanvasRig* rig = &canv->rigs[i];        
        _AvxCanvRelinkDrawBuffersCb_SW(canv, FALSE, i, 1, NIL);
        AFX_ASSERT(!rig->ras);
#if 0
        avxRaster ras = rig->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AfxDisposeObjects(1, &ras);
            canv->rigs[i].ras = NIL;
        }
#endif
    }

    afxAllocation const stashs[] =
    {
        AFX_ALLOCATION(canv->rigCnt, sizeof(canv->rigs[0]), 0, &canv->rigs)
    };
    if (AfxFailed(AfxDeallocateInstanceData(canv, ARRAY_SIZE(stashs), stashs)))
        AfxThrowError();

    return err;
}

_avxCanvDdi const _AVX_CANV_DDI =
{
    .relink = _AvxCanvRelinkDrawBuffersCb_SW,
};

_AVX afxError _AvxCanvCtorCb(avxCanvas canv, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    (void)invokeNo;

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxCanvasConfig const* cfg = args[1];
    AFX_ASSERT(cfg);

    //afxWarp const* extent = args[1];
    //afxUnit surCnt = *(afxUnit const *)args[2];
    //avxCanvasRig const* surCfgs = args[3];

    if (!cfg->rigCnt)
    {
        AfxThrowError();
        return err;
    }

    canv->tag = cfg->tag;
    canv->udd = cfg->udd;

    avxLimits const* limits = _AvxDsysGetLimits(dsys);
    // @extent must be less than or equal to maxCanvasWhd
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.w, cfg->extent.w);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.h, cfg->extent.h);
    AFX_ASSERT_CAPACITY(limits->maxCanvasWhd.d, cfg->extent.d);

    canv->extent.w = AFX_MAX(1, cfg->extent.w);
    canv->extent.h = AFX_MAX(1, cfg->extent.h);
    canv->extent.d = AFX_MAX(1, cfg->extent.d);
    canv->extentMin = canv->extent;
    canv->extentMax = (avxExtent){ limits->maxCanvasWhd.w, limits->maxCanvasWhd.h, limits->maxCanvasWhd.d};

    AFX_ASSERT_CAPACITY(limits->canvasNoAttachmentsSampleCnts, cfg->lodCnt);
    canv->lodCnt = AFX_MAX(1, cfg->lodCnt);
    
    canv->ddi = &_AVX_CANV_DDI;

    canv->flags = cfg->flags;

    afxBool combinedDs = FALSE;
    afxUnit depthInIdx = AFX_INVALID_INDEX;
    afxUnit stencilInIdx = AFX_INVALID_INDEX;
    afxUnit rigCnt = 0;
    afxUnit colorCnt = 0;

    for (afxUnit i = 0; i < cfg->rigCnt; i++)
    {
        avxCanvasRig const* sur = &cfg->rigs[i];
        AFX_ASSERT(sur->fmt);
        AFX_ASSERT(sur->lodCnt);

        if (AvxTestDepthFormat(sur->fmt))
        {
            if (depthInIdx == AFX_INVALID_INDEX)
                ++rigCnt;

            depthInIdx = i;

            if ((combinedDs = AvxTestCombinedDsFormat(sur->fmt)))
                stencilInIdx = depthInIdx;

            continue;
        }

        if (AvxTestStencilFormat(sur->fmt))
        {
            if (stencilInIdx == AFX_INVALID_INDEX)
                ++rigCnt;

            stencilInIdx = i;
            continue;
        }

        ++colorCnt;
        ++rigCnt;
    }

    if (!colorCnt)
        canv->flags |= avxCanvasFlag_COLOR;

    if (!rigCnt)
    {
        canv->flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_DS_COMBO);
        canv->flags |= avxCanvasFlag_VOID;
    }

    canv->linkedCnt = 0;
    AFX_ASSERT_RANGE(limits->maxColorAttachments, 0, colorCnt);
    canv->colorCnt = colorCnt;
    canv->ownershipMask = NIL;

    if (stencilInIdx == AFX_INVALID_INDEX)
        canv->dsRigIdx[1] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_STENCIL;
        canv->dsRigIdx[1] = (rigCnt - 1);
    }

    if (depthInIdx == AFX_INVALID_INDEX)
        canv->dsRigIdx[0] = AFX_INVALID_INDEX;
    else
    {
        canv->flags |= avxCanvasFlag_DEPTH;
        if (combinedDs)
        {
            canv->dsRigIdx[0] = canv->dsRigIdx[1];
            canv->flags |= avxCanvasFlag_DS_COMBO;
        }
        else if (stencilInIdx != AFX_INVALID_INDEX) canv->dsRigIdx[0] = (canv->dsRigIdx[1] - 1);
        else canv->dsRigIdx[0] = (rigCnt - 1);
    }

    afxAllocation const stashs[] =
    {
        AFX_ALLOCATION(rigCnt, sizeof(canv->rigs[0]), 0, &canv->rigs)
    };
    if (AfxFailed(AfxAllocateInstanceData(canv, ARRAY_SIZE(stashs), stashs)))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(canv->rigs);
    canv->rigCnt = rigCnt;
    avxCanvasRig const* rigCfg;
    _avxCanvasRig* rig;

    for (afxUnit i = 0; i < rigCnt; i++)
    {
        rig = &canv->rigs[i];
        *rig = (_avxCanvasRig) { 0 };

        rigCfg = &cfg->rigs[i];

        if (i == canv->dsRigIdx[1])
            rigCfg = &cfg->rigs[stencilInIdx];

        if (i == canv->dsRigIdx[0])
            rigCfg = &cfg->rigs[depthInIdx];

        avxRaster ras = rigCfg->ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

            avxRasterInfo rasi;
            AvxGetRasterInfo(ras, &rasi);

            rig->ras = ras;
            rig->managed = FALSE;
            rig->resolve = NIL;
            rig->fmt = rasi.fmt;
            rig->usage = rasi.usage;
            rig->flags = rasi.flags;
            rig->lodCnt = rasi.lodCnt;

            if (_AvxRigdDrawBuffers(canv, i, 1, &ras))
            {
                AfxThrowError();
            }
            AFX_ASSERT(canv->rigs[i].ras == ras); // relink must set it.
        }
        else
        {
            rig->ras = NIL;
            rig->managed = TRUE;
            rig->resolve = NIL;
            rig->fmt = rigCfg->fmt;
            rig->usage = rigCfg->usage | avxRasterUsage_DRAW;
            rig->flags = rigCfg->flags;
            rig->lodCnt = rigCfg->lodCnt;

            if (_AvxCanvRelinkDrawBuffersCb_SW(canv, TRUE, i, 1, &ras))
            {
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxUnit j = i; j--> 0;)
            {
#if 0
                if ((ras = canv->rigs[i].ras))
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                    AfxDisposeObjects(1, &ras);
                }
#else
                _AvxRigdDrawBuffers(canv, j, 1, NIL);
#endif
            }
        }
    }

#if 0
    if (_AvxRedoDrawBuffers(canv))
        AfxThrowError();
#endif

    if (err)
    {
        if (AfxFailed(AfxDeallocateInstanceData(canv, ARRAY_SIZE(stashs), stashs)))
            AfxThrowError();
    }
    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_CANV =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .desc = "Fixed-Function Buffered Canvas",
    .fixedSiz = sizeof(AFX_OBJECT(avxCanvas)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxCanvCtorCb,
    .dtor = (void*)_AvxCanvDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxDsysSwConfigureCanvCb(afxDrawSystem dsys, avxCanvasConfig* cfg)
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    /*
        A framebuffer in Vulkan is a collection of image views that are used as attachments in a render pass instance.
        Each framebuffer must match the render pass in:
            Number of attachments
            Format of each attachment
            Sample count of each attachment

        Sample count (VkSampleCountFlagBits) defines how many samples per pixel are used in the attachments, 
        enabling multisample anti-aliasing (MSAA). Common values: VK_SAMPLE_COUNT_1_BIT (no MSAA), 2, 4, 8, 16, depending on GPU support.

        Framebuffer Limitations with Sample Count
        All attachments must have the same sample count:
            Vulkan requires that all color, depth, and stencil attachments in a framebuffer must have the same sample count.
            You can't mix VK_SAMPLE_COUNT_1_BIT and VK_SAMPLE_COUNT_4_BIT attachments in the same framebuffer.

        Must match render pass sample count:
            If a render pass defines multisampled attachments, then the framebuffer must contain images with the same sample count.

        Hardware-dependent maximums:
            Use VkPhysicalDeviceProperties.limits.framebufferColorSampleCounts, framebufferDepthSampleCounts, etc., 
            to check what sample counts are supported.

        Resolve attachments required:
            If your render pass is multisampled, you typically need resolve attachments (single-sample images) 
            to copy the final color output for presentation.
    */

    avxCanvasConfig cfg2 = { 0 };
    cfg2.udd = cfg->udd;
    cfg2.tag = cfg->tag;

    avxLimits const* limits = _AvxDsysGetLimits(dsys);
    // @extent must be less than or equal to limits(maxCanvasWhd).
    cfg2.extent.w = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.w, cfg->extent.w));
    cfg2.extent.h = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.h, cfg->extent.h));
    cfg2.extent.d = AFX_MAX(1, AFX_MIN(limits->maxCanvasWhd.d, cfg->extent.d));
    
    cfg2.lodCnt = AFX_MAX(1, AFX_MIN(limits->canvasNoAttachmentsSampleCnts, cfg->lodCnt));

    cfg2.rigCnt = AFX_MIN(cfg->rigCnt, limits->maxColorAttachments + 2);

    if (!cfg2.rigCnt)
    {
        cfg2.flags |= cfg->flags & ~(avxCanvasFlag_COLOR | avxCanvasFlag_DEPTH | avxCanvasFlag_STENCIL | avxCanvasFlag_DS_COMBO);
        cfg2.flags |= avxCanvasFlag_VOID;
    }
    else
    {
        afxBool combinedDs = FALSE;
        afxUnit depthInIdx = AFX_INVALID_INDEX;
        afxUnit stencilInIdx = AFX_INVALID_INDEX;
        afxUnit rigCnt = 0;
        afxUnit colorCnt = 0;

        afxBool expectsD = (cfg->flags & avxCanvasFlag_DEPTH);
        afxBool expectsS = (cfg->flags & avxCanvasFlag_STENCIL);
        afxBool expectsDS = (cfg->flags & avxCanvasFlag_DS_COMBO);

        for (afxUnit i = 0; i < cfg->rigCnt; i++)
        {
            avxCanvasRig const* ac = &cfg->rigs[i];
            avxFormat fmt = ac->fmt;
            avxRasterUsage usage = ac->usage;
            avxRasterFlags flags = ac->flags;
            avxRaster buf = ac->ras;
            afxUnit lodCnt = ac->lodCnt;

            if (!fmt)
            {
                if (!colorCnt)
                {
                    fmt = avxFormat_RGBA8un;
                }
            }
            else
            {
                if (AvxTestDepthFormat(fmt))
                {
                    if (depthInIdx == AFX_INVALID_INDEX)
                        ++rigCnt;

                    depthInIdx = i;

                    if ((combinedDs = AvxTestCombinedDsFormat(fmt)))
                        stencilInIdx = depthInIdx;

                    continue;
                }

                if (AvxTestStencilFormat(fmt))
                {
                    if (stencilInIdx == AFX_INVALID_INDEX)
                        ++rigCnt;

                    stencilInIdx = i;
                    continue;
                }
            }

            avxCanvasRig* c = &cfg2.rigs[colorCnt];
            c->lodCnt = AFX_MAX(lodCnt, cfg2.lodCnt);
            c->usage = avxRasterUsage_DRAW | usage;
            c->flags = flags;
            c->fmt = fmt;
            c->ras = buf;

            ++colorCnt;
            ++rigCnt;
        }

        if (!rigCnt) cfg2.flags |= avxCanvasFlag_VOID;
        else
        {
            if (colorCnt)
                cfg2.flags |= avxCanvasFlag_COLOR;

            if (stencilInIdx != AFX_INVALID_INDEX)
            {
                cfg2.flags |= avxCanvasFlag_STENCIL;

                if (!combinedDs)
                {
                    avxCanvasRig* s = &cfg2.rigs[(rigCnt - 1)];
                    avxCanvasRig const* sc = &cfg->rigs[stencilInIdx];
                    *s = *sc;
                    s->ras = sc->ras;
                    s->fmt = sc->fmt;
                    s->flags = sc->flags;
                    s->usage = sc->usage | avxRasterUsage_DRAW;
                    s->lodCnt = AFX_MAX(1, sc->lodCnt);
                }
            }

            if (depthInIdx != AFX_INVALID_INDEX)
            {
                cfg2.flags |= avxCanvasFlag_DEPTH;

                if (!combinedDs)
                {
                    avxCanvasRig* d = &cfg2.rigs[colorCnt];
                    avxCanvasRig const* dc = &cfg->rigs[depthInIdx];
                    *d = *dc;
                    d->ras = dc->ras;
                    d->fmt = dc->fmt;
                    d->flags = dc->flags;
                    d->usage = dc->usage | avxRasterUsage_DRAW;
                    d->lodCnt = AFX_MAX(1, dc->lodCnt);
                }
                else
                {
                    cfg2.flags |= avxCanvasFlag_DS_COMBO;

                    avxCanvasRig* ds = &cfg2.rigs[colorCnt];
                    avxCanvasRig const* dsc = &cfg->rigs[depthInIdx];
                    *ds = *dsc;
                    ds->ras = dsc->ras;
                    ds->fmt = dsc->fmt;
                    ds->flags = dsc->flags;
                    ds->usage = dsc->usage | avxRasterUsage_DRAW;
                    ds->lodCnt = AFX_MAX(1, dsc->lodCnt);

                    // How to nest separated D/S?
                }
            }

        }
    }

    for (afxUnit i = 0; i < AVX_MAX_CANVAS_BUFFERS; i++)
        cfg2.rigs[i].usage |= avxRasterUsage_DRAW;

    *cfg = cfg2;
    return err;
}

_AVX afxError AvxConfigureCanvas(afxDrawSystem dsys, avxCanvasConfig* cfg)
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AFX_ASSERT(cfg);
    if (AfxFailed(_AvxDsysGetDdi(dsys)->cfgCanvCb(dsys, cfg)))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AVX afxError _AvxDsysSwAcquireCanvCb(afxDrawSystem dsys, avxCanvasConfig const* cfg, afxUnit cnt, avxCanvas canvases[])
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(canvases);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->canvCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_CANV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)canvases, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CANV, cnt, canvases);

    return err;
}

_AVX afxError AvxAcquireCanvas(afxDrawSystem dsys, avxCanvasConfig const* cfg, afxUnit cnt, avxCanvas canvases[])
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(canvases);
    AFX_ASSERT(cfg);

    if (AfxFailed(_AvxDsysGetDdi(dsys)->acqCanvCb(dsys, cfg, cnt, canvases)))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CANV, cnt, canvases);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxCanvas canv = canvases[i];

        //AFX_ASSERT(canv->rigCnt >= cfg->surCnt);
        AFX_ASSERT(canv->extent.w >= cfg->extent.w);
        AFX_ASSERT(canv->extent.h >= cfg->extent.h);
        AFX_ASSERT(canv->extent.d >= cfg->extent.d);
        AFX_ASSERT((canv->flags & cfg->flags) == cfg->flags);
        AFX_ASSERT(canv->tag.start == cfg->tag.start);

        for (afxUnit j = 0; j < cfg->rigCnt; j++)
        {
            _avxCanvasRig* surf = &canv->rigs[j];

            // combined DS reduces one rig.
            if (j == canv->rigCnt)
                break;

            if (!cfg->rigs[j].ras)
            {
                AFX_ASSERT((surf->fmt == cfg->rigs[j].fmt) || !(cfg->rigs[j].fmt));
                AFX_ASSERT((surf->flags & cfg->rigs[j].flags) == cfg->rigs[j].flags);
                AFX_ASSERT((surf->usage & cfg->rigs[j].usage) == cfg->rigs[j].usage);
                AFX_ASSERT((surf->usage & avxRasterUsage_DRAW) == avxRasterUsage_DRAW);
                AFX_ASSERT(surf->lodCnt >= cfg->rigs[j].lodCnt);
            }
            else
            {
                AFX_ASSERT(surf->ras == cfg->rigs[j].ras);
            }
        }
    }
#endif
    return err;
}
