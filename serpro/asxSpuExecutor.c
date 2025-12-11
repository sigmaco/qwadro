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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ASX_STEP_C
#define _ASX_STEP_BRIDGE_C
#define _ASX_STEP_QUEUE_C
#define _ASX_STEP_SYSTEM_C
#define _ASX_STEP_CONTEXT_C
#define _ASX_BUFFER_C
#include "asxIcd.h"

#define _YIELD_WITH_SLEEP_AFTER_BRIDGE_PROC TRUE

_ASX afxError _AsxSpuWork_CallbackCb(asxSpu* spu, _asxIoReqPacket* work)
{
    afxError err = { 0 };
    return work->Callback.f(spu, work->Callback.udd);
}

_ASX afxError _AsxSpuWork_ExecuteCb(asxSpu* spu, _asxIoReqPacket* work)
{
    afxError err = { 0 };
    afxUnit cnt = work->Execute.cmdbCnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxStepContext sctx = work->Execute.cmdbs[i].sctx;
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
        
        AFX_ASSERT( (sctx->state == asxContextState_PENDING) ||
                    (sctx->state == asxContextState_INTERNAL_EXECUTING));

        _AsxSpuRollStepContexts(spu, sctx);

        // Must be disposed because AsxSubmitStepCommands() reacquires it.
        AfxDisposeObjects(1, &sctx);
    }

    return err;
}

_ASX afxError _AsxSpuWork_SinkCb(asxSpu* spu, _asxIoReqPacket* work)
{
    afxError err = { 0 };

    //work->Sink.sink->pushCb(work->Sink.sink, work->Sink.buf, &work->Sink.seg);
    return err;
}

_ASX afxError _AsxSpuWork_RemapCb(asxSpu* spu, _asxIoReqPacket* work)
{
    afxError err = { 0 };

    //work->Remap.buf, work->Remap.off, work->Remap.ran, work->Remap.flags, work->Remap.placeholder)

    //AfxDisposeObjects(1, &work->Remap.);

    return err;
}

_ASX afxError _AsxSpuWork_TransferCb(asxSpu* spu, _asxIoReqPacket* work)
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
            asxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                asxBufferIo const* op = &work->Transfer.bufOps[i];
                _AsxUpdateBuffer(buf, op, work->Transfer.src.src);
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
        asxBuffer buf = work->Transfer.src.buf;

        switch (work->Transfer.dstFcc)
        {
        case NIL: // buf to raw
        {
            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                asxBufferIo const* op = &work->Transfer.bufOps[i];
                _AsxDumpBuffer(buf, op, work->Transfer.dst.dst);
            }

            break;
        }
        case afxFcc_MBUF: // mbuf to mbuf
        {
            asxBuffer buf2 = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                asxBufferCopy const* op = &work->Transfer.bufCpyOps[i];
                _AsxCopyBuffer(buf2, op, buf);
            }

            AfxDisposeObjects(1, &buf2);

            break;
        }
        case afxFcc_IOB: // buf to iob
        {
            afxStream iob = work->Transfer.dst.iob;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                asxBufferIo const* op = &work->Transfer.bufOps[i];
                _AsxDownloadBuffer(buf, op, iob);
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
            asxBuffer buf = work->Transfer.dst.buf;

            for (afxUnit i = 0; i < work->Transfer.opCnt; i++)
            {
                asxBufferIo const* op = &work->Transfer.bufOps[i];
                _AsxUploadBuffer(buf, op, iob);
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

_ASX _asxIoReqLut const _ASX_SPU_IORP_VMT =
{
    .Callback = _AsxSpuWork_CallbackCb,
    .Execute = _AsxSpuWork_ExecuteCb,
    .Sink = _AsxSpuWork_SinkCb,
    .Transfer = _AsxSpuWork_TransferCb,
    //.Remap = _AsxSpuWork_RemapCb,
};

_ASX afxBool _AsxSpu_ProcCb(asxSpu* spu)
{
    afxError err = { 0 };
    
    afxStepBridge sexu = spu->sexu;
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    afxStepSystem ssys = AfxGetHost(sexu);

    _asxIoReqLut const* iorpVmt = sexu->iorpVmt;

    afxClass const* squeCls = _AsxSexuGetSqueClass(sexu);
    afxUnit queCnt = squeCls->instCnt;

    for (afxUnit queIdx = 0; queIdx < queCnt; queIdx++)
    {
        afxStepQueue sque;
        if (!AsxGetStepQueues(sexu, queIdx, 1, &sque) || !sque)
        {
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

        if (AfxTryLockMutex(&sque->iorpChnMtx))
        {
            _asxIoReqPacket* iorp;
            AFX_ITERATE_CHAIN_B2F(iorp, hdr.chain, &sque->iorpChn)
            {
                AFX_ASSERT(sque->iorpChn.cnt);
                AfxGetTime(&iorp->hdr.pullTime);

                iorpVmt->f[iorp->hdr.id](spu, iorp);

                AfxGetTime(&iorp->hdr.complTime);
                _AsxSquePopIoReqPacket(sque, iorp);
            }
            AfxSignalCondition(&sque->idleCnd);
            AfxUnlockMutex(&sque->iorpChnMtx);
        }
    }
    //_AsxSexu_PingCb(sexu, 0);
    return TRUE;
}

_ASX afxInt _ASX_SPU_THREAD_PROC(afxStepBridge sexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    afxThread thr;
    AfxGetThread(&thr);
    AFX_ASSERT(thr == sexu->worker);

    afxUnit exuId;
    afxStepSystem ssys = AsxGetBridgedStepSystem(sexu, &exuId);
    //afxUnit portId = sexu->portId;

    asxSpu* spu;
    if (AfxAllocate(sizeof(*spu), 0, AfxHere(), (void**)&spu))
        AfxThrowError();

    AfxZero(spu, sizeof(*spu));
    spu->sexu = sexu;
    spu->ssys = ssys;
    spu->exuIdx = exuId;
    spu->running = TRUE;
    spu->thr = thr;

    AfxReportf(6, NIL, "Step Execution Unit %.03.", exuId);
    
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

_ASX afxError _AsxSexu_PingCb(afxStepBridge sexu, afxUnit queIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);
    AfxLockMutex(&sexu->schedCndMtx);
    sexu->schedCnt += 1;
    AfxSignalCondition(&sexu->schedCnd);
    AfxUnlockMutex(&sexu->schedCndMtx);
    return err;
}
