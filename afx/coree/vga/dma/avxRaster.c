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
#define _AVX_RASTER_C
#include "../icd/avxIcd.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x extent >> lod

_AVXINL afxDrawSystem AvxGetRasterSystem(avxRaster ras)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    afxDrawSystem dsys = AfxGetHost(ras);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVXINL avxFormat AvxGetRasterFormat(avxRaster ras)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif
    return ras->fmt;
}

_AVXINL avxFormat AvxDescribeRasterFormat(avxRaster ras, avxFormatDescription* pfd)
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_S(pfd, "must be a valid pointer to a avxFormatDescription structure.");
#endif//AVX_VALIDATION_ENABLED

    // This function is a utility to ease introdution of device-dependent formats.
    // Give the multi-device nature of Qwadro, it takes a index of a bridge established in the provider system.

    avxFormat fmt = ras->fmt;
    avxFormatDescription desc;
    AvxDescribeFormat(fmt, &desc);
    *pfd = desc;
    return fmt;
}

_AVXINL avxRasterUsage AvxGetRasterUsage(avxRaster ras, avxRasterUsage mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (!mask) ? ras->usage : (ras->usage & mask);
}

_AVXINL avxRasterFlags AvxGetRasterFlags(avxRaster ras, avxRasterFlags mask)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
#endif//AVX_VALIDATION_ENABLED
    return (!mask) ? ras->flags : (ras->flags & mask);
}

_AVXINL void AvxGetRasterSwizzling(avxRaster ras, avxSwizzling* csw)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(csw);
#endif//AVX_VALIDATION_ENABLED
    *csw = ras->swizzling;
}

_AVXINL avxExtent AvxGetRasterExtent(avxRaster ras, afxUnit lodIdx)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->mipCnt, lodIdx, 1);

    afxBool const is3d = (ras->flags & avxRasterFlag_3D);
    avxExtent extent = ras->extent;

    if (is3d) while (lodIdx--)
    {
        extent.w = extent.w >> 1;
        extent.h = extent.h >> 1;
        extent.d = extent.d >> 1;
    }
    else while (lodIdx--)
    {
        extent.w = extent.w >> 1;
        extent.h = extent.h >> 1;
    }

    return extent;
}

_AVXINL avxExtent AvxGetRasterExtentDisplaced(avxRaster ras, afxUnit lodIdx, avxOrigin const origin)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT_RANGE(ras->mipCnt, lodIdx, 1);

    afxBool const is3d = (ras->flags & avxRasterFlag_3D);
    avxExtent extent = ras->extent;
    avxOrigin origin2 = origin;

    if (is3d) while (lodIdx--)
    {
        extent.w = extent.w >> 1;
        extent.h = extent.h >> 1;
        extent.d = extent.d >> 1;

        origin2.x = origin2.y >> 1;
        origin2.y = origin2.y >> 1;
        origin2.z = origin2.z >> 1;
    }
    else while (lodIdx--)
    {
        extent.w = extent.w >> 1;
        extent.h = extent.h >> 1;

        origin2.x = origin2.y >> 1;
        origin2.y = origin2.y >> 1;
    }

    extent.w = AFX_CLAMP(extent.w, 0, origin2.x);
    extent.h = AFX_CLAMP(extent.h, 0, origin2.y);
    extent.d = AFX_CLAMP(extent.d, 0, origin2.z);

    return extent;
}

_AVXINL void AvxGetRasterInfo(avxRaster ras, avxRasterInfo* desc)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(desc);
#endif//AVX_VALIDATION_ENABLED
    desc->lodCnt = ((ras->spp > 1) ? (ras->spp >> 1) : ras->mipCnt);
    desc->extent = AvxGetRasterExtent(ras, 0);
    desc->fmt = ras->fmt;
    desc->flags = ras->flags;
    desc->usage = ras->usage;

    desc->exuMask = 0;
    desc->udd = ras->udd;
    desc->tag = ras->tag;
}

_AVXINL afxError AvxGetSubrasterInfo(avxRaster ras, avxSubrasterInfo* sub)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(sub);
#endif//AVX_VALIDATION_ENABLED

    *sub = (avxSubrasterInfo)
    {
        .ras = ras->base,
            .baseLayer = ras->baseLayer,
            .baseLod = ras->baseMip,
            .swizzle = ras->swizzling
    };

    sub->lodCnt = ((ras->spp > 1) ? (ras->spp >> 1) : ras->mipCnt);
    sub->fmt = ras->fmt;
    sub->flags = ras->flags;
    
    sub->udd = ras->udd;
    sub->tag = ras->tag;

    return (ras->base == ras) ? afxError_INVALID : afxError_NONE;
}

