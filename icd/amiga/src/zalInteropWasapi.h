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

#ifndef ZAL_INTEROP_WASAPI_H
#define ZAL_INTEROP_WASAPI_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include "zalBase.h"

AFX_DEFINE_STRUCT(zalWasapi)
{
    IMMDeviceEnumerator* pEnumerator;
    IMMDevice* pDevice;
    IAudioClient* pAudioClient;
    IAudioRenderClient* pRenderClient;
    IAudioCaptureClient* pCaptureClient;
    afxBool exclusive;
    afxUnit64 defPeriodNs;
    afxUnit64 minPeriodNs;
    UINT32 bufferFrameCount;
    UINT32 lockedBufferCap;
    BYTE* pRenderData;
    BYTE* pCaptureData;
    WAVEFORMATEXTENSIBLE* pwfx;
    /*
        By default, WASAPI in shared mode uses:
        Interleaved audio
        32-bit IEEE floating point (float) samples
        Little-endian byte order
        Channel layout matching the system's speaker configuration (stereo, 5.1, etc.)
    */
    REFERENCE_TIME defaultPeriod;
    REFERENCE_TIME minimumPeriod;
    HANDLE bufferReady;
    AUDCLNT_SHAREMODE shareMode;
    DWORD dwClsCtx;
    afxUnit lockedOutBaseFrame;
    afxUnit lockedOutFrameCnt;
    void*   lockedOutPtr;
    afxUnit lockedInBase;
    afxUnit lockedInFrameCnt;
    void*   lockedInPtr;
};

ZAL afxError _ZalWasapiCreate(zalWasapi* idd, amxFormat fmt, afxUnit chCnt, afxUnit sampRate, afxUnit latency/*scaler*/, afxUnit periodicity/*ns*/, afxBool exclusive);
ZAL afxError _ZalWasapiDestroy(zalWasapi* idd);
ZAL afxError _ZalWasapiReadCapture(zalWasapi* idd, afxUnit frameCap, void* dst, afxUnit* frameCnt);
ZAL afxError _ZalWasapiWriteParture(zalWasapi* idd, afxUnit frameCnt, void const* src);

ZAL afxError _ZalWasapiPause(zalWasapi* idd, afxBool pause);
ZAL afxError _ZalWasapiReset(zalWasapi* idd);

ZAL afxUnit     wasapiInputGetLength(zalWasapi* idd);
ZAL afxError    wasapiInputUnlock(zalWasapi* idd, afxUnit frameCnt);
ZAL afxError    wasapiInputLock(zalWasapi* idd, void** pDst, afxUnit* frameCnt);

ZAL afxUnit     wasapiOutputGetRoom(zalWasapi* idd, afxUnit* paddingFrameCnt, afxUnit* availFrameCnt);
ZAL afxError    wasapiOutputUnlock(zalWasapi* idd, afxUnit frameCnt, afxFlags flags);
ZAL afxError    wasapiOutputLock(zalWasapi* idd, afxUnit frameCnt, void** pMap);

ZAL afxError wasapiOutputRb(zalWasapi* idd, AudioRingBuffer* rb);
ZAL afxError wasapiInputRb(zalWasapi* idd, AudioRingBuffer* rb);

#endif//ZAL_INTEROP_WASAPI_H
