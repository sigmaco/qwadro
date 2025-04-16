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
//#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include "zalSdev.h"
#pragma comment(lib, "mmdevapi")

#define MINIAUDIO_IMPLEMENTATION
#include "../../../dep_/miniaudio/miniaudio.h"

#include "../../../dep_/miniaudio/miniaudio.h"

_ZAL afxUnit _ZalAsioGetAvailableFrames_Parture(afxSink asi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);
    return 0;
}

_ZAL afxError _ZalPushAsioDataWasapiCb(afxSink asi, afxUnit frameCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);
    ma_result result;
    ma_uint32 framesWritten;

    /* We need to write to the ring buffer. Need to do this in a loop. */
    framesWritten = 0;
    while (framesWritten < frameCnt) {
        void* pMappedBuffer;
        ma_uint32 framesToWrite = frameCnt - framesWritten;

        result = ma_pcm_rb_acquire_write(&asi->rb, &framesToWrite, &pMappedBuffer);
        if (result != MA_SUCCESS) {
            break;
        }

        if (framesToWrite == 0) {
            break;
        }

        /* Copy the data from the capture buffer to the ring buffer. */
        ma_copy_pcm_frames(pMappedBuffer, ma_offset_pcm_frames_const_ptr_f32(src, framesWritten, asi->m.chanCnt), framesToWrite, asi->deviceConfig.playback.format, asi->deviceConfig.playback.channels);

        result = ma_pcm_rb_commit_write(&asi->rb, framesToWrite);

        if (result != MA_SUCCESS) {
            break;
        }

        framesWritten += framesToWrite;
    }
    return err;
}

_ZAL afxError _ZalRelinkAsioWasapiCb(afxMixDevice sdev, afxMixSystem ssys, afxUnit cnt, afxSink asios[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &sdev);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MSYS, 1, &ssys);
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, cnt, asios);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSink asi = asios[i];

        if (asi)
        {
            if (ma_device_stop(&asi->device) != MA_SUCCESS)
            {
                AfxThrowError();
                AfxReportError("Failed to start playback device.\n");
                return err;
            }
        }

        if (ssys)
        {
            if (ma_device_start(&asi->device) != MA_SUCCESS)
            {
                AfxThrowError();
                AfxReportError("Failed to start playback device.\n");
                return err;
            }
        }
    }
    return err;
}

_ZAL afxError _ZalAsioDtorCb(afxSink asi)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixDevice sdev = AfxGetProvider(asi);

    ma_device_uninit(&asi->device);

    return err;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    afxError err;
    afxSink asi = pDevice->pUserData;
    AFX_ASSERT(asi != NULL);
    ma_result result;
    ma_uint32 framesRead;

    /* We need to write to the ring buffer. Need to do this in a loop. */
    framesRead = 0;
    while (framesRead < frameCount) {
        void* pMappedBuffer;
        ma_uint32 framesToRead = frameCount - framesRead;

        result = ma_pcm_rb_acquire_read(&asi->rb, &framesToRead, &pMappedBuffer);
        if (result != MA_SUCCESS) {
            break;
        }

        if (framesToRead == 0) {
            break;
        }

        /* Copy the data from the capture buffer to the ring buffer. */
        ma_copy_pcm_frames(ma_offset_pcm_frames_const_ptr_f32(pOutput, framesRead, pDevice->playback.channels), pMappedBuffer, framesToRead, pDevice->playback.format, pDevice->playback.channels);

        result = ma_pcm_rb_commit_read(&asi->rb, framesToRead);

        if (result != MA_SUCCESS) {
            break;
        }

        framesRead += framesToRead;
    }
}

_ZAL afxError _ZalAsioCtorCb(afxSink asi, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_ASIO, 1, &asi);

    afxMixDevice sdev = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &sdev);
    afxSinkConfig const* cfg = ((afxSinkConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    afxBool record = *(afxBool*)(args[2]);

    afxUri dev;
    afxUri endpoint;
    //AfxClipUriDevice(&dev, &endpoint);
    
    "//./wasapi/mic";
    "//./wasapi/aux";
    "//./wasapi/mix";

    if (_AMX_ASIO_CLASS_CONFIG.ctor(asi, (void*[]) { sdev, (void*)cfg }, 0)) AfxThrowError();
    else
    {
        asi->m.chanCnt = cfg->chanCnt;
        asi->m.freq = cfg->freq;
        asi->m.fmt = cfg->bps == 16 ? amxFormat_S16i : amxFormat_S32f;
        asi->m.frameSiz = cfg->sampleCnt;

        ma_device_config deviceConfig;
        ma_device device;
        deviceConfig = ma_device_config_init(ma_device_type_playback);
        deviceConfig.playback.format = ma_format_f32;
        deviceConfig.playback.channels = cfg->chanCnt;
        deviceConfig.sampleRate = cfg->freq;
        deviceConfig.dataCallback = data_callback;
        deviceConfig.pUserData = asi;

        if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
        {
            AfxReportError("Failed to open playback device.\n");
            AfxThrowError();
            return err;
        }
        ma_device_info;
        ma_pcm_rb rb;
        ma_pcm_rb_set_sample_rate(&rb, device.sampleRate);
        asi->deviceConfig = deviceConfig;
        asi->device = device;
        asi->rb = rb;


    }
    return err;
}
