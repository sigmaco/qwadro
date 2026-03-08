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

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

//#define _AVOID_WGL_FUNCS 1

//#define USE_SCREEN_DC 1

extern afxClassConfig _ZglDexuMgrCfg;

_ZGL afxString const targaSignature = AFX_STRING(
    "           :::::::::::     :::     :::::::::   ::::::::      :::               \n"
    "               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             \n"
    "               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            \n"
    "               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           \n"
    "               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           \n"
    "               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           \n"
    "               ###     ###     ### ###    ###  ########  ###     ###           \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_ZGL afxError _ZglProcessDeletionQueue(glVmt const* gl, afxInterlockedQueue* deletionQueue)
{
    afxError err = { 0 };

    _zglDeleteGlRes res;
    while (AfxPopInterlockedQueue(deletionQueue, &res))
    {
        switch (res.type)
        {
        case GL_BUFFER:
            AFX_ASSERT(gl->IsBuffer(res.gpuHandle));
            gl->DeleteBuffers(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsBuffer(res.gpuHandle));
            break;
        case GL_TEXTURE:
            AFX_ASSERT(gl->IsTexture(res.gpuHandle));
            gl->DeleteTextures(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsTexture(res.gpuHandle));
            break;
        case GL_RENDERBUFFER:
            AFX_ASSERT(gl->IsRenderBuffer(res.gpuHandle));
            gl->DeleteRenderbuffers(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsRenderBuffer(res.gpuHandle));
            break;
        case GL_FRAMEBUFFER:
            AFX_ASSERT(gl->IsFramebuffer(res.gpuHandle));
            gl->DeleteFramebuffers(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsFramebuffer(res.gpuHandle));
            break;
        case GL_SAMPLER:
            AFX_ASSERT(gl->IsSampler(res.gpuHandle));
            gl->DeleteSamplers(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsSampler(res.gpuHandle));
            break;
        case GL_PROGRAM:
            AFX_ASSERT(gl->IsProgram(res.gpuHandle));
            gl->DeleteProgram(res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsProgram(res.gpuHandle));
            break;
        case GL_SHADER:
            AFX_ASSERT(gl->IsShader(res.gpuHandle));
            gl->DeleteShader(res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsShader(res.gpuHandle));
            break;
        case GL_VERTEX_ARRAY:
            AFX_ASSERT(gl->IsVertexArray(res.gpuHandle));
            gl->GenVertexArrays(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsVertexArray(res.gpuHandle));
            break;
        case GL_PROGRAM_PIPELINE:
            AFX_ASSERT(gl->IsProgramPipeline(res.gpuHandle));
            gl->DeleteProgramPipelines(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsProgramPipeline(res.gpuHandle));
            break;
        case GL_SYNC_FENCE:
            AFX_ASSERT(gl->IsSync(res.gpuHandlePtr));
            gl->DeleteSync(res.gpuHandlePtr); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsSync(res.gpuHandlePtr));
            break;
        case GL_QUERY_BUFFER:
            AFX_ASSERT(gl->IsQuery(res.gpuHandle));
            gl->DeleteQueries(1, &res.gpuHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(!gl->IsQuery(res.gpuHandle));
            break;
        default:
            AfxReportError("");
            break;
        }
    }
    return err;
}

_ZGL void _ZglDsysEnqueueDeletion(afxDrawSystem dsys, afxUnit exuIdx, afxUnit type, afxSize gpuHandle)
{
    //AfxEntry("dsys=%p", dsys);
    afxError err = { 0 };

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxDrawBridge dexu = dsys->m.bridges[exuIdx];
    afxDrawDevice ddev = dexu->m.ddev;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    
    _zglDeleteGlRes delRes;
    delRes.gpuHandlePtr = (void*)gpuHandle;
    delRes.type = type;

    while(!AfxPushInterlockedQueue(&dexu->deletionQueue, &delRes))
        AfxYield();

    dexu->m.schedCnt += 1;
    AfxSignalCondition2(&dexu->m.schedCnd);
}

_ZGL afxResult _ZglDdevIoctrlCb(afxDrawDevice ddev, afxUnit reqCode, va_list va)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case 0:
    {
        
        break;
    }
    case 1: // init
    {
#if 0
        if (!ddev->idd)
        {
            if (AfxAllocate(sizeof(ddev->idd[0]), 0, AfxHere(), (void**)&ddev->idd)) AfxThrowError();
            else
            {
                afxUri uri;
                afxUnit devPagIdx = ddev->dev.manifestPagNo, recIdx;
                afxModule icd = AfxGetDeviceDriver(&ddev->dev);
                afxManifest const* ini = AfxGetModuleManifest(icd);

                //if (!AfxFindInitializationRecord(ini, devPagIdx, &AFX_STRING("OpenglDll"), &recIdx) || !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
                //else
                AfxMakeUri(&uri, 0, "//./c/opengl32.dll", 0);
                {
                    if (AfxAcquireModule(&uri, NIL, &ddev->idd->oglDll)) AfxThrowError();
                    else
                    {
                        //ZglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, ddev->p);

                        ddev->idd->wndClss.cbSize = sizeof(ddev->idd->wndClss); // only on EX
                        ddev->idd->wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
                        ddev->idd->wndClss.lpfnWndProc = _ZglWndHndlngPrcW32Callback;
                        ddev->idd->wndClss.cbClsExtra = sizeof(ddev);
                        ddev->idd->wndClss.cbWndExtra = sizeof((afxSurface)0);
                        ddev->idd->wndClss.hInstance = GetModuleHandleA(NULL);
                        ddev->idd->wndClss.hIcon = LoadIconA(NULL, IDI_SHIELD);
                        ddev->idd->wndClss.hCursor = LoadCursorA(NULL, IDC_ARROW);
                        ddev->idd->wndClss.hbrBackground = NULL;
                        ddev->idd->wndClss.lpszMenuName = NULL;
                        ddev->idd->wndClss.lpszClassName = "SIGMA GL/2 Continuous Integration --- Qwadro Video Graphics Infrastructure (c) 2017 SIGMA --- Public Test Build";
                        ddev->idd->wndClss.hIconSm = LoadIconA(NULL, IDI_SHIELD);

                        if (!(RegisterClassExA(&(ddev->idd->wndClss)))) AfxThrowError();
                        else
                        {
                            AfxReportComment("Listing mem available display devices...\n");

                            //HDC hdc = NIL;
                            afxUnit idx = 0;
                            while (1)
                            {
                                DISPLAY_DEVICE dispdev = { 0 };
                                dispdev.cb = sizeof(dispdev);

                                if (!(EnumDisplayDevicesA(NULL, idx, &dispdev, NIL))) break;
                                else
                                {
                                    AfxReportComment("#%u = %s (%s) %x", idx, dispdev.DeviceString, dispdev.DeviceName, dispdev.StateFlags);
                                    idx++;
                                }
                            }

                            afxUnit verMajor, verMinor, verPatch;
                            wglLoadWsiSymbolsSIG(AfxGetModuleIdd(ddev->idd->oglDll), &verMajor, &verMinor, &verPatch);
                            ddev->idd->oglVerMajor = verMajor;
                            ddev->idd->oglVerMinor = verMinor;
                            ddev->idd->oglVerPatch = verPatch;
                            
                            HWND tmpHwnd;
                            HDC tmpHdc;
                            int pixelFmt;
                            PIXELFORMATDESCRIPTOR pfd;

                            if (wglCreateSurfaceSIGMA(&ddev->idd->wndClss, &tmpHwnd, &tmpHdc, &pixelFmt, &pfd)) AfxThrowError();
                            else
                            {
                                HDC bkpHdc = wglGetCurrentDrawDCSIG();
                                HGLRC bkpGlrc = wglGetCurrentContextSIG();

                                HGLRC tmpHglrc;
                                glVmt gl;

                                if (wglCreateContextSIGMA(ddev->idd->oglDll, tmpHdc, NIL, &tmpHglrc, &gl)) AfxThrowError();
                                else
                                {
                                    wglMakeContextCurrentSIG(tmpHdc, tmpHdc, tmpHglrc);

                                    wglDetectDeviceFeaturesSIGMA(&gl, tmpHdc, &ddev->features);
                                    wglDetectDeviceLimitsSIGMA(&gl, &ddev->limits);

                                    ddev->idd->hasDxInterop2 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop2");
                                    ddev->idd->hasDxInterop1 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop");

                                    if (ddev->idd->hasDxInterop1 || ddev->idd->hasDxInterop2)
                                    {
                                        //ddev->idd->useDxgiSwapchain = FALSE;
                                    }

#if 0
                                    if (0 == unitIdx)
                                    {
                                        GLint n = 0;
                                        gl->GetIntegerv(GL_NUM_EXTENSIONS, &n); _ZglThrowErrorOccuried();
                                        AfxReportMessage("Listing #%03u \"core\" GL supported extensions.", n);

                                        for (GLint i = 0; i < n; i++)
                                        {
                                            const char* extension = (const char*)gl->GetStringi(GL_EXTENSIONS, i); _ZglThrowErrorOccuried();
                                            AfxReportMessage("%.3u %s ext found.", i, extension);
                                        }
                                    }
#endif

                                    wglMakeContextCurrentSIG(NIL, NIL, NIL);
                                    wglDeleteContextSIG(tmpHglrc), tmpHglrc = NIL;
                                }
                                ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
                                DestroyWindow(tmpHwnd), tmpHwnd = NIL;
                                wglMakeContextCurrentSIG(bkpHdc, bkpHdc, bkpGlrc);
                            }

                            if (err)
                                UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);
                        }

                        if (err)
                            AfxDisposeObjects(1, &ddev->idd->oglDll);
                    }
                }

                if (err)
                    AfxDeallocate((void**)&ddev->idd, AfxHere());
            }
        }
#endif
        break;
    }
    case 2: // Terminates the device. The device must first be stopped and then closed (in that order). This function should be called before the system shuts down.
    {
#if 0
        if (ddev->idd)
        {
            AfxExhaustChainedClasses(&ddev->dev.classes);

            UnregisterClassA(ddev->idd->wndClss.lpszClassName, ddev->idd->wndClss.hInstance);

            if (ddev->idd->oglDll)
                AfxDisposeObjects(1, &ddev->idd->oglDll);

            AfxDeallocate((void**)&ddev->idd, AfxHere());
            ddev->idd = NIL;
        }
#endif
        break;
    }
    case 3: // Opens the device. This function must only be called after the device has been initialized.
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (dsys)
        {

        }
        break;
    }
    case 4: // Closes the device. This function must only be called after the device has been stopped.
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (dsys)
        {

        }
        break;
    }
    case 5: 
        // run up the bridges. 
        // Starts it.
        // This function must only be called after the device has been initialized and opened (in that order).
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (!dsys) AfxThrowError();
        else
        {
            for (afxUnit i = 0; i < dsys->m.bridgeCnt; i++)
            {
                afxDrawBridge dexu = dsys->m.bridges[i];
                AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

                if (!AfxIsThreadRunning(dexu->m.worker))
                {
                    AfxRunThread(dexu->m.worker, (void*)dexu->m.workerProc, dexu);
                }
            }
        }
        break;
    }
    case 6:
        // run down the bridges. 
        // Stops it. 
        // This function must be called before any attempt is made to close (and then terminate, if required) the device.
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (!dsys) AfxThrowError();
        else
        {
            for (afxUnit i = 0; i < dsys->m.bridgeCnt; i++)
            {
                afxDrawBridge dexu = dsys->m.bridges[i];
                AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);

                afxInt exitCode = 0;
                do
                {
                    AfxRequestThreadInterruption(dexu->m.worker);
                    AvxWaitForIdleDrawBridge(dexu, AFX_TIMEOUT_INFINITE);
                } while (!AfxWaitForThreadExit(dexu->m.worker, &exitCode));
            }
        }
        break;
    }
    default: break;
    }

    // Note the sequence of function calls used to run-up and run-down the engine is as follows:
    // - RwEngineInit, RwEngineOpen, RwEngineStart to run-up; and
    // - RwEngineStop, RwEngineClose, RwEngineTerm to run-down.

    return rslt;
}