_AVXINL afxError AvxGetExorasterInfo(avxRaster ras, avxExorasterInfo* exo)
{
#if AVX_VALIDATION_ENABLED
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(exo);
#endif//AVX_VALIDATION_ENABLED
    exo->rasi.lodCnt = ((ras->spp > 1) ? (ras->spp >> 1) : ras->mipCnt);
    exo->rasi.extent = AvxGetRasterExtent(ras, 0);
    exo->rasi.fmt = ras->fmt;
    exo->rasi.flags = ras->flags;
    exo->rasi.usage = ras->usage;

    exo->rasi.exuMask = 0;
    exo->rasi.udd = ras->udd;
    exo->rasi.tag = ras->tag;

    //return (ras->base != ras) ? afxError_INVALID : afxError_NONE;
    return afxError_INVALID;
}

_AVXINL avxRasterRegion _AvxGetSanitizedRasterRegion(avxRaster ras, avxRasterRegion const* op)
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(op);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterRegion struct to be within the bounds of a raster object.
    */

    afxUnit lodIdx = AFX_MIN(op->lodIdx, ras->mipCnt - 1);
    avxExtent maxExt = AvxGetRasterExtent(ras, lodIdx);

    avxOrigin origin = {    AFX_MIN(op->origin.x,   (afxInt)(maxExt.w - 1)),
                            AFX_MIN(op->origin.y,   (afxInt)(maxExt.h - 1)),
                            AFX_MIN(op->origin.z,   (afxInt)(maxExt.d - 1)) };

    avxExtent extent = {    AFX_CLAMP(op->extent.w, 1, (maxExt.w - origin.x)),
                            AFX_CLAMP(op->extent.h, 1, (maxExt.h - origin.y)),
                            AFX_CLAMP(op->extent.d, 1, (maxExt.d - origin.z)) };

    // extent.w MUST not be 0.
    // extent.h MUST not be 0.
    // extent.d MUST not be 0.
    AFX_ASSERT(extent.w);
    AFX_ASSERT(extent.h);
    AFX_ASSERT(extent.d);

    avxRasterRegion rgn = { 0 };
    rgn.lodIdx = lodIdx;
    rgn.origin = origin;
    rgn.extent = extent;
    return rgn;
}

_AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[])
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
    AFX_ASSERT(cnt);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterRegion struct to be within the bounds of a raster object.
    */
    
    for (afxUnit i = 0; i < cnt; i++)
    {
#if !0
        san[i] = _AvxGetSanitizedRasterRegion(ras, &raw[i]);
#else
        avxRasterRegion* s = &san[i];
        avxRasterRegion const* r = &raw[i];
        s->lodIdx = AFX_MIN(r->lodIdx, ras->mipCnt - 1);
        avxExtent extent = AvxGetRasterExtent(ras, s->lodIdx);
        
        s->rc = AFX_LAYERED_RECT(   AFX_MIN(r->rc.area.x,   (afxInt)(extent.w - 1)),
                                    AFX_MIN(r->rc.area.y,   (afxInt)(extent.h - 1)),
                                    AFX_MIN(r->rc.baseLayer,(afxInt)(extent.d - 1)),
                                    AFX_CLAMP(r->rc.area.w,     1, extent.w - s->rc.area.x),
                                    AFX_CLAMP(r->rc.area.h,     1, extent.h - s->rc.area.y),
                                    AFX_CLAMP(r->rc.layerCnt,   1, extent.d - s->rc.baseLayer));

        // extent.w MUST not be 0.
        // extent.h MUST not be 0.
        // extent.d MUST not be 0.
        AFX_ASSERT(s->rc.area.w);
        AFX_ASSERT(s->rc.area.h);
        AFX_ASSERT(s->rc.layerCnt);
#endif
    }
}

