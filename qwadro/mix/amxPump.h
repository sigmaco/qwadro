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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_SOURCE_H
#define AMX_SOURCE_H

#include "qwadro/mix/amxAudio.h"

AFX_DEFINE_STRUCT(amxStream)
// Represents one stream in a media source.
{
    int a;
};

// Stops all active streams in the media source.
// This method is asynchronous. When the operation completes, the media source sends and MESourceStopped event, and every active stream sends an MEStreamStopped event.
// When a media source is stopped, its current position reverts to zero. After that, if the Start method is called with VT_EMPTY for the starting position, playback starts from the beginning of the presentation.
// While the source is stopped, no streams produce data.
afxError AmxStopSource(amxPump msrc);

// Starts, seeks, or restarts the media source by specifying where to start playback.
// A call to Start results in a seek if the previous state was started or paused, and the new starting position is not VT_EMPTY. Not every media source can seek.
afxError AfxStartSource(amxPump msrc, afxSize at);

// Pauses all active streams in the media source.
// The media source must be in the started state. The method fails if the media source is paused or stopped.
afxError AfxPauseSource(amxPump msrc);

// Shuts down the media source and releases the resources it is using.
// AfxDisposeObjects(1, &msrc);

AFX_DEFINE_STRUCT(amxPacket)
{
    afxFlags    flags;
    afxUnit     iosIdx; // index of source stream
    afxInt64    posn; // in stream
    afxInt64    dts; // time at which the packet is decompressed
    afxInt64    pts; // time at which the decompressed packet will be presented
    amxBuffer   buf; // packet data storage
    afxByte*    data;
    afxUnit     siz;
    afxUnit64   dur; // duration
    void*       udd;
};

AFX_DEFINE_STRUCT(amxPumpInfo)
{
    afxUnit iosCnt;
    void* udd;
    afxString tag;
};

AFX_DEFINE_STRUCT(amxPumpConfig)
{
    afxUnit iosCnt;
    void* udd;
    afxString tag;
};

AMX afxError AmxAcquirePumps
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxPumpConfig const cfgs[],
    amxPump pumps[]
);

#endif//AMX_SOURCE_H
