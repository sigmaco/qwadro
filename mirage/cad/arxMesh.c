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

AFX_DEFINE_STRUCT(_acxCollidableMesh)
{
    arxMesh msh;
};

AFX_DEFINE_STRUCT(_arxRenderableMesh)
{
    arxMesh msh;
    afxUnit reqCnt;
};

ASX afxError AfxAcquireMeshTopology(arxScenario scio, arxMeshBlueprint const* blueprint, arxMeshTopology* topology);

////////////////////////////////////////////////////////////////////////////////

_ARXINL arxScenario ArxGetMeshHost(arxMesh msh)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    arxScenario scio = AfxGetHost(msh);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio;
}

_ARXINL afxBool ArxGetMeshUrn(arxMesh msh, afxString* id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(id);
    *id = msh->urn;
    return msh->urn.len;
}

_ARXINL afxBool ArxIsMeshDeformable(arxMesh msh)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    return (1 < msh->biasCnt);
}

_ARXINL afxUnit* ArxGetMeshIndices(arxMesh msh, afxUnit baseTriIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->triCnt, baseTriIdx, 1);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    baseTriIdx = AFX_MIN(baseTriIdx, msh->triCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    if (!msh->indices)
    {
#if 0
        arxBuffer buf;
        arxBufferInfo bufi = { 0 };
        bufi.size = sizeof(msh->indices[0]) * msh->idxCnt;
        bufi.usage = arxBufferUsage_INDEX;
        bufi.flags = arxBufferFlag_RWX;

        if (AsxAcquireBuffers(AfxGetHost(AfxGetHost(msh)), 1, &bufi, &buf))
        {
            AfxThrowError();
        }
#endif

        if (AfxAllocate(msh->idxCnt * sizeof(msh->indices[0]), AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&msh->indices))
        {
            AfxThrowError();
            return NIL;
        }
    }
    return &msh->indices[baseTriIdx * ARX_INDICES_PER_TRI];
}

_ARXINL void ArxDescribeMesh(arxMesh msh, arxMeshInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(info);
    info->topology = msh->topology;
    info->vtxCnt = msh->vtxCnt;
    info->attrCnt = msh->attrCnt;
    info->biasCnt = msh->biasCnt;
    info->triCnt = msh->triCnt;
    info->idxCnt = msh->idxCnt;
    info->edgeCnt = msh->triCnt * ARX_INDICES_PER_TRI;
    info->morphCnt = msh->morphCnt;
    info->secCnt = msh->secCnt;
    info->mtlCnt = msh->mtlCnt;
    info->minIdxSiz = msh->minIdxSiz;
    AfxMakeString32(&info->urn, &msh->urn);
}

_ARXINL afxUnit ArxDescribeMeshMorphes(arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph morphes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, baseMorphIdx, cnt);
    
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    cnt = AFX_MIN(cnt, msh->morphCnt - baseMorphIdx);
    baseMorphIdx = AFX_MIN(baseMorphIdx, msh->morphCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    for (afxUnit i = 0; i < cnt; i++)
        morphes[i] = msh->morphs[baseMorphIdx + i];

    return cnt;
}