_AVXINL avxRasterCopy _AvxGetSanitizedRasterCopy(avxRaster ras, avxRaster src, avxRasterCopy const* op)
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);
    // ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(op);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterCopy struct to be within the bounds of two raster objects.
    */

    afxUnit srcLodIdx = AFX_MIN(op->src.lodIdx, src->mipCnt - 1);
    afxUnit dstLodIdx = AFX_MIN(op->dstLodIdx, ras->mipCnt - 1);

    avxExtent srcMaxExt = AvxGetRasterExtent(src, srcLodIdx);
    avxExtent dstMaxExt = AvxGetRasterExtent(ras, dstLodIdx);

    avxOrigin srcOrigin = { AFX_MIN(op->src.origin.x,   (afxInt)(srcMaxExt.w - 1)),
                            AFX_MIN(op->src.origin.y,   (afxInt)(srcMaxExt.h - 1)),
                            AFX_MIN(op->src.origin.z,   (afxInt)(srcMaxExt.d - 1)) };

    avxOrigin dstOrigin = { AFX_MIN(op->dstOrigin.x, (afxInt)(dstMaxExt.w - 1)),
                            AFX_MIN(op->dstOrigin.y, (afxInt)(dstMaxExt.h - 1)),
                            AFX_MIN(op->dstOrigin.z, (afxInt)(dstMaxExt.d - 1)) };

    avxExtent extent = {AFX_CLAMP(op->src.extent.w, 1, AFX_MIN((srcMaxExt.w - srcOrigin.x), (dstMaxExt.w - dstOrigin.x))),
                        AFX_CLAMP(op->src.extent.h, 1, AFX_MIN((srcMaxExt.h - srcOrigin.y), (dstMaxExt.h - dstOrigin.y))),
                        AFX_CLAMP(op->src.extent.d, 1, AFX_MIN((srcMaxExt.d - srcOrigin.z), (dstMaxExt.d - dstOrigin.z))) };

    avxRasterCopy cr = { 0 };
    cr.src.lodIdx = srcLodIdx;
    cr.src.origin = srcOrigin;
    cr.src.extent = extent;
    cr.dstLodIdx = dstLodIdx;
    cr.dstOrigin = dstOrigin;
    return cr;
}

_AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[])
{
    afxError err = { 0 };
#if AVX_VALIDATION_ENABLED
    // src must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &src);
    // ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(raw);
    AFX_ASSERT(san);
    AFX_ASSERT(cnt);
#endif//AVX_VALIDATION_ENABLED

    /*
        This code sanitizes a avxRasterCopy struct to be within the bounds of two raster objects.
    */

    afxUnit srcLodCnt = src->mipCnt;
    afxUnit dstLodCnt = ras->mipCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
#if !0
        san[i] = _AvxGetSanitizedRasterCopy(ras, src, &raw[i]);
#else
        avxRasterCopy* s = &san[i];
        avxRasterCopy const* r = &raw[i];
        s->src.lodIdx = AFX_MIN(r->src.lodIdx, srcLodCnt - 1);
        avxExtent srcExtent = AvxGetRasterExtent(src, s->src.lodIdx);
        s->src.origin.x = AFX_MIN(r->src.origin.x, (afxInt)(srcExtent.w - 1));
        s->src.origin.y = AFX_MIN(r->src.origin.y, (afxInt)(srcExtent.h - 1));
        s->src.rc.baseLayer = AFX_MIN(r->src.rc.baseLayer, (afxInt)(srcExtent.d - 1));

        s->dstLodIdx = AFX_MIN(r->dstLodIdx, dstLodCnt - 1);
        avxExtent dstExtent = AvxGetRasterExtent(ras, s->dstLodIdx);
        s->dstOrigin.x = AFX_MIN(r->dstOrigin.x, (afxInt)(dstExtent.w - 1));
        s->dstOrigin.y = AFX_MIN(r->dstOrigin.y, (afxInt)(dstExtent.h - 1));
        s->dstOrigin.z = AFX_MIN(r->dstOrigin.z, (afxInt)(dstExtent.d - 1));

        s->src.extent.w = AFX_CLAMP(r->src.extent.w, 1, AFX_MIN((srcExtent.w - s->src.origin.x), (dstExtent.w - s->dstOrigin.x)));
        s->src.extent.h = AFX_CLAMP(r->src.extent.h, 1, AFX_MIN((srcExtent.h - s->src.origin.y), (dstExtent.h - s->dstOrigin.y)));
        s->src.extent.d = AFX_CLAMP(r->src.extent.d, 1, AFX_MIN((srcExtent.d - s->src.rc.baseLayer), (dstExtent.d - s->dstOrigin.z)));
#endif
    }
}

