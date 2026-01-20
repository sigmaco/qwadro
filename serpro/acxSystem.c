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

// This software is part of Advanced Multimedia Extensions.

// It is hard to invent something when there is nothing to be copied.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ACX_STEP_C
#define _ACX_STEP_SYSTEM_C
#include "acxIcd.h"

_ACX _acxDdiSsys const* _AcxSsysGetDdi(afxStepSystem ssys)
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    return ssys->ddi;
}

_ACX afxMask _AcxSsysGetIoExuMask(afxStepSystem ssys, afxMask* dedIoExuMask)
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    if (dedIoExuMask) *dedIoExuMask = ssys->dedIoExuMask;
    return ssys->ioExuMask;
}

_ACX afxModule AcxGetStepSystemIcd(afxStepSystem ssys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxModule mdle = AfxGetHost(ssys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle;
}

_ACX void AcxGetEnabledSystemFeatures(afxStepSystem ssys, acxFeatures* features)
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    AFX_ASSERT(features);
    *features = ssys->requirements;

    return;
}

_ACX afxClass const* _AcxSsysGetSexuClass(afxStepSystem ssys)
{
    afxError err = { 0 };
    // ssys must be a valid afxStepDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->sexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);
    return cls;
}

_ACX afxClass const* _AcxSsysGetSctxClass(afxStepSystem ssys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->sctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCTX);
    return cls;
}

_ACX afxClass const* _AcxSsysGetBufClass(afxStepSystem ssys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxClass const* cls = &ssys->sbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);
    return cls;
}

_ACX _acxDdiSsys const _ACX_SSYS_IMPL =
{
#if 0
    .fencCls = _AcxSsysGetFencClassCb_SW,
    .sexuCls = _AcxSsysGetDexuClassCb_SW,
    .sinkCls = _AcxSsysGetDoutClassCb_SW,
    .bufCls = _AcxSsysGetBufClassCb_SW,
#endif
    .transferCb = _AcxSsysTransferCb_SW,
    .cohereCb = _AcxSsysCohereMappedBuffersCb_SW,
    .remapCb = _AcxSsysRemapBuffersCb_SW,
#if 0
    .allocBufCb = _AcxSsysAllocateBuffersCb_SW,
    .deallocBufCb = _AcxSsysDeallocateBuffersCb_SW,
#endif
};

_ACX afxUnit AcxGetStepBridges(afxStepSystem ssys, afxUnit baseIdx, afxUnit cnt, afxStepBridge bridges[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    afxUnit bridgeCnt = ssys->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxStepBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxStepBridge sexu = ssys->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
        bridges[rslt++] = sexu;
    }
    return rslt;
}

