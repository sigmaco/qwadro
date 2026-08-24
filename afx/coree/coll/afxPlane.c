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

#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/math/afxArithmetic2.h"

_AFXINL afxBool AfxPlaneIsEqual(afxPlane a, afxPlane b)
{
    // Should be compatible with XMPlaneEqual

    return (0 == AfxV4dDot(a.uvwd, b.uvwd));
}

_AFXINL afxBool AfxPlaneIsNearEqual(afxPlane const a, afxPlane const b, afxV4d const epsilon)
{
    // Should be compatible with XMPlaneNearEqual

    afxPlane na = AfxPlaneNormalize(a);
    afxPlane nb = AfxPlaneNormalize(b);
    return AfxV4dDot(na.uvwd, nb.uvwd) > (epsilon.v[0] * epsilon.v[0]);
}

_AFXINL afxBool AfxPlaneIsNotEqual(afxPlane const a, afxPlane const b)
{
    // Should be compatible with XMPlaneNotEqual

    return AfxV4dDot(a.uvwd, b.uvwd) != 0;
}

_AFXINL afxBool AfxPlaneIsNaN(afxPlane const p)
{
    // Should be compatible with XMPlaneIsNaN

    for (afxUnit i = 0; i < 4; ++i) if (isnan(p.uvwd.v[i])) return TRUE;
    return FALSE;
}

_AFXINL afxBool AfxPlaneIsInfinite(afxPlane const p)
{
    // Should be compatible with XMPlaneIsInfinite

    for (afxUnit i = 0; i < 4; ++i) if (isinf(p.uvwd.v[i])) return TRUE;
    return FALSE;
}

_AFXINL afxReal AfxPlaneDot(afxPlane const a, afxPlane const b)
{
    // Should be compatible with XMPlaneDot

    return AfxV4dDot(a.uvwd, b.uvwd);
}

_AFXINL afxReal AfxPlaneDotCoord(afxPlane const p, afxV3d const v)
{
    // Should be compatible with XMPlaneDotCoord

    // Mask the W component to 1.
    return AfxPlaneDot(p, AFX_PLANE(v.v[0], v.v[1], v.v[2], 1));
}

_AFXINL afxReal AfxPlaneDotNormal(afxPlane p, afxV3d const v)
{
    // Should be compatible with XMPlaneDotNormal

    return AfxV3dDot(p.uvwd.v3, v);
}

_AFXINL afxPlane AfxPlaneNormalizeEst(afxPlane p)
{
    // Should be compatible with XMPlaneNormalizeEst

    afxPlane n;
    afxReal rlen = AfxV3dMagRecip(p.uvwd.v3);
    n.uvwd = AfxV4dScale(p.uvwd, rlen);    
    return n;
}

_AFXINL afxPlane AfxPlaneNormalize(afxPlane p)
{
    // Should be compatible with XMPlaneNormalize

    afxPlane n;
    afxReal len = AfxV3dMag(p.uvwd.v3);
    if (len > 0) len = 1.f / len;
    n.uvwd = AfxV4dScale(p.uvwd, len);
    return n;
}

_AFXINL afxPlane AfxPlaneIntersectLine(afxPlane p, afxV4d const start, afxV4d const end)
{
    // Should be compatible with XMPlaneIntersectLine

    afxReal dot1 = AfxV3dDot(p.uvwd.v3, start.v3);
    afxReal dot2 = AfxV3dDot(p.uvwd.v3, end.v3);
    afxReal d = dot1 - dot2;
    afxReal vt = AfxPlaneDotCoord(p, start.v3);
    vt /= d;

    afxV4d pnt = AfxV4dSub(end, start);
    pnt = AfxV4dMad(start, pnt, AFX_V4D(vt, vt, vt, vt));
    p.uvwd = pnt;
    return p;
}

