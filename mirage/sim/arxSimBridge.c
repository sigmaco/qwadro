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

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#include "../sim/asxImplementation.h"

_ARX afxClass const* _AsxGetSimQueueClass(arxSimBridge sexu)
{
    afxError err = { 0 };
    // sexu must be a valid arxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxClass const* cls = &sexu->squeCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SQUE);
    return cls;
}

_ARX arxEngine ArxGetMathBridgeDevice(arxSimBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    arxEngine seng = sexu->seng;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
    return seng;
}

_ARX arxSimulation ArxGetMathBridgeContext(arxSimBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    arxSimulation sim = AfxGetHost(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ARX afxUnit ArxQuerySimBridgePort(arxSimBridge sexu, arxEngine* engine)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (engine)
    {
        arxEngine seng = sexu->seng;
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &seng);
        *engine = seng;
    }
    return sexu->portId;
}

_ARX afxUnit ArxGetSimQueues(arxSimBridge sexu, afxUnit baseQueIdx, afxUnit cnt, arxSimQueue queues[])
{
    afxError err = { 0 };
    // sexu must be a valid arxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    // queues must be a valid pointer to the arxSimQueue handles.
    AFX_ASSERT(queues);

    afxClass const* squeCls = _AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(squeCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, rslt, queues);
    return rslt;
}

_ARX afxError AfxWaitForEmptySimQueue(arxSimBridge sexu, afxUnit queIdx, afxTime timeout)
{
    afxError err = { 0 };
    // sexu must be a valid arxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    arxSimQueue sque;
    if (!ArxGetSimQueues(sexu, queIdx, 1, &sque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (!sque->waitCb)
    {
        AfxLockMutex(&sque->workChnMtx);

        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        sexu->pingCb(sexu, 0);

        while (!AfxIsChainEmpty(&sque->workChn))
        {
            afxError cndErr = AfxWaitTimedCondition(&sque->idleCnd, &sque->workChnMtx, &ts);

            if (cndErr)
            {
                if (cndErr == afxError_TIMEOUT)
                    err = afxError_TIMEOUT;
                else AfxThrowError();

                break;
            }
        }

        if (!err)
        {
            AFX_ASSERT(AfxIsChainEmpty(&sque->workChn));
        }

        AfxUnlockMutex(&sque->workChnMtx);
    }
    else if (sque->waitCb(sque, timeout))
        AfxThrowError();

    return err;
}

_ARX afxError ArxWaitForIdleSimBridge(arxSimBridge sexu, afxTime timeout)
{
    afxError err = { 0 };
    // sexu must be a valid arxSimBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (sexu->pingCb)
        sexu->pingCb(sexu, 0);

    afxClass const* squeCls = _AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AfxWaitForEmptySimQueue(sexu, i, timeout);

    return err;
}

_ARX afxError _AsxSexuDtorCb(arxSimBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    arxSimulation sim = ArxGetMathBridgeContext(sexu);

    ArxWaitForSimulation(sim, 0);
    ArxWaitForSimulation(sim, 0); // yes, two times.

    AfxDeregisterChainedClasses(&sexu->classes);

    if (sexu->worker)
    {
        afxInt exitCode;
        AfxWaitForThreadExit(sexu->worker, &exitCode);
        AfxDisposeObjects(1, &sexu->worker);
    }

    AfxDismantleCondition(&sexu->schedCnd);
    AfxDismantleMutex(&sexu->schedCndMtx);

    return err;
}

_ARX afxError _AsxSexuCtorCb(arxSimBridge sexu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    arxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    _acxSimBridgeAcquisition const* cfg = AFX_CAST(_acxSimBridgeAcquisition const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    sexu->seng = cfg->seng;
    AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    sexu->portId = cfg->portId;
    sexu->exuIdx = cfg->exuIdx;

    sexu->pingCb = _AsxSexu_PingCb;
    sexu->workerProc = _ASX_SPU_THREAD_PROC;
    sexu->procCb = _AsxSpu_ProcCb;

    sexu->workVmt = &_ASX_SPU_WORK_VMT;
    //sexu->cmdVmt = &_ASX_MPU_CMD_VMT;

    AfxDeployMutex(&sexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sexu->schedCnd);
    sexu->schedCnt = 0;

    AfxMakeChain(&sexu->classes, sexu);
    afxClassConfig clsCfg = cfg->squeClsCfg ? *cfg->squeClsCfg : _ASX_SQUE_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SQUE);
    AfxMountClass(&sexu->squeCls, NIL, &sexu->classes, &clsCfg);

    afxClass* squeCls = (afxClass*)_AsxGetSimQueueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    arxSimQueue queues[ASX_MAX_SIM_QUEUE_PER_BRIDGE];
    AFX_ASSERT(ASX_MAX_SIM_QUEUE_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(squeCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { sexu, cfg }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = DrawThreadProc;
        thrCfg.usage = afxThreadUsage_SIM;
        thrCfg.udd[0] = sexu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &sexu->worker))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        AfxDeregisterChainedClasses(&sexu->classes);
        AfxDismantleCondition(&sexu->schedCnd);
        AfxDismantleMutex(&sexu->schedCndMtx);
    }
    return err;
}

_ARX afxClassConfig const _ASX_SEXU_CLASS_CONFIG =
{
    .fcc = afxFcc_SEXU,
    .name = "MathBrige",
    .desc = "Math Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(arxSimBridge)),
    .ctor = (void*)_AsxSexuCtorCb,
    .dtor = (void*)_AsxSexuDtorCb
};
