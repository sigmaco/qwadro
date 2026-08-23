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

#define _ARX_MESH_C
//#define _ARX_GEOMETRY_C
#define _ARX_VERTEX_BUILDER_C
#define _ARX_MESH_TOPOLOGY_C
#include "../scene/arxIcd.h"

#define MaxNumWeights 8

struct WeightVertex
{
    afxUnit8 BoneWeights[MaxNumWeights];
    afxUnit8 BoneIndices[MaxNumWeights];
    afxUnit BonesUsed;
};

struct TriWeightData
{
    afxUnit jntCnt;
    afxUnit8 jntIdx[MaxNumWeights * 3];
    afxTriangle32 vtxIdx;
};

#define AFX_V2D_IN(arr_, stride_, idx_) ((afxV2d const*)&(((afxByte const*)(arr_))[(stride_) * (idx_)]))
#define AFX_V2D_OUT(arr_, stride_, idx_) ((afxV2d*)&(((afxByte*)(arr_))[(stride_) * (idx_)]))

#define AFX_V3D_IN(arr_, stride_, idx_) ((afxV3d const*)&(((afxByte const*)(arr_))[(stride_) * (idx_)]))
#define AFX_V3D_OUT(arr_, stride_, idx_) ((afxV3d*)&(((afxByte*)(arr_))[(stride_) * (idx_)]))

#define AFX_V4D_IN(arr_, stride_, idx_) ((afxV4d const*)&(((afxByte const*)(arr_))[(stride_) * (idx_)]))
#define AFX_V4D_OUT(arr_, stride_, idx_) ((afxV4d*)&(((afxByte*)(arr_))[(stride_) * (idx_)]))

#define AFX_VTX_IDX(arr_, stride_, i_) ((afxUnit32 const*)&(((afxByte const*)(arr_))[(stride_) * (i_)]))
#define AFX_VTX_IDX16(arr_, stride_, i_) ((afxUnit16 const*)&(((afxByte const*)(arr_))[(stride_) * (i_)]))
#define AFX_VTX_IDX8(arr_, stride_, i_) ((afxUnit8 const*)&(((afxByte const*)(arr_))[(stride_) * (i_)]))

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

_ARX void ArxInvertIndexedTriangles(afxUnit triCnt, afxUnit indices[], afxUnit idxStride)
// Invert the winding for a stream of indexed triangles.
{
    afxError err = { 0 };
    AFX_ASSERT(idxStride);
    AFX_ASSERT(indices);
    AFX_ASSERT(triCnt);

    switch (idxStride)
    {
    case sizeof(afxUnit32):
    {
        afxUnit32* indices2 = (void*)indices;
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit idx = indices2[i * ARX_INDICES_PER_TRI + 0];
            indices2[i * ARX_INDICES_PER_TRI + 0] = indices2[i * ARX_INDICES_PER_TRI + 2];
            indices2[i * ARX_INDICES_PER_TRI + 2] = idx;
        }
        break;
    }
    case sizeof(afxUnit16):
    {
        afxUnit16* indices2 = (void*)indices;
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit idx = indices2[i * ARX_INDICES_PER_TRI + 0];
            indices2[i * ARX_INDICES_PER_TRI + 0] = indices2[i * ARX_INDICES_PER_TRI + 2];
            indices2[i * ARX_INDICES_PER_TRI + 2] = idx;
        }
        break;
    }
    case sizeof(afxUnit8):
    {
        afxUnit8* indices2 = (void*)indices;
        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit idx = indices2[i * ARX_INDICES_PER_TRI + 0];
            indices2[i * ARX_INDICES_PER_TRI + 0] = indices2[i * ARX_INDICES_PER_TRI + 2];
            indices2[i * ARX_INDICES_PER_TRI + 2] = idx;
        }
        break;
    }
    default: AfxThrowError(); break;
    }    
}

static inline afxBool compareTriEdges(afxUnit a1, afxUnit a2, afxUnit b1, afxUnit b2)
// Function to compare two pairs of integers (edges)
{
    return (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1);
}

_ARX void ArxExtractIndexedLines(afxUnit triCnt, afxUnit triIndices[], afxUnit triIdxStride, afxUnit bufCap, afxUnit edgeIndices[], afxUnit* numLines)
// Function to generate an indexed line list from an indexed triangle list
{
    afxError err = { 0 };
    AFX_ASSERT(bufCap >= triCnt * 3); // Maximum 3 lines per triangle
    // Dynamic memory allocation for lines (initially assume a large number of lines)
    afxUnit maxLines = triCnt * 3;  // Maximum 3 lines per triangle
    afxUnit maxLineIndices = maxLines * 2 * sizeof(afxUnit); // Each line has 2 indices
    afxUnit lineCnt = 0;

    // Loop through each triangle to extract edges
    for (afxUnit i = 0; i < triCnt; i++)
    {
        // For each triangle, the 3 vertex indices
        afxUnit v0 = triIndices[i * 3 + 0];
        afxUnit v1 = triIndices[i * 3 + 1];
        afxUnit v2 = triIndices[i * 3 + 2];

        // Check and add edges (v0-v1, v1-v2, v2-v0)
        afxBool edgeExists;

        // v0-v1
        edgeExists = FALSE;
        for (afxUnit j = 0; j < lineCnt; j++)
        {
            if (compareTriEdges(v0, v1, edgeIndices[j * 2], edgeIndices[j * 2 + 1]))
            {
                edgeExists = TRUE;
                break;
            }
        }

        if (!edgeExists)
        {
            edgeIndices[lineCnt * 2] = v0;
            edgeIndices[lineCnt * 2 + 1] = v1;
            lineCnt++;
        }

        // v1-v2
        edgeExists = FALSE;
        for (afxUnit j = 0; j < lineCnt; j++)
        {
            if (compareTriEdges(v1, v2, edgeIndices[j * 2], edgeIndices[j * 2 + 1]))
            {
                edgeExists = TRUE;
                break;
            }
        }

        if (!edgeExists)
        {
            edgeIndices[lineCnt * 2] = v1;
            edgeIndices[lineCnt * 2 + 1] = v2;
            lineCnt++;
        }

        // v2-v0
        edgeExists = FALSE;
        for (afxUnit j = 0; j < lineCnt; j++)
        {
            if (compareTriEdges(v2, v0, edgeIndices[j * 2], edgeIndices[j * 2 + 1]))
            {
                edgeExists = TRUE;
                break;
            }
        }

        if (!edgeExists)
        {
            edgeIndices[lineCnt * 2] = v2;
            edgeIndices[lineCnt * 2 + 1] = v0;
            lineCnt++;
        }
    }

    *numLines = lineCnt;
    afxUnit edgeIndicesCnt = lineCnt * 2 * sizeof(afxUnit); // 2 indices per line    
}