_AFXINL void AfxPlaneIntersectPlane(afxPlane const a, afxPlane const b, afxV4d* start, afxV4d* end)
{
    // Plane-plane intersection function
    // Should be compatible with XMPlaneIntersectPlane

    // Compute the cross product of P2 and P1
    afxV4d V1;
    V1.v3 = AfxV3dCross(b.uvwd.v3, a.uvwd.v3);
    V1.w = 0;

    // Compute the squared length of V1
    afxReal lenSq = AfxV3dSq(V1.v3);

    // Compute the cross product of P2 and V1
    afxV4d V2;
    V2.v3 = AfxV3dCross(b.uvwd.v3, V1.v3);
    V2.w = 0;

    // Multiply V2 by the W component of P1
    afxV4d Point = AfxV4dMultiply(V2, AFX_V4D(a.uvwd.v[3], a.uvwd.v[3], a.uvwd.v[3], a.uvwd.v[3]));

    // Compute the cross product of V1 and P1
    afxV4d V3;
    V3.v3 = AfxV3dCross(V1.v3, a.uvwd.v3);
    V3.w = 0;

    // Multiply V3 by P2W and add to Point
    Point = AfxV4dMad(Point, V3, AFX_V4D(b.uvwd.v[3], b.uvwd.v[3], b.uvwd.v[3], b.uvwd.v[3]));

    // Divide Point by LengthSq to get LinePoint1
    afxV4d LinePoint1 = AfxV4dDivs(Point, lenSq);

    // Add V1 to LinePoint1 to get LinePoint2
    afxV4d LinePoint2 = AfxV4dAdd(LinePoint1, V1);

    *start = AfxV4dClamp(LinePoint1, AFX_V4D_EPSILON, LinePoint1);
    *end = AfxV4dClamp(LinePoint2, AFX_V4D_EPSILON, LinePoint2);
}

_AFXINL afxPlane AfxPlaneTransform(afxPlane p, afxM4d const m)
{
    // Should be compatible with XMPlaneTransform

    afxPlane t;
    afxV4d a = AfxV4dMultiply(AFX_V4D(p.uvwd.v[3], p.uvwd.v[3], p.uvwd.v[3], p.uvwd.v[3]), m.v4[3]);
    afxV4d b = AfxV4dMad(a, AFX_V4D(p.uvwd.v[2], p.uvwd.v[2], p.uvwd.v[2], p.uvwd.v[2]), m.v4[2]);
    a = AfxV4dMad(b, AFX_V4D(p.uvwd.v[1], p.uvwd.v[1], p.uvwd.v[1], p.uvwd.v[1]), m.v4[1]);
    t.uvwd = AfxV4dMad(a, AFX_V4D(p.uvwd.v[0], p.uvwd.v[0], p.uvwd.v[0], p.uvwd.v[0]), m.v4[0]);
    return t;
}

_AFXINL afxPlane AfxPlaneFromPointNormal(afxV4d const point, afxV4d const normal)
{
    // Should be compatible with XMPlaneFromPointNormal

    afxReal dot = AfxV4dDot(point, normal);
    dot = -dot;
    return AFX_PLANE(dot, dot, dot, dot);
}

_AFXINL afxPlane AfxPlaneFromPoints(afxV4d const a, afxV4d const b, afxV4d const c)
{
    // Should be compatible with XMPlaneFromPoints

    afxV4d D;
    afxV4d ba = AfxV4dSub(a, b);
    afxV4d ca = AfxV4dSub(a, c);
    afxV4d n;
    n.v3 = AfxV3dCross(ba.v3, ca.v3);
    n.v3 = AfxV3dNormalize(n.v3, NIL);
    
    afxPlane p = AFX_PLANE(n.v[0], n.v[1], n.v[2], 0);
    afxReal d = AfxPlaneDotNormal(p, a.v3);
    d *= -1;
    p.uvwd.v[AFX_PLANE_DIST] = d;
}






////////////////////////////////////////////////////////////////

/*
    The AfxMakePlane() sets a normalized plane from a given normal vector (normal) and a distance value (dist).
*/

_AFXINL afxPlane AfxMakePlane(afxV3d const normal, afxReal dist)
{
    afxError err = { 0 };
    return AfxPlaneNormalize(AFX_PLANE(normal.v[0], normal.v[1], normal.v[2], dist));
}

/*
    The AfxMakePlaneFromPointNormal() function constructs a plane from a point on the plane and a normal vector.
    The plane equation is: n * x + d = 0;
    Where: d = - n * p;
    
    The caller must ensure n is normalized.

    p.uvwd[0..2] = normalized normal
    p.uvwd[3]    = distance term
*/

