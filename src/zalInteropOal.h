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

#ifndef ZAL_INTEROP_OAL_H
#define ZAL_INTEROP_OAL_H

#define AL_LIBTYPE_STATIC 1
#include "../dep/al/al.h"
#include "../dep/al/alc.h"
#include "../dep/al/efx.h"
#include "zalBase.h"

AFX_DEFINE_UNION(alcVmt)
{
    struct
    {
        LPALCCREATECONTEXT CreateContext;
        LPALCMAKECONTEXTCURRENT MakeContextCurrent;
        LPALCPROCESSCONTEXT ProcessContext;
        LPALCSUSPENDCONTEXT SuspendContext;
        LPALCDESTROYCONTEXT DestroyContext;
        LPALCGETCURRENTCONTEXT GetCurrentContext;
        LPALCGETCONTEXTSDEVICE GetContextsDevice;
        LPALCOPENDEVICE OpenDevice;
        LPALCCLOSEDEVICE CloseDevice;
        LPALCGETERROR GetError;
        LPALCISEXTENSIONPRESENT IsExtensionPresent;
        LPALCGETPROCADDRESS GetProcAddress;
        LPALCGETENUMVALUE GetEnumValue;
        LPALCGETSTRING GetString;
        LPALCGETINTEGERV GetIntegerv;
        LPALCCAPTUREOPENDEVICE CaptureOpenDevice;
        LPALCCAPTURECLOSEDEVICE CaptureCloseDevice;
        LPALCCAPTURESTART CaptureStart;
        LPALCCAPTURESTOP CaptureStop;
        LPALCCAPTURESAMPLES CaptureSamples;
    };
    void* ptr;
};

