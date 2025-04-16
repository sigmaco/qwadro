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
#include <endpointvolume.h>
//#include <comdef.h>
#include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
//#include "AuxOverWin32.h"
#pragma comment(lib, "mmdevapi")

#include "zalInteropWasapi.h"
#include "zalObjects.h"

#if !0
#ifndef GUID_SECT
#define GUID_SECT
#endif
#define __DEFINE_GUID(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) static const GUID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#define __DEFINE_IID(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) static const IID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#define __DEFINE_CLSID(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) static const CLSID n GUID_SECT = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#define PA_DEFINE_CLSID(className, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    __DEFINE_CLSID(CLSID_##className, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)
#define PA_DEFINE_IID(interfaceName, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    __DEFINE_IID(IID_##interfaceName, 0x##l, 0x##w1, 0x##w2, 0x##b1, 0x##b2, 0x##b3, 0x##b4, 0x##b5, 0x##b6, 0x##b7, 0x##b8)

 // "1CB9AD4C-DBFA-4c32-B178-C2F568A703B2"
PA_DEFINE_IID(IAudioClient, 1cb9ad4c, dbfa, 4c32, b1, 78, c2, f5, 68, a7, 03, b2);
// "726778CD-F60A-4EDA-82DE-E47610CD78AA"
PA_DEFINE_IID(IAudioClient2, 726778cd, f60a, 4eda, 82, de, e4, 76, 10, cd, 78, aa);
// "7ED4EE07-8E67-4CD4-8C1A-2B7A5987AD42"
PA_DEFINE_IID(IAudioClient3, 7ed4ee07, 8e67, 4cd4, 8c, 1a, 2b, 7a, 59, 87, ad, 42);
// "1BE09788-6894-4089-8586-9A2A6C265AC5"
PA_DEFINE_IID(IMMEndpoint, 1be09788, 6894, 4089, 85, 86, 9a, 2a, 6c, 26, 5a, c5);
// "A95664D2-9614-4F35-A746-DE8DB63617E6"
PA_DEFINE_IID(IMMDeviceEnumerator, a95664d2, 9614, 4f35, a7, 46, de, 8d, b6, 36, 17, e6);
// "BCDE0395-E52F-467C-8E3D-C4579291692E"
PA_DEFINE_CLSID(MMDeviceEnumerator, bcde0395, e52f, 467c, 8e, 3d, c4, 57, 92, 91, 69, 2e);
// "F294ACFC-3146-4483-A7BF-ADDCA7C260E2"
PA_DEFINE_IID(IAudioRenderClient, f294acfc, 3146, 4483, a7, bf, ad, dc, a7, c2, 60, e2);
// "C8ADBD64-E71E-48a0-A4DE-185C395CD317"
PA_DEFINE_IID(IAudioCaptureClient, c8adbd64, e71e, 48a0, a4, de, 18, 5c, 39, 5c, d3, 17);
// *2A07407E-6497-4A18-9787-32F79BD0D98F*  Or this??
PA_DEFINE_IID(IDeviceTopology, 2A07407E, 6497, 4A18, 97, 87, 32, f7, 9b, d0, d9, 8f);
// *AE2DE0E4-5BCA-4F2D-AA46-5D13F8FDB3A9*
PA_DEFINE_IID(IPart, AE2DE0E4, 5BCA, 4F2D, aa, 46, 5d, 13, f8, fd, b3, a9);
// *4509F757-2D46-4637-8E62-CE7DB944F57B*
PA_DEFINE_IID(IKsJackDescription, 4509F757, 2D46, 4637, 8e, 62, ce, 7d, b9, 44, f5, 7b);