_ARX afxError ArxComputeTriangleTangents(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d const uv[], afxUnit uvStride, afxBool perTriOut, afxV3d tan[], afxUnit tanStride, afxV3d bit[], afxUnit bitStride)
{
    afxError err = { 0 };
    AFX_ASSERT(indices);
    AFX_ASSERT(pos);
    AFX_ASSERT(uv);
    AFX_ASSERT(tan || bit);

    AFX_ASSERT_ALIGNMENT(pos, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(uv, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(tan, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(bit, AFX_PTR_ALIGNMENT);

    if (!indices)
    {
        // Not indexed

        for (afxUnit i = 0; i < triCnt; i++)
        {
            afxUnit const ia = i * ARX_INDICES_PER_TRI + 0;
            afxUnit const ib = i * ARX_INDICES_PER_TRI + 1;
            afxUnit const ic = i * ARX_INDICES_PER_TRI + 2;

            // edges
            afxV3d ea = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ib), *AFX_V3D_IN(pos, posStride, ia));
            afxV3d eb = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ia));
            //pos = pos2 += posStride;
            //pos += posStep;

            afxV2d deltaUv1 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ib), *AFX_V2D_IN(uv, uvStride, ia));
            afxV2d deltaUv2 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ic), *AFX_V2D_IN(uv, uvStride, ia));
            //uv = uv2 += uvStride;
            //uv += uvStep;

            afxReal f = 1.f / (deltaUv1.v[0] * deltaUv2.v[1] - deltaUv2.v[0] * deltaUv1.v[1]);
            
            if (tan)
            {
                AFX_V3D_OUT(tan, tanStride, ia)->v[0] = f * (deltaUv2.v[1] * ea.v[0] - deltaUv1.v[1] * eb.v[0]);
                AFX_V3D_OUT(tan, tanStride, ia)->v[1] = f * (deltaUv2.v[1] * ea.v[1] - deltaUv1.v[1] * eb.v[1]);
                AFX_V3D_OUT(tan, tanStride, ia)->v[2] = f * (deltaUv2.v[1] * ea.v[2] - deltaUv1.v[1] * eb.v[2]);

                // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.

                if (!perTriOut)
                {
                    *AFX_V3D_OUT(tan, tanStride, ib) = *AFX_V3D_IN(tan, tanStride, ia);
                    *AFX_V3D_OUT(tan, tanStride, ic) = *AFX_V3D_IN(tan, tanStride, ia);
                }
                //tan = tan2 += tanStride;
                //tan += tanStep;
            }

            if (bit)
            {
                AFX_V3D_OUT(bit, bitStride, ia)->v[0] = f * (-deltaUv2.v[0] * ea.v[0] + deltaUv1.v[0] * eb.v[0]);
                AFX_V3D_OUT(bit, bitStride, ia)->v[1] = f * (-deltaUv2.v[0] * ea.v[1] + deltaUv1.v[0] * eb.v[1]);
                AFX_V3D_OUT(bit, bitStride, ia)->v[2] = f * (-deltaUv2.v[0] * ea.v[2] + deltaUv1.v[0] * eb.v[2]);

                if (!perTriOut)
                {
                    // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                    *AFX_V3D_OUT(bit, bitStride, ib) = *AFX_V3D_IN(bit, bitStride, ia);
                    *AFX_V3D_OUT(bit, bitStride, ic) = *AFX_V3D_IN(bit, bitStride, ia);
                }
                //bit = bit2 += bitStride;
                //bit += bitStep;
            }
        }
    }
    else
    {
        AFX_ASSERT((idxStride == 1) || (idxStride == 2) || (idxStride == 4));

        switch(idxStride)
        {
        case sizeof(afxUnit32):
        {
            afxUnit32 const* indices2 = (void*)indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit const ia = indices2[i * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[i * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[i * ARX_INDICES_PER_TRI + 2];

                // edges
                afxV3d ea = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ib), *AFX_V3D_IN(pos, posStride, ia));
                afxV3d eb = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ia));
                //pos = pos2 += posStride;
                //pos += posStep;

                afxV2d deltaUv1 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ib), *AFX_V2D_IN(uv, uvStride, ia));
                afxV2d deltaUv2 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ic), *AFX_V2D_IN(uv, uvStride, ia));
                //uv = uv2 += uvStride;
                //uv += uvStep;

                afxReal f = 1.f / (deltaUv1.v[0] * deltaUv2.v[1] - deltaUv2.v[0] * deltaUv1.v[1]);

                if (tan)
                {
                    AFX_V3D_OUT(tan, tanStride, ia)->v[0] = f * (deltaUv2.v[1] * ea.v[0] - deltaUv1.v[1] * eb.v[0]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[1] = f * (deltaUv2.v[1] * ea.v[1] - deltaUv1.v[1] * eb.v[1]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[2] = f * (deltaUv2.v[1] * ea.v[2] - deltaUv1.v[1] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(tan, tanStride, ib) = tan[ia];
                        *AFX_V3D_OUT(tan, tanStride, ic) = tan[ia];
                    }
                    //tan = tan2 += tanStride;
                    //tan += tanStep;
                }

                if (bit)
                {
                    AFX_V3D_OUT(bit, bitStride, ia)->v[0] = f * (-deltaUv2.v[0] * ea.v[0] + deltaUv1.v[0] * eb.v[0]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[1] = f * (-deltaUv2.v[0] * ea.v[1] + deltaUv1.v[0] * eb.v[1]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[2] = f * (-deltaUv2.v[0] * ea.v[2] + deltaUv1.v[0] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(bit, bitStride, ib) = *AFX_V3D_IN(bit, bitStride, ia);
                        *AFX_V3D_OUT(bit, bitStride, ic) = *AFX_V3D_IN(bit, bitStride, ia);
                    }
                    //bit = bit2 += bitStride;
                    //bit += bitStep;
                }
                break;
            }
        }
        case sizeof(afxUnit16):
        {
            afxUnit16 const* indices2 = (void*)indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit const ia = indices2[i * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[i * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[i * ARX_INDICES_PER_TRI + 2];

                // edges
                afxV3d ea = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ib), *AFX_V3D_IN(pos, posStride, ia));
                afxV3d eb = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ia));
                //pos = pos2 += posStride;
                //pos += posStep;

                afxV2d deltaUv1 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ib), *AFX_V2D_IN(uv, uvStride, ia));
                afxV2d deltaUv2 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ic), *AFX_V2D_IN(uv, uvStride, ia));
                //uv = uv2 += uvStride;
                //uv += uvStep;

                afxReal f = 1.f / (deltaUv1.v[0] * deltaUv2.v[1] - deltaUv2.v[0] * deltaUv1.v[1]);

                if (tan)
                {
                    AFX_V3D_OUT(tan, tanStride, ia)->v[0] = f * (deltaUv2.v[1] * ea.v[0] - deltaUv1.v[1] * eb.v[0]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[1] = f * (deltaUv2.v[1] * ea.v[1] - deltaUv1.v[1] * eb.v[1]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[2] = f * (deltaUv2.v[1] * ea.v[2] - deltaUv1.v[1] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(tan, tanStride, ib) = tan[ia];
                        *AFX_V3D_OUT(tan, tanStride, ic) = tan[ia];
                    }
                    //tan = tan2 += tanStride;
                    //tan += tanStep;
                }

                if (bit)
                {
                    AFX_V3D_OUT(bit, bitStride, ia)->v[0] = f * (-deltaUv2.v[0] * ea.v[0] + deltaUv1.v[0] * eb.v[0]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[1] = f * (-deltaUv2.v[0] * ea.v[1] + deltaUv1.v[0] * eb.v[1]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[2] = f * (-deltaUv2.v[0] * ea.v[2] + deltaUv1.v[0] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(bit, bitStride, ib) = *AFX_V3D_IN(bit, bitStride, ia);
                        *AFX_V3D_OUT(bit, bitStride, ic) = *AFX_V3D_IN(bit, bitStride, ia);
                    }
                    //bit = bit2 += bitStride;
                    //bit += bitStep;
                }
            }
        }
        case sizeof(afxUnit8):
        {
            afxUnit8 const* indices2 = (void*)indices;

            for (afxUnit i = 0; i < triCnt; i++)
            {
                afxUnit const ia = indices2[i * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[i * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[i * ARX_INDICES_PER_TRI + 2];

                // edges
                afxV3d ea = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ib), *AFX_V3D_IN(pos, posStride, ia));
                afxV3d eb = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ia));
                //pos = pos2 += posStride;
                //pos += posStep;

                afxV2d deltaUv1 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ib), *AFX_V2D_IN(uv, uvStride, ia));
                afxV2d deltaUv2 = AfxV2dSub(*AFX_V2D_IN(uv, uvStride, ic), *AFX_V2D_IN(uv, uvStride, ia));
                //uv = uv2 += uvStride;
                //uv += uvStep;

                afxReal f = 1.f / (deltaUv1.v[0] * deltaUv2.v[1] - deltaUv2.v[0] * deltaUv1.v[1]);

                if (tan)
                {
                    AFX_V3D_OUT(tan, tanStride, ia)->v[0] = f * (deltaUv2.v[1] * ea.v[0] - deltaUv1.v[1] * eb.v[0]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[1] = f * (deltaUv2.v[1] * ea.v[1] - deltaUv1.v[1] * eb.v[1]);
                    AFX_V3D_OUT(tan, tanStride, ia)->v[2] = f * (deltaUv2.v[1] * ea.v[2] - deltaUv1.v[1] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(tan, tanStride, ib) = tan[ia];
                        *AFX_V3D_OUT(tan, tanStride, ic) = tan[ia];
                    }
                    //tan = tan2 += tanStride;
                    //tan += tanStep;
                }

                if (bit)
                {
                    AFX_V3D_OUT(bit, bitStride, ia)->v[0] = f * (-deltaUv2.v[0] * ea.v[0] + deltaUv1.v[0] * eb.v[0]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[1] = f * (-deltaUv2.v[0] * ea.v[1] + deltaUv1.v[0] * eb.v[1]);
                    AFX_V3D_OUT(bit, bitStride, ia)->v[2] = f * (-deltaUv2.v[0] * ea.v[2] + deltaUv1.v[0] * eb.v[2]);

                    if (!perTriOut)
                    {
                        // Because a triangle is always a flat shape, we only need to calculate a single tangent/bitangent pair per triangle as they will be the same for each of the triangle's vertices.
                        *AFX_V3D_OUT(bit, bitStride, ib) = *AFX_V3D_IN(bit, bitStride, ia);
                        *AFX_V3D_OUT(bit, bitStride, ic) = *AFX_V3D_IN(bit, bitStride, ia);
                    }
                    //bit = bit2 += bitStride;
                    //bit += bitStep;
                }
            }
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_ARX afxError ArxComputeTriangleNormals(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV3d nrm[], afxUnit nrmStride)
{
    afxError err = { 0 };
    AFX_ASSERT(pos);
    AFX_ASSERT(nrm);

    // nrm must be already zeroed.
    AFX_ASSERT(AfxV3dIsZero(nrm[0]));

    /*
        For each face on the mesh, calc the face normal (without normalization, as just explained), and directly accumulate this normal in each vertex belonging to the face.
        After you are done with the faces, each vertex will have recieved all the face normals it was supposed to recieve. That simple.
    */

    AFX_ASSERT_ALIGNMENT(pos, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(nrm, AFX_PTR_ALIGNMENT);

    afxV3d* nrm2 = nrm;

    if (!indices)
    {
        for (afxUnit j = 0; j < triCnt; j++)
        {
            afxUnit const ia = j * ARX_INDICES_PER_TRI + 0;
            afxUnit const ib = j * ARX_INDICES_PER_TRI + 1;
            afxUnit const ic = j * ARX_INDICES_PER_TRI + 2;

            afxV3d e1 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ia), *AFX_V3D_IN(pos, posStride, ib));
            afxV3d e2 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ib));
            afxV3d no = AfxV3dCross(e1, e2);
            
            *AFX_V3D_OUT(nrm, nrmStride, ia) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ia), no);
            *AFX_V3D_OUT(nrm, nrmStride, ib) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ib), no);
            *AFX_V3D_OUT(nrm, nrmStride, ic) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ic), no);
            //nrm = nrm2 += nrmStride;
            //nrm += nrmStep;
        }
    }
    else
    {
        AFX_ASSERT((idxStride == 1) || (idxStride == 2) || (idxStride == 4));

        switch (idxStride)
        {
        case sizeof(afxUnit32):
        {
            afxUnit32 const* indices2 = (void*)indices;

            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                afxV3d e1 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ia), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d e2 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d no = AfxV3dCross(e1, e2);
                //pos = pos2 += posStride;
                //pos += posStep;

                *AFX_V3D_OUT(nrm, nrmStride, ia) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ia), no);
                *AFX_V3D_OUT(nrm, nrmStride, ib) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ib), no);
                *AFX_V3D_OUT(nrm, nrmStride, ic) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ic), no);
                //nrm = nrm2 += nrmStride;
                //nrm += nrmStep;
            }
            break;
        }
        case sizeof(afxUnit16):
        {
            afxUnit16 const* indices2 = (void*)indices;

            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                afxV3d e1 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ia), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d e2 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d no = AfxV3dCross(e1, e2);
                //pos = pos2 += posStride;
                //pos += posStep;

                *AFX_V3D_OUT(nrm, nrmStride, ia) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ia), no);
                *AFX_V3D_OUT(nrm, nrmStride, ib) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ib), no);
                *AFX_V3D_OUT(nrm, nrmStride, ic) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ic), no);
                //nrm = nrm2 += nrmStride;
                //nrm += nrmStep;
            }
            break;
        }
        case sizeof(afxUnit8):
        {
            afxUnit8 const* indices2 = (void*)indices;

            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                afxV3d e1 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ia), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d e2 = AfxV3dSub(*AFX_V3D_IN(pos, posStride, ic), *AFX_V3D_IN(pos, posStride, ib));
                afxV3d no = AfxV3dCross(e1, e2);
                //pos = pos2 += inStride;
                //pos += posStep;

                *AFX_V3D_OUT(nrm, nrmStride, ia) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ia), no);
                *AFX_V3D_OUT(nrm, nrmStride, ib) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ib), no);
                *AFX_V3D_OUT(nrm, nrmStride, ic) = AfxV3dAdd(*AFX_V3D_IN(nrm, nrmStride, ic), no);
                //nrm = nrm2 += outStride;
                //nrm += nrmStep;
            }
            break;
        }
        default: AfxThrowError();  break;
        }
    }

    // Serially normalize all them in one go.
    //AfxNormalizeArrayedV3d(triCnt * ARX_INDICES_PER_TRI, (void*)nrm2, (void*)nrm2);


    for (afxUnit j = 0; j < triCnt; j++)
    {
        afxUnit16 const* indices2 = (void*)indices;

        afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
        afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
        afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

        *AFX_V3D_OUT(nrm, nrmStride, ia) = AfxV3dNormalize(*AFX_V3D_IN(nrm, nrmStride, ia), NIL);
        *AFX_V3D_OUT(nrm, nrmStride, ib) = AfxV3dNormalize(*AFX_V3D_IN(nrm, nrmStride, ib), NIL);
        *AFX_V3D_OUT(nrm, nrmStride, ic) = AfxV3dNormalize(*AFX_V3D_IN(nrm, nrmStride, ic), NIL);
    }
    return err;
}

