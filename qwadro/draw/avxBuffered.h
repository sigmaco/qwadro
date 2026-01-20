/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO VIDEO MEMORY BUFFER                                               //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_BUFFERED_H
#define AVX_BUFFERED_H

#include "qwadro/draw/avxBuffer.h"

AFX_DEFINE_STRUCT(avxBufferedRing)
{
    afxUnit     rounds;
    afxSize     blockSiz;
    afxUnit     blockAlign;
    avxBuffer   buf;
    afxSize     maxSiz;
    afxUnit     blockCnt;
    afxByte*    basePtr;
    afxSize     currOffset;
};

AVX afxError AvxMakeBufferedRing
(
    avxBufferedRing* rng, 
    afxUnit rounds, 
    afxUnit blockSiz, 
    afxUnit blockAlign, 
    avxBuffer buf, 
    afxSize bufCap, 
    void* mapped
);

AVX afxSize AvxCycleBufferedRing
(
    avxBufferedRing* rng
);

AVX void* AvxAdvanceBufferedRing
(
    avxBufferedRing* rng, 
    afxUnit reqSiz, 
    afxSize* pOffset, 
    afxUnit* pRange
);

AFX_DEFINE_STRUCT(avxBufferedPumpStash)
{
    avxBuffer buffer;
    void* mapped_ptr;
    afxSize capacity;
    afxSize used;
    int frame_in_use; // When it was last used
};

AFX_DEFINE_STRUCT(avxBufferedPump)
{
    avxBufferedPumpStash* last;
    avxBufferedPumpStash* chunks;
    afxSize num_chunks;
    afxSize capacity_chunks;
    int current_frame;

    afxUnit rounds;
    afxUnit blockAlign;
    afxUnit minChunkSiz;
    avxBufferFlags bufFlags;
    avxBufferUsage bufUsage;
    afxDrawSystem dsys;
};

AVX afxError AvxDeployBufferedPump
(
    avxBufferedPump* pump, 
    avxBufferUsage usage, 
    avxBufferFlags flags, 
    afxUnit minChunkSiz, 
    afxUnit blockAlign, 
    afxUnit rounds,
    afxDrawSystem dsys
);

AVX void* AvxRequestBufferedPump
(
    avxBufferedPump* pump,
    afxSize size, 
    avxBuffer* out_buffer, 
    afxSize* out_offset
);

AVX afxError AvxAdvanceBufferedPump
(
    avxBufferedPump* pump
);

AVX afxError AvxDismantleBufferedPump
(
    avxBufferedPump* pump
);

#endif//AVX_BUFFERED_H
