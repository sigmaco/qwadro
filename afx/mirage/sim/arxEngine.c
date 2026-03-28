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

// This file is part of Advanced RenderWare Extensions.

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_SIM_C
#define _ASX_ENGINE_C
#define _ASX_SIMULATION_C
#include "../sim/asxImplementation.h"


////////////////////////////////////////////////////////////////////////////////
// MATH DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_ARX afxUnit ArxCountEnginePorts(arxEngine seng)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng->portCnt;
}

_ARX afxBool ArxIsEnginePrompt(arxEngine seng)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng->dev.serving;
}

_ARX afxUnit ArxChooseSimPorts(arxEngine seng, afxSimPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    afxUnit rslt = 0;

    for (afxUnit i = seng->portCnt; i-- > 0;)
    {
        afxSimPortCaps const* port = &seng->ports[i].caps;

        if (!caps || (caps == (caps & port->capabilities)))
        {
            if (!accel || (accel == (accel & port->acceleration)))
            {
                portId[rslt++] = i;
            }
        }

        if (rslt == maxCnt)
            break;
    }
    return rslt;
}

_ARX afxError _AsxMdevDtorCb(arxEngine seng)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    if (seng->stopCb && seng->stopCb(seng))
        AfxThrowError();

    AFX_ASSERT(!seng->idd);
    AfxDeregisterChainedClasses(&seng->dev.classes);

    AfxDismantleMutex(&seng->relinkedCndMtx);
    AfxDismantleCondition(&seng->relinkedCnd);

    afxObjectStash stashes[] =
    {
        {
            .cnt = seng->portCnt,
            .siz = sizeof(seng->ports[0]),
            .var = (void**)&seng->ports
        }
    };
    AfxDeallocateInstanceData(seng, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ARX afxError _AsxMdevCtorCb(arxEngine seng, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxEngineInfo const* info = ((afxEngineInfo const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&seng->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    AfxDeployCondition(&seng->relinkedCnd);
    AfxDeployMutex(&seng->relinkedCndMtx, AFX_MTX_PLAIN);

    seng->idd = NIL;

    seng->startCb = info->startCb;
    seng->stopCb = info->stopCb;
    seng->openSimCb = info->openSimCb;
    seng->closeSimCb = info->closeSimCb;

    seng->closeMdgeCb = info->closeMdgeCb;
    seng->openMdgeCb = info->openMdgeCb;

    seng->portCnt = info->portCnt;

    afxObjectStash stashes[] =
    {
        {
            .cnt = seng->portCnt,
            .siz = sizeof(seng->ports[0]),
            .var = (void**)&seng->ports
        }
    };

    if (AfxAllocateInstanceData(seng, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
    }
    else
    {
        if (!seng->portCnt) AfxThrowError();
        else
        {
            AFX_ASSERT(seng->ports);

            for (afxUnit i = 0; i < seng->portCnt; i++)
            {
                seng->ports[i].caps = info->portCaps[i];
            }

            if (AfxCallDevice(&seng->dev, afxFcc_MSYS, NIL)) AfxThrowError();
            else
            {
                if (err)
                {
                    AfxDeregisterChainedClasses(&seng->dev.classes);
                }
            }
        }

        if (err)
            AfxDeallocateInstanceData(seng, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ARX afxClassConfig const _ASX_SDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_SDEV,
    .name = "Engine",
    .desc = "Compute Engine Interface",
    .fixedSiz = sizeof(AFX_OBJECT(arxEngine)),
    .ctor = (void*)_AsxMdevCtorCb,
    .dtor = (void*)_AsxMdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_ARX afxUnit ArxInvokeEngines(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*,arxEngine), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);
        break;
    }
    return rslt;
}

_ARX afxUnit ArxEvokeEngines(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*,arxEngine), afxUnit cnt, arxEngine devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ARX afxUnit ArxEnumerateEngines(afxUnit icd, afxUnit first, afxUnit cnt, arxEngine devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetEngineClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_ARX afxUnit ArxInvokeSimulations(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, arxSimulation), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);
        break;
    }
    return rslt;
}

_ARX afxUnit ArxEvokeSimulations(afxUnit icd, afxUnit first, void* udd, afxBool(*flt)(void*, arxSimulation), afxUnit cnt, arxSimulation simulations[])
{
    afxError err = { 0 };
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEvokeObjects(cls, (void*)flt, udd, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}

_ARX afxUnit ArxEnumerateSimulations(afxUnit icd, afxUnit first, afxUnit cnt, arxSimulation simulations[])
{
    afxError err = { 0 };
    AFX_ASSERT(simulations);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AsxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ASX) == (afxModuleFlag_ICD | afxModuleFlag_ASX));
        afxClass const* cls = _AsxGetSimulationClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_SIM);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)simulations);
        break;
    }
    return rslt;
}
