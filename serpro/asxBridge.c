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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _ASX_STEP_C
#define _ASX_STEP_BRIDGE_C
#define _ASX_STEP_QUEUE_C
#define _ASX_STEP_CONTEXT_C
#include "asxIcd.h"

_ASX afxClass const* _AsxSexuGetSqueClass(afxStepBridge sexu)
{
    afxError err = { 0 };
    // sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxClass const* cls = &sexu->squeCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SQUE);
    return cls;
}

_ASX afxStepSystem AsxGetBridgedStepSystem(afxStepBridge sexu, afxUnit* bridgeId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxStepSystem ssys = AfxGetHost(sexu);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    if (bridgeId) *bridgeId = AfxGetObjectId(sexu);
    return ssys;
}

_ASX afxStepDevice AsxGetBridgedStepDevice(afxStepBridge sexu, afxUnit* sdevId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxStepDevice sdev = sexu->sdev;
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    if (sdevId) *sdevId = AfxGetObjectId(sdev);
    return sdev;
}

_ASX afxUnit AsxGetStepQueues(afxStepBridge sexu, afxUnit baseQueIdx, afxUnit cnt, afxStepQueue queues[])
{
    afxError err = { 0 };
    // sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    // queues must be a valid pointer to the afxStepQueue handles.
    AFX_ASSERT(queues);

    afxClass const* squeCls = _AsxSexuGetSqueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(squeCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, rslt, queues);
    return rslt;
}