// Media formats:
__DEFINE_GUID(pa_KSDATAFORMAT_SUBTYPE_PCM, 0x00000001, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
__DEFINE_GUID(pa_KSDATAFORMAT_SUBTYPE_ADPCM, 0x00000002, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
__DEFINE_GUID(pa_KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 0x00000003, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
__DEFINE_GUID(pa_KSDATAFORMAT_SUBTYPE_IEC61937_PCM, 0x00000000, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
#endif

// Convert PCM 16-bit to float [-1.0, 1.0]
float pcm16_to_float(int16_t pcm_value) {
    return pcm_value / 32768.0f;
}

// Resample from 44.1kHz to 48kHz using linear interpolation and return float samples
void resample_44100_to_48000_float(const int16_t* input, size_t input_len, float* output, size_t* output_len)
{
    // Input sample rate: 44100Hz, Output sample rate: 48000Hz
    double ratio = 48000.0 / 44100.0;  // Ratio of new sample rate to old sample rate
    size_t new_length = (size_t)(input_len * ratio);  // Number of samples in output
    *output_len = new_length;

    // Perform linear interpolation to resample
    for (size_t i = 0; i < new_length; i++) {
        double src_pos = i / ratio;  // Calculate the source position
        size_t index1 = (size_t)src_pos;  // The integer part (before interpolation)
        size_t index2 = (index1 + 1 < input_len) ? index1 + 1 : index1;  // Next sample (ensure we stay within bounds)
        double frac = src_pos - index1;  // Fractional part (for interpolation)

        // Perform linear interpolation on float values
        float value1 = pcm16_to_float(input[index1]);
        float value2 = pcm16_to_float(input[index2]);
        float interpolated_value = (1 - frac) * value1 + frac * value2;

        // Store the interpolated value in the output array
        output[i] = interpolated_value;
    }
}

// Resample from 44.1kHz to 48kHz using linear interpolation and return float samples
void resample_44100_to_48000_float2(const int16_t* input, size_t input_len, float* output, size_t* output_len, afxUnit dstFreq, afxUnit srcFreq)
{
    // Input sample rate: 44100Hz, Output sample rate: 48000Hz
    double ratio = (afxReal)dstFreq / (afxReal)srcFreq;  // Ratio of new sample rate to old sample rate
    size_t new_length = (size_t)(input_len * ratio);  // Number of samples in output
    *output_len = new_length;

    // Perform linear interpolation to resample
    for (size_t i = 0; i < new_length; i++) {
        double src_pos = i / ratio;  // Calculate the source position
        size_t index1 = (size_t)src_pos;  // The integer part (before interpolation)
        size_t index2 = (index1 + 1 < input_len) ? index1 + 1 : index1;  // Next sample (ensure we stay within bounds)
        double frac = src_pos - index1;  // Fractional part (for interpolation)

        // Perform linear interpolation on float values
        float value1 = pcm16_to_float(input[index1]);
        float value2 = pcm16_to_float(input[index2]);
        float interpolated_value = (1 - frac) * value1 + frac * value2;

        // Store the interpolated value in the output array
        output[i] = interpolated_value;
    }
}

// Resample from 44.1kHz to 48kHz (stereo) using linear interpolation
void resample_44100_to_48000_stereo(const int16_t* input_left, const int16_t* input_right,
    size_t input_len, float* output_left, float* output_right,
    size_t* output_len) {
    // Input sample rate: 44100Hz, Output sample rate: 48000Hz
    double ratio = 48000.0 / 44100.0;  // Ratio of new sample rate to old sample rate
    size_t new_length = (size_t)(input_len * ratio);  // Number of samples in output
    *output_len = new_length;

    // Perform linear interpolation to resample
    for (size_t i = 0; i < new_length; i++) {
        double src_pos = i / ratio;  // Calculate the source position
        size_t index1 = (size_t)src_pos;  // The integer part (before interpolation)
        size_t index2 = (index1 + 1 < input_len) ? index1 + 1 : index1;  // Next sample (ensure we stay within bounds)
        double frac = src_pos - index1;  // Fractional part (for interpolation)

        // Perform linear interpolation for left channel
        float left_value1 = pcm16_to_float(input_left[index1]);
        float left_value2 = pcm16_to_float(input_left[index2]);
        float left_interpolated = (1 - frac) * left_value1 + frac * left_value2;

        // Perform linear interpolation for right channel
        float right_value1 = pcm16_to_float(input_right[index1]);
        float right_value2 = pcm16_to_float(input_right[index2]);
        float right_interpolated = (1 - frac) * right_value1 + frac * right_value2;

        // Store the interpolated values in the output arrays for each channel
        output_left[i] = left_interpolated;
        output_right[i] = right_interpolated;
    }
}

// Resample from 44.1kHz to 48kHz (interleaved stereo) using linear interpolation
void resample_44100_to_48000_interleaved(const int16_t* input, size_t input_len,
    float* output, size_t* output_len) {
    // Input sample rate: 44100Hz, Output sample rate: 48000Hz
    double ratio = 48000.0 / 44100.0;  // Ratio of new sample rate to old sample rate
    size_t new_length = (size_t)(input_len * ratio);  // Number of samples in output
    *output_len = new_length;

    // Perform linear interpolation to resample
    for (size_t i = 0; i < new_length; i++) {
        double src_pos = i / ratio;  // Calculate the source position
        size_t index1 = (size_t)src_pos;  // The integer part (before interpolation)
        size_t index2 = (index1 + 1 < input_len) ? index1 + 1 : index1;  // Next sample (ensure we stay within bounds)
        double frac = src_pos - index1;  // Fractional part (for interpolation)

        // Interpolating the left and right channels separately (interleaved format)
        // Left channel is at even indices in the input array
        // Right channel is at odd indices in the input array

        // Left channel interpolation
        int16_t left_value1 = input[index1 * 2];  // Left sample at index1
        int16_t left_value2 = input[index2 * 2];  // Left sample at index2
        float left_interpolated = (1 - frac) * pcm16_to_float(left_value1) + frac * pcm16_to_float(left_value2);

        // Right channel interpolation
        int16_t right_value1 = input[index1 * 2 + 1];  // Right sample at index1
        int16_t right_value2 = input[index2 * 2 + 1];  // Right sample at index2
        float right_interpolated = (1 - frac) * pcm16_to_float(right_value1) + frac * pcm16_to_float(right_value2);

        // Store the interpolated values in the output array (interleaved)
        output[i * 2] = left_interpolated;
        output[i * 2 + 1] = right_interpolated;
    }
}

_ZAL afxUnit _ZalWasapiGetWriteRoom(zalWasapi* idd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);
    
    UINT32 numFramesPadding;
    // See how much buffer space is available.
    HRESULT hr = idd->pAudioClient->lpVtbl->GetCurrentPadding(idd->pAudioClient, &numFramesPadding);
    //AFX_ASSERT(!hr);
    UINT32 bufferFrameCount = idd->bufferFrameCount;
    UINT32 numFramesAvailable = bufferFrameCount - numFramesPadding;
    return numFramesAvailable;
}

_ZAL afxError _ZalWasapiWriteParture(zalWasapi* idd, afxUnit frameCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);

    while (1)
    {
        HRESULT hr;
        UINT32 bufferFrameCount = idd->bufferFrameCount;

        BYTE *pData;
        // Grab the entire buffer for the initial fill operation.
        //hr = asi->pRenderClient->lpVtbl->GetBuffer(asi->pRenderClient, bufferFrameCount, &pData);

        // Calculate the actual duration of the allocated buffer.
        REFERENCE_TIME hnsActualDuration = (double)10000000 * bufferFrameCount / idd->pwfx->nSamplesPerSec;

        UINT32 numFramesPadding;
        // See how much buffer space is available.
        hr = idd->pAudioClient->lpVtbl->GetCurrentPadding(idd->pAudioClient, &numFramesPadding);
        //AFX_ASSERT(!hr);

        UINT32 numFramesAvailable = bufferFrameCount - numFramesPadding;


        BYTE* pRenderData;
        hr = idd->pRenderClient->lpVtbl->GetBuffer(idd->pRenderClient, bufferFrameCount, &pRenderData);
        //AFX_ASSERT(!hr);

        if (!pRenderData)
            continue;
        
        afxUnit outLen;
        //resample_44100_to_48000_float2(src, frameCnt, pRenderData, &outLen, asi->bufferFrameCount, frameCnt);
        //resample_44100_to_48000_float(src, frameCnt, pRenderData, &outLen);
        resample_44100_to_48000_interleaved(src, frameCnt, pRenderData, &outLen);

        //AfxCopy(pRenderData, src, bufferFrameCount * asi->pwfx->nBlockAlign);  // Copy captured data to render buffer

        hr = idd->pRenderClient->lpVtbl->ReleaseBuffer(idd->pRenderClient, bufferFrameCount, 0);
        //AFX_ASSERT(!hr);
        break;
    }
    return err;
}

_ZAL afxUnit _ZalWasapiGetReadLength(zalWasapi* idd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);
    // Capture audio
    UINT32 bufferFrameCount = 0;
    HRESULT hr = idd->pCaptureClient->lpVtbl->GetNextPacketSize(idd->pCaptureClient, &bufferFrameCount);
    return bufferFrameCount;
}

_ZAL afxError _ZalWasapiReadCapture(zalWasapi* idd, afxUnit frameCap, void* dst, afxUnit* frameCnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);

    // Capture audio
    UINT32 bufferFrameCount = 0;
    HRESULT hr = idd->pCaptureClient->lpVtbl->GetNextPacketSize(idd->pCaptureClient, &bufferFrameCount);
    AFX_ASSERT(!hr);

    if (bufferFrameCount > 0)
    {
        BYTE* pCaptureData = NIL;
        hr = idd->pCaptureClient->lpVtbl->GetBuffer(idd->pCaptureClient, &pCaptureData, &bufferFrameCount, NULL, NULL, NULL);
        AFX_ASSERT(!hr);

        AfxCopy(dst, pCaptureData, bufferFrameCount * idd->pwfx->nBlockAlign);  // Copy captured data to render buffer

        hr = idd->pCaptureClient->lpVtbl->ReleaseBuffer(idd->pCaptureClient, bufferFrameCount);
        AFX_ASSERT(!hr);
    }

    *frameCnt = bufferFrameCount;
    return err;
}

