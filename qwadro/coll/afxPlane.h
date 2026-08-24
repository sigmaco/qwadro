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

#ifndef AFX_PLANE_H
#define AFX_PLANE_H

#include "qwadro/coll/afxBox.h"
#include "qwadro/math/afxArithmetic.h"

#define AFX_PLANE_DIST (3)

typedef enum afxFloorPlane
// The orientation of the the floor plane with respect to the X,Y,Z axes, 
// and which axes represent the side, front and up vectors as a basis for rotations.
{
    // The floor is oriented along the ZX-plane. The default value.
    // The front vector points towards +Z, the up vector towards +Y, and the side vector towards +X.
    afxFloorPlane_XZ, // the default

    // The floor is oriented along the XY-plane. 
    // The front vector points towards +X, the up vector towards +Z, and the side vector towards +Y.
    afxFloorPlane_XY,

    // The floor is oriented along the YZ-plane. 
    // The front vector points towards +Y, the up vector towards +X, and the side vector towards +Z.
    afxFloorPlane_YZ
} afxFloorPlane;

#define AFX_PLANE(u_, v_, w_, d_) \
    (afxPlane){ .uvwd = { { { (u_), (v_), (w_), (d_) } } } }

AFXINL afxBool      AfxPlaneIsEqual(afxPlane const a, afxPlane const b);
AFXINL afxBool      AfxPlaneIsNearEqual(afxPlane const a, afxPlane const b, afxV4d const epsilon);
AFXINL afxBool      AfxPlaneIsNotEqual(afxPlane const a, afxPlane const b);
AFXINL afxBool      AfxPlaneIsNaN(afxPlane const p);
AFXINL afxBool      AfxPlaneIsInfinite(afxPlane const p);
AFXINL afxReal      AfxPlaneDot(afxPlane const a, afxPlane const b);
AFXINL afxReal      AfxPlaneDotCoord(afxPlane const p, afxV3d const v);
AFXINL afxReal      AfxPlaneDotNormal(afxPlane const p, afxV3d const v);
AFXINL afxPlane     AfxPlaneNormalizeEst(afxPlane p);
AFXINL afxPlane     AfxPlaneNormalize(afxPlane p);
AFXINL afxPlane     AfxPlaneIntersectLine(afxPlane const p, afxV4d const start, afxV4d const end);
AFXINL void         AfxPlaneIntersectPlane(afxPlane const a, afxPlane const b, afxV4d* start, afxV4d* end);
AFXINL afxPlane     AfxPlaneTransform(afxPlane const p, afxM4d const m);
AFXINL afxPlane     AfxPlaneFromPointNormal(afxV4d const point, afxV4d const normal);
AFXINL afxPlane     AfxPlaneFromPoints(afxV4d const a, afxV4d const b, afxV4d const c);

AFXINL afxPlane AfxMakePlane(afxV3d const normal, afxReal dist);

/*
    The AfxMakePlaneFromPointNormal() function constructs a plane from a point on the plane and a normal vector.
    The plane equation is: n * x + d = 0;
    Where: d = - n * p;

    The caller must ensure n is normalized.

    p->uvwd[0..2] = normalized normal
    p->uvwd[3]    = distance term
*/

AFXINL afxPlane AfxMakePlaneFromPointNormal
(
    afxV3d const point, 
    afxV3d const normal
);

/*
    The AfxMakePlaneFromTriangle() function constructs a plane from three points in the form:
        n * x + d = 0;

    Where:
        n is the normalized plane normal
        d is the distance term

    Given points a, b, c, the normal is computed as:
        n = normalize( (b - a) X (c - a) )
    And the plane distance is:
        d = - dot(n, a)
    This is a standard plane-from-triangle computation.
*/

AFXINL afxPlane AfxMakePlaneFromTriangle
(
    afxV3d const a, 
    afxV3d const b, 
    afxV3d const c
);

AFXINL afxV3d       AfxGetPlaneNormal(afxPlane p);

AFXINL afxReal      AfxGetPlaneDistance(afxPlane const p); // aka GetPlaneDistance

AFXINL afxReal      AfxFindPlaneDistance(afxPlane const p, afxV3d const point);

AFXINL afxReal      AfxFindPlaneHitInterpolationConstant(afxPlane const p, afxV3d const a, afxV3d const b);

// Test this plane with an AABB
// <0 if the box is completly on the back side of the plane
// >0 if the box is completly on the front side of the plane
// 0 if the box intersects with the plane
AFXINL afxResult    AfxPlaneTestAabbs(afxPlane const p, afxUnit cnt, afxBox const aabb[]);
AFXINL afxResult    AfxPlaneTestAabb(afxPlane const p, afxBox const aabb);

// Test this plane with a collision sphere.
AFXINL afxResult    AfxPlaneTestSpheres(afxPlane const p, afxUnit cnt, afxSphere const spheres[]);
AFXINL afxResult    AfxPlaneTestSphere(afxPlane const p, afxSphere const sph);

////////////////////////////////////////

AFXINL void         AfxCopyPlanes(afxUnit cnt, afxPlane const src[], afxPlane dst[]);

#endif//AFX_PLANE_H