_ZGL afxError _ZglDdevDtorCb(afxDrawDevice ddev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    AfxExhaustChainedClasses(&ddev->m.dev.classes);

    if (_AVX_CLASS_CONFIG_DDEV.dtor(ddev))
        AfxThrowError();

    if (ddev->wglDll)
        AfxDisposeObjects(1, &ddev->wglDll);

    if (ddev->d3d9Dll)
        AfxDisposeObjects(1, &ddev->d3d9Dll);

    return err;
}

_ZGL afxError _ZglDdevCtorCb(afxDrawDevice ddev, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _avxDdevReg const* info = (_avxDdevReg const *)(args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AVX_CLASS_CONFIG_DDEV.ctor(ddev, (void*[]) { icd, (void*)info, args[2] }, 0))
    {
        AfxThrowError();
        return err;
    }

    ddev->m.limits = info->limits;
    ddev->m.features = info->features;

#if 0
    afxUri uri;
    afxUnit devPagIdx = ddev->m.dev.manifestPagNo, recIdx;
    afxManifest const* ini = AfxGetModuleManifest(icd);

    //if (!AfxFindInitializationRecord(ini, devPagIdx, &AFX_STRING("OpenglDll"), &recIdx) || !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
    //else
    AfxMakeUri(&uri, 0, "//./c/opengl32.dll", 0);
    {
        if (AfxAcquireModule(&uri, NIL, &ddev->oglDll)) AfxThrowError();
        else
        {
            //ZglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, ddev->p);

            afxUnit verMajor, verMinor, verPatch;
            //wglLoadWsiSymbolsSIG(AfxGetModuleIdd(ddev->oglDll), &verMajor, &verMinor, &verPatch);

            wglLoadModule(AfxGetModuleIdd(ddev->oglDll), &verMajor, &verMinor, &verPatch);

            ddev->oglVerMajor = verMajor;
            ddev->oglVerMinor = verMinor;
            ddev->oglVerPatch = verPatch;

            HWND tmpHwnd;
            HDC tmpHdc;
            int pixelFmt;
            PIXELFORMATDESCRIPTOR pfd;
            if (wglCreateSurfaceSIGMA(0, 0, &tmpHwnd, &tmpHdc, &pixelFmt, &pfd)) AfxThrowError();
            else
            {
                HDC bkpHdc = wglGetCurrentDCWIN();
                HGLRC bkpGlrc = wglGetCurrentContextWIN();


#if _AFX_DEBUG
                afxBool echoSymbols = TRUE;
                        
#else
                afxBool echoSymbols = FALSE;
#endif

                glVmt gl;
                HGLRC tmpHglrc;
                if (wglCreateContextSIGMA(tmpHdc, NIL, verMajor, verMinor, FALSE, &tmpHglrc, &gl, echoSymbols)) AfxThrowError();
                else
                {
                    wglMakeCurrentWIN(tmpHdc, tmpHglrc);

                    TestCoreSymbols(AfxGetModuleIdd(ddev->oglDll), &gl);

                    wglDetectDeviceFeaturesSIGMA(&gl, tmpHdc, &ddev->m.features);
                    wglDetectDeviceLimitsSIGMA(&gl, &ddev->m.limits);

                    //EnumDisplayMonitors(tmpHdc, NIL, );

                    ddev->hasDxInterop2 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop2");
                    ddev->hasDxInterop1 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop");
                    
                    // 16384 * 16384 * 4 / 1024 / 1024 = 1024MB

                    if (ddev->hasDxInterop1 || ddev->hasDxInterop2)
                    {
                        //ddev->idd->useDxgiSwapchain = FALSE;
                    }

#if !0
                    if (1)
                    {
                        GLint n = 0;
                        gl.GetIntegerv(GL_NUM_EXTENSIONS, &n);
                        AfxReportMessage("GL: Listing #%03u supported extensions.", n);

                        for (GLint i = 0; i < n; i++)
                        {
                            const char* extension = (const char*)gl.GetStringi(GL_EXTENSIONS, i);
                            AfxReportMessage("  %3u %s", i, extension);
                        }
                    }
#endif

                    static avxDeviceInfo const portCaps[] =
                    {
                        {
                            .capabilities = avxAptitude_GFX | avxAptitude_PCX | avxAptitude_DMA | avxAptitude_BLIT | avxAptitude_SAMPLE | avxAptitude_VCX,
                            .minQueCnt = 4,
                            .maxQueCnt = 16,
                            .acceleration = afxAcceleration_DPU
                        },
                        {
                            .capabilities = avxAptitude_PCX | avxAptitude_DMA,
                            .minQueCnt = 2,
                            .maxQueCnt = 16,
                            .acceleration = afxAcceleration_DPU
                        },
                        {
                            .capabilities = avxAptitude_DMA,
                            .minQueCnt = 2,
                            .maxQueCnt = 16,
                            .acceleration = afxAcceleration_DPU
                        },
                        {
                            .capabilities = avxAptitude_PRESENT,
                            .minQueCnt = 2,
                            .maxQueCnt = 16,
                            .acceleration = afxAcceleration_DPU
                        }
                    };

                    if (wglHasWsiExtensionSIG(tmpHdc, "GL_ARB_clip_control"))
                    {
                        ddev->m.clipSpaceDepth = (avxClipSpaceDepth_NEG_ONE_TO_ONE | avxClipSpaceDepth_ZERO_TO_ONE);
                    }
                    else
                    {
                        ddev->m.clipSpaceDepth = avxClipSpaceDepth_NEG_ONE_TO_ONE;
                    }

                    for (afxUnit i = 0; i < ddev->m.portCnt; i++)
                    {
                        avxDeviceInfo* caps = &ddev->m.ports[i].caps;
                        *caps = portCaps[i];
                    }

                    wglMakeCurrentWIN(NIL, NIL);
                    wglDeleteContextWIN(tmpHglrc), tmpHglrc = NIL;
                }
                ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
                DestroyWindow(tmpHwnd), tmpHwnd = NIL;
                wglMakeCurrentWIN(bkpHdc, bkpGlrc);
            }

            if (err)
                AfxDisposeObjects(1, &ddev->oglDll);
        }
    }
#endif
    if (err && _AVX_CLASS_CONFIG_DDEV.dtor(ddev))
        AfxThrowError();

    return err;
}

