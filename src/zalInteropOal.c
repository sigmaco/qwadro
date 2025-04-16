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


#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include "zalInteropOal.h"

char const *alVmtNames[] =
{
    // v1
    "alEnable",
    "alDisable",
    "alIsEnabled",
    "alGetString",
    "alGetBooleanv",
    "alGetIntegerv",
    "alGetFloatv",
    "alGetDoublev",
    "alGetBoolean",
    "alGetInteger",
    "alGetFloat",
    "alGetDouble",
    "alGetError",
    "alIsExtensionPresent",
    "alGetProcAddress",
    "alGetEnumValue",
    "alListenerf",
    "alListener3f",
    "alListenerfv",
    "alListeneri",
    "alListener3i",
    "alListeneriv",
    "alGetListenerf",
    "alGetListener3f",
    "alGetListenerfv",
    "alGetListeneri",
    "alGetListener3i",
    "alGetListeneriv",
    "alGenSources",
    "alDeleteSources",
    "alIsSource",
    "alSourcef",
    "alSource3f",
    "alSourcefv",
    "alSourcei",
    "alSource3i",
    "alSourceiv",
    "alGetSourcef",
    "alGetSource3f",
    "alGetSourcefv",
    "alGetSourcei",
    "alGetSource3i",
    "alGetSourceiv",
    "alSourcePlayv",
    "alSourceStopv",
    "alSourceRewindv",
    "alSourcePausev",
    "alSourcePlay",
    "alSourceStop",
    "alSourceRewind",
    "alSourcePause",
    "alSourceQueueBuffers",
    "alSourceUnqueueBuffers",
    "alGenBuffers",
    "alDeleteBuffers",
    "alIsBuffer",
    "alBufferData",
    "alBufferf",
    "alBuffer3f",
    "alBufferfv",
    "alBufferi",
    "alBuffer3i",
    "alBufferiv",
    "alGetBufferf",
    "alGetBuffer3f",
    "alGetBufferfv",
    "alGetBufferi",
    "alGetBuffer3i",
    "alGetBufferiv",

    "DopplerFactor",
    "DopplerVelocity",
    "SpeedOfSound",
    "DistanceModel",

    // EFX
    "alGenEffects",
    "alDeleteEffects",
    "alIsEffect",
    "alEffecti",
    "alEffectiv",
    "alEffectf",
    "alEffectfv",
    "alGetEffecti",
    "alGetEffectiv",
    "alGetEffectf",
    "alGetEffectfv",
    "alGenFilters",
    "alDeleteFilters",
    "alIsFilter",
    "alFilteri",
    "alFilteriv",
    "alFilterf",
    "alFilterfv",
    "alGetFilteri",
    "alGetFilteriv",
    "alGetFilterf",
    "alGetFilterfv",

    "alGetAuxiliaryEffectSlots",
    "alDeleteAuxiliaryEffectSlots",
    "alIsAuxiliaryEffectSlot",
    "alAuxiliaryEffectSloti",
    "alAuxiliaryEffectSlotiv",
    "alAuxiliaryEffectSlotf",
    "alAuxiliaryEffectSlotfv",
    "alGetAuxiliaryEffectSloti",
    "alGetAuxiliaryEffectSlotiv",
    "alGetAuxiliaryEffectSlotf",
    "alGetAuxiliaryEffectSlotfv"
};

_ZAL LPALCCREATECONTEXT zalcCreateContext = NIL;
_ZAL LPALCMAKECONTEXTCURRENT zalcMakeContextCurrent = NIL;
_ZAL LPALCPROCESSCONTEXT zalcProcessContext = NIL;
_ZAL LPALCSUSPENDCONTEXT zalcSuspendContext = NIL;
_ZAL LPALCDESTROYCONTEXT zalcDestroyContext = NIL;
_ZAL LPALCGETCURRENTCONTEXT zalcGetCurrentContext = NIL;
_ZAL LPALCGETCONTEXTSDEVICE zalcGetContextsDevice = NIL;
_ZAL LPALCOPENDEVICE zalcOpenDevice = NIL;
_ZAL LPALCCLOSEDEVICE zalcCloseDevice = NIL;
_ZAL LPALCGETERROR zalcGetError = NIL;
_ZAL LPALCISEXTENSIONPRESENT zalcIsExtensionPresent = NIL;
_ZAL LPALCGETPROCADDRESS zalcGetProcAddress = NIL;
_ZAL LPALCGETENUMVALUE zalcGetEnumValue = NIL;
_ZAL LPALCGETSTRING zalcGetString = NIL;
_ZAL LPALCGETINTEGERV zalcGetIntegerv = NIL;
_ZAL LPALCCAPTUREOPENDEVICE zalcCaptureOpenDevice = NIL;
_ZAL LPALCCAPTURECLOSEDEVICE zalcCaptureCloseDevice = NIL;
_ZAL LPALCCAPTURESTART zalcCaptureStart = NIL;
_ZAL LPALCCAPTURESTOP zalcCaptureStop = NIL;
_ZAL LPALCCAPTURESAMPLES zalcCaptureSamples = NIL;
_ZAL HMODULE hOpenal32Dll = NIL;

