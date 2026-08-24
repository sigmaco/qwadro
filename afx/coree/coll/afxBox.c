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
#include "qwadro/coll/afxSphere.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/afxMemory.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"

_AFXINL afxBox AfxMakeAabb(afxUnit cnt, afxV3d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(!cnt || points);

    // Initialize the AABB with extreme values
    afxBox bb =
    {
        .min = AFX_V4D_MAX,
        .max = AFX_V4D_MIN
    };
    
    if (cnt)
    {
        // Add the points regularly.
        bb = AfxEmboxVectors(bb, cnt, points);
    }
    return bb;
}

_AFXINL afxV3d AfxGetAabbExtents(afxBox const bb, afxBool* isValid)
{
    afxError err = { 0 };
    // Calculate the bounding box extent: (max - min).
    afxV3d extent = AfxV3dSub(bb.max.v3, bb.min.v3);
    // Multiply by 0.5 to get half the extent (center-to-corner) if so desired.
    if (isValid) *isValid = !!(AfxV3dSum(extent));
    return extent;
}

_AFXINL afxV4d AfxGetAabbCentre(afxBox const bb, afxReal* halfSiz)
{
    afxError err = { 0 };
    // (min + max) / 2
    // 0.5 * (min + max)
    afxV4d centre;
    centre.v[0] = (bb.min.v[0] + bb.max.v[0]) * 0.5;
    centre.v[1] = (bb.min.v[1] + bb.max.v[1]) * 0.5;
    centre.v[2] = (bb.min.v[2] + bb.max.v[2]) * 0.5;
    centre.v[3] = 1.f;
    if (halfSiz) *halfSiz = fmaxf(fmaxf((bb.max.v[0] - bb.min.v[0]) * 0.5, (bb.max.v[1] - bb.min.v[1]) * 0.5), (bb.max.v[2] - bb.min.v[2]) * 0.5);
    return centre;
}

_AFXINL void AfxGetAabbCorners(afxBox const bb, afxV3d vertices[AFX_NUM_BOX_CORNERS])
// Generate vertices from box corners.
{
    // The vertices are defined as 8 unique corner points.

    afxError err = { 0 };
    AFX_ASSERT(vertices);

    // Initialize the 8 unique vertices (the 8 corners of the cuboid)
    vertices[0] = AfxV3dMake(bb.min.v[0], bb.min.v[1], bb.min.v[2]); // Front-left bottom
    vertices[1] = AfxV3dMake(bb.max.v[0], bb.min.v[1], bb.min.v[2]); // Front-right bottom
    vertices[2] = AfxV3dMake(bb.max.v[0], bb.min.v[1], bb.max.v[2]); // Back-right bottom
    vertices[3] = AfxV3dMake(bb.min.v[0], bb.min.v[1], bb.max.v[2]); // Back-left bottom
    vertices[4] = AfxV3dMake(bb.min.v[0], bb.max.v[1], bb.min.v[2]); // Front-left top
    vertices[5] = AfxV3dMake(bb.max.v[0], bb.max.v[1], bb.min.v[2]); // Front-right top
    vertices[6] = AfxV3dMake(bb.max.v[0], bb.max.v[1], bb.max.v[2]); // Back-right top
    vertices[7] = AfxV3dMake(bb.min.v[0], bb.max.v[1], bb.max.v[2]); // Back-left top
}

_AFXINL afxBox AfxEmboxTriangles(afxBox bb, afxUnit triCnt, afxV3d const vertices[], afxUnit const indices[], afxUnit idxStride)
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of vertices, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates. 
    // These minimum and maximum values will define the corners of the AABB.

    if (!indices)
    {
        // Loop through the points and adjust the min and max values
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit ia = i * 3 + 0;
            afxUnit ib = i * 3 + 1;
            afxUnit ic = i * 3 + 2;
            AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ia]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ia]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ib]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ib]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ic]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ic]);
        }
        return bb;
    }

    if (idxStride == 4)
    {
        afxUnit32 const* indices2 = (void*)indices;

        // Loop through the points and adjust the min and max values
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit ia = indices2[i * 3 + 0];
            afxUnit ib = indices2[i * 3 + 1];
            afxUnit ic = indices2[i * 3 + 2];
            AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ia]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ia]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ib]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ib]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ic]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ic]);
        }
        return bb;
    }
    
    if (idxStride == 2)
    {
        afxUnit16 const* indices2 = (void*)indices;

        // Loop through the points and adjust the min and max values
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit ia = indices2[i * 3 + 0];
            afxUnit ib = indices2[i * 3 + 1];
            afxUnit ic = indices2[i * 3 + 2];
            AFX_ASSERT_TRIANGLE_BOUNDS(ia, ib, ic, triCnt);

            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ia]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ia]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ib]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ib]);
            bb.min.v3 = AfxV3dMin(bb.min.v3, vertices[ic]);
            bb.max.v3 = AfxV3dMax(bb.max.v3, vertices[ic]);
        }
        return bb;
    }

    AfxThrowError();
    return bb;
}

