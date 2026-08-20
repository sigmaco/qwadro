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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
//#define _AFX_IO_DEVICE_C
//#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_IO_CONTEXT_C
#define _AFX_BUFFER_C
#include "afxIoQueueDDK.h"
#include "../afxSystemDDK.h"

_AFX afxError _AfxXqueUnlockIoReqChain(afxIoQueue xque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxUnlockMutex(&xque->iorpChnMtx);
    return err;
}

_AFX afxError _AfxXqueLockIoReqChain(afxIoQueue xque, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    
    if (timeout == 0)
    {
        if (!AfxTryLockMutex(&xque->iorpChnMtx))
            return afxError_TIMEOUT;
    }
    else
    {
        afxTimeSpec ts;
        AfxMakeTimeSpec(&ts, timeout);

        if (afxError_TIMEOUT == AfxLockMutexTimed(&xque->iorpChnMtx, &ts))
            return afxError_TIMEOUT;
        else
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxXquePopIoReqPacket(afxIoQueue xque, _afxIoReqPacket* iorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxPopLink(&iorp->hdr.chain);
    AfxReclaimArena(&xque->iorpArena, iorp, iorp->hdr.siz);
    return err;
}

_AFX afxError _AfxXquePushIoReqPacket(afxIoQueue xque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _afxIoReqPacket** pIorp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    _afxIoReqPacket* iorp = AfxRequestArena(&xque->iorpArena, siz, 1, NIL, 0);
    
    if (!iorp) AfxThrowError();
    else
    {
        AfxZero(iorp, sizeof(iorp->hdr));
        iorp->hdr.id = id;
        iorp->hdr.siz = siz;
        AfxGetClock(&iorp->hdr.pushTime);
        AFX_ASSERT(cmdId);
        *cmdId = AfxPushLink(&iorp->hdr.chain, &xque->iorpChn);
        AFX_ASSERT(pIorp);
        *pIorp = iorp;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxIoSystem AfxGetIoQueueDock(afxIoQueue xque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    afxIoSystem iom = xque->iom;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &iom);
    return iom;
}

_AFX void _AfxBeginIoQueueDebugScope(afxIoQueue xque, afxString const* name, afxV4d const color)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxThrowError();
}

_AFX void _AfxEndIoQueueDebugScope(afxIoQueue xque)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxThrowError();
}

_AFX void _AfxPushIoQueueDebugLabel(afxIoQueue xque, afxString const* name, afxV4d const color)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AfxThrowError();
}

