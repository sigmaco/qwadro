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

#ifndef AMX_SINK_DDK_H
#define AMX_SINK_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioDDK.h"
#include "afxMixContextDDK.h"
#include "mpu/amxMpuExecutor.h"
#include "amxSoundscapeDDK.h"
#include "afxMixSystemDDK.h"

typedef struct
// Overwrite-on-Full Audio Ring
{
    afxByte* buffer;        // Interleaved audio buffer
    afxUnit bufStride;
    size_t capacity;      // Total frames
    size_t channels;      // Channels per frame
    size_t write_pos;     // Absolute frame index (can overflow safely)
    size_t read_pos;      // Absolute frame index (can overflow safely)
} AudioRingBuffer;

AFX_DECLARE_STRUCT(_amxSinkIdd);
AFX_DEFINE_STRUCT(_amxSinkDdi)
{
    afxError(*iddDtorCb)(afxSink);
    afxBool(*getIddCb)(afxSink, afxUnit, void*);

    afxError(*lockCb)(afxSink asi, afxUnit64 timeout, afxMask exuMask, afxUnit minFrameCnt, amxBufferedTrack* room);
    afxError(*unlockCb)(afxSink asi, afxFlags flags);
    void(*flushCb)(afxSink asi);
    afxError(*pushCb)(afxSink asio, amxAudio buf, amxAudioPeriod const* seg);
    afxError(*pullCb)(afxSink asio, afxUnit, void*, afxUnit*);
    afxUnit(*getAvailFramesCb)(afxSink asi);

    afxError(*pauseCb)(afxSink asi, afxBool pause);
    afxError(*resetCb)(afxSink asi);

    afxBool (*shouldProcessCb)(afxSink asi, afxUnit idx);
    afxError(*processCb)(afxSink asi, afxUnit idx, amxSample const* samp);
};

#ifdef _AMX_SINK_C
#ifdef _AMX_SINK_IMPL
AFX_OBJECT(_amxSink)
#else
AFX_OBJECT(afxSink)
#endif
{
    _amxSinkDdi const*  ddi;
    _amxSinkIdd*        idd;
    afxString const     tag;
    void*               udd; // user-defined data

    afxUnit             trakCnt;
    amxTrack            traks[8]; // temporarily fixed/static.

    amxFormat           fmt;
    afxUnit             freq; // Hz
    afxUnit             chanCnt;
    afxUnit             framesPerSample; // in samples
    afxUnit             latency; // frames ready

    afxUnit             muteReqCnt;

    afxClock    startClock;
    afxClock    lastClock;
    afxClock    outCntResetClock;
};
#endif//_AMX_SINK_C

AMX int audio_ringbuffer_init(AudioRingBuffer* rb, void* bufPtr, afxUnit bufStride, size_t capacity_frames, size_t channels);

AMX void audio_ringbuffer_free(AudioRingBuffer* rb);

AMX void audio_ringbuffer_write(AudioRingBuffer* rb, const afxByte* input, afxUnit srcStride, size_t frames);
AMX void audio_ringbuffer_advance_write(AudioRingBuffer* rb, size_t frames);

AMX size_t audio_ringbuffer_read(AudioRingBuffer* rb, afxByte* output, afxUnit dstStride, size_t max_frames);
AMX size_t audio_ringbuffer_advance_read(AudioRingBuffer* rb, size_t max_frames);

AMX size_t audio_ringbuffer_available(const AudioRingBuffer* rb);

AMX size_t audio_ringbuffer_writable(const AudioRingBuffer* rb);

#endif//AMX_SINK_DDK_H
