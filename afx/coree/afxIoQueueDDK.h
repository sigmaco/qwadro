/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AFX_IO_QUEUE_DDK_H
#define AFX_IO_QUEUE_DDK_H

//#include "afxIommuDDK.h"
#include "afxIoContextDDK.h"
#include "afxFenceDDK.h"
#include "../afxBufferDDK.h"
#include "qwadro/afxIoQueue.h"

AFX_DECLARE_UNION(_afxIoReqPacket);
AFX_DECLARE_UNION(_afxIoReqLut);

AFX_DECLARE_STRUCT(_afxIddXque);
AFX_DEFINE_STRUCT(_afxDdiXque)
{
    afxError(*waitCb)(afxIoQueue, afxUnit64);
};

#ifdef _AFX_IO_QUEUE_C
#ifdef _AFX_IO_QUEUE_IMPL
AFX_OBJECT(_afxIoQueue)
#else
AFX_OBJECT(afxIoQueue)
#endif
{
    _afxDdiXque const*  ddi;
    _afxIddXque*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxDevice           dev;
    //afxUnit           portId;
    afxUnit             exuIdx;
    afxMask             exuMask;
    afxIommu            iom; // owner context
    afxBool             immediate; // 0 = deferred, 1 = immediate
    afxBool             closed; // can't enqueue

    afxCondition        idleCnd;
    //afxMutex          idleCndMtx;
    afxFutex            iorpArenaSlock;
    afxArena            iorpArena; // used by submission of queue operations, not stream commands.        
    afxChain            iorpChn;
    afxMutex            iorpChnMtx;

    afxChain            classes;
    // one stream manager per queue to avoid thread interaction
};
#endif//_AFX_IO_QUEUE_C

AFX_DEFINE_UNION(_afxIoReqLut)
{
    struct
    {
        void* Callback;
        void* Execute;
        void* Transfer;
        void* Remap;
        void* SyncMaps;
    };
    afxError(*f[])(void*, _afxIoReqPacket*);
};

#define _AFX_GET_STD_IORP_ID(cmdName_) (offsetof(_afxIoReqLut, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(_afxIoReqPacketHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxClock pushTime; // submission (into input) time
    afxClock pullTime; // fecth (by queue) time
    afxBool pulled;
    afxUnit dpuId;
    afxUnit syncUnitId;
    void* syncIdd0;
    afxSize syncIdd1;
    afxError(*exec)(void*, afxIoBridge, afxUnit queIdx, _afxIoReqPacket*);
    afxClock complTime; // completation time
    afxBool completed;
    //afxFence completionFence;
    afxSize idd[4];


    void*       udd;
    afxString   tag;
};

AFX_DEFINE_UNION(_afxIoReqPacket)
{
    _afxIoReqPacketHdr hdr;
    struct
    {
        _afxIoReqPacketHdr hdr;

        afxUnit     cmdbCnt;
        struct
        {
            afxFence    wait;
            afxUnit64   waitValue;
            afxMask     waitStageMask;
            afxUnit32   waitReserved;
            afxFence    signal;
            afxUnit64   signalValue;
            afxMask     signalStageMask;
            afxUnit32   signalReserved;

            afxIoContext  dctx;
            afxUnit         batchId;
        } AFX_SIMD cmdbs[];
    } Execute;
    struct
    {
        _afxIoReqPacketHdr hdr;

        afxFence    wait;
        afxUnit64   waitValue;
        afxMask     waitStageMask;
        afxUnit32   waitReserved;
        afxFence    signal;
        afxUnit64   signalValue;
        afxMask     signalStageMask;
        afxUnit32   signalReserved;

        union
        {
            afxBuffer   buf;
            void*       dst;
            void const* src;
            afxStream   iob;
        }               src;
        afxFcc          srcFcc;
        union
        {
            afxBuffer   buf;
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
            afxBufferCopy AFX_SIMD bufCpyOps[];
            afxBufferIo AFX_SIMD bufOps[];
        };
    } Transfer;
    struct
    {
        _afxIoReqPacketHdr hdr;
        afxUnit     firstMapIdx;
        afxUnit     mapCnt;
        afxUnit     firstUnmapIdx;
        afxUnit     unmapCnt;
        union
        {
            _afxBufferRemapping mapOps[];
            _afxBufferRemapping unmapOps[];
        };
    } Remap;
    struct
    {
        _afxIoReqPacketHdr  hdr;
        afxUnit     baseFlushIdx;
        afxUnit     flushCnt;
        afxUnit     baseFetchIdx;
        afxUnit     fetchCnt;
        union
        {
            afxBufferedMap ops[];
        };
    } SyncMaps;
    struct
    {
        _afxIoReqPacketHdr hdr;

        afxUnit         submType;
        afxError        (*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        _afxIoReqPacketHdr hdr;

        afxBool waitAll;
        afxUnit64 timeout;
        afxUnit cnt;
        afxFence fences[];
    } WaitForFences;
};

AFX afxClassConfig const _AFX_CLASS_CONFIG_XQUE;

AFX _afxIoReqLut const _AFX_XPU_IORP_VMT;

AFX afxError _AfxAcquireIoQueues
(
    afxIommu iom,
    afxUnit cnt,
    afxMask const bridges[],
    afxIoQueue queues[]
);

AFX afxError _AfxXqueUnlockIoReqChain(afxIoQueue dque);
AFX afxError _AfxXqueLockIoReqChain(afxIoQueue dque, afxUnit64 timeout);
AFX afxError _AfxXquePopIoReqPacket(afxIoQueue dque, _afxIoReqPacket* work);
AFX afxError _AfxXquePushIoReqPacket(afxIoQueue dque, afxUnit id, afxUnit siz, afxCmdId* cmdId, _afxIoReqPacket** pIorp);

// Common queued operations
AFX afxError AfxSubmitIoCommands(afxIoQueue dque, afxUnit cnt, afxSubmission const subms[]);
AFX afxError _AfxXqueTransferResources(afxIoQueue dque, afxTransference const* ctrl, afxUnit opCnt, void const* ops);
AFX afxError _AfxXqueRemapBuffers(afxIoQueue dque, afxUnit mapCnt, _afxBufferRemapping const maps[], afxUnit unmapCnt, _afxBufferRemapping const unmaps[]);
AFX afxError _AfxXqueCohereMappedBuffers(afxIoQueue dque, afxUnit flushCnt, afxBufferedMap const flushes[], afxUnit fetchCnt, afxBufferedMap const fetches[]);
AFX afxError _AfxXqueSubmitCallback(afxIoQueue dque, afxError(*f)(void*, void*), void* udd);

AFX void _AfxBeginIoQueueDebugScope(afxIoQueue dque, afxString const* name, afxV4d const color);
AFX void _AfxPushIoQueueDebugLabel(afxIoQueue dque, afxString const* name, afxV4d const color);
AFX void _AfxEndIoQueueDebugScope(afxIoQueue dque);

#endif//AFX_IO_QUEUE_DDK_H