_AFXINL afxBox AfxEmboxVectors(afxBox bb, afxUnit cnt, afxV3d const vectors[])
{
    afxError err = { 0 };
    AFX_ASSERT(vectors);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of points, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates (assuming you're working with 3D points). 
    // These minimum and maximum values will define the corners of the AABB.

    // Loop through the points and adjust the min and max values
    for (afxUnit i = 0; i < cnt; i++)
    {
        bb.min.v3 = AfxV3dMin(bb.min.v3, vectors[i]);
        bb.max.v3 = AfxV3dMax(bb.max.v3, vectors[i]);
    }
    return bb;
}

_AFXINL afxBox AfxEmboxPoints(afxBox bb, afxUnit cnt, afxV4d const points[])
{
    afxError err = { 0 };
    AFX_ASSERT(points);

    // To generate an Axis-Aligned Bounding Box (AABB) from an array of points, you'll need to calculate the minimum 
    // and maximum values for the x, y, and z coordinates (assuming you're working with 3D points). 
    // These minimum and maximum values will define the corners of the AABB.

    // Loop through the points and adjust the min and max values
    for (afxUnit i = 0; i < cnt; i++)
    {
        bb.min.v3 = AfxV3dMin(bb.min.v3, points[i].v3);
        bb.max.v3 = AfxV3dMax(bb.max.v3, points[i].v3);
    }
    return bb;
}

_AFXINL afxBox AfxEmboxAabbs(afxBox bb, afxUnit cnt, afxBox const boxes[])
{
    afxError err = { 0 };
    AFX_ASSERT(boxes);

    // To compute the Axis-Aligned Bounding Box(AABB) for an array of AABBs, the process is straightforward. 
    // An AABB is already defined by its minimum and maximum corners (i.e., two 3D points, one for min and one for max), 
    // so you simply need to find the overall minimum and maximum x, y, and z values across all the AABBs in the array.

    AFX_STATIC_ASSERT(offsetof(afxBox, min) == 0, "");

    for (afxUnit i = 0; i < cnt; i++)
    {
        bb.min.v3 = AfxV3dMin(bb.min.v3, boxes[i].min.v3);
        bb.max.v3 = AfxV3dMax(bb.max.v3, boxes[i].max.v3);
    }
    return bb;
}

_AFXINL afxBox AfxEmboxSpheres(afxBox bb, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = { 0 };
    AFX_ASSERT(spheres);

    // To compute the Axis-Aligned Bounding Box (AABB) for an array of spheres, the concept is similar to computing the AABB for an array of points. 
    // However, a sphere has both a center (point in 3D space) and a radius, so you'll need to account for the radius when computing the AABB.

    // Loop through each sphere and adjust the min and max values of the AABB
    for (afxUnit i = 0; i < cnt; i++)
    {
        bb = AfxEmboxAabbs(bb, 1, (afxBox[]) { AfxGetSphereAabb(spheres[i]) });
    }
    bb.max.v[3] = (bb.min.v[3] = AFX_REAL(1));
    return bb;
}

_AFXINL afxUnit AfxGenerateIndexedLinesForAabbs(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][AFX_NUM_BOX_CORNERS], afxUnit vtxStride, afxUnit indices[][AFX_NUM_BOX_EDGE_VERTICES], afxUnit idxSiz)
// Generate an Indexed Line List for AABBs.
// A line list means we're rendering edges, so each line connects 2 vertices, and we use pairs of indices to define each line segment.
// For a cube (AABB), there are 12 edges, so we need 24 indices per AABB.
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    // vertices[] array must be capable of storing @cnt * 8 vertices.
    AFX_ASSERT(indices);
    // indices[] array must be capable of stroring @cnt * 24 indices.
    
    //if (outIdxCnt) *outIdxCnt = cnt * 24;
    //if (outVtxCnt) *outVtxCnt = cnt * 8;

    afxUnit rslt = cnt * AFX_NUM_BOX_EDGES;

    static afxUnit const cubeLineIndices[AFX_NUM_BOX_EDGE_VERTICES] =
    {
        // Bottom edges
        0, 1,  1, 3,  3, 2,  2, 0,
        // Top edges
        4, 5,  5, 7,  7, 6,  6, 4,
        // Vertical edges
        0, 4,  1, 5,  2, 6,  3, 7
    };

    for (afxUnit bbIdx = 0; bbIdx < cnt; ++bbIdx)
    {
        afxBox const* box = &aabbs[bbIdx];
        afxReal xmin = box->min.v[0], ymin = box->min.v[1], zmin = box->min.v[2];
        afxReal xmax = box->max.v[0], ymax = box->max.v[1], zmax = box->max.v[2];
        afxV3d const corners[AFX_NUM_BOX_CORNERS] =
        {
            AFX_V3D(xmin, ymin, zmin), // 0
            AFX_V3D(xmax, ymin, zmin), // 1
            AFX_V3D(xmin, ymax, zmin), // 2
            AFX_V3D(xmax, ymax, zmin), // 3
            AFX_V3D(xmin, ymin, zmax), // 4
            AFX_V3D(xmax, ymin, zmax), // 5
            AFX_V3D(xmin, ymax, zmax), // 6
            AFX_V3D(xmax, ymax, zmax)  // 7
        };

        // vertices[bbIdx * 8 + j], corners[j]
        for (afxUnit j = 0; j < AFX_NUM_BOX_CORNERS; ++j)
        {
            vertices[bbIdx][j] = corners[j];
        }

        // Copy line indices (offset by current AABB vertex index base)
        // indices[bbIdx * 24 + j] = cubeLineIndices[j] + bbIdx * 8;

        if (sizeof(afxUnit16) == idxSiz)
        {
            afxUnit16* indices16 = (afxUnit16*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices16[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else if (sizeof(afxUnit32) == idxSiz)
        {
            afxUnit32* indices32 = (afxUnit32*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices32[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else if (sizeof(afxUnit8) == idxSiz)
        {
            afxUnit8* indices8 = (afxUnit8*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_EDGE_VERTICES; ++j)
                indices8[(bbIdx * AFX_NUM_BOX_EDGE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeLineIndices[j];
        }
        else
        {
            AFX_ASSERT((idxSiz == sizeof(afxUnit16)) || (idxSiz == sizeof(afxUnit32)) || (idxSiz == sizeof(afxUnit8)));
            AfxThrowError();
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxGenerateIndexedFacesForAabbs(afxUnit cnt, afxBox const aabbs[], afxV3d vertices[][AFX_NUM_BOX_CORNERS], afxUnit vtxStride, afxUnit indices[][AFX_NUM_BOX_FACE_VERTICES], afxUnit idxSiz)
// Generate an indexed triangle list for an array of AABBs.
// Each AABB will be converted into a cube (box) made of 12 triangles (2 per face × 6 faces).
// Each triangle consists of 3 vertex indices, so you'll generate 36 indices per AABB.
// Each AABB has 8 unique vertices(corners).
{
    afxError err = { 0 };
    AFX_ASSERT(vertices);
    // vertices[] array must be capable of storing @cnt * 8 vertices.
    AFX_ASSERT(indices);
    // indices[] array must be capable of stroring @cnt * 36 indices.
    
    //if (outIdxCnt) *outIdxCnt = cnt * 36;
    //if (outVtxCnt) *outVtxCnt = cnt * 8;

    afxUnit rslt = cnt * AFX_NUM_BOX_FACE_VERTICES;

    static afxUnit const cubeFaceIndices[AFX_NUM_BOX_FACE_VERTICES] =
    {
#if 0 // CW
        // Front face
        0, 1, 2,  2, 1, 3,
        // Back face
        5, 4, 7,  7, 4, 6,
        // Left face
        4, 0, 6,  6, 0, 2,
        // Right face
        1, 5, 3,  3, 5, 7,
        // Top face
        2, 3, 6,  6, 3, 7,
        // Bottom face
        4, 5, 0,  0, 5, 1
#else // CCW
        // Front face (zmin)
        0, 2, 1,  1, 2, 3,
        // Back face (zmax)
        5, 7, 4,  4, 7, 6,
        // Left face (xmin)
        4, 6, 0,  0, 6, 2,
        // Right face (xmax)
        1, 3, 5,  5, 3, 7,
        // Top face (ymax)
        2, 6, 3,  3, 6, 7,
        // Bottom face (ymin)
        4, 0, 5,  5, 0, 1
#endif
    };

    for (afxUnit bbIdx = 0; bbIdx < cnt; ++bbIdx)
    {
        afxBox const* box = &aabbs[bbIdx];
        afxReal xmin = box->min.v[0], ymin = box->min.v[1], zmin = box->min.v[2];
        afxReal xmax = box->max.v[0], ymax = box->max.v[1], zmax = box->max.v[2];
        afxV3d const cubeCorners[AFX_NUM_BOX_CORNERS] =
        {
            AFX_V3D(xmin, ymin, zmin), // 0
            AFX_V3D(xmax, ymin, zmin), // 1
            AFX_V3D(xmin, ymax, zmin), // 2
            AFX_V3D(xmax, ymax, zmin), // 3
            AFX_V3D(xmin, ymin, zmax), // 4
            AFX_V3D(xmax, ymin, zmax), // 5
            AFX_V3D(xmin, ymax, zmax), // 6
            AFX_V3D(xmax, ymax, zmax)  // 7
        };
        
        // vertices[bbIdx * 8 + j], corners[j]
        for (afxUnit j = 0; j < AFX_NUM_BOX_CORNERS; ++j)
        {
            vertices[bbIdx][j] = cubeCorners[j];
        }

        // Copy indices (offset by vertex start index)
        // indices[bbIdx * 36 + j] = cubeIndices[j] + bbIdx * 8;

        if (sizeof(afxUnit16) == idxSiz)
        {
            afxUnit16* indices16 = (afxUnit16*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices16[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else if (sizeof(afxUnit32) == idxSiz)
        {
            afxUnit32* indices32 = (afxUnit32*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices32[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else if (sizeof(afxUnit8) == idxSiz)
        {
            afxUnit8* indices8 = (afxUnit8*)indices;

            for (afxUnit j = 0; j < AFX_NUM_BOX_FACE_VERTICES; ++j)
                indices8[(bbIdx * AFX_NUM_BOX_FACE_VERTICES) + j] = (bbIdx * AFX_NUM_BOX_CORNERS) + cubeFaceIndices[j];
        }
        else
        {
            AFX_ASSERT((idxSiz == sizeof(afxUnit16)) || (idxSiz == sizeof(afxUnit32)) || (idxSiz == sizeof(afxUnit8)));
            AfxThrowError();
        }
    }
    return rslt;
}

_AFXINL afxBox AfxGetIntersectedAabb(afxBox const a, afxBox const b)
{
    // Compute the intersection of two AABBs
    // Returns true if they intersect, and fills the result AABB
    
    afxBox bb;
    bb.min.v[0] = AFX_MAX(a.min.v[0], b.min.v[0]);
    bb.min.v[1] = AFX_MAX(a.min.v[1], b.min.v[1]);
    bb.min.v[2] = AFX_MAX(a.min.v[2], b.min.v[2]);
    bb.min.v[3] = 1;
    bb.max.v[0] = AFX_MIN(a.max.v[0], b.max.v[0]);
    bb.max.v[1] = AFX_MIN(a.max.v[1], b.max.v[1]);
    bb.max.v[2] = AFX_MIN(a.max.v[2], b.max.v[2]);
    bb.max.v[3] = 1;
    return bb;
}

_AFXINL afxBool AfxAabbIntersects(afxBox const a, afxBox const b)
{
    // Compute the intersection of two AABBs
    // Returns true if they intersect, and fills the result AABB

    afxBox bb = AfxGetIntersectedAabb(a, b);

    // Check if the intersection is valid (non-empty)
    return (bb.min.v[0] <= bb.max.v[0]) && (bb.min.v[1] <= bb.max.v[1]) && (bb.min.v[2] <= bb.max.v[2]);
}

_AFXINL afxUnit AfxAabbIncludes(afxBox const bb, afxBox const other)
{
    afxError err = { 0 };
    afxUnit rslt = 0;

    // Check if an AABB intersects with another AABB (overlapping region)

    if ((bb.min.v[0] < other.max.v[0] && bb.max.v[0] > other.min.v[0]) &&
        (bb.min.v[1] < other.max.v[1] && bb.max.v[1] > other.min.v[1]) &&
        (bb.min.v[2] < other.max.v[2] && bb.max.v[2] > other.min.v[2]))
    {
        ++rslt;
    }
    return rslt;
}

_AFXINL afxUnit AfxAabbIncludesAny(afxBox const bb, afxUnit cnt, afxBox const other[])
{
    afxError err = { 0 };
    AFX_ASSERT(other);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        // Check if an AABB intersects with another AABB (overlapping region)

        if ((bb.min.v[0] < other[i].max.v[0] && bb.max.v[0] > other[i].min.v[0]) &&
            (bb.min.v[1] < other[i].max.v[1] && bb.max.v[1] > other[i].min.v[1]) &&
            (bb.min.v[2] < other[i].max.v[2] && bb.max.v[2] > other[i].min.v[2]))
        {
            ++rslt;
        }
    }
    return rslt;
}

_AFXINL afxUnit AfxAabbIncludesAtv3d(afxBox const bb, afxUnit cnt, afxV3d const point[])
{
    afxError err = { 0 };
    AFX_ASSERT(point);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if( (!(bb.max.v[0] < point[i].v[0])) &&
            (!(bb.max.v[1] < point[i].v[1])) &&
            (!(bb.max.v[2] < point[i].v[2])) &&
            (!(bb.min.v[0] > point[i].v[0])) &&
            (!(bb.min.v[1] > point[i].v[1])) &&
            (!(bb.min.v[2] > point[i].v[2]))
            )
        {
            ++rslt;
        }
    }
    return rslt;
}

afxBool AfxTestSphereBox(afxSphere const bs, afxBox const bb)
{
    afxError err = { 0 };

    // test if a bounding box is fully inside a bounding sphere.

    for (afxUnit i = 0; i < 3; i++)
        if ((bs.xyzr.v[i] + bs.xyzr.v[AFX_SPHERE_RADIUS] < bb.min.v[i]) || 
            (bs.xyzr.v[i] - bs.xyzr.v[AFX_SPHERE_RADIUS] > bb.max.v[i]))
            return FALSE;

    return TRUE;
}

_AFXINL afxBox AfxTransformAabb(afxBox bb, afxM4d const m)
{
    afxError err = { 0 };
    bb.max = AfxV4dPostMultiplyM4d(m, bb.max);
    bb.min = AfxV4dPostMultiplyM4d(m, bb.min);
    return bb;
}

_AFXINL void AfxTransformObbs(afxM3d const ltm, afxV3d const atv, afxUnit cnt, afxBox const in[], afxBox out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // box = obb [min, max]

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxV3d pos;
        afxV3d max = AfxV3dFill(-3.4028235e38);
        afxV3d min = AfxV3dFill(3.4028235e38);

        for (afxUnit z = 0; z < 2; z++)
        {
            for (afxUnit y = 0; y < 2; y++)
            {
                for (afxUnit x = 0; x < 2; x++)
                {
                    afxV3d tmp = AfxV3dMake(x ? in[i].max.v[0] : in[i].min.v[0],
                                            y ? in[i].max.v[1] : in[i].min.v[1],
                                            z ? in[i].max.v[2] : in[i].min.v[2]);

                    pos = AfxV3dPostMultiplyM3d(ltm, tmp);
                    pos = AfxV3dAdd(pos, atv);

                    min = AfxV3dMin(min, pos);
                    max = AfxV3dMax(max, pos);
                }
            }
        }

        out[i].max = AfxV4dFromV3d(max);
        out[i].min = AfxV4dFromV3d(min);
    }
}

_AFXINL void AfxCopyBoxes(afxUnit cnt, afxBox const in[], afxUnit inStride, afxBox out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride >= sizeof(afxBox) || cnt == 1);
    AFX_ASSERT(outStride >= sizeof(afxBox) || cnt == 1);
    AFX_ASSERT_ALIGNMENT(inStride, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(outStride, AFX_PTR_ALIGNMENT);

    // is a array copy operation (not sparsed elements)?
    if ((inStride == outStride) && ((inStride == sizeof(afxBox)) || (inStride == 0)))
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[i] = in[i];
        }
    }
    // is a spam operation (single to multiple elements)?
    else if (inStride == 0)
    {
        AFX_ASSERT((cnt == 1) || (outStride > sizeof(afxBox)));

        afxBox box = in[0];
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[0] = box;
            out = (afxBox*)(AFX_CAST(afxByte const*, out) + outStride);
        }
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            out[0] = in[0];
            out = (afxBox*)(AFX_CAST(afxByte const*, out) + outStride);
            in = (afxBox const*)(AFX_CAST(afxByte const*, in) + inStride);
        }
    }
}

_AFXINL void AfxResetBoxes(afxUnit cnt, afxBox boxes[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT(boxes);
    AFX_ASSERT(cnt);
    AFX_ASSERT(stride >= sizeof(afxBox) || cnt == 1);
    AFX_ASSERT_ALIGNMENT(stride, AFX_PTR_ALIGNMENT);

    // if elements are not sparsed.
    if (!stride || (stride == sizeof(afxBox)))
    {
        for (afxUnit i = 0; i < cnt; i++)
            boxes[i] = AfxMakeAabb(0, NIL);
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            boxes[0] = AfxMakeAabb(0, NIL);
            boxes = (afxBox*)(AFX_CAST(afxByte const*, boxes) + stride);
        }
    }
}
