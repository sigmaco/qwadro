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

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#define _ARX_MESH_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_TERRAIN_C
#define _ARX_RENDER_CONTEXT_C
#define _ARX_SCENARIO_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"

/*
    2025/12/18, 20h02. Enquanto eu tentava consertar esse código, num dos meus piores dias, eu tive uma visão de um jardim.
    Eu via esse jardim atravás de um portal arqueado, havia um arco acima dele, com um letreiro que não consegui ler.
    Esta passagem arqueada estava acompanhada de um muro branco que se eXtendia dos confis do oriente aos confins do ocidente.
    O jardim era bastante belo, mas não consigo descrever porque. Parece haver um mundo inteiro lá, e eu estava limitado pela
    perspectiva da passagem arqueada, com a sensação de estar vendo um mundo maior que o meu por uma greta. Era realmente um lugar 
    desejado, me convidando para entrar. Porém, eu não consegui atrassavar o portal. Eu quis ver se eu conseguia ler o letreiro pelo 
    lado de dentro, ou como o muro contrastante com aquele lugar pelo lado de dentro, no entanto, eu não consegui. E não sei porque 
    eu não consegui. As imagens me fizera lembrar do país dos sonhos verdes. Eu não entendi, mas me lembro de já ter visões anteriores 
    com portais arqueados na infância e adolescência.
*/

AFX_DEFINE_STRUCT(avxIndexBuffer)
{
    afxChain    rows;
    avxBuffer   buf;
    afxChain    rooms;
};

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    avxBuffer               vbo;
    afxUnit32                offset;
    afxUnit32                size;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    avxBuffer               vbo;
};

AFX_OBJECT(arxBufferizer)
{
    arxScenario        scio;
    afxUnit32           DefaultVBSize;

    freeVBlistEntry*    freelist;
    createdVBlistEntry* vblist;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;
};

