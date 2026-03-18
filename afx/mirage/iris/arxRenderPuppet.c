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

#define _ARX_SCENE_C
#define _ARX_SCENARIO_C
#define _ARX_RENDER_CONTEXT_C
#define _ARX_NODE_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/coree/draw/avxIcd.h"

_ARX afxError ArxCmdRenderPuppets(arxRenderContext rctx, afxM4d m, afxUnit cnt, arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit pupIdx = 0; pupIdx < cnt; pupIdx++)
    {
        arxPuppet pup = puppets[pupIdx];
        if (!pup) continue;
        AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

        arxModel mdl;
        if (!ArxGetPuppetModel(pup, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxSkeleton skl = ArxGetSkeleton(mdl);
        AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

        _arxVisualPuppet* instPup;
        afxUnit pupId = AfxGetObjectId(pup);
        if (!AfxGetPoolUnit(&rctx->visualPuppets, pupId, (void**)&instPup))
        {
            AfxRequestPoolUnitsAt(&rctx->visualPuppets, AfxHere(), 1, &pupId, (void**)&instPup);

            afxUnit mtxBufSiz = sizeof(afxM4d) * mdli.skli.boneCnt;

            // workaround to get triple buffered matrices
            for (afxUnit i = 0; i < rctx->frameCnt; i++)
            {
                avxBuffer buf;
                avxBufferInfo bufi = { 0 };
                bufi.usage = avxBufferUsage_UNIFORM;
                bufi.flags = avxBufferFlag_WX;
                bufi.size = mtxBufSiz;
                AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);

                instPup->pup = pup;
                instPup->mdl = mdl;
                instPup->boneCnt = mdli.skli.boneCnt;
                instPup->perFrame[i].mtxUbo = buf;
                instPup->perFrame[i].mtxUboBase = 0;
                instPup->perFrame[i].mtxUboRange = mtxBufSiz;
            }
        }

        arxPose pose;
        arxPosture plce;
        ArxGetPuppetPose(instPup->pup, &pose, &plce);

#if 0
        AfxSamplePuppetAnimationsAccelerated(bod, instBod->jntCnt, /*bod->placement*/NIL, pose, plce, 0.0);
#else
        if (!ArxCountPuppetMotives(pup))
        {
            ArxProducePosture(plce, NIL, skl, 0, instPup->boneCnt, 0, instPup->boneCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
        else
        {
            afxReal allowedErr = 0.f;
            afxUnit lodJntCnt = ArxCountJoints(mdl, allowedErr);

            ArxCommencePoseAccumulation(pose, 0, instPup->boneCnt, NIL);
            ArxAccumulatePuppetAnimations(pose, pup, 0, instPup->boneCnt, allowedErr, NIL);
            ArxConcludePoseAccumulation(pose, 0, instPup->boneCnt, mdl, allowedErr, NIL);

            ArxProducePosture(plce, pose, skl, 0, instPup->boneCnt, 0, lodJntCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
#endif

        afxM4d matrices[256];
        AFX_ASSERT(ARRAY_SIZE(matrices) >= instPup->boneCnt);
        ArxProduceCompositeMatrices(plce, skl, 0, instPup->boneCnt, FALSE, matrices);

        //AfxCopy(matrices, _ArxPlceGetMmArray(plce, 0), sizeof(matrices[0]) * instPup->boneCnt);
#if 0
        if (instPup->boneCnt == 10)
        {
            static afxM4d matrices2[256];

            if (AfxMemcmp(matrices, matrices2, sizeof(matrices[0]) * 10))
            {
                int a = 1;
            }

            AfxCopy(matrices2, matrices, sizeof(matrices[0]) * 10);
        }
#endif
#if 0
        if (instBod->boneCnt == 10)
        {
            AfxReportComment("Matrices:");
            for (afxUnit i = 0; i < instBod->boneCnt; i++)
            {
                AfxReportComment("  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n  %f %f %f %f\n",
                    matrices[i][0][0],
                    matrices[i][0][1],
                    matrices[i][0][2],
                    matrices[i][0][3],
                    matrices[i][1][0],
                    matrices[i][1][1],
                    matrices[i][1][2],
                    matrices[i][1][3],
                    matrices[i][2][0],
                    matrices[i][2][1],
                    matrices[i][2][2],
                    matrices[i][2][3],
                    matrices[i][3][0],
                    matrices[i][3][1],
                    matrices[i][3][2],
                    matrices[i][3][3]
                );
            }
        }
#endif

#if !0

        //AvxCmdUpdateBuffer(frame->transferDctx, instPup->perFrame[rctx->frameIdx].mtxUbo, instPup->perFrame[rctx->frameIdx].mtxUboBase, instPup->perFrame[rctx->frameIdx].mtxUboRange, matrices);
        AvxCmdUpdateBuffer(frame->transferDctx, instPup->perFrame[rctx->frameIdx].mtxUbo, instPup->perFrame[rctx->frameIdx].mtxUboBase, instPup->perFrame[rctx->frameIdx].mtxUboRange, matrices);
        //AvxCohereMappedBuffer(instBod->perFrame[rctx->frameIdx].mtxUbo, instBod->perFrame[rctx->frameIdx].mtxUboBase, instBod->perFrame[rctx->frameIdx].mtxUboRange, NIL, FALSE);
        
        frame->mtxStackBaseOffset = instPup->perFrame[rctx->frameIdx].mtxUboBase;
        //AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 3, 30, 1, (avxBufferedMap[]) { { 0 } });
        AvxCmdBindBuffers(frame->drawDctx, avxBus_GFX, 3, 30, 1, (avxBufferedMap[]) { { .buf = instPup->perFrame[rctx->frameIdx].mtxUbo, .offset = instPup->perFrame[rctx->frameIdx].mtxUboBase } });
#else
        void* matrixStack = ArxPostUniform(rctx, 3, 30, instPup->boneCnt, sizeof(matrices[0]), matrices, sizeof(matrices[0]));
        AfxCopy(matrixStack, matrices, sizeof(matrices[0]) * instPup->boneCnt);
#endif
        ArxCmdRenderModels(rctx, 1, &mdl);
    }
    return err;
}

AFX_DEFINE_STRUCT(arxPuppetData)
{
    avxBuffer mtxUbo;
    afxUnit32 mtxUboBase;
    afxUnit32 biasMapFboBaseIdx; // biasMapOffset;
    afxUnit32 mtxStackBaseIdx; // mtxBufOffset;
    afxUnit32 mtlStackBaseIdx;
    afxUnit32 mtlMapFboBaseIdx;
};
