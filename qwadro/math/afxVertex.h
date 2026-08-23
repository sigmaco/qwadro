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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_VERTEX_H
#define AFX_VERTEX_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxReal16.h"
#include "qwadro/math/afxArithmetic.h"
#include "qwadro/math/afxArithmetic2.h"

AFXINL afxReal  AfxGetAngleBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL afxReal  AfxGetDistanceBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL void     AfxExtractNormalV3dComponents(afxV3d const v, afxV3d const normal, afxV3d parallel, afxV3d perpendicular);

AFXINL afxV2d     AfxV2dReflect(afxV2d const incident, afxV2d const normal);
AFXINL afxV3d     AfxV3dReflect(afxV3d const incident, afxV3d const normal);
AFXINL afxV4d     AfxV4dReflect(afxV3d const incident, afxV3d const normal);

AFXINL afxV2d     AfxV2dRefract(afxV2d const incident, afxV2d const normal, afxReal refracIdx);
AFXINL afxV3d     AfxV3dRefract(afxV3d const incident, afxV3d const normal, afxReal refracIdx);
AFXINL afxV3d     AfxV3dRefract(afxV3d const incident, afxV3d const normal, afxReal refracIdx);

#endif//AFX_VERTEX_H