_ZGL afxError afxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig ddevClsCfg = _AVX_CLASS_CONFIG_DDEV;
    ddevClsCfg.fixedSiz = sizeof(AFX_OBJECT(afxDrawDevice));
    ddevClsCfg.ctor = (void*)_ZglDdevCtorCb;
    ddevClsCfg.dtor = (void*)_ZglDdevDtorCb;

    afxClassConfig dsysClsCfg = _AVX_CLASS_CONFIG_DSYS;
    dsysClsCfg.fixedSiz = sizeof(AFX_OBJ(afxDrawSystem));
    dsysClsCfg.ctor = (void*)_ZglDsysCtorCb;
    dsysClsCfg.dtor = (void*)_ZglDsysDtorCb;

    _avxImplementation impl = { 0 };
    impl.ddevCls = ddevClsCfg;
    impl.dsysCls = dsysClsCfg;

    if (_AvxIcdImplement(icd, &impl))
    {
        AfxThrowError();
        return err;
    }

    afxUri uri;
    afxModule wglDll;
    //AfxMakeUri(&uri, 0, "//./c/opengl32.dll", 0);
    AfxMakeUri(&uri, 0, "opengl32.dll", 0);
    if (AfxAcquireModule(&uri, NIL, &wglDll))
    {
        AfxMakeUri(&uri, 0, "//./c/opengl32.dll", 0);
        if (AfxAcquireModule(&uri, NIL, &wglDll))
        {
            AfxThrowError();
            AfxReportError("Could not load WGL DLL.");
        }
    }
    HMODULE oglDllIdd = AfxGetModuleIdd(wglDll);
    AFX_ASSERT(oglDllIdd);

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);
    AfxGetModulePath(wglDll, &urib.uri);

    AfxReportComment("WGL Module: %.*s", AfxPushString(&urib.uri.s));

    afxUnit oglVerMajor, oglVerMinor, oglVerPatch;
    if (wglLoadModule(oglDllIdd, &oglVerMajor, &oglVerMinor, &oglVerPatch))
    {
        AfxThrowError();
    }

    AfxReportComment("GL Version: %u, %u, %u", oglVerMajor, oglVerMinor, oglVerPatch);
    AfxReportComment("Acquiring HW drawable surface...");
    
    HDC tmpHdc;
    HWND tmpHwnd;
    int pixelFmt;
    PIXELFORMATDESCRIPTOR pfd;
    if (wglCreateSurfaceSIGMA(0, 0, &tmpHwnd, &tmpHdc, &pixelFmt, &pfd))
    {
        AfxThrowError();
        AfxReportError("Could not create HW drawable surface.");
    }