_ARX afxError ArxComputeTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxV3d const pos[], afxUnit posStride, afxV2d uv[], afxUnit uvStride)
{
    afxError err = { 0 };
    AFX_ASSERT(indices);
    AFX_ASSERT(pos);
    AFX_ASSERT(uv);

    AFX_ASSERT_ALIGNMENT(pos, AFX_PTR_ALIGNMENT);
    AFX_ASSERT_ALIGNMENT(uv, AFX_PTR_ALIGNMENT);

    if (!indices)
    {
        // Loop through each triangle and calculate the UVs
        for (afxUnit j = 0; j < triCnt; j++)
        {
            afxUnit const ia = j * ARX_INDICES_PER_TRI + 0;
            afxUnit const ib = j * ARX_INDICES_PER_TRI + 1;
            afxUnit const ic = j * ARX_INDICES_PER_TRI + 2;

            // Project the 3D vertices to 2D (XY plane)
            afxReal u1 = AFX_V3D_IN(pos, posStride, ia)->v[0], v1y = AFX_V3D_IN(pos, posStride, ia)->v[1];
            afxReal u2 = AFX_V3D_IN(pos, posStride, ib)->v[0], v2y = AFX_V3D_IN(pos, posStride, ib)->v[1];
            afxReal u3 = AFX_V3D_IN(pos, posStride, ic)->v[0], v3y = AFX_V3D_IN(pos, posStride, ic)->v[1];
            //pos = pos2 += posStride;
            //pos += posStep;

            // Find the min and max values for the u and v coordinates
            afxReal min_u = fminf(fminf(u1, u2), u3);
            afxReal max_u = fmaxf(fmaxf(u1, u2), u3);
            afxReal min_v = fminf(fminf(v1y, v2y), v3y);
            afxReal max_v = fmaxf(fmaxf(v1y, v2y), v3y);

            // Normalize UV coordinates to the range [0, 1]
            afxReal range_u = max_u - min_u;
            afxReal range_v = max_v - min_v;

            // Store the UV coordinates for this triangle
            AFX_V2D_OUT(uv, uvStride, ia)->v[0] = (u1 - min_u) / range_u;
            AFX_V2D_OUT(uv, uvStride, ia)->v[1] = (v1y - min_v) / range_v;

            AFX_V2D_OUT(uv, uvStride, ib)->v[0] = (u2 - min_u) / range_u;
            AFX_V2D_OUT(uv, uvStride, ib)->v[1] = (v2y - min_v) / range_v;

            AFX_V2D_OUT(uv, uvStride, ic)->v[0] = (u3 - min_u) / range_u;
            AFX_V2D_OUT(uv, uvStride, ic)->v[1] = (v3y - min_v) / range_v;
            //uv = uv2 += uvStride;
            //uv += uvStep;
        }
    }
    else
    {
        AFX_ASSERT((idxStride == 1) || (idxStride == 2) || (idxStride == 4));

        switch (idxStride)
        {
        case sizeof(afxUnit32):
        {
            afxUnit32 const* indices2 = (void*)indices;

            // Loop through each triangle and calculate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Project the 3D vertices to 2D (XY plane)
                afxReal u1 = AFX_V3D_IN(pos, posStride, ia)->v[0], v1y = AFX_V3D_IN(pos, posStride, ia)->v[1];
                afxReal u2 = AFX_V3D_IN(pos, posStride, ib)->v[0], v2y = AFX_V3D_IN(pos, posStride, ib)->v[1];
                afxReal u3 = AFX_V3D_IN(pos, posStride, ic)->v[0], v3y = AFX_V3D_IN(pos, posStride, ic)->v[1];
                //pos = pos2 += posStride;
                //pos += posStep;

                // Find the min and max values for the u and v coordinates
                afxReal min_u = fminf(fminf(u1, u2), u3);
                afxReal max_u = fmaxf(fmaxf(u1, u2), u3);
                afxReal min_v = fminf(fminf(v1y, v2y), v3y);
                afxReal max_v = fmaxf(fmaxf(v1y, v2y), v3y);

                // Normalize UV coordinates to the range [0, 1]
                afxReal range_u = max_u - min_u;
                afxReal range_v = max_v - min_v;

                // Store the UV coordinates for this triangle
                AFX_V2D_OUT(uv, uvStride, ia)->v[0] = (u1 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ia)->v[1] = (v1y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ib)->v[0] = (u2 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ib)->v[1] = (v2y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ic)->v[0] = (u3 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ic)->v[1] = (v3y - min_v) / range_v;
                //uv = uv2 += uvStride;
                //uv += uvStep;
            }
            break;
        }
        case sizeof(afxUnit16):
        {
            afxUnit16 const* indices2 = (void*)indices;

            // Loop through each triangle and calculate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Project the 3D vertices to 2D (XY plane)
                afxReal u1 = AFX_V3D_IN(pos, posStride, ia)->v[0], v1y = AFX_V3D_IN(pos, posStride, ia)->v[1];
                afxReal u2 = AFX_V3D_IN(pos, posStride, ib)->v[0], v2y = AFX_V3D_IN(pos, posStride, ib)->v[1];
                afxReal u3 = AFX_V3D_IN(pos, posStride, ic)->v[0], v3y = AFX_V3D_IN(pos, posStride, ic)->v[1];
                //pos = pos2 += posStride;
                //pos += posStep;

                // Find the min and max values for the u and v coordinates
                afxReal min_u = fminf(fminf(u1, u2), u3);
                afxReal max_u = fmaxf(fmaxf(u1, u2), u3);
                afxReal min_v = fminf(fminf(v1y, v2y), v3y);
                afxReal max_v = fmaxf(fmaxf(v1y, v2y), v3y);

                // Normalize UV coordinates to the range [0, 1]
                afxReal range_u = max_u - min_u;
                afxReal range_v = max_v - min_v;

                // Store the UV coordinates for this triangle
                AFX_V2D_OUT(uv, uvStride, ia)->v[0] = (u1 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ia)->v[1] = (v1y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ib)->v[0] = (u2 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ib)->v[1] = (v2y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ic)->v[0] = (u3 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ic)->v[1] = (v3y - min_v) / range_v;
                //uv = uv2 += uvStride;
                //uv += uvStep;
            }
            break;
        }
        case sizeof(afxUnit8) :
        {
            afxUnit8 const* indices2 = (void*)indices;

            // Loop through each triangle and calculate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Project the 3D vertices to 2D (XY plane)
                afxReal u1 = AFX_V3D_IN(pos, posStride, ia)->v[0], v1y = AFX_V3D_IN(pos, posStride, ia)->v[1];
                afxReal u2 = AFX_V3D_IN(pos, posStride, ib)->v[0], v2y = AFX_V3D_IN(pos, posStride, ib)->v[1];
                afxReal u3 = AFX_V3D_IN(pos, posStride, ic)->v[0], v3y = AFX_V3D_IN(pos, posStride, ic)->v[1];
                //pos = pos2 += posStride;
                //pos += posStep;

                // Find the min and max values for the u and v coordinates
                afxReal min_u = fminf(fminf(u1, u2), u3);
                afxReal max_u = fmaxf(fmaxf(u1, u2), u3);
                afxReal min_v = fminf(fminf(v1y, v2y), v3y);
                afxReal max_v = fmaxf(fmaxf(v1y, v2y), v3y);

                // Normalize UV coordinates to the range [0, 1]
                afxReal range_u = max_u - min_u;
                afxReal range_v = max_v - min_v;

                // Store the UV coordinates for this triangle
                AFX_V2D_OUT(uv, uvStride, ia)->v[0] = (u1 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ia)->v[1] = (v1y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ib)->v[0] = (u2 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ib)->v[1] = (v2y - min_v) / range_v;

                AFX_V2D_OUT(uv, uvStride, ic)->v[0] = (u3 - min_u) / range_u;
                AFX_V2D_OUT(uv, uvStride, ic)->v[1] = (v3y - min_v) / range_v;
                //uv = uv2 += uvStride;
                //uv += uvStep;
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_ARX afxError ArxGenerateTriangleSkins(afxUnit triCnt, void const* indices, afxUnit idxStride, afxUnit stride, afxV2d uv[])
{
    afxError err = { 0 };
    AFX_ASSERT(indices);
    AFX_ASSERT(uv);

    AFX_ASSERT_ALIGNMENT(uv, AFX_PTR_ALIGNMENT);

    if (!indices)
    {
        // Loop through each triangle and generate the UVs
        for (afxUnit j = 0; j < triCnt; j++)
        {
            afxUnit const ia = j * ARX_INDICES_PER_TRI + 0;
            afxUnit const ib = j * ARX_INDICES_PER_TRI + 1;
            afxUnit const ic = j * ARX_INDICES_PER_TRI + 2;

            // Store the UV coordinates for this triangle

            if (j % 2)
            {
                AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                AFX_V2D_OUT(uv, stride, ia)->v[1] = 1;

                AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                AFX_V2D_OUT(uv, stride, ic)->v[0] = 1;
                AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
            }
            else
            {
                AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                AFX_V2D_OUT(uv, stride, ia)->v[1] = 0;

                AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                AFX_V2D_OUT(uv, stride, ic)->v[0] = 0;
                AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
            }
            //uv = uv2 += uvStride;
            //uv += uvStep;
        }
    }
    else
    {
        AFX_ASSERT((idxStride == 1) || (idxStride == 2) || (idxStride == 4));

        switch (idxStride)
        {
        case sizeof(afxUnit32):
        {
            afxUnit32 const* indices2 = indices;

            // Loop through each triangle and generate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Store the UV coordinates for this triangle

                if (j % 2)
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 1;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
                else
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
                //uv = uv2 += uvStride;
                //uv += uvStep;
            }
            break;
        }
        case sizeof(afxUnit16):
        {
            afxUnit16 const* indices2 = indices;

            // Loop through each triangle and generate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Store the UV coordinates for this triangle

                if (j % 2)
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 1;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
                else
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
            }
            break;
        }
        case sizeof(afxUnit8):
        {
            afxUnit8 const* indices2 = indices;

            // Loop through each triangle and generate the UVs
            for (afxUnit j = 0; j < triCnt; j++)
            {
                afxUnit const ia = indices2[j * ARX_INDICES_PER_TRI + 0];
                afxUnit const ib = indices2[j * ARX_INDICES_PER_TRI + 1];
                afxUnit const ic = indices2[j * ARX_INDICES_PER_TRI + 2];

                // Store the UV coordinates for this triangle

                if (j % 2)
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 1;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
                else
                {
                    AFX_V2D_OUT(uv, stride, ia)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ia)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ib)->v[0] = 1;
                    AFX_V2D_OUT(uv, stride, ib)->v[1] = 0;

                    AFX_V2D_OUT(uv, stride, ic)->v[0] = 0;
                    AFX_V2D_OUT(uv, stride, ic)->v[1] = 1;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_ARX afxError ArxDestripifyTriangles(afxUnit triCnt, void const* src, afxUnit srcIdxSiz, void* dst, afxUnit dstIdxSiz)
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    afxByte const* src2 = src;
    afxByte* dst2 = dst;
    afxUnit stripLen = triCnt + 2;

    afxUnit stride = AFX_MAX(dstIdxSiz, AFX_MIN(dstIdxSiz, srcIdxSiz));

    for (afxUnit i = 0; i < stripLen - 2; i++)
    {
        if (i % 2 == 0)
        {
            // Even index: v0, v1, v2
            for (afxUnit j = 0; j < ARX_INDICES_PER_TRI; j++)
                AfxCopy(&dst2[dstIdxSiz * i + j], &src2[srcIdxSiz * i + j], stride);
        }
        else
        {
            // Odd index: v1, v2, v0
            afxUnit idx0;
            // to allowed in-place conversion, we must cache at least of indices here before swapping.
            AfxCopy(&idx0, &src2[srcIdxSiz * i + 0], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 0], &src2[srcIdxSiz * i + 1], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 1], &src2[srcIdxSiz * i + 2], stride);
            AfxCopy(&dst2[dstIdxSiz * i + 2], &idx0, stride);
        }
    }
    return err;
}

_ARXINL void ArxEndMeshBuilding(arxTriangulation* mshb)
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);

    AfxEmptyArray(&mshb->biases, FALSE, FALSE);

    if (mshb->faces)
        AfxDeallocate(AfxHere(), (void**)&mshb->faces);

    if (mshb->vtx)
        AfxDeallocate(AfxHere(), (void**)&mshb->vtx);

    if (mshb->posn)
        AfxDeallocate(AfxHere(), (void**)&mshb->posn);

    if (mshb->nrm)
        AfxDeallocate(AfxHere(), (void**)&mshb->nrm);

    if (mshb->uv)
        AfxDeallocate(AfxHere(), (void**)&mshb->uv);

    if (mshb->pivots)
        AfxDeallocate(AfxHere(), (void**)&mshb->pivots);

    //AfxAssignFcc(mshb, NIL);
}

