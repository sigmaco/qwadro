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

#include "qwadro/coll/afxSphere.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/math/afxArithmetic2.h"

_AFXINL afxSphere AfxMakeSphere(afxV3d const centre, afxReal radius)
{
    afxError err = { 0 };
    afxSphere sph;
    sph.xyzr = AFX_V4D(centre.v[0], centre.v[1], centre.v[2], radius);
    return sph;
}

_AFXINL afxReal AfxGetSphereRadius(afxSphere const sph)
{
    afxError err = { 0 };
    //AFX_ASSERT(sph);
    return sph.xyzr.v[AFX_SPHERE_RADIUS];
}

_AFXINL afxV4d AfxGetSphereOrigin(afxSphere const sph)
{
    afxError err = { 0 };
    return AFX_V4D(sph.xyzr.v[0], sph.xyzr.v[1], sph.xyzr.v[2], 1);
}

_AFXINL afxBox AfxGetSphereAabb(afxSphere const sph)
{
    afxError err = { 0 };
    // Calculate the AABB for this sphere
    afxReal r = sph.xyzr.v[AFX_SPHERE_RADIUS];
    return AFX_AABB(sph.xyzr.v[0] - r, sph.xyzr.v[1] - r, sph.xyzr.v[2] - r,
                    sph.xyzr.v[0] + r, sph.xyzr.v[1] + r, sph.xyzr.v[2] + r);
}

/*
    The AfxDoesSphereIntersects() function tests whether a single sphere intersects with an array of spheres.
    It implement a function that checks for each sphere in the array if it intersects with the target sphere.
    The intersection test between two spheres is straightforward: two spheres intersect if the distance between
    their centers is less than or equal to the sum of their radii.
*/

_AFXINL afxUnit AfxSphereIntersect(afxSphere const sph, afxSphere const b)
{
    afxError err = { 0 };

    afxReal distSquared = AfxV3dDist(sph.xyzr.v3, b.xyzr.v3);
    afxReal radiusSum = sph.xyzr.v[AFX_SPHERE_RADIUS] + b.xyzr.v[AFX_SPHERE_RADIUS];

    // Compare the squared distance with the squared sum of the radii to avoid square root
    if (distSquared <= radiusSum * radiusSum)
    {
        // Return i if any sphere intersects with the target sphere
        return 1;
    }
    return 0;  // Return 0 if no sphere intersects with the target sphere
}

_AFXINL afxUnit AfxSphereIntersectsAny(afxSphere const sph, afxUnit cnt, afxSphere const others[])
{
    afxError err = { 0 };
    AFX_ASSERT(!cnt || others);

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxSphere const b = others[i];

        afxReal distSquared = AfxV3dDist(sph.xyzr.v3, b.xyzr.v3);
        afxReal radiusSum = sph.xyzr.v[AFX_SPHERE_RADIUS] + b.xyzr.v[AFX_SPHERE_RADIUS];

        // Compare the squared distance with the squared sum of the radii to avoid square root
        if (distSquared <= radiusSum * radiusSum)
        {
            // Return i if any sphere intersects with the target sphere
            return i;
        }
    }
    return AFX_INVALID_INDEX;  // Return 0 if no sphere intersects with the target sphere
}

/*
    The AfxDoesSphereIntersectsAabbs() function tests whether a sphere intersects with an array of axis-aligned boxes.
    It need to perform collision detection between the box and the sphere. This involves checking if the sphere's
    center is inside the box or if the distance from the sphere's center to the closest point on the box is less than
    or equal to the radius of the sphere.

    Returns the index for the first collided box in array, else AFX_INVALID_INDEX for none of them.
*/

_AFXINL afxUnit AfxSphereIntersectsAabbs(afxSphere const sph, afxUnit cnt, afxBox const boxes[])
{
    afxError err = { 0 };
    AFX_ASSERT(!cnt || boxes);

    afxReal r2 = sph.xyzr.v[AFX_SPHERE_RADIUS] * sph.xyzr.v[AFX_SPHERE_RADIUS];

    for (afxUnit i = 0; i < cnt; ++i)
    {
        afxBox const box = boxes[i];

        // Find the closest point on the box to the sphere's center
        afxV3d closest = AfxV3dMin(sph.xyzr.v3, box.max.v3);
        closest = AfxV3dMax(box.min.v3, closest);
        // Calculate the squared distance from the sphere's center to the closest point
        afxReal distSquared = AfxV3dDist(sph.xyzr.v3, closest);

        // Check if the distance is less than or equal to the squared radius
        if (distSquared <= r2)
        {
            return i;
        }
    }
    // Return 0 if no box intersects with the sphere
    return AFX_INVALID_INDEX;
}

_AFXINL afxUnit AfxSphereIntersectsAabb(afxSphere const sph, afxBox const box)
{
    afxError err = { 0 };

    afxReal r2 = sph.xyzr.v[AFX_SPHERE_RADIUS] * sph.xyzr.v[AFX_SPHERE_RADIUS];

    // Find the closest point on the box to the sphere's center
    afxV3d closest = AfxV3dMin(sph.xyzr.v3, box.max.v3);
    closest = AfxV3dMax(box.min.v3, closest);
    // Calculate the squared distance from the sphere's center to the closest point
    afxReal distSquared = AfxV3dDist(sph.xyzr.v3, closest);

    // Check if the distance is less than or equal to the squared radius
    if (distSquared <= r2)
    {
        return 1;
    }
    // Return 0 if no box intersects with the sphere
    return 0;
}

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
_AFXINL afxInt AfxTestSphereEnglobingPoint(afxSphere const sph, afxV3d const point)
{
    afxError err = { 0 };

    afxV3d s = AfxV3dSub(point, sph.xyzr.v3);
    afxReal distSq = AfxV3dDot(s, s);
    afxReal radiusSq = sph.xyzr.v[AFX_SPHERE_RADIUS] * sph.xyzr.v[AFX_SPHERE_RADIUS];

    if (distSq < radiusSq)
        return 1; // Inside the sphere
    else if (fabs(distSq - radiusSq) < 1e-9)
        return 0; // On the sphere surface (within a tiny margin)
    else
        return -1; // Outside the sphere
}

// Tests whether a given point is inside, on, or outside a sphere. 
// The function compares the squared distance between the point and the center of the sphere with the square of the radius.
// Returns: -1 = outside, 0 = on surface, 1 = inside
_AFXINL afxInt AfxTestSphereEnglobingPoints(afxSphere const sph, afxBool all, afxBool fully, afxUnit cnt, afxV3d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(points);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d s = AfxV3dSub(points[i], sph.xyzr.v3);
        afxReal distSq = AfxV3dDot(s, s);
        afxReal radiusSq = sph.xyzr.v[AFX_SPHERE_RADIUS] * sph.xyzr.v[AFX_SPHERE_RADIUS];

        if (distSq < radiusSq)
        {
            //return 1; // Inside the sphere
        }
        else if (AFX_ABS(distSq - radiusSq) < 1e-9)
        {
            if (fully && all)
                return 0; // On the sphere surface (within a tiny margin)
        }
        else
        {
            if (all)
                return -1; // Outside the sphere
        }
    }
    return 1;
}