_ZAL afxError _ZalWasapiStartStop(zalWasapi* idd, afxBool start)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);

    if (idd->pAudioClient)
        idd->pAudioClient->lpVtbl->Stop(idd->pAudioClient);
        
    HRESULT hr = 0;

    // Step 9: Start both audio clients

    if (start)
    {
        if (idd->pAudioClient)
            hr = idd->pAudioClient->lpVtbl->Start(idd->pAudioClient);

        AFX_ASSERT(!hr);
    }
    return err;
}

_ZAL afxError _ZalWasapiDestroy(zalWasapi* idd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(idd);

    if (idd->pRenderClient)
        idd->pRenderClient->lpVtbl->Release(idd->pRenderClient);

    if (idd->pCaptureClient)
        idd->pCaptureClient->lpVtbl->Release(idd->pCaptureClient);

    if (idd->pAudioClient)
        idd->pAudioClient->lpVtbl->Release(idd->pAudioClient);

    if (idd->pDevice)
        idd->pDevice->lpVtbl->Release(idd->pDevice);

    if (idd->pEnumerator)
        idd->pEnumerator->lpVtbl->Release(idd->pEnumerator);

    if (idd->pwfx)
        CoTaskMemFree(idd->pwfx);

    if (idd->bufferReady)
        CloseHandle(idd->bufferReady);

    return err;
}

