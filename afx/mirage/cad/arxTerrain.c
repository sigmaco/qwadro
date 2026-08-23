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

// This file is part of Advanced RenderWare Extensions.

#define _AFX_SIM_C
#define _ARX_TERRAIN_C
#define _ASX_SIMULATION_C
#include "../scene/arxIcd.h"

/*
    Benefits of world-aligned sectors:
        Easy spatial queries - no transform logic needed.
        Raycasts, AI pathfinding, or physics don't care which sector they're in.
        World is fully seamless - no "jumps" or gaps between sectors.
*/

// Function to check if a point is in front of a plane
afxBool isPointInFrontOfPlane(afxPlane const plane, afxV3d const point)
{
    return (plane.uvwd.v[0] * point.v[0] +
        plane.uvwd.v[1] * point.v[1] +
        plane.uvwd.v[2] * point.v[2] + plane.uvwd.v[AFX_PLANE_DIST]) >= 0.0f;
}

// Function to check if the AABB is in front of all frustum planes
afxBool isBoxCulled(const afxBox box, const afxFrustum frustum)
{
    // Check all planes of the frustum
    for (int i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        afxPlane const plane = frustum.planes[i];

        // Check the 8 corners of the AABB
        afxBool isBehindPlane = TRUE;
        for (int j = 0; j < 8; ++j)
        {
            // Generate the 8 corners of the AABB
            afxV3d corner = AFX_V3D(
                (j & 1) ? box.max.v[0] : box.min.v[0],
                (j & 2) ? box.max.v[1] : box.min.v[1],
                (j & 4) ? box.max.v[2] : box.min.v[2]
            );

            // If any corner is in front of the plane, the AABB is not culled
            if (isPointInFrontOfPlane(plane, corner))
            {
                isBehindPlane = FALSE;
                break;
            }
        }

        // If the AABB is completely behind the plane, cull it
        if (isBehindPlane) {
            return TRUE;  // AABB is outside of the frustum, so it's culled
        }
    }

    return FALSE;  // AABB is within the frustum
}

// New function to check an array of AABBs
void cullAABBs(const afxBox *boxes, size_t numBoxes, const afxFrustum frustum, afxBool *culledResults)
{
    for (size_t i = 0; i < numBoxes; ++i)
    {
        // Cull each box and store the result in the array
        culledResults[i] = isBoxCulled(boxes[i], frustum);
    }
}

// Optimized function to check if the AABB is in front of all frustum planes
void cullAABBs2(const afxBox *boxes, size_t numBoxes, const afxFrustum frustum, afxBool *culledResults)
{
    for (size_t i = 0; i < numBoxes; ++i)
    {
        const afxBox box = boxes[i];
        afxBool isCulled = FALSE;

        // Check all planes of the frustum
        for (int planeIndex = 0; planeIndex < afxCubeFace_TOTAL; ++planeIndex)
        {
            const afxPlane plane = frustum.planes[planeIndex];

            // Check the 8 corners of the AABB
            afxBool isBehindPlane = TRUE;
            for (int j = 0; j < 8; ++j)
            {
                // Generate the 8 corners of the AABB
                afxV3d corner = AFX_V3D(
                    (j & 1) ? box.max.v[0] : box.min.v[0],
                    (j & 2) ? box.max.v[1] : box.min.v[1],
                    (j & 4) ? box.max.v[2] : box.min.v[2]
                );

                // If any corner is in front of the plane, the AABB is not culled
                if (isPointInFrontOfPlane(plane, corner))
                {
                    isBehindPlane = FALSE;
                    break;
                }
            }

            // If the AABB is completely behind the plane, cull it
            if (isBehindPlane)
            {
                isCulled = TRUE;  // AABB is outside of the frustum, so it's culled
                break;
            }
        }

        // Store the result in the array
        culledResults[i] = isCulled;
    }
}

