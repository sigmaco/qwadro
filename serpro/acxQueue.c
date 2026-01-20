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
// This software is part of Advanced Multimedia Extensions.

#define _ACX_STEP_C
#define _ACX_STEP_BRIDGE_C
#define _ACX_STEP_QUEUE_C
#define _ACX_STEP_CONTEXT_C
#define _ACX_BUFFER_C
#include "acxIcd.h"

_ACX afxError _AcxSqueUnlockIoReqChain(afxStepQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxUnlockMutex(&sque->iorpChnMtx);
    return err;
}

_ACX afxError _AcxSqueLockIoReqChain(afxStepQueue sque, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (timeout == 0)
    {
        if (!AfxTryLockMutex(&sque->iorpChnMtx))
            return afxError_TIMEOUT;
    }
    else
    {
        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        if (afxError_TIMEOUT == AfxLockMutexTimed(&sque->iorpChnMtx, &ts))
            return afxError_TIMEOUT;
        else
            AfxThrowError();
    }
    return err;
}

_ACX afxError _AcxSquePopBlob(afxStepQueue sque, void* blob, afxUnit siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AfxReclaimArena(&sque->iorpArena, blob, siz);
    return err;
}

_ACX void* _AcxSquePushBlob(afxStepQueue sque, afxUnit siz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    void* blob = AfxRequestArena(&sque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(blob);
    return blob;
}

_ACX afxError _AcxSquePopIoReqPacket(afxStepQueue sque, _acxIoReqPacket* iorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(iorp);
    AfxPopLink(&iorp->hdr.chain);
    AfxReclaimArena(&sque->iorpArena, iorp, iorp->hdr.siz);
    return err;
}

_ACX afxError _AcxSquePushIoReqPacket(afxStepQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _acxIoReqPacket** pIorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    _acxIoReqPacket* iorp = AfxRequestArena(&sque->iorpArena, siz, 1, NIL, 0);
    AFX_ASSERT(iorp);
    iorp->hdr.id = id;
    iorp->hdr.siz = siz;
    AfxGetTime(&iorp->hdr.pushTime);

    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&iorp->hdr.chain, &sque->iorpChn);
    
    AFX_ASSERT(pIorp);
    *pIorp = iorp;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_ACX afxStepDevice AcxGetStepQueueDevice(afxStepQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxStepDevice sdev = AcxGetBridgedStepDevice(sexu, NIL);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev;
}

_ACX afxStepSystem AcxGetStepQueueSystem(afxStepQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    afxStepSystem ssys = sque->ssys;
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    return ssys;
}

_ACX afxError _AcxSqueSubmitCallback(afxStepQueue sque, afxError(*f)(void*, void*), void* udd)
{
    afxError err = { 0 };
    // sque must be a valid afxStepQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(f);

    if (!AfxTryLockMutex(&sque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _acxIoReqPacket* iorp;

    if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Callback), sizeof(iorp->Callback), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->Callback.f = f;
        iorp->Callback.udd = udd;
    }

    AfxUnlockMutex(&sque->iorpChnMtx);

    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _AcxSexu_PingCb(sexu, 0);

    return err;
}

_ACX afxError _AcxSqueSubmitStepCommands(afxStepQueue sque, afxUnit cnt, acxSubmission subms[])
{
    afxError err = { 0 };
    // sque must be a valid afxStepQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(subms);

    /*
        If any step context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a step context complete, it moves from the pending state, back to the executable state.
        If a step context was recorded with the ONCE flag, it instead moves back to the invalid state.
    */

    if (!AfxTryLockMutex(&sque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _acxIoReqPacket* iorp;

    if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Execute), sizeof(iorp->Execute) + (cnt * sizeof(iorp->Execute.cmdbs[0])), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        for (afxUnit i = 0; i < cnt; i++)
        {
            iorp->Execute.cmdbCnt = 1;

            afxStepContext sctx = subms[i].sctx;
            AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
            AfxReacquireObjects(1, &sctx);
            AFX_ASSERT(!(sctx->cmdFlags & acxCmdFlag_DEFERRED));

            if (sctx->state != acxContextState_INTERNAL_EXECUTING)
            {
                if (sctx->state = acxContextState_EXECUTABLE)
                    sctx->state = acxContextState_PENDING;
            }
            else
            {
                AFX_ASSERT(sctx->cmdFlags & acxCmdFlag_SHARED);
            }

            iorp->Execute.cmdbs[i].sctx = sctx;
            iorp->Execute.cmdbs[i].wait = subms[i].wait;
            iorp->Execute.cmdbs[i].waitValue = subms[i].waitValue;
            iorp->Execute.cmdbs[i].waitReserved = subms[i].waitReserved;
            iorp->Execute.cmdbs[i].waitStageMask = subms[i].waitStageMask;
            iorp->Execute.cmdbs[i].signal = subms[i].signal;
            iorp->Execute.cmdbs[i].signalValue = subms[i].signalValue;
            iorp->Execute.cmdbs[i].signalReserved = subms[i].signalReserved;
            iorp->Execute.cmdbs[i].signalStageMask = subms[i].signalStageMask;
        }
    }

    AfxUnlockMutex(&sque->iorpChnMtx);

    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _AcxSexu_PingCb(sexu, 0);

    return err;
}

_ACX afxError _AcxSqueTransferResources(afxStepQueue sque, acxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // sque must be a valid afxStepQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    if (!AfxTryLockMutex(&sque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _acxIoReqPacket* iorp;

    if ((ctrl->dstFcc == afxFcc_AUD) && (ctrl->srcFcc == afxFcc_AUD))
    {
#if 0
        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.rasCpyOps[0])), &cmdId, &iorp))
