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

#define _ASX_SIM_C
#define _ASX_SIMULATION_C
#include "../sim/asxImplementation.h"

extern afxClassConfig const _ASX_SHAP_CLASS_CONFIG;

//extern afxChain* _AfxGetSystemClassChain(void);

// SIMULATION API //////////////////////////////////////////////////////////////

_ASX afxModule ArxGetSimulationIcd(arxSimulation sim)
{
    afxError err = { 0 };
    // sim must be a valid arxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxModule icd = AfxGetHost(sim);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX));
    return icd;
}

_ASX afxDrawSystem ArxGetSimulationDrawSystem(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->dsys;
}

_ASX arxScenario ArxGetSimulationDrawInput(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim->scio;
}

_ASX afxClass const* _AsxGetMathBridgeClass(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->sexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);
    return cls;
}

_ASX afxClass const* _AsxGetShapClass(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = &sim->shapCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SHAP);
    return cls;
}

_ASX afxBool _AsxStepBodCb(arxPuppet bod, void** udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &bod);

    afxReal dt = *(afxReal*)udd[0];
    //ArxDoPuppetDynamics(bod, dt);

    return TRUE;
}

_ASX afxError ArxStepSimulation(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    //afxResult cnt = 0;

    afxSize stepTime = AfxGetTimer();
    afxReal deltaTime = sim->stepDeltaTime = AFX_REAL(stepTime - sim->stepTime) * 0.001f; // multiplying by 0.001f turns deltaTime in seconds
    sim->stepTime = stepTime;

    //AfxStepDag(sim->dag, AFX_REAL(deltaTime));
    //AfxNodeUpdate(AfxConnectionGetObject(&sim->dagRoot));

    if (1000 <= (stepTime - sim->swapTime))
    {
        sim->swapTime = stepTime;
        sim->stepsPerSecond = sim->stepNum;
        sim->stepNum = 0;
    }

    sim->stepCb(sim, 0);

    //ArxInvokePuppets(sim, 0, AFX_U32_MAX, (void*)_AsxStepBodCb, (void*[]) { &deltaTime });

    ++sim->stepNum;

    return err;
}

_ASX afxUnit ArxGetSimBridges(arxSimulation sim, afxUnit baseExuIdx, afxUnit cnt, arxSimBridge bridges[])
{
    afxError err = { 0 };
    // sim must be a valid arxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    // bridges must be a valid pointer to arxSimBridge handles.
    AFX_ASSERT(bridges);
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);

    if (baseExuIdx < bridgeCnt)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            arxSimBridge sexu = sim->bridges[baseExuIdx + i];
            AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
            bridges[rslt++] = sexu;
        }
    }
    return rslt;
}