_ZAL afxError _ZalWasapiCreate(zalWasapi* idd)
{
    afxResult err = NIL;
    AFX_ASSERT(idd);

    HRESULT hr;

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioClient* pAudioClient = NULL;
    IAudioRenderClient* pRenderClient = NULL;
    IAudioCaptureClient* pCaptureClient = NULL;
    UINT32 bufferFrameCount;
    BYTE* pRenderData = NULL;
    BYTE* pCaptureData = NULL;
    WAVEFORMATEX* pwfx = NULL;
#if 0
    if (FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), 0, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),(void**)(&pEnumerator))))
    {
        return err;
    }
#endif
    /*
    CLSCTX_INPROC_SERVER indicates that the COM object should be created in the same process as the client that is requesting it.
    This is typically used for performance reasons, as creating the object in-process eliminates the need for inter-process communication.
    In-process server means the COM object runs in the same memory space (or process) as the calling application.
    Out-of-process server would run in a different process, which is usually more resource-intensive but can be used for isolation or running in a different security context.

    CLSCTX_INPROC_SERVER: In-process server (same process).
    CLSCTX_INPROC_HANDLER: In-process handler (used for moniker handling).
    CLSCTX_LOCAL_SERVER: Out-of-process server (same machine).
    CLSCTX_REMOTE_SERVER: Remote server (different machine).
    CLSCTX_ALL: Any of the above contexts.
*/
    DWORD dwClsCtx = CLSCTX_INPROC_SERVER;// CLSCTX_ALL;
    afxBool record = FALSE;

    // Step 1: Get the device enumerator
    hr = CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, dwClsCtx, &IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    AFX_ASSERT(!hr);
    // Step 2: Get the default audio rendering and capturing devices
    hr = pEnumerator->lpVtbl->GetDefaultAudioEndpoint(pEnumerator, record ? eCapture : eRender, eConsole, &pDevice);
    AFX_ASSERT(!hr);

    // Step 3: Activate IAudioClient interfaces for both devices
    hr = pDevice->lpVtbl->Activate(pDevice, &IID_IAudioClient, dwClsCtx, NULL, (void**)&pAudioClient);
    AFX_ASSERT(!hr);

    // Step 4: Get the mix format for the render client and use it for capture
    hr = pAudioClient->lpVtbl->GetMixFormat(pAudioClient, &pwfx);
    AFX_ASSERT(!hr);

    REFERENCE_TIME defaultPeriod;
    REFERENCE_TIME minimumPeriod;
    pAudioClient->lpVtbl->GetDevicePeriod(pAudioClient, &defaultPeriod, &minimumPeriod);

    DWORD dwStreamFlags = AUDCLNT_STREAMFLAGS_EVENTCALLBACK;
    AUDCLNT_SHAREMODE shareMode = AUDCLNT_SHAREMODE_SHARED;

    // Step 5: Initialize the render audio client
    hr = pAudioClient->lpVtbl->Initialize(pAudioClient, shareMode, dwStreamFlags, minimumPeriod, 0, pwfx, NULL);
    AFX_ASSERT(!hr);

    // Step 7: Get buffer size for both devices
    hr = pAudioClient->lpVtbl->GetBufferSize(pAudioClient, &bufferFrameCount);
    AFX_ASSERT(!hr);

    HANDLE bufferReady = CreateEventA(NIL, FALSE, FALSE, NIL);

    pAudioClient->lpVtbl->SetEventHandle(pAudioClient, bufferReady);

    if (!record)
    {
        // Step 8: Get render and capture clients
        hr = pAudioClient->lpVtbl->GetService(pAudioClient, &IID_IAudioRenderClient, (void**)&pRenderClient);
        AFX_ASSERT(!hr);
    }
    else
    {
        hr = pAudioClient->lpVtbl->GetService(pAudioClient, &IID_IAudioCaptureClient, (void**)&pCaptureClient);
        AFX_ASSERT(!hr);
    }

    //asi->m.frameSiz = pwfx->nBlockAlign;
    //AfxDeployRing(&asi->m.ioRing, asi->m.frameSiz, 512);

    if (!err)
    {
        idd->bufferReady = bufferReady;
        idd->minimumPeriod = minimumPeriod;
        idd->defaultPeriod = defaultPeriod;
        idd->dwClsCtx = dwClsCtx;
        idd->bufferFrameCount = bufferFrameCount;
        idd->pwfx = pwfx;
        idd->pEnumerator = pEnumerator;
        idd->pDevice = pDevice;
        idd->pAudioClient = pAudioClient;
        idd->pCaptureClient = pCaptureClient;
        idd->pRenderClient = pRenderClient;
    }
    else
    {
        if (pRenderClient)
            pRenderClient->lpVtbl->Release(pRenderClient);

        if (pCaptureClient)
            pCaptureClient->lpVtbl->Release(pCaptureClient);

        if (pAudioClient)
            pAudioClient->lpVtbl->Release(pAudioClient);

        if (pDevice)
            pDevice->lpVtbl->Release(pDevice);

        if (pEnumerator)
            pEnumerator->lpVtbl->Release(pEnumerator);

        if (pwfx)
            CoTaskMemFree(pwfx);

        if (bufferReady)
            CloseHandle(bufferReady);
    }
    return err;
}

