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

#define _CRT_SECURE_NO_WARNINGS 1
//#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include "zalSdev.h"
#pragma comment(lib, "mmdevapi")

AFX_DEFINE_STRUCT(zalOalSinkIdd)
{
    afxModule oalDll;
    alcVmt alc;
    alVmt al;
    ALCdevice*alDev;
    ALCcontext*alCtx;
    ALuint alBufs[2];
    ALuint alSrc;
    void* bufData;
    afxUnit bufDataSiz;
    amxAudio frames[2];
};

_ZAL afxUnit _ZalAsioGetAvailableFrames_Parture(afxSink asi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);
    return 0;
}

_ZAL afxError _ZalPushAsioDataCb(afxSink asi, amxAudio buf, amxAudioPeriod const* seg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    zalOalSinkIdd* idd = asi->m.idd;

    ALint state;
    ALuint buffer = 0;
    ALint buffersProcessed = 0;
    idd->al.GetSourcei(idd->alSrc, AL_BUFFERS_PROCESSED, &buffersProcessed);

    //while (buffersProcessed--)
    {
        idd->al.SourceUnqueueBuffers(idd->alSrc, 1, &buffer);

        void* src = &buf->m.bytemap[0];
        afxUnit siz = seg->chanCnt * seg->sampCnt * buf->m.fmtStride;

        idd->al.BufferData(buffer, AL_FORMAT_STEREO16, src, siz, asi->m.freq);

        idd->al.SourceQueueBuffers(idd->alSrc, 1, &buffer);
    }

    idd->al.GetSourcei(idd->alSrc, AL_SOURCE_STATE, &state);

    if (state != AL_PLAYING)
        idd->al.SourcePlay(idd->alSrc);

    return err;
}

_ZAL void _ZalOalSinkFlush(afxSink asi)
{
    afxError err = NIL;
    zalOalSinkIdd* idd = asi->m.idd;

    if (!idd)
        return;

    while (!AfxIsInterlockedQueueEmpty(&asi->m.readyBuffers))
    {
        ALint buffersProcessed = 0;
        idd->al.GetSourcei(idd->alSrc, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (!buffersProcessed)
            break;

        afxUnit bufIdx;
        if (!AfxPopInterlockedQueue(&asi->m.readyBuffers, &bufIdx))
            break;

        amxAudio buf = asi->m.buffers[bufIdx];
        amxAudioCopy op = { 0 };
        op.src.sampCnt = buf->m.chanCnt;
        op.src.chanCnt = buf->m.sampCnt;
        //_AmxTransposeAudio(buf, buf, &op); // interleave it before passing out.

        ALuint buffer = 0;
        idd->al.SourceUnqueueBuffers(idd->alSrc, 1, &buffer);
        idd->al.BufferData(buffer, AL_FORMAT_STEREO16, buf->m.data, buf->m.bufCap, asi->m.freq);
        idd->al.SourceQueueBuffers(idd->alSrc, 1, &buffer);

        if (!AfxPushInterlockedQueue(&asi->m.freeBuffers, &bufIdx))
            AfxThrowError();

        // Start playing it if it was paused.

        ALint state;
        idd->al.GetSourcei(idd->alSrc, AL_SOURCE_STATE, &state);

        if (state != AL_PLAYING)
            idd->al.SourcePlay(idd->alSrc);
    }
}

_ZAL void _ZalSpuInitOalSink(amxMpu* spu, afxSink asi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxUri uri;
    afxModule oalDll;
    AfxMakeUri(&uri, 0, "//./c/openal32.dll", 0);
    if (AfxLoadModule(&uri, NIL, &oalDll))
    {
        AfxThrowError();
        return;
    }
    
    // TODO detect features and limits

    if (err)
        AfxDisposeObjects(1, &oalDll);

    zalOalSinkIdd* idd;
    if (AfxAllocate(sizeof(zalOalSinkIdd), 0, AfxHere(), (void**)&idd))
    {
        AfxThrowError();
        return;
    }

    idd->oalDll = oalDll;
    _ZalSpuLoadBaseSymbols(AfxGetModuleIdd(oalDll), &idd->alc.ptr);
    _ZalLoadOpenAlVmt(AfxGetModuleIdd(oalDll), 0, (sizeof(alVmt) / sizeof(void*)), alVmtNames, &idd->al.ptr, FALSE);

    idd->alDev = idd->alc.OpenDevice(NIL);
    idd->alCtx = idd->alc.CreateContext(idd->alDev, NIL);
    
    if (!idd->alc.MakeContextCurrent(idd->alCtx))
        AfxThrowError();

    idd->al.GenBuffers(2, idd->alBufs);
    idd->al.GenSources(1, &idd->alSrc);

    idd->bufDataSiz = asi->m.freq;

    if (AfxAllocate(idd->bufDataSiz, 0, AfxHere(), (void**)&idd->bufData))
    {
        AfxThrowError();
        //return err;
    }

    amxAudioInfo audi = { 0 };
    audi.chanCnt = asi->m.chanCnt;
    audi.fmt = amxFormat_S16i; // asi->m.fmt;
    audi.freq = asi->m.freq;
    audi.sampCnt = asi->m.samplesPerFrame;
    afxMixSystem ssys = AfxGetHost(asi);
    AmxAcquireAudios(ssys, 1, &audi, &idd->frames[0]);
    AmxAcquireAudios(ssys, 1, &audi, &idd->frames[1]);
    //AmxAcquireAudios(ssys, 1, &audi, &idd->frames[2]);

    for (afxUnit i = 0; i < ARRAY_SIZE(idd->alBufs); i++)
    {
        AfxZero(idd->bufData, idd->bufDataSiz);
        idd->al.BufferData(idd->alBufs[i], AL_FORMAT_STEREO16, idd->bufData, idd->bufDataSiz, asi->m.freq);
        idd->al.SourceQueueBuffers(idd->alSrc, 1, &idd->alBufs[i]);
    }

    idd->al.SourcePlay(idd->alSrc);

    asi->m.idd = idd;

    // TODO Dtor

    if (err)
        AfxDeallocate((void**)&idd, AfxHere());
}

_ZAL afxError _ZalAsioDtorCb(afxSink asi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixDevice sdev = AfxGetHost(asi);

    return err;
}

_ZAL afxError _ZalAsioCtorCb(afxSink asi, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &ssys);
    afxSinkConfig const* cfg = ((afxSinkConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    afxBool record = *(afxBool*)(args[2]);

    if (_AMX_ASIO_CLASS_CONFIG.ctor(asi, (void*[]) { ssys, (void*)cfg }, 0)) AfxThrowError();
    else
    {
        asi->m.pushCb = _ZalPushAsioDataCb;
        asi->m.flushCb = _ZalOalSinkFlush;

        afxMixBridge sexu;
        AmxGetMixBridges(ssys, 0, 1, &sexu);
        if (_AmxMexuSubmitCallback(sexu, (void*)_ZalSpuInitOalSink, asi))
            AfxThrowError();

    }
    return err;
}
