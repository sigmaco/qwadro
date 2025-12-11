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
// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_STEP_BRIDGE_H
#define ASX_STEP_BRIDGE_H

#include "qwadro/pro/afxStepQueue.h"

#define ASX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum asxAptitude
{
    // Supports transfers via direct memory access.
    asxAptitude_DMA     = AFX_BITMASK(0),
    // Supports general purpose computation.
    asxAptitude_PCX     = AFX_BITMASK(1),
    // Supports parallel threaded execution.
    asxAptitude_PTX     = AFX_BITMASK(2),
} asxAptitude;

AFX_DEFINE_STRUCT(asxBridgeConfig)
{
    afxUnit         sdevId;
    afxAcceleration acceleration;
    asxAptitude     capabilities; // specifies capabilities of queues in a port.
    afxUnit         minQueCnt;
    afxReal const*  queuePriority;
};

ASX afxStepSystem   AsxGetBridgedStepSystem(afxStepBridge sexu, afxUnit* bridgeId);
ASX afxStepDevice   AsxGetBridgedStepDevice(afxStepBridge sexu, afxUnit* sdevId);

ASX afxUnit         AsxGetStepQueues(afxStepBridge sexu, afxUnit first, afxUnit cnt, afxStepQueue queues[]);

ASX afxError        AsxWaitForIdleStepBridge(afxStepBridge sexu, afxUnit64 timeout);
ASX afxError        AsxWaitForEmptyStepQueue(afxStepBridge sexu, afxUnit queIdx, afxUnit64 timeout);

#endif//ASX_STEP_BRIDGE_H
