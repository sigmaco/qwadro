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
#define _ARX_TERRAIN_C
#define _ARX_SCENARIO_C
#define _ARX_RENDER_CONTEXT_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/coree/draw/avxIcd.h"



void cullAABBs2(const afxBox *boxes, size_t numBoxes, const afxFrustum *frustum, afxBool *culledResults);

afxBool RenderCells(arxRenderContext rctx, afxDrawContext dctx, avxVertexInput vin, arxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines)
{
    afxBool result;

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit cellsCulledCnt = 0;
    afxUnit cellsToBeDrawnCnt = 0;
    afxUnit cellsToBeDrawnIdx[64] = { 0 };

    for (afxUnit i = 0; i < ter->secCnt; i++)
    {
        // Get the current cell dimensions.
        afxBox aabb = ter->sectors[i].aabb;

        // Check to see if the positions are in this cell.
        //if (AfxDoesFrustumCullAabbs(frustum, 1, &aabb))
        afxBool culled = TRUE;
        cullAABBs2(&aabb, 1, frustum, &culled);

        //culled = FALSE;
        if (i >= 63) break;

        if (culled)
        {
            // Increment the number of cells that were culled.
            ++cellsCulledCnt;
        }
        else
        {
            cellsToBeDrawnIdx[cellsToBeDrawnCnt] = i;
            ++cellsToBeDrawnCnt;
        }
    }

    if (showDbgLines)
    {
#if 0
        if (!ter->dbgLinesVbo)
        {
            // We store edges as 24 16-bit indices
            // 2730 * 24 = 65520 indices
            // It will crash if there are more than 2730 sectors.

            avxBuffer bufs[2];
            avxBufferInfo bufis[2] = { 0 };
            bufis[0].size = ter->secCnt * 8 * sizeof(afxV3d);
            bufis[0].usage = avxBufferUsage_VERTEX;
            bufis[0].flags = avxBufferFlag_WX;
            bufis[1].size = ter->secCnt * 24 * sizeof(afxUnit16);
            bufis[1].usage = avxBufferUsage_INDEX;
            bufis[1].flags = avxBufferFlag_WX;
            AvxAcquireBuffers(ter->dsys, 2, bufis, bufs);

            afxV3d* vertices = NIL;
            afxUnit16* indices = NIL;
            avxBufferedMap maps[2] = { 0 };
            maps[0].buf = bufs[0];
            maps[0].range = bufis[0].size;
            maps[1].buf = bufs[1];
            maps[1].range = bufis[1].size;
            AvxMapBuffers(ter->dsys, 2, maps, (void**[]) { &vertices, &indices });

            ArxPostVertices(rctx, ter->secCnt * 8, sizeof(afxV3d), &vertices);
            ArxPostVertexIndices(rctx, ter->secCnt * 24, sizeof(afxUnit16), &indices);
            AsxGenerateIndexedLinesForAabbs(ter->secCnt, ter->secAabb, vertices, sizeof(vertices[0]), indices, sizeof(indices[0]));
#if 0
            afxUnit baseIdx = 0;
            for (afxUnit i = 0; i < ter->secCnt; i++)
            {
                _acxTerrSec* sec = &ter->sectors[i];
                //baseIdx = sec->gridNode;
                AsxGenerateIndexedLinesForAabbs(1, &sec->aabb, &vertices[i * AFX_NUM_BOX_CORNERS], baseIdx * AFX_NUM_BOX_EDGE_VERTICES, &indices[i * AFX_NUM_BOX_EDGE_VERTICES] /*i == 0 ? indices : 0*/, NIL, NIL, NIL);
                baseIdx += AFX_NUM_BOX_EDGE_VERTICES;
            }
#endif       
            AvxUnmapBuffers(ter->dsys, 2, maps);
            ter->dbgLinesVbo = bufs[0];
            ter->dbgLinesVboRange = bufis[0].size;
            ter->dbgLinesVboStride = sizeof(afxV3d);
            ter->dbgLinesIbo = bufs[1];
            ter->dbgLinesIboRange = bufis[1].size;
            ter->dbgLinesIboStride = sizeof(afxUnit16);
        }
#endif

        AvxCmdBindPipeline(dctx, ter->dbgAabbPip, vin, NIL);

        afxV3d* vertices = ArxPostVertices(rctx, ter->secCnt * 8, sizeof(afxV3d), NIL, 0);
        afxUnit16* indices = ArxPostVertexIndices(rctx, ter->secCnt * 24, sizeof(afxUnit16), NIL, 0);
        AfxGenerateIndexedLinesForAabbs(ter->secCnt, ter->secAabb, vertices, sizeof(vertices[0]), indices, sizeof(indices[0]));

        afxReal* color = ArxPostUniform(rctx, 1, 1, 1, sizeof(afxV4d), NIL, 0);
        AfxV4dSet(color, 0, 0.3, 0.3, 1);
        //AvxCmdPushConstants(dctx, 0, sizeof(pvm), pvm);
        afxM4d* pvm = ArxPostUniform(rctx, 0, 0, 3, sizeof(afxM4d), NIL, 0);
        AfxM4dCopy(pvm[0], frame->viewConsts.p);
        AfxM4dCopy(pvm[1], frame->viewConsts.v);
        AfxM4dCopy(pvm[2], AFX_M4D_IDENTITY);
        avxBufferedStream stream = { 0 };
        stream.buf = ter->dbgLinesVbo;
        stream.offset = 0;
        stream.range = ter->dbgLinesVboRange;
        stream.stride = ter->dbgLinesVboStride;
        //AvxCmdBindVertexBuffers(dctx, 0, 1, &stream);
        //AvxCmdBindIndexBuffer(dctx, ter->dbgLinesIbo, 0, ter->dbgLinesIboRange, ter->dbgLinesIboStride);

#if !0
        AvxCmdDrawIndexed(dctx, ter->secCnt * 24, 1, 0, 0, 0);
#else
        afxUnit vtxOffset = 0;
        afxUnit idxBase = 0;
        //for (afxUnit i = 0; i < cellsToBeDrawnCnt; i++)
        for (afxUnit i = 0; i < ter->secCnt; i++)
        {
            _acxTerrSec* sec = &ter->sectors[i];

            AvxCmdDrawIndexed(dctx, 24, 1, idxBase, vtxOffset, 0);
            vtxOffset += 8 * 12;
            idxBase += 24;
        }
#endif
    }

#if 0
    if (showFaces)
    {
        //for (afxUnit cellIdx = 0; cellIdx < cellsToBeDrawnCnt; cellIdx++)
        for (afxUnit cellIdx = 0; cellIdx < ter->secCnt; cellIdx++)
        {
            //_arxTerrSec* sec = &ter->sectors[cellsToBeDrawnIdx[cellIdx]];
            _arxTerrSec* sec = &ter->sectors[cellIdx];

            if (!sec->vbo)
            {
                struct
                {
                    afxV3d pos;
                    afxV3d nrm;
                    afxV3d tan;
                    afxV3d bit;
                    afxV2d uv;
                }*vertices = NIL;
                afxUnit16* indices = NIL;

                avxBuffer bufs[2];
                avxBufferInfo bufis[2] = { 0 };
                bufis[0].size = sec->vtxCnt * sizeof(vertices[0]);
                bufis[0].usage = avxBufferUsage_VERTEX;
                bufis[0].flags = avxBufferFlag_WX;
                bufis[1].size = sec->vtxIdxCnt * sizeof(indices[0]);
                bufis[1].usage = avxBufferUsage_INDEX;
                bufis[1].flags = avxBufferFlag_WX;
                AvxAcquireBuffers(ter->dsys, 2, bufis, bufs);
#if !0
                sec->vbo = bufs[0];
                sec->vboRange = bufis[0].size;
                sec->vboStride = sizeof(vertices[0]);
                sec->ibo = bufs[1];
                sec->iboRange = bufis[1].size;
                sec->iboStride = sizeof(indices[0]);

                avxBufferedMap maps[2] = { 0 };
                maps[0].buf = bufs[0];
                maps[0].range = bufis[0].size;
                maps[1].buf = bufs[1];
                maps[1].range = bufis[1].size;
                AvxMapBuffers(ter->dsys, 2, maps, (void**[]) { &vertices, &indices });

                AfxStream2(sec->vtxIdxCnt, sec->visuVtxMap, sizeof(sec->visuVtxMap[0]), indices, sizeof(indices[0]));
                AfxStream2(sec->vtxCnt, sec->visuVtxPos, sizeof(sec->visuVtxPos[0]), vertices, sizeof(vertices[0]));

                AvxUnmapBuffers(ter->dsys, 2, maps);
#endif
            }
        }

        AvxCmdBindPipeline(dctx, ter->meshPip, NIL, NIL);

        if (!ter->texd)
        {
            avxRasterInfo rasi = { 0 };
            rasi.usage = avxRasterUsage_TEXTURE;
            AvxLoadRasters(ter->dsys, 1, &rasi, AfxUri("../dirt01d.tga"), NIL, &ter->texd);
        }

        AvxCmdBindRasters(dctx, avxBus_GFX, 0, 1, 1, &ter->texd);
        AvxCmdBindSamplers(dctx, avxBus_GFX, 0, 1, 1, NIL);

        //for (afxUnit i = 0; i < cellsToBeDrawnCnt; i++)
        for (afxUnit i = 0; i < ter->secCnt; i++)
        {
            //_arxTerrSec* sec = &ter->sectors[cellsToBeDrawnIdx[i]];
            _arxTerrSec* sec = &ter->sectors[i];

            avxBufferedStream stream = { 0 };
            stream.buf = sec->vbo;
            stream.offset = sec->vboBase;
            stream.range = sec->vboRange;
            stream.stride = sec->vboStride;
            AvxCmdBindVertexBuffers(dctx, 0, 1, &stream);
            AvxCmdBindIndexBuffer(dctx, sec->ibo, sec->iboBase, sec->iboRange, sec->iboStride);

            AvxCmdDrawIndexed(dctx, sec->vtxIdxCnt, 1, 0, 0, 0);
        }
    }
#endif
    return TRUE;
}

_ARX afxError ArxCmdRenderTerrain(arxRenderContext rctx, arxTerrain ter)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    arxMesh msh;
    if (ArxGetTerrainMeshes(ter, 0, 1, &msh))
    {
        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        arxIndexCache idxCache;
        arxVertexCache vtxCache;
        ArxBufferizeMesh(msh, 0, &vtxCache, &idxCache); // request drawable cache.

        avxBufferedStream vtxSrcs[] =
        {
            {
                .buf = vtxCache.buf,
                .offset = vtxCache.streams[0].base,
                .range = vtxCache.streams[0].range,
                .stride = vtxCache.streams[0].stride
            },
            {
                .buf = vtxCache.buf,
                .offset = vtxCache.streams[1].base,
                .range = vtxCache.streams[1].range,
                .stride = vtxCache.streams[1].stride
            }
        };

        AvxCmdBindVertexBuffers(frame->drawDctx, 0, ARRAY_SIZE(vtxSrcs), vtxSrcs);

        AvxCmdBindIndexBuffer(frame->drawDctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

        AvxCmdDrawIndexed(frame->drawDctx, mshi.idxCnt, 1, 0, NIL, 0);
    }

    return err;
}
