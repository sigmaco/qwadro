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

#include "qwadro/coll/afxCapsule.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/coll/afxBox.h"



// Compute Total Height
_AFXINL afxReal AfxGetCapsuleHeight(afxCapsule const cap)
{
    return cap.length + 2.0f * cap.radius;
}

// Compute Endpoints of the Capsule
// Get the Y-aligned end points of the capsule’s internal segment (cylinder)
_AFXINL afxReal AfxGetCapsuleEndpoints(afxCapsule const cap, afxV3d* outA, afxV3d* outB)
{
    afxError err;
    //AFX_ASSERT(cap);
    AFX_ASSERT(outA);
    AFX_ASSERT(outB);

    *outA = cap.origin;
    *outB = cap.origin;
    afxReal seg = cap.length / 2.0f;
    // Endpoint A = origin - offset (bottom hemisphere center)
    // Endpoint B = origin + offset (top hemisphere center)
    outA->v[1] -= seg;
    outB->v[1] += seg;
    return seg;
}

// Compute Axis-Aligned Bounding Box (AABB)
_AFXINL afxAabb AfxGetCapsuleAabb(afxCapsule const cap)
{
    afxError err;
    //AFX_ASSERT(cap);

    afxReal halfHeight = (cap.length / 2.0f) + cap.radius;

    afxAabb bb;

    bb.min.v[0] = cap.origin.v[0] - cap.radius;
    bb.min.v[1] = cap.origin.v[1] - halfHeight;
    bb.min.v[2] = cap.origin.v[2] - cap.radius;

    bb.max.v[0] = cap.origin.v[0] + cap.radius;
    bb.max.v[1] = cap.origin.v[1] + halfHeight;
    bb.max.v[2] = cap.origin.v[2] + cap.radius;
    return bb;
}

// Distance from Point to Capsule
_AFXINL afxReal AfxDistanceFromCapsule(afxCapsule const cap, afxV3d const point)
{
    afxError err;

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, &a, &b);

    // Compute vector projection of point onto line segment
    afxV3d ab = { b.v[0] - a.v[0], b.v[1] - a.v[1], b.v[2] - a.v[2] };
    afxV3d ap = { point.v[0] - a.v[0], point.v[1] - a.v[1], point.v[2] - a.v[2] };

    afxReal ab_len2 = ab.v[0] * ab.v[0] + ab.v[1] * ab.v[1] + ab.v[2] * ab.v[2];
    afxReal t = (ap.v[0] * ab.v[0] + ap.v[1] * ab.v[1] + ap.v[2] * ab.v[2]) / ab_len2;

    // Clamp t to [0,1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    afxV3d closest =
    {
        a.v[0] + ab.v[0] * t,
        a.v[1] + ab.v[1] * t,
        a.v[2] + ab.v[2] * t
    };

    afxReal dx = point.v[0] - closest.v[0];
    afxReal dy = point.v[1] - closest.v[1];
    afxReal dz = point.v[2] - closest.v[2];
    afxReal dist2 = dx * dx + dy * dy + dz * dz;
    afxReal dist = AfxSqrtf(dist2);
    return dist - cap.radius; // Subtract radius for surface distance
}

// Check if a Point is Inside the Capsule
_AFXINL afxBool AfxIsPointInsideCapsule(afxCapsule const cap, afxV3d const point)
{
    afxError err;
    //AFX_ASSERT(cap);
    //AFX_ASSERT(point);

    afxV3d a, b;
    AfxGetCapsuleEndpoints(cap, &a, &b);

    // Vector from A to B
    afxV3d ab = { b.v[0] - a.v[0], b.v[1] - a.v[1], b.v[2] - a.v[2] };
    afxV3d ap = { point.v[0] - a.v[0], point.v[1] - a.v[1], point.v[2] - a.v[2] };

    afxReal ab_len2 = ab.v[0] * ab.v[0] + ab.v[1] * ab.v[1] + ab.v[2] * ab.v[2];
    afxReal t = (ap.v[0] * ab.v[0] + ap.v[1] * ab.v[1] + ap.v[2] * ab.v[2]) / ab_len2;

    // Clamp t to [0,1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Closest point on the segment
    afxV3d closest =
    {
        a.v[0] + ab.v[0] * t,
        a.v[1] + ab.v[1] * t,
        a.v[2] + ab.v[2] * t
    };

    // Distance squared from point to closest point
    afxReal dx = point.v[0] - closest.v[0];
    afxReal dy = point.v[1] - closest.v[1];
    afxReal dz = point.v[2] - closest.v[2];
    afxReal dist2 = dx * dx + dy * dy + dz * dz;

    return dist2 <= (cap.radius * cap.radius);
}

