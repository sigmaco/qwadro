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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced RenderWare Extensions.

#define _ARX_MESH_C
#define _ARX_MESH_TOPOLOGY_C
#include "../scene/arxIcd.h"

#if defined(_AFX_DEBUG)
#   define ASX_DBG_ECHO_MESH 1
#   define _ARX_DBG_MESH_SANITIZE_ARGS 1
#endif

_ARX afxError ArxUpdateVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(src);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = ArxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    AfxStream2(vtxCnt, src, srcStride, data, unitSiz);
    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ARX afxError ArxUploadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void* data;

    if (!(data = ArxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }
    
    //AfxDoStreamInput(in, stride, vtxCnt, data, unitSiz);
    AfxReadStream2(in, vtxCnt * unitSiz, stride, data, unitSiz);
    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ARX afxError ArxDownloadVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void const* data;

    if (!(data = ArxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    AfxDoStreamOutput(out, stride, vtxCnt, data, unitSiz);
    return err;
}

_ARX afxError ArxExportMeshData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt, void* dst, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(dst);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
    vtxCnt = AFX_MIN(vtxCnt, msh->vtxCnt - baseVtx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    void const* data;
    if (!(data = ArxAccessVertexData(msh, attrIdx, morphIdx, baseVtx)))
    {
        AfxThrowError();
        return err;
    }

    AfxStream2(vtxCnt, data, unitSiz, dst, stride);
    return err;
}

_ARX afxError ArxExportMeshDataDeindexed(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseIdx, afxUnit idxCnt, void* dst, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->idxCnt, baseIdx, idxCnt);
    AFX_ASSERT(dst);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseIdx = AFX_MIN(baseIdx, (msh->triCnt * 3) - 1);
    idxCnt = AFX_MIN(idxCnt, (msh->triCnt * 3) - baseIdx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    afxByte const* data;
    if (!(data = ArxAccessVertexData(msh, attrIdx, morphIdx, 0)))
    {
        AfxThrowError();
        return err;
    }

    afxUnit const* indices = &msh->indices[baseIdx];
    afxUnit vtxSiz = pfd.stride;

    afxByte* dst2 = dst;

    for (afxUnit i = 0; i < idxCnt; ++i)
    {
        unsigned int idx = indices[i];
        AfxStream2(1, &data[idx * vtxSiz], vtxSiz, &dst2[i * stride], stride);
    }
    return err;
}

// UPDATE/DUMP INDEXES /////////////////////////////////////////////////////////

_ARX afxError ArxUpdateMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void const* src, afxUnit srcIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    afxUnit idxCnt = triCnt * ARX_INDICES_PER_TRI;
    afxUnit* indices = ArxGetMeshIndices(msh, baseTriIdx);
    afxIndex8 const* src8 = src;
    afxIndex16 const* src16 = src;
    afxIndex32 const* src32 = src;

    switch (srcIdxSiz)
    {
    case sizeof(afxUnit8):  for (afxUnit i = 0; i < idxCnt; i++) indices[i] =  src8[i]; break;
    case sizeof(afxUnit16): for (afxUnit i = 0; i < idxCnt; i++) indices[i] = src16[i]; break;
    case sizeof(afxUnit32): for (afxUnit i = 0; i < idxCnt; i++) indices[i] = src32[i]; break;
    default: AfxThrowError(); break;
    }

    if (!err)
    {
#if _AFX_DEBUG
        for (afxUnit morphIdx = 0; morphIdx < msh->morphCnt; morphIdx++)
        {
            arxMeshMorph mshm;
            ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm);
            for (afxUnit i = 0; i < idxCnt; i++)
            {
                AFX_ASSERT_RANGE(msh->vtxCnt, indices[i], 1);
            }
        }
#endif
#if 0 // Can not be done here. It can cause crashes due to garbage in vertex data. Also, it is unnecessarely implicit.
        for (afxUnit secIdx = 0; secIdx < msh->secCnt; secIdx++)
        {
            arxMeshSection mshs;
            ArxDescribeMeshSections(msh, secIdx, 1, &mshs);

            if (baseTriIdx >= mshs.baseTriIdx && baseTriIdx + triCnt <= mshs.baseTriIdx + mshs.triCnt)
                for (afxUnit morphIdx = 0; morphIdx < msh->morphCnt; morphIdx++)
                    ArxRecomputeMeshBounds(msh, morphIdx, secIdx, 1, 0);
        }
#endif
    }
    return err;
}