#if 0
#if _AFX_DEBUG
    afxBool echoSymbols = TRUE;
#else
    afxBool echoSymbols = FALSE;
#endif
#else
    afxBool echoSymbols = FALSE;
#endif

    AfxReportComment("Acquiring HW operation context...");

    glVmt gl;
    HGLRC tmpHglrc;
    HDC bkpHdc = wglGetCurrentDCWIN();
    HGLRC bkpGlrc = wglGetCurrentContextWIN();
    if (wglCreateContextSIGMA(tmpHdc, NIL, oglVerMajor, oglVerMinor, FALSE, FALSE, FALSE, &tmpHglrc, &gl, echoSymbols))
    {
        AfxThrowError();
        AfxReportError("Could not create HW operation context.");
    }
    else
    {
        //AfxReportComment("wglMakeCurrentWIN");

        if (!wglMakeCurrentWIN(tmpHdc, tmpHglrc))
        {
            AfxThrowError();
        }

        //AfxReportComment("Testing core symbols...");
        TestCoreSymbols(oglDllIdd, &gl);
#if !0
        afxBool echoExts = TRUE;
        if (echoExts)
        {
            /*
                As of OpenGL 3.0, glGetString(GL_EXTENSIONS) is deprecated, and the extension query must be done with 
                glGetStringi(GL_EXTENSIONS, index) for modern OpenGL contexts.
            */

            GLint n = 0;
            gl.GetIntegerv(GL_NUM_EXTENSIONS, &n);
            AfxReportMessage("GL: Listing #%03u supported extensions.", n);

            for (GLint i = 0; i < n; i++)
            {
                const char* extension = (const char*)gl.GetStringi(GL_EXTENSIONS, i);
                AfxReportMessage("  %3u %s", i, extension);
            }
        }
#endif
    }

    static avxLimits drawLimits = { 0 };
    static avxFeatures drawFeatures =
    {
        .robustness = TRUE,
        .fullDrawIdxUint32 = TRUE,
        .rasterCubeArray = TRUE,
        .independentBlend = TRUE,
        .primShader = TRUE,
        .dualSrcBlend = TRUE,
        .logicOp = TRUE,
        .mdi = TRUE,
        .baseInst = TRUE,
        .depthClamp = TRUE,
        .depthBiasClamp = TRUE,
        .fillModeNonSolid = TRUE,
        .viewports = TRUE,
        .dxt = TRUE,
        .clipDist = TRUE,
        .cullDist = TRUE,
        .alphaToOne = TRUE
    };

    // 16384 * 16384 * 4 / 1024 / 1024 = 1024MB

    wglDetectDeviceFeaturesSIGMA(&gl, tmpHdc, &drawFeatures);
    wglDetectDeviceLimitsSIGMA(&gl, &drawLimits);

    //EnumDisplayMonitors(tmpHdc, NIL, );

    afxBool hasDxInterop2 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop2");
    afxBool hasDxInterop1 = wglHasWsiExtensionSIG(tmpHdc, "WGL_NV_DX_interop");
    if (hasDxInterop1 || hasDxInterop2)
    {
        //ddev->idd->useDxgiSwapchain = FALSE;
    }

    avxClipSpaceDepth clipSpaceDepth = avxClipSpaceDepth_NEG_ONE_TO_ONE;
    if (glHasExtensionSIG(&gl, "GL_ARB_clip_control"))
        clipSpaceDepth = (avxClipSpaceDepth_NEG_ONE_TO_ONE | avxClipSpaceDepth_ZERO_TO_ONE);

    afxBool hasCompute = glHasExtensionSIG(&gl, "GL_ARB_compute_shader");