_ARXINL afxError ArxBeginMeshBuilding(arxTriangulation* mshb, afxString const* id, afxUnit vtxCnt, afxUnit triCnt, afxUnit secCnt, afxUnit artCnt)
{
    afxError err = { 0 };
    AFX_ASSERT(mshb);
    AFX_ASSERT(vtxCnt);
    AFX_ASSERT(triCnt);
    AFX_ASSERT(secCnt);
    AFX_ASSERT(artCnt);
    mshb->vtxCnt = vtxCnt;
    mshb->artCnt = artCnt;
    mshb->triCnt = triCnt;
    mshb->secCnt = secCnt;
    mshb->faces = NIL;

    if (secCnt && AfxAllocate(AfxHere(), triCnt * sizeof(mshb->faces[0]), 0, (void**)&mshb->faces)) AfxThrowError();
    else
    {
        AfxZero(mshb->faces, sizeof(mshb->faces[0]) * triCnt);
    }

    AfxMakeString32(&mshb->id, id);

    AfxMakeArray(&mshb->biases, sizeof(arxVertexBias), artCnt, (arxVertexBias[]) { 0 }, 0);

    AfxAllocate(AfxHere(), vtxCnt * sizeof(mshb->vtx[0]), NIL, (void**)&mshb->vtx);
    AfxZero(mshb->vtx, sizeof(mshb->vtx[0]) * vtxCnt);

    AfxAllocate(AfxHere(), artCnt * sizeof(mshb->pivots[0]), NIL, (void**)&mshb->pivots);

    AfxAllocate(AfxHere(), vtxCnt * sizeof(mshb->posn[0]), AFX_SIMD_ALIGNMENT, (void**)&mshb->posn);
    mshb->nrm = NIL;
    mshb->uv = NIL;

    //AfxAssignFcc(mshb, afxFcc_MSHB);

    return err;
}