#if !0
afxBool ArxBufferizeIndices(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, arxIndexCache* cache)
{
    // based on RwD3D9CreateVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(cache);
    
    freeVBlistEntry* freelist = vbMgr->freelist;

    while (freelist) // Search for an available free space
    {
        if (freelist->size >= size)
            break;

        freelist = freelist->next;
    }

    if (!freelist)
    {
        // If there is not free space, create a new VB
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        AFX_ASSERT(freelist);
        freelist->offset = 0;
        freelist->size = (((vbMgr->DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;
        
        afxDrawSystem dsys = ArxGetScenarioDrawSystem(vbMgr->scio);

        avxBuffer vbo;
        avxBufferInfo spec = { 0 };
        spec.size = freelist->size;
        spec.flags = avxBufferFlag_W;
        spec.usage = avxBufferUsage_INDEX;

        if (AvxAcquireBuffers(dsys, 1, &spec, &vbo))
        {
            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);            
            return FALSE;
        }
        
        freelist->vbo = vbo;
        freelist->next = vbMgr->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (vbMgr->freelist)
            vbMgr->freelist->prev = freelist;

        vbMgr->freelist = freelist;

        // Add an entry to the vb list
        createdVBlistEntry* vblist = AfxPushSlabUnit(&vbMgr->CreatedVBFreeList);
        vblist->vbo = vbo;
        vblist->next = vbMgr->vblist;
        vbMgr->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    cache->buf = freelist->vbo;
    cache->base = freelist->offset;
    afxUnit32 nextOffset = (cache->base) + size;

    AFX_ASSERT(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;
    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev)
            freelist->prev->next = freelist->next;

        if (freelist->next)
            freelist->next->prev = freelist->prev;

        if (vbMgr->freelist == freelist)
            vbMgr->freelist = freelist->next;

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void DestroyIndexBuffer(arxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, avxBuffer vbo, afxUnit32 offset)
{
    // based on RwD3D9DestroyVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo != NULL);

    // Search for an available free space to add
    freeVBlistEntry* freelist = vbMgr->freelist;

    while (freelist)
    {
        freeVBlistEntry* nextFreelist = freelist->next;

        if (freelist->vbo == vbo)
        {
            // Could be added to the end of the free space?

            if ((freelist->offset + freelist->size) == offset) break;
            else
            {
                // Could be added to the begin of the free space?

                if (offset + size == freelist->offset)
                    break;
            }
        }
        freelist = nextFreelist;
    }

    if (freelist)
    {
        afxBool canCompactMore = FALSE;
        afxUnit32 newoffset = offset;
        afxUnit32 newSize = size;
        freeVBlistEntry* oldFreelist = freelist;

        do
        {
            // Could be added to the end of the free space?

            if ((freelist->offset + freelist->size) == newoffset) freelist->size += newSize;
            else
            {
                // Could be added to the begin of the free space?

                if (newoffset + newSize == freelist->offset)
                {
                    freelist->offset = newoffset;
                    freelist->size += newSize;
                }
            }

            // Could be compact more?
            newoffset = freelist->offset;
            newSize = freelist->size;
            oldFreelist = freelist;
            canCompactMore = FALSE;
            freelist = vbMgr->freelist;

            while (freelist)
            {
                freeVBlistEntry* nextFreelist = freelist->next;

                if (freelist->vbo == vbo)
                {
                    // Could be added to the end of the free space?

                    if ((freelist->offset + freelist->size) == newoffset)
                    {
                        canCompactMore = TRUE;
                        break;
                    }
                    else
                    {
                        // Could be added to the begin of the free space?

                        if (newoffset + newSize == freelist->offset)
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                    }
                }
                freelist = nextFreelist;
            }

            // Remove old list

            if (canCompactMore)
            {
                if (oldFreelist->prev)
                    oldFreelist->prev->next = oldFreelist->next;

                if (oldFreelist->next)
                    oldFreelist->next->prev = oldFreelist->prev;

                if (vbMgr->freelist == oldFreelist)
                    vbMgr->freelist = oldFreelist->next;

                AfxPopSlabUnit(&vbMgr->FreeVBFreeList, oldFreelist);
            }
        } while (canCompactMore);
    }
    else
    {
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        freelist->offset = offset;
        freelist->size = size;
        freelist->vbo = vbo;

        // Add it to the begining
        freelist->next = vbMgr->freelist;
        freelist->prev = NULL;

        if (vbMgr->freelist)
            vbMgr->freelist->prev = freelist;

        vbMgr->freelist = freelist;
    }

#if defined(RWDEBUG)
    BlocksCreated--;
    BytesReserved -= size;
#endif
}

afxBool _IndexBufferManagerOpen(arxBufferizer vbMgr)
{
    afxError err = { 0 };
    //avxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxDeploySlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxDeploySlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    return TRUE;
}

void _IndexBufferManagerClose(arxBufferizer vbMgr)
{
    //avxVertexBufferizer* vbMgr = *vbMgrPtr;
#if defined(RWDEBUG)
    AFX_ASSERT(BlocksCreated == 0);
    AFX_ASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    /*  Destroy the free list */
    while (vbMgr->freelist != NULL)
    {
        freeVBlistEntry   *nextFreelist = vbMgr->freelist->next;

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, vbMgr->freelist);

        vbMgr->freelist = nextFreelist;
    }

    /*  Destroy the VB list */
    while (vbMgr->vblist != NULL)
    {
        createdVBlistEntry   *nextvblist = vbMgr->vblist->next;

        if (vbMgr->vblist->vbo != NULL)
        {
            AfxDisposeObjects(1, &(vbMgr->vblist->vbo));
        }

        AfxPopSlabUnit(&vbMgr->CreatedVBFreeList, vbMgr->vblist);

        vbMgr->vblist = nextvblist;
    }

    AfxDismantleSlabAllocator(&vbMgr->CreatedVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->FreeVBFreeList);

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */
}

#endif

_ARX afxError ArxBufferizeMesh(arxMesh msh, afxUnit morphIdx, arxVertexCache* vtxCache, arxIndexCache* idxCache)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    
    if (msh->vtxCache.buf)
    {
        if (vtxCache)
            *vtxCache = msh->vtxCache;
    }
    else
    {
        afxString const positionals[] = { AFX_STRING("pos"), AFX_STRING("pvt"), AFX_STRING("wgt") };

        afxUnit cacheStride[2] = { 0 };
        afxUnit cacheIdx[ARX_MAX_VERTEX_ATTRIBUTES] = { 0 };
        afxUnit cachedAttrOffset[ARX_MAX_VERTEX_ATTRIBUTES] = { 0 };
        avxFormat cachedAttrFmt[ARX_MAX_VERTEX_ATTRIBUTES] = { 0 };

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            arxMeshAttr* attr = &msh->attrInfo[i];
            avxFormat fmt = attr->fmt;
            AFX_ASSERT(fmt < avxFormat_TOTAL);

            afxUnit sIdx = AFX_INVALID_INDEX;
            AfxCompareStrings(&msh->attrIds[i], 0, FALSE, ARRAY_SIZE(positionals), positionals, &sIdx);

#if !0
            if (attr->flags & arxVertexFlag_POSITIONAL)
            {

            }
#endif
            switch (sIdx)
            {
            case 0: // pos
            {
                cachedAttrFmt[i] = avxFormat_RGB32f;
                cacheIdx[i] = 0;
                break;
            }
            case 1: // pvt
            {
                switch (fmt)
                {
                case avxFormat_R16u:
                case avxFormat_R32u:
                case avxFormat_R8i:
                case avxFormat_R16i:
                case avxFormat_R32i:
                    cachedAttrFmt[i] = avxFormat_R8u;
                    break;
                default:
                    cachedAttrFmt[i] = fmt;
                    break;
                }
                cacheIdx[i] = 0;
                break;
            }
            case 2: // wgt
            {
                switch (fmt)
                {
                case avxFormat_R32f:
                    cachedAttrFmt[i] = avxFormat_R8un;
                    break;
                case avxFormat_RG32f:
                    cachedAttrFmt[i] = avxFormat_RG8un;
                    break;
                case avxFormat_RGB32f:
                    cachedAttrFmt[i] = avxFormat_RGB8un;
                    break;
                case avxFormat_RGBA32f:
                    cachedAttrFmt[i] = avxFormat_RGBA8un;
                    break;
                default:
                    cachedAttrFmt[i] = fmt;
                    break;
                }
                cacheIdx[i] = 0;
                break;
            }
            default:
            {
                cachedAttrFmt[i] = attr->fmt;
                cacheIdx[i] = 1;
                break;
            }
            }
            cachedAttrOffset[i] = cacheStride[cacheIdx[i]];
            avxFormatDescription pfd;
            AvxDescribeFormats(1, &fmt, &pfd);
            cacheStride[cacheIdx[i]] += pfd.stride;//AfxVertexFormatGetSize(cachedAttrFmt[i]);
        }

        arxVertexCache* cache = &msh->vtxCache;
        AfxPushLink(&cache->vbuf, NIL);
        cache->vin = NIL;

        avxBufferInfo vboSpecs[] = 
        {
            {
                //.flags = avxBufferFlag_W,
                .usage = avxBufferUsage_VERTEX,
                .size = 0
            },
            // transient
            {
                .flags = avxBufferFlag_W,
                .usage = avxBufferUsage_UPLOAD,
                .size = 0
            }
        };

        for (afxUnit i = 0; i < 2; i++)
        {
            cache->streams[i].base = i ? msh->vtxCache.streams[i - 1].range : 0;
            cache->streams[i].range = AFX_ALIGN_SIZE(cacheStride[i] * msh->vtxCnt, AVX_BUFFER_ALIGNMENT);
            cache->streams[i].stride = cacheStride[i];

            vboSpecs[0].size += cache->streams[i].range;
        }

        // set the size for the transient buffer.
        vboSpecs[1].size = vboSpecs[0].size;

        avxBuffer buffers[2];
        arxScenario scio = AfxGetHost(msh);
        afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);
        AvxAcquireBuffers(/*scio*/dsys, 2, vboSpecs, buffers);
        cache->buf = buffers[0];
        avxBuffer buf = buffers[1];

        for (afxUnit srcIdx = 0; srcIdx < 2; srcIdx++)
        {
            if (!cacheStride[srcIdx])
            {
                AFX_ASSERT(srcIdx != 0);
                continue;
            }

            AFX_ASSERT(cacheStride[srcIdx]);
            AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), cache->streams[srcIdx].base, cache->streams[srcIdx].range);
            void* dst;

            if (AvxMapBuffer(buf, cache->streams[srcIdx].base, cache->streams[srcIdx].range, NIL, &dst)) AfxThrowError();
            else
            {
                for (afxUnit j = 0; j < msh->attrCnt; j++)
                {
                    if (cacheIdx[j] == srcIdx)
                    {
                        arxMeshAttr* attr = &msh->attrInfo[j];
                        //AFX_ASSERT(data);
                        avxFormat fmt = attr->fmt;
                        AFX_ASSERT(fmt < avxFormat_TOTAL);

                        afxByte* data = ArxAccessVertexData(msh, j, morphIdx, 0);

                        if (data)
                        {
                            avxFormatDescription pfd;
                            AvxDescribeFormats(1, &fmt, &pfd);
                            afxSize srcStride = pfd.stride;// AfxVertexFormatGetSize(fmt);
                            AFX_ASSERT(srcStride);
                            AfxStream3(msh->vtxCnt, data, 0, srcStride, dst, cachedAttrOffset[j], cache->streams[srcIdx].stride);
                        }
                    }
                }
                AvxUnmapBuffer(buf, TRUE);
            }
        }

        avxBufferedCopy copy = { 0 };
        copy.dst = cache->buf;
        copy.src = buf;
        copy.range = vboSpecs[1].size;
        if (AvxCopyBuffers(dsys, NIL, 1, &copy, NIL))
            AfxThrowError();

        // Dispose the transient buffer.
        AfxDisposeObjects(1, &buf);

        AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, NIL);

        if (vtxCache)
            *vtxCache = *cache;
    }

    //arxIndexCache* cache = &msh->cache;

    if (!msh->ibo)
    {
        arxScenario scio = AfxGetHost(msh);
        afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);

        afxUnit idxSiz = msh->minIdxSiz;
        idxSiz = AFX_MAX(sizeof(afxUnit16), idxSiz); // atleast 16-bit.

        avxBufferInfo bufis[2] =
        {
            {
                .size = msh->idxCnt * idxSiz,
                //.flags = avxBufferFlag_W,
                .usage = avxBufferUsage_INDEX
            },
            // transient
            {
                .size = msh->idxCnt * idxSiz,
                .flags = avxBufferFlag_W,
                .usage = avxBufferUsage_UPLOAD
            }
        };

        avxBuffer buffers[2];

        if (AvxAcquireBuffers(dsys, 2, bufis, buffers)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 2, buffers);

            msh->ibo = buffers[0];
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &msh->ibo);
            msh->iboBase = 0;
            msh->iboRange = bufis[0].size;
            msh->iboStride = msh->iboRange / msh->idxCnt;
            AFX_ASSERT(msh->iboStride == idxSiz);
            //cache->idxSiz = idxSiz;

            AFX_ASSERT(msh->iboStride);
            AFX_ASSERT_RANGE(AvxGetBufferCapacity(msh->ibo, 0), msh->iboBase, msh->iboRange);
            //AvxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxUnit* indices = msh->indices;

            avxBufferIo iop = { 0 };
            iop.dstOffset = msh->iboBase;
            iop.dstStride = msh->iboStride;
            iop.srcStride = sizeof(indices[0]);
            iop.rowCnt = msh->idxCnt;
            AvxUpdateBuffer(buffers[1], 1, &iop, indices, 0, NIL);

            avxBufferedCopy copy = { 0 };
            copy.dst = msh->ibo;
            copy.src = buffers[1];
            copy.range = msh->iboRange;
            if (AvxCopyBuffers(dsys, NIL, 1, &copy, NIL))
                AfxThrowError();

            // Dispose the transient buffer.
            AfxDisposeObjects(1, &buffers[1]);

            AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, NIL);