_ASX afxUnit ArxQuerySimBridges(arxSimulation sim, afxUnit sengId, afxUnit portId, afxUnit first, afxUnit cnt, arxSimBridge bridges[])
{
    afxError err = { 0 };
    // sim must be a valid arxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxUnit found = 0;
    afxUnit rslt = 0;

    afxUnit bridgeCnt = sim->bridgeCnt;
    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        arxSimBridge sexu = sim->bridges[i];
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
        arxEngine seng;
        afxUnit portId2 = ArxQuerySimBridgePort(sexu, &seng);
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

        if ((sengId != AFX_INVALID_INDEX) && (sengId != AfxGetObjectId(seng)))
            continue;

        if ((portId != AFX_INVALID_INDEX) && (portId != portId2))
            continue;

        if (cnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = sexu;
            }

            if (cnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_ASX afxError ArxWaitForSimBridge(arxSimulation sim, afxUnit exuIdx, afxTime timeout)
{
    afxError err = { 0 };
    // sim must be a valid arxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    // portIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE((afxUnit)sim->bridgeCnt, exuIdx, 1);

    arxSimBridge sexu;
    while (!ArxGetSimBridges(sim, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    ArxWaitForIdleSimBridge(sexu, timeout);

    return err;
}

_ASX afxError ArxWaitForSimulation(arxSimulation sim, afxTime timeout)
{
    afxError err = { 0 };
    // sim must be a valid arxSimulation handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (!sim->waitCb)
    {
        ArxStepSimulation(sim);

        afxUnit bridgeCnt = sim->bridgeCnt;

        for (afxUnit i = 0; i < bridgeCnt; i++)
            ArxWaitForSimBridge(sim, i, timeout);
    }
    else if (sim->waitCb(sim, timeout))
        AfxThrowError();

    return err;
}

_ASX afxError ArxExecuteSampleCommands(arxSimulation sim, asxSubmission* ctrl, afxUnit cnt, arxContext contexts[])
{
    afxError err = { 0 };
    // sim must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(contexts);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(cnt);

    arxSimBridge sexu;
    if (!ArxGetSimBridges(sim, ctrl->portIdx, 1, &sexu))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* dqueCls = _AsxGetSimQueueClass(sexu);

    // sanitize arguments
    afxUnit totalQueCnt = dqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);
    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            arxSimQueue sque;
            if (!ArxGetSimQueues(sexu, queIdx, 1, &sque))
            {
                AfxThrowError();
                break;
            }

            afxError err2 = _AsxExecuteSampleCommands(sque, ctrl, 1, contexts);

            if (!err2)
            {
                queued = TRUE;
                break;
            }

            if (err2 == afxError_TIMEOUT)
                continue;

            AfxThrowError();
            break;
        }

        if (err || queued)
            break; // reiterate if not queue for timeout?
    }
    return err;
}

_ASX afxError _AsxSimDtorCb(arxSimulation sim)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    AfxDeregisterChainedClasses(&sim->classes);

    return err;
}

