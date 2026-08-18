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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_TRACK_DDK_H
#define AMX_TRACK_DDK_H

#include "amxBufferDDK.h"

#ifdef _AMX_TRACK_C
#ifdef _AMX_TRACK_IMPL
AFX_OBJECT(_amxTrack)
#else
AFX_OBJECT(amxTrack)
#endif
{
    afxString       tag;
    void*           udd;

    amxFormat       fmt; // type and bits per sample
    afxUnit         frameCnt; // Number of samples (e.g., time slots)
    // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // Number of channels (e.g., stereo = 2)
    afxUnit         segCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    
    afxUnit64 current_sample_time;

    
    amxBufferFlags  bufFlags;
    afxUnit         bufCap; // with any alignment
    amxBuffer       buf;
};
#endif

#endif//AMX_TRACK_DDK_H
