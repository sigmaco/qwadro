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

#ifndef ARX_COLLIDER_H
#define ARX_COLLIDER_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/afxFixedString.h"
#include "qwadro/cad/arxMesh.h"

AFX_DEFINE_STRUCT(arxCollTriangle)
{
    afxV3d  v0; // Reference vertex of the triangle
    afxV3d  e1; // First edge of the triangle (v1 - v0)
    afxV3d  e2; // Second edge of the triangle (v2 - v0)
    afxV3d  nrm; // Normal of the triangle (cross(edge1, edge2))
};



#endif//ARX_COLLIDER_H