#if 0//_AFX_DEBUG
            void* p;
            AfxWaitForDrawBridge(dsys, 0, 0);
            AvxMapBuffer(cache->buf, cache->base, cache->range, NIL, &p);

            for (afxUnit i = 0; i < mshi.idxCnt; i++)
            {
                if (mshi.minIdxSiz == 1)
                {
                    afxIndex8* idx8 = p;
                    AFX_ASSERT(indices[i] == idx8[i]);
                }
                else if (mshi.minIdxSiz == 2)
                {
                    afxIndex16* idx16 = p;
                    AFX_ASSERT(indices[i] == idx16[i]);
                }
                else if (mshi.minIdxSiz == 4)
                {
                    afxIndex* idx32 = p;
                    AFX_ASSERT(indices[i] == idx32[i]);
                }

                if (err)
                {
                    int a = 1;
                }
            }

            AvxUnmapBuffer(cache->buf, FALSE);
#endif
        }
    }

    if (idxCache)
    {
        idxCache->buf = msh->ibo;
        idxCache->base = msh->iboBase;
        idxCache->range = msh->iboRange;
        idxCache->stride = msh->iboStride;
    }
    return err;
}

_ARX afxError ArxPrepareRenderableMeshes(arxRenderContext rctx, afxUnit cnt, arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        arxMesh msh = meshes[mshIdx];
        if (!msh) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        _arxVisualMesh* instMsh;
        afxUnit mshId = AfxGetObjectId(msh);
        if (!AfxGetPoolUnit(&rctx->visualMeshes, mshId, (void**)&instMsh))
        {
            AfxRequestPoolUnitsAt(&rctx->visualMeshes, AfxHere(), 1, &mshId, (void**)&instMsh);

            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            instMsh->msh = msh;

            ArxBufferizeMesh(msh, 0, &instMsh->vtxCache, &instMsh->idxCache);
        }
    }
    return err;
}

