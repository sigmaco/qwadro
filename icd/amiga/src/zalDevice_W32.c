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

#include "zalObjects.h"

_ZAL afxResult _ZalSdevIoctrlCb(afxMixDevice sdev, afxUnit reqCode, va_list va)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &sdev);
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
        if (!sdev->idd)
        {
            if (AfxAllocate(sizeof(sdev->idd[0]), 0, AfxHere(), (void**)&sdev->idd)) AfxThrowError();
            else
            {
                afxUri uri;
                afxUnit devPagIdx = sdev->dev.manifestPagNo, recIdx;
                afxModule icd = AfxGetDeviceDriver(&sdev->dev);
                afxManifest const* ini = AfxGetModuleManifest(icd);

                //if (!AfxFindInitializationRecord(ini, devPagIdx, &AFX_STRING("OpenglDll"), &recIdx) || !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
                //else
                AfxMakeUri(&uri, 0, "//./c/openal32.dll", 0);
                {
                    if (AfxAcquireModule(&uri, NIL, &sdev->idd->oalDll)) AfxThrowError();
                    else
                    {
                        alcVmt alc;
                        _ZalSpuLoadBaseSymbols(AfxGetModuleIdd(sdev->idd->oalDll), &alc);

                        // TODO detect features and limits

                        if (err)
                            AfxDisposeObjects(1, &sdev->idd->oalDll);
                    }
                }

                if (err)
                    AfxDeallocate((void**)&sdev->idd, AfxHere());
            }
        }
#endif
        break;
    }
    case 2: // Terminates the device. The device must first be stopped and then closed (in that order). This function should be called before the system shuts down.
    {
#if 0
        if (sdev->idd)
        {
            AfxExhaustChainedClasses(&sdev->dev.classes);

            if (sdev->idd->oalDll)
                AfxDisposeObjects(1, &sdev->idd->oalDll);

            AfxDeallocate((void**)&sdev->idd, AfxHere());
            sdev->idd = NIL;
        }
#endif
        break;
    }
    case 3: // Opens the device. This function must only be called after the device has been initialized.
    {
        afxMixSystem ssys = va_arg(va, afxMixSystem);

        if (ssys)
        {

        }
        break;
    }
    case 4: // Closes the device. This function must only be called after the device has been stopped.
    {
        afxMixSystem ssys = va_arg(va, afxMixSystem);

        if (ssys)
        {

        }
        break;
    }
    case 5:
        // run up the bridges. 
        // Starts it.
        // This function must only be called after the device has been initialized and opened (in that order).
    {
        afxMixSystem ssys = va_arg(va, afxMixSystem);

        if (!ssys) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &ssys);

            for (afxUnit i = 0; i < ssys->m.bridgeCnt; i++)
            {
                afxMixBridge sexu = ssys->m.bridges[i];
                AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &sexu);

                if (!AfxIsThreadRunning(sexu->m.worker))
                {
                    AfxRunThread(sexu->m.worker, (void*)sexu->m.workerProc, sexu);
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
        afxMixSystem ssys = va_arg(va, afxMixSystem);

        if (!ssys) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &ssys);

            for (afxUnit i = 0; i < ssys->m.bridgeCnt; i++)
            {
                afxMixBridge sexu = ssys->m.bridges[i];
                AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &sexu);

                afxInt exitCode = 0;
                do
                {
                    AfxRequestThreadInterruption(sexu->m.worker);
                    AmxWaitForIdleMixBridge(sexu, AFX_TIMEOUT_INFINITE);
                } while (!AfxWaitForThreadExit(sexu->m.worker, &exitCode));
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

_ZAL afxError _ZalMdevDtorCb(afxMixDevice mdev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxExhaustChainedClasses(&mdev->m.dev.classes);

    if (_AMX_MDEV_CLASS_CONFIG.dtor(mdev))
        AfxThrowError();

    if (mdev->oalDll)
        AfxDisposeObjects(1, &mdev->oalDll);

    return err;
}

