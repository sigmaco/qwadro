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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA Future Storage.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
//#include "afxIoDDK.h"
//#include "../afxSystemDDK.h"
#include "afxIoBridgeDDK.h"


_AFX afxClass const* _AfxExuGetXqueClass(afxIoBridge exu)
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxClass const* cls = &exu->xqueCls;
    AFX_ASSERT_CLASS(cls, afxFcc_XQUE);
    return cls;
}

_AFX afxIommu AfxGetBridgedIoSystem(afxIoBridge exu, afxUnit* bridgeId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxIommu iom = AfxGetHost(exu);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    if (bridgeId) *bridgeId = AfxGetObjectId(exu);
    return iom;
}

_AFX afxDevice AfxGetBridgedIoDevice(afxIoBridge exu, afxUnit* devId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    afxDevice dev = exu->dev;
    AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);
    if (devId) *devId = AfxGetObjectId(dev);
    return dev;
}

_AFX afxUnit AfxGetIoQueues(afxIoBridge exu, afxUnit baseQueIdx, afxUnit cnt, afxIoQueue queues[])
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    // queues must be a valid pointer to the afxIoQueue handles.
    AFX_ASSERT(queues);

    afxClass const* xqueCls = _AfxExuGetXqueClass(exu);
    AFX_ASSERT_CLASS(xqueCls, afxFcc_XQUE);
    afxUnit rslt = _AfxEnumerateObjectsUnlocked(xqueCls, FALSE, baseQueIdx, cnt, (afxObject*)queues);
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, rslt, queues);
    return rslt;
}