// Function to enumerate audio endpoints
HRESULT EnumerateAudioEndpoints(IMMDeviceEnumerator *pEnumerator)
{
    IMMDeviceCollection *pDevices = NULL;
    HRESULT hr = pEnumerator->lpVtbl->EnumAudioEndpoints(pEnumerator, eAll, DEVICE_STATE_ACTIVE, &pDevices);
    if (FAILED(hr)) {
        //printf("Failed to enumerate audio endpoints: 0x%lx\n", hr);
        return hr;
    }

    UINT deviceCount = 0;
    pDevices->lpVtbl->GetCount(pDevices, &deviceCount);
    if (deviceCount == 0) {
        //printf("No audio devices found.\n");
        pDevices->lpVtbl->Release(pDevices);
        return S_OK;
    }

    //printf("Audio endpoints found:\n");
    for (UINT i = 0; i < deviceCount; ++i) {
        IMMDevice *pDevice = NULL;
        hr = pDevices->lpVtbl->Item(pDevices, i, &pDevice);
        if (FAILED(hr)) {
            continue;
        }

        // Get the device name
        IPropertyStore *pProps = NULL;
        hr = pDevice->lpVtbl->OpenPropertyStore(pDevice, STGM_READ, &pProps);
        if (FAILED(hr)) {
            pDevice->lpVtbl->Release(pDevice);
            continue;
        }

        PROPVARIANT varName;
        PropVariantInit(&varName);
        hr = pProps->lpVtbl->GetValue(pProps, &PKEY_Device_FriendlyName, &varName);
        if (SUCCEEDED(hr)) {
            AfxReportError("[%u] %s\n", i, varName.pwszVal);
            PropVariantClear(&varName);
        }
        pProps->lpVtbl->Release(pProps);
        pDevice->lpVtbl->Release(pDevice);
    }

    pDevices->lpVtbl->Release(pDevices);
    return S_OK;
}