_ARX afxError ArxDumpMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit mshTriCnt = msh->triCnt;
    AFX_ASSERT_RANGE(mshTriCnt, baseTriIdx, triCnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    triCnt = AFX_MIN(triCnt, mshTriCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, mshTriCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    afxUnit const* indices = ArxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT(dst);
    AFX_ASSERT(dstIdxSiz);
    AfxStream2(ARX_INDICES_PER_TRI * triCnt, indices, sizeof(indices[0]), dst, dstIdxSiz);

    return err;
}

_ARX afxError ArxExportMeshTopology(arxMesh msh, avxTopology top, afxUnit baseTriIdx, afxUnit triCnt, void* dst, afxUnit dstIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit mshTriCnt = msh->triCnt;
    AFX_ASSERT_RANGE(mshTriCnt, baseTriIdx, triCnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    triCnt = AFX_MIN(triCnt, mshTriCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, mshTriCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    AFX_ASSERT(dst);
    AFX_ASSERT(dstIdxSiz);

    afxUnit const* indices = ArxGetMeshIndices(msh, baseTriIdx);

    if (top == msh->topology)
    {
        AfxStream2(ARX_INDICES_PER_TRI * triCnt, indices, sizeof(indices[0]), dst, dstIdxSiz);
        return err;
    }

    AFX_ASSERT(msh->topology == avxTopology_TRI_LIST);

    switch (top)
    {
    case avxTopology_TRI_STRIP:
    {
        /*
            Triangle Strip

            In a triangle strip, the winding order is maintained by reordering the vertices. 
            The first two vertices of the first triangle are used, and then each subsequent triangle shares 
            two vertices with the previous one.

            In a CCW-oriented mesh, the winding order of triangles in the strip should still follow CCW.
            The function creates new triangles by using the previous vertices and the new vertex for each iteration, 
            but the winding order must be checked to make sure it’s preserved.
            For example, if the first triangle in the list is defined by vertices (A, B, C), the next triangle in the strip 
            should use (B, C, D). This ensures the order remains CCW.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt - 2; i++)
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit i2 = indices[i + 2];

            dst2[3 * i] = i0;
            dst2[3 * i + 1] = i1;
            dst2[3 * i + 2] = i2;
        }
        break;
    }
    case avxTopology_TRI_FAN:
    {
        /*
            Triangle Fan

            In a triangle fan, all triangles share a common central vertex. The fan works by connecting the central 
            vertex to consecutive vertices along the perimeter.
            The function uses the center vertex and then forms triangles from the center to consecutive vertices.
            Since the center vertex is fixed, the ordering of the outer vertices around the center needs to maintain 
            a CCW winding order. The CCW ordering of the original mesh should work if the outer vertices are ordered 
            correctly around the fan center.
        */
        afxUnit* dst2 = dst;
        afxUnit center = indices[0];  // The central vertex for the fan.
        for (size_t i = 1; i < triCnt - 1; i++)
        {
            afxUnit i1 = indices[i];
            afxUnit i2 = indices[i + 1];

            dst2[3 * (i - 1)] = center;
            dst2[3 * (i - 1) + 1] = i1;
            dst2[3 * (i - 1) + 2] = i2;
        }
        break;
    }
    case avxTopology_LINE_LIST:
    {
        /*
            Line List

            For line lists, you convert each triangle into two lines, effectively treating every pair of vertices as a line segment. 
            Each triangle in the list gets converted into 6 indices (forming 3 lines).
            The order of the vertices in the triangle doesn't need to be reversed since we're just pulling pairs of indices for the lines, 
            and the line order in OpenGL doesn't depend on CCW winding order.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt; i++)
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit i2 = indices[i + 2];

            dst2[6 * i + 0] = i0;
            dst2[6 * i + 1] = i1;
            dst2[6 * i + 2] = i1;
            dst2[6 * i + 3] = i2;
            dst2[6 * i + 4] = i2;
            dst2[6 * i + 5] = i0;
        }
        break;
    }
    case avxTopology_LINE_STRIP:
    {
        /*
            Line Strip

            A line strip connects consecutive vertices to form a continuous path. Since a line strip doesn't rely on the face's 
            orientation (CCW or CW), it will work regardless of vertex ordering.
            The line strip will connect the vertices in order without changing the CCW winding of the individual triangles.
            No change needed.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt - 1; i++) // For line strip, connect vertices in a strip.
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];

            dst2[2 * i] = i0;
            dst2[2 * i + 1] = i1;
        }
        break;
    }
    case avxTopology_LINE_LIST_ADJACENT:
    {
        /*
            Line List Adjacent

            For line list adjacent, you are not only considering the line endpoints but also the adjacent vertices (previous and next vertices in the sequence).
            As with the standard line list, this doesn't change the CCW winding order of the individual triangles, 
            because we are still connecting pairs of vertices into lines.
            The adjacency here refers to the lines and their neighbors, but the vertex winding order remains CCW.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt; i++)
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit i2 = indices[i + 2];
            afxUnit prevI = indices[(i - 1 + triCnt) % triCnt];
            afxUnit nextI = indices[(i + 3) % triCnt];

            dst2[8 * i] = prevI;
            dst2[8 * i + 1] = i0;
            dst2[8 * i + 2] = i1;
            dst2[8 * i + 3] = nextI;
            dst2[8 * i + 4] = i1;
            dst2[8 * i + 5] = i2;
            dst2[8 * i + 6] = prevI;
            dst2[8 * i + 7] = i2;
        }
        break;
    }
    case avxTopology_LINE_STRIP_ADJACENT:
    {
        /*
            Line Strip Adjacent

            For line strip adjacent, this is a variation of a line strip where each segment connects not just two consecutive vertices 
            but also considers the adjacent vertices to form the lines.
            Again, this doesn't affect the original triangle's winding order since we're still working with line segments.
            The adjacency information adds complexity to the connections but doesn’t change the CCW order.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt - 1; i++) // Similar to line strip, but include adjacent vertices.
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit prevI = indices[(i - 1 + triCnt) % triCnt];
            afxUnit nextI = indices[(i + 2) % triCnt];

            dst2[4 * i] = prevI;
            dst2[4 * i + 1] = i0;
            dst2[4 * i + 2] = i1;
            dst2[4 * i + 3] = nextI;
        }
        break;
    }
    case avxTopology_TRI_LIST_ADJACENT:
    {
        /*
            Triangle List Adjacent

            For triangle list adjacent, this topology works by considering not only the vertices of the triangles but also their neighbors. 
            Each triangle in the adjacency list is expanded with adjacent triangle information, forming 12 indices (4 vertices for each triangle pair).
            The adjacency information is added in the correct order (CCW), so it should work as expected without disrupting the winding 
            order of the original triangles.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt; i++)
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit i2 = indices[i + 2];
            afxUnit prevI = indices[(i - 1 + triCnt) % triCnt];
            afxUnit nextI = indices[(i + 3) % triCnt];

            dst2[12 * i] = prevI;
            dst2[12 * i + 1] = i0;
            dst2[12 * i + 2] = i1;
            dst2[12 * i + 3] = i2;
            dst2[12 * i + 4] = nextI;
            dst2[12 * i + 5] = i1;
            dst2[12 * i + 6] = i2;
            dst2[12 * i + 7] = prevI;
            dst2[12 * i + 8] = i2;
            dst2[12 * i + 9] = nextI;
            dst2[12 * i + 10] = prevI;
            dst2[12 * i + 11] = i0;
        }
        break;
    }
    case avxTopology_TRI_STRIP_ADJACENT:
    {
        /*
            Triangle Strip Adjacent

            For triangle strip adjacent, you're working with a strip where each triangle is adjacent to its neighbor in the strip. 
            You need to handle four vertices per triangle pair (two from each triangle, plus their neighbors).
            Again, since the mesh is CCW, the function should properly maintain this orientation when creating adjacent triangles in the strip.
        */
        afxUnit* dst2 = dst;
        for (size_t i = 0; i < triCnt - 2; i++) // Strip adjacency has four vertices per pair of triangles.
        {
            afxUnit i0 = indices[i];
            afxUnit i1 = indices[i + 1];
            afxUnit i2 = indices[i + 2];
            afxUnit i3 = indices[i + 3];
            afxUnit i4 = indices[i + 4];
            afxUnit i5 = indices[i + 5];

            // Each triangle pair shares two vertices.
            dst2[12 * i + 0] = i0;
            dst2[12 * i + 1] = i1;
            dst2[12 * i + 2] = i2; // First triangle in pair
            dst2[12 * i + 3] = i1;
            dst2[12 * i + 4] = i2;
            dst2[12 * i + 5] = i3; // Second triangle in pair
            dst2[12 * i + 6] = i2;
            dst2[12 * i + 7] = i3;
            dst2[12 * i + 8] = i4;
            dst2[12 * i + 9] = i3;
            dst2[12 * i + 10] = i4;
            dst2[12 * i + 11] = i5; // Continue the next triangle in the strip
        }
        break;
    }
    case avxTopology_PATCH_LIST:
    {
        /*
            For patch lists, you'd typically need to handle more complex primitives like quadrilaterals or higher-order patches. 
            Depending on the type of patch, you would need to ensure that the patch's control points are defined in CCW order.
            Since patch list handling isn't fully implemented in your current code, we'd assume it needs additional logic for 
            handling specific patch types (e.g., quadrilaterals or NURBS patches).
        */

        // Patch list typically requires control points for patches, which would need to be defined based on your patch type (e.g., quadratic patches).
        break;
    }
    default: AfxThrowError(); break;
    }

    return err;
}

// UPLOAD/DOWNLOAD INDICES /////////////////////////////////////////////////////

_ARX afxError ArxUploadMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream in, afxUnit srcIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit mshTriCnt = msh->triCnt;
    AFX_ASSERT_RANGE(mshTriCnt, baseTriIdx, triCnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    triCnt = AFX_MIN(triCnt, mshTriCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, mshTriCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    afxUnit idxCnt = triCnt * ARX_INDICES_PER_TRI;
    afxUnit* indices = ArxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);

    if (AfxReadStream2(in, srcIdxSiz * idxCnt, srcIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    if (!err)
    {
#if _AFX_DEBUG
        arxMeshMorph mshm;
        ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

        for (afxUnit i = 0; i < idxCnt; i++)
        {
            AFX_ASSERT_RANGE(msh->vtxCnt, indices[i], 1);

            if (msh->vtxCnt < indices[i])
                AfxThrowError();
        }
#endif
#if 0 // Can not be done here. It can cause crashes due to garbage in vertex data. Also, it is unnecessarely implicit.
        for (afxUnit i = 0; i < msh->secCnt; i++)
        {
            arxMeshSection mshs;
            ArxDescribeMeshSections(msh, i, 1, &mshs);

            if (mshs.baseTriIdx >= baseTriIdx && triCnt >= mshs.triCnt)
                for (afxUnit j = 0; j < msh->morphCnt; j++)
                    ArxRecomputeMeshBounds(msh, j, i, 1, 0);
        }
#endif
    }
    return err;
}

_ARX afxError ArxDownloadMeshIndices(arxMesh msh, afxUnit baseTriIdx, afxUnit triCnt, afxStream out, afxUnit dstIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, triCnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    triCnt = AFX_MIN(triCnt, msh->triCnt - baseTriIdx);
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    afxUnit idxCnt = triCnt * ARX_INDICES_PER_TRI;
    afxUnit* indices = ArxGetMeshIndices(msh, baseTriIdx);

    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);

    if (AfxWriteStream2(out, dstIdxSiz * idxCnt, dstIdxSiz, indices, sizeof(indices[0])))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////