_AFXINL afxPlane AfxMakePlaneFromPointNormal(const afxV3d point, const afxV3d normal)
{
    afxError err = { 0 };

    afxPlane p = { 0 };

#ifdef _AFX_PLANE_CHECK_DEGENERACY
    if (AfxV3dSq(normal) < 1e-12f)
    {
        p.uvwd[0] = p.uvwd[1] = p.uvwd[2] = 0.0f;
        p.uvwd[3] = 0.0f;
        return;
    }
#else
    // Normalize the input normal
    p.uvwd.v3 = AfxV3dNormalize(normal, NIL);
#endif

    // d = -dot(n, point)
    p.uvwd.v[AFX_PLANE_DIST] = -AfxV3dDot(p.uvwd.v3, point);
}

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

_AFXINL afxPlane AfxMakePlaneFromTriangle(afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxError err = { 0 };

    afxPlane p = { 0 };

    afxV3d ba = AfxV3dSub(b, a);
    afxV3d ca = AfxV3dSub(c, a);

    // Compute normal
    p.uvwd.v3 = AfxV3dCross(ba, ca);

#ifndef _AFX_PLANE_DONT_CHECK_DEGENERACY
    // Check degeneracy (optional)
    if (AfxV3dSq(p.uvwd.v3) < 1e-12f)
    {
        // Default normal (0,0,0) and distance = 0
        p.uvwd.v[0] = p.uvwd.v[1] = p.uvwd.v[2] = 0.0f;
        p.uvwd.v[3] = 0.0f;
        return p;
    }
#endif

    // Normalize
    p.uvwd.v3 = AfxV3dNormalize(p.uvwd.v3, NIL);

    // Distance term
    p.uvwd.v[AFX_PLANE_DIST] = -AfxV3dDot(p.uvwd.v3, a);
    return p;
}

_AFXINL afxV3d AfxGetPlaneNormal(afxPlane const p)
{
    afxError err = { 0 };
    return p.uvwd.v3;
}

_AFXINL afxReal AfxGetPlaneDistance(afxPlane const p)
{
    afxError err = { 0 };
    return p.uvwd.v[AFX_PLANE_DIST];
}

_AFXINL afxReal AfxFindPlaneDistance(afxPlane const p, afxV3d const point)
{
    afxError err = { 0 };
    // Returns whether a point is in front of the plane.
    return AfxV3dDot(p.uvwd.v3, point) + p.uvwd.v[AFX_PLANE_DIST];
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const p, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    afxV3d t = AfxV3dSub(a, b);
    return (AfxFindPlaneDistance(p, a)) / AfxV3dDot(p.uvwd.v3, t);
}

_AFXINL afxBool AfxPlaneTestSphere(afxPlane const p, afxSphere const sph)
{
    afxError err = { 0 };

    if (AfxV3dDot(sph.xyzr.v3, p.uvwd.v3) + p.uvwd.v[AFX_PLANE_DIST] <= -sph.xyzr.v[AFX_SPHERE_RADIUS])
        return FALSE;

    return TRUE;
}

_AFXINL afxBool AfxPlaneTestSpheres(afxPlane const p, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = { 0 };
    AFX_ASSERT(spheres);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSphere const s = spheres[i];

        if (AfxV3dDot(s.xyzr.v3, p.uvwd.v3) + p.uvwd.v[AFX_PLANE_DIST] <= -s.xyzr.v[AFX_SPHERE_RADIUS])
            return FALSE;
    }
    return TRUE;
}

_AFXINL afxResult AfxPlaneTestAabb(afxPlane const p, afxBox const aabb)
{
    afxError err = { 0 };

    afxReal minD;
    afxReal maxD;

    if (p.uvwd.v[0] > 0.0f)
    {
        minD = p.uvwd.v[0] * aabb.min.v[0];
        maxD = p.uvwd.v[0] * aabb.max.v[0];
    }
    else
    {
        minD = p.uvwd.v[0] * aabb.max.v[0];
        maxD = p.uvwd.v[0] * aabb.min.v[0];
    }

    if (p.uvwd.v[1] > 0.0f)
    {
        minD += p.uvwd.v[1] * aabb.min.v[1];
        maxD += p.uvwd.v[1] * aabb.max.v[1];
    }
    else
    {
        minD += p.uvwd.v[1] * aabb.max.v[1];;
        maxD += p.uvwd.v[1] * aabb.min.v[1];
    }

    if (p.uvwd.v[2] > 0.0f)
    {
        minD += p.uvwd.v[2] * aabb.min.v[2];
        maxD += p.uvwd.v[2] * aabb.max.v[2];
    }
    else
    {
        minD += p.uvwd.v[2] * aabb.max.v[2];
        maxD += p.uvwd.v[2] * aabb.min.v[2];
    }

    // What side of the plane

    if (minD >= -p.uvwd.v[AFX_PLANE_DIST])
        return +1;

    if (maxD <= -p.uvwd.v[AFX_PLANE_DIST])
        return -1;

    //Intersection
    return 0;
}

