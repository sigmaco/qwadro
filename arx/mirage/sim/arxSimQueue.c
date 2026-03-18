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
#define _AFX_DEVICE_C
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#include "../sim/asxImplementation.h"

_ASX afxError _AsxUnlockSimQueue(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxUnlockMutex(&sque->workChnMtx);
    return err;
}

_ASX afxError _AsxLockSimQueue(arxSimQueue sque, afxBool wait, afxTimeSpec const* ts)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (wait)
    {
        if (!AfxTryLockMutex(&sque->workChnMtx))
            return 1;
    }
    else
    {
        if (ts)
            err = AfxLockMutexTimed(&sque->workChnMtx, ts);
        else
            if (AfxLockMutex(&sque->workChnMtx))
                AfxThrowError();
    }
    return err;
}

_ASX afxError _AsxSquePopBlob(arxSimQueue sque, void* blob, afxUnit siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxReclaimArena(&sque->workArena, blob, siz);
    return err;
}

_ASX void* _AsxSquePushBlob(arxSimQueue sque, afxUnit siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    void* blob = AfxRequestArena(&sque->workArena, siz, 1, NIL, 0);
    AFX_ASSERT(blob);
    return blob;
}

_ASX afxError _AsxSquePopWork(arxSimQueue sque, asxWork* work)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxPopLink(&work->hdr.chain);
    AfxReclaimArena(&sque->workArena, work, work->hdr.siz);
    return err;
}

_ASX asxWork* _AsxSquePushWork(arxSimQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    asxWork* work = AfxRequestArena(&sque->workArena, siz, 1, NIL, 0);
    AFX_ASSERT(work);
    work->hdr.id = id;
    work->hdr.siz = siz;
    AfxGetTime(&work->hdr.pushTime);
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&work->hdr.chain, &sque->workChn);
    return work;
}

////////////////////////////////////////////////////////////////////////////////

_ASX afxClass const* _AsxGetCatalystClass(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxClass const* cls = &sque->cmdbCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CTX);
    return cls;
}

_ASX arxEngine AfxGetSampleQueueDevice(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    arxSimBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    arxEngine sdev = ArxGetMathBridgeDevice(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ASX arxSimulation AfxGetSampleQueueContext(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    arxSimulation sim = sque->sim;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    return sim;
}

_ASX afxUnit AfxGetSampleQueuePort(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    return sque->portId;
}

_ASX afxError _AsxExecuteSampleCommands(arxSimQueue sque, asxSubmission const* ctrl, afxUnit cnt, arxContext contexts[])
{
    afxError err = { 0 };
    // sque must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(contexts);

    if (!AfxTryLockMutex(&sque->workChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    asxWork* work = _AsxSquePushWork(sque, ASX_GET_STD_WORK_ID(Execute), sizeof(work->Execute) + (cnt * sizeof(work->Execute.cmdbs[0])), &cmdId);
    AFX_ASSERT(work);

    if (ctrl)
    {
        work->Execute.fence = ctrl->fence;
        work->Execute.signal = ctrl->signalSems;
        work->Execute.wait = ctrl->waitSems;
    }

    work->Execute.cmdbCnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxContext sctx = contexts[i];
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &sctx);

        AfxReacquireObjects(1, &sctx);
        work->Execute.cmdbs[i] = sctx;
        AfxIncAtom32(&sctx->submCnt);
        work->Execute.cmdbs[i]->state = asxContextState_PENDING;
    }

    AfxUnlockMutex(&sque->workChnMtx);

    return err;
}

_ASX afxError _AsxSqueDtorCb(arxSimQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    AfxDeregisterChainedClasses(&sque->classes);

    AfxDismountClass(&sque->cmdbCls);
    AfxCleanUpFutex(&sque->cmdbReqLock);
    AfxDismantleQueue(&sque->cmdbRecycQue);

    AfxDismantleMutex(&sque->workChnMtx);
    AfxDismantleArena(&sque->workArena);
    AfxCleanUpFutex(&sque->workArenaSlock);
    AfxDismantleCondition(&sque->idleCnd);
    //AfxDismantleMutex(&sque->idleCndMtx);

    return err;
}

_ASX afxError _AsxSqueCtorCb(arxSimQueue sque, void** args, afxUnit invokeNo)
{
    (void)invokeNo;
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    arxSimBridge sexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _acxSimBridgeAcquisition const* cfg = AFX_CAST(_acxSimBridgeAcquisition const*, args[1]);
    AFX_ASSERT(cfg);

    sque->seng = cfg->seng;
    sque->portId = cfg->portId;
    sque->exuIdx = cfg->exuIdx;
    sque->sim = ArxGetMathBridgeContext(sexu);

    sque->immediate = 0;// !!spec->immedate;

    AfxSetUpFutex(&sque->workArenaSlock);
    AfxMakeArena(&sque->workArena, NIL, AfxHere());

    AfxDeployMutex(&sque->workChnMtx, AFX_MTX_PLAIN);
    AfxMakeChain(&sque->workChn, sexu);
    //AfxDeployMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sque->idleCnd);

    sque->closed = FALSE;

    AfxMakeChain(&sque->classes, (void*)sque);

    sque->cmdbLockedForReq = FALSE;
    AfxSetUpFutex(&sque->cmdbReqLock);
    AfxMakeQueue(&sque->cmdbRecycQue, sizeof(afxDrawContext), 3, NIL, 0);

    afxClassConfig tmpClsCfg = { 0 };
    tmpClsCfg = cfg->mctxClsCfg ? *cfg->mctxClsCfg : _ASX_CTX_CLASS_CONFIG;
    AFX_ASSERT(tmpClsCfg.fcc == afxFcc_CTX);
    AfxMountClass(&sque->cmdbCls, NIL, &sque->classes, &tmpClsCfg);

    return err;
}

_ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_SQUE,
    .name = "MathQueue",
    .desc = "Math Device Submission Queue",
    .fixedSiz = sizeof(AFX_OBJECT(arxSimQueue)),
    .ctor = (void*)_AsxSqueCtorCb,
    .dtor = (void*)_AsxSqueDtorCb
};