_ARXINL afxError ArxReformMesh(arxMesh msh, afxUnit baseMorphIdx, afxUnit cnt, arxMeshMorph const morphes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(morphes);

    afxUnit mshMorphCnt = msh->morphCnt;
    AFX_ASSERT_RANGE(mshMorphCnt, baseMorphIdx, cnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    cnt = AFX_MIN(cnt, mshMorphCnt - baseMorphIdx);
    baseMorphIdx = AFX_MIN(baseMorphIdx, mshMorphCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    for (afxUnit morphIdx, i = 0; i < cnt; i++)
    {
        morphIdx = baseMorphIdx + i;
        arxMeshMorph* mshm = &msh->morphs[morphIdx];
        arxMeshMorph const* in = &morphes[i];

        mshm->flags = in->flags;
        mshm->baseVtx = in->baseVtx;

        if (in->affectedAttrs && morphIdx != 0)
            mshm->affectedAttrs = in->affectedAttrs;

        ArxRecomputeMeshBounds(msh, morphIdx, 0, msh->secCnt, 0);
    }
    return err;
}

_ARXINL afxUnit ArxDescribeMeshBiases(arxMesh msh, afxUnit baseBiasIdx, afxUnit cnt, arxMeshBias biases[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(biases);

    afxUnit mshBiasCnt = msh->biasCnt;
    AFX_ASSERT_RANGE(mshBiasCnt, baseBiasIdx, cnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    cnt = AFX_MIN(cnt, mshBiasCnt - baseBiasIdx);
    baseBiasIdx = AFX_MIN(baseBiasIdx, mshBiasCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    for (afxUnit biasIdx, i = 0; i < cnt; i++)
    {
        biasIdx = baseBiasIdx + i;
        biases[i] = msh->biases[biasIdx];
    }
    return cnt;
}

_ARXINL afxString* ArxGetMeshBiasTags(arxMesh msh, afxUnit baseBiasIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->biasCnt, baseBiasIdx, 1);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    baseBiasIdx = AFX_MIN(baseBiasIdx, msh->biasCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    return &msh->biasId[baseBiasIdx];
}

_ARXINL afxString* ArxGetMeshMaterials(arxMesh msh, afxUnit baseMtlIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->mtlCnt, baseMtlIdx, 1);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    baseMtlIdx = AFX_MIN(baseMtlIdx, msh->mtlCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    return &msh->materials[baseMtlIdx];
}

_ARXINL afxUnit ArxDescribeMeshSections(arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection sections[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(sections);

    afxUnit mshSecCnt = msh->secCnt;
    AFX_ASSERT_RANGE(mshSecCnt, baseSecIdx, cnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    cnt = AFX_MIN(cnt, mshSecCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, mshSecCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    for (afxUnit secIdx, i = 0; i < cnt; i++)
    {
        secIdx = baseSecIdx + i;
        sections[i] = msh->sections[secIdx];
    }
    return cnt;
}

_ARXINL afxError ArxSectionizeMesh(arxMesh msh, afxUnit baseSecIdx, afxUnit cnt, arxMeshSection const sections[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(sections);

    afxUnit mshSecCnt = msh->secCnt;
    AFX_ASSERT_RANGE(mshSecCnt, baseSecIdx, cnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    cnt = AFX_MIN(cnt, mshSecCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, mshSecCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    for (afxUnit secIdx, i = 0; i < cnt; i++)
    {
        secIdx = baseSecIdx + i;
        arxMeshSection* dst = &msh->sections[secIdx];
        arxMeshSection const* src = &sections[i];
        
        AFX_ASSERT_RANGE(msh->triCnt, src->baseTriIdx, src->triCnt);
        AFX_ASSERT_RANGE(msh->mtlCnt, src->mtlIdx, 1);

        // sanitize arguments
        dst->baseTriIdx = AFX_MIN(src->baseTriIdx, msh->triCnt - 1);
        dst->triCnt = AFX_MIN(src->triCnt, msh->triCnt - dst->baseTriIdx);
        dst->mtlIdx = AFX_MIN(src->mtlIdx, msh->mtlCnt - 1);
    }
    return err;
}

_ARXINL afxUnit ArxRevestMeshSections(arxMesh msh, afxUnit baseSecIdx, afxUnit secCnt, afxUnit mtlIdxCnt, afxUnit const mtlIdxLut[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(mtlIdxLut);
    afxUnit rslt = 0;

    afxUnit mshSecCnt = msh->secCnt;
    AFX_ASSERT_RANGE(mshSecCnt, baseSecIdx, secCnt);    
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    secCnt = AFX_MIN(secCnt, mshSecCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, mshSecCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    arxMeshSection mshs;

    for (afxUnit secIdx, i = 0; i < mshSecCnt; i++)
    {
        secIdx = baseSecIdx + i;
        ArxDescribeMeshSections(msh, secIdx, 1, &mshs);

        if (mshs.mtlIdx < mtlIdxCnt)
        {
            mshs.mtlIdx = mtlIdxLut[mshs.mtlIdx];
            ++rslt;
        }
    }
    return rslt;
}

_ARXINL void ArxInvertMeshTopology(arxMesh msh)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit triCnt = msh->triCnt;
    afxUnit* indices = ArxGetMeshIndices(msh, 0);
    
    for (afxUnit vtxIdx, i = 0; i < triCnt; i++)
    {
        vtxIdx = indices[i * ARX_INDICES_PER_TRI + 0];
        indices[i * ARX_INDICES_PER_TRI + 0] = indices[i * ARX_INDICES_PER_TRI + 2];
        indices[i * ARX_INDICES_PER_TRI + 2] = vtxIdx;
    }

    afxUnit* adjacent = msh->sideToAdjacentMap;

    if (adjacent)
    {
        afxUnit edgeCnt = msh->idxCnt / ARX_INDICES_PER_TRI;

        for (afxUnit edgeIdx, i = 0; i < edgeCnt; ++i)
        {
            afxUnit edgeIdx = adjacent[i + 0];
            adjacent[i + 0] = adjacent[i + 1];
            adjacent[i + 1] = edgeIdx;

            afxUnit32 faceA = adjacent[i + 0] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeA = adjacent[i + 0] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            afxUnit32 faceB = adjacent[i + 1] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeB = adjacent[i + 1] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            afxUnit32 faceC = adjacent[i + 2] & 0xFFFFFFFC; // the high-order 30 bits specify the actual triangle index - you would multiply it by three to get its location
            afxUnit32 edgeC = adjacent[i + 2] & 0x00000003; // 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined
            
            adjacent[i + 0] = (edgeA = (edgeA == 0x01 ? 0x00 : (edgeA == 0x00 ? 0x01 : edgeA))) | faceA;
            adjacent[i + 1] = (edgeB = (edgeB == 0x01 ? 0x00 : (edgeB == 0x00 ? 0x01 : edgeB))) | faceB;
            adjacent[i + 2] = (edgeC = (edgeC == 0x01 ? 0x00 : (edgeC == 0x00 ? 0x01 : edgeC))) | faceC;
        }
    }
}

_ARXINL afxUnit ArxFindVertexAttributes(arxMesh msh, afxUnit cnt, afxString const usages[], afxUnit indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(indices);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
        indices[i] = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < msh->attrCnt; i++)
    {
        afxString str = msh->attrInfo[i].usage.s;

        if (AfxCompareStrings(&usages[rslt], 0, TRUE, 1, &str, NIL))
            indices[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_ARXINL afxError ArxDescribeVertexAttribute(arxMesh msh, afxUnit attrIdx, arxVertexAttribute* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(info);

    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    info->fmt = msh->attrInfo[attrIdx].fmt;
    info->flags = msh->attrInfo[attrIdx].flags;
    AfxMakeString8(&info->usage, &msh->attrInfo[attrIdx].usage.s);
    return err;
}

_ARXINL afxError ArxFormatVertexAttribute(arxMesh msh, afxUnit attrIdx, avxFormat fmt, arxVertexFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    arxMeshAttr* attr = &msh->attrInfo[attrIdx];
    attr->fmt = fmt;
    attr->flags = flags;
    //AfxCopyString(&attr->usage.s, 0, usage, 0);
    AfxResetBoxes(1, &attr->aabb, 0);
    attr->aabbUpdReq = TRUE;

    // Fix for usage. But what about the storage? The usage should be specified in blueprint for allocation.
    // Also, what about attr->usage in mesh?
    //AfxCopyString(&msh->attrIds[attrIdx], 0, usage, 0);

    if (msh->vtxAttrData[attrIdx])
        AfxDeallocate((void**)&msh->vtxAttrData[attrIdx], AfxHere());

    return err;
}

_ARXINL void* ArxAccessVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, 1);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    attrIdx = AFX_MIN(attrIdx, msh->attrCnt - 1);
    baseVtx = AFX_MIN(baseVtx, msh->vtxCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    avxFormat fmt = msh->attrInfo[attrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);
    
    afxUnit32 attrBit = AFX_BITMASK(attrIdx);

    if (!(msh->morphs[morphIdx].affectedAttrs & attrBit))
        return NIL;

    if (!msh->vtxAttrData[attrIdx])
    {
        afxUnit32 dataSiz = 0;

        for (afxUnit i = 0; i < msh->morphCnt; i++)
            if (msh->morphs[i].affectedAttrs & attrBit)
                dataSiz += msh->vtxCnt * unitSiz;

        if (AfxAllocate(dataSiz, AFX_SIMD_ALIGNMENT, AfxHere(), (void**)&msh->vtxAttrData[attrIdx]))
        {
            AfxThrowError();
            return NIL;
        }
    }

    afxUnit32 dataOff = 0;

    for (afxUnit i = 0; i < morphIdx; i++)
        if (msh->morphs[i].affectedAttrs & attrBit)
            dataOff += msh->vtxCnt * unitSiz;

    afxByte* data = &msh->vtxAttrData[attrIdx][dataOff + (baseVtx * unitSiz)];

    return data;
}

_ARXINL afxError ArxInvertVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);
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

    if (!(msh->attrInfo[attrIdx].flags & arxVertexFlag_NORMALIZED))
    {
        AfxThrowError();
        return err;
    }

    // Switching the winding order to clockwise allow us to mirroring geometry via negative scale transforms.

    afxV3d* nrm = ArxAccessVertexData(msh, attrIdx, morphIdx, baseVtx);

    for (afxUnit i = 3; i < vtxCnt; i++)
        AfxV3dScale(nrm[i], nrm[i], -1.f);

    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ARXINL afxError ArxNormalizeVertexData(arxMesh msh, afxUnit attrIdx, afxUnit morphIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(msh->vtxCnt, baseVtx, vtxCnt);

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

    if (!(msh->attrInfo[attrIdx].flags & arxVertexFlag_NORMALIZED))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(fmt == avxFormat_RG32f || fmt == avxFormat_RGB32f || fmt == avxFormat_RGBA32f);
    //afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    switch (fmt)
    {
    case avxFormat_RG32f: AfxNormalizeArrayedV2d(vtxCnt, data, data); break;
    case avxFormat_RGB32f: AfxNormalizeArrayedV3d(vtxCnt, data, data); break;
    case avxFormat_RGBA32f: AfxNormalizeArrayedV4d(vtxCnt, data, data); break;
    default: AfxThrowError(); break;
    }

    msh->attrInfo[attrIdx].aabbUpdReq = TRUE;
    return err;
}

_ARXINL afxError ArxRecomputeMeshBounds(arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxUnit posAttrIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT_RANGE(msh->morphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(msh->secCnt, baseSecIdx, cnt);

#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, msh->morphCnt - 1);
    baseSecIdx = AFX_MIN(baseSecIdx, msh->secCnt - 1);
    cnt = AFX_MIN(cnt, msh->secCnt - baseSecIdx);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    arxMeshMorph mshm;
    if (!ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    afxBox* morphSecAabbs = &msh->secAabb[morphIdx * msh->secCnt];
    AfxResetBoxes(cnt, morphSecAabbs, 0);

    void* data = ArxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);

    afxUnit triCnt = msh->triCnt;
    afxUnit secCnt = msh->secCnt;
    

    avxFormat attrFmt = msh->attrInfo[posAttrIdx].fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &attrFmt, &pfd);
    afxUnit32 unitSiz = pfd.stride;// AfxVertexFormatGetSize(fmt);

    if (attrFmt == avxFormat_RGB32f)
    {
        afxV3d* data3 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            arxMeshSection mshs;
            ArxDescribeMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = ArxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * ARX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxEmboxVectors(&morphSecAabbs[secIdx], 1, &data3[i]);
        }
    }
    else if (attrFmt == avxFormat_RGBA32f)
    {
        afxV4d* data4 = data;

        for (afxUnit j = 0; j < cnt; j++)
        {
            afxUnit secIdx = baseSecIdx + j;

            arxMeshSection mshs;
            ArxDescribeMeshSections(msh, secIdx, 1, &mshs);
            afxUnit const* indices = ArxGetMeshIndices(msh, mshs.baseTriIdx);
            afxUnit idxCnt = mshs.triCnt * ARX_INDICES_PER_TRI;

            for (afxUnit i = 0; i < idxCnt; i++)
                AfxEmboxPoints(&morphSecAabbs[secIdx], 1, &data4[i]);
        }
    }
    else AfxThrowError();

    return err;
}

_ARXINL afxError ArxUpdateMeshBounds(arxMesh msh, afxUnit morphIdx, afxUnit baseSecIdx, afxUnit cnt, afxBox const aabbs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AFX_ASSERT(aabbs);

    afxUnit mshSecCnt = msh->secCnt;
    afxUnit mshMorphCnt = msh->morphCnt;
    AFX_ASSERT_RANGE(mshMorphCnt, morphIdx, 1);
    AFX_ASSERT_RANGE(mshSecCnt, baseSecIdx, cnt);
#ifdef _ARX_DBG_MESH_SANITIZE_ARGS
    // sanitize arguments
    morphIdx = AFX_MIN(morphIdx, mshMorphCnt - 1);
    cnt = AFX_MIN(cnt, mshSecCnt - baseSecIdx);
    baseSecIdx = AFX_MIN(baseSecIdx, mshSecCnt - 1);
#endif//_ARX_DBG_MESH_SANITIZE_ARGS

    arxMeshMorph mshm;
    if (!ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm))
    {
        AfxThrowError();
        return err;
    }

    // Recompute it if <aabbs> is not specified.

    afxBox* morphSecAabbs = &msh->secAabb[mshSecCnt * morphIdx + baseSecIdx];
    AfxCopyBoxes(cnt, aabbs, 0, morphSecAabbs, 0);

    return err;
}

_ARXINL afxError ArxRecomputeMeshNormals(arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit nrmAttrIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    arxMeshInfo mshi;
    ArxDescribeMesh(msh, &mshi);

    AFX_ASSERT(mshi.vtxCnt == mshi.idxCnt);

    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm);

    /*
        For each face on the mesh, calc the face normal (without normalization, as just explained), and directly accumulate this normal in each vertex belonging to the face.
        After you are done with the faces, each vertex will have recieved all the face normals it was supposed to recieve. That simple.
    */

    afxUnit const* indices = ArxGetMeshIndices(msh, 0);

    afxV3d const* pos = ArxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);
    afxV3d* nrm = ArxAccessVertexData(msh, nrmAttrIdx, morphIdx, mshm.baseVtx);

    ArxUpdateVertexData(msh, nrmAttrIdx, morphIdx, mshm.baseVtx, msh->vtxCnt, AFX_V3D_ZERO, 0); // zero it

    ArxComputeTriangleNormals(mshi.triCnt, indices, sizeof(indices[0]), pos, sizeof(pos[0]), nrm, sizeof(nrm[0]));
    
    return err;
}

_ARXINL afxError ArxRecomputeMeshTangents(arxMesh msh, afxUnit morphIdx, afxUnit posAttrIdx, afxUnit uvAttrIdx, afxUnit tanAttrIdx, afxUnit bitAttrIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    arxMeshInfo mshi;
    ArxDescribeMesh(msh, &mshi);

    arxMeshMorph mshm;
    ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm);

    AFX_ASSERT(mshi.vtxCnt == mshi.idxCnt);

    afxUnit const* indices = ArxGetMeshIndices(msh, 0);

    afxV3d const* pos = ArxAccessVertexData(msh, posAttrIdx, morphIdx, mshm.baseVtx);
    afxV2d const* uv = ArxAccessVertexData(msh, uvAttrIdx, morphIdx, mshm.baseVtx);

    afxV3d* tan = ArxAccessVertexData(msh, tanAttrIdx, morphIdx, mshm.baseVtx);
    afxV3d* bit = ArxAccessVertexData(msh, bitAttrIdx, morphIdx, mshm.baseVtx);

    ArxComputeTriangleTangents(mshi.triCnt, indices, sizeof(indices[0]), pos, sizeof(pos[0]), uv, sizeof(uv), FALSE, tan, sizeof(tan), bit, sizeof(bit));
    
    return err;
}

_ARX afxError _ArxMshDtorCb(arxMesh msh)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit idxCnt = msh->idxCnt;
    afxUnit edgeCnt = msh->idxCnt;

    if (msh->morphs)
    {
        afxUnit morphCnt = msh->morphCnt;

        for (afxUnit i = 0; i < morphCnt; i++)
        {
            arxMeshMorph* morph = &msh->morphs[i];

        }
    }

    for (afxUnit i = 0; i < msh->attrCnt; i++)
    {
        if (msh->vtxAttrData[i])
        {
            AfxDeallocate((void**)&msh->vtxAttrData[i], AfxHere());
        }
    }

    if (msh->indices)
        AfxDeallocate((void**)&msh->indices, AfxHere());

    afxObjectStash stashes[] =
    {
        {
            .cnt = msh->sideToAdjacentMap ? edgeCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = msh->mtlCnt,
            .siz = sizeof(msh->materials[0]),
            .var = (void**)&msh->materials
        },
        {
            .cnt = msh->secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasId[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasId
        },
        // VERTEX DATA
        {
            .cnt = msh->vtxToVtxMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->vtxToVtxMap[0]),
            .var = (void**)&msh->vtxToVtxMap
        },
        {
            .cnt = msh->vtxToTriMap ? msh->vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .var = (void**)&msh->vtxToTriMap
        },
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->attrInfo[0]),
            .var = (void**)&msh->attrInfo
        },
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->attrIds[0]),
            .var = (void**)&msh->attrIds
        },
        {
            .cnt = msh->attrCnt,
            .siz = sizeof(msh->vtxAttrData[0]),
            .var = (void**)&msh->vtxAttrData
        },
        // SHAPE DATA
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphTags[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphTags
        },
        {
            .cnt = msh->secCnt * msh->morphCnt,
            .siz = sizeof(msh->secAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->secAabb
        },
        {
            .cnt = msh->biasCnt * msh->morphCnt,
            .siz = sizeof(msh->biasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasObb
        },
    };

    if (AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!msh->morphs);
    AFX_ASSERT(!msh->morphTags);
    AFX_ASSERT(!msh->biases);
    AFX_ASSERT(!msh->biasId);
    AFX_ASSERT(!msh->biasObb);

    AFX_ASSERT(!msh->indices);

    if (msh->vtxCache.buf)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->vtxCache.buf);
        AfxDisposeObjects(1, &msh->vtxCache.buf);
    }

    if (msh->ibo)
    {
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->ibo);
        AfxDisposeObjects(1, &msh->ibo);
    }

    return err;
}

_ARX afxError _ArxMshCtorCb(arxMesh msh, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxString const* urn = !args[1] ? &(afxString const) {0} : AFX_CAST(afxString const*, args[1]) + invokeNo;
    AFX_ASSERT(args[2]);
    arxMeshBlueprint const* blueprint = AFX_CAST(arxMeshBlueprint const*, args[2]) + invokeNo;

    if (!AfxCatalogStrings(_ArxScioGetModelUrnStringBase(scio), 1, urn, &msh->urn))
        AfxThrowError();

    afxUnit secCnt = AFX_MAX(1, blueprint->secCnt);
    AFX_ASSERT(secCnt);

    afxUnit mtlCnt = AFX_MAX(1, blueprint->mtlCnt);
    
    afxUnit triCnt = blueprint->triCnt;
    AFX_ASSERT(triCnt);

    afxUnit attrCnt = blueprint->attrCnt;
    AFX_ASSERT(attrCnt);
    afxUnit vtxCnt = blueprint->vtxCnt;
    AFX_ASSERT(vtxCnt);
    afxUnit edgeCnt = triCnt * ARX_INDICES_PER_TRI;
    afxUnit idxCnt = triCnt * ARX_INDICES_PER_TRI;

    afxUnit morphCnt = AFX_MAX(1, blueprint->morphCnt);
    afxUnit biasCnt = AFX_MAX(1, blueprint->biasCnt);

    msh->morphs = NIL;
    msh->morphTags = NIL;
    msh->materials = NIL;
    msh->biases = NIL;
    msh->biasId = NIL;
    msh->biasObb = NIL;
    msh->secAabb = NIL;
    msh->topology = avxTopology_TRI_LIST;

    afxObjectStash stashes[] =
    {
        // TOPOLOGY DATA
        {
            .cnt = blueprint->sideToAdjacentMap ? edgeCnt : 0,
            .siz = sizeof(msh->sideToAdjacentMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->sideToAdjacentMap
        },
        {
            .cnt = mtlCnt,
            .siz = sizeof(msh->materials[0]),
            .var = (void**)&msh->materials
        },
        {
            .cnt = secCnt,
            .siz = sizeof(msh->sections[0]),
            .var = (void**)&msh->sections
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biases
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(msh->biasId[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasId
        },
        // VERTEX DATA
        
        {
            .cnt = blueprint->vtxToVtxMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToVtxMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->vtxToVtxMap
        },
        {
            .cnt = blueprint->vtxToTriMap ? vtxCnt : 0,
            .siz = sizeof(msh->vtxToTriMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->vtxToTriMap
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->attrInfo[0]),
            .var = (void**)&msh->attrInfo
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->attrIds[0]),
            .var = (void**)&msh->attrIds
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(msh->vtxAttrData[0]),
            .var = (void**)&msh->vtxAttrData
        },
        // SHAPE DATA
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = morphCnt,
            .siz = sizeof(msh->morphTags[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->morphTags
        },
        {
            .cnt = morphCnt * secCnt,
            .siz = sizeof(msh->secAabb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->secAabb
        },
        {
            .cnt = morphCnt * biasCnt,
            .siz = sizeof(msh->biasObb[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&msh->biasObb
        }
    };

    if (AfxAllocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(msh->morphs);
    //AFX_ASSERT(!extraMorphCnt || msh->extraMorphTagMap);
    AFX_ASSERT(!biasCnt || msh->biases);
    AFX_ASSERT(!biasCnt || msh->biasId);
    AFX_ASSERT(!biasCnt || msh->biasObb);

    msh->topology = avxTopology_TRI_LIST;
    msh->triCnt = triCnt;

    // materials

    AFX_ASSERT(mtlCnt);
    msh->mtlCnt = mtlCnt;

    if (!blueprint->materials)
        AfxResetStrings(mtlCnt, msh->materials);
    else if (mtlCnt != (afxUnit)AfxCatalogStrings(_ArxScioGetMorphTagStringBase(scio), mtlCnt, blueprint->materials, msh->materials))
        AfxThrowError();

#if 0
    // Temporary workaround to declare materials.
    // TODO: Make it adequate. Transfer to mesh rig?
    arxMaterial materials[256];
    ArxDeclareMaterials(scio, mtlCnt, msh->materials, materials);
#endif

    // sections

    AFX_ASSERT(secCnt);
    msh->secCnt = secCnt;

    afxUnit surTriDiv = triCnt / secCnt;
    afxUnit baseTriIdx = 0;

    for (afxUnit i = 0; i < secCnt; i++)
    {
        arxMeshSection* mshs = &msh->sections[i];

        if (!blueprint->sections)
        {
            mshs->mtlIdx = 0; // Must be at least one material
            mshs->baseTriIdx = baseTriIdx;
            mshs->triCnt = surTriDiv;
            baseTriIdx += surTriDiv;
        }
        else
        {
            arxMeshSection const* in = &blueprint->sections[i];
            AFX_ASSERT_RANGE(msh->triCnt, in->baseTriIdx, in->triCnt);
            AFX_ASSERT_RANGE(msh->mtlCnt, in->mtlIdx, 1);
            mshs->baseTriIdx = AFX_MIN(in->baseTriIdx, msh->triCnt - 1);
            mshs->triCnt = AFX_MIN(in->triCnt, msh->triCnt - mshs->baseTriIdx);
            mshs->mtlIdx = AFX_MIN(in->mtlIdx, msh->mtlCnt - 1);
            mshs->flags = in->flags;
        }
    }

    // deformation data

    AFX_ASSERT(biasCnt);
    msh->biasCnt = biasCnt;

    AFX_ASSERT(biasCnt < 2 || blueprint->biases);

    if (!blueprint->biases)
        AfxResetStrings(biasCnt, msh->biasId);
    else if (biasCnt != (afxUnit)AfxCatalogStrings(_ArxScioGetPivotTagStringBase(scio), biasCnt, blueprint->biases, msh->biasId))
        AfxThrowError();

    for (afxUnit i = 0; i < biasCnt; i++)
    {
        arxMeshBias* bias = &msh->biases[i];
        bias->triCnt = 0;
        bias->tris = NIL;
    }

    AfxResetBoxes(morphCnt * biasCnt, msh->biasObb, 0);

    msh->jointsForTriCnt = 0;
    msh->jointsForTriMap = NIL; // jointsForTriMap[triIdx] = num of joints influencing this triangle?
    msh->triToJointCnt = 0;
    msh->triToJointMap = NIL; // triToJointMap[jntIdx] = num of triangles influenced by this joint?

    // vertex data

    msh->vtxCnt = vtxCnt;
    msh->minIdxSiz = (msh->vtxCnt > AFX_U8_MAX ? (msh->vtxCnt > AFX_U16_MAX ? sizeof(afxUnit32) : sizeof(afxUnit16)) : sizeof(afxUnit8));
    msh->idxCnt = idxCnt;

    msh->attrCnt = attrCnt;
    afxMask allAttrEnabledMask = NIL;

    if (!blueprint->attrs)
        AfxResetStrings(attrCnt, msh->attrIds);
    else if (attrCnt != (afxUnit)AfxCatalogStrings(_ArxScioGetMorphTagStringBase(scio), attrCnt, blueprint->attrs, msh->attrIds))
        AfxThrowError();

    for (afxUnit i = 0; i < attrCnt; i++)
    {
        arxMeshAttr* attr = &msh->attrInfo[i];

        allAttrEnabledMask |= AFX_BITMASK(i);

        AfxMakeString8(&attr->usage, NIL);
        attr->flags = NIL;
        attr->fmt = avxFormat_RGBA32f;

        msh->vtxAttrData[i] = NIL;
    }

    // morph/shape data

    msh->morphCnt = morphCnt;

    for (afxUnit i = 0; i < morphCnt; i++)
    {
        arxMeshMorph* dst = &msh->morphs[i];
        arxMeshMorph const* src;

        if (i == 0)
        {
            dst->baseVtx = 0;
            dst->flags = 0;
            dst->affectedAttrs = allAttrEnabledMask;
        }
        else
        {
            if (!blueprint->morphs)
                src = (arxMeshMorph const[]) { 0 };
            else
                src = &blueprint->morphs[i];

            dst->baseVtx = src->baseVtx;
            dst->flags = (src->flags & arxMeshMorphFlag_DELTA);
            dst->affectedAttrs = src->affectedAttrs ? src->affectedAttrs : allAttrEnabledMask;
        }
    }

    if (!blueprint->morphTags)
        AfxResetStrings(morphCnt, msh->morphTags);
    else if (morphCnt != (afxUnit)AfxCatalogStrings(_ArxScioGetMorphTagStringBase(scio), morphCnt, blueprint->morphTags, msh->morphTags))
        AfxThrowError();

    AfxResetBoxes(morphCnt * secCnt, msh->secAabb, 0);

    msh->vtxCache.buf = NIL;
    msh->ibo = NIL;
    msh->iboBase = 0;
    msh->iboRange = 0;
    msh->iboStride = 0;

    msh->idd = NIL;
    msh->udd = NIL;

    if (!err)
    {
#if !0
        // EXHIBIT STATISTICAS
        afxString s;
        ArxGetMeshUrn(msh, &s);

        AfxReportMessage("%.*s Mesh <%.*s> built at %p.\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u sections.\n    Listing %u biases..:",
            AfxPushString(msh->biasCnt > 1 ? &AFX_STRING("Skinned") : &AFX_STRING("Rigid")),
            AfxPushString(&s), msh, msh->vtxCnt, msh->attrCnt, msh->triCnt, msh->minIdxSiz, msh->secCnt, msh->biasCnt
        );

        for (afxUnit i = 0; i < msh->biasCnt; i++)
        {
            s = msh->biasId[i];
            AfxReportMessage("    %3u <%.*s> %3u", i, AfxPushString(&s), msh->biases ? msh->biases[i].triCnt : 0);
        }

        AfxReportMessage("    Listing %u vertex attributes..:", msh->attrCnt);

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            s = msh->attrIds[i];
            AfxReportMessage("    %3u <%.*s>", i, AfxPushString(&s));
        }
#endif
    }
    else if (AfxDeallocateInstanceData(msh, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxClassConfig const _ARX_MSH_CLASS_CONFIG =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .desc = "Morphable Topological Mesh",
    .fixedSiz = sizeof(AFX_OBJECT(arxMesh)),
    .ctor = (void*)_ArxMshCtorCb,
    .dtor = (void*)_ArxMshDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARXINL afxError ArxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(meshes);
    //AfxThrowError();
    (void)atTol;
    (void)ltTol;

    for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        arxMesh msh = meshes[mshIdx];

        if (!msh)
            continue;

        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        for (afxUnit morphIdx = 0; morphIdx < mshi.morphCnt; morphIdx++)
        {
            arxMeshMorph mshm;
            ArxDescribeMeshMorphes(msh, morphIdx, 1, &mshm);

            afxBool renormalize = (flags & 1); // sinalizado para renormalizar normals

            if (mshm.flags & arxMeshMorphFlag_DELTA)
                renormalize = FALSE; // don't renormalize data treated as delta

            for (afxUnit attrIdx = 0; attrIdx < mshi.attrCnt; attrIdx++)
            {
                afxUnit32 attrBit = AFX_BITMASK(attrIdx);

                if (!(msh->morphs[morphIdx].affectedAttrs & attrBit))
                    continue;

                arxMeshAttr* va = &msh->attrInfo[attrIdx];
                    
                void* data = ArxAccessVertexData(msh, attrIdx, morphIdx, 0);
                afxBool linearFlag = va->flags & arxVertexFlag_LTM;
                afxBool invLinearFlag = va->flags & arxVertexFlag_ILTM;
                afxBool affineFlag = va->flags & arxVertexFlag_ATV;
                afxBool deltaFlag = va->flags & arxVertexFlag_DELTA;
                afxBool normalizedFlag = va->flags & arxVertexFlag_NORMALIZED;
                avxFormat fmt = va->fmt;
                afxBool normalized;

                if (deltaFlag)
                    renormalize = FALSE; // don't renormalize data treated as delta
                
#if 0
                if (va->flags & arxVertexFlag_ATV)
                {
                    if (fmt == afxVertexFormat_V4D)
                        AfxM3dPostMultiplyLtv4d(ltm, geoi.vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                    if (!(va->flags & arxVertexFlag_DELTA))
                    {
                        if (fmt == afxVertexFormat_V4D)
                            for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                                AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                        else if (fmt == afxVertexFormat_V3D)
                            for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }
                    normalized = FALSE;
                }
#endif
                afxString const attrs[] = { AFX_STRING("pos"), AFX_STRING("tan"), AFX_STRING("bit"), AFX_STRING("tbc"), AFX_STRING("nrm") };

                afxUnit sIdx;
                if (!AfxCompareStrings(&va->usage.s, 0, FALSE, ARRAY_SIZE(attrs), attrs, &sIdx))
                    continue;

                switch (sIdx)
                {
                case 0: // pos
                {
                    if (fmt == avxFormat_RGBA32f)
                    {
                        AfxM3dPostMultiplyLtv4d(ltm, mshi.vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxUnit k = 0; k < mshi.vtxCnt; k++)
                                AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                    }
                    else if (fmt == avxFormat_RGB32f)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxUnit k = 0; k < mshi.vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }

                    normalized = FALSE;
                    break;
                }
                case 1: // tan
                {
                    AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 2: // bit
                {
                    AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 3: // tbc
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, mshi.vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                case 4: // nrm
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, mshi.vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, mshi.vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                    break;
                }
                default: break;
                }

                if (renormalize && normalized)
                {
                    if (fmt == avxFormat_RG32f)
                        AfxZeroOrNormalizeArrayedV2d(mshi.vtxCnt, data, data);
                    else if (fmt == avxFormat_RGB32f)
                        AfxZeroOrNormalizeArrayedV3d(mshi.vtxCnt, data, data);
                    else if (fmt == avxFormat_RGBA32f)
                        AfxZeroOrNormalizeArrayedV4d(mshi.vtxCnt, data, data);
                }

#if 0
                if (data && AsxGetVertexAttributeFlags(vtd2, j) & afxVertexUsage_SPATIAL)
                {
                    switch (AsxGetVertexAttributeFormat(vtd2, j))
                    {
                    case afxVertexFormat_V2D:
                    {
                        AfxThrowError();
                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxM3dPostMultiplyV3d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyV3d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);

                        if (renormalize)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dNormalize(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxM3dPostMultiplyLtv4d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyLtv4d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxUnit k = 0; k < vtxCnt; k++)
                                AfxV4dNormalize(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

                        break;
                    }
                    default: AfxThrowError(); break;
                    }
                }
#endif
            }
        }

        // When a mesh primitive uses any triangle-based topology,
        // the determinant of the transform defines the winding order of that mesh. 
        // If the determinant is a positive value, the winding order triangle faces is counterclockwise; 
        // in the opposite case, the winding order is clockwise.

        if ((flags & 2) && (AfxM3dDet(ltm) < 0.0)) // se sinalizado para reordenar índices de faces
            ArxInvertMeshTopology(msh);

        AfxTransformObbs(ltm, atv, msh->biasCnt, msh->biasObb, msh->biasObb);
    }
    return err;
}

_ARXINL afxUnit ArxEnumerateMeshes(arxScenario scio, afxUnit first, afxUnit cnt, arxMesh meshes[])
{
    afxError err = { 0 };
    // scio must be a valid arxGeome handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetMshClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    // meshes [out] must be a valid pointer to an array of arxMesh handles.
    AFX_ASSERT(meshes);
    // cnt is the number of meshes to be enumerated. Must be greater than 0.
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)meshes);
}

_ARXINL afxBool RctxIdentifyMshCb(arxMesh msh, void *udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxMesh* meshes;
        afxUnit foundCnt;
    } *udd2 = udd;

    afxUnit matchedIdx;
    if (AfxCompareStrings(&msh->urn, 0, FALSE, udd2->cnt, &udd2->ids[udd2->foundCnt], &matchedIdx))
    {
        udd2->meshes[udd2->foundCnt] = msh;
        ++udd2->foundCnt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 1;
    }
    return 0;
}

_ARXINL afxBool ArxFindMeshes(arxScenario scio, afxUnit cnt, afxString const ids[], arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetMshClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxMesh* meshes;
        afxUnit foundCnt;
    } udd =
    {
        .cnt = cnt,
        .ids = ids,
        .meshes = meshes,
        .foundCnt = 0
    };
    AfxInvokeObjects(cls, (void*)RctxIdentifyMshCb, &udd, 0, cnt);
    return udd.foundCnt;
}

_ARXINL afxError ArxBuildMeshes(arxScenario scio, afxUnit cnt, afxString const urns[], arxMeshBlueprint const blueprints[], arxMesh meshes[])
{
    afxError err = { 0 };
    // scio must be a valid arxScenario handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(blueprints);
    // meshes [out] must be a valid pointer to an array of arxMesh handles.
    AFX_ASSERT(meshes);
    // cnt is the number of meshes to be compiled. Must be greater than 0.
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetMshClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)meshes, (void const*[]) { scio, urns, blueprints }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
#ifdef _ARX_DBG_ECHO_MESH
        /*
            meshes[cnt]
            +-- mesh <name> ptr
            |   +-- attributes[cnt]
            |   |   +-- <pos>
            |   |   +-- <nrm>
            |   |   +-- <uv>
            |   +-- biases[cnt]
            |   |   +-- <bias1>
            |   |   +-- <bias2>
            |   +-- materials[cnt]
            |   |   +-- <material0>
            |   |   +-- <material1>
            |   +-- morphs[cnt]
            |       +-- <morph0>
            |
        */
        AfxReportMessage("\nmeshes[%u]", cnt);
        
        for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
        {
            arxMesh msh = meshes[mshIdx];

            AfxReportMessage("+-- msh <%.*s> %p", AfxPushString(&msh->urn), msh);
            AfxReportMessage("|   +-- topology : %u", msh->topology);
            AfxReportMessage("|   +-- triCnt : %u", msh->triCnt);
            AfxReportMessage("|   +-- vtxCnt : %u", msh->vtxCnt);

            AfxReportMessage("|   +-- attributes[%u]", msh->attrCnt);
            for (afxUnit i = 0; i < msh->attrCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->attrIds[i]));
            }

            AfxReportMessage("|   +-- materials[%u]", msh->mtlCnt);
            for (afxUnit i = 0; i < msh->mtlCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->materials[i]));
            }

            AfxReportMessage("|   +-- sections[%u]", msh->secCnt);
            for (afxUnit i = 0; i < msh->secCnt; i++)
            {
                AfxReportMessage("|   |   +-- [%u, %u] #%u", msh->sections[i].baseTriIdx, msh->sections[i].triCnt, msh->sections[i].mtlIdx);
            }

            AfxReportMessage("|   +-- biases[%u]", msh->biasCnt);
            for (afxUnit i = 0; i < msh->biasCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->biasId[i]));
            }

            AfxReportMessage("|   +-- morphes[%u]", msh->morphCnt);
            for (afxUnit i = 0; i < msh->morphCnt; i++)
            {
                AfxReportMessage("|   |   +-- <%.*s>", AfxPushString(&msh->morphTags[i]));
            }
        }
#endif//_ARX_DGB_ECHO_MESH
    }
    return err;
}