_ARXINL void ArxUpdateVertices(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxUnit const baseBiasIdx[], afxUnit const biasCnt[])
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);

    for (afxUnit i = 0; i < vtxCnt; i++)
    {
        akxVertex *v = &mshb->vtx[baseVtxIdx + i];
        //AfxV4dCopyAtv3d(v->posn, posn[i]);
        v->baseBiasIdx = baseBiasIdx ? baseBiasIdx[i] : 0;
        v->biasCnt = biasCnt ? biasCnt[i] : 1;
        //AfxV2dCopy(v->uv, uv ? uv[i] : AFX_V2D(0, 0));

        AFX_ASSERT_RANGE(mshb->biases.pop, v->baseBiasIdx, v->biasCnt);
    }
}

_ARXINL void ArxUpdateVertexPositions(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const posn[], afxUnit srcStride)
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(posn);

    if (srcStride == sizeof(posn[0]))
    {
        for (afxUnit i = 0; i < vtxCnt; i++)
            mshb->posn[baseVtxIdx + i] = AfxV4dFromAtv3d(posn[i]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;
        
        for (afxUnit i = 0; i < vtxCnt; i++)
            mshb->posn[baseVtxIdx + i] = AfxV4dFromAtv3d(*(afxV3d*)(&posnBytemap[i * srcStride]));
    }
}

