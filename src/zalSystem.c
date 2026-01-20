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

#include <d3d9.h>
#include <dxva.h>
#include <dxva2api.h>
#include <d3dhalex.h>

#include "zalObjects.h"
#include "zalInteropWasapi.h"

_ZAL afxError _ZalMsysDtorCb(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    _AMX_MSYS_CLASS_CONFIG.dtor(msys);

    return err;
}

_ZAL afxError _ZalMsysCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    __amxMsysAcq const* cfg = AFX_CAST(__amxMsysAcq const*, args[1]) + invokeNo;
    _amxMexuAcq* bridgeCfgs = AFX_CAST(_amxMexuAcq*, args[2]) + invokeNo;

    __amxMsysAcq cfg2 = *cfg;

    afxClassConfig mbufClsCfg = _AMX_MBUF_CLASS_CONFIG;
    mbufClsCfg.fixedSiz = sizeof(AFX_OBJ(amxBuffer));
    mbufClsCfg.ctor = (void*)_ZalMbufCtorCb;
    mbufClsCfg.dtor = (void*)_ZalMbufDtorCb;

    afxClassConfig mpmpClsCfg = _AMX_MSRC_CLASS_CONFIG;
    mpmpClsCfg.fixedSiz = sizeof(AFX_OBJ(amxPump));
    mpmpClsCfg.ctor = (void*)_ZalMpmpCtorCb;
    mpmpClsCfg.dtor = (void*)_ZalMpmpDtorCb;

    afxClassConfig vidClsCfg = _AMX_VID_CLASS_CONFIG;
    vidClsCfg.fixedSiz = sizeof(AFX_OBJ(amxVideo));
    vidClsCfg.ctor = (void*)_ZalVidCtorCb;
    vidClsCfg.dtor = (void*)_ZalVidDtorCb;

    afxClassConfig mexuClsCfg = _AMX_MEXU_CLASS_CONFIG;
    mexuClsCfg.fixedSiz = sizeof(AFX_OBJ(afxMixBridge));
    mexuClsCfg.ctor = (void*)_ZalMexuCtorCb;
    mexuClsCfg.dtor = (void*)_ZalMexuDtorCb;

    cfg2.mbufClsCfg = &mbufClsCfg;
    cfg2.mexuClsCfg = &mexuClsCfg;
    cfg2.pmpClsCfg = &mpmpClsCfg;
    cfg2.vidClsCfg = &vidClsCfg;

    if (_AMX_MSYS_CLASS_CONFIG.ctor(msys, (void*[]) { icd, &cfg2, bridgeCfgs }, 0))
    {
        AfxThrowError();
        return err;
    }

#if 0
    HRESULT hr;
    D3DPRESENT_PARAMETERS d3dpp = { 0 };
    D3DDISPLAYMODE        d3ddm;
    unsigned resetToken = 0;
    UINT adapter = D3DADAPTER_DEFAULT;

    IDirect3D9Ex* d3d9;
    Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9);

    if (!d3d9)
    {
        // "Failed to create IDirect3D object\n"
    }

    //if (hac->hwaccel_device)
    {
        //adapter = atoi(hac->hwaccel_device);
        // "Using HWAccel device %d\n" adapter
    }

    IDirect3D9_GetAdapterDisplayMode(d3d9, adapter, &d3ddm);
    d3dpp.Windowed = TRUE;
    d3dpp.BackBufferWidth = 640;
    d3dpp.BackBufferHeight = 480;
    d3dpp.BackBufferCount = 0;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.Flags = D3DPRESENTFLAG_VIDEO;

    IDirect3DDevice9Ex* d3d9device;

    hr = IDirect3D9Ex_CreateDeviceEx(d3d9, adapter, D3DDEVTYPE_HAL, GetShellWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, NULL, &d3d9device);
    if (FAILED(hr))
    {
        AfxThrowError();
        //"Failed to create Direct3D device\n"
    }

    IDirect3DDeviceManager9* d3d9devmgr;

    hr = DXVA2CreateDirect3DDeviceManager9(&resetToken, &d3d9devmgr);
    if (FAILED(hr))
    {
        AfxThrowError();
        //"Failed to create Direct3D device manager\n"
    }

    hr = d3d9devmgr->lpVtbl->ResetDevice(d3d9devmgr, d3d9device, resetToken);
    if (FAILED(hr))
    {
        AfxThrowError();
        //"Failed to bind Direct3D device to device manager\n"
    }

    HANDLE deviceHandle;

    hr = d3d9devmgr->lpVtbl->OpenDeviceHandle(d3d9devmgr, &deviceHandle);
    if (FAILED(hr))
    {
        AfxThrowError();
        //"Failed to open device handle\n"
    }

    IDirectXVideoDecoderService* decoder_service;

    hr = d3d9devmgr->lpVtbl->GetVideoService(d3d9devmgr, deviceHandle, &IID_IDirectXVideoDecoderService, (void **)&decoder_service);
    if (FAILED(hr))
    {
        AfxThrowError();
        //"Failed to create IDirectXVideoDecoderService\n"
    }

    msys->deviceHandle = deviceHandle;
    msys->decoder_service = decoder_service;
    msys->d3d9devmgr = d3d9devmgr;
    msys->d3d9device = d3d9device;
    msys->d3d9 = d3d9;
    msys->resetToken = resetToken;
    msys->adapter = adapter;
#endif

    return err;
}