_AFX afxError AfxWaitForEmptyIoQueue(afxIoQueue xque, afxUnit64 timeout)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    
    if (xque->ddi->waitCb)
    {
        if (xque->ddi->waitCb(xque, timeout))
            AfxThrowError();
    }
    else
    {
#if 0
        afxBool again = TRUE;

        while (again)
        {
            afxTimeSpec ts = { 0 };
            if (timeout && (timeout != AFX_TIMEOUT_INFINITE))
                AfxMakeTimeSpec(&ts, timeout);

            AfxLockMutex(&xque->idleCndMtx);

            if (xque->iorpChn.cnt)
            {
                if (timeout == AFX_TIMEOUT_INFINITE)
                {
                    AfxWaitCondition(&xque->idleCnd, &xque->idleCndMtx);
                    again = FALSE;
                    break;
                }
                else if (afxError_TIMEOUT == AfxWaitTimedCondition(&xque->idleCnd, &xque->idleCndMtx, &ts))
                    if (timeout != AFX_TIMEOUT_INFINITE)
                        break;

                if (!timeout)
                {
                    again = FALSE;
                    break; // do not stall; return immediately.
                }
                else if (timeout == AFX_TIMEOUT_INFINITE)
                    continue; // cycle again
            }
            else again = FALSE;

            AfxUnlockMutex(&xque->idleCndMtx);
        }
#else
        if (!timeout)
        {
            if (!AfxTryLockMutex(&xque->iorpChnMtx)) err = afxError_TIMEOUT;
            else
            {
                while (!AfxIsChainEmpty(&xque->iorpChn))
                {
                    err = afxError_TIMEOUT;
                    break; // while
                }

                if (!err)
                {
                    AFX_ASSERT(AfxIsChainEmpty(&xque->iorpChn));
                    AFX_ASSERT(xque->iorpChn.cnt == 0);
                }

                AfxUnlockMutex(&xque->iorpChnMtx);
            }
        }
        else if ((timeout == AFX_TIMEOUT_INFINITE))
        {
            if (AfxLockMutex(&xque->iorpChnMtx))
                AfxThrowError();

            while (!AfxIsChainEmpty(&xque->iorpChn))
            {
                if (AfxWaitCondition(&xque->idleCnd, &xque->iorpChnMtx))
                {
                    AfxThrowError();
                    break;
                }
            }

            AFX_ASSERT(AfxIsChainEmpty(&xque->iorpChn));
            AFX_ASSERT(xque->iorpChn.cnt == 0);

            AfxUnlockMutex(&xque->iorpChnMtx);
        }
        else
        {
            // TODO: timed?
            AfxLockMutex(&xque->iorpChnMtx);

            afxTimeSpec ts = { 0 };
            AfxMakeTimeSpec(&ts, timeout);

            while (!AfxIsChainEmpty(&xque->iorpChn))
            {
                afxError cndErr = AfxWaitTimedCondition(&xque->idleCnd, &xque->iorpChnMtx, &ts);

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
                AFX_ASSERT(AfxIsChainEmpty(&xque->iorpChn));
            }

            AfxUnlockMutex(&xque->iorpChnMtx);
        }
#endif
    }
    return err;
}

_AFX afxError _AfxXqueSubmitCallback(afxIoQueue xque, afxError(*f)(void*, void*), void* udd)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AFX_ASSERT(f);

    if (!AfxTryLockMutex(&xque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _afxIoReqPacket* iorp;
    
    if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Callback), sizeof(iorp->Callback), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        iorp->Callback.f = f;
        iorp->Callback.udd = udd;
    }

    AfxUnlockMutex(&xque->iorpChnMtx);

    afxIoBridge dexu = AfxGetHost(xque);
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &dexu);
    _AfxDexu_PingCb(dexu, 0);

    return err;
}

