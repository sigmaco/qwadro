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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ACX_WARP_C
#define _ACX_WARP_BRIDGE_C
#define _ACX_WARP_QUEUE_C
#define _ACX_WARP_SYSTEM_C
#define _ACX_WARP_CONTEXT_C
#define _ACX_BUFFER_C
#include "../acxIcd.h"

#define _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC TRUE

_ACX afxError _AcxSpuWork_CallbackCb(acxSpu* spu, _acxIoReqPacket* work)
{
    afxError err = { 0 };
    return work->Callback.f(spu, work->Callback.udd);
}

_ACX afxError _AcxSpuWork_ExecuteCb(acxSpu* spu, _acxIoReqPacket* work)
{
    afxError err = { 0 };
    afxUnit cnt = work->Execute.cmdbCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxWarpContext sctx = work->Execute.cmdbs[i].sctx;
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
        
        AFX_ASSERT( (sctx->state == acxContextState_PENDING) ||
                    (sctx->state == acxContextState_INTERNAL_EXECUTING));

        _AcxSpuRollWarpContexts(spu, sctx);

        // Must be disposed because AcxSubmitWarpCommands() reacquires it.
        AfxDisposeObjects(1, &sctx);
    }

    return err;
}

_ACX afxError _AcxSpuWork_SinkCb(acxSpu* spu, _acxIoReqPacket* work)
{
    afxError err = { 0 };

    //work->Sink.sink->pushCb(work->Sink.sink, work->Sink.buf, &work->Sink.seg);
    return err;
}

_ACX afxError _AcxSpuWork_RemapCb(acxSpu* spu, _acxIoReqPacket* work)
{
    afxError err = { 0 };

    //work->Remap.buf, work->Remap.off, work->Remap.ran, work->Remap.flags, work->Remap.placeholder)

    //AfxDisposeObjects(1, &work->Remap.);

    return err;
}

_ACX afxError _AcxSpuWork_TransferCb(acxSpu* spu, _acxIoReqPacket* work)
{
    afxError err = { 0 };

    switch (work->Transfer.srcFcc)
    {
    case NIL:
    {
        switch (work->Transfer.dstFcc)
        {
        case afxFcc_BUF: // raw to mbuf
        {
            acxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                acxBufferIo const* op = &work->Transfer.bufOps[i];
                _AcxUpdateBuffer(buf, op, work->Transfer.src.src);
            }

            AfxDisposeObjects(1, &buf);

            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxFcc_MBUF:
    {
        acxBuffer buf = work->Transfer.src.buf;

        switch (work->Transfer.dstFcc)
        {
        case NIL: // buf to raw
        {
            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                acxBufferIo const* op = &work->Transfer.bufOps[i];
                _AcxDumpBuffer(buf, op, work->Transfer.dst.dst);
            }

            break;
        }
        case afxFcc_MBUF: // mbuf to mbuf
        {
            acxBuffer buf2 = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                acxBufferCopy const* op = &work->Transfer.bufCpyOps[i];
                _AcxCopyBuffer(buf2, op, buf);
            }

            AfxDisposeObjects(1, &buf2);

            break;
        }
        case afxFcc_IOB: // buf to iob
        {
            afxStream iob = work->Transfer.dst.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                acxBufferIo const* op = &work->Transfer.bufOps[i];
                _AcxDownloadBuffer(buf, op, iob);
            }

            AfxDisposeObjects(1, &iob);

            break;
        }
        default: AfxThrowError(); break;
        }

        AfxDisposeObjects(1, &buf);

        break;
    }
    case afxFcc_IOB:
    {
        afxStream iob = work->Transfer.src.iob;

        switch (work->Transfer.dstFcc)
        {
        case afxFcc_MBUF: // iob to mbuf
        {
            acxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                acxBufferIo const* op = &work->Transfer.bufOps[i];
                _AcxUploadBuffer(buf, op, iob);
            }

            AfxDisposeObjects(1, &buf);

            break;
        }
        default: AfxThrowError(); break;
        }

        AfxDisposeObjects(1, &iob);

        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ACX _acxIoReqLut const _ACX_SPU_IORP_VMT =
{
    .Callback = _AcxSpuWork_CallbackCb,
    .Execute = _AcxSpuWork_ExecuteCb,
    .Sink = _AcxSpuWork_SinkCb,
    .Transfer = _AcxSpuWork_TransferCb,
    //.Remap = _AcxSpuWork_RemapCb,
};

_ACX afxBool _AcxSpu_ProcCb(acxSpu* spu)
{
    afxError err = { 0 };
    
    afxWarpBridge sexu = spu->sexu;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxWarpSystem ssys = AfxGetHost(sexu);

    _acxIoReqLut const* iorpVmt = sexu->iorpVmt;

    afxClass const* squeCls = _AcxSexuGetSqueClass(sexu);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxWarpQueue sque;
        if (!AcxGetWarpQueues(sexu, queIdx, 1, &sque) || !sque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (AfxTryLockMutex(&sque->iorpChnMtx))
        {
            _acxIoReqPacket* iorp;
            AFX_ITERATE_CHAIN_B2F(iorp, hdr.chain, &sque->iorpChn)
            {
                AFX_ASSERT(sque->iorpChn.cnt);
                AfxGetTime(&iorp->hdr.pullTime);

                iorpVmt->f[iorp->hdr.id](spu, iorp);

                AfxGetTime(&iorp->hdr.complTime);
                _AcxSquePopIoReqPacket(sque, iorp);
            }
            AfxSignalCondition(&sque->idleCnd);
            AfxUnlockMutex(&sque->iorpChnMtx);
        }
    }
    //_AcxSexu_PingCb(sexu, 0);
    return TRUE;
}

_ACX afxInt _ACX_SPU_THREAD_PROC(afxWarpBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == sexu->worker);

    afxUnit exuId;
    afxWarpSystem ssys = AcxGetBridgedWarpSystem(sexu, &exuId);
    //afxUnit portId = sexu->portId;

    acxSpu* spu;
    if (AfxAllocate(sizeof(*spu), 0, AfxHere(), (void**)&spu))
        AfxThrowError();

    AfxZero(spu, sizeof(*spu));
    spu->sexu = sexu;
    spu->ssys = ssys;
    spu->exuIdx = exuId;
    spu->running = TRUE;
    spu->thr = thr;

    AfxReportf(6, NIL, "Warp Execution Unit %.03.", exuId);
    
    sexu->schedCnt = 1;

    do
    {
#if !0
        AfxLockMutex(&sexu->schedCndMtx);

        while (!sexu->schedCnt)
            AfxWaitTimedCondition(&sexu->schedCnd, &sexu->schedCndMtx, (afxTimeSpec[]) { { .nsecs = AFX_NANOSECS_PER_MILLISECS(16) } });

        sexu->schedCnt = 0;
        AfxUnlockMutex(&sexu->schedCndMtx);
#endif

        sexu->procCb(spu);
        
        //_ZalProcessDeletionQueue(al, &sexu->deletionQueue);

        if (AfxShouldThreadBeInterrupted())
            break;

#ifdef _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC
        AfxSleep(1);
#else
        AfxYield();
#endif//_YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC

    } while (1);

    AFX_ASSERT(spu == sexu->spu);
    AfxDeallocate((void**)&spu, AfxHere());

    return 0;
}

_ACX afxError _AcxSexu_PingCb(afxWarpBridge sexu, afxUnit queIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AfxLockMutex(&sexu->schedCndMtx);
    sexu->schedCnt += 1;
    AfxSignalCondition(&sexu->schedCnd);
    AfxUnlockMutex(&sexu->schedCndMtx);
    return err;
}
