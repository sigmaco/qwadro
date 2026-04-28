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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This file is part of Advanced RenderWare Extensions.

/*
    A scene is a observation of a simulation.
*/

#ifndef ARX_SKY_H
#define ARX_SKY_H

#include "qwadro/iris/arxIrisDefs.h"

typedef enum arxSkyType
{
    arxSkyType_BOX,
    arxSkyType_DOME,
} arxSkyType;

AFX_DEFINE_STRUCT(arxSkyInfo)
{
    arxSkyType skyType;
};

ARX afxError ArxBuildSky
(
    arxScenario scio, 
    arxSkyInfo const* info, 
    arxSky* sky
);

#endif//ARX_SKY_H