_ACX afxUnit AcxChooseStepBridges(afxStepSystem ssys, afxUnit sdevId, acxAptitude caps, afxMask exuMask, afxUnit first, afxUnit maxCnt, afxStepBridge bridges[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((sdevId == AFX_INVALID_INDEX) && (!caps) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, ssys->bridgeCnt - first) : AFX_MIN(ssys->bridgeCnt, ssys->bridgeCnt - first);

    afxUnit bridgeCnt = ssys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < bridgeCnt; exuIdx++)
    {
        // skip if a mask is specified and this EXU is not bitmapped in it.
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxStepBridge sexu = ssys->bridges[exuIdx];
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

        afxStepDevice sdev = AcxGetBridgedStepDevice(sexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

        if ((sdevId != AFX_INVALID_INDEX) && (sdevId != AfxGetObjectId(sdev)))
            continue;

        if (caps)
        {
            acxDeviceInfo capsi;
            AcxQueryStepCapabilities(sdev, &capsi);

            if ((capsi.capabilities & caps) != caps)
                continue;
        }

        if (maxCnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = sexu;
            }

            if (maxCnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_ACX afxError AcxWaitForStepQueue(afxStepSystem ssys, afxTime timeout, afxUnit exuIdx, afxUnit queId)
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(ssys->bridgeCnt, exuIdx, 1);

    afxStepBridge sexu;
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(ssys->bridgeCnt, exuIdx, 1);
    while (!AcxGetStepBridges(ssys, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxStepQueue sque;
    if (!AcxGetStepQueues(sexu, queId, 1, &sque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (AcxWaitForEmptyStepQueue(sexu, queId, timeout))
        AfxThrowError();

    return err;
}

_ACX afxError AcxWaitForStepBridges(afxStepSystem ssys, afxTime timeout, afxMask exuMask)
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    afxUnit exuCnt = ssys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(AFX_TEST_BIT_SET(exuMask, exuIdx)))
            continue;

        afxStepBridge sexu;
        if (!AcxGetStepBridges(ssys, exuIdx, 1, &sexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
        AcxWaitForIdleStepBridge(sexu, timeout);
    }
    return err;
}

_ACX afxError AcxWaitForStepSystem(afxStepSystem ssys, afxTime timeout)
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    if (!ssys->ddi->waitCb)
    {
        AcxWaitForStepBridges(ssys, timeout, AFX_MASK_IGNORED);
    }
    else if (ssys->ddi->waitCb(ssys, timeout))
        AfxThrowError();

    return err;
}

_ACX afxError _AcxSsysDtorCb(afxStepSystem ssys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    afxModule icd = AcxGetStepSystemIcd(ssys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    AcxWaitForStepSystem(ssys, AFX_TIMEOUT_INFINITE);
    AcxWaitForStepSystem(ssys, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = ssys->bridgeCnt;

    AfxDeregisterChainedClasses(&ssys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));

    for (afxUnit i = bridgeCnt; i-- > 0;)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &ssys->bridges[i]);
        AfxDisposeObjects(1, &ssys->bridges[i]);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(ssys->bridges[0]),
            .var = (void**)&ssys->bridges
        }
    };

    if (AfxDeallocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    if (ssys->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &ssys->dsys);
        AfxDisposeObjects(1, &ssys->dsys);
    }

    return err;
}

_ACX afxError _AcxSsysCtorCb(afxStepSystem ssys, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    __acxSsysAcq const* cfg = AFX_CAST(__acxSsysAcq const*, args[1]) + invokeNo;
    _acxSexuAcq* bridgeCfgs = AFX_CAST(_acxSexuAcq*, args[2]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    ssys->ddi = &_ACX_SSYS_IMPL;
    ssys->idd = NIL;
    ssys->udd = cfg->udd;
    ssys->tag = cfg->tag;

    ssys->running = FALSE;

    ssys->requirements = cfg->reqFeatures;

    {
        afxChain* classes = &ssys->ctx.classes;
        AfxMakeChain(classes, (void*)ssys);

        afxClassConfig clsCfg;

        clsCfg = cfg->sbufClsCfg ? *cfg->sbufClsCfg : _ACX_SBUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SBUF);
        AfxMountClass(&ssys->sbufCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sctxClsCfg ? *cfg->sctxClsCfg : _ACX_SCTX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SCTX);
        AfxMountClass(&ssys->sctxCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sexuClsCfg ? *cfg->sexuClsCfg : _ACX_SEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SEXU);
        AfxMountClass(&ssys->sexuCls, NIL, classes, &clsCfg);
    }

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    ssys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(ssys->bridges[0]),
            .var = (void**)&ssys->bridges
        }
    };

    if (AfxAllocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&ssys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));
        return err;
    }

    if ((ssys->dsys = cfg->dsys))
    {
        ssys->dsys = cfg->dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &ssys->dsys);
        AfxReacquireObjects(1, &ssys->dsys);
    }

    if (_AcxSsysBridgeDevices(ssys, ssys->bridgeCnt, bridgeCfgs, ssys->bridges))
    {
        AfxThrowError();

        AfxDisposeObjects(1, &ssys->dsys);

        AfxDeallocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&ssys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, ssys->bridgeCnt, ssys->bridges);

    ssys->ioExuMask = NIL;
    ssys->dedIoExuMask = NIL;
    ssys->cfxExuMask = NIL;
    ssys->dedCfxExuMask = NIL;

    for (afxUnit i = 0; i < ssys->bridgeCnt; i++)
    {
        afxStepBridge sexu;
        AcxGetStepBridges(ssys, i, 1, &sexu);
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

        afxStepDevice sdev = AcxGetBridgedStepDevice(sexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

        acxDeviceInfo capsi;
        AcxQueryStepCapabilities(sdev, &capsi);

        if ((capsi.capabilities & acxAptitude_DMA) == acxAptitude_DMA)
            ssys->ioExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (acxAptitude_DMA | acxAptitude_PCX)) == acxAptitude_DMA)
            ssys->dedIoExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & acxAptitude_PCX) == acxAptitude_PCX)
            ssys->cfxExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (acxAptitude_PCX)) == acxAptitude_PCX)
            ssys->dedCfxExuMask |= AFX_BITMASK(i);
    }

    afxStepDevice sdev = AcxGetBridgedStepDevice(ssys->bridges[0], NIL);

    AfxCallDevice((afxDevice)sdev, 3, ssys);
    AfxCallDevice((afxDevice)sdev, 5, ssys);

    if (AfxDoDeviceService((afxDevice)sdev)) AfxThrowError(); // let the device build its SPUs.
    else
    {
        ssys->running = TRUE;
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&ssys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&ssys->ctx.classes));

        AfxDeallocateInstanceData(ssys, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ACX afxClassConfig const _ACX_SSYS_CLASS_CONFIG =
{
    .fcc = afxFcc_SSYS,
    .name = "StepSystem",
    .desc = "Step I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepSystem)),
    .ctor = (void*)_AcxSsysCtorCb,
    .dtor = (void*)_AcxSsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ACX afxUnit AcxEnumerateStepSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxStepSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));

    afxClass const* cls = _AcxIcdGetSsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);

    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, rslt, systems);

    return rslt;
}