AFX_DEFINE_UNION(alVmt)
{
    struct
    {
        // 110
        LPALENABLE Enable;
        LPALDISABLE Disable;
        LPALISENABLED IsEnabled;
        LPALGETSTRING GetString;
        LPALGETBOOLEANV GetBooleanv;
        LPALGETINTEGERV GetIntegerv;
        LPALGETFLOATV GetFloatv;
        LPALGETDOUBLEV GetDoublev;
        LPALGETBOOLEAN GetBoolean;
        LPALGETINTEGER GetInteger;
        LPALGETFLOAT GetFloat;
        LPALGETDOUBLE GetDouble;
        LPALGETERROR GetError;
        LPALISEXTENSIONPRESENT IsExtensionPresent;
        LPALGETPROCADDRESS GetProcAddress;
        LPALGETENUMVALUE GetEnumValue;
        LPALLISTENERF Listenerf;
        LPALLISTENER3F Listener3f;
        LPALLISTENERFV Listenerfv;
        LPALLISTENERI Listeneri;
        LPALLISTENER3I Listener3i;
        LPALLISTENERIV Listeneriv;
        LPALGETLISTENERF GetListenerf;
        LPALGETLISTENER3F GetListener3f;
        LPALGETLISTENERFV GetListenerfv;
        LPALGETLISTENERI GetListeneri;
        LPALGETLISTENER3I GetListener3i;
        LPALGETLISTENERIV GetListeneriv;
        LPALGENSOURCES GenSources;
        LPALDELETESOURCES DeleteSources;
        LPALISSOURCE IsSource;
        LPALSOURCEF Sourcef;
        LPALSOURCE3F Source3f;
        LPALSOURCEFV Sourcefv;
        LPALSOURCEI Sourcei;
        LPALSOURCE3I Source3i;
        LPALSOURCEIV Sourceiv;
        LPALGETSOURCEF GetSourcef;
        LPALGETSOURCE3F GetSource3f;
        LPALGETSOURCEFV GetSourcefv;
        LPALGETSOURCEI GetSourcei;
        LPALGETSOURCE3I GetSource3i;
        LPALGETSOURCEIV GetSourceiv;
        LPALSOURCEPLAYV SourcePlayv;
        LPALSOURCESTOPV SourceStopv;
        LPALSOURCEREWINDV SourceRewindv;
        LPALSOURCEPAUSEV SourcePausev;
        LPALSOURCEPLAY SourcePlay;
        LPALSOURCESTOP SourceStop;
        LPALSOURCEREWIND SourceRewind;
        LPALSOURCEPAUSE SourcePause;
        LPALSOURCEQUEUEBUFFERS SourceQueueBuffers;
        LPALSOURCEUNQUEUEBUFFERS SourceUnqueueBuffers;
        LPALGENBUFFERS GenBuffers;
        LPALDELETEBUFFERS DeleteBuffers;
        LPALISBUFFER IsBuffer;
        LPALBUFFERDATA BufferData;
        LPALBUFFERF Bufferf;
        LPALBUFFER3F Buffer3f;
        LPALBUFFERFV Bufferfv;
        LPALBUFFERI Bufferi;
        LPALBUFFER3I Buffer3i;
        LPALBUFFERIV Bufferiv;
        LPALGETBUFFERF GetBufferf;
        LPALGETBUFFER3F GetBuffer3f;
        LPALGETBUFFERFV GetBufferfv;
        LPALGETBUFFERI GetBufferi;
        LPALGETBUFFER3I GetBuffer3i;
        LPALGETBUFFERIV GetBufferiv;

        LPALDOPPLERFACTOR DopplerFactor;
        LPALDOPPLERVELOCITY DopplerVelocity;
        LPALSPEEDOFSOUND SpeedOfSound;
        LPALDISTANCEMODEL DistanceModel;

        // EFX
        LPALGENEFFECTS GenEffects;
        LPALDELETEEFFECTS alDeleteEffects;
        LPALISEFFECT alIsEffect;
        LPALEFFECTI alEffecti;
        LPALEFFECTIV alEffectiv;
        LPALEFFECTF alEffectf;
        LPALEFFECTFV alEffectfv;
        LPALGETEFFECTI alGetEffecti;
        LPALGETEFFECTIV alGetEffectiv;
        LPALGETEFFECTF alGetEffectf;
        LPALGETEFFECTFV alGetEffectfv;

        LPALGENFILTERS GenFilters;
        LPALDELETEFILTERS DeleteEffects;
        LPALISFILTER IsFilter;
        LPALFILTERI Filteri;
        LPALFILTERIV Filteriv;
        LPALFILTERF Filterf;
        LPALFILTERFV Filterfv;
        LPALGETFILTERI GetFilteri;
        LPALGETFILTERIV GetFilteriv;
        LPALGETFILTERF GetFilterf;
        LPALGETFILTERFV GetFilterfv;

        LPALGENAUXILIARYEFFECTSLOTS GetAuxiliaryEffectSlots;
        LPALDELETEAUXILIARYEFFECTSLOTS DeleteAuxiliaryEffectSlots;
        LPALISAUXILIARYEFFECTSLOT IsAuxiliaryEffectSlot;
        LPALAUXILIARYEFFECTSLOTI AuxiliaryEffectSloti;
        LPALAUXILIARYEFFECTSLOTIV AuxiliaryEffectSlotiv;
        LPALAUXILIARYEFFECTSLOTF AuxiliaryEffectSlotf;
        LPALAUXILIARYEFFECTSLOTFV AuxiliaryEffectSlotfv;
        LPALGETAUXILIARYEFFECTSLOTI GetAuxiliaryEffectSloti;
        LPALGETAUXILIARYEFFECTSLOTIV GetAuxiliaryEffectSlotiv;
        LPALGETAUXILIARYEFFECTSLOTF GetAuxiliaryEffectSlotf;
        LPALGETAUXILIARYEFFECTSLOTFV GetAuxiliaryEffectSlotfv;
    };
    void* ptr;
};

ZAL LPALCCREATECONTEXT zalcCreateContext;
ZAL LPALCMAKECONTEXTCURRENT zalcMakeContextCurrent;
ZAL LPALCPROCESSCONTEXT zalcProcessContext;
ZAL LPALCSUSPENDCONTEXT zalcSuspendContext;
ZAL LPALCDESTROYCONTEXT zalcDestroyContext;
ZAL LPALCGETCURRENTCONTEXT zalcGetCurrentContext;
ZAL LPALCGETCONTEXTSDEVICE zalcGetContextsDevice;
ZAL LPALCOPENDEVICE zalcOpenDevice;
ZAL LPALCCLOSEDEVICE zalcCloseDevice;
ZAL LPALCGETERROR zalcGetError;
ZAL LPALCISEXTENSIONPRESENT zalcIsExtensionPresent;
ZAL LPALCGETPROCADDRESS zalcGetProcAddress;
ZAL LPALCGETENUMVALUE zalcGetEnumValue;
ZAL LPALCGETSTRING zalcGetString;
ZAL LPALCGETINTEGERV zalcGetIntegerv;
ZAL LPALCCAPTUREOPENDEVICE zalcCaptureOpenDevice;
ZAL LPALCCAPTURECLOSEDEVICE zalcCaptureCloseDevice;
ZAL LPALCCAPTURESTART zalcCaptureStart;
ZAL LPALCCAPTURESTOP zalcCaptureStop;
ZAL LPALCCAPTURESAMPLES zalcCaptureSamples;

#endif//ZAL_INTEROP_OAL_H
