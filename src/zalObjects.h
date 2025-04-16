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

#ifndef ZAL_OBJECTS_H
#define ZAL_OBJECTS_H

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
    zalWasapi wasapi;
};

AFX_OBJECT(afxMixBridge)
{
    AFX_OBJECT(_amxMixBridge) m;
    zalSpu                  spu;
    ALCdevice*              alcdev;
    ALCcontext*             alctx;
    afxMixSystem          activeSctx;
    afxUnit                 verMajor, verMinor, verPatch;
    afxString               subsysName;
    afxString               subsysVer;

    afxInterlockedQueue     deletionQueue;
};

AFX_OBJECT(afxSink)
{
    AFX_OBJECT(_amxSink) m;
    union
    {
        zalWasapi wasapi;
        struct
        {
            ma_device_config deviceConfig;
            ma_device device;
            ma_pcm_rb rb;
        } miniaud;
    } idd;
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
    AFX_OBJECT(_amxAudient) m;

};

AFX_OBJECT(amxAudio)
{
    AFX_OBJECT(_amxAudio) m;
    ALuint  alHandle;
    afxFlags updFlags;
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

#endif//ZAL_OBJECTS_H