_ARXINL void ArxUpdateVertexPositions4(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV4d const posn[], afxUnit srcStride)
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(posn);

    if (srcStride == sizeof(posn[0]))
    {
        AfxCopyArrayedV4d(vtxCnt, posn, &mshb->posn[baseVtxIdx]);
    }
    else
    {
        afxByte* posnBytemap = (void*)posn;

        for (afxUnit i = 0; i < vtxCnt; i++)
            mshb->posn[baseVtxIdx + i] = *(afxV4d*)(&posnBytemap[i * srcStride]);
    }
}

_ARXINL afxError ArxUpdateVertexNormals(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV3d const nrm[], afxUnit srcStride)
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(nrm);

    if (!mshb->nrm && AfxAllocate(AfxHere(), mshb->vtxCnt * sizeof(mshb->nrm[0]), AFX_SIMD_ALIGNMENT, (void**)&mshb->nrm))
        AfxThrowError();

    if (!err)
    {
        if (srcStride == sizeof(nrm[0]))
        {
            AfxCopyArrayedV3d(vtxCnt, nrm, &mshb->nrm[baseVtxIdx]);
        }
        else
        {
            afxByte* nrmBytemap = (void*)nrm;

            for (afxUnit i = 0; i < vtxCnt; i++)
                mshb->nrm[baseVtxIdx + i] = *(afxV3d*)(&nrmBytemap[i * srcStride]);
        }
    }
    return err;
}

