/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_QUEUE_DDK_H
#define ASX_QUEUE_DDK_H

#include "asxStorageDDK.h"

AFX_DECLARE_UNION(_asxIoReqPacket);
AFX_DECLARE_UNION(_asxIoReqLut);

#ifdef _ASX_STEP_QUEUE_C
#ifdef _ASX_STEP_QUEUE_IMPL
AFX_OBJECT(_asxStepQueue)
#else
AFX_OBJECT(afxStepQueue)
#endif
{
    afxStepDevice   sdev;
    //afxUnit         portId;
    afxUnit         exuIdx;
    afxStepSystem   ssys;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxFutex        iorpArenaSlock;
    afxArena        iorpArena; // used by submission of queue operations, not stream commands.        
    afxChain        iorpChn;
    afxMutex        iorpChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError(*waitCb)(afxStepQueue, afxTime);
    //afxError(*submCb)(afxStepQueue, asxFence fenc, afxUnit cnt, asxQueueOp const req[]);
};
#endif//_ASX_STEP_QUEUE_C

AFX_DEFINE_STRUCT(_asxIoReqPacketHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxUnit dpuId;
    afxUnit syncUnitId;
    void* syncIdd0;
    afxSize syncIdd1;
    afxError(*exec)(void*, afxStepBridge, afxUnit queIdx, _asxIoReqPacket*);
    afxTime complTime; // completation time
    asxFence completionFence;
    afxSize idd[4];
};

AFX_DEFINE_UNION(_asxIoReqLut)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Sink;
        void* Remap;
        void* SyncMaps;
    };
    afxError(*f[])(void*, _asxIoReqPacket const*);
};

#define _ASX_GET_STD_IORP_ID(cmdName_) (offsetof(_asxIoReqLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_asxIoReqPacket)
{
    _asxIoReqPacketHdr hdr;
    struct
    {
        _asxIoReqPacketHdr hdr;

        afxUnit         cmdbCnt;
        struct
        {
            asxFence    wait;
            afxUnit64   waitValue;
            asxBusStage waitStageMask;
            afxUnit32   waitReserved;
            asxFence    signal;
            afxUnit64   signalValue;
            asxBusStage signalStageMask;
            afxUnit32   signalReserved;

            afxStepContext sctx;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _asxIoReqPacketHdr hdr;

        asxFence    wait;
        afxUnit64   waitValue;
        asxBusStage waitStageMask;
        afxUnit32   waitReserved;
        asxFence    signal;
        afxUnit64   signalValue;
        asxBusStage signalStageMask;
        afxUnit32   signalReserved;

        union
        {
            //amxAudio    aud;
            asxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            //amxAudio    aud;
            asxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               dst;
        afxFcc          dstFcc;
        afxCodec        codec;
        afxUnit         decSiz;
        afxUnit         opCnt;
        union
        {
            //amxAudioIo AFX_SIMD wavOps[];
            //amxAudioCopy AFX_SIMD wavCpyOps[];
            asxBufferIo AFX_SIMD bufOps[];
            asxBufferCopy AFX_SIMD bufCpyOps[];
        };
    } Transfer;
    struct
    {
        _asxIoReqPacketHdr hdr;
        afxUnit     firstMapIdx;
        afxUnit     mapCnt;
        afxUnit     firstUnmapIdx;
        afxUnit     unmapCnt;
        union
        {
            _asxBufferRemapping mapOps[];
            _asxBufferRemapping unmapOps[];
        };
    } Remap;
    struct
    {
        _asxIoReqPacketHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            asxBufferedMap ops[];
        };
    } SyncMaps;
    struct
    {
        _asxIoReqPacketHdr hdr;

        afxUnit         submType;
        afxError(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        _asxIoReqPacketHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        asxFence fences[];
    } WaitForFences;
};

ASX afxError _AsxSquePopIoReqPacket(afxStepQueue sque, _asxIoReqPacket* work);
ASX afxError _AsxSquePushIoReqPacket(afxStepQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _asxIoReqPacket** iorp);
ASX afxError _AsxSquePopBlob(afxStepQueue sque, void* blob, afxUnit siz);
ASX void* _AsxSquePushBlob(afxStepQueue sque, afxUnit siz);

ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG;

// Common queued operations

ASX afxError _AsxSqueSubmitCallback(afxStepQueue sque, afxError(*f)(void*, void*), void* udd);
ASX afxError _AsxSqueSubmitStepCommands(afxStepQueue sque, afxUnit cnt, asxSubmission subms[]);
ASX afxError _AsxSqueTransferResources(afxStepQueue sque, asxTransference const* ctrl, afxUnit opCnt, void const* ops);
ASX afxError _AsxSqueRemapBuffers(afxStepQueue sque, afxUnit mapCnt, _asxBufferRemapping const maps[], afxUnit unmapCnt, _asxBufferRemapping const unmaps[]);
ASX afxError _AsxSqueCohereMappedBuffers(afxStepQueue sque, afxUnit flushCnt, asxBufferedMap const flushes[], afxUnit fetchCnt, asxBufferedMap const fetches[]);

#endif//ASX_QUEUE_DDK_H
