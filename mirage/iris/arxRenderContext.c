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

#define _AFX_DRAW_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_MATERIAL_C
#define _ARX_SKELETON_C
#define _ARX_SCENARIO_C
#define _ARX_RENDER_CONTEXT_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"

AFX_DEFINE_STRUCT(_arxVisualTechnique)
{
    avxPipeline pipeline;
    afxChain    materials;
};

_ARX afxError ArxEruptIndexedTriangleVertices
(
    afxUnit triCnt,
    afxUnit const* indices,
    afxV3d const* vertices,
    afxV3d* vtxDst
)
{
    afxError err = { 0 };
    AFX_ASSERT(triCnt);

    afxUnit32* indices2 = (void*)indices;
    for (afxUnit i = 0; i < triCnt; i++)
    {
        afxUnit ia = indices2[i * ARX_INDICES_PER_TRI + 0];
        afxUnit ib = indices2[i * ARX_INDICES_PER_TRI + 1];
        afxUnit ic = indices2[i * ARX_INDICES_PER_TRI + 2];

        AfxV3dCopy(vtxDst[i * ARX_INDICES_PER_TRI], vertices[ia]);
        AfxV3dCopy(vtxDst[i * ARX_INDICES_PER_TRI], vertices[ib]);
        AfxV3dCopy(vtxDst[i * ARX_INDICES_PER_TRI], vertices[ic]);
    }
    return err;
}

_ARX afxError ArxGenerateTriangleIdentityBarycentrics
(
    afxUnit triCnt, 
    afxV3d barycentrics[]
)
{
    afxError err = { 0 };
    AFX_ASSERT(triCnt);

    for (afxUnit i = 0; i < triCnt; i++)
    {
        AfxV3dCopy(barycentrics[i * ARX_INDICES_PER_TRI], AFX_V3D_X);
        AfxV3dCopy(barycentrics[i * ARX_INDICES_PER_TRI], AFX_V3D_Y);
        AfxV3dCopy(barycentrics[i * ARX_INDICES_PER_TRI], AFX_V3D_Z);
    }
}

_ARX afxDrawSystem ArxGetRenderDrawSystem(arxRenderContext rctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    afxDrawSystem dsys = rctx->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_ARX void* ArxGetRenderContextUdd(arxRenderContext rctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    return rctx->udd;
}

_ARX void* ArxPostUniform(arxRenderContext rctx, afxUnit set, afxUnit binding, afxUnit unitCnt, afxUnit unitSiz, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(unitSiz);
    AFX_ASSERT(unitCnt);
    AFX_ASSERT(!src || (src && srcStride));

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit dataSiz = unitCnt * AFX_ALIGN_SIZE(unitSiz, AFX_SIMD_ALIGNMENT);

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = AvxRequestBufferedPump(&rctx->dynUnfmAlloc, dataSiz, &out_buffer, &out_offset);
    AFX_ASSERT(p);
    
    if (!p)
    {
        AfxThrowError();
        return NIL;
    }

    if (frame->boundDynUbos[set][binding] != out_buffer)
    {
        if (frame->boundDynUbos[set][binding])
        {
            AvxCohereMappedBuffer(frame->boundDynUbos[set][binding], frame->boundDynUbosBaseToBeFlushed[set][binding], frame->boundDynUbosRangeToBeFlushed[set][binding], NIL, FALSE);
            frame->boundDynUbos[set][binding] = NIL;
            frame->boundDynUbosBaseToBeFlushed[set][binding] = 0;
            frame->boundDynUbosRangeToBeFlushed[set][binding] = 0;
        }

        frame->boundDynUbos[set][binding] = out_buffer;

        if (out_buffer)
        {
            frame->boundDynUbosBaseToBeFlushed[set][binding] = out_offset;
            frame->boundDynUbosRangeToBeFlushed[set][binding] = dataSiz;
        }
        else
        {
            AFX_ASSERT(frame->boundDynUbosBaseToBeFlushed[set][binding] == 0);
            AFX_ASSERT(frame->boundDynUbosRangeToBeFlushed[set][binding] == 0);
        }
    }
    else
    {
        AFX_ASSERT(out_buffer);

        if (frame->boundDynUbosBaseToBeFlushed[set][binding] > out_offset)
            frame->boundDynUbosBaseToBeFlushed[set][binding] = out_offset;

        if ((out_offset + dataSiz) >
            (frame->boundDynUbosBaseToBeFlushed[set][binding] + frame->boundDynUbosRangeToBeFlushed[set][binding]))
            frame->boundDynUbosRangeToBeFlushed[set][binding] = dataSiz;
    }

    //if (frame->boundDynUbos[set][binding])
    {
        // TODO: Defer up to the first next draw call using boundDynUbosBaseToBeFlushed and boundDynUbosRangeToBeFlushed?

        avxBufferedMap map = AVX_BUFFERED_MAP(  out_buffer, 
                                                out_offset,
                                                dataSiz,
                                                NIL);
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, set, binding, 1, &map);
    }

    if (src)
    {
        AFX_ASSERT(p);
        if (p)
        {
            AfxStream2(unitCnt, src, srcStride, p, unitSiz);
        }
    }

    return p;
}