#if !0
    if (!err)
    {
        // required by avxVertexInput.
        afxBool has_ARB_vertex_array_object = glHasExtensionSIG(&gl, "GL_ARB_vertex_array_object");
        AFX_ASSERT(has_ARB_vertex_array_object);
        // required by avxVertexInput for attribute location.
        // required by avxVertexInput for setting the buffer binding point data. (glBindVertexBuffer, glVertexBindingDivisor)
        afxBool has_ARB_vertex_attrib_binding = glHasExtensionSIG(&gl, "GL_ARB_vertex_attrib_binding");
        AFX_ASSERT(has_ARB_vertex_attrib_binding);

        // required by avxBuffer.
        afxBool has_ARB_buffer_storage = glHasExtensionSIG(&gl, "GL_ARB_buffer_storage");
        AFX_ASSERT(has_ARB_buffer_storage);
        // required by avxBuffer for avxBufferUsage_SSBO.
        afxBool has_ARB_shader_storage_buffer_object = glHasExtensionSIG(&gl, "GL_ARB_shader_storage_buffer_object");
        AFX_ASSERT(has_ARB_shader_storage_buffer_object);
        // required by avxBuffer (or avxLigature?) for avxBufferUsage_FETCH.
        afxBool has_ARB_texture_buffer_range = glHasExtensionSIG(&gl, "GL_ARB_texture_buffer_range");
        AFX_ASSERT(has_ARB_texture_buffer_range);
        
        // required by avxRaster.
        afxBool has_ARB_texture_storage = glHasExtensionSIG(&gl, "GL_ARB_texture_storage");
        AFX_ASSERT(has_ARB_texture_storage);

        // required by avxSampler.
        afxBool has_ARB_sampler_objects = glHasExtensionSIG(&gl, "GL_ARB_sampler_objects");
        AFX_ASSERT(has_ARB_sampler_objects);
        // required by avxSampler for anisotropy.
        afxBool has_ARB_texture_filter_anisotropic = glHasExtensionSIG(&gl, "GL_ARB_texture_filter_anisotropic") | glHasExtensionSIG(&gl, "GL_EXT_texture_filter_anisotropic");
        AFX_ASSERT(has_ARB_texture_filter_anisotropic);
        
        err = 0;
    }
