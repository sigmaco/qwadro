/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_RASTER_DDK_H
#define AVX_RASTER_DDK_H

#include "avxBufferDDK.h"

AFX_DEFINE_STRUCT(_avxRasMem)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize size; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
    union
    {
        struct
        {
            union
            {
                afxSize     addr;
                afxByte*    bytemap;
                afxUnit8*   u8;
                afxUnit16*  u16;
                afxUnit32*  u32;
                afxInt8*    i8;
                afxInt16*   i16;
                afxInt32*   i32;
                afxReal32*  f32;
                afxReal64*  f64;
                afxV2d*     f32v2;
                afxV3d*     f32v3;
                afxV4d*     f32v4;
            };
            afxBool external;
        } host;
        struct
        {
            int fd;
        } fd;
        struct
        {
            afxString type;
            union
            {
                void* handle;
                void* name;
            };
        } w32;
    };
};

AFX_DECLARE_STRUCT(_avxRasDdi);
AFX_DECLARE_STRUCT(_avxRasIdd);

#ifdef _AVX_RASTER_C
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(avxRaster)
#endif
{
    _avxRasDdi const*   ddi;
    _avxRasIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxRaster           base; // sub
    avxRasterFlags      flags;
    avxRasterUsage      usage;
    afxUnit             baseMip; // sub
    afxUnit             mipCnt; // mip level cnt
    afxUnit             baseLayer; // sub
    avxRange            whd; // extent of image
    afxUnit             spp; // samples per pixel --- 1, 2, 4, 8, 16, 32, or 64.
    avxFormat           fmt;
    afxUnit             fmtStride; // cached to avoid queries.
    avxSwizzling        swizzling; // sub
    
    afxBool             sub;
    afxBool             exo;

    // STORAGE
    // required size of this storage block.
    afxSize             reqSiz;
    afxUnit             reqAlign;
    // required memory conditions for this storage block.
    afxFlags            reqMemType;
    _avxRasMem      storage[1]; // non-sparse
    afxSize             storageOffset;

    // GAMBIARRA
    afxBool             _dout; // DOUT buffer
    afxBool             _doutBlitSrc; // it will never be scanout directly to display but will be copied to a dma-buf that can be scanout.
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_RAS;

AVXINL void _AvxSanitizeRasterIo(avxRaster ras, afxSize bufCap, afxUnit cnt, avxRasterIo const raw[], avxRasterIo san[]);
AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[]);
AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[]);

AVXINL afxError _AvxDsysSW_DeallocateRastersCb(afxDrawSystem dsys, afxUnit cnt, avxRaster rasters[]);
AVXINL afxError _AvxDsysSW_AllocateRastersCb(afxDrawSystem dsys, afxUnit cnt, avxRasterInfo const infos[], avxRaster rasters[]);

AVXINL void _AvxAllocateRasters(afxUnit cnt, avxRaster rasters[]);

AVXINL void* _AvxGetClientRasterData(avxRaster ras, afxSize from);

#endif//AVX_RASTER_DDK_H
