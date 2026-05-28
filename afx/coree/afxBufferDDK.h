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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of SIGMA Future Storage.

#ifndef AFX_BUFFER_DDK_H
#define AFX_BUFFER_DDK_H

//#include "afxIommuDDK.h"
#include "qwadro/afxBuffered.h"
#include "qwadro/math/afxVector.h"

AFX_DECLARE_STRUCT(_afxBufDdi);
AFX_DECLARE_STRUCT(_afxBufIdd);

typedef enum _afxMemFlag
{
    _afxMemFlag_MAPPEABLE = AFX_BITMASK(0), // visible to CPU
    _afxMemFlag_CACHED = AFX_BITMASK(1), // cached by CPU
    _afxMemFlag_ENCLAVED = AFX_BITMASK(2), // virtually separated portion
    _afxMemFlag_EMBEDDED = AFX_BITMASK(3) // on-chip memory
} _afxMemFlags;

AFX_DEFINE_STRUCT(_afxBufMem)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize size; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped; // TODO: Eliminate it? Now we have ref-counted mapping.
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxUnit     mapRefCnt;
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

#ifdef _AFX_BUFFER_C
#ifdef _AFX_BUFFER_IMPL
AFX_OBJECT(_afxBuffer)
#else
AFX_OBJECT(afxBuffer)
#endif
{
    _afxBufDdi const*   ddi;
    _afxBufIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxBuffer           base; // handle used by subacquision of another buffer
    afxSize             from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    afxBufferUsage      usage;
    afxBufferFlags      flags;
    afxFormat           fmt; // for tensor buffer

    afxMask             exuMask;

    // STORAGE
    // required size of this storage block.
    afxSize             reqSiz;
    afxUnit             reqAlign;
    // required memory conditions for this storage block.
    afxFlags            reqMemType;
    _afxBufMem      storage[1]; // non-sparse
    afxSize             storageOffset;
};
#endif//_AFX_BUFFER_C

AFX_DEFINE_STRUCT(_afxBufRemapping)
{
    afxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    afxMappingFlags flags;
    afxBool         unmap;
    void**          placeholder;
};

AFX afxClassConfig const _AFX_CLASS_CONFIG_BUF;

AFXINL void _AfxSanitizeBufferCopy
(
    afxBuffer buf, 
    afxBuffer src, 
    afxUnit cnt, 
    afxBufferCopy const raw[],
    afxBufferCopy san[]
);

AFXINL void _AfxSanitizeBufferIo
(
    afxBuffer buf, 
    afxBuffer src, 
    afxUnit cnt, 
    afxBufferIo const raw[], 
    afxBufferIo san[]
);

AFXINL afxError _AfxIomSW_DeallocateBuffersCb(afxIommu iom, afxUnit cnt, afxBuffer buffers[]);

AFXINL afxError _AfxIomSW_AllocateBuffersCb(afxIommu iom, afxUnit cnt, afxBufferInfo const infos[], afxBuffer buffers[]);

#endif//AFX_BUFFER_DDK_H