_ARX afxError ArxPostConstants(arxRenderContext rctx, afxUnit offset, afxUnit siz, void const* value)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    AvxCmdPushConstants(frame->drawDctx, offset, siz, value);

    return err;
}

_ARX void* ArxPostVertices(arxRenderContext rctx, afxUnit vtxCnt, afxUnit vtxSiz, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(vtxCnt);
    AFX_ASSERT(vtxSiz);
    AFX_ASSERT(!src || (src && srcStride));

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit dataSiz = vtxCnt * vtxSiz;

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = AvxRequestBufferedPump(&rctx->dynVtxAlloc, dataSiz, &out_buffer, &out_offset);
    
    if (frame->boundDynVbo != out_buffer)
    {
        if (frame->boundDynVbo)
        {
            AvxCohereMappedBuffer(frame->boundDynVbo, frame->boundDynVboBaseToBeFlushed, frame->boundDynVboRangeToBeFlushed, NIL, FALSE);
            frame->boundDynVbo = NIL;
            frame->boundDynVboBaseToBeFlushed = 0;
            frame->boundDynVboRangeToBeFlushed = 0;
        }

        frame->boundDynVbo = out_buffer;

        if (out_buffer)
        {
            frame->boundDynVboBaseToBeFlushed = out_offset;
            frame->boundDynVboRangeToBeFlushed = dataSiz;
        }
    }
    else
    {
        if (frame->boundDynVboBaseToBeFlushed > out_offset)
            frame->boundDynVboBaseToBeFlushed = out_offset;

        if ((out_offset + dataSiz) > 
            (frame->boundDynVboBaseToBeFlushed + frame->boundDynVboRangeToBeFlushed))
            frame->boundDynVboRangeToBeFlushed = dataSiz;
    }

    //if (out_buffer)
    {
        avxBufferedStream stream = AVX_BUFFERED_STREAM( out_buffer, 
                                                        out_offset, 
                                                        dataSiz, 
                                                        vtxSiz);
        AvxCmdBindVertexBuffers(frame->drawDctx, 0, 1, &stream);
    }

    if (src)
    {
        AFX_ASSERT(p);
        if (p)
        {
            AfxStream2(vtxCnt, src, srcStride, p, vtxSiz);
        }
    }

    return p;
}

_ARX void* ArxPostVertexIndices(arxRenderContext rctx, afxUnit idxCnt, afxUnit idxSiz, void const* src, afxUnit srcStride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(idxCnt);
    AFX_ASSERT(idxSiz);
    AFX_ASSERT(!src || (src && srcStride));

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit dataSiz = idxCnt * idxSiz;

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = AvxRequestBufferedPump(&rctx->dynIdxAlloc, dataSiz, &out_buffer, &out_offset);
    
    if (frame->boundDynIbo != out_buffer)
    {
        if (frame->boundDynIbo)
        {
            AvxCohereMappedBuffer(frame->boundDynIbo, frame->boundDynIboBaseToBeFlushed, frame->boundDynIboRangeToBeFlushed, NIL, FALSE);
            frame->boundDynIbo = NIL;
            frame->boundDynIboBaseToBeFlushed = 0;
            frame->boundDynIboRangeToBeFlushed = 0;
        }

        frame->boundDynIbo = out_buffer;

        if (out_buffer)
        {
            frame->boundDynIboBaseToBeFlushed = out_offset;
            frame->boundDynIboRangeToBeFlushed = dataSiz;
        }
    }
    else
    {
        if (out_offset < frame->boundDynIboBaseToBeFlushed)
            frame->boundDynIboBaseToBeFlushed = out_offset;

        if ((out_offset + dataSiz) > (frame->boundDynIboBaseToBeFlushed + frame->boundDynIboRangeToBeFlushed))
            frame->boundDynIboRangeToBeFlushed = dataSiz;
    }

    if (out_buffer)
    {
        frame->boundDynIboBaseToBeFlushed = out_offset;
        frame->boundDynIboRangeToBeFlushed = dataSiz;
        avxBufferedStream stream = AVX_BUFFERED_STREAM( out_buffer, 
                                                        out_offset, 
                                                        dataSiz, 
                                                        idxSiz);
        AvxCmdBindIndexBuffer(frame->drawDctx, stream.buf, stream.offset, stream.range, stream.stride);
    }

    if (src)
    {
        AFX_ASSERT(p);
        if (p)
        {
            AfxStream2(idxCnt, src, srcStride, p, idxSiz);
        }
    }

    return p;
}

