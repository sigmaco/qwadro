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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_VERTEX_H
#define AFX_VERTEX_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxReal16.h"

AFXINL afxReal  AfxGetAngleBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL afxReal  AfxGetDistanceBetweenV3d(afxV3d const v, afxV3d const other);

AFXINL void     AfxExtractNormalV3dComponents(afxV3d const v, afxV3d const normal, afxV3d parallel, afxV3d perpendicular);

AFXINL void     AfxReflectV2d(afxV2d v, afxV2d const incident, afxV2d const normal);
AFXINL void     AfxReflectV3d(afxV3d v, afxV3d const incident, afxV3d const normal);
AFXINL void     AfxReflectV4d(afxV4d v, afxV3d const incident, afxV3d const normal);

AFXINL void     AfxRefractV2d(afxV2d v, afxV2d const incident, afxV2d const normal, afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxV3d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx);
AFXINL void     AfxRefractV3d(afxV3d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx);

#endif//AFX_VERTEX_H