afxBool CheckHeightOfTriangle(float x, float z, float* height, afxV3d v0, afxV3d v1, afxV3d v2)
{
    afxV3d startVector, directionVector, edge1, edge2, normal;
    afxV3d Q, e1, e2, e3, edgeNormal, temp;
    float magnitude, D, denominator, numerator, t, determinant;


    // Starting position of the ray that is being cast.
    startVector.v[0] = x;
    startVector.v[1] = 0.0f;
    startVector.v[2] = z;

    // The direction the ray is being cast.
    directionVector.v[0] = 0.0f;
    directionVector.v[1] = -1.0f;
    directionVector.v[2] = 0.0f;

    // Calculate the two edges from the three points given.
    edge1.v[0] = v1.v[0] - v0.v[0];
    edge1.v[1] = v1.v[1] - v0.v[1];
    edge1.v[2] = v1.v[2] - v0.v[2];

    edge2.v[0] = v2.v[0] - v0.v[0];
    edge2.v[1] = v2.v[1] - v0.v[1];
    edge2.v[2] = v2.v[2] - v0.v[2];

    // Calculate the normal of the triangle from the two edges.
    normal.v[0] = (edge1.v[1] * edge2.v[2]) - (edge1.v[2] * edge2.v[1]);
    normal.v[1] = (edge1.v[2] * edge2.v[0]) - (edge1.v[0] * edge2.v[2]);
    normal.v[2] = (edge1.v[0] * edge2.v[1]) - (edge1.v[1] * edge2.v[0]);

    magnitude = (float)sqrt((normal.v[0] * normal.v[0]) + (normal.v[1] * normal.v[1]) + (normal.v[2] * normal.v[2]));
    normal.v[0] = normal.v[0] / magnitude;
    normal.v[1] = normal.v[1] / magnitude;
    normal.v[2] = normal.v[2] / magnitude;

    // Find the distance from the origin to the plane.
    D = ((-normal.v[0] * v0.v[0]) + (-normal.v[1] * v0.v[1]) + (-normal.v[2] * v0.v[2]));

    // Get the denominator of the equation.
    denominator = ((normal.v[0] * directionVector.v[0]) + (normal.v[1] * directionVector.v[1]) + (normal.v[2] * directionVector.v[2]));

    // Make sure the result doesn't get too close to zero to prevent divide by zero.
    if (fabs(denominator) < 0.0001f)
    {
        return FALSE;
    }

    // Get the numerator of the equation.
    numerator = -1.0f * (((normal.v[0] * startVector.v[0]) + (normal.v[1] * startVector.v[1]) + (normal.v[2] * startVector.v[2])) + D);

    // Calculate where we intersect the triangle.
    t = numerator / denominator;

    // Find the intersection vector.
    Q.v[0] = startVector.v[0] + (directionVector.v[0] * t);
    Q.v[1] = startVector.v[1] + (directionVector.v[1] * t);
    Q.v[2] = startVector.v[2] + (directionVector.v[2] * t);

    // Find the three edges of the triangle.
    e1.v[0] = v1.v[0] - v0.v[0];
    e1.v[1] = v1.v[1] - v0.v[1];
    e1.v[2] = v1.v[2] - v0.v[2];

    e2.v[0] = v2.v[0] - v1.v[0];
    e2.v[1] = v2.v[1] - v1.v[1];
    e2.v[2] = v2.v[2] - v1.v[2];

    e3.v[0] = v0.v[0] - v2.v[0];
    e3.v[1] = v0.v[1] - v2.v[1];
    e3.v[2] = v0.v[2] - v2.v[2];

    // Calculate the normal for the first edge.
    edgeNormal.v[0] = (e1.v[1] * normal.v[2]) - (e1.v[2] * normal.v[1]);
    edgeNormal.v[1] = (e1.v[2] * normal.v[0]) - (e1.v[0] * normal.v[2]);
    edgeNormal.v[2] = (e1.v[0] * normal.v[1]) - (e1.v[1] * normal.v[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp.v[0] = Q.v[0] - v0.v[0];
    temp.v[1] = Q.v[1] - v0.v[1];
    temp.v[2] = Q.v[2] - v0.v[2];

    determinant = ((edgeNormal.v[0] * temp.v[0]) + (edgeNormal.v[1] * temp.v[1]) + (edgeNormal.v[2] * temp.v[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Calculate the normal for the second edge.
    edgeNormal.v[0] = (e2.v[1] * normal.v[2]) - (e2.v[2] * normal.v[1]);
    edgeNormal.v[1] = (e2.v[2] * normal.v[0]) - (e2.v[0] * normal.v[2]);
    edgeNormal.v[2] = (e2.v[0] * normal.v[1]) - (e2.v[1] * normal.v[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp.v[0] = Q.v[0] - v1.v[0];
    temp.v[1] = Q.v[1] - v1.v[1];
    temp.v[2] = Q.v[2] - v1.v[2];

    determinant = ((edgeNormal.v[0] * temp.v[0]) + (edgeNormal.v[1] * temp.v[1]) + (edgeNormal.v[2] * temp.v[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Calculate the normal for the third edge.
    edgeNormal.v[0] = (e3.v[1] * normal.v[2]) - (e3.v[2] * normal.v[1]);
    edgeNormal.v[1] = (e3.v[2] * normal.v[0]) - (e3.v[0] * normal.v[2]);
    edgeNormal.v[2] = (e3.v[0] * normal.v[1]) - (e3.v[1] * normal.v[0]);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp.v[0] = Q.v[0] - v2.v[0];
    temp.v[1] = Q.v[1] - v2.v[1];
    temp.v[2] = Q.v[2] - v2.v[2];

    determinant = ((edgeNormal.v[0] * temp.v[0]) + (edgeNormal.v[1] * temp.v[1]) + (edgeNormal.v[2] * temp.v[2]));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return FALSE;
    }

    // Now we have our height.
    *height = Q.v[1];

    return TRUE;
}

afxBool GetHeightAtPosition(arxTerrain ter, float inputX, float inputZ, float* height)
{
    float vertex1[3], vertex2[3], vertex3[3];
    afxBool foundHeight;

    afxV3d v = AFX_V3D(inputX, 0, inputZ );

    // Loop through all of the terrain cells to find out which one the inputX and inputZ would be inside.
    afxUnit cellId = -1;
    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        // Get the current cell dimensions.
        afxBox aabb = ter->sectors[i].aabb;

        // Check to see if the positions are in this cell.
        if (AfxAabbIncludesAtv3d(aabb, 1, &v))
        {
            cellId = i;
            break;
        }
    }

    // If we didn't find a cell then the input position is off the terrain grid.
    if (cellId == -1)
    {
        return FALSE;
    }

    _arxTerrSec* sec = &ter->sectors[cellId];
    afxUnit collVtxCnt = sec->collVtxCnt;
    afxV3d const* collVtx = sec->collVtx;

    // If this is the right cell then check all the triangles in this cell to see what the height of the triangle at this position is.
    for (afxUnit i = 0; i < (collVtxCnt / 3); i++)
    {
        // Check to see if this is the polygon we are looking for.
        foundHeight = CheckHeightOfTriangle(inputX, inputZ, height, sec->collVtx[i], sec->collVtx[i + 1], sec->collVtx[i + 2]);

        if (foundHeight)
        {
            return TRUE;
        }
    }
    return FALSE;
}

_ARX afxError ArxLoadHeighmap(arxTerrain ter, afxUnit secIdx, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, 1);

    afxStream iob;
    if (AfxOpenFile(uri, afxFileFlag_RX, &iob))
    {
        AfxThrowError();
        return err;
    }
    avxRasterFile tga;
    AvxReadRasterFile(&tga, iob);

    afxByte* data;
    AfxAllocate(AfxHere(), tga.decSiz, AFX_SIMD_ALIGNMENT, (void**)&data);
    AvxDecodeRasterFile(&tga, iob, data);
    AfxDisposeObjects(1, &iob);

    avxFormat fmt = tga.fmt;
    avxFormatDescription pfd;
    AvxDescribeFormat(fmt, &pfd);




    arxMesh msh;
    ArxGetTerrainMeshes(ter, secIdx, 1, &msh);
    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    for (unsigned int i = 0; i < tga.width; i++)
    {
        for (unsigned int j = 0; j < tga.height; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + tga.width * i) * pfd.compCnt;
            // raw height at coordinate
            unsigned char y = texel[0];

            // vertex
            pos[i * j + j] = AfxV3dMake(-tga.height / 2.0f + i, (int)y * yScale - yShift, -tga.width / 2.0f + j);
        }
    }

    AfxDeallocate(AfxHere(), (void**)&data);

    return err;
}

_ARX afxUnit ArxGetTerrainMeshes(arxTerrain ter, afxUnit secIdx, afxUnit cnt, arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, secIdx, cnt);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxMesh msh = ter->sectors[secIdx].msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_ARX afxError ArxResetTerrainSector(arxTerrain ter, afxUnit secIdx, arxMesh msh)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    
    if (ter->sectors[secIdx].msh != msh)
    {
        if (msh)
        {
            AfxReacquireObjects(1, &msh);
        }

        if (ter->sectors[secIdx].msh)
        {
            AfxDisposeObjects(1, &ter->sectors[secIdx].msh);
        }

        ter->sectors[secIdx].msh = msh;
    }
    return err;
}

_ARX afxUnit findSectorIndex(arxTerrain ter, afxReal x, afxReal z)
{
    int sectorX = (int)(x / ter->secWidth);
    int sectorZ = (int)(z / ter->secDepth);
    return sectorZ * ter->rowSecCnt + sectorX;
}

_ARX afxBool getHeightInSector(arxTerrain ter, afxUnit secIdx, afxReal worldX, afxReal worldZ, afxReal* worldY)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    _arxTerrSec* s = &ter->sectors[secIdx];

    // Compute which quad (cell) we're in, directly in world space
    int localX = (int)(worldX)-(s->gridNodeX * ter->secWidth);
    int localZ = (int)(worldZ)-(s->gridNodeZ * ter->secDepth);

    if (localX < 0 || localX >= ter->secWidth || localZ < 0 || localZ >= ter->secDepth)
    {
        // Outside sector bounds
        *worldY = 0.0f; 
        return FALSE;
    }

    afxReal fx = worldX - (s->gridNodeX * ter->secWidth + localX);
    afxReal fz = worldZ - (s->gridNodeZ * ter->secDepth + localZ);

    int vertsPerRow = ter->secWidth + 1;
    int topLeft = localZ * vertsPerRow + localX;
    int topRight = topLeft + 1;
    int bottomLeft = (localZ + 1) * vertsPerRow + localX;
    int bottomRight = bottomLeft + 1;

    afxUnit ia, ib, ic;

    if (fx + fz <= 1.0f)
    {
        // Lower-left triangle
        ia = topLeft;
        ib = bottomLeft;
        ic = topRight;
    }
    else
    {
        // Upper-right triangle
        ia = topRight;
        ib = bottomLeft;
        ic = bottomRight;
    }

    afxV3d const v0 = AFX_V3D(
        s->collVtx[ia].v[0],
        s->collVtx[ia].v[1],
        s->collVtx[ia].v[2]
    );
    afxV3d const v1 = AFX_V3D(
        s->collVtx[ib].v[0],
        s->collVtx[ib].v[1],
        s->collVtx[ib].v[2]
    );
    afxV3d const v2 = AFX_V3D(
        s->collVtx[ic].v[0],
        s->collVtx[ic].v[1],
        s->collVtx[ic].v[2]
    );

    // Barycentric interpolation
    // This is the denominator of a barycentric coordinate equation - it's based on the 2D positions of the triangle's vertices. 
    // It is using a mix of x and z (assuming the triangle lies in xz-plane for interpolation).
    afxReal denom = (v1.v[2] - v2.v[2]) * (v0.v[0] - v2.v[0]) + (v2.v[0] - v1.v[0]) * (v0.v[2] - v2.v[2]);
    
    // This checks if the triangle is degenerate - meaning, it has near-zero area (the points are colinear or very close to it).
    // A floating point tolerance check is used to avoid division by nearly-zero, which could cause instability or garbage results.
    if (fabsf(denom) < 1e-6f)
    {
        *worldY = v0.v[1];
        return FALSE;
    }

    // After checking that the triangle isn't degenerate (that denom isn't near-zero), we are computing barycentric coordinates 
    // of a point (worldX, worldZ) relative to a triangle defined by three vertices v0, v1, v2.

    afxReal lambda1 = ((v1.v[2] - v2.v[2]) * (worldX - v2.v[0]) + (v2.v[0] - v1.v[0]) * (worldZ - v2.v[2])) / denom;
    afxReal lambda2 = ((v2.v[2] - v0.v[2]) * (worldX - v2.v[0]) + (v0.v[0] - v2.v[0]) * (worldZ - v2.v[2])) / denom;
    afxReal lambda3 = 1.0f - lambda1 - lambda2;

    *worldY = lambda1 * v0.v[1] + lambda2 * v1.v[1] + lambda3 * v2.v[1];
    return TRUE;
}

_ARX afxBool ArxGetTerrainHeightAt(arxTerrain ter, afxReal x, afxReal z, afxReal* y)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxInt sectorX = (afxInt)(x / ter->secWidth);
    afxInt sectorZ = (afxInt)(z / ter->secDepth);

    if (sectorX < 0 || sectorX >= ter->rowSecCnt || sectorZ < 0 || sectorZ >= ter->sliceSecCnt)
    {
        *y = 0.0f; // Out of bounds
        return FALSE;
    }

    afxUnit secIdx = sectorZ * ter->rowSecCnt + sectorX;
    return getHeightInSector(ter, secIdx, x, z, y);
}

_ARX afxError _ArxBuildTerrainCollideShape(arxTerrain ter, afxUnit baseSecIdx, afxUnit secCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);
    AFX_ASSERT_RANGE(ter->secCnt, baseSecIdx, secCnt);

    for (afxUnit secIter = 0; secIter < secCnt; secIter++)
    {
        afxUnit secIdx = baseSecIdx + secIter;
        _arxTerrSec* sec = &ter->sectors[secIdx];

        if (!sec->collVtx)
        {
            sec->collTriCnt = sec->visuVtxIdxCnt / 3;
            sec->collVtxCnt = sec->collTriCnt * 3;

            if (AfxAllocate(AfxHere(), sec->collVtxCnt * sizeof(sec->collVtx[0]), AFX_SIMD_ALIGNMENT, (void**)&sec->collVtx))
            {
                AfxThrowError();
                continue;
            }
            AfxZero(sec->collVtx, sec->collVtxCnt * sizeof(sec->collVtx[0]));

            afxUnit triCnt = sec->visuVtxIdxCnt / 3;

            for (int i = 0; i < triCnt; i++)
            {
                int i0 = sec->visuVtxMap[i * 3 + 0];
                int i1 = sec->visuVtxMap[i * 3 + 1];
                int i2 = sec->visuVtxMap[i * 3 + 2];

                sec->collVtx[i * 3 + 0] = sec->visuVtxPos[i0];
                sec->collVtx[i * 3 + 1] = sec->visuVtxPos[i1];
                sec->collVtx[i * 3 + 2] = sec->visuVtxPos[i2];
            }
        }
    }
    return err;
}

_ARX float getHeight(float x, float z, float heightScale)
// Simple height function
{
    return sinf(x * 0.1f) * cosf(z * 0.1f) * heightScale;
}

_ARX afxError _ArxBuildTerrainVisualShape(arxTerrain ter, afxUnit baseSecIdx, afxUnit secCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    for (afxUnit secIter = 0; secIter < secCnt; secIter++)
    {
        afxUnit secIdx = baseSecIdx + secIter;
        _arxTerrSec* sec = &ter->sectors[secIdx];

        // bump up the reference counter.
        sec->visuReqCnt++;

        // Skip if it is already loaded.
        if (sec->visuReqCnt > 1)
            continue;

        afxV3d* vertices = NIL;
        afxUnit16* indices = NIL;

        AFX_ASSERT(sec->vtxCnt == ((ter->secWidth + 1) * (ter->secDepth + 1)));
        sec->visuVtxCnt = ((ter->secWidth + 1) * (ter->secDepth + 1));
        AfxAllocate(AfxHere(), sec->visuVtxCnt * sizeof(vertices[0]), AFX_SIMD_ALIGNMENT, (void**)&vertices);
        AFX_ASSERT(sec->vtxIdxCnt == ((ter->secWidth * ter->secDepth) * 6));
        sec->visuVtxIdxCnt = ((ter->secWidth * ter->secDepth) * 6);
        AfxAllocate(AfxHere(), sec->visuVtxIdxCnt * sizeof(indices[0]), AFX_SIMD_ALIGNMENT, (void**)&indices);

        // Generate vertices
        for (afxUnit z = 0; z <= ter->secDepth; z++)
        {
            for (afxUnit x = 0; x <= ter->secWidth; x++)
            {
                afxUnit idx = z * sec->vtxPerRow + x;
                afxReal worldX = (afxReal)(sec->vtxStartX + x);
                afxReal worldZ = (afxReal)(sec->vtxStartZ + z);
                afxReal y = 0;// getHeight(worldX, worldZ, ter->heightScale);

                vertices[idx] = AfxV3dMake(worldX, y, worldZ);
            }
        }

        sec->visuVtxPos = vertices;

        // Generate indices in CCW order
        afxUnit idx = 0;
        for (afxUnit z = 0; z < ter->secDepth; z++)
        {
            for (afxUnit x = 0; x < ter->secWidth; x++)
            {
                afxUnit topLeft = z * sec->vtxPerRow + x;
                afxUnit topRight = topLeft + 1;
                afxUnit bottomLeft = (z + 1) * sec->vtxPerRow + x;
                afxUnit bottomRight = bottomLeft + 1;

                // Triangle 1 (CCW): topLeft -> bottomLeft -> topRight
                indices[idx++] = topLeft;
                indices[idx++] = bottomLeft;
                indices[idx++] = topRight;

                // Triangle 2 (CCW): topRight -> bottomLeft -> bottomRight
                indices[idx++] = topRight;
                indices[idx++] = bottomLeft;
                indices[idx++] = bottomRight;
            }
        }
        AFX_ASSERT(sec->vtxIdxCnt == idx);
        sec->visuVtxMap = indices;

        ter->secAabb[secIdx] = AfxMakeAabb(0, NIL);
        ter->secAabb[secIdx] = AfxEmboxTriangles(ter->secAabb[secIdx], sec->vtxIdxCnt / 3, vertices, (afxUnit const*)indices, sizeof(indices[0]));
        sec->aabb = ter->secAabb[secIdx];
    }
    return err;
}

AFX_DEFINE_STRUCT(arxTerrainSectorConfig)
{
    afxUnit width;
    afxUnit depth;
    afxUri  heightmap;
    afxReal heightScale;
};

_ARX afxError ArxAddTerrainSector(arxTerrain ter)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    _arxTerrSec* sec;
    if (AfxAllocate(AfxHere(), sizeof(*sec), 0, (void**)&sec))
    {
        AfxThrowError();
        return err;
    }

    

    return err;
}

_ARX afxError ArxGetTerrainHeights(arxTerrain ter, afxUnit x, afxUnit z, afxUnit w, afxUnit d, afxReal heights[], afxUnit heightStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    for (afxUnit i = 0; i < d; i++) for (afxUnit j = 0; j < w; j++)
    {
        ter->secAabb;
    }

    return err;
}

_ARX afxError ArxSetTerrainHeights(arxTerrain ter, afxUnit x, afxUnit z, afxUnit w, afxUnit d, afxReal const heights[], afxUnit heightStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    for (afxUnit i = 0; i < d; i++) for (afxUnit j = 0; j < w; j++)
    {
        ter->secAabb;
    }

    return err;
}

_ARX afxError _ArxTerDtorCb(arxTerrain ter)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    afxAllocation const stashes[] =
    {
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        },
        {
            .cnt = ter->secCnt,
            .siz = sizeof(ter->secAabb[0]),
            .var = (void**)&ter->secAabb
        }
    };

    if (AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxError _ArxTerCtorCb(arxTerrain ter, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TER, 1, &ter);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxTerrainConfig const* cfg = AFX_CAST(arxTerrainConfig const*, args[1]);

    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);

    ter->dsys = dsys;

    ter->width = AFX_MAX(128, cfg->width);
    ter->depth = AFX_MAX(128, cfg->depth);
    //ter->heightScale = AFX_MAX(16, cfg->heightScale);
    ter->heightmap = (afxUri) { 0 };// cfg->heightmap;

    ter->displace = cfg->displace;
    ter->scale = cfg->scale;
    //AFX_ASSERT(AfxV3dMag(cfg->scale));

    // Set the height and width of each terrain cell to a fixed 33x33 vertex array.
    ter->secWidth = AFX_MAX(cfg->blockSiz, 32);
    ter->secDepth = AFX_MAX(cfg->blockSiz, 32);

    // Calculate the number of cells needed to store the terrain data.
    ter->rowSecCnt = ter->width / ter->secWidth;
    ter->sliceSecCnt = ter->depth / ter->secDepth;
    ter->secCnt = ter->sliceSecCnt * ter->rowSecCnt;

    ter->sectors = NIL;
    ter->secAabb = NIL;

    afxAllocation const stashes[] =
    {
        {
            .cnt = ter->secCnt,
            .align = AFX_SIMD_ALIGNMENT,
            .siz = sizeof(ter->sectors[0]),
            .var = (void**)&ter->sectors
        },
        {
            .cnt = ter->secCnt,
            .align = AFX_SIMD_ALIGNMENT,
            .siz = sizeof(ter->secAabb[0]),
            .var = (void**)&ter->secAabb
        }
    };

    if (AfxAllocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    // Loop through and initialize all the terrain cells.
    for (afxUnit gridZ = 0; gridZ < ter->sliceSecCnt; gridZ++)
    {
        for (afxUnit gridX = 0; gridX < ter->rowSecCnt; gridX++)
        {
            afxUnit index = (ter->rowSecCnt * gridZ) + gridX;

            _arxTerrSec* sec = &ter->sectors[index];
            int a = 1;
            //*sec = (_arxTerrSec) { 0 };
            AfxZero(sec, sizeof(sec[0]));

            sec->gridNode = index;
            sec->gridNodeX = gridX;
            sec->gridNodeZ = gridZ;

            afxUnit vtxStartX = gridX * ter->secWidth;
            afxUnit vtxStartZ = gridZ * ter->secDepth;
            // Calculate the number of vertices in this terrain cell.
            afxUnit vtxPerRow = ter->secWidth + 1;
            afxUnit vtxPerCol = ter->secDepth + 1;
            afxUnit vtxCnt = vtxPerRow * vtxPerCol;
            afxUnit quadCnt = ter->secWidth * ter->secDepth;
            afxUnit idxCnt = quadCnt * 6;

            sec->vtxCnt = vtxCnt;
            sec->vtxIdxCnt = idxCnt;
            sec->quadCnt = quadCnt;
            sec->vtxStartZ = vtxStartZ;
            sec->vtxStartX = vtxStartX;
            sec->vtxPerRow = vtxPerRow;
            sec->vtxPerCol = vtxPerCol;

            sec->msh = NIL;
        }
    }

    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        _ArxBuildTerrainVisualShape(ter, i, 1);
        _ArxBuildTerrainCollideShape(ter, i, 1);
    }

    avxShader shd;
    AvxAcquireShaders(dsys, 1, NIL, &shd);
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    avxShaderSpecialization specs0[2] = { 0 };
    specs0[0].stage = avxShaderType_VERTEX;
    specs0[0].prog = AFX_STRING("terrainMeshVsh");
    specs0[1].stage = avxShaderType_FRAGMENT;
    specs0[1].prog = AFX_STRING("terrainMeshFsh");
    AvxCompileShaderFromDisk(shd, &specs0[0].prog, AfxUri("../gfx/terrainMeshVsh.glsl"));
    AvxCompileShaderFromDisk(shd, &specs0[1].prog, AfxUri("../gfx/terrainMeshFsh.glsl"));
    
    avxShaderSpecialization specs1[2] = { 0 };
    specs1[0].stage = avxShaderType_VERTEX;
    specs1[0].prog = AFX_STRING("terrainAabbVsh");
    specs1[1].stage = avxShaderType_FRAGMENT;
    specs1[1].prog = AFX_STRING("terrainAabbFsh");
    AvxCompileShaderFromDisk(shd, &specs1[0].prog, AfxUri("../gfx/terrainAabbVsh.glsl"));
    AvxCompileShaderFromDisk(shd, &specs1[1].prog, AfxUri("../gfx/terrainAabbFsh.glsl"));
    
    avxVertexInput vin[2];
    avxVertexLayout vtxl[2] = { 0 };
    vtxl[0].tag = AFX_STRING("terrainMesh");
    vtxl[0].binCnt = 1;
    vtxl[0].bins[0] = AVX_VERTEX_STREAM(0, 0, 0);
    vtxl[0].attrCnt = 5;
    vtxl[0].attrs[0] = AVX_VERTEX_ATTR(0, 0, 0, avxFormat_RGB32f);
    vtxl[0].attrs[1] = AVX_VERTEX_ATTR(1, 0, 12, avxFormat_RG32f);
    vtxl[0].attrs[2] = AVX_VERTEX_ATTR(2, 0, 20, avxFormat_RGB32f);
    vtxl[0].attrs[3] = AVX_VERTEX_ATTR(3, 0, 32, avxFormat_RGB32f);
    vtxl[0].attrs[4] = AVX_VERTEX_ATTR(4, 0, 44, avxFormat_RGB32f);

    vtxl[1].tag = AFX_STRING("terrainAabb");
    vtxl[1].binCnt = 1;
    vtxl[1].bins[0] = AVX_VERTEX_STREAM(0, 0, 0);
    vtxl[1].attrCnt = 1;
    vtxl[1].attrs[0] = AVX_VERTEX_ATTR(0, 0, 0, avxFormat_RGB32f);

    AvxAcquireVertexInputs(dsys, 2, vtxl, vin);

    avxPipeline pip[2];
    avxPipelineConfig pipb[2] = { 0 };
    pipb[0].depthTestEnabled = TRUE;
    pipb[0].vin = vin[0];
    pipb[0].tag = AFX_STRING("terrainMesh");
    pipb[0].primTop = avxTopology_TRI_LIST;
    pipb[0].shd = shd;
    pipb[0].progCnt = 2;
    pipb[0].progSpecs = specs0;
    pipb[1].depthTestEnabled = TRUE;
    pipb[1].vin = vin[1];
    pipb[1].tag = AFX_STRING("terrainAabb");
    pipb[1].primTop = avxTopology_LINE_LIST;
    pipb[1].fillMode = avxFillMode_LINE;
    pipb[1].shd = shd;
    pipb[1].progCnt = 2;
    pipb[1].progSpecs = specs1;
    AvxAssembleGfxPipelines(dsys, 2, pipb, pip);
    AfxDisposeObjects(2, vin);

    AfxDisposeObjects(1, &shd);

    ter->dbgAabbPip = pip[1];
    ter->meshPip = pip[0];

    if (err && AfxDeallocateInstanceData(ter, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxClassConfig const _ARX_TER_CLASS_CONFIG =
{
    .fcc = afxFcc_TER,
    .name = "Terrain",
    .desc = "Managed Terrain",
    .fixedSiz = sizeof(AFX_OBJECT(arxTerrain)),
    .ctor = (void*)_ArxTerCtorCb,
    .dtor = (void*)_ArxTerDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireTerrain(arxScenario scio, arxTerrainConfig const* cfg, arxTerrain* terrain)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass *)_ArxScioGetTerClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_TER);

    if (AfxAcquireObjects(cls, 1, (afxObject*)terrain, (void const*[]) { scio, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ARX afxError ArxGenerateTerrain(arxScenario scio, afxWarp const whd, arxTerrain* terrain)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxUnit width = whd.w, height = whd.h, depth = whd.d;

    arxMesh msh;
    //msh = AfxBuildPlaneMesh(sim->scio, 256, 256, 256, 256);
    //msh = AfxBuildParallelepipedMesh(sim->scio, AfxWhd(10, 10, 10), 10, 10);
    //msh = AfxBuildCubeMesh(sim->scio, 10, 1);
    ArxBuildGridMesh(scio, 100, 100, 1000, 1000, AFX_V3D_ZERO, &msh);
    //msh = AfxBuildGridMesh(sim->scio, 10, 10, 10, 10);
    //msh = AfxBuildDiscMesh(sim->scio, 10, 10);
    //AfxInvertMeshTopology(msh);

    arxTerrain ter;
    arxTerrainConfig terc = { 0 };
    ArxAcquireTerrain(scio, &terc, &ter);
    ArxResetTerrainSector(ter, 0, msh);
    AfxDisposeObjects(1, &msh);
    *terrain = ter;

    return err;
}

_ARX afxError ArxGenerateHeightmappedTerrain(arxScenario scio, afxUri const* uri, arxTerrain* terrain)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

#if 0
    avxRaster ras;
    AvxLoadRasters(sim->dsys, 1, NIL, uri, &ras);
    AvxPrintRaster(ras, AfxUri("../heightmap_dump.tga"), NIL, 1, 0);
#endif

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &iobi, &iob);
    AfxReopenFile(iob, uri, afxFileFlag_R);

    avxRasterFile tga;
    AvxReadRasterFile(&tga, iob);

    afxByte* data;
    AfxAllocate(AfxHere(), tga.decSiz, AFX_SIMD_ALIGNMENT, (void**)&data);
    AvxDecodeRasterFile(&tga, iob, data);

    avxFormat fmt = tga.fmt;
    avxFormatDescription pfd;
    AvxDescribeFormat(fmt, &pfd);

    //tga.width = tga.width, tga.height = tga.height / 8;

    arxMesh msh;
    ArxBuildGridMesh(scio, tga.width, tga.height, tga.width * 10, tga.height * 10, AFX_V3D_ZERO, &msh);

    arxTerrain ter;
    arxTerrainConfig terc = {0};
    ArxAcquireTerrain(scio, &terc, &ter);
    ArxResetTerrainSector(ter, 0, msh);

    //arxMesh msh;
    ArxGetTerrainMeshes(ter, 0, 1, &msh);
    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

    afxV3d* pos = ArxAccessVertexData(msh, 0, 0, 0);

    afxReal yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data

    afxUnit vtxIdx = 0;
    for (unsigned int i = 0; i < tga.height; i++)
    {
        for (unsigned int j = 0; j < tga.width; j++)
        {
            unsigned char* pixelOffset = data + (j + tga.width * i) * pfd.stride;
            unsigned char y = pixelOffset[0];
#if 0
#if 0
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.height / 2.0f + tga.height * i / (float)tga.height), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.width / 2.0f + tga.width * j / (float)tga.width));   // vz
#else
            // vertex
            AfxV3dSet(pos[vtxIdx++],
                (-tga.width / 2.0f + tga.width * j / (float)tga.width), // vx
                ((int)y * yScale - yShift), // vy
                (-tga.height / 2.0f + tga.height * i / (float)tga.height));   // vz
#endif
#endif
            pos[(j + tga.width * i)].v[1] = ((afxReal)y * yScale - yShift);

        }
    }
    *terrain = ter;
    return err;
}
