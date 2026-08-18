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

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_RENDERABLE_TERRAIN_H
#define ARX_RENDERABLE_TERRAIN_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/afxArray.h"
#include "qwadro/iris/arxIrisDefs.h"

AFX_DEFINE_STRUCT(arxRenderTerrain)
{
    arxTerrain ter;
};

ARX afxError ArxCmdRenderTerrain
(
    arxRenderContext rctx,
    arxTerrain ter
);

ARX afxBool RenderCells
(
    arxRenderContext rctx,
    afxDrawContext dctx,
    avxVertexInput vin,
    arxTerrain ter,
    afxFrustum* frustum,
    afxBool showFaces,
    afxBool showDbgLines
);

#endif//ARX_RENDERABLE_TERRAIN_H