_ARX afxError ArxCmdRenderMeshes(arxRenderContext rctx, afxUnit cnt, arxMeshDataArg const data[], arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    ArxPrepareRenderableMeshes(rctx, cnt, meshes);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mshIdx = 0; mshIdx < cnt; mshIdx++)
    {
        arxMesh msh = meshes[mshIdx];
        if (!msh) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        _arxVisualMesh* instMsh;
        afxUnit mshId = AfxGetObjectId(msh);
        if (!AfxGetPoolUnit(&rctx->visualMeshes, mshId, (void**)&instMsh))
        {
            continue;
        }

        avxBufferedStream vtxSrcs[] =
        {
            {
                .buf = instMsh->vtxCache.buf,
                .offset = instMsh->vtxCache.streams[0].base,
                .range = instMsh->vtxCache.streams[0].range,
                .stride = instMsh->vtxCache.streams[0].stride
            },
            {
                .buf = instMsh->vtxCache.buf,
                .offset = instMsh->vtxCache.streams[1].base,
                .range = instMsh->vtxCache.streams[1].range,
                .stride = instMsh->vtxCache.streams[1].stride
            }
        };
        AvxCmdBindVertexBuffers(frame->drawDctx, 0, /*ARRAY_SIZE(vtxSrcs)*/2, vtxSrcs);

        AvxCmdBindIndexBuffer(frame->drawDctx, 
            instMsh->idxCache.buf,
            instMsh->idxCache.base,
            instMsh->idxCache.range,
            instMsh->idxCache.stride);

        arxMeshMorph mshm;
        ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

        afxUnit mtlDataOffset = 0;

        for (afxUnit mtlIdx = 0; mtlIdx < mshi.mtlCnt; mtlIdx++)
        {
            //rig.txd;
            // TODO: write materials
            mtlDataOffset = 0;
        }

        //AvxCmdBindBuffers(rctx->drawDctx, 2, 0, 1, (avxBufferedMap[]) { {.buf = visualMsh->mtlUbo } });

        afxUnit surfCnt = mshi.secCnt;

        for (afxUnit j = 0; j < surfCnt; j++)
        {
            arxMeshSection sec;
            ArxDescribeMeshSections(msh, j, 1, &sec);

            if (sec.mtlIdx != AFX_INVALID_INDEX)
            {
                // Push textures

                _arxVisualMtd* visualMtl;
                //AfxGetPoolUnit(&rctx->visualMaterials, mdl->rigs[mshIdx]->mtlToTxdMap[sec.mtlIdx], &visualMtl);

                //AvxCmdBindBuffers(frame->drawDctx, 2, 0, 1, &AVX_BUFFERED_MAP(visualMtl->dataUbo, visualMtl->dataUboBase, visualMtl->dataUboRange, 0));
            }

            afxUnit idxCnt = (sec.triCnt * 3);
            afxUnit firstIdx = (sec.baseTriIdx * 3);

            DrawIndexedIndirectCmd cmd;
            cmd.cmd.baseIdx = firstIdx;
            cmd.cmd.baseInst = 0;// scn->framesets[frameIdx].indDrawCnt;
            cmd.cmd.idxCnt = idxCnt;
            cmd.cmd.instCnt = 1;
            cmd.cmd.vtxOffset = mshm.baseVtx;
            cmd.jntMapBase = data[mshIdx].biasMapFboBaseIdx; // biasMapOffset;
            cmd.jntMtxStackBase = data[mshIdx].mtxStackBaseIdx; // mtxBufOffset;
            cmd.mtlStackBase = data[mshIdx].mtlStackBaseIdx;
            cmd.mtlMapBase = data[mshIdx].mtlMapFboBaseIdx;
            //cmd.mtlIdx = sec.mtlIdx;

            AvxCmdPushConstants(frame->drawDctx, 0, sizeof(afxUnit32) * 4, &cmd.jntMtxStackBase);

            AvxCmdDrawIndexed(frame->drawDctx, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
        }
    }
    return err;
}