_ZAL afxError _ZalLoadOpenAl(HMODULE openal32, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch)
{
    afxError err = NIL;
    static afxBool loaded = FALSE;
    static afxUnit loadedVerMajor = 0;
    static afxUnit loadedVerMinor = 0;
    static afxUnit loadedVerPatch = 0;

    if (!loaded)
    {
        hOpenal32Dll = openal32;

        zalcGetProcAddress = (void*)GetProcAddress(openal32, "alcGetProcAddress");

        zalcCreateContext = zalcGetProcAddress(NIL, "alcCreateContext");
        zalcMakeContextCurrent = zalcGetProcAddress(NIL, "alcMakeContextCurrent");
        zalcProcessContext = zalcGetProcAddress(NIL, "alcProcessContext");
        zalcSuspendContext = zalcGetProcAddress(NIL, "alcSuspendContext");
        zalcDestroyContext = zalcGetProcAddress(NIL, "alcDestroyContext");
        zalcGetCurrentContext = zalcGetProcAddress(NIL, "alcGetCurrentContext");
        zalcGetContextsDevice = zalcGetProcAddress(NIL, "alcGetContextsDevice");
        zalcOpenDevice = zalcGetProcAddress(NIL, "alcOpenDevice");
        zalcCloseDevice = zalcGetProcAddress(NIL, "alcCloseDevice");
        zalcGetError = zalcGetProcAddress(NIL, "alcGetError");
        zalcIsExtensionPresent = zalcGetProcAddress(NIL, "alcIsExtensionPresent");
        zalcGetEnumValue = zalcGetProcAddress(NIL, "alcGetEnumValue");
        zalcGetString = zalcGetProcAddress(NIL, "alcGetString");
        zalcGetIntegerv = zalcGetProcAddress(NIL, "alcGetIntegerv");
        zalcCaptureOpenDevice = zalcGetProcAddress(NIL, "alcCaptureOpenDevice");
        zalcCaptureCloseDevice = zalcGetProcAddress(NIL, "alcCaptureCloseDevice");
        zalcCaptureStart = zalcGetProcAddress(NIL, "alcCaptureStart");
        zalcCaptureStop = zalcGetProcAddress(NIL, "alcCaptureStop");
        zalcCaptureSamples = zalcGetProcAddress(NIL, "alcCaptureSamples");

        afxString ver;
        afxUnit vMajor, vMinor, vPatch;
        AfxMakeString(&ver, 0, (afxChar const*)zalcGetString(NIL, AL_VERSION), 0);
        AfxScanString(&ver, "%u.%u.%u", &vMajor, &vMinor, &vPatch);

        loadedVerMajor = vMajor;
        loadedVerMinor = vMinor;
        loadedVerPatch = vPatch;

        if (!err)
            loaded = TRUE;
    }

    if (verMajor)
        *verMajor = loadedVerMajor;
    if (verMinor)
        *verMinor = loadedVerMinor;
    if (verPatch)
        *verPatch = loadedVerPatch;

    return err;
}

_ZAL afxError _ZalLoadOalVmt(HMODULE openal32, afxUnit base, afxUnit cnt, char const *names[], void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;

    AFX_ASSERT(names);
    AFX_ASSERT(vmt);
    AFX_ASSERT(openal32);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT(names[base + i]);
        void *sym = NIL;

        if ((sym = (void*)GetProcAddress(openal32, names[base + i])))
        {
            if (echo)
            {
                AfxReportMessage("  %3u %s", i, names[base + i]);
            }
        }

        vmt[base + i] = sym;
    }
    return err;
}

_ZAL void _ListAllDevs(ALCdevice* dev)
{
    if (zalcIsExtensionPresent(dev, "ALC_ENUMERATION_EXT"))
    {
        ALCchar const *pDeviceNames = zalcGetString(dev, ALC_DEVICE_SPECIFIER);
        ALCchar const *pDefaultDevice = zalcGetString(dev, ALC_DEFAULT_DEVICE_SPECIFIER);

        if (strlen(pDeviceNames))
        {
            afxUnit iDeviceIndex = 0;
            while (pDeviceNames && *pDeviceNames)
            {
                AfxReportMessage("%-2d - %s", iDeviceIndex, pDeviceNames);
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    if (zalcIsExtensionPresent(dev, "ALC_ENUMERATE_ALL_EXT"))
    {
        AfxReportMessage("Listing devices available with ALC_ENUMERATE_ALL_EXT");
        ALCchar const *pDeviceNames = zalcGetString(dev, ALC_ALL_DEVICES_SPECIFIER);
        ALCchar const *pDefaultDevice = zalcGetString(dev, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

        if (strlen(pDeviceNames))
        {
            afxUnit iDeviceIndex = 0;
            while (pDeviceNames && *pDeviceNames)
            {
                AfxReportMessage("%-2d - %s", iDeviceIndex, pDeviceNames);
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    // Check for Capture Extension support

    if (zalcIsExtensionPresent(dev, "ALC_EXT_CAPTURE"))
    {
        // Get list of available Capture Devices
        const ALchar *pDeviceList = zalcGetString(dev, ALC_CAPTURE_DEVICE_SPECIFIER);

        if (pDeviceList)
        {
            AfxReportMessage("Listing available capture devices:");

            afxUnit iDeviceIndex = 0;
            while (*pDeviceList)
            {
                AfxReportMessage("%-2d - %s", iDeviceIndex, pDeviceList);
                pDeviceList += strlen(pDeviceList) + 1;
                iDeviceIndex++;
            }
        }
    }
}
