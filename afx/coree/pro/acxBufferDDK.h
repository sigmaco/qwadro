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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Parallel Computing Extensions.

#ifndef ACX_BUFFER_DDK_H
#define ACX_BUFFER_DDK_H

#include "qwadro/pro/afxWarpSystem.h"

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

#ifdef _ACX_BUFFER_C
#ifdef _ACX_BUFFER_IMPL
AFX_OBJECT(_acxBuffer)
#else
AFX_OBJECT(acxBuffer)
#endif
{
    afxString       tag;
    void*           udd;

    acxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    acxBufferUsage  usage;
    acxBufferFlags  flags;
    acxFormat       fmt; // for tensor buffer
    afxUnit         fmtBps;
    afxUnit         fmtStride;

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _acxBufStorage  storage[1]; // non-sparse
};
#endif

ACX afxClassConfig const _ACX_SBUF_CLASS_CONFIG;

ACX afxError _AcxSsysTransferCb_SW(afxWarpSystem ssys, acxTransference* ctrl, afxUnit opCnt, void const* ops);
ACX afxError _AcxSsysRemapBuffersCb_SW(afxWarpSystem ssys, afxBool unmap, afxUnit cnt, _acxBufferRemapping const maps[]);
ACX afxError _AcxSsysCohereMappedBuffersCb_SW(afxWarpSystem ssys, afxBool discard, afxUnit cnt, acxBufferedMap const maps[]);

#endif//ACX_BUFFER_DDK_H
