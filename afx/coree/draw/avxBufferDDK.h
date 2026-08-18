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

#ifndef AVX_BUFFER_DDK_H
#define AVX_BUFFER_DDK_H

#include "avxFenceDDK.h"

typedef enum _avxMemFlag
{
    _avxMemFlag_MAPPEABLE = AFX_BITMASK(0), // visible to CPU
    _avxMemFlag_CACHED = AFX_BITMASK(1), // cached by CPU
    _avxMemFlag_ENCLAVED = AFX_BITMASK(2), // virtually separated portion
    _avxMemFlag_EMBEDDED = AFX_BITMASK(3) // on-chip memory
} _avxMemFlags;

AFX_DEFINE_STRUCT(_avxBufMem)
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

AFX_DEFINE_STRUCT(_avxBufRemapping)
{
    avxBuffer       buf;
    afxSize         offset;
    afxUnit         range;
    avxMappingFlags flags;
    afxBool         unmap;
    void**          placeholder;
};

AFX_DECLARE_STRUCT(_avxBufDdi);
AFX_DECLARE_STRUCT(_avxBufIdd);

#ifdef _AVX_BUFFER_C
#ifdef _AVX_BUFFER_IMPL
AFX_OBJECT(_avxBuffer)
#else
AFX_OBJECT(avxBuffer)
#endif
{
    _avxBufDdi const*   ddi;
    _avxBufIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxBuffer           base; // handle used by subacquision of another buffer
    afxSize             from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    avxBufferUsage      usage;
    avxBufferFlags      flags;
    avxFormat           fmt; // for tensor buffer

    afxMask             exuMask;

    // STORAGE
    // required size of this storage block.
    afxSize             reqSiz;
    afxUnit             reqAlign;
    // required memory conditions for this storage block.
    afxFlags            reqMemType;
    _avxBufMem      storage[1]; // non-sparse
    afxSize             storageOffset;
};
#endif//_AVX_BUFFER_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_BUF;

AVXINL void _AvxSanitizeBufferCopy(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferCopy const raw[], avxBufferCopy san[]);
AVXINL void _AvxSanitizeBufferIo(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferIo const raw[], avxBufferIo san[]);

AVXINL afxError _AvxDsysSW_DeallocateBuffersCb(afxDrawSystem dsys, afxUnit cnt, avxBuffer buffers[]);
AVXINL afxError _AvxDsysSW_AllocateBuffersCb(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

AVXINL void _AvxAllocateBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

AVXINL void* _AvxGetClientBufferData(avxBuffer buf, afxSize from);

AVX afxError _AvxDsysSW_CohereMappedBuffersCb(afxDrawSystem dsys, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[]);
AVX afxError _AvxDsysSW_RemapBuffersCb(afxDrawSystem dsys, afxBool unmap, afxUnit cnt, _avxBufRemapping const maps[]);

#endif//AVX_BUFFER_DDK_H