_AVX afxBool AvxQueryRasterArrangement(avxRaster ras, avxRasterRegion const* rgn, avxRasterArrangement* layout)
{
    afxError err = { 0 };
    // @ras must be a valid avxRaster handle.
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxUnit spp = ras->spp;
    afxUnit mipCnt = ras->mipCnt;
    afxUnit fmtStride = ras->fmtStride;
    afxUnit align = ras->reqAlign;
    if (!align) align = AVX_RASTER_ALIGNMENT;

    if (!rgn)
    {
        afxUnit size = ras->reqSiz;

        if (!size)
        {
            afxWarp extent = { ras->extent.w, ras->extent.h, ras->extent.d };
            afxBool is3d = !!AvxGetRasterFlags(ras, avxRasterFlag_3D);

            for (afxUnit i = 0; i < mipCnt; i++)
            {
                afxUnit rowStride = AFX_ALIGN_SIZE(extent.w * fmtStride * spp, AFX_SIMD_ALIGNMENT);
                afxUnit imgStride = AFX_ALIGN_SIZE(extent.h * rowStride, AFX_SIMD_ALIGNMENT);
                size += extent.d * imgStride;

                extent.w = AFX_MAX(1, extent.w >> 1);
                extent.h = AFX_MAX(1, extent.h >> 1);
                if (is3d) extent.d = AFX_MAX(1, extent.d >> 1);
            }

            size = AFX_ALIGN_SIZE(size, align);
        }

        AFX_ASSERT(layout);
        layout->offset = ras->storageOffset;
        layout->size = size;
        layout->align = align;
        afxUnit rowStride = AFX_ALIGN_SIZE(ras->extent.w * spp, align);
        layout->rowStride = rowStride;
        layout->fmtStride = fmtStride;

        if ((ras->flags & avxRasterFlag_LAYER))
            layout->imgStride = AFX_ALIGN_SIZE(ras->extent.h * rowStride, align);
        else
            layout->imgStride = ras->extent.h * rowStride;

        return !err;
    }

    AFX_ASSERT_RANGE(ras->mipCnt, rgn->lodIdx, 1);
    AFX_ASSERT_RANGE(ras->extent.d, rgn->origin.z, rgn->extent.d);

    avxFormatDescription pfd;
    AvxDescribeFormat(ras->fmt, &pfd);

    afxBool is3d = AvxGetRasterFlags(ras, avxRasterFlag_3D);
    AFX_ASSERT(!rgn->origin.z || !is3d);

    afxUnit targetMipLevel = rgn->lodIdx;
    afxUnit baseArrayLayer = (is3d) ? 0 : rgn->origin.z;
    afxUnit layerCount = (is3d) ? 1 : rgn->extent.d;

    afxUnit imageWidth = ras->extent.w;
    afxUnit imageHeight = ras->extent.h;
    afxUnit imageDepth = (is3d) ? ras->extent.d : 1;
    afxSize offset = 0;

    // Accumulate all previous mip levels (for all layers).
    for (afxUnit mip = 0; mip < targetMipLevel; ++mip)
    {
        afxUnit mipW = AFX_MAX(1, imageWidth >> mip);
        afxUnit mipH = AFX_MAX(1, imageHeight >> mip);
        afxUnit mipD = ((is3d) ? AFX_MAX(1, imageDepth >> mip) : 1);

        // TODO: texel blocks

        afxUnit rowPitch = AFX_ALIGN_SIZE(mipW * fmtStride * spp, ras->reqAlign);
        afxUnit depthPitch = rowPitch * mipH;
        afxUnit mipSize = depthPitch * mipD;

        offset += mipSize * ((is3d) ? 1 : imageDepth); // uses total number of layers in image
    }

    // Now compute dimensions for target mip level.
    afxUnit mipW = AFX_MAX(1, imageWidth >> targetMipLevel);
    afxUnit mipH = AFX_MAX(1, imageHeight >> targetMipLevel);
    afxUnit mipD = ((is3d) ? AFX_MAX(1, imageDepth >> targetMipLevel) : 1);

    // TODO: texel blocks

    afxUnit rowPitch = AFX_ALIGN_SIZE(mipW * fmtStride * spp, ras->reqAlign);
    afxUnit depthPitch = rowPitch * mipH;
    afxUnit arrayPitch = depthPitch * ((is3d) ? 1 : mipD);

    // Accumulate offset from previous array layers (up to baseArrayLayer).
    offset += baseArrayLayer * arrayPitch;

    // Add offset inside selected region.
    offset += rgn->origin.z * ((is3d) ? depthPitch : 1);
    offset += rgn->origin.y * rowPitch;
    offset += rgn->origin.x * pfd.stride;

    // Compute size across all layers selected.
    // For each layer: depthPitch * (depth - 1) + rowPitch * (height - 1) + width * texelSize.
    afxSize singleLayerSize =
        (rgn->extent.d - 1) * depthPitch +
        (rgn->extent.h - 1) * rowPitch +
         rgn->extent.w * pfd.stride;

    afxSize totalSize = singleLayerSize * layerCount;

    AFX_ASSERT(layout);
    layout->imgStride = (is3d) ? depthPitch : arrayPitch;
    layout->rowStride = rowPitch;
    layout->offset = offset + ras->storageOffset;
    layout->size = totalSize;
    return !err;
}

