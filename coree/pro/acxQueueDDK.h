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

// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_QUEUE_DDK_H
#define ACX_QUEUE_DDK_H

#include "acxStorageDDK.h"

AFX_DECLARE_UNION(_acxIoReqPacket);
AFX_DECLARE_UNION(_acxIoReqLut);

#ifdef _ACX_WARP_QUEUE_C
#ifdef _ACX_WARP_QUEUE_IMPL
AFX_OBJECT(_acxStepQueue)
#else
AFX_OBJECT(afxWarpQueue)
#endif
{
    afxWarpDevice   sdev;
    //afxUnit         portId;
    afxUnit         exuIdx;
    afxWarpSystem   ssys;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxFutex        iorpArenaSlock;
    afxArena        iorpArena; // used by submission of queue operations, not stream commands.        
    afxChain        iorpChn;
    afxMutex        iorpChnMtx;

    afxCondition    idleCnd;
    afxMutex        idleCndMtx;

    afxError(*waitCb)(afxWarpQueue, afxTime);
    //afxError(*submCb)(afxWarpQueue, acxFence fenc, afxUnit cnt, acxQueueOp const req[]);
};
#endif//_ACX_WARP_QUEUE_C

AFX_DEFINE_STRUCT(_acxIoReqPacketHdr)
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
    afxError(*exec)(void*, afxWarpBridge, afxUnit queIdx, _acxIoReqPacket*);
    afxTime complTime; // completation time
    acxFence completionFence;
    afxSize idd[4];
};

AFX_DEFINE_UNION(_acxIoReqLut)
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
    afxError(*f[])(void*, _acxIoReqPacket const*);
};

#define _ACX_GET_STD_IORP_ID(cmdName_) (offsetof(_acxIoReqLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_acxIoReqPacket)
{
    _acxIoReqPacketHdr hdr;
    struct
    {
        _acxIoReqPacketHdr hdr;

        afxUnit         cmdbCnt;
        struct
        {
            acxFence    wait;
            afxUnit64   waitValue;
            acxBusStage waitStageMask;
            afxUnit32   waitReserved;
            acxFence    signal;
            afxUnit64   signalValue;
            acxBusStage signalStageMask;
            afxUnit32   signalReserved;

            afxWarpContext sctx;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _acxIoReqPacketHdr hdr;

        acxFence    wait;
        afxUnit64   waitValue;
        acxBusStage waitStageMask;
        afxUnit32   waitReserved;
        acxFence    signal;
        afxUnit64   signalValue;
        acxBusStage signalStageMask;
        afxUnit32   signalReserved;

        union
        {
            //amxAudio    aud;
            acxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            //amxAudio    aud;
            acxBuffer   buf;
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
            acxBufferIo AFX_SIMD bufOps[];
            acxBufferCopy AFX_SIMD bufCpyOps[];
        };
    } Transfer;
    struct
    {
        _acxIoReqPacketHdr hdr;
        afxUnit     firstMapIdx;
        afxUnit     mapCnt;
        afxUnit     firstUnmapIdx;
        afxUnit     unmapCnt;
        union
        {
            _acxBufferRemapping mapOps[];
            _acxBufferRemapping unmapOps[];
        };
    } Remap;
    struct
    {
        _acxIoReqPacketHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            acxBufferedMap ops[];
        };
    } SyncMaps;
    struct
    {
        _acxIoReqPacketHdr hdr;

        afxUnit         submType;
        afxError(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        _acxIoReqPacketHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        acxFence fences[];
    } WaitForFences;
};

ACX afxError _AcxSquePopIoReqPacket(afxWarpQueue sque, _acxIoReqPacket* work);
ACX afxError _AcxSquePushIoReqPacket(afxWarpQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _acxIoReqPacket** iorp);
ACX afxError _AcxSquePopBlob(afxWarpQueue sque, void* blob, afxUnit siz);
ACX void* _AcxSquePushBlob(afxWarpQueue sque, afxUnit siz);

ACX afxClassConfig const _ACX_SQUE_CLASS_CONFIG;

// Common queued operations

ACX afxError _AcxSqueSubmitCallback(afxWarpQueue sque, afxError(*f)(void*, void*), void* udd);
ACX afxError _AcxSqueSubmitWarpCommands(afxWarpQueue sque, afxUnit cnt, acxSubmission subms[]);
ACX afxError _AcxSqueTransferResources(afxWarpQueue sque, acxTransference const* ctrl, afxUnit opCnt, void const* ops);
ACX afxError _AcxSqueRemapBuffers(afxWarpQueue sque, afxUnit mapCnt, _acxBufferRemapping const maps[], afxUnit unmapCnt, _acxBufferRemapping const unmaps[]);
ACX afxError _AcxSqueCohereMappedBuffers(afxWarpQueue sque, afxUnit flushCnt, acxBufferedMap const flushes[], afxUnit fetchCnt, acxBufferedMap const fetches[]);

#endif//ACX_QUEUE_DDK_H
