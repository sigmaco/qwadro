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

#ifndef ZAL_OBJECTS_H
#define ZAL_OBJECTS_H

#include <d3d9.h>
#include <dxva.h>
#include <dxva2api.h>
#include <d3dhalex.h>

#include "zalBase.h"
#include "zalInteropOal.h"
#include "zalInteropWasapi.h"

AFX_DEFINE_STRUCT(zalSpu)
{
    alVmt const al;
    afxUnit         exuIdx;
    afxUnit         portId; // exuIdx
    afxMixSystem  activeSctx;

    afxBool         eaxEnabled;

    afxBool         running;
    afxBool         instanced;

    afxMixContext        activeMix;
    afxReal         bufA[4096];
    afxReal         bufB[4096];
};

struct _afxSdevIdd
{
    afxModule   oalDll;
    afxUnit     spuCnt;
    zalSpu*     spus;
};

AFX_DEFINE_STRUCT(_zalDeleteAlRes)
{
    union
    {
        ALuint  handle;
        void*   handlePtr;
    };
    ALuint  type;
};

AFX_OBJECT(afxMixDevice)
{
    AFX_OBJECT(_amxMixDevice) m;
    afxModule   oalDll;
    afxUnit     spuCnt;
    zalSpu*     spus;
};

AFX_OBJECT(afxMixSystem)
{
    AFX_OBJECT(_amxMixSystem) m;

    // DXVA
    IDirect3D9Ex* d3d9;
    unsigned resetToken;
    UINT adapter;
    HANDLE deviceHandle;
    IDirect3DDeviceManager9* d3d9devmgr;
    IDirect3DDevice9Ex* d3d9device;
    IDirectXVideoDecoderService* decoder_service;
};

AFX_OBJECT(afxMixBridge)
{
    AFX_OBJECT(_amxMixBridge) m;
    zalSpu                  spu;

};

AFX_OBJECT(amxSink)
{
    AFX_OBJECT(_amxSink) m;
};

AFX_OBJECT(amxBuffer)
{
    AFX_OBJECT(_amxBuffer) m;
};

AFX_OBJECT(afxSemaphore)
{
    AFX_OBJECT(_afxSemaphore) m;
};

AFX_OBJECT(amxSound)
{
    AFX_OBJECT(_amxSound) m;

};

AFX_OBJECT(amxSoundscape)
{
    AFX_OBJECT(_amxSoundscape) m;

};

AFX_OBJECT(amxAudio)
{
    AFX_OBJECT(_amxAudio) m;
    ALuint  alHandle;
    afxFlags updFlags;
};

AFX_OBJECT(amxVideo)
{
    AFX_OBJECT(_amxVideo) m;
};

AFX_OBJECT(amxPump)
{
    AFX_OBJECT(_amxPump) m;
};

AFX_OBJECT(afxMixContext)
{
    AFX_OBJECT(_amxMixContext) m;

};

ZAL afxBool _SdgeProcCb(afxMixBridge sdge, afxThread thr);

ZAL afxError _ZalAsioDtorCb(afxSink asio);
ZAL afxError _ZalAsioCtorCb(afxSink asio, void** args, afxUnit invokeNo);

ZAL afxError _ZalRelinkAsioWasapiCb(afxMixDevice sdev, afxMixSystem ssys, afxUnit cnt, afxSink asios[]);

ZAL afxError _SpuLoadWav(zalSpu* spu, amxAudio wav, afxByte const* src, afxUnit opCnt, amxAudioIo const* ops);
ZAL afxError _SpuStoreWav(zalSpu* spu, amxAudio wav, afxByte* dst, afxUnit opCnt, amxAudioIo const* ops);
ZAL afxError _SpuOutputWav(zalSpu* spu, amxAudio wav, afxStream out, afxUnit opCnt, amxAudioIo const* ops);
ZAL afxError _SpuInputWav(zalSpu* spu, amxAudio wav, afxStream in, afxUnit opCnt, amxAudioIo const* ops);

ZAL afxError _ZalAsioInitOalCb(afxSink asi, afxSinkConfig const* cfg);

ZAL afxError _ZalMsysCtorCb(afxMixSystem ssys, void** args, afxUnit invokeNo);
ZAL afxError _ZalMsysDtorCb(afxMixSystem ssys);

ZAL afxError _ZalVidDtorCb(amxVideo vid);
ZAL afxError _ZalVidCtorCb(amxVideo vid, void** args, afxUnit invokeNo);

ZAL afxError _ZalTraxDtorCb(amxTracker trax);
ZAL afxError _ZalTraxCtorCb(amxTracker trax, void** args, afxUnit invokeNo);

ZAL afxError _ZalMpmpDtorCb(amxPump pmp);
ZAL afxError _ZalMpmpCtorCb(amxPump pmp, void** args, afxUnit invokeNo);

ZAL afxError _ZalMbufDtorCb(amxBuffer mbuf);
ZAL afxError _ZalMbufCtorCb(amxBuffer mbuf, void** args, afxUnit invokeNo);

ZAL afxError _ZalMexuDtorCb(afxMixBridge mexu);
ZAL afxError _ZalMexuCtorCb(afxMixBridge mexu, void** args, afxUnit invokeNo);

#endif//ZAL_OBJECTS_H