_ARXINL afxError ArxUpdateVertexWraps(arxTriangulation* mshb, afxUnit baseVtxIdx, afxUnit vtxCnt, afxV2d const uv[], afxUnit srcStride)
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AFX_ASSERT(uv);

    if (!mshb->uv && AfxAllocate(AfxHere(), mshb->vtxCnt * sizeof(mshb->uv[0]), AFX_SIMD_ALIGNMENT, (void**)&mshb->uv))
        AfxThrowError();

    if (!err)
    {
        if (srcStride == sizeof(uv[0]))
        {
            AfxCopyArrayedV2d(vtxCnt, uv, &mshb->uv[baseVtxIdx]);
        }
        else
        {
            afxByte* uvBytemap = (void*)uv;

            for (afxUnit i = 0; i < vtxCnt; i++)
                mshb->uv[baseVtxIdx + i] = *(afxV2d*)(&uvBytemap[i * srcStride]);
        }
    }
    
    return err;
}

_ARXINL void ArxEmitTriangles(arxTriangulation* mshb, afxUnit secIdx, afxUnit baseTriIdx, afxUnit triCnt, afxUnit const triVtxIdx[][3])
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->secCnt, secIdx, 1);
    AFX_ASSERT_RANGE(mshb->triCnt, baseTriIdx, triCnt);

    for (afxUnit i = 0; i < triCnt; i++)
    {
        mshb->faces[baseTriIdx + i].vtxIdx[0] = triVtxIdx[i][0];
        mshb->faces[baseTriIdx + i].vtxIdx[1] = triVtxIdx[i][1];
        mshb->faces[baseTriIdx + i].vtxIdx[2] = triVtxIdx[i][2];
        mshb->faces[baseTriIdx + i].secIdx = secIdx;
        mshb->faces[baseTriIdx + i].artCnt = 0;

        AFX_ASSERT(secIdx == mshb->faces[baseTriIdx + i].secIdx);
        AFX_ASSERT_RANGE(mshb->secCnt, mshb->faces[baseTriIdx + i].secIdx, 1);
    }
}

