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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qowAudio_W32.h"
#include "../qwadro/icd/amiga/src/zalInteropWasapi.h"
//#include "qowBase.h"

_QOW afxError _QowSinkPauseCb(afxSink asi, afxBool pause)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    _ZalWasapiPause(&asi->idd.wasapi, pause);

    return err;
}

_QOW afxError _QowSinkResetCb(afxSink asi)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    _ZalWasapiReset(&asi->idd.wasapi);

    return err;
}

_QOW afxError _QowSinkLockCb(afxSink asi, afxUnit64 timeout, afxMask exuMask, afxUnit minFrameCnt, amxBufferedTrack* room)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxUnit paddingFrameCnt, availBufFrameCnt;
    afxUnit wRoom = wasapiOutputGetRoom(&asi->idd.wasapi, &paddingFrameCnt, &availBufFrameCnt);
    
    void* p;
    err = wasapiOutputLock(&asi->idd.wasapi, minFrameCnt, &p);

    *room = (amxBufferedTrack) { .offset = (afxSize)p, .frameCnt = asi->idd.wasapi.lockedOutFrameCnt, .freq = asi->m.freq };

    return err;
}

_QOW afxError _QowSinkUnlockCb(afxSink asi, afxFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);
#if 0
    amxAudioIo op = { 0 };
    op.chansPerFrame = 2;
    op.samplesPerChan = 1;
    op.period.chanCnt = 2;
    op.period.sampCnt = asi->idd.wasapi.lockedOutFrameCnt;
    _AmxDumpAudio(asi->m.buffers[0], &op, asi->idd.wasapi.lockedOutPtr);
#endif
    //AfxCopy(asi->idd.wasapi.lockedOutPtr, AmxGetBufferMap(asi->m.buffers[0]->buf, asi->idd.wasapi.lockedOutBaseFrame * 2 * 2, 1), asi->idd.wasapi.lockedOutFrameCnt * 2 * 2);
    err = wasapiOutputUnlock(&asi->idd.wasapi, asi->idd.wasapi.lockedOutFrameCnt, NIL);

    return err;
}

_QOW void _QowSinkFlushCb(afxSink asi)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxUnit paddingFrameCnt, bufFrameCnt;
    afxUnit wRoom = wasapiOutputGetRoom(&asi->idd.wasapi, &paddingFrameCnt, &bufFrameCnt);
#if 0
    if (wRoom)
        _QowWasapiWriteParture(&asi->idd.wasapi, wRoom, NIL);
#endif

    if (bufFrameCnt)
    {
        //AFX_ASSERT(asi->idd.wasapi.bufferFrameCount >= bufFrameCnt - paddingFrameCnt);
        
        afxUnit frameCnt = 0;
        //if ((frameCnt = audio_ringbuffer_available(&asi->m.rb)))
        {
#if 0
            afxUnit minFrameCnt = AFX_MIN(frameCnt, bufFrameCnt);
            void* p;
            err = wasapiOutputLock(&asi->idd.wasapi, minFrameCnt, &p);
            if (!err)
            {
                audio_ringbuffer_read(&asi->m.rb, p, 4, asi->idd.wasapi.lockedOutFrameCnt);
                err = wasapiOutputUnlock(&asi->idd.wasapi, asi->idd.wasapi.lockedOutFrameCnt, NIL);
            }
#else
            if (asi->idd.wasapi.pRenderClient)
            {
                //wasapiOutputRb(&asi->idd.wasapi, &asi->m.rb);
            }
            else
            {
                //wasapiInputRb(&asi->idd.wasapi, &asi->m.rb);
            }
#endif
        }
    }
}

_QOW afxBool _QowSinkShouldProcessCb(afxSink asi, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    return wasapiShouldProcess(&asi->idd.wasapi);
}

_QOW afxError _QowSinkProcessSampleCb(afxSink asi, afxUnit idx, amxSample const* samp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxUnit paddingFrameCnt, availBufFrameCnt;
    afxUnit wRoom = wasapiOutputGetRoom(&asi->idd.wasapi, &paddingFrameCnt, &availBufFrameCnt);

    afxUnit minFrameCnt = samp->iob.range / samp->iob.stride;// asi->m.freq * samp->durTime;
    minFrameCnt = AFX_MIN(minFrameCnt, availBufFrameCnt);

    afxByte* p;
    err = wasapiOutputLock(&asi->idd.wasapi, minFrameCnt, (void**)&p);

    afxSize addr = AmxGetBufferedStreamAddress(&samp->iob);

    AfxStream2(minFrameCnt*2, (void*)addr, samp->iob.stride/2, p, asi->m.chanCnt * sizeof(float)/2);

    err = wasapiOutputUnlock(&asi->idd.wasapi, asi->idd.wasapi.lockedOutFrameCnt, NIL);

    return err;
}

_QOW _amxSinkDdi const _QOW_SINK_IMPL =
{
    .flushCb = _QowSinkFlushCb,
    .lockCb = _QowSinkLockCb,
    .unlockCb = _QowSinkUnlockCb,
    .pauseCb = _QowSinkPauseCb,
    .resetCb = _QowSinkResetCb,
    .shouldProcessCb = _QowSinkShouldProcessCb,
    .processCb = _QowSinkProcessSampleCb,
};

_QOW afxError _QowSinkDtorCb(afxSink asi)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixDevice sdev = AfxGetHost(asi);

    _ZalWasapiPause(&asi->idd.wasapi, 1);
    _ZalWasapiReset(&asi->idd.wasapi);

    if (_ZalWasapiDestroy(&asi->idd.wasapi))
    {
        AfxThrowError();
    }

    return err;
}

_QOW afxError _QowSinkCtorCb(afxSink asi, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxSinkConfig const* cfg = ((afxSinkConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    afxBool record = *(afxBool*)(args[2]);

    afxUri dev;
    afxUri endpoint;
    //AfxExcerptUriDevice(&dev, &endpoint);
    
    "//./wasapi/mic";
    "//./wasapi/aux";
    "//./wasapi/mix";


    if (_AMX_ASIO_CLASS_CONFIG.ctor(asi, (void*[]) { msys, (void*)cfg }, 0))
    {
        AfxThrowError();
        return err;
    }
    
    if (_ZalWasapiCreate(&asi->idd.wasapi, asi->m.fmt, asi->m.chanCnt, asi->m.freq, cfg->latency, 0, FALSE))
    {
        AfxThrowError();
    }

    asi->m.ddi = &_QOW_SINK_IMPL;

    _ZalWasapiPause(&asi->idd.wasapi, 0);

    return err;
}