_AVX afxError _AvxRasDtorCb(avxRaster ras)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    if (ras->base)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras->base);
        AfxDisposeObjects(1, &ras->base);
    }

    afxDrawSystem dsys = AvxGetRasterSystem(ras);
    if (_AvxDsysGetDdi(dsys)->deallocRasCb(dsys, 1, &ras))
    {
        AfxThrowError();
    }

    return err;
}

_AVX afxError _AvxRasCtorCb(avxRaster ras, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);

    afxDrawSystem dsys = AvxGetRasterSystem(ras);
    avxRasterInfo const* rasi = args[1] ? ((avxRasterInfo const*)args[1]) + invokeNo : NIL;
    avxSubrasterInfo const* subi = args[2] ? ((avxSubrasterInfo const*)args[2]) + invokeNo : NIL;
    avxExorasterInfo const* exorasi = args[3] ? ((avxExorasterInfo const*)args[3]) + invokeNo : NIL;
    
    if (subi)
    {
        ras->udd = subi->udd;
        ras->tag = subi->tag;

        avxRaster base = subi->ras;

        if (!base)
        {
            AFX_ASSERT(subi->ras);
            AfxThrowError();
            return err;
        }
        else if (subi->ras->base && (subi->ras->base != subi->ras))
        {
            // Base raster must be a storage raster, not a raster view.
            AFX_ASSERT(!subi->ras->base);
            AfxThrowError();
            return err;
        }

        ras->base = base;
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &base);

        if (!subi->fmt) ras->fmt = base->fmt;
        else
        {
            if (!AvxTestCompatibleFormats(base->fmt, subi->fmt))
            {
                // must have a specified format.
                AfxThrowError();
                err = afxError_INVALID;
                return err;
            }
            ras->fmt = subi->fmt;
        }
        // Stride should be always equal in compatible formats.
        ras->fmtStride = base->fmtStride;

        ras->baseMip = AFX_MIN(subi->baseLod, base->mipCnt - 1);
        ras->mipCnt = AFX_MAX(1, AFX_MIN(subi->lodCnt, base->mipCnt - ras->baseMip));
        ras->baseLayer = AFX_MIN(subi->baseLayer, base->extent.d - 1);
        ras->extent = AVX_EXTENT(base->extent.w, base->extent.h, AFX_MAX(1, AFX_MIN(subi->layerCnt, base->extent.d - ras->baseLayer)));

        ras->swizzling.r = subi->swizzle.r;
        ras->swizzling.g = subi->swizzle.g;
        ras->swizzling.b = subi->swizzle.b;
        ras->swizzling.a = subi->swizzle.a;

        ras->flags = base->flags | subi->flags;
        ras->usage = base->usage;

        ras->storage[0] = base->storage[0];
        ras->storageOffset = base->storageOffset;

        ras->sub = TRUE;

        AfxReacquireObjects(1, &base);

        return err;
    }

    ras->udd = rasi->udd;
    ras->tag = rasi->tag;

    // FLAGS
    avxRasterFlags const mipAndMs = (avxRasterFlag_MIP | avxRasterFlag_MULTISAMP);
    avxRasterFlags flags = (rasi->flags & mipAndMs);
    ras->flags = NIL;

    if (flags == mipAndMs)
    {
        // can not be mipmapped and multisampled at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->mipCnt > 1))
    {
        // If not specified, default to mipmap
        ras->flags |= avxRasterFlag_MIP;
    }
    else
    {
        ras->flags |= (flags & mipAndMs);
    }

    flags = (rasi->flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYER));

    if ((flags == (avxRasterFlag_3D | avxRasterFlag_LAYER)) &&
        (flags == (avxRasterFlag_3D | avxRasterFlag_CUBEMAP)))
    {
        // can not be volumetric (3D) and layered or cubemap at same time.
        AfxThrowError();
        return err;
    }
    else if ((flags == NIL) && (ras->extent.d > 1))
    {
        // If not specified, default to layered.
        ras->flags |= avxRasterFlag_LAYER;
    }
    else
    {
        ras->flags |= (flags & (avxRasterFlag_3D | avxRasterFlag_CUBEMAP | avxRasterFlag_LAYER));

        if (flags & avxRasterFlag_CUBEMAP)
        {
            // if cubemap, layers must be a multiple of 6.

            if (ras->extent.d % 6)
            {
                AfxThrowError();
                return err;
            }
        }
    }

    flags = (rasi->flags & (avxRasterFlag_1D | avxRasterFlag_2D | avxRasterFlag_3D));

    if ((flags == (avxRasterFlag_2D | avxRasterFlag_1D)) ||
        (flags == (avxRasterFlag_2D | avxRasterFlag_3D)) ||
        (flags == (avxRasterFlag_3D | avxRasterFlag_1D)))
    {
        // Can not be both at same time.
        AfxThrowError();
        return err;
    }
    else if (flags == NIL)
    {
        // default to 2D if nothing is specified.
        ras->flags |= avxRasterFlag_2D;
        // default to layered if depth is present. See code above.
    }
    else
    {
        ras->flags |= flags;
    }

    ras->base = NIL;
    ras->baseMip = 0;
    ras->mipCnt = (ras->flags & avxRasterFlag_MIP) ? AFX_MAX(rasi->lodCnt, 1) : 1;
    ras->baseLayer = 0;
    ras->swizzling.r = avxColorSwizzle_R;
    ras->swizzling.g = avxColorSwizzle_G;
    ras->swizzling.b = avxColorSwizzle_B;
    ras->swizzling.a = avxColorSwizzle_A;

    if (!rasi->fmt)
    {
        // must have a specified format.
        AfxThrowError();
        return err;
    }
    avxFormat fmt = rasi->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormat(fmt, &pfd);
    ras->fmt = fmt;
    ras->fmtStride = pfd.stride;

    ras->usage = rasi->usage;

    ras->extent = AvxGetMaxiExtent(AVX_EXTENT(AFX_MAX(1, pfd.bcWh[0]), AFX_MAX(1, pfd.bcWh[1]), 1), rasi->extent);


    // STORAGE

    {
        avxRasterArrangement lay;
        AvxQueryRasterArrangement(ras, NIL, &lay);

        ras->reqSiz = lay.size;
        ras->reqAlign = lay.align;
        ras->reqMemType = NIL;

        ras->storageOffset = 0;
        ras->storage[0].mmu = AFX_INVALID_INDEX;
        ras->storage[0].host.addr = NIL;
    }
    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_RAS =
{
    .fcc = afxFcc_RAS,
    .name = "Raster",
    .desc = "Formatted Video Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(avxRaster)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxRasCtorCb,
    .dtor = (void*)_AvxRasDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireRasters(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].flags;

        if ((flags & avxRasterFlag_CUBEMAP) && (info[i].extent.d % 6))
        {
            AFX_ASSERT(info[i].extent.d % 6 == 0);
            AfxThrowError();

            if ((info[i].extent.d / 6) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_AND((info[i].extent.d / 6), (flags & avxRasterFlag_LAYER));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].extent.h > 1))
        {
            AFX_ASSERT(1 >= info[i].extent.h);
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].extent.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYER))
        {
            AfxThrowError();

            if ((info[i].extent.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].extent.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info, NIL, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

    if (_AvxDsysGetDdi(dsys)->allocRasCb(dsys, cnt, info, rasters))
    {
        AfxDisposeObjects(cnt, rasters);
        AfxThrowError();
        return err;
    }

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterInfo rasi;
        AvxGetRasterInfo(rasters[i], &rasi);

        AFX_ASSERT(rasi.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(rasi.extent.w >= info[i].extent.w);
        AFX_ASSERT(rasi.extent.h >= info[i].extent.h);
        AFX_ASSERT(rasi.extent.d >= info[i].extent.d);
        AFX_ASSERT(rasi.exuMask == info[i].exuMask);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(rasi.udd == info[i].udd);
        AFX_ASSERT((rasi.usage & info[i].usage) == info[i].usage);
    }
#endif

    return err;
}

_AVX afxError AvxAcquireExorasters(afxDrawSystem dsys, afxUnit cnt, avxExorasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].rasi.flags;

        if ((flags & avxRasterFlag_CUBEMAP) && (info[i].rasi.extent.d % 6))
        {
            AFX_ASSERT(info[i].rasi.extent.d % 6 == 0);
            AfxThrowError();

            if ((info[i].rasi.extent.d / 6) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_AND((info[i].rasi.extent.d / 6), (flags & avxRasterFlag_LAYER));
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_1D) && (info[i].rasi.extent.h > 1))
        {
            AFX_ASSERT(1 >= info[i].rasi.extent.h);
            AfxThrowError();

            if ((info[i].rasi.extent.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].rasi.extent.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }

        if ((flags & avxRasterFlag_2D) && (info[i].rasi.extent.d > 1) && !(flags & avxRasterFlag_LAYER))
        {
            AfxThrowError();

            if ((info[i].rasi.extent.d > 1) && !(flags & avxRasterFlag_LAYER))
            {
                AFX_ASSERT_OR(1 >= info[i].rasi.extent.d, flags & avxRasterFlag_LAYER);
                AfxThrowError();
            }
        }
    }
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, info, NIL, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

    if (_AvxDsysGetDdi(dsys)->allocRasCb(dsys, cnt, info, rasters))
    {
        AfxDisposeObjects(cnt, rasters);
        AfxThrowError();
        return err;
    }

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxExorasterInfo exo;
        avxRasterInfo rasi;
        AvxGetRasterInfo(rasters[i], &rasi);

        AFX_ASSERT(rasi.lodCnt >= info[i].rasi.lodCnt);
        AFX_ASSERT(rasi.extent.w >= info[i].rasi.extent.w);
        AFX_ASSERT(rasi.extent.h >= info[i].rasi.extent.h);
        AFX_ASSERT(rasi.extent.d >= info[i].rasi.extent.d);
        AFX_ASSERT(rasi.exuMask == info[i].rasi.exuMask);
        AFX_ASSERT((rasi.flags & info[i].rasi.flags) == info[i].rasi.flags);
        AFX_ASSERT(rasi.fmt == info[i].rasi.fmt);
        AFX_ASSERT(rasi.tag.start == info[i].rasi.tag.start);
        AFX_ASSERT(rasi.udd == info[i].rasi.udd);
        AFX_ASSERT((rasi.usage & info[i].rasi.usage) == info[i].rasi.usage);
    }
