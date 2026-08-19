/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions.

/*
    A mix bridge, as well as other bridges and queues in Qwadro, is highly based on Commondore Amiga-way 
    of handling device connections, here specially AHI connection. Despite the main enginner never having touched on in his life.
*/

#ifndef AMX_MIX_BRIDGE_H
#define AMX_MIX_BRIDGE_H

#include "qwadro/mix/afxMixQueue.h"

#define AMX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum amxService
{
    // Supports transfers via direct memory access.
    amxService_DMA     = AFX_BITMASK(0),
    // Supports sound effects.
    amxService_SFX     = AFX_BITMASK(1),
    // Supports video effects.
    amxService_VFX     = AFX_BITMASK(2),
    // Supports general purpose computation.
    amxService_PCX     = AFX_BITMASK(3),
    // Supports on-device output.
    amxService_SINK    = AFX_BITMASK(4),
    // Supports video acceleration on input/output.
    amxService_VAIO    = AFX_BITMASK(5),
} amxService;

AFX_DEFINE_STRUCT(amxBridgeConfig)
{
    afxUnit         mdevId;
    afxAcceleration acceleration;
    amxService     capabilities; // specifies capabilities of queues in a port.
    afxUnit         minQueCnt;
    afxReal const*  queuePriority;
};

AMX afxMixSystem    AmxGetBridgedMixSystem(afxMixBridge mexu, afxUnit* bridgeId);
AMX afxMixDevice    AmxGetBridgedMixDevice(afxMixBridge mexu, afxUnit* mdevId);

AMX afxUnit         AmxGetMixQueues(afxMixBridge mexu, afxUnit first, afxUnit cnt, afxMixQueue queues[]);

AMX afxError        AmxWaitForIdleMixBridge(afxMixBridge mexu, afxUnit64 timeout);
AMX afxError        AmxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxUnit64 timeout);

#endif//AMX_MIX_BRIDGE_H