_AFXINL afxResult AfxPlaneTestAabbs(afxPlane const p, afxUnit cnt, afxBox const aabb[])
{
    afxError err = { 0 };
    AFX_ASSERT(aabb);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        if (p.uvwd.v[0] > 0.0f)
        {
            minD = p.uvwd.v[0] * aabb[i].min.v[0];
            maxD = p.uvwd.v[0] * aabb[i].max.v[0];
        }
        else
        {
            minD = p.uvwd.v[0] * aabb[i].max.v[0];
            maxD = p.uvwd.v[0] * aabb[i].min.v[0];
        }

        if (p.uvwd.v[1] > 0.0f)
        {
            minD += p.uvwd.v[1] * aabb[i].min.v[1];
            maxD += p.uvwd.v[1] * aabb[i].max.v[1];
        }
        else
        {
            minD += p.uvwd.v[1] * aabb[i].max.v[1];;
            maxD += p.uvwd.v[1] * aabb[i].min.v[1];
        }

        if (p.uvwd.v[2] > 0.0f)
        {
            minD += p.uvwd.v[2] * aabb[i].min.v[2];
            maxD += p.uvwd.v[2] * aabb[i].max.v[2];
        }
        else
        {
            minD += p.uvwd.v[2] * aabb[i].max.v[2];
            maxD += p.uvwd.v[2] * aabb[i].min.v[2];
        }

        // What side of the plane

        if (minD >= -p.uvwd.v[AFX_PLANE_DIST])
            return +1;

        if (maxD <= -p.uvwd.v[AFX_PLANE_DIST])
            return -1;
    }
    //Intersection
    return 0;
}

_AFXINL afxBool AfxIntersectPlanes(afxPlane const  p1, afxPlane const p2, afxPlane const p3, afxV3d* v)
// Function to solve the system of three plane equations using Cramer's rule
{
    float det, detX, detY, detZ;

    // Matrix of the coefficients of the planes
    afxM3d mat =
    {
        .m = { { p1.uvwd.v[0], p1.uvwd.v[1], p1.uvwd.v[2] },
          { p2.uvwd.v[0], p2.uvwd.v[1], p2.uvwd.v[2] },
          { p3.uvwd.v[0], p3.uvwd.v[1], p3.uvwd.v[2] } }
    };

    // Determinant of the coefficient matrix
    det =   mat.m[0][0] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
            mat.m[0][1] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) +
            mat.m[0][2] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]);

    if (fabs(det) < 1e-6)
    {
        // If the determinant is zero, the planes are parallel (no intersection)
        *v = AfxV3dMake(0.0f, 0.0f, 0.0f);
        return FALSE;
    }

    // Determinant for x, y, and z
    detX =-(p1.uvwd.v[AFX_PLANE_DIST] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
            p2.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][1] * mat.m[2][2] - mat.m[0][2] * mat.m[2][1]) +
            p3.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1]));

    detY =-(p1.uvwd.v[AFX_PLANE_DIST] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) -
            p2.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0]) +
            p3.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][0] * mat.m[1][2] - mat.m[0][2] * mat.m[1][0]));

    detZ =-(p1.uvwd.v[AFX_PLANE_DIST] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) -
            p2.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][0] * mat.m[2][1] - mat.m[0][1] * mat.m[2][0]) +
            p3.uvwd.v[AFX_PLANE_DIST] * (mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]));

    // Return the intersection point (x, y, z)
    *v = AfxV3dMake(detX / det, detY / det, detZ / det);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxCopyPlanes(afxUnit cnt, afxPlane const src[], afxPlane dst[])
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        dst[i].uvwd.v3 = src[i].uvwd.v3;
        dst[i].uvwd.v[AFX_PLANE_DIST] = src[i].uvwd.v[AFX_PLANE_DIST];
    }
}
