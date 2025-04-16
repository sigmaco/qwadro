/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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
    UINT32 bufferFrameCount;
    UINT32 lockedBufferCap;
    BYTE* pRenderData;
    BYTE* pCaptureData;
    WAVEFORMATEX* pwfx;
    REFERENCE_TIME defaultPeriod;
    REFERENCE_TIME minimumPeriod;
    HANDLE bufferReady;
    AUDCLNT_SHAREMODE shareMode;
    DWORD dwClsCtx;
};

ZAL afxError _ZalWasapiCreate(zalWasapi* idd);
ZAL afxError _ZalWasapiDestroy(zalWasapi* idd);
ZAL afxError _ZalWasapiStartStop(zalWasapi* idd, afxBool start);
ZAL afxError _ZalWasapiReadCapture(zalWasapi* idd, afxUnit frameCap, void* dst, afxUnit* frameCnt);
ZAL afxUnit _ZalWasapiGetReadLength(zalWasapi* idd);
ZAL afxError _ZalWasapiWriteParture(zalWasapi* idd, afxUnit frameCnt, void const* src);
ZAL afxUnit _ZalWasapiGetWriteRoom(zalWasapi* idd);


#endif//ZAL_INTEROP_WASAPI_H
