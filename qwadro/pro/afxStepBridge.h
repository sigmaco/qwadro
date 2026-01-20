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

#ifndef ACX_STEP_BRIDGE_H
#define ACX_STEP_BRIDGE_H

#include "qwadro/pro/afxStepQueue.h"

#define ACX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum acxAptitude
{
    // Supports transfers via direct memory access.
    acxAptitude_DMA     = AFX_BITMASK(0),
    // Supports general purpose computation.
    acxAptitude_PCX     = AFX_BITMASK(1),
    // Supports parallel threaded execution.
    acxAptitude_PTX     = AFX_BITMASK(2),
} acxAptitude;

AFX_DEFINE_STRUCT(acxBridgeConfig)
{
    afxUnit         sdevId;
    afxAcceleration acceleration;
    acxAptitude     capabilities; // specifies capabilities of queues in a port.
    afxUnit         minQueCnt;
    afxReal const*  queuePriority;
};

ACX afxStepSystem   AcxGetBridgedStepSystem(afxStepBridge sexu, afxUnit* bridgeId);
ACX afxStepDevice   AcxGetBridgedStepDevice(afxStepBridge sexu, afxUnit* sdevId);

ACX afxUnit         AcxGetStepQueues(afxStepBridge sexu, afxUnit first, afxUnit cnt, afxStepQueue queues[]);

ACX afxError        AcxWaitForIdleStepBridge(afxStepBridge sexu, afxUnit64 timeout);
ACX afxError        AcxWaitForEmptyStepQueue(afxStepBridge sexu, afxUnit queIdx, afxUnit64 timeout);

#endif//ACX_STEP_BRIDGE_H
