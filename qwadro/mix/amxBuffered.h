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
 // MIX MEMORY BUFFERIZATION                                                 //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_BUFFERED_H
#define AMX_BUFFERED_H

#include "qwadro/mix/amxBuffer.h"

AFX_DEFINE_STRUCT(amxBufferedRing)
{
    amxBuffer   mbuf;
    afxSize     mbufBase;
    afxUnit     mbufRange;
    afxByte*    basePtr;
    afxSize     currOffset;
    afxUnit     blockAlign;
    afxSize     blockSiz;
    afxUnit     blockCnt;
    afxUnit     rounds;
};

AMX afxError AmxMakeBufferedRing
(
    amxBufferedRing* rng, 
    amxBuffer mbuf, 
    afxSize bufBase, 
    afxUnit bufRange, 
    afxUnit blockSiz, 
    afxUnit blockAlign, 
    afxUnit rounds
);

AMX afxSize AmxCycleBufferedRing
(
    amxBufferedRing* rng
);

AMX void* AmxAdvanceBufferedRing
(
    amxBufferedRing* rng, 
    afxUnit reqSiz, 
    afxSize* pOffset, 
    afxUnit* pRange
);

#endif//AMX_BUFFERED_H
