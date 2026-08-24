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

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
#include "../icd/avxIcd.h"

/*
    Algorithm: Sutherland–Hodgman Clipping (Adapted for homogeneous 4D)
    This is a polygon clipping algorithm extended to work in 4D homogeneous clip space.

    Clip the triangle against each of the 6 clip planes.
    For each plane:
        If all points are inside, then keep it.
        If all points are outside, then discard it.
        If the triangle straddles the plane:
            Interpolate to compute new intersection points (on the plane).
            Create one or two new triangles from the split.
*/

afxBool ClipTestInside(afxV4d v, int plane)
{
    // Core Plane Clipping Function (Example: Clip against x <= w)

    switch (plane)
    {
    case 0: return v.v[0] >= -v.v[3]; // left
    case 1: return v.v[0] <= v.v[3]; // right
    case 2: return v.v[1] >= -v.v[3]; // bottom
    case 3: return v.v[1] <= v.v[3]; // top
    case 4: return v.v[2] >= -v.v[3]; // near
    case 5: return v.v[2] <= v.v[3]; // far
    }
    return FALSE;
}

afxV4d ClipIntersectVertices(afxV4d a, afxV4d b, int plane)
{

    // Interpolating Between Vertices
    // If we have two vertices, v0 and v1, and one is inside while the other is outside, 
    // we find the intersection point on the clip plane using homogeneous interpolation.

    afxReal t;
    afxReal aVal, bVal;

    switch (plane)
    {
    case 0: aVal = a.v[0] + a.v[3]; bVal = b.v[0] + b.v[3]; break; // x >= -w
    case 1: aVal = a.v[3] - a.v[0]; bVal = b.v[3] - b.v[0]; break; // x <= +w
    case 2: aVal = a.v[1] + a.v[3]; bVal = b.v[1] + b.v[3]; break;
    case 3: aVal = a.v[3] - a.v[1]; bVal = b.v[3] - b.v[1]; break;
    case 4: aVal = a.v[2] + a.v[3]; bVal = b.v[2] + b.v[3]; break;
    case 5: aVal = a.v[3] - a.v[2]; bVal = b.v[3] - b.v[2]; break;
    }

    t = aVal / (aVal - bVal);
    return AfxV4dMix(a, b, t); // mix(a, b, t); // Linear interpolation in homogeneous space
}

afxUnit ClipTriangle(afxUnit vtxCnt, afxV4d input[], afxV4d vertices[])
{
    afxUnit rsltCnt = 0;

    for (afxUnit plane = 0; plane < 6; ++plane)
    {
        for (afxUnit i = 0; i < vtxCnt; ++i)
        {
            //afxReal* current = input[i];
            //afxReal* prev = input[(i + vtxCnt - 1) % vtxCnt];

            afxBool currIn = ClipTestInside(/*current*/input[i], plane);
            afxBool prevIn = ClipTestInside(/*prev*/input[(i + vtxCnt - 1) % vtxCnt], plane);

            if (currIn && prevIn)
            {
                vertices[rsltCnt] = /*current*/input[i];
                ++rsltCnt;
            }
            else if (currIn && !prevIn)
            {
                vertices[rsltCnt] = ClipIntersectVertices(/*prev*/input[(i + vtxCnt - 1) % vtxCnt], /*current*/input[i], plane);
                ++rsltCnt;
                vertices[rsltCnt] = /*current*/input[i];
                ++rsltCnt;
            }
            else if (!currIn && prevIn)
            {
                vertices[rsltCnt] = ClipIntersectVertices(/*prev*/input[(i + vtxCnt - 1) % vtxCnt], /*current*/input[i], plane);
                ++rsltCnt;
            }
            // else both out -> discard edge.
        }

        if (rsltCnt < 3) break; // Clipped away completely
    }

    /*
        Clipping is done in 4D clip space to preserve linear interpolation across planes.
        GPU hardware clips automatically, but in custom engines or geometry shaders, this logic matters.
        After clipping, continue as normal: divide by W, transform to NDC, map via viewport

        Output.
        After clipping, the original triangle might turn into:
            1 triangle (if partially clipped)
            2 triangles (if cut across a plane)
            0 triangles (if fully outside the frustum)
        Each output vertex is still a vec4 in clip space, and we'll still perform the perspective divide (v.xyz / v.w) afterward.
    */

    return rsltCnt;
}