_ASX afxError AsxWaitForEmptyStepQueue(afxStepBridge sexu, afxUnit queIdx, afxUnit64 timeout)
{
    afxError err = { 0 };
    // sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxStepQueue sque;
    if (!AsxGetStepQueues(sexu, queIdx, 1, &sque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (!sque->waitCb)
    {
        AfxLockMutex(&sque->idleCndMtx);

        afxTimeSpec ts = { 0 };
        ts.nsecs = AFX_MAX(1, timeout) * 100000; // 100000 = 0.0001 sec

        while (sque->iorpChn.cnt)
        {
            sexu->pingCb(sexu, 0);
            AfxWaitTimedCondition(&sque->idleCnd, &sque->idleCndMtx, &ts);
        }
        AfxUnlockMutex(&sque->idleCndMtx);
    }
    else if (sque->waitCb(sque, timeout))
        AfxThrowError();

    return err;
}

_ASX afxError AsxWaitForIdleStepBridge(afxStepBridge sexu, afxUnit64 timeout)
{
    afxError err = { 0 };
    // sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    if (sexu->pingCb)
        sexu->pingCb(sexu, 0);

    afxClass const* squeCls = _AsxSexuGetSqueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit i = 0; i < queCnt; i++)
        AsxWaitForEmptyStepQueue(sexu, i, timeout);

    return err;
}

_ASX afxError _AsxSexuTransferStepMemory(afxStepBridge sexu, asxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxBool queued = FALSE;
    afxClass const* squeCls = _AsxSexuGetSqueClass(sexu);

    // sanitize arguments
    afxUnit totalQueCnt = squeCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxStepQueue sque;
            if (!AsxGetStepQueues(sexu, queIdx, 1, &sque))
            {
                AfxThrowError();
                continue; // for
            }

            afxError err2 = _AsxSqueTransferResources(sque, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        if (err || queued) break; // while --- reiterate if not queue for timeout?
    }
    return err;
}

_ASX afxError _AsxSexuRemapBuffers(afxStepBridge sexu, afxBool unmap, afxUnit cnt, _asxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxStepQueue sque;
    afxUnit queIdx = 0;
    while (AsxGetStepQueues(sexu, queIdx++, 1, &sque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (unmap)
        {
            afxError err2 = _AsxSqueRemapBuffers(sque, 0, NIL, cnt, maps);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If synchronization fails, throw an error.
            AfxThrowError();
        }
        else
        {
            afxError err2 = _AsxSqueRemapBuffers(sque, cnt, maps, 0, NIL);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If flushing fails, throw an error
            AfxThrowError();
        }
    }

    if (queued)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
        // Wait for the draw queue to finish the operation, otherwise we will have not pointers.
        //if (AsxWaitForEmptyStepQueue(sque, AFX_TIMEOUT_INFINITE))
        if (AsxWaitForEmptyStepQueue(sexu, queIdx - 1, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_ASX afxError _AsxSexuCohereMappedBuffers(afxStepBridge sexu, afxBool discard, afxUnit cnt, asxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @sexu must be a valid afxStepBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxStepQueue sque;
    afxUnit queIdx = 0;
    while (AsxGetStepQueues(sexu, queIdx++, 1, &sque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (discard)
        {
            afxError err2 = _AsxSqueCohereMappedBuffers(sque, 0, NIL, cnt, maps);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If synchronization fails, throw an error.
            AfxThrowError();
        }
        else
        {
            afxError err2 = _AsxSqueCohereMappedBuffers(sque, cnt, maps, 0, NIL);
            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // while --- iterate queues
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // while

            // If flushing fails, throw an error
            AfxThrowError();
        }
    }
    return err;
}

_ASX afxError _AsxSexuDtorCb(afxStepBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxStepSystem ssys = AsxGetBridgedStepSystem(sexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);

    AsxWaitForStepSystem(ssys, AFX_TIMEOUT_INFINITE);
    AsxWaitForStepSystem(ssys, AFX_TIMEOUT_INFINITE); // yes, two times.
    
    AfxDeregisterChainedClasses(&sexu->classes);

    if (sexu->worker)
    {
        while (AfxIsThreadRunning(sexu->worker))
        {
            AfxRequestThreadInterruption(sexu->worker);
        }
        afxInt exitCode;
        AfxWaitForThreadExit(sexu->worker, &exitCode);
        AfxDisposeObjects(1, &sexu->worker);
    }

    AfxDismantleCondition(&sexu->schedCnd);
    AfxDismantleMutex(&sexu->schedCndMtx);

    return err;
}

_ASX afxError _AsxSexuCtorCb(afxStepBridge sexu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxStepSystem ssys = AFX_CAST(afxStepSystem, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    _asxSexuAcq const* cfg = AFX_CAST(_asxSexuAcq const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    sexu->sdev = cfg->sdev;
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    //sexu->portId = cfg->portId;
    sexu->exuIdx = cfg->exuIdx;

    sexu->procCb = _AsxSpu_ProcCb;
    sexu->workerProc = _ASX_SPU_THREAD_PROC;
    sexu->iorpVmt = &_ASX_SPU_IORP_VMT;
    sexu->pingCb = _AsxSexu_PingCb;

    AfxDeployMutex(&sexu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sexu->schedCnd);
    sexu->schedCnt = 0;

    AfxMakeChain(&sexu->classes, sexu);

    afxClassConfig squeClsCfg = cfg->squeClsCfg ? *cfg->squeClsCfg : _ASX_SQUE_CLASS_CONFIG;
    AFX_ASSERT(squeClsCfg.fcc == afxFcc_SQUE);
    AfxMountClass(&sexu->squeCls, NIL, &sexu->classes, &squeClsCfg);

    afxClassConfig SctxClsCfg = cfg->sctxClsCfg ? *cfg->sctxClsCfg : _ASX_SCTX_CLASS_CONFIG;
    AFX_ASSERT(SctxClsCfg.fcc == afxFcc_SCTX);
    //AfxMountClass(&sexu->sctxCls, NIL, &sexu->classes, &sctxClsCfg);

    afxClass* squeCls = (afxClass*)_AsxSexuGetSqueClass(sexu);
    AFX_ASSERT_CLASS(squeCls, afxFcc_SQUE);
    afxDrawQueue queues[ASX_MAX_QUEUES_PER_BRIDGE];
    AFX_ASSERT(ASX_MAX_QUEUES_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(squeCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { sexu, cfg }))
    {
        AfxThrowError();
    }
    
    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = SoundThreadProc;
        thrCfg.usage = afxThreadUsage_COMPUTE;
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

_ASX afxClassConfig const _ASX_SEXU_CLASS_CONFIG =
{
    .fcc = afxFcc_SEXU,
    .name = "StepBridge",
    .desc = "Step Device Execution Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepBridge)),
    .ctor = (void*)_AsxSexuCtorCb,
    .dtor = (void*)_AsxSexuDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError _AsxSsysBridgeDevices(afxStepSystem ssys, afxUnit cnt, _asxSexuAcq const configs[], afxStepBridge bridges[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(configs);
    AFX_ASSERT(bridges);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxSsysGetSexuClass(ssys);
    AFX_ASSERT_CLASS(cls, afxFcc_SEXU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bridges, (void const*[]) { ssys, configs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SEXU, cnt, bridges);
    }

    return err;
}