_ARX void ArxGetRenderingClipSpaceInfo(arxRenderContext rctx, avxClipSpaceDepth* depth, afxReal* depthEpsilon, afxBool* nonRhcs)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (depth)
        *depth = rctx->clipSpaceDepth;

    if (depthEpsilon)
        *depthEpsilon = rctx->clipDepthEpsilon;

    if (nonRhcs)
        *nonRhcs = rctx->nonRhcs;
}

_ARX void ArxComputeLookToMatrices(arxRenderContext rctx, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(dir);
    AFX_ASSERT(!AfxV3dIsZero(dir));
    AFX_ASSERT(!AfxV3dIsInfinite(dir));
    AFX_ASSERT(v);
    AfxComputeLookToMatrix(v, eye, dir, AFX_V4D_Y, rctx->nonRhcs);

    if (iv)
        AfxM4dInvert(iv, v);
}

_ARX void ArxComputeLookAtMatrices(arxRenderContext rctx, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(target);
    AFX_ASSERT(eye);
    AFX_ASSERT(v);
    AfxComputeLookAtMatrix(v, eye, target, AFX_V4D_Y, rctx->nonRhcs);

    if (iv)
        AfxM4dInvert(iv, v);
}

_ARX void ArxComputeOrthographicMatrices(arxRenderContext rctx, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOrthographicMatrix(p, extent, near, far, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeOffcenterOrthographicMatrices(arxRenderContext rctx, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOffcenterOrthographicMatrix(p, left, right, bottom, top, near, far, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBoundingOrthographicMatrices(arxRenderContext rctx, afxBox const aabb, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    //AFX_ASSERT(aabb); // afxBox deve estar no view space.
    AFX_ASSERT(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBasicOrthographicMatrices(arxRenderContext rctx, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(aspectRatio); // w/h
    AFX_ASSERT(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputePerspectiveMatrices(arxRenderContext rctx, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputePerspectiveMatrix(p, extent, near, far, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeFovMatrices(arxRenderContext rctx, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeFovPerspectiveMatrix(p, fovY, aspectRatio, near, far, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeFrustrumMatrices(arxRenderContext rctx, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeOffcenterPerspectiveMatrix(p, left, right, bottom, top, near, far, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBasicPerspectiveMatrices(arxRenderContext rctx, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, rctx->nonRhcs, rctx->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}


_ARX afxError ArxStageMaterials(arxRenderContext rctx, arxMtd mtd, afxUnit cnt, afxUnit indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    _arxVisualMtd* instMtd;
    afxUnit mtdId = AfxGetObjectId(mtd);
    if (!AfxGetPoolUnit(&rctx->visualMaterials, mtdId, &instMtd))
    {
        AfxRequestPoolUnitsAt(&rctx->visualMaterials, AfxHere(), 1, &mtdId, &instMtd);

        arxScenario scio = rctx->scio;

        afxClass* mtlCls = (afxClass*)&mtd->mtlCls;

        struct
        {
            afxV3d albedo;
            afxReal roughness;
            afxReal metallic;
            afxInt albedoTexIdx;
            afxInt padding8[2];
        } mtlDefShd;
        AFX_ASSERT(sizeof(mtlDefShd) % 16 == 0);

        avxBuffer buf = NIL;
        avxBufferInfo bufi = { 0 };
        bufi.size = mtd->mtlCls.instCnt * sizeof(mtlDefShd);
        bufi.usage = avxBufferUsage_UNIFORM;
        bufi.flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/;
        if (AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf))
            AfxThrowError();

        instMtd->mtd = mtd;
        instMtd->mtlUbo = buf;
        instMtd->mtlUboBase = 0;
        instMtd->mtlUboRange = bufi.size;

#if 0
        for (afxUnit i = 0; i < cnt; i++)
        {
            arxMaterial mtl;
            if (!ArxEnumerateMaterials(mtd, indices[i], 1, &mtl))
                continue;

            AfxV3dCopy(mtlDefShd.albedo, mtl->albedo);
            mtlDefShd.metallic = mtl->metallic;
            mtlDefShd.roughness = mtl->roughness;
            mtlDefShd.albedoTexIdx = -1;
            AvxCmdUpdateBuffer(frame->transferDctx, instMtd->mtlUbo, instMtd->mtlUboBase, instMtd->mtlUboRange, &mtlDefShd);
        }
#else
        afxUnit baseOffset = instMtd->mtlUboBase;
        for (afxUnit i = 0; i < mtd->mtlCls.instCnt; i++)
        {
            arxMaterial mtl;
            if (!ArxEnumerateMaterials(mtd, i, 1, &mtl))
                continue;

            AfxV3dCopy(mtlDefShd.albedo, mtl->albedo);
            mtlDefShd.metallic = mtl->metallic;
            mtlDefShd.roughness = mtl->roughness;
            mtlDefShd.albedoTexIdx = -1;
            AvxCmdUpdateBuffer(frame->transferDctx, instMtd->mtlUbo, baseOffset, sizeof(mtlDefShd), &mtlDefShd);
            baseOffset += sizeof(mtlDefShd);
        }
#endif
    }

    avxBufferedMap bufm = { 0 };
    bufm.buf = instMtd->mtlUbo;
    bufm.offset = instMtd->mtlUboBase;
    bufm.range = instMtd->mtlUboRange;
    AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 2, 20, 1, &bufm);

    return err;
}

_ARX afxBool ArxStdNodeCullCallback(void* udd, arxNode nod)
{
    return FALSE; // do not cull
}

_ARX afxBool ArxStdBodyCullCallback(void* udd, arxPuppet bod)
{
    return FALSE; // do not cull
}

_ARX afxBool ArxStdModelCullCallback(void* udd, arxModel mdl)
{
    return FALSE;
}

_ARX afxError ArxExecuteRenderContext(arxRenderContext rctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    afxCmdId lastId = 0; // DBG

    return err;
}

_ARX afxError ArxUseRenderTechnique(arxRenderContext rctx, arxTechnique dtec, afxUnit passId, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    AvxCmdUseDrawTechniqueSIGMA(frame->drawDctx, dtec, passId, vin, dynamics);

    return err;
}

_ARX afxError ArxUpdateViewConstants(arxRenderContext rctx, arxViewConstants const* viewConsts)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(viewConsts);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    arxViewConstants* viewConsts2 = &frame->viewConsts;
    *viewConsts2 = *viewConsts;

    AvxCmdUpdateBuffer(frame->transferDctx, frame->viewUbo, 0, sizeof(*viewConsts2), viewConsts2);

    AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { {.buf = frame->viewUbo } });

    return err;
}

_ARX afxError ArxSetOpticalMatrices(arxRenderContext rctx, afxM4d iv, afxM4d v, afxM4d ip, afxM4d ipv, afxM4d pv, afxM4d p)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(p);
    AFX_ASSERT(v);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];
    arxViewConstants* viewConsts = &frame->viewConsts;

    AfxM4dCopy(viewConsts->p, p);

    if (ip)
        AfxM4dCopy(viewConsts->ip, ip);
    else
        AfxM4dInvert(viewConsts->ip, p);

    AfxM4dCopyAtm(viewConsts->v, v);

    if (iv)
        AfxM4dCopy(viewConsts->iv, iv);
    else
        AfxM4dInvertAtm(viewConsts->iv, v);

    if (pv)
        AfxM4dCopy(viewConsts->pv, pv);
    else
        AfxM4dMultiply(viewConsts->pv, p, v);

    if (ipv)
        AfxM4dCopy(viewConsts->ipv, ipv);
    else
        AfxM4dInvertAtm(viewConsts->ipv, viewConsts->pv);

    AfxV4dCopyAtv3d(viewConsts->viewPos, viewConsts->iv[3]);

    return err;
}

_ARX afxError ArxUseCamera(arxRenderContext rctx, arxCamera cam, afxRect const* drawArea)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(drawArea);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    arxViewConstants* viewConsts = &frame->viewConsts;

    if ((frame->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { drawArea->w, drawArea->h };
        ArxAdjustCameraAspectRatio(cam, AfxFindPhysicalAspectRatio(drawArea->w, drawArea->h), extent, extent);

        viewConsts->viewExtent[0] = drawArea->w;
        viewConsts->viewExtent[1] = drawArea->h;

        afxV4d viewPos;
        ArxGetCameraTranslation(cam, viewPos);
        AfxV4dCopyAtv3d(viewConsts->viewPos, viewPos);

        afxM4d v, iv, p, ip, pv, ipv;
        ArxGetProjectionMatrices(cam, ip, ipv, pv, p);
        ArxGetCameraMatrices(cam, iv, v);

        AfxM4dCopy(viewConsts->p, p);
        AfxM4dCopy(viewConsts->ip, ip);
        AfxM4dCopyAtm(viewConsts->v, v);
        AfxM4dCopy(viewConsts->iv, iv);
        AfxM4dCopy(viewConsts->pv, pv);
        AfxM4dCopy(viewConsts->ipv, ipv);
    }

    AvxCmdUpdateBuffer(frame->transferDctx, frame->viewUbo, 0, sizeof(*viewConsts), viewConsts);

    AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { { .buf = frame->viewUbo } });

    return err;
}

_ARX afxError ArxWaitFrame(arxRenderContext rctx, void* param, arxNextFrameInfo* nextInfo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    // xrWaitFrame must be callable from any thread, including a different thread than xrBeginFrame/xrEndFrame are being called from.

    afxUnit frameIdx = rctx->frameIdx;
    arxRenderFrame* frame = &rctx->frames[frameIdx];

    // 1. Wait until previous GPU work for this frame finished.
    err = AvxWaitForFence(frame->frameReady, frame->frameReadyNextValue, AFX_NANOSECS_PER_MILLISECS(rctx->frameCnt));

    nextInfo->predictedDisplayPeriod = 0;
    nextInfo->predictedDisplayTime = 0;
    nextInfo->shouldRender = TRUE;
    nextInfo->frameReady = frame->frameReady;
    nextInfo->frameReadyNextValue = frame->frameReadyNextValue;
    nextInfo->frameAvail = frame->frameAvail;
    nextInfo->frameAvailNextValue = frame->frameAvailNextValue;
    nextInfo->frameIdx = frameIdx;
    nextInfo->frameId = rctx->frameId;

    return err;
}

_ARX afxError ArxCancelFrame(arxRenderContext rctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (!rctx->inFrameGoingOn)
    {
        // Just an error for validation.
        AFX_ASSERT(rctx->inFrameGoingOn);
        return err;
    }

    afxUnit frameIdx = rctx->frameIdx;
    arxRenderFrame* frame = &rctx->frames[frameIdx];

    if (!frame->locked)
    {
        // Just an error for validation.
        AFX_ASSERT(frame->locked);
        return err;
    }

    frame->locked = FALSE;
    rctx->inFrameGoingOn = FALSE;

    return err;
}

_ARX afxError ArxBeginFrame(arxRenderContext rctx, void* param)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (rctx->inFrameGoingOn)
    {
        AFX_ASSERT(!rctx->inFrameGoingOn);
        return err;
    }

    afxUnit frameIdx = rctx->frameIdx;
    arxRenderFrame* frame = &rctx->frames[frameIdx];

    if (frame->locked)
    {
        AFX_ASSERT(!frame->locked);
        return err;
    }

    frame->locked = TRUE;
    rctx->inFrameGoingOn = TRUE;
    
    AvxAdvanceBufferedPump(&rctx->dynIdxAlloc);
    AvxAdvanceBufferedPump(&rctx->dynVtxAlloc);
    AvxAdvanceBufferedPump(&rctx->dynUnfmAlloc);

    if (!frame->viewUbo)
    {
        avxBuffer buf;
        avxBufferInfo bufi = { 0 };
        bufi.size = sizeof(frame->viewConsts);
        bufi.usage = avxBufferUsage_UNIFORM;
        bufi.flags = avxBufferFlag_WX/* | avxBufferFlag_C*/;
        AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);
        frame->viewUbo = buf;
    }
    return err;
}

_ARX afxError ArxEndFrame(arxRenderContext rctx, void* param, afxUnit* perSecFrame)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (!rctx->inFrameGoingOn)
    {
        // Just an error for validation.
        AFX_ASSERT(rctx->inFrameGoingOn);
        return err;
    }

    afxUnit frameIdx = rctx->frameIdx;
    arxRenderFrame* frame = &rctx->frames[frameIdx];

    if (perSecFrame) *perSecFrame = rctx->perSecFrame;

    //AvxResetFences(rctx->dsys, 1, &frame->frameAvail);
    //AvxResetFences(rctx->dsys, 1, &frame->frameReady);

    frame->locked = FALSE;
    rctx->inFrameGoingOn = FALSE;

    return err;
}

_ARX afxError ArxAdvanceFrame(arxRenderContext rctx, afxUnit* nextFrameIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit64 frnv = 1 + AvxGetCompletedFenceValue(frame->frameReady);
    frame->frameReadyNextValue = frnv;
    //frame->frameReadyNextValue += 1;
    afxUnit64 fanv = 1 + AvxGetCompletedFenceValue(frame->frameAvail);
    frame->frameAvailNextValue = fanv;
    //frame->frameAvailNextValue += 1;

    afxUnit frameCnt = rctx->frameCnt;
    afxUnit frameIdx = (rctx->frameIdx + 1) % frameCnt;
    rctx->frameIdx = frameIdx;
    rctx->frameId += 1;

    if (nextFrameIdx) *nextFrameIdx = frameIdx;
}

_ARX afxError ArxAdvanceSceneLayer(arxRenderContext rctx, arxSceneMode mode)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    if (mode == arxSceneMode_WIRE_FACES)
    {
        if (!rctx->wireBaryPip)
        {
            avxPipeline wireBaryPip = NIL;
            if (ArxLoadWireframePipeline(rctx, arxSceneMode_WIRE_FACES, NIL, &wireBaryPip))
                AfxThrowError();

            rctx->wireBaryPip = wireBaryPip;
        }

        if (!rctx->wireBaryPip)
        {
            AfxThrowError();
        }

        AvxCmdBindPipeline(frame->drawDctx, rctx->wireBaryPip, NIL, NIL);

        //AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { {.buf = 0 } });
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { {.buf = frame->viewUbo } });

    }
    else if (mode == arxSceneMode_WIRE_LINES)
    {
        if (!rctx->wirePip)
        {
            avxPipeline wirePip = NIL;
            if (ArxLoadWireframePipeline(rctx, arxSceneMode_WIRE_LINES, NIL, &wirePip))
                AfxThrowError();

            rctx->wirePip = wirePip;
        }

        if (!rctx->wirePip)
        {
            AfxThrowError();
        }

        AvxCmdBindPipeline(frame->drawDctx, rctx->wirePip, NIL, NIL);

        //AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { {.buf = 0 } });
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 0, 0, 1, (avxBufferedMap[]) { {.buf = frame->viewUbo } });

    }

    return err;
}

_ARX afxError ArxBeginScene(arxRenderContext rctx, afxDrawContext transferDctx, afxDrawContext drawDctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    if (!transferDctx)
    {
        afxDrawContext dctx;
        avxContextConfig ctxi = { 0 };
        ctxi.caps = avxAptitude_DMA;
        AvxAcquireDrawContexts(rctx->dsys, NIL, &ctxi, 1, &dctx);
        
        if (frame->transferDctx)
            AfxDisposeObjects(1, &frame->transferDctx);

        frame->transferDctx = dctx;
        frame->internalTransferDctx = TRUE;

        AvxPrepareDrawCommands(dctx, FALSE, avxCmdFlag_ONCE);
    }
    else
    {
        frame->internalTransferDctx = FALSE;
        frame->transferDctx = transferDctx;
        AfxReacquireObjects(1, &frame->transferDctx);
    }

    if (!drawDctx)
    {
        afxDrawContext dctx;
        avxContextConfig ctxi = { 0 };
        ctxi.caps = avxAptitude_GFX;
        AvxAcquireDrawContexts(rctx->dsys, NIL, &ctxi, 1, &dctx);

        if (frame->drawDctx)
            AfxDisposeObjects(1, &frame->drawDctx);

        frame->drawDctx = dctx;
        frame->internalDrawDctx = TRUE;

        AvxPrepareDrawCommands(dctx, FALSE, avxCmdFlag_ONCE);
    }
    else
    {
        frame->internalDrawDctx = FALSE;
        frame->drawDctx = drawDctx;
        AfxReacquireObjects(1, &frame->drawDctx);
    }

    return err;
}

_ARX afxError ArxEndScene(arxRenderContext rctx, afxUnit id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    
    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    // flush dynamic buffers

    if (frame->boundDynVbo)
    {
        if (frame->boundDynVboRangeToBeFlushed)
        {
            AvxCohereMappedBuffer(frame->boundDynVbo, frame->boundDynVboBaseToBeFlushed, frame->boundDynVboRangeToBeFlushed, NIL, FALSE);
            frame->boundDynVboRangeToBeFlushed = 0;
        }
        frame->boundDynVbo = NIL;
        frame->boundDynVboBaseToBeFlushed = 0;
        frame->boundDynVboRangeToBeFlushed = 0;
    }

    if (frame->boundDynIbo)
    {
        if (frame->boundDynIboRangeToBeFlushed)
        {
            AvxCohereMappedBuffer(frame->boundDynIbo, frame->boundDynIboBaseToBeFlushed, frame->boundDynIboRangeToBeFlushed, NIL, FALSE);
            frame->boundDynIboRangeToBeFlushed = 0;
        }
        frame->boundDynIbo = NIL;
        frame->boundDynIboBaseToBeFlushed = 0;
        frame->boundDynIboRangeToBeFlushed = 0;
    }

    for (afxUnit i = 0; i < 4; i++)
    {
        for (afxUnit j = 0; j < 32; j++)
        {
            if (frame->boundDynUbos[i][j])
            {
                if (frame->boundDynUbosRangeToBeFlushed[i][j])
                {
                    AvxCohereMappedBuffer(frame->boundDynUbos[i][j], frame->boundDynUbosBaseToBeFlushed[i][j], frame->boundDynUbosRangeToBeFlushed[i][j], NIL, FALSE);
                    frame->boundDynUbosRangeToBeFlushed[i][j] = 0;
                }
                frame->boundDynUbos[i][j] = NIL;
                frame->boundDynUbosBaseToBeFlushed[i][j] = 0;
                frame->boundDynUbosRangeToBeFlushed[i][j] = 0;
            }
        }
    }


    ArxExecuteRenderContext(rctx);


    if (frame->internalTransferDctx && frame->internalDrawDctx)
    {
        AvxCompileDrawCommands(frame->drawDctx);
        AvxCompileDrawCommands(frame->transferDctx);

        avxSubmission subms[] =
        {
            {
                .dctx = frame->transferDctx
            },
            {
                .dctx = frame->drawDctx
            }
        };
        if (AvxExecuteDrawCommands(rctx->dsys, 2, subms))
            AfxThrowError();
    }
    else if (frame->internalTransferDctx)
    {
        AvxCompileDrawCommands(frame->transferDctx);

        avxSubmission subm = { .dctx = frame->transferDctx };
        if (AvxExecuteDrawCommands(rctx->dsys, 1, &subm))
            AfxThrowError();
    }
    else if(frame->internalDrawDctx)
    {
        AvxCompileDrawCommands(frame->drawDctx);

        avxSubmission subm = { .dctx = frame->drawDctx };
        if (AvxExecuteDrawCommands(rctx->dsys, 1, &subm))
            AfxThrowError();
    }

    AfxDisposeObjects(1, &frame->drawDctx);
    AfxDisposeObjects(1, &frame->transferDctx);

    return err;
}

_ARX afxError _ArxRctxDtorCb(arxRenderContext rctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    AvxDismantleBufferedPump(&rctx->dynVtxAlloc);
    AvxDismantleBufferedPump(&rctx->dynIdxAlloc);
    AvxDismantleBufferedPump(&rctx->dynUnfmAlloc);

    if (AfxDeallocate((void**)&rctx->frames, AfxHere()))
        AfxThrowError();

    return err;
}

_ARX afxError _ArxRctxCtorCb(arxRenderContext rctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxScenario scio = args[0];
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    AFX_ASSERT(args[1]);
    arxRenderConfiguration const* rcfg = AFX_CAST(arxRenderConfiguration const*, args[1]) + invokeNo;
    
    arxSimulation sim = rcfg->sim;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxDrawSystem dsys = rcfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    rctx->udd = rcfg->udd;
    rctx->tag = rcfg->tag;

    rctx->sim = sim;
    rctx->dsys = dsys;

    rctx->nonRhcs = FALSE;
    rctx->clipDepthEpsilon = AFX_EPSILON;
    rctx->clipSpaceDepth = AvxGetSystemClipSpaceDepth(dsys, NIL);

    rctx->scio = scio;

    rctx->frameCnt = AFX_MAX(1, AFX_MIN(rcfg->frameCnt, 3));
    //rctx->frameIdx = rctx->frameCnt - 1;
    //rctx->frameId = AFX_U64_MAX;
    // Now, we use AdvanceFrame to step it.
    rctx->frameIdx = 0;
    rctx->frameId = 0;

    if (AfxAllocate(rctx->frameCnt * sizeof(rctx->frames[0]), NIL, AfxHere(), (void**)&rctx->frames))
    {
        AfxThrowError();
        return err;
    }

    AfxZero(rctx->frames, sizeof(rctx->frames[0]) * rctx->frameCnt);
    
    for (afxUnit i = 0; i < rctx->frameCnt; i++)
    {
        arxRenderFrame* frame = &rctx->frames[i];

        avxFence fences[2] = { 0 };
        avxFenceInfo fencesInfo[2] = { 0 };
        fencesInfo[0].initialVal = 0;
        fencesInfo[1].initialVal = 0; // start avail already signaled
        if (AvxAcquireFences(dsys, 2, fencesInfo, fences))
        {
            AfxThrowError();
        }
        frame->frameReady = fences[0];
        frame->frameAvail = fences[1];
        //AvxSignalFence(fences[0], 1);
        //AvxSignalFence(fences[1], 1);
        //frame->frameAvailNextValue = AFX_U64_MAX;
        //frame->frameReadyNextValue = AFX_U64_MAX;
    }

    AfxMakeArena(&rctx->cmdArena, NIL, AfxHere());
    AfxMakeChain(&rctx->commands, rctx);

    AfxSetUpPool(&rctx->visualPuppets, sizeof(_arxVisualPuppet), 1, NIL);
    AfxSetUpPool(&rctx->visualMeshes, sizeof(_arxVisualMesh), 1, NIL);
    AfxSetUpPool(&rctx->visualModels, sizeof(_arxVisualModel), 1, NIL);
    AfxSetUpPool(&rctx->visualMaterials, sizeof(_arxVisualMtd), 1, NIL);

    AvxDeployBufferedPump(&rctx->dynVtxAlloc, avxBufferUsage_VERTEX, rcfg->vboFlags, rcfg->minVtxPagSiz, rcfg->vtxBlockAlign, rctx->frameCnt, dsys);
    AvxDeployBufferedPump(&rctx->dynIdxAlloc, avxBufferUsage_INDEX, rcfg->iboFlags, rcfg->minIdxPagSiz, rcfg->idxBlockAlign, rctx->frameCnt, dsys);
    AvxDeployBufferedPump(&rctx->dynUnfmAlloc, avxBufferUsage_UNIFORM, rcfg->uboFlags, rcfg->minUniPagSiz, rcfg->uniBlockAlign, rctx->frameCnt, dsys);
    
    return err;
}

_ARX afxClassConfig const _ARX_RCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_RCTX,
    .name = "RenderContext",
    .desc = "Drawing Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(arxRenderContext)),
    .ctor = (void*)_ArxRctxCtorCb,
    .dtor = (void*)_ArxRctxDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxConfigureRenderContext(arxScenario scio, arxRenderConfiguration* rcfg)
{
    afxError err = { 0 };
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(rcfg);
    
    afxDrawSystem dsys = rcfg->dsys ? rcfg->dsys : (scio ? ArxGetScenarioDrawSystem(scio) : NIL); // temp workaround to get a context.
    rcfg->dsys = dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &rcfg->dsys);

    rcfg->frameCnt = AFX_MAX(1, AFX_MIN(rcfg->frameCnt, 3));

    return err;
}

_ARX afxError ArxAcquireRenderContext(arxScenario scio, arxRenderConfiguration const* rcfg, arxRenderContext* context)
{
    afxError err = { 0 };
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(context);
    AFX_ASSERT(rcfg);

    static afxBool rctxClsInited = FALSE;
    static afxClass rctxCls = { 0 };

    if (!rctxClsInited)
    {
        AfxMountClass(&rctxCls, NIL, NIL, &_ARX_RCTX_CLASS_CONFIG);
        rctxClsInited = TRUE;
    }

    afxClass* cls = (afxClass*)&rctxCls;//_ArxScioGetRctxClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_RCTX);

    if (AfxAcquireObjects(cls, 1, (afxObject*)context, (void const*[]) { scio, (void*)rcfg }))
        AfxThrowError();

    return err;
}