_ARXINL afxUnit ArxAddVertexBiases(arxTriangulation* mshb, afxUnit cnt, afxUnit const jntIdx[], afxReal const weight[])
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AfxReserveArraySpace(&mshb->biases, mshb->biases.pop + cnt);

    afxUnit baseBiasIdx = AFX_INVALID_INDEX;
    AfxPushArrayUnits(&mshb->biases, cnt, &baseBiasIdx, NIL, FALSE);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT(weight && 1.f >= weight[i]);
        AfxUpdateAtArray(&mshb->biases, baseBiasIdx + i, 1, 0, (const arxVertexBias[]) { { .pivotIdx = jntIdx ? jntIdx[i] : 0, .weight = weight ? weight[i] : 1.f } }, sizeof(arxVertexBias));
        AFX_ASSERT_RANGE(mshb->artCnt, ((arxVertexBias const*)AfxGetAtArray(&mshb->biases, baseBiasIdx + i))->pivotIdx, 1);
    }
    return baseBiasIdx;
}

_ARXINL void ArxRenameVertexPivots(arxTriangulation* mshb, afxUnit basePivotIdx, afxUnit cnt, afxString const name[])
{
    afxError err = { 0 };
    //AfxAssertType(mshb, afxFcc_MSHB);
    AFX_ASSERT_RANGE(mshb->artCnt, basePivotIdx, cnt);
    AFX_ASSERT(name);

    for (afxUnit i = 0; i < cnt; i++)
    {
        mshb->pivots[basePivotIdx + i] = name[i];
    }
}
