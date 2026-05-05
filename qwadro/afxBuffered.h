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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO IOMMU BUFFERIZATION                                               //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of SIGMA Future Storage.

#ifndef AFX_BUFFERED_H
#define AFX_BUFFERED_H

#include "qwadro/afxBuffer.h"

AFX_DEFINE_STRUCT(afxBufferedRing)
{
    afxBuffer   buf;

    afxSize     bufBase;

    afxUnit     bufRange;

    afxByte*    basePtr;

    afxSize     currOffset;

    afxUnit     blockAlign;

    afxSize     blockSiz;

    afxUnit     blockCnt;

    afxUnit     rounds;
};

AFX afxError AfxMakeBufferedRing
(
    afxBufferedRing* rng,

    afxBuffer buf,

    afxSize bufBase,

    afxUnit bufRange,

    afxUnit blockSiz,

    afxUnit blockAlign,

    afxUnit rounds
);

AFX afxSize AfxCycleBufferedRing
(
    afxBufferedRing* rng
);

AFX void* AfxAdvanceBufferedRing
(
    afxBufferedRing* rng, 
    afxUnit reqSiz, 
    afxSize* pOffset, 
    afxUnit* pRange
);

AFX_DEFINE_STRUCT(afxBufferedPumpStash)
{
    afxBuffer buffer;

    void* mapped_ptr;

    afxSize capacity;

    afxSize used;

    int frame_in_use; // When it was last used
};

AFX_DEFINE_STRUCT(afxBufferedPump)
{
    afxBufferedPumpStash* last;

    afxBufferedPumpStash* chunks;

    afxSize num_chunks;

    afxSize capacity_chunks;

    int current_frame;

    afxUnit rounds;

    afxUnit blockAlign;

    afxUnit minChunkSiz;

    afxBufferFlags bufFlags;

    afxBufferUsage bufUsage;

    afxIoSystem xsys;
};

AFX afxError AfxDeployBufferedPump
(
    afxBufferedPump* pump, 

    afxBufferUsage usage, 

    afxBufferFlags flags, 

    afxUnit minChunkSiz, 

    afxUnit blockAlign, 

    afxUnit rounds,

    afxIoSystem xsys
);

AFX void* AfxRequestBufferedPump
(
    afxBufferedPump* pump,

    afxSize size, 

    afxBuffer* out_buffer, 

    afxSize* out_offset
);

AFX afxError AfxAdvanceBufferedPump
(
    afxBufferedPump* pump
);

AFX afxError AfxDismantleBufferedPump
(
    afxBufferedPump* pump
);

#endif//AFX_BUFFERED_H