_AFX afxError AfxWaitForIdleIoBridge(afxIoBridge exu, afxUnit64 timeout)
{
    afxError err = { 0 };
    // exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    if (exu->ddi->pingCb)
        exu->ddi->pingCb(exu, 0);

    if (!exu->ddi->waitCb)
    {
        afxClass const* xqueCls = _AfxExuGetXqueClass(exu);

        for (afxUnit i = 0; i < xqueCls->instCnt; i++)
        {
            afxIoQueue xque;
            AfxGetIoQueues(exu, i, 1, &xque);
            AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

            afxError err2;
            if ((err2 = AfxWaitForEmptyIoQueue(xque, timeout)) && (err2 != afxError_TIMEOUT))
            {
                AfxThrowError();
            }
        }
    }
    else if (exu->ddi->waitCb(exu, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxExuTransferMemory(afxIoBridge exu, afxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxBool queued = FALSE;
    afxClass const* xqueCls = _AfxExuGetXqueClass(exu);

    // sanitize arguments
    afxUnit totalQueCnt = xqueCls->instCnt;
    afxUnit baseQueIdx = AFX_MIN(ctrl->baseQueIdx, totalQueCnt - 1);
    afxUnit queCnt = AFX_CLAMP(ctrl->queCnt, 1, totalQueCnt - baseQueIdx);
    AFX_ASSERT(queCnt);

    while (1)
    {
        for (afxUnit i = 0; i < queCnt; i++)
        {
            afxUnit queIdx = baseQueIdx + i;

            afxIoQueue xque;
            if (!AfxGetIoQueues(exu, queIdx, 1, &xque))
            {
                AfxThrowError();
                continue; // for
            }

            afxError err2 = _AfxXqueTransferResources(xque, ctrl, opCnt, ops);

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

_AFX afxError _AfxExuRemapBuffers(afxIoBridge exu, afxBool unmap, afxUnit cnt, _afxBufRemapping const maps[])
{
    afxError err = { 0 };
    // @exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxIoQueue xque;
    afxUnit queIdx = 0;
    while (AfxGetIoQueues(exu, queIdx++, 1, &xque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

        if (unmap)
        {
            afxError err2 = _AfxXqueRemapBuffers(xque, 0, NIL, cnt, maps);
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
            afxError err2 = _AfxXqueRemapBuffers(xque, cnt, maps, 0, NIL);
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
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
        // Wait for the IO queue to finish the operation, otherwise we will have not pointers.
        if (AfxWaitForEmptyIoQueue(xque, AFX_TIMEOUT_INFINITE))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxExuCohereMappedBuffers(afxIoBridge exu, afxBool invalidate, afxUnit cnt, afxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @exu must be a valid afxIoBridge handle.
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxBool queued = FALSE;
    afxIoQueue xque;
    afxUnit queIdx = 0;
    while (AfxGetIoQueues(exu, queIdx++, 1, &xque))
    {
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

        if (invalidate)
        {
            afxError err2 = _AfxXqueCohereMappedBuffers(xque, 0, NIL, cnt, maps);
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
            afxError err2 = _AfxXqueCohereMappedBuffers(xque, cnt, maps, 0, NIL);
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

_AFX _afxExuDdi const _AFX_DDI_EXU =
{
    .pingCb = _AfxExu_PingCb
};

_AFX afxError _AfxExuDtorCb(afxIoBridge exu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxIommu iom = AfxGetBridgedIoSystem(exu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    //AfxWaitForIoSystem(iom, AFX_TIMEOUT_INFINITE);
    //AfxWaitForIoSystem(iom, AFX_TIMEOUT_INFINITE); // yes, two times.

    if (exu->worker)
    {
        while (AfxIsThreadRunning(exu->worker))
        {
            AfxRequestThreadInterruption(exu->worker);
            exu->ddi->pingCb(exu, 0);
        }
        afxInt exitCode;
        AfxWaitForThreadExit(exu->worker, &exitCode);
        AfxDisposeObjects(1, &exu->worker);
    }

    AfxDeregisterChainedClasses(&exu->classes);

    AfxDismantleCondition(&exu->schedCnd);
    AfxDismantleMutex(&exu->schedCndMtx);

    return err;
}

_AFX afxError _AfxExuCtorCb(afxIoBridge exu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

    afxIommu iom = AFX_CAST(afxIommu, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    _afxExuAcq const* cfg = AFX_CAST(_afxExuAcq const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    exu->dev = cfg->dev;
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    //exu->portId = cfg->portId;
    exu->exuIdx = cfg->exuIdx;
    AFX_ASSERT(exu->exuIdx == AfxGetObjectId(exu));

    exu->ddi = &_AFX_DDI_EXU;
    exu->workerProc = _AFX_XPU_THREAD_PROC;
    exu->procCb = _AfxXpu_ProcCb;

    exu->iorpVmt = &_AFX_XPU_IORP_VMT;
    exu->cmdVmt = &_AFX_XPU_CMD_VMT;

    AfxDeployMutex(&exu->schedCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&exu->schedCnd);
    exu->schedCnt = 0;

    AfxMakeChain(&exu->classes, exu);

    afxClassConfig xqueClsCfg = cfg->xqueClsCfg ? *cfg->xqueClsCfg : _AFX_CLASS_CONFIG_XQUE;
    AFX_ASSERT(xqueClsCfg.fcc == afxFcc_XQUE);
    AfxMountClass(&exu->xqueCls, NIL, &exu->classes, &xqueClsCfg);

    afxClass* xqueCls = (afxClass*)_AfxExuGetXqueClass(exu);
    AFX_ASSERT_CLASS(xqueCls, afxFcc_XQUE);
    afxIoQueue queues[AFX_MAX_QUEUES_PER_BRIDGE];
    AFX_ASSERT(AFX_MAX_QUEUES_PER_BRIDGE >= cfg->minQueCnt);

    if (AfxAcquireObjects(xqueCls, cfg->minQueCnt, (afxObject*)queues, (void const*[]) { exu, cfg }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, cfg->minQueCnt, queues);

        afxThreadConfig thrCfg = { 0 };
        //thrCfg.procCb = IoThreadProc;
        thrCfg.usage = afxThreadUsage_DRAW;
        thrCfg.udd[0] = exu;

        if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &exu->worker))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        AfxDeregisterChainedClasses(&exu->classes);
        AfxDismantleCondition(&exu->schedCnd);
        AfxDismantleMutex(&exu->schedCndMtx);
    }
    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_EXU =
{
    .fcc = afxFcc_EXU,
    .name = "IoBridge",
    .desc = "I/O Bridge",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoBridge)),
    .ctor = (void*)_AfxExuCtorCb,
    .dtor = (void*)_AfxExuDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError _AfxIomBridgeDevices(afxIommu iom, afxUnit cnt, _afxExuAcq const configs[], afxIoBridge bridges[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(configs);
    AFX_ASSERT(bridges);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AfxIomSW_GetExuClassCb(iom);
    AFX_ASSERT_CLASS(cls, afxFcc_EXU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)bridges, (void const*[]) { iom, configs }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_EXU, cnt, bridges);
    }

    return err;
}