#endif

    return err;
}

_AVX afxError AvxAcquireSubrasters(afxDrawSystem dsys, afxUnit cnt, avxSubrasterInfo const info[], avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(rasters);
    AFX_ASSERT(info);
    AFX_ASSERT(cnt);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRasterFlags flags = info[i].flags;

        if (!info[i].ras)
        {
            AFX_ASSERT(info[i].ras);
            AfxThrowError();
        }
    }

    if (err) return err;
#endif//AVX_VALIDATION_ENABLED

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->rasCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)rasters, (void const*[]) { dsys, NIL, info, NIL }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RAS, cnt, rasters);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSubrasterInfo sub;
        avxRasterInfo rasi;
        AvxGetRasterInfo(rasters[i], &rasi);
        AvxGetSubrasterInfo(rasters[i], &sub);

        AFX_ASSERT(sub.ras == info[i].ras);
        AFX_ASSERT(sub.lodCnt >= info[i].lodCnt);
        AFX_ASSERT(sub.baseLayer == info[i].baseLayer);
        AFX_ASSERT(sub.baseLod == info[i].baseLod);
        AFX_ASSERT((rasi.flags & info[i].flags) == info[i].flags);
        AFX_ASSERT(rasi.fmt == info[i].fmt);
        AFX_ASSERT(rasi.tag.start == info[i].tag.start);
        AFX_ASSERT(sub.swizzle.r == info[i].swizzle.r);
        AFX_ASSERT(sub.swizzle.g == info[i].swizzle.g);
        AFX_ASSERT(sub.swizzle.b == info[i].swizzle.b);
        AFX_ASSERT(sub.swizzle.a == info[i].swizzle.a);
        AFX_ASSERT(rasi.udd == info[i].udd);
    }
#endif

    return err;
}
