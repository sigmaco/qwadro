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

#ifndef ARX_RENDERABLE_PUPPET_H
#define ARX_RENDERABLE_PUPPET_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/vga/afxDrawDefs.h"
#include "qwadro/afxArray.h"
#include "qwadro/iris/arxIrisDefs.h"
#include "qwadro/scene/arxNode.h"


ARX afxError ArxCmdRenderPuppets
(
    arxRenderContext rctx,
    afxM4d m,
    afxUnit cnt,
    arxPuppet puppets[]
);

ARX afxError ArxCmdRenderNode
(
    arxRenderContext rctx,
    afxUnit cnt,
    arxNode nodes[]
);


#endif//ARX_RENDERABLE_PUPPET_H
