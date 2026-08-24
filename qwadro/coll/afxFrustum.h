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

#ifndef AFX_FRUSTUM_H
#define AFX_FRUSTUM_H

#include "qwadro/math/afxMatrix.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/coll/afxPlane.h"

typedef enum afxCubeFace
{
    afxCubeFace_L, // -0
    afxCubeFace_R, // +0
    afxCubeFace_B, // -0
    afxCubeFace_T, // +0
    afxCubeFace_N, // -0
    afxCubeFace_F, // +0

    afxCubeFace_TOTAL
} afxCubeFace;

typedef enum afxCubeCorner
{
    // near
    afxCubeCorner_LBN,
    afxCubeCorner_RBN,
    afxCubeCorner_RTN,
    afxCubeCorner_LTN,
    // far
    afxCubeCorner_LBF,
    afxCubeCorner_RBF,
    afxCubeCorner_RTF,
    afxCubeCorner_LTF,

    afxCubeCorner_TOTAL,
} afxCubeCorner;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxFrustum)
{
    afxPlane    planes[afxCubeFace_TOTAL];

    afxV4d      corners[afxCubeCorner_TOTAL];

    afxV4d      origin;
};

#define AFX_V4D_LBN AFX_V4D(-1,-1,-1, 1) // left-bottom-near
#define AFX_V4D_RBN AFX_V4D( 1,-1,-1, 1) // right-bottom-near
#define AFX_V4D_RTN AFX_V4D( 1, 1,-1, 1) // right-top-near
#define AFX_V4D_LTN AFX_V4D(-1, 1,-1, 1) // left-top-near

#define AFX_V4D_LBF AFX_V4D(-1,-1, 1, 1) // left-bottom-far
#define AFX_V4D_RBF AFX_V4D( 1,-1, 1, 1) // right-bottom-far
#define AFX_V4D_RTF AFX_V4D( 1, 1, 1, 1) // right-top-far
#define AFX_V4D_LTF AFX_V4D(-1, 1, 1, 1) // left-top-far

// frustrum and matrix must be in same space
AFXINL afxFrustum     AfxMakeFrustum(afxM4d const pv, afxM4d const ipv);

AFXINL afxBool  AfxFrustumCullsAabb(afxFrustum const f, afxBox const aabb);
AFXINL afxBool  AfxFrustumCullsAabbs(afxFrustum const f, afxUnit cnt, afxBox const aabb[]);
AFXINL afxBool  AfxFrustumCullsSphere(afxFrustum const f, afxSphere const sph);
AFXINL afxBool  AfxFrustumCullsSpheres(afxFrustum const f, afxUnit cnt, afxSphere const spheres[]);
AFXINL afxBool  AfxFrustumCullsFrustum(afxFrustum const f, afxFrustum const other);
AFXINL afxBool  AfxFrustumCullsFrustums(afxFrustum const f, afxUnit cnt, afxFrustum const others[]);

AFXINL afxBox     AfxEmboxFrustum(afxBox aabb, afxFrustum const f);

AFXINL void AfxGetFrustumCorners(afxFrustum const f, afxV3d vertices[AFX_NUM_BOX_CORNERS]);

AFXINL void AfxGetFrustumEdges(afxFrustum const f, afxV3d vertices[AFX_NUM_BOX_CORNERS], afxUnit indices[AFX_NUM_BOX_EDGE_VERTICES]);

AFXINL afxResult AfxAabbTestPlane(afxBox const aabb, afxPlane const plan);
AFXINL afxResult AfxAabbTestPlanes(afxBox const aabb, afxUnit cnt, afxPlane const planes[]);

#endif//AFX_FRUSTUM_H
