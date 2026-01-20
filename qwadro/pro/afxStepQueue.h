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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_STEP_QUEUE_H
#define ACX_STEP_QUEUE_H

#include "qwadro/pro/afxStepDefs.h"

typedef enum acxBusStage
// Bitmask-based value specifying pipeline stages.
{
    acxBusStage_HOST = AFX_BITMASK(0),
    acxBusStage_COMMANDS = AFX_BITMASK(1),
    acxBusStage_COPY = AFX_BITMASK(4),
    acxBusStage_RESOLVE = AFX_BITMASK(5),
    acxBusStage_TRANSFER = (acxBusStage_COPY | acxBusStage_RESOLVE),
    acxBusStage_COMPUTE = AFX_BITMASK(8),
} acxBusStage;

AFX_DEFINE_STRUCT(acxSubmission)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxFlags            flags;

    // An array of step contexts that represent the individual stepping commands or operations to be executed. 
    // Each afxMixContext could contain specific information about a set of stepping commands, such as commands 
    // for synthetizing objects, setting up filters, and managing resources.
    afxStepContext      sctx;

    acxFence            wait;
    afxUnit64           waitValue;
    acxBusStage         waitStageMask;
    afxUnit32           waitReserved;
    // A fence which will be signaled when the operation have completed execution.
    acxFence            signal;
    afxUnit64           signalValue;
    acxBusStage         signalStageMask;
    afxUnit32           signalReserved;
};

AFX_DEFINE_STRUCT(acxTransference)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    // A semaphore upon which to wait on before the operation begin execution.
    acxFence    wait;
    afxUnit64   waitValue;
    acxBusStage waitStageMask;
    afxUnit32   waitReserved;
    // A semaphore which will be signaled when the operation have completed execution.
    // A fence which will be signaled when the operation have completed execution.
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
    afxUnit         encSiz;
    afxUnit         decSiz;
};

ACX afxStepDevice    AcxGetStepQueueDevice(afxStepQueue sque);
ACX afxStepSystem    AcxGetStepQueueSystem(afxStepQueue sque);

#endif//ACX_STAGE_QUEUE_H