// Function to select an audio endpoint
HRESULT SelectAudioEndpoint(IMMDeviceEnumerator *pEnumerator, UINT index)
{
    IMMDeviceCollection *pDevices = NULL;
    HRESULT hr = pEnumerator->lpVtbl->EnumAudioEndpoints(pEnumerator, eAll, DEVICE_STATE_ACTIVE, &pDevices);
    if (FAILED(hr)) {
        //printf("Failed to enumerate audio endpoints: 0x%lx\n", hr);
        return hr;
    }

    IMMDevice *pDevice = NULL;
    hr = pDevices->lpVtbl->Item(pDevices, index, &pDevice);
    if (FAILED(hr)) {
        //printf("Failed to select the audio device: 0x%lx\n", hr);
        pDevices->lpVtbl->Release(pDevices);
        return hr;
    }

    // Print the name of the selected device
    IPropertyStore *pProps = NULL;
    hr = pDevice->lpVtbl->OpenPropertyStore(pDevice, STGM_READ, &pProps);
    if (FAILED(hr)) {
        pDevice->lpVtbl->Release(pDevice);
        pDevices->lpVtbl->Release(pDevices);
        return hr;
    }

    PROPVARIANT varName;
    PropVariantInit(&varName);
    hr = pProps->lpVtbl->GetValue(pProps, &PKEY_Device_FriendlyName, &varName);
    if (SUCCEEDED(hr)) {
        AfxReportError("Selected audio device: %s\n", varName.pwszVal);
        PropVariantClear(&varName);
    }

    pProps->lpVtbl->Release(pProps);
    pDevice->lpVtbl->Release(pDevice);
    pDevices->lpVtbl->Release(pDevices);
    return S_OK;
}
