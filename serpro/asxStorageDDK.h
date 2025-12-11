/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_STORAGE_DDK_H
#define ASX_STORAGE_DDK_H

#include "qwadro/pro/afxStepSystem.h"

AFX_DEFINE_STRUCT(_asxBufStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize offset; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxUnit     mapRefCnt;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
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
    } hostedAlloc;
};

AFX_DEFINE_STRUCT(_asxBufferRemapping)
{
    asxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    afxFlags        flags;
    afxBool         unmap;
    void**          placeholder;
};


ASX afxError _AsxSsysTransferCb_SW(afxStepSystem ssys, asxTransference* ctrl, afxUnit opCnt, void const* ops);
ASX afxError _AsxSsysRemapBuffersCb_SW(afxStepSystem ssys, afxBool unmap, afxUnit cnt, _asxBufferRemapping const maps[]);
ASX afxError _AsxSsysCohereMappedBuffersCb_SW(afxStepSystem ssys, afxBool discard, afxUnit cnt, asxBufferedMap const maps[]);

#endif//ASX_STORAGE_DDK_H