_ARX afxError ArxPrepareRenderableModels(arxRenderContext rctx, afxUnit cnt, arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxModel mdl = models[mdlIdx];
        if (!mdl) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

        _arxVisualModel* instMdl;
        afxUnit mdlId = AfxGetObjectId(mdl);
        if (!AfxGetPoolUnit(&rctx->visualModels, mdlId, (void**)&instMdl))
        {
            AfxRequestPoolUnitsAt(&rctx->visualModels, AfxHere(), 1, &mdlId, (void**)&instMdl);

            afxUnit biasMapPop = 0;
            afxUnit biasMapBufSiz = 0;
            afxUnit mtlMapPop = 0;
            afxUnit mtlMapBufSiz = 0;

            for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
            {
                arxMesh msh = NIL;
                ArxGetRiggedMeshes(mdl, rigIdx, 1, &msh);

                arxMeshRigInfo rig;
                ArxDescribeMeshRigs(mdl, rigIdx, 1, &rig);

                if (!msh) continue;
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                arxMeshInfo mshi;
                ArxDescribeMesh(msh, &mshi);

                biasMapBufSiz += mshi.biasCnt * sizeof(rig.biasToJntMap[0]);
                biasMapPop += mshi.biasCnt;

                mtlMapBufSiz += mshi.mtlCnt * sizeof(rig.mtlMap[0]);
                mtlMapPop += mshi.mtlCnt;
            }

            avxBuffer mtlMapFbo = NIL;
            avxBufferInfo mtlMapFboInfo = { 0 };
            mtlMapFboInfo.size = mtlMapBufSiz;
            mtlMapFboInfo.usage = avxBufferUsage_FETCH;
            mtlMapFboInfo.flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/;
            mtlMapFboInfo.fmt = avxFormat_R32u;
            if (AvxAcquireBuffers(rctx->dsys, 1, &mtlMapFboInfo, &mtlMapFbo))
                AfxThrowError();

            avxBuffer biasMapFbo = NIL;
            avxBufferInfo biasMapFboInfo = { 0 };
            biasMapFboInfo.size = biasMapBufSiz;
            biasMapFboInfo.usage = avxBufferUsage_FETCH;
            biasMapFboInfo.flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/;
            biasMapFboInfo.fmt = avxFormat_R32u;
            if (AvxAcquireBuffers(rctx->dsys, 1, &biasMapFboInfo, &biasMapFbo))
                AfxThrowError();

            instMdl->mdl = mdl;
            instMdl->biasMapPop = biasMapPop;
            instMdl->biasMapFbo = biasMapFbo;
            instMdl->biasMapFboBase = 0;
            instMdl->biasMapFboRange = biasMapFboInfo.size;

            instMdl->mtlMapPop = mtlMapPop;
            instMdl->mtlMapFbo = mtlMapFbo;
            instMdl->mtlMapFboBase = 0;
            instMdl->mtlMapFboRange = mtlMapFboInfo.size;

            afxUnit biasMapBufBase = 0;
            afxUnit mtlMapBufBase = 0;

            for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
            {
                arxMesh msh = NIL;
                ArxGetRiggedMeshes(mdl, rigIdx, 1, &msh);

                arxMeshRigInfo rig;
                ArxDescribeMeshRigs(mdl, rigIdx, 1, &rig);

                if (!msh) continue;
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                arxMeshInfo mshi;
                ArxDescribeMesh(msh, &mshi);

                afxUnit biasMapSiz = mshi.biasCnt * sizeof(rig.biasToJntMap[0]);
                AvxCmdUpdateBuffer(frame->transferDctx, instMdl->biasMapFbo, biasMapBufBase, biasMapSiz, rig.biasToJntMap);
                biasMapBufBase += biasMapSiz;

                afxUnit mtlMapSiz = mshi.mtlCnt * sizeof(rig.mtlMap[0]);
                AvxCmdUpdateBuffer(frame->transferDctx, instMdl->mtlMapFbo, mtlMapBufBase, mtlMapSiz, rig.mtlMap);
                mtlMapBufBase += mtlMapSiz;
            }
        }
    }

    return err;
}