_AFX afxError AfxSubmitIoCommands(afxIoQueue xque, afxUnit cnt, afxSubmission const subms[])
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AFX_ASSERT(cnt);
    AFX_ASSERT(subms);

    /*
        If any IO context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a IO context complete, it moves from the pending state, back to the executable state.
        If a IO context was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    if (!AfxTryLockMutex(&xque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _afxIoReqPacket* iorp;

    if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Execute), sizeof(iorp->Execute) + (cnt * sizeof(iorp->Execute.cmdbs[0])), &cmdId, &iorp))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(iorp);

        for (afxUnit i = 0; i < cnt; i++)
        {
            iorp->Execute.cmdbCnt = 1;

            afxIoContext ctx = subms[i].dctx;
            AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
            AfxReacquireObjects(1, &ctx);
            AFX_ASSERT(!(ctx->cmdFlags & afxCmdFlag_DEFERRED));
            
            if (ctx->state != afxContextState_INTERNAL_EXECUTING)
            {
                if (ctx->state = afxContextState_EXECUTABLE)
                    ctx->state = afxContextState_PENDING;
            }
            else
            {
                AFX_ASSERT(ctx->cmdFlags & afxCmdFlag_SHARED);
            }

            iorp->Execute.cmdbs[i].dctx = ctx;

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

    AfxUnlockMutex(&xque->iorpChnMtx);

    afxIoBridge exu = AfxGetHost(xque);
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    _AfxExu_PingCb(exu, 0);

    return err;
}

_AFX afxError _AfxXqueTransferResources(afxIoQueue xque, afxTransference const* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ops);

    if (!AfxTryLockMutex(&xque->iorpChnMtx))
        return afxError_TIMEOUT;

    afxCmdId cmdId;
    _afxIoReqPacket* iorp;

    if ((ctrl->dstFcc == afxFcc_BUF) && (ctrl->srcFcc == afxFcc_BUF))
    {
        if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufCpyOps[0])), &cmdId, &iorp))
            AfxThrowError();
    }
    else if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Transfer), sizeof(iorp->Transfer) + (opCnt * sizeof(iorp->Transfer.bufOps[0])), &cmdId, &iorp))
        AfxThrowError();

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
        case afxFcc_BUF:
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
        case afxFcc_BUF:
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

        if ((ctrl->srcFcc == afxFcc_BUF) || (ctrl->dstFcc == afxFcc_BUF))
        {
            if ((ctrl->srcFcc == afxFcc_BUF) && (ctrl->dstFcc == afxFcc_BUF))
            {
                afxBufferCopy const* biops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.bufCpyOps[i] = biops[i];
                }
            }
            else
            {
                afxBufferIo const* biops = ops;
                for (afxUnit i = 0; i < opCnt; i++)
                {
                    iorp->Transfer.bufOps[i] = biops[i];
                }
            }
            int a = 1;
        }
    }

    AfxUnlockMutex(&xque->iorpChnMtx);

    afxIoBridge exu = AfxGetHost(xque);
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    _AfxExu_PingCb(exu, 0);

    return err;
}

_AFX afxError _AfxXqueRemapBuffers(afxIoQueue xque, afxUnit mapCnt, _afxBufRemapping const maps[], afxUnit unmapCnt, _afxBufRemapping const unmaps[])
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    if (AfxLockMutex(&xque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (mapCnt)
    {
        afxCmdId cmdId;
        _afxIoReqPacket* iorp;
        
        if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (mapCnt * sizeof(iorp->Remap.mapOps[0])), &cmdId, &iorp))
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
                _afxBufRemapping const* map = &maps[i];

                iorp->Remap.mapOps[i].buf = map->buf;
                iorp->Remap.mapOps[i].offset = map->offset;
                iorp->Remap.mapOps[i].range = map->range;
                iorp->Remap.mapOps[i].flags = map->flags;
                iorp->Remap.mapOps[i].placeholder = map->placeholder;

                AfxReacquireObjects(1, &map->buf);
                AfxAtomicInc32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    if (unmapCnt)
    {
        afxCmdId cmdId;
        _afxIoReqPacket* iorp;
        
        if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(Remap), sizeof(iorp->Remap) + (unmapCnt * sizeof(iorp->Remap.unmapOps[0])), &cmdId, &iorp))
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
                _afxBufRemapping const* map = &unmaps[i];

                iorp->Remap.unmapOps[i].buf = map->buf;

                AfxReacquireObjects(1, &map->buf);
                AfxAtomicInc32(&map->buf->storage[0].pendingRemap);
            }
        }
    }

    AfxUnlockMutex(&xque->iorpChnMtx);

    afxIoBridge exu = AfxGetHost(xque);
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    _AfxExu_PingCb(exu, 0);

    return err;
}

_AFX afxError _AfxXqueCohereMappedBuffers(afxIoQueue xque, afxUnit flushCnt, afxBufferedMap const flushes[], afxUnit fetchCnt, afxBufferedMap const fetches[])
{
    afxError err = { 0 };
    // xque must be a valid afxIoQueue handle.
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    if (AfxLockMutex(&xque->iorpChnMtx))
        return afxError_TIMEOUT;

    if (flushCnt)
    {
        afxCmdId cmdId;
        _afxIoReqPacket* iorp;
        
        if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (flushCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
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
                afxBufferedMap const* map = &flushes[i];

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
        _afxIoReqPacket* iorp;
        
        if (_AfxXquePushIoReqPacket(xque, _AFX_GET_STD_IORP_ID(SyncMaps), sizeof(iorp->SyncMaps) + (fetchCnt * sizeof(iorp->SyncMaps.ops[0])), &cmdId, &iorp))
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
                afxBufferedMap const* map = &fetches[i];

                iorp->SyncMaps.ops[i].buf = map->buf;
                iorp->SyncMaps.ops[i].offset = map->offset;
                iorp->SyncMaps.ops[i].range = map->range;

                AfxReacquireObjects(1, &map->buf);
            }
        }
    }

    AfxUnlockMutex(&xque->iorpChnMtx);

    afxIoBridge exu = AfxGetHost(xque);
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);
    _AfxExu_PingCb(exu, 0);

    return err;
}

_AFX _afxXqueDdi const _AFX_DDI_XQUE =
{

};

_AFX afxError _AfxXqueDtorCb(afxIoQueue xque)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    AfxDeregisterChainedClasses(&xque->classes);

    AfxDismantleMutex(&xque->iorpChnMtx);
    AfxDismantleArena(&xque->iorpArena);
    AfxCleanUpFutex(&xque->iorpArenaSlock);
    AfxDismantleCondition(&xque->idleCnd);
    //AfxDismantleMutex(&xque->idleCndMtx);

    return err;
}

_AFX afxError _AfxXqueCtorCb(afxIoQueue xque, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &xque);

    afxIoBridge exu = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DEXU, 1, &exu);
    _afxExuAcq const* cfg = AFX_CAST(_afxExuAcq const*, args[1]);

#if 0
    afxMask const* bridges = AFX_CAST(afxMask const*, args[1]);
    AFX_ASSERT(bridges);
    afxMask exuMask = bridges[invokeNo];

    afxUnit exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, AFX_INVALID_INDEX, NIL, 0, 0, NIL);
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxIoBridge dexu;
        if (!AfxGetIoBridges(iom, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }
    }
    xque->exuMask = exuMask;
#else
    //AFX_ASSERT(cfg->portId != AFX_INVALID_INDEX);
    xque->dev = cfg->dev;
    //xque->portId = cfg->portId;
    xque->exuIdx = cfg->exuIdx;
#endif
    xque->iom = AfxGetBridgedIoSystem(exu, NIL);

    xque->immediate = 0;// !!spec->immedate;

    AfxSetUpFutex(&xque->iorpArenaSlock);
    AfxMakeArena(&xque->iorpArena, NIL, AfxHere());

    AfxDeployMutex(&xque->iorpChnMtx, AFX_MTX_PLAIN);
    AfxMakeChain(&xque->iorpChn, xque);
    //AfxDeployMutex(&xque->idleCndMtx, AFX_MTX_PLAIN);
    AfxDeployCondition(&xque->idleCnd);

    xque->closed = FALSE;

    AfxMakeChain(&xque->classes, (void*)xque);

    xque->ddi = &_AFX_DDI_XQUE;
    xque->idd = NIL;

    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_XQUE =
{
    .fcc = afxFcc_XQUE,
    .name = "IoQueue",
    .desc = "Device-Execution Queue",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoQueue)),
    .ctor = (void*)_AfxXqueCtorCb,
    .dtor = (void*)_AfxXqueDtorCb
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxError _AfxAcquireIoQueues(afxIommu iom, afxUnit cnt, afxMask const bridges[], afxIoQueue queues[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &iom);

    afxClass* xqueCls = (afxClass*)_AfxExuGetXqueClass(iom);
    AFX_ASSERT_CLASS(xqueCls, afxFcc_XQUE);

    AFX_ASSERT(AFX_MAX_QUEUES_PER_BRIDGE >= cnt);

    if (AfxAcquireObjects(xqueCls, cnt, (afxObject*)queues, (void const*[]) { iom, bridges }))
    {
        AfxThrowError();
    }

    if (!err)
    {
        AFX_ASSERT_OBJECTS(afxFcc_XQUE, cnt, queues);
    }

    return err;
}
#endif