#endif
            AfxThrowError();
    }
    else if ((ctrl->dstFcc == afxFcc_AUD) || (ctrl->srcFcc == afxFcc_AUD))
    {
#if 0
        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.wavOps[0])), &cmdId, &iorp))
            AfxThrowError();
#endif
    }
    else
    {
        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT(iorp);

        iorp->Transfer.wait = ctrl->wait;
        iorp->Transfer.waitValue = ctrl->waitValue;
        iorp->Transfer.waitReserved = ctrl->waitReserved;
        iorp->Transfer.waitStageMask = ctrl->waitStageMask;
        iorp->Transfer.signal = ctrl->signal;
        iorp->Transfer.signalValue = ctrl->signalValue;
        iorp->Transfer.signalReserved = ctrl->signalReserved;
        iorp->Transfer.signalStageMask = ctrl->signalStageMask;

        iorp->Transfer.srcFcc = ctrl->srcFcc;
        iorp->Transfer.dstFcc = ctrl->dstFcc;
        iorp->Transfer.codec = ctrl->codec;
        iorp->Transfer.decSiz = ctrl->decSiz;

        iorp->Transfer.opCnt = opCnt;

        switch (ctrl->srcFcc)
        {
        case afxFcc_SBUF:
        {
            iorp->Transfer.src.buf = ctrl->src.buf;
            AfxReacquireObjects(1, &iorp->Transfer.src.buf);
            break;
        }
        case afxFcc_IOB:
        {
            iorp->Transfer.src.iob = ctrl->src.iob;
            AfxReacquireObjects(1, &iorp->Transfer.src.iob);
            break;
        }
        case NIL:
        {
            iorp->Transfer.src.src = ctrl->src.src;
            break;
        }
        default: AfxThrowError(); break;
        }

        switch (ctrl->dstFcc)
        {
        case afxFcc_SBUF:
        {
            iorp->Transfer.dst.buf = ctrl->dst.buf;
            AfxReacquireObjects(1, &iorp->Transfer.dst.buf);
            break;
        }
        case afxFcc_IOB:
        {
            iorp->Transfer.dst.iob = ctrl->dst.iob;
            AfxReacquireObjects(1, &iorp->Transfer.dst.iob);
            break;
        }
        case NIL:
        {
            iorp->Transfer.dst.dst = ctrl->dst.dst;
            break;
        }
        default: AfxThrowError(); break;
        }
#if 0
        if ((ctrl->dstFcc == afxFcc_AUD) || (ctrl->srcFcc == afxFcc_AUD))
        {
            acxAudioIo const* riops = ops;
            for (afxUnit i = 0; i < opCnt; i++)
            {
                iorp->Transfer.wavOps[i] = riops[i];
            }
        }
        else
#endif
        {
            acxBufferIo const* riops = ops;
            for (afxUnit i = 0; i < opCnt; i++)
            {
                iorp->Transfer.bufOps[i] = riops[i];
            }
        }
    }

    AfxUnlockMutex(&sque->iorpChnMtx);

    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _AcxSexu_PingCb(sexu, 0);

    return err;
}

