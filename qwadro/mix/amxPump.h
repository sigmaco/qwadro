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

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_PUMP_H
#define AMX_PUMP_H

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
//
// Pauses all active streams in the media source.
// The media source must be in the started state. The method fails if the media source is paused or stopped.
afxError AmxStopPump(amxPump msrc, afxBool pause);

// Starts, seeks, or restarts the media source by specifying where to start playback.
// A call to Start results in a seek if the previous state was started or paused, and the new starting position is not VT_EMPTY. Not every media source can seek.
afxError AfxStartPump(amxPump msrc, afxBool resume, afxSize at);

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

typedef enum amxPumpType
{
    amxPumpType_MUX,
    amxPumpType_DEMUX
} amxPumpType;

AFX_DEFINE_STRUCT(amxPumpConfig)
{
    afxMask exuMask;
    afxUnit latency;
    amxBuffer latencyBuf;
    afxSize latencyBufBase;
    afxUnit latencyBufRange;
    afxUnit iosCnt;
    void* udd;
    afxString tag;
};

AMX afxError AmxAcquireInputPumps
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxPumpConfig const cfgs[],
    amxPump pumps[]
);

AFX_DEFINE_STRUCT(_amxPumpBin)
{
    amxCodec cdc;
    afxUnit idx;
    // This is the fundamental unit of time(in seconds) in terms of which frame timestamps are represented.
    afxRational timeBase;
    // Decoding : pts of the first frame of the stream in presentation order, in stream time base.
    afxInt64 start_time;
    // Decoding : duration of the stream, in stream time base.
    afxInt64 duration;
    // number of frames in this stream if known or 0
    afxInt64 frameCnt;
    afxRational avgFrameRate;
};

#ifdef _AMX_PUMP_C
#ifdef _AMX_PUMP_IMPL
AFX_OBJECT(_amxPump)
#else
AFX_OBJECT(amxPump)
#endif
// Media sources are objects that generate media data. For example, the data might come from a video file, 
// a network stream, or a hardware device, such as a camera. Each media source contains one or more streams, 
// and each stream delivers data of one type, such as audio or video.
{
    // input or output URL.
    afxUnit capsCnt;
    struct
    {
        afxUri128 uri;
        afxStream iob;
        afxSize iobBase;
    } caps[2];

    afxUnit binCnt;
    _amxPumpBin bins[2];

    // Position of the first frame of the component, in AV_TIME_BASE fractional seconds.
    afxInt64 start_time;
    // Duration of the stream, in AV_TIME_BASE fractional seconds.
    afxInt64 duration;
    // Total stream bitrate in bit/s, 0 if not available.
    afxInt64 bit_rate;
};
#endif//_AMX_PUMP_C

AFX_DEFINE_STRUCT(amxPumpStreamInfo)
{
    amxCodec cdc;
    // The stream index in pump.
    afxUnit idx;
    // This is the fundamental unit of time(in seconds) in terms of which frame timestamps are represented.
    afxRational timeBase;
    // Decoding : pts of the first frame of the stream in presentation order, in stream time base.
    afxInt64 start_time;
    // Decoding : duration of the stream, in stream time base.
    afxInt64 duration;
    // number of frames in this stream if known or 0
    afxInt64 frameCnt;
    afxRational avgFrameRate;
};

#endif//AMX_PUMP_H
