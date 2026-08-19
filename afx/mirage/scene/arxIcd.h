/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.
// This file is part of Integrated Raster Imaging System.

#ifndef ARX_ICD_H
#define ARX_ICD_H

#include "../anim/arxMotiveDDK.h"
#include "../scene/arxScenarioDDK.h"
#include "../iris/arxRenderContextDDK.h"
#include "../sim/asxImplementation.h"

AFX_DECLARE_STRUCT(_arxIddIcd);

#ifndef _ARX_SIM_C
AFX_DECLARE_STRUCT(_arxDdiIcd);
#else
AFX_DEFINE_STRUCT(_arxDdiIcd)
{
    afxError(*cfgScioCb)(afxModule, arxScenarioConfig*);
    afxError(*acqScioCb)(afxModule, arxScenarioConfig const*, arxScenario*);
    afxClass const*(*getScioClsCb)(afxModule icd);
};
#endif

ARX afxClass const* _ArxIcdGetScioClass(afxModule icd);

ARX afxError _ArxIcdImplement(afxModule icd, afxClassConfig const* scioCls);

ARX afxError _ArxAmendEcosystem(afxSystem sys);

ARX afxBool _ArxGetIcd(afxSystem sys, afxUnit icdIdx, afxModule* driver);

AFX afxError ArxGetIcd(afxUnit unit, afxModule* arxIcd);

ARX afxError _ArxIcdConfigureScioSW(afxModule arxIcd, arxScenarioConfig* cfg);

ARX afxError _ArxIcdAcquireScioSW(afxModule arxIcd, arxScenarioConfig const* cfg, arxScenario* scenario);

ARX _arxDdiIcd const* _ArxGetDdi(afxModule arxIcd);

#endif//ARX_ICD_H
