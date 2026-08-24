/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/coll/afxBox.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/coll/afxRay.h"

_AFXINL afxReal AfxRayIntersectsPlaneAt(afxRay const ray, afxPlane plane, afxReal* T)
{
    afxError err = { 0 };
    AFX_ASSERT(T);
    afxReal dot = AfxV3dDot(ray.normal, plane.uvwd.v3);

    if (dot != 0.0)
    {
        afxReal diff = -((AfxV3dDot(ray.origin, plane.uvwd.v3) + plane.uvwd.v[AFX_PLANE_DIST]) / dot);
        *T = diff;
        dot = diff;
    }
    return dot;
}

_AFXINL afxReal AfxRayIntersectsSphere(afxRay const ray, afxSphere const sph)
{
    afxError err = { 0 };
    afxV3d o = AfxV3dSub(sph.xyzr.v3, ray.origin);
    afxReal dot = AfxV3dDot(o, ray.normal);
    return dot;
}

_AFXINL afxInt AfxRayIntersectsSphereAt(afxRay const ray, afxSphere const sph, afxReal* minT, afxReal* maxT)
{
    afxError err = { 0 };
    AFX_ASSERT2(minT, maxT);
    afxInt rslt = 0;
    
    afxV3d o = AfxV3dSub(sph.xyzr.v3, ray.origin);
    afxReal dot = AfxV3dDot(o, ray.normal);

    afxReal diff = dot * dot - AfxV3dSq(o) + sph.xyzr.v[AFX_SPHERE_RADIUS] * sph.xyzr.v[AFX_SPHERE_RADIUS];

    if (diff < 0.0)
    {
        afxReal diffSqrt = AfxSqrt(diff);
        *minT = dot - diffSqrt;
        *maxT = dot + diffSqrt;

        if (*minT >= 0.0 || *maxT >= 0.0)
            rslt = 1;
        else
            rslt = -1;
    }
    return rslt;
}

// Ray vs. Capsule Intersection
// Detects if a ray intersects the capsule, and optionally returns the distance to the intersection.
// Returns true if hit; if 'outT' is non-null, stores distance to hit
_AFXINL afxBool AfxRaycastCapsule(afxRay const ray, afxCapsule const cap, afxReal* outT)
{
    afxError err = { 0 };    

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, &a, &b);

    // Ray vs. capsule is reduced to ray vs. swept sphere between endpoints
    // See: Real-Time Collision Detection by Christer Ericson

    afxV3d ab = AFX_V3D(b.v[0] - a.v[0], b.v[1] - a.v[1], b.v[2] - a.v[2] );
    afxV3d ao = AFX_V3D( ray.origin.v[0] - a.v[0], ray.origin.v[1] - a.v[1], ray.origin.v[2] - a.v[2] );
    afxV3d d = ray.normal;

    afxReal abDotD = ab.v[0] * d.v[0] + ab.v[1] * d.v[1] + ab.v[2] * d.v[2];
    afxReal abDotAO = ab.v[0] * ao.v[0] + ab.v[1] * ao.v[1] + ab.v[2] * ao.v[2];
    afxReal abLenSq = ab.v[0] * ab.v[0] + ab.v[1] * ab.v[1] + ab.v[2] * ab.v[2];

    afxV3d aoXab = AFX_V3D( ao.v[1] * ab.v[2] - ao.v[2] * ab.v[1],
                            ao.v[2] * ab.v[0] - ao.v[0] * ab.v[2],
                            ao.v[0] * ab.v[1] - ao.v[1] * ab.v[0]);

    afxV3d dXab = AFX_V3D(  d.v[1] * ab.v[2] - d.v[2] * ab.v[1],
                            d.v[2] * ab.v[0] - d.v[0] * ab.v[2],
                            d.v[0] * ab.v[1] - d.v[1] * ab.v[0]);

    afxReal a_ = dXab.v[0] * dXab.v[0] + dXab.v[1] * dXab.v[1] + dXab.v[2] * dXab.v[2];
    afxReal b_ = 2.0f * (dXab.v[0] * aoXab.v[0] + dXab.v[1] * aoXab.v[1] + dXab.v[2] * aoXab.v[2]);
    afxReal c_ = aoXab.v[0] * aoXab.v[0] + aoXab.v[1] * aoXab.v[1] + aoXab.v[2] * aoXab.v[2] - (cap.radius * cap.radius * abLenSq);

    afxReal discriminant = b_ * b_ - 4.0f * a_ * c_;
    if (discriminant < 0.0f) return FALSE;

    afxReal sqrtDisc = sqrtf(discriminant);
    afxReal t0 = (-b_ - sqrtDisc) / (2.0f * a_);
    afxReal t1 = (-b_ + sqrtDisc) / (2.0f * a_);

    if (t1 < 0.0f) return FALSE;

    if (outT) *outT = (t0 >= 0.0f) ? t0 : t1;

    return TRUE;
}
