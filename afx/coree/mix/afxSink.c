/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#include "amxIcd.h"
#include "qwadro/mix/afxSink.h"

_AMX afxMixSystem AmxGetSinkHost(afxSink sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixSystem msys = AfxGetHost(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys;
}

_AMX afxMixDevice AfxGetAudioSinkDevice(afxSink sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixSystem msys = AmxGetSinkHost(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxMixDevice mdev = AfxGetHost(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev;
}

_AMX afxError AmxGetSinkIdd(afxSink sink, afxUnit code, void* dst)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    sink->getIddCb(sink, code, dst);
    return err;
}

_AMX afxError AfxMuteAudioSink(afxSink sink, afxBool mute)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);

    sink->muteReqCnt += !!mute;

    return err;
}

_AMX afxError AmxGetSinkTrack(afxSink sink, amxAudio* track)
{
    afxError err = { 0 };
    // sink must be a valid afxSink handle.
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    *track = sink->buffers[0];
    return err;
}

_AMX afxError AmxLockSinkBuffer(afxSink sink, afxUnit64 timeout, afxMask exuMask, afxUnit minFrameCnt, amxBufferedTrack* room)
// Pull an available sink buffer
{
    afxError err = { 0 };
    // sink must be a valid afxSink handle.
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxUnit bufIdx2 = AFX_INVALID_INDEX;

#if !0
    if (sink->lockCb)
    {
        if (!(err = sink->lockCb(sink, timeout, exuMask, minFrameCnt, room)))
        {
            //AFX_ASSERT(AFX_INVALID_INDEX != bufIdx2);
            //AFX_ASSERT_RANGE(sink->latency, bufIdx2, 1);
        }
    }
#else
    else
    {
        afxBool success = FALSE;
        afxTime time, t2;
        AfxGetTime(&time);

        afxClock start, last;

        if (timeout)
        {
            AfxGetClock(&start);
            last = start;
        }

        while (1)
        {
            afxUnit lockedBufIdx = AFX_INVALID_INDEX;

            if (AfxPopInterlockedQueue(&sink->freeBuffers, &lockedBufIdx))
            {
                amxAudio wav = sink->buffers[lockedBufIdx];

                if (wav)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_AUD, 1, &wav);
                }
                bufIdx2 = lockedBufIdx;
                success = TRUE;
            }

            if (success)
                break;

            if (!success && (!timeout || timeout < AfxGetTime(&t2) - time))
            {
                err = __LINE__;
                bufIdx2 = AFX_INVALID_INDEX;
                break;
            }
        }
    }
    AFX_ASSERT(bufIdx);
    *bufIdx = bufIdx2;
#endif
    return err;
}

_AMX afxError AmxUnlockSinkBuffer(afxSink sink, afxFlags flags)
{
    afxError err = { 0 };
    // sink must be a valid afxSink handle.
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
#if 0
    AFX_ASSERT_RANGE(sink->latency, bufIdx, 1);

    if (bufIdx < sink->latency)
        AfxPushInterlockedQueue(&sink->freeBuffers, (afxUnit[]) { bufIdx });
    else
        AfxThrowError();
#else
    if (sink->unlockCb)
    {
        if (!(err = sink->unlockCb(sink, flags)))
        {

        }
    }
#endif
    return err;
}

_AMX afxError _AmxAsioDtorCb(afxSink sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);
    afxMixDevice mdev = AfxGetAudioSinkDevice(sink);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxExhaustInterlockedQueue(&sink->freeBuffers);
    AfxExhaustInterlockedQueue(&sink->readyBuffers);

    // Dispose all acquired buffer objects.
    AfxDisposeObjects(sink->latency, sink->buffers);

    afxObjectStash const stashs[] =
    {
        {
            .cnt = sink->latency,
            .siz = sizeof(sink->buffers[0]),
            .var = (void**)&sink->buffers
        }
    };
    AfxDeallocateInstanceData(sink, ARRAY_SIZE(stashs), stashs);

    if (sink->iddDtorCb)
        sink->iddDtorCb(sink);

    AFX_ASSERT(!sink->idd);

    return err;
}

