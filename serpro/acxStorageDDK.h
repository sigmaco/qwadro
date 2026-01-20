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

// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_STORAGE_DDK_H
#define ACX_STORAGE_DDK_H

#include "qwadro/pro/afxStepSystem.h"

AFX_DEFINE_STRUCT(_acxBufStorage)
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

AFX_DEFINE_STRUCT(_acxBufferRemapping)
{
    acxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    afxFlags        flags;
    afxBool         unmap;
    void**          placeholder;
};


ACX afxError _AcxSsysTransferCb_SW(afxStepSystem ssys, acxTransference* ctrl, afxUnit opCnt, void const* ops);
ACX afxError _AcxSsysRemapBuffersCb_SW(afxStepSystem ssys, afxBool unmap, afxUnit cnt, _acxBufferRemapping const maps[]);
ACX afxError _AcxSsysCohereMappedBuffersCb_SW(afxStepSystem ssys, afxBool discard, afxUnit cnt, acxBufferedMap const maps[]);

#endif//ACX_STORAGE_DDK_H