#endif

    _avxDdevReg const gfxDdevInfo =
    {
        .dev.urn = AFX_STRING("targa-gfx"),
        .dev.type = afxDeviceType_DRAW,
        .dev.ioctl = (void*)_ZglDdevIoctrlCb,

        //.features = drawFeatures,
        .clipSpaceDepth = clipSpaceDepth,

        //.portCnt = 4,

        .capabilities = avxAptitude_GFX | 
                        avxAptitude_DMA | 
                        avxAptitude_PRESENT,
        .minQueCnt = 4,
        .maxQueCnt = 16,
        .acceleration = afxAcceleration_DPU | afxAcceleration_GPU
    };
    _avxDdevReg const pcxDdevInfo =
    {
        .dev.urn = AFX_STRING("targa-pcx"),
        .dev.type = afxDeviceType_DRAW,
        .dev.ioctl = (void*)_ZglDdevIoctrlCb,

        //.features = drawFeatures,
        .clipSpaceDepth = clipSpaceDepth,

        //.portCnt = 4,

        .capabilities = avxAptitude_PCX | 
                        avxAptitude_DMA,
        .minQueCnt = 2,
        .maxQueCnt = 16,
        .acceleration = afxAcceleration_DPU | afxAcceleration_GPU
    };
    _avxDdevReg const dmaDdevInfo =
    {
        .dev.urn = AFX_STRING("targa-dma"),
        .dev.type = afxDeviceType_DRAW,
        .dev.ioctl = (void*)_ZglDdevIoctrlCb,

        //.features = drawFeatures,
        .clipSpaceDepth = clipSpaceDepth,

        //.portCnt = 4,

        .capabilities = avxAptitude_DMA,
        .minQueCnt = 2,
        .maxQueCnt = 16,
        .acceleration = afxAcceleration_DPU | afxAcceleration_GPU
    };
    _avxDdevReg const vcxDdevInfo =
    {
        .dev.urn = AFX_STRING("targa-vcx"),
        .dev.type = afxDeviceType_DRAW,
        .dev.ioctl = (void*)_ZglDdevIoctrlCb,

        //.features = drawFeatures,
        .clipSpaceDepth = clipSpaceDepth,

        //.portCnt = 4,

        .capabilities = avxAptitude_PRESENT | avxAptitude_DMA | avxAptitude_VCX,
        .minQueCnt = 2,
        .maxQueCnt = 16,
        .acceleration = afxAcceleration_DPU
    };

    afxUnit ddevCnt = 0;
    _avxDdevReg ddevInfos[16] = { 0 };

    // add device's graphics port
    ddevInfos[ddevCnt] = gfxDdevInfo;
    //if (hasCompute) ddevInfos[ddevCnt].capabilities |= avxAptitude_PCX;
    ++ddevCnt;

    // TODO: Find a better way of doing it.
    //if (drawLimits.maxComputeWarpCnt.w)
    if (hasCompute)
    {
        // add device's compute port
        ddevInfos[ddevCnt] = pcxDdevInfo;
        ++ddevCnt;
    }

    // add devices' transfer port
    ddevInfos[ddevCnt] = dmaDdevInfo;
    ++ddevCnt;

    // add devices' present port
    ddevInfos[ddevCnt] = vcxDdevInfo;
    ++ddevCnt;

    for (afxUnit ddevIdx = 0; ddevIdx < ddevCnt; ddevIdx++)
    {
        _avxDdevReg* ddevn = &ddevInfos[ddevIdx];
        ddevn->limits = drawLimits;
        ddevn->features = drawFeatures;
    }

    afxDrawDevice ddevices[ARRAY_SIZE(ddevInfos)] = { 0 };

    if (_AvxIcdRegisterDevices(icd, ddevCnt, ddevInfos, ddevices))
    {
        AfxThrowError();
    }

    for (afxUnit ddevIdx = 0; ddevIdx < ddevCnt; ddevIdx++)
    {
        afxDrawDevice ddev = ddevices[ddevIdx];
        ddev->wglDll = wglDll;
        ddev->hasDxInterop1 = hasDxInterop1;
        ddev->hasDxInterop2 = hasDxInterop2;
        ddev->useDxgiSwapchain = FALSE;
        ddev->oglVerMajor = oglVerMajor;
        ddev->oglVerMinor = oglVerMinor;
        ddev->oglVerPatch = oglVerPatch;
    }

    wglMakeCurrentWIN(NIL, NIL);
    wglDeleteContextWIN(tmpHglrc), tmpHglrc = NIL;
    
    ReleaseDC(tmpHwnd, tmpHdc), tmpHdc = NIL;
    DestroyWindow(tmpHwnd), tmpHwnd = NIL;
    wglMakeCurrentWIN(bkpHdc, bkpGlrc);

    if (!err)
    {
    }
    return err;
}
