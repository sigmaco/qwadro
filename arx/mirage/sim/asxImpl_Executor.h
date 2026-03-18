/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ASX_IMPL___EXECUTOR_H
#define ASX_IMPL___EXECUTOR_H

#include "qwadro/sim/arxSimulation.h"

#define ASX_MAX_SIM_QUEUE_PER_BRIDGE 32

AFX_DECLARE_UNION(asxWork);
AFX_DECLARE_UNION(asxWorkList);
AFX_DECLARE_UNION(asxCmdList);

AFX_DEFINE_STRUCT(asxSpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    arxSimBridge    sexu;
    afxThread       thr;
    arxSimulation   sim;

    afxBool         running;
    afxBool         instanced;
    
    arxPose         pose;
    afxUnit         basXform;
    afxUnit         xformCnt;
    
    arxModel        skl;
    afxUnit         baseJnt;
    afxUnit         jntCnt;
    afxReal         allowedErr;
    afxReal         allowedErrEnd;
    afxReal         allowedErrScaler;
    afxUnit const*  sparseJntMap;
    afxM4d          displacement;
    afxUnit         baseArtIdx;
    afxUnit         artCnt;

    struct
    {
        avxBuffer   buf;
        afxUnit     base;
        afxUnit     range;
        afxByte*    bytemap;
    } bufBindings[16];
    afxUnit bufBindCnt;
};

AFX_DEFINE_STRUCT(_acxSimBridgeAcquisition)
{
    arxEngine               seng;
    afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   mctxClsCfg;
};

#ifdef _ASX_SIM_BRIDGE_C
#ifdef _ASX_SIM_BRIDGE_IMPL
AFX_OBJECT(_acxSimBridge)
#else
AFX_OBJECT(arxSimBridge)
#endif
{
    arxEngine seng;
    afxUnit portId;
    afxUnit exuIdx;
    afxChain classes;
    afxClass squeCls;

    afxError(*pingCb)(arxSimBridge,afxUnit);

    // MPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt(*workerProc)(arxSimBridge);
    asxWorkList const*workVmt;
    asxCmdList const*cmdVmt;
    afxError(*endOfCtxCb)(asxSpu*);
    afxBool(*procCb)(asxSpu*);
    asxSpu*         spu;
};
#endif//_ASX_SIM_BRIDGE_C

#ifdef _ASX_SIM_QUEUE_C
#ifdef _ASX_SIM_QUEUE_IMPL
AFX_OBJECT(_acxSimQueue)
#else
AFX_OBJECT(arxSimQueue)
#endif
{
    arxEngine       seng;
    afxUnit         portId;
    afxUnit         exuIdx;
    arxSimulation   sim;
    afxBool         immediate; // 0 = deferred, 1 = immediate
    afxBool         closed; // can't enqueue

    afxCondition idleCnd;
    //afxMutex idleCndMtx;
    afxFutex workArenaSlock;
    afxArena workArena; // used by submission of queue operations, not stream commands.        
    afxChain workChn;
    afxMutex workChnMtx;


    afxChain        classes;
    // one command pool per queue to avoid thread interation.
    afxClass        cmdbCls;
    // one stream manager per queue to avoid thread interaction
    afxQueue        cmdbRecycQue;
    afxFutex        cmdbReqLock;
    afxBool         cmdbLockedForReq;

    afxError(*waitCb)(arxSimQueue, afxTime);
};
#endif//_ASX_SIM_QUEUE_C

AFX_DEFINE_UNION(asxWorkList)
{
    struct
    {
        void* Callback;
        void* Execute;
    };
    void(*f[])(void*, asxWork const*);
};

#define ASX_GET_STD_WORK_ID(cmdName_) (offsetof(asxWorkList, cmdName_) / sizeof(void*))

AFX_DEFINE_STRUCT(asxWorkHdr)
{
    afxLink chain;
    afxUnit id;
    afxUnit siz;
    afxUnit submNo; // this submission number ordinal (B2F)
    afxUnit reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime pushTime; // submission (into input) time
    afxTime pullTime; // fecth (by queue) time
    afxTime complTime; // completation time
    afxError(*exec)(void*, arxSimBridge, afxUnit queIdx, asxWork*);
};

AFX_DEFINE_UNION(asxWork)
{
    asxWorkHdr hdr;
    struct
    {
        asxWorkHdr hdr;

        afxUnit         submType;
        void(*f)(void*, void*);
        void*           udd;
        afxUnit         dataSiz;
        afxByte AFX_SIMD data[];
    } Callback;
    struct
    {
        asxWorkHdr hdr;

        afxSemaphore    wait;
        afxSemaphore    signal;
        avxFence        fence;
        afxUnit         cmdbCnt;
        arxContext AFX_SIMD cmdbs[];
    } Execute;
};

ASX afxError _AsxSquePopWork(arxSimQueue sque, asxWork* work);
ASX asxWork* _AsxSquePushWork(arxSimQueue sque, afxUnit id, afxUnit siz, afxCmdId* cmdId);
ASX afxError _AsxSquePopBlob(arxSimQueue sque, void* blob, afxUnit siz);
ASX void* _AsxSquePushBlob(arxSimQueue sque, afxUnit siz);

ASX afxClass const* _AsxGetCatalystClass(arxSimQueue sque);

ASX afxClass const* _AsxGetSimQueueClass(arxSimBridge mdge);
ASX afxUnit _AsxCountSampleQueues(arxSimBridge mdge, afxUnit baseQueIdx);
ASX afxBool _AsxGetSampleQueue(arxSimBridge mdge, afxUnit queIdx, arxSimQueue* queue);
ASX afxError _AsxWaitForIdleSampleQueue(arxSimBridge mdge, afxUnit queIdx);
ASX afxError _AsxWaitForIdleSampleBridge(arxSimBridge mdge);

ASX arxEngine AfxGetSampleQueueDevice(arxSimQueue sque);
ASX arxSimulation AfxGetSampleQueueContext(arxSimQueue sque);
ASX afxUnit AfxGetSampleQueuePort(arxSimQueue sque);

ASX asxWorkList const _ASX_SPU_WORK_VMT;
ASX afxInt   _ASX_SPU_THREAD_PROC(arxSimBridge sexu);
ASX afxBool  _AsxSpu_ProcCb(asxSpu*);
ASX afxError _AsxSexu_PingCb(arxSimBridge sexu, afxUnit queIdx);

ASX afxError _AsxSpuRollContext(asxSpu* spu, arxContext ctx);

ASX afxClassConfig const _ASX_SEXU_CLASS_CONFIG;
ASX afxClassConfig const _ASX_SQUE_CLASS_CONFIG;

ASX afxError _AsxExecuteSampleCommands(arxSimQueue sque, asxSubmission const* ctrl, afxUnit cnt, arxContext contexts[]);

#endif//ASX_IMPL___EXECUTOR_H