_ASX afxError _AsxSimCtorCb(arxSimulation sim, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _acxSimulationAcquisition const* cfg = AFX_CAST(_acxSimulationAcquisition const*, args[1]) + invokeNo;
    _acxSimBridgeAcquisition* bridgeCfgs = AFX_CAST(_acxSimBridgeAcquisition*, args[2]) + invokeNo;

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

    sim->running = FALSE;

    sim->requirements = cfg->requirements;

    // UNIVERSE

    // OBJECTS

    {
        afxChain* classes = &sim->classes;
        AfxMakeChain(classes, (void*)sim);

        afxClassConfig clsCfg;

        clsCfg = /*cfg->shapCls ? *cfg->shapCls :*/ _ASX_SHAP_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SHAP);
        AfxMountClass(&sim->shapCls, NIL, classes, &clsCfg);

        clsCfg = cfg->sexuClsCfg ? *cfg->sexuClsCfg : _ASX_SEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SEXU);
        AfxMountClass(&sim->sexuCls, NIL, classes, &clsCfg); // require all

    }

    sim->dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &sim->dsys);

    sim->scio = cfg->scio;
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &sim->scio);

    {
        sim->stepNum = 0;
        sim->stepsPerSecond = 0;
        sim->stepTime = 0;

    }


    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    sim->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(sim->bridges[0]),
            .var = (void**)&sim->bridges
        }
    };

    if (AfxAllocateInstanceData(sim, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&sim->classes);
        AFX_ASSERT(AfxIsChainEmpty(&sim->classes));
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetMathBridgeClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);

    if (AfxAcquireObjects(cls, sim->bridgeCnt, (afxObject*)sim->bridges, (void const*[]) { sim, bridgeCfgs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, sim->bridgeCnt, sim->bridges);

        arxEngine seng = ArxGetMathBridgeDevice(sim->bridges[0]);

        AfxCallDevice((afxDevice)seng, 3, sim);
        AfxCallDevice((afxDevice)seng, 5, sim);

        if (AfxDoDeviceService((afxDevice)seng)) AfxThrowError(); // let the device build its SPUs.
        else
        {
            sim->running = TRUE;
        }
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&sim->classes);
        AFX_ASSERT(AfxIsChainEmpty(&sim->classes));

        AfxDeallocateInstanceData(sim, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG =
{
    .fcc = afxFcc_SIM,
    .name = "Simulation",
    .desc = "Simulation",
    .fixedSiz = sizeof(AFX_OBJECT(arxSimulation)),
    .ctor = (void*)_AsxSimCtorCb,
    .dtor = (void*)_AsxSimDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError ArxConfigureSimulation(afxUnit icd, arxSimulationConfig* cfg)
{
    afxError err = { 0 };

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    //*cfg = (arxSimulationConfig const) { 0 };

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    arxEngine seng;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!ArxEnumerateEngines(icd, 0, 1, &seng))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    cfg->prime.minQueCnt = _ASX_SQUE_CLASS_CONFIG.unitsPerPage;

    return err;
}

_ASX afxError ArxEstablishSimulation(afxUnit icd, arxSimulationConfig const* cfg, arxSimulation* simulation)
{
    afxError err = { 0 };

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    arxEngine seng;
    AFX_ASSERT(cfg->prime.engId != AFX_INVALID_INDEX);
    if (!ArxEnumerateEngines(icd, cfg->prime.engId, 1, &seng))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);

    if (AfxCallDevice((afxDevice)seng, 1, NIL))
    {
        AfxThrowError(); // let the device build its DPUs.
        return err;
    }

    // Acquire bridges and queues
    afxUnit portId = AFX_INVALID_INDEX;
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[ASX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    _acxSimBridgeAcquisition bridgeCfg[ASX_MAX_SIM_BRIDGE_PER_CONTEXT] = { 0 };
    afxUnit bridgeCnt = 0;

    if (!ArxEnumerateEngines(icd, cfg->prime.engId, 1, &seng))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    bridgeCfg[bridgeCnt].seng = seng;
    if (!ArxChooseSimPorts(seng, cfg->prime.capabilities, cfg->prime.acceleration, 1, &portId))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT(portId != AFX_INVALID_INDEX);
    bridgeCfg[bridgeCnt].portId = portId;
    bridgeCfg[bridgeCnt].exuIdx = 0;
    bridgeCfg[bridgeCnt].minQueCnt = AFX_CLAMP(cfg->prime.minQueCnt, 1, ASX_MAX_SIM_QUEUE_PER_BRIDGE);
    ++bridgeCnt;

    if (cfg->auxCnt && cfg->auxs)
    {
        AFX_ASSERT(15 >= cfg->auxCnt);

        for (afxUnit i = 0; i < cfg->auxCnt; i++)
        {
            afxUnit bridgeIdx = AFX_INVALID_INDEX;
#if 0 // allowed multibridge to same port
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
            if (bridgeIdx == AFX_INVALID_INDEX)
            {
                if (!ArxEnumerateEngines(icd, cfg->auxs[i].engId, 1, &seng))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
                bridgeCfg[bridgeCnt].seng = seng;
                if (!ArxChooseSimPorts(seng, cfg->auxs[i].capabilities, cfg->auxs[i].acceleration, 1, &portId))
                {
                    AfxThrowError();
                    return err;
                }
                AFX_ASSERT(portId != AFX_INVALID_INDEX);
                bridgeCfg[bridgeCnt].portId = portId;
                bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
                bridgeCfg[bridgeCnt].minQueCnt = AFX_CLAMP(cfg->auxs[i].minQueCnt, 1, ASX_MAX_SIM_QUEUE_PER_BRIDGE);
                ++bridgeCnt;
            }
        }
    }

    _acxSimulationAcquisition cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.extensionCnt = cfg->extensionCnt;
    cfg2.extensions = cfg->extensions;
    cfg2.requirements = cfg->requirements;
    cfg2.udd = cfg->udd;

    cfg2.scio = cfg->scio;
    cfg2.dsys = cfg->dsys;

    afxClass* cls = (afxClass*)_AsxGetSimulationClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_SIM);

    arxSimulation sim;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&sim, (void const*[]) { driver, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(simulation);
    *simulation = sim;

    return err;
}
