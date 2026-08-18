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

#ifndef AMX_TRACK_H
#define AMX_TRACK_H

#include "qwadro/mix/afxMixDefs.h"
#include "qwadro/mix/amxFormat.h"

typedef enum amxTrackUsage
{
    amxTrackUsage_SAMPLED = AFX_BITMASK(0),
    amxTrackUsage_MIX = AFX_BITMASK(1),
    amxTrackUsage_SINK = AFX_BITMASK(2)
} amxTrackUsage;

typedef enum amxTrackFlag
{
    amxTrackFlag_RESERVED = AFX_BITMASK(31)
} amxTrackFlags;

AFX_DEFINE_STRUCT(amxTrackInfo)
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         freq; // the sample rate, the number of sample frames played per second. (SPF)
    afxUnit         frameCnt; // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).
    afxUnit         segCnt;
    afxUnit         siz;

    afxString       tag;
    void*           udd;
};

AFX_DEFINE_STRUCT(amxTrackPeriod)
{
    afxUnit         clipIdx;

    // The base (starting) index for samples in the audio data block.
    // It specifies where the sample data begins within the block.
    afxUnit         baseFrame; // X

    // The base (starting) index for channels.
    // It determines where the channel data begins in the audio structure.
    afxUnit         baseChan; // Y

    afxUnit         baseSeg;

    // The count of samples to be handled. 
    // This determines how many samples are involved in the operation, starting from the baseSamp position.
    afxUnit         frameCnt; // W
    
    // The count of channels to be handled. 
    // It specifies how many audio channels are involved, starting from the baseChan position.
    afxUnit         chanCnt; // H

    afxUnit         segCnt;

    // Arbitrary origins.
    afxUnit x, y, z;
    // Arbitrary ranges.
    afxUnit w, h, d;
};

AFX_DEFINE_STRUCT(amxTrackCopy)
{
    amxTrackPeriod  src;
    afxUnit         dstBaseFrame; // X
    afxUnit         dstBaseChan; // Y
    afxUnit         dstBaseSeg; // Z
};

AFX_DEFINE_STRUCT(amxTrackInterference)
{
    amxTrackPeriod  src;
    afxUnit         srcFreq;
    amxTrackPeriod  dst;
    afxUnit         dstFreq;
};

AFX_DEFINE_STRUCT(amxTrackIo)
/// Especificação de operação de transferência arbitrária de afxWaveform.
{
    amxTrackPeriod  period;

    // An additional offset (in bytes) that might be used in input/output operations 
    // to locate data more precisely within a buffer or stream.
    afxSize         offset;
    
    // The number of samples per channel in the upload buffer. 
    // This helps in calculating the memory layout when accessing the upload buffer.
    afxUnit         framesPerChan;
    
    // The number of rows in the upload buffer per frame in the audio object. 
    // This helps to understand how many sample rows exist in the buffer per frame for multi-channel layouts.
    afxUnit         chansPerSample;

    // freq?
    //amxFormat       fmt;
};

AMX amxBuffer   AmxGetTrackBuffer(amxTrack trak);

/// Returns a real representing the duration, in seconds, of the PCM data stored in the buffer.
AMX afxReal     AmxGetTrackDuration(amxTrack trak);

AMX void        AmxDescribeTrack(amxTrack trak, amxTrackInfo* desc);

AMX afxError    AmxPackTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], amxBuffer buf, amxFence signal);
AMX afxError    AmxUnpackTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], amxBuffer buf, amxFence signal);

AMX afxError    AmxUploadTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], afxStream in, amxFence signal);
AMX afxError    AmxDownloadTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], afxStream out, amxFence signal);

AMX afxError    AmxDumpTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], void* dst, amxFence signal);
AMX afxError    AmxUpdateTrack(amxTrack trak, afxUnit opCnt, amxTrackIo const ops[], void const* src, amxFence signal);

AMX afxError    AmxPrintTrack(amxTrack trak, amxTrackPeriod const* op, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(amxTrackConfig)
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         freq; // the sample rate, the number of sample frames played per second. (SPF)
    afxUnit         frameCnt; // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // the number of channels (1 for mono, 2 for stereo, etc.).
    afxUnit         segCnt;
    afxUnit         siz;

    afxString       tag;
    void*           udd;
};

AMX afxError    AmxAcquireTracks(afxMixSystem msys, afxUnit cnt, amxTrackConfig const cfgs[], amxTrack tracks[]);

AMX afxError    AmxLoadTracks(afxMixSystem msys, afxUnit cnt, afxUri const uris[], amxTrack tracks[]);

#endif//AMX_TRACK_H
