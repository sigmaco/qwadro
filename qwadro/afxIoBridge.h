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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Future Storage.

#ifndef AFX_IO_BRIDGE_H
#define AFX_IO_BRIDGE_H

#include "qwadro/afxDevice.h"
#include "qwadro/afxIoQueue.h"
#include "qwadro/afxFile.h"

#define AFX_MAX_QUEUES_PER_BRIDGE (32)

AFX_DEFINE_STRUCT(afxIoBridgeConfig)
{
    afxUnit         devId;

    afxAcceleration acceleration;

    afxAptitude     capabilities; // specifies capabilities of queues in a port.

    afxUnit         minQueCnt;

    afxReal const*  queuePriority;
};

AFX afxIommu AfxGetBridgedIoSystem
(
    afxIoBridge exu,

    afxUnit* bridgeId
);

AFX afxDevice AfxGetBridgedIoDevice
(
    afxIoBridge exu,

    afxUnit* ddevId
);

AFX afxUnit AfxGetIoQueues
(
    afxIoBridge exu,

    afxUnit baseQueIdx,

    afxUnit cnt,

    afxIoQueue queues[]
);

AFX afxError AfxWaitForIdleIoBridge
(
    afxIoBridge exu,

    afxUnit64 timeout
);

#endif//AFX_IO_BRIDGE_H
