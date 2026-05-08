/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_BUFFER_DDK_H
#define AMX_BUFFER_DDK_H

#include "qwadro/mix/afxMixSystem.h"

AFX_DEFINE_STRUCT(_amxBufStorage)
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

AFX_DEFINE_STRUCT(_amxBufferRemapping)
{
    amxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    afxFlags        flags;
    afxBool         unmap;
    void**          placeholder;
};

#ifdef _AMX_BUFFER_C
#ifdef _AMX_BUFFER_IMPL
AFX_OBJECT(_amxBuffer)
#else
AFX_OBJECT(amxBuffer)
#endif
{
    afxString       tag;
    void*           udd;

    amxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    amxBufferUsage  usage;
    amxBufferFlags  flags;
    amxFormat       fmt; // for tensor buffer
    afxUnit         fmtBps;
    afxUnit         fmtStride;

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _amxBufStorage  storage[1]; // non-sparse
};
#endif

AMX afxClassConfig const _AMX_MBUF_CLASS_CONFIG;

AMX afxError _AmxMsysTransferCb_SW(afxMixSystem msys, amxTransference* ctrl, afxUnit opCnt, void const* ops);
AMX afxError _AmxMsysRemapBuffersCb_SW(afxMixSystem msys, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[]);
AMX afxError _AmxMsysCohereMappedBuffersCb_SW(afxMixSystem msys, afxBool discard, afxUnit cnt, amxBufferedMap const maps[]);

#endif//AMX_BUFFER_DDK_H