_ACX afxError _AcxSqueRemapBuffers(afxStepQueue sque, afxUnit mapCnt, _acxBufferRemapping const maps[], afxUnit unmapCnt, _acxBufferRemapping const unmaps[])
{
    afxError err = { 0 };
    // sque must be a valid afxStepQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (AfxLockMutex(&sque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (mapCnt)
    {
        afxCmdId cmdId;
        _acxIoReqPacket* iorp;

        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (mapCnt * sizeof(iorp->Remap.mapOps[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->Remap.firstMapIdx = 0;
            iorp->Remap.mapCnt = mapCnt;
            iorp->Remap.firstUnmapIdx = 0;
            iorp->Remap.unmapCnt = 0;

            for (afxUnit i = 0; i < mapCnt; i++)
            {
                _acxBufferRemapping const* map = &maps[i];

                iorp->Remap.mapOps[i].buf = map->buf;
                iorp->Remap.mapOps[i].offset = map->offset;
                iorp->Remap.mapOps[i].range = map->range;
                iorp->Remap.mapOps[i].flags = map->flags;
                iorp->Remap.mapOps[i].placeholder = map->placeholder;

                AfxReacquireObjects(1, &map->buf);
                AfxIncAtom32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    if (unmapCnt)
    {
        afxCmdId cmdId;
        _acxIoReqPacket* iorp;

        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (unmapCnt * sizeof(iorp->Remap.unmapOps[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->Remap.firstMapIdx = 0;
            iorp->Remap.firstUnmapIdx = 0;
            iorp->Remap.mapCnt = 0;
            iorp->Remap.unmapCnt = unmapCnt;

            for (afxUnit i = 0; i < unmapCnt; i++)
            {
                _acxBufferRemapping const* map = &unmaps[i];

                iorp->Remap.unmapOps[i].buf = map->buf;

                AfxReacquireObjects(1, &map->buf);
                AfxIncAtom32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    AfxUnlockMutex(&sque->iorpChnMtx);

    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _AcxSexu_PingCb(sexu, 0);

    return err;
}

_ACX afxError _AcxSqueCohereMappedBuffers(afxStepQueue sque, afxUnit flushCnt, acxBufferedMap const flushes[], afxUnit fetchCnt, acxBufferedMap const fetches[])
{
    afxError err = { 0 };
    // sque must be a valid afxStepQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (AfxLockMutex(&sque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (flushCnt)
    {
        afxCmdId cmdId;
        _acxIoReqPacket* iorp;

        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (flushCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->SyncMaps.baseFetchIdx = 0;
            iorp->SyncMaps.fetchCnt = 0;
            iorp->SyncMaps.baseFlushIdx = 0;
            iorp->SyncMaps.flushCnt = flushCnt;

            for (afxUnit i = 0; i < flushCnt; i++)
            {
                acxBufferedMap const* map = &flushes[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    if (fetchCnt)
    {
        afxCmdId cmdId;
        _acxIoReqPacket* iorp;

        if (_AcxSquePushIoReqPacket(sque, _ACX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (fetchCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(iorp);

            iorp->SyncMaps.baseFetchIdx = 0;
            iorp->SyncMaps.flushCnt = 0;
            iorp->SyncMaps.baseFlushIdx = 0;
            iorp->SyncMaps.fetchCnt = fetchCnt;

            for (afxUnit i = 0; i < fetchCnt; i++)
            {
                acxBufferedMap const* map = &fetches[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    AfxUnlockMutex(&sque->iorpChnMtx);

    afxStepBridge sexu = AfxGetHost(sque);
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _AcxSexu_PingCb(sexu, 0);

    return err;
}

_ACX afxError _AcxSqueDtorCb(afxStepQueue sque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    AfxDismantleMutex(&sque->iorpChnMtx);
    AfxDismantleArena(&sque->iorpArena);
    AfxCleanUpFutex(&sque->iorpArenaSlock);
    AfxDismantleCondition(&sque->idleCnd);
    AfxDismantleMutex(&sque->idleCndMtx);

    return err;
}

_ACX afxError _AcxSqueCtorCb(afxStepQueue sque, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    afxStepBridge sexu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    _acxSexuAcq const* cfg = AFX_CAST(_acxSexuAcq const*, args[1]);
    AFX_ASSERT(cfg);

    sque->sdev = cfg->sdev;
    //sque->portId = cfg->portId;
    sque->exuIdx = cfg->exuIdx;
    sque->ssys = AcxGetBridgedStepSystem(sexu, NIL);

    sque->immediate = 0;// !!spec->immedate;

    AfxSetUpFutex(&sque->iorpArenaSlock);
    AfxMakeArena(&sque->iorpArena, NIL, AfxHere());

    AfxDeployMutex(&sque->iorpChnMtx, AFX_MTX_PLAIN);
    AfxMakeChain(&sque->iorpChn, sexu);
    AfxDeployMutex(&sque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&sque->idleCnd);

    sque->closed = FALSE;

    return err;
}

_ACX afxClassConfig const _ACX_SQUE_CLASS_CONFIG =
{
    .fcc = afxFcc_SQUE,
    .name = "StepQueue",
    .desc = "Step Device Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepQueue)),
    .ctor = (void*)_AcxSqueCtorCb,
    .dtor = (void*)_AcxSqueDtorCb
};