_AMX afxError _AmxAsioCtorCb(afxSink sink, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };

    AfxZero(sink, sizeof(sink[0]));

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxSinkConfig const* cfg = ((afxSinkConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    sink->udd = cfg->udd;

    afxSinkConfig def;
    AfxConfigureAudioSink(msys, &def);

    sink->fmt = cfg->fmt ? cfg->fmt : def.fmt;
    sink->chanCnt = cfg->chanCnt ? cfg->chanCnt : def.chanCnt;
    sink->freq = cfg->freq ? cfg->freq : def.freq;
    sink->samplesPerFrame = cfg->samplesPerFrame ? cfg->samplesPerFrame : def.samplesPerFrame;
    sink->latency = cfg->latency ? cfg->latency : def.latency;

    afxObjectStash const stashs[] =
    {
        {
            .cnt = sink->latency,
            .siz = sizeof(sink->buffers[0]),
            .var = (void**)&sink->buffers
        }
    };

    if (AfxAllocateInstanceData(sink, ARRAY_SIZE(stashs), stashs))
    {
        AfxThrowError();
        return err;
    }

    AfxZero(sink->buffers, sizeof(sink->buffers[0]) * sink->latency);

    AfxMakeInterlockedQueue(&sink->freeBuffers, sizeof(afxUnit), sink->latency);
    AfxMakeInterlockedQueue(&sink->readyBuffers, sizeof(afxUnit), sink->latency);

    amxAudioInfo audi = { 0 };
    audi.chanCnt = sink->chanCnt;
    audi.fmt = sink->fmt;
    audi.freq = sink->freq;
    audi.sampCnt = sink->freq * AFX_MAX(1, sink->latency);
    audi.segCnt = 1;
    audi.udd = sink;

    if (AmxAcquireAudios(msys, 1, &audi, &sink->buffers[0]))
    {
        AfxThrowError();
        // Dispose all objects acquire up to this iteration.
        //AfxDisposeObjects(1, &sink->buffers[0]);
    }

    // Enqueue our buffer into the queue of disponible buffers.
    AfxPushInterlockedQueue(&sink->freeBuffers, (afxUnit[]) { 0 });
        
    audio_ringbuffer_init(&sink->rb, (void*)AmxGetBufferAddress(AmxGetAudioBuffer(sink->buffers[0]), 0), 4, sink->freq, 2);

    if (err)
        AfxDeallocateInstanceData(sink, ARRAY_SIZE(stashs), stashs);
    
    return err;
}

_AMX afxClassConfig const _AMX_ASIO_CLASS_CONFIG =
{
    .fcc = afxFcc_ASIO,
    .name = "Sink",
    .desc = "Audio Stream Input/Output",
    .fixedSiz = sizeof(AFX_OBJECT(afxSink)),
    .ctor = (void*)_AmxAsioCtorCb,
    .dtor = (void*)_AmxAsioDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxConfigureAudioSink(afxMixSystem msys, afxSinkConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cfg);
    *cfg = (afxSinkConfig) { 0 };
    cfg->fmt = amxFormat_S32f; // 32-bit stereo
    cfg->freq = 48000; // DVD
    cfg->chanCnt = 2; // stereo
    cfg->samplesPerFrame = cfg->freq / 60;
    cfg->latency = 6;
    return err;
}

_AMX afxError AfxOpenAudioSink(afxMixSystem msys, afxSinkConfig const* cfg, afxSink* sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cfg);

    afxClass* cls = (afxClass*)_AmxMsysGetSinkClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    afxSink snk;
    afxBool record = FALSE;

    if (AfxAcquireObjects(cls, 1, (afxObject*)&snk, (void const*[]) { msys, cfg, &record }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &snk);
    AFX_ASSERT(sink);
    *sink = snk;
    return err;
}

_AMX afxError AmxFlushSinks(afxMixSystem msys, afxUnit cnt, amxFlush const flushes[], afxUnit queueingMap[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(flushes);
    AFX_ASSERT(cnt);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo,
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine.
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine.
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */

    for (afxUnit doutIt = 0; doutIt < cnt; doutIt++)
    {
        amxFlush const* pres = &flushes[doutIt];

        afxSink sink = pres->sink;
        if (!sink)
        {
            AFX_ASSERT(sink);
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);

        afxUnit sampleCnt = pres->sampleCnt;
        if (sampleCnt >= sink->samplesPerFrame)
        {
            AFX_ASSERT_RANGE(sink->samplesPerFrame, sampleCnt, 1);
            continue;
        }

        afxMask exuMask = pres->exuMask;
        afxUnit exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIdx = nextExuIdx; exuIdx < exuCnt; exuIdx++)
            {
                nextExuIdx = 0;

                if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                    continue;

                afxMixBridge mexu;
                if (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
                {
                    AfxThrowError();
                    return err;
                }

                afxError err2 = _AmxMexuFlushSinks(mexu, 1, &flushes[doutIt], queueingMap);
                err = err2;

                if (!err2)
                {
                    queued = TRUE;
                    break; // for --- iterate bridges
                }

                if ((err2 == afxError_TIMEOUT) || (err2 == afxError_BUSY))
                {
                    continue;
                }

                AfxThrowError();
            }

            if (err || queued)
                break; // while --- find bridges
        }

        if (err || queued)
            break; // for
    }
    return err;
}

_AMX afxError AmxRefillSinks(afxMixSystem msys, afxUnit cnt, amxCaption const captions[], afxUnit queueingMap[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys);
    AFX_ASSERT(captions);
    AFX_ASSERT(cnt);

    for (afxUnit doutIt = 0; doutIt < cnt; doutIt++)
    {
        amxCaption const* cap = &captions[doutIt];

        afxSink sink = cap->sink;
        if (!sink)
        {
            AFX_ASSERT(sink);
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &sink);

        afxUnit sampleCnt = cap->sampleCnt;
        if (sampleCnt >= sink->samplesPerFrame)
        {
            AFX_ASSERT_RANGE(sink->samplesPerFrame, sampleCnt, 1);
            continue;
        }

        afxMask exuMask = cap->exuMask;
        afxUnit exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIdx = nextExuIdx; exuIdx < exuCnt; exuIdx++)
            {
                nextExuIdx = 0;

                if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                    continue;

                afxMixBridge mexu;
                if (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
                {
                    AfxThrowError();
                    return err;
                }

                afxError err2 = _AmxMexuRefillSinks(mexu, 1, &captions[doutIt], queueingMap);
                err = err2;

                if (!err2)
                {
                    queued = TRUE;
                    break; // for --- iterate bridges
                }

                if ((err2 == afxError_TIMEOUT) || (err2 == afxError_BUSY))
                {
                    continue;
                }

                AfxThrowError();
            }

            if (err || queued)
                break; // while --- find bridges
        }

        if (err || queued)
            break; // for
    }
    return err;
}
