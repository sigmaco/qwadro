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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
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

ARX afxClass const* _ArxIcdGetScioClass(afxModule icd);

ARX afxError _ArxIcdImplement(afxModule icd, afxClassConfig const* scioCls);

ARX afxBool _ArxGetIcd(afxUnit icdIdx, afxModule* driver);

ARX afxError _ArxAmendEcosystem(afxSystem sys);

#endif//ARX_ICD_H