_ACX afxUnit AcxEvokeStepSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxStepSystem), afxUnit cnt, afxStepSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));
    
    afxClass const* cls = _AcxIcdGetSsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
    
    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, rslt, systems);

    return rslt;
}

_ACX afxUnit AcxInvokeStepSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxStepSystem), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));
    
    afxClass const* cls = _AcxIcdGetSsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);

    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_ACX afxError AcxConfigureStepSystem(afxUnit icd, acxSystemConfig* cfg)
{
    afxError err = { 0 };

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    acxAptitude caps = cfg->caps;
    afxAcceleration accel = cfg->accel;

    if (icd != AFX_INVALID_INDEX)
    {
        afxModule drv;
        AFX_ASSERT(icd != AFX_INVALID_INDEX);
        if (!_AcxGetIcd(icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_ACX));
    }

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AVX_MAX_BRIDGES_PER_SYSTEM; i++)
        {
            afxUnit sdevId = i;

            afxStepDevice sdev;
            if (AcxEnumerateStepDevices(icd, sdevId, 1, &sdev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

                acxDeviceInfo capsi;
                AcxQueryStepCapabilities(sdev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].sdevId = sdevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
        }
    }
    else
    {
        afxUnit exuCnt = cfg->exuCnt;
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < exuCnt; i++)
        {
            acxDeviceInfo capsi = { 0 };
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;

            afxUnit sdevId;
            if (AcxChooseStepDevices(icd, &capsi, NIL, NIL, 1, &sdevId))
            {
                afxStepDevice sdev;
                AcxEnumerateStepDevices(icd, sdevId, 1, &sdev);
                AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

                AcxQueryStepCapabilities(sdev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].sdevId = sdevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
#if 0
            else
            {
                cfg->exus[i].capabilities = caps.capabilities;
                cfg->exus[i].acceleration = caps.acceleration;
                cfg->exus[i].minQueCnt = caps.minQueCnt;
                cfg->exus[i].ddevId = NIL;
            }
#endif
        }
    }
    return err;
}

_ACX afxError AcxEstablishStepSystem(afxUnit icd, acxSystemConfig const* cfg, afxStepSystem* system)
{
    afxError err = { 0 };
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(system);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT(cfg->exuCnt);

        if (!cfg->exuCnt)
        {
            AfxThrowError();
            return err;
        }
    }

    afxModule drv = NIL;
    afxClass* ssysCls = NIL;

    if (icd != AFX_INVALID_INDEX)
    {
        if (!_AcxGetIcd(icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_ACX));

        ssysCls = (afxClass*)_AcxIcdGetSsysClass(drv);
        AFX_ASSERT_CLASS(ssysCls, afxFcc_SSYS);
    }
    else
    {
        static afxBool clsInited = FALSE;
        static afxClass staticSsysCls = { 0 };

        if (!clsInited)
        {
            AfxMountClass(&staticSsysCls, NIL, NIL, &_ACX_SSYS_CLASS_CONFIG);
            clsInited = TRUE;
        }

        ssysCls = &staticSsysCls;
        AFX_ASSERT_CLASS(ssysCls, afxFcc_SSYS);
    }

    // Acquire bridges and queues
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[ACX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    _acxSexuAcq bridgeCfg[ACX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(ACX_MAX_BRIDGES_PER_SYSTEM, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        acxBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable to allowed multibridge to same port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalSqueCnt;
                totalSqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxStepDevice sdev;
        if (!AcxEnumerateStepDevices(icd, exuCfg->sdevId, 1, &sdev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

            if (AfxCallDevice((afxDevice)sdev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].sdev = sdev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, ACX_MAX_QUEUES_PER_BRIDGE);;
        acxDeviceInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].squeClsCfg = &_ACX_SQUE_CLASS_CONFIG;
        bridgeCfg[bridgeCnt].sctxClsCfg = &_ACX_SCTX_CLASS_CONFIG;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    __acxSsysAcq cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.reqExtCnt = cfg->extCnt;
    cfg2.reqExts = cfg->exts;
    cfg2.reqFeatures = cfg->features;
    cfg2.udd = cfg->udd;
    cfg2.tag = cfg->tag;
    cfg2.dsys = cfg->dsys;

    AFX_ASSERT_CLASS(ssysCls, afxFcc_SSYS);

    afxStepSystem ssys;
    if (AfxAcquireObjects(ssysCls, 1, (afxObject*)&ssys, (void const*[]) { drv, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    *system = ssys;
    return err;
}