_ZAL afxError _ZalMdevCtorCb(afxMixDevice mdev, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxMdevAcq const* info = (_amxMdevAcq const *)(args[1]) + invokeNo;
    AFX_ASSERT(info);

    static amxDeviceInfo const portCaps[] =
    {
        {
            .capabilities = amxService_SFX | amxService_DMA,
            .minQueCnt = 2,
            .maxQueCnt = 16,
            .acceleration = afxAcceleration_CPU
        }
    };

    if (_AMX_MDEV_CLASS_CONFIG.ctor(mdev, (void*[]) { icd, (void*)info }, 0))
    {
        AfxThrowError();
        return err;
    }

    afxUri uri;
    afxUnit devPagIdx = mdev->m.dev.manifestPagNo, recIdx;
    afxManifest const* ini = AfxGetModuleManifest(icd);

    //if (!AfxFindInitializationRecord(ini, devPagIdx, &AFX_STRING("OpenglDll"), &recIdx) || !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
    //else
#if 0
    AfxMakeUri(&uri, 0, "//./c/openal32.dll", 0);
    {
        if (AfxAcquireModule(&uri, NIL, &mdev->oalDll)) AfxThrowError();
        else
        {
            // TODO detect features and limits

            if (err)
                AfxDisposeObjects(1, &mdev->oalDll);
        }
    }
#endif

    if (err && _AMX_MDEV_CLASS_CONFIG.dtor(mdev))
        AfxThrowError();

    return err;
}

_ZAL afxError afxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig mdevClsCfg = _AMX_MDEV_CLASS_CONFIG;
    mdevClsCfg.fixedSiz = sizeof(AFX_OBJECT(afxMixDevice));
    mdevClsCfg.ctor = (void*)_ZalMdevCtorCb;
    mdevClsCfg.dtor = (void*)_ZalMdevDtorCb;

    afxClassConfig msysClsCfg = _AMX_MSYS_CLASS_CONFIG;
    msysClsCfg.fixedSiz = sizeof(AFX_OBJ(afxMixSystem));
    msysClsCfg.ctor = (void*)_ZalMsysCtorCb;
    msysClsCfg.dtor = (void*)_ZalMsysDtorCb;

    _amxImplementation impl = { 0 };
    impl.icd = icd;
    impl.mcdcCls = _AMX_MCDC_CLASS_CONFIG;
    impl.mdevCls = mdevClsCfg;
    impl.msysCls = msysClsCfg;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    if (_AmxIcdImplement(sys, &impl))
    {
        AfxThrowError();
        return err;
    }

    static amxFeatures features = { 0 };

    _amxMdevAcq mdevInfos[] =
    {
        {
            .dev.urn = AFX_STRING("amiga-sfx"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,
            .capabilities = amxService_SFX | amxService_DMA | amxService_PCX | amxService_SINK,
            .acceleration = afxAcceleration_MPU,
            .minQueCnt = 2,
            .maxQueCnt = 16,

            .dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

        },
        {
            .dev.urn = AFX_STRING("amiga-pcx"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,
            .capabilities = amxService_DMA | amxService_PCX,
            .acceleration = afxAcceleration_MPU,
            .minQueCnt = 2,
            .maxQueCnt = 16,

            .dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

        },
        {
            .dev.urn = AFX_STRING("amiga-dma"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,
            .capabilities = amxService_DMA,
            .acceleration = afxAcceleration_MPU,
            .minQueCnt = 2,
            .maxQueCnt = 16,

            .dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

        },
        {
            .dev.urn = AFX_STRING("amiga-sink"),
            .dev.type = afxDeviceType_SOUND,

            .features = features,
            .capabilities = amxService_SINK,
            .acceleration = afxAcceleration_MPU,
            .minQueCnt = 2,
            .maxQueCnt = 16,

            .dev.ioctl = (void*)_ZalSdevIoctrlCb,
            //.relinkAsioCb = _ZalRelinkAsioWasapiCb,

        },
    };

    afxMixDevice mdevices[ARRAY_SIZE(mdevInfos)];

    if (_AmxIcdRegisterDevices(icd, ARRAY_SIZE(mdevInfos), mdevInfos, mdevices))
    {
        AfxThrowError();
    }
    else
    {

    }
    return err;
}