_ARX afxError ArxCmdRemapBoneLods(arxRenderContext rctx, afxReal const le[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    rctx->next.sklLe = le;
    rctx->next.sklLeStride = stride;
    return err;
}

_ARX afxError ArxCmdRemapBoneParents(arxRenderContext rctx, afxUnit const pi[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    rctx->next.sklPi = pi;
    rctx->next.sklPiStride = stride;
    return err;
}

_ARX afxError ArxCmdRemapBoneTransforms(arxRenderContext rctx, afxTransform const lt[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    rctx->next.sklLt = lt;
    rctx->next.sklLtStride = stride;
    return err;
}

_ARX afxError ArxCmdRemapBoneInversors(arxRenderContext rctx, afxM4d const iwm[], afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    rctx->next.sklIwm = iwm;
    rctx->next.sklIwmStride = stride;
    return err;
}

_ARX afxError ArxCmdBindSkeleton(arxRenderContext rctx, arxSkeleton skl, afxUnit maxBoneCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    rctx->next.skl = skl;
    rctx->next.maxBoneCnt = maxBoneCnt;

    ArxCmdRemapBoneInversors(rctx, _ArxSklGetIwArray(skl, 0), sizeof(afxM4d));
    ArxCmdRemapBoneTransforms(rctx, _ArxSklGetLtArray(skl, 0), sizeof(afxTransform));
    ArxCmdRemapBoneParents(rctx, _ArxSklGetPiArray(skl, 0), sizeof(afxUnit));
    ArxCmdRemapBoneLods(rctx, _ArxSklGetLeArray(skl, 0), sizeof(afxReal));

    return err;
}

_ARX afxError ArxCmdBindModel(arxRenderContext rctx, arxModel mdl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    rctx->next.mdl = mdl;
    ArxCmdBindSkeleton(rctx, ArxGetSkeleton(mdl), 0);

    return err;
}

_ARX afxError ArxCmdDrawModel(arxRenderContext rctx, arxModel mdl, afxBool skl)
{

}

_ARX afxError ArxCmdRenderModels(arxRenderContext rctx, afxUnit cnt, arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    ArxPrepareRenderableModels(rctx, cnt, models);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxModel mdl = models[mdlIdx];
        if (!mdl) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

        _arxVisualModel* instMdl;
        afxUnit mdlId = AfxGetObjectId(mdl);
        if (!AfxGetPoolUnit(&rctx->visualModels, mdlId, (void**)&instMdl))
        {
            continue;
        }

        frame->mtlStackBaseIdx = 0;
        frame->mtlStackBaseOffset = 0;
        frame->mtlMapFboBaseOffset = instMdl->mtlMapFboBase;
        frame->mtlMapFboBaseIdx = 0;
        //AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 2, 20, 1, (avxBufferedMap[]) { {.buf = instMdl->mtlUbo } });
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 2, 21, 1, (avxBufferedMap[]) { { .buf = instMdl->mtlMapFbo, .offset = instMdl->mtlMapFboBase } });

        frame->biasMapFboBaseOffset = instMdl->biasMapFboBase;
        frame->biasMapFboBaseIdx = 0;
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 3, 31, 1, (avxBufferedMap[]) { { .buf = instMdl->biasMapFbo, .offset = instMdl->biasMapFboBase } });

        afxUnit biasMapBufBaseOffset = 0;
        afxUnit biasMapBufBaseIdx = 0;
        afxUnit mtlMapBufBaseOffset = 0;
        afxUnit mtlMapBufBaseIdx = 0;

        for (afxUnit rigIdx = 0; rigIdx < mdli.rigCnt; rigIdx++)
        {
            arxMeshRigInfo rigi;
            ArxDescribeMeshRigs(mdl, rigIdx, 1, &rigi);
            arxMesh msh = rigi.msh;
            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            arxMeshInfo mshi;
            ArxDescribeMesh(msh, &mshi);

            ArxStageMaterials(rctx, mdli.mtd, mshi.mtlCnt, rigi.mtlMap);

            afxUnit biasMapSiz = mshi.biasCnt * sizeof(rigi.biasToJntMap[0]);
            frame->biasMapFboBaseOffset = instMdl->biasMapFboBase + biasMapBufBaseOffset;
            frame->biasMapFboBaseIdx = biasMapBufBaseIdx;
            biasMapBufBaseIdx += mshi.biasCnt;
            biasMapBufBaseOffset += biasMapSiz;

            afxUnit mtlMapSiz = mshi.mtlCnt * sizeof(rigi.mtlMap[0]);
            frame->mtlMapFboBaseOffset = instMdl->mtlMapFboBase + mtlMapBufBaseOffset;
            frame->mtlMapFboBaseIdx = mtlMapBufBaseIdx;
            mtlMapBufBaseIdx += mshi.mtlCnt;
            mtlMapBufBaseOffset += mtlMapSiz;


            arxMeshDataArg mdas[64];
            mdas[rigIdx] = (arxMeshDataArg) { 0 };
            mdas[rigIdx].biasMapFboBaseIdx = frame->biasMapFboBaseIdx;
            mdas[rigIdx].mtlMapFboBaseIdx = frame->mtlMapFboBaseIdx;
            mdas[rigIdx].mtlStackBaseIdx = frame->mtlStackBaseIdx;
            mdas[rigIdx].mtxStackBaseIdx = frame->mtxStackBaseIdx;

            ArxCmdRenderMeshes(rctx, 1, &mdas[rigIdx], &msh);
        }
    }
    return err;
}
