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

#ifndef ASX_IMPLEMENTATION_H
#define ASX_IMPLEMENTATION_H

#include "../qwadro_afx/coree/exec/afxSystemDDK.h"
#include "../qwadro_afx/coree/draw/avxIcd.h"
#include "qwadro/sim/arxEngine.h"
#include "qwadro/sim/arxSimulation.h"
#include "asxImpl_Executor.h"
#include "asxImpl_Context.h"
#include "asxImpl_Simulation.h"
#include "../anim/arxAnimationDDK.h"
#include "../anim/arxCurveDDK.h"
#include "../anim/arxPuppetDDK.h"

AFX_DEFINE_STRUCT(afxEngineInfo)
{
    afxDeviceInfo           dev;
    afxUnit                 portCnt;
    afxSimPortCaps const*portCaps;
    arxEngineFeatures       features;
    arxEngineLimits         limits;

    afxError(*stopCb)(arxEngine);
    afxError(*startCb)(arxEngine);
    afxError(*openSimCb)(arxEngine, arxSimulation, void** udd, afxUnit invokeNo); // unused if a custom dsys ctor is installed.
    afxError(*closeSimCb)(arxEngine, arxSimulation); // unused if a custom dsys ctor is installed.

    afxError(*closeMdgeCb)(arxEngine, arxSimBridge);
    afxError(*openMdgeCb)(arxEngine, arxSimBridge, afxSimBridgeConfig const*);
};

#ifdef _ASX_ENGINE_C
#ifdef _ASX_ENGINE_IMPL
AFX_OBJECT(_acxEngine)
#else
AFX_OBJECT(arxEngine)
#endif
{
    AFX_OBJECT(afxDevice) dev;

    arxEngineFeatures   features;
    arxEngineLimits     limits;
    afxUnit             portCnt;
    struct
    {
        afxSimPortCaps  caps;
    }*                  ports;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    afxError(*stopCb)(arxEngine);
    afxError(*startCb)(arxEngine);
    afxError(*openSimCb)(arxEngine, arxSimulation, void** udd, afxUnit invokeNo); // unused if a custom dsys ctor is installed.
    afxError(*closeSimCb)(arxEngine, arxSimulation); // unused if a custom dsys ctor is installed.

    afxError(*closeMdgeCb)(arxEngine, arxSimBridge);
    afxError(*openMdgeCb)(arxEngine, arxSimBridge, afxSimBridgeConfig const*);

    struct _afxSdevIdd* idd;
};
#endif//_ASX_ENGINE_C

ASX afxError _AsxRegisterEngines(afxModule icd, afxUnit cnt, afxEngineInfo const infos[], arxEngine devices[]);
ASX afxError _AsxImplementSimulation(afxModule icd, afxClassConfig const* sdevCls, afxClassConfig const* simCls);

ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG;




ASX afxClass const* _AsxGetEngineClass(afxModule icd);

ASX afxClass const* _AsxGetSimulationClass(afxModule icd);
ASX afxBool _AsxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//ASX_IMPLEMENTATION_H
