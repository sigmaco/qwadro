/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.
// This file is part of Integrated Raster Imaging System.

#ifndef ARX_RENDER_CONTEXT_DDK_H
#define ARX_RENDER_CONTEXT_DDK_H

#include "../anim/arxMotiveDDK.h"
#include "../scene/arxScenarioDDK.h"
#include "../sim/asxImplementation.h"

#ifdef _ARX_RENDER_CONTEXT_C

#pragma pack(push, 1)
AFX_DEFINE_STRUCT(DrawIndexedIndirectCmd)
{
    avxDrawIndexedIndirect cmd;
    afxUnit32 jntMtxStackBase;
    afxUnit32 jntMapBase;
    afxUnit32 mtlStackBase;
    afxUnit32 mtlMapBase;
};

AFX_DEFINE_STRUCT(StepCmd)
{
    afxUnit nodId;
    afxUnit bodId;
    afxReal dist;
};

AFX_DEFINE_STRUCT(RenderCmd)
{
    afxUnit nodId;
    afxUnit bodId;
    afxUnit prtId;
    afxUnit mdlId;
    afxUnit mshId;
    afxUnit mshsId;
    afxUnit primId;
    afxReal dist;

    avxDrawIndexedIndirect cmd;
    afxUnit mtlDataOffset;
    afxUnit mtxStackOffset;
    afxUnit jntMapOffset;
};
#pragma pack(pop)


AFX_DEFINE_STRUCT(_arxVisualMtd)
{
    afxLink     rctx;
    afxLink     technique;
    arxMtd      mtd;
    avxBuffer   mtlUbo;
    afxUnit     mtlUboBase;
    afxUnit     mtlUboRange;
};

AFX_DEFINE_STRUCT(_arxVisualMesh)
{
    afxLink rctx;
    arxMesh msh;
    arxVertexCache vtxCache;
    arxIndexCache idxCache;
};

AFX_DEFINE_STRUCT(_arxVisualModel)
{
    afxLink     rctx;
    arxModel    mdl;
    afxUnit     rigCnt;
    // mapping between the rigged meshes' bias and skeleton joints.
    afxUnit     biasMapPop; // total
    avxBuffer   biasMapFbo;
    afxUnit     biasMapFboBase;
    afxUnit     biasMapFboRange;
    // mapping between rigged meshes' materials and the MTD.
    afxUnit     mtlMapPop; // total
    avxBuffer   mtlMapFbo;
    afxUnit     mtlMapFboBase;
    afxUnit     mtlMapFboRange;

    avxBufferedStream sklVbo;
    avxBufferedStream sklIbo;
};

AFX_DEFINE_STRUCT(_arxVisualPuppet)
{
    afxLink     rctx;
    arxPuppet     pup;
    arxModel    mdl;
    afxUnit     boneCnt;
    struct
    {
        avxBuffer   mtxUbo;
        afxUnit     mtxUboBase;
        afxUnit     mtxUboRange;
    } perFrame[3];
};

AFX_DEFINE_STRUCT(arxRenderFrame)
{
    afxBool locked;
    avxFence frameAvail;
    afxUnit64 frameAvailNextValue;
    avxFence frameReady;
    afxUnit64 frameReadyNextValue;

    arxCamera activeCam;
    arxViewConstants viewConsts;

    avxBuffer viewUbo;

    arxModel activeSkl;
    afxSize biasMapFboBaseOffset;
    afxSize biasMapFboBaseIdx;
    afxSize mtxStackBaseOffset;
    afxSize mtxStackBaseIdx;
    afxSize mtlStackBaseOffset;
    afxSize mtlStackBaseIdx;
    afxSize mtlMapFboBaseOffset;
    afxSize mtlMapFboBaseIdx;

    afxBool internalTransferDctx;
    afxDrawContext transferDctx;
    afxBool internalDrawDctx;
    afxDrawContext drawDctx;

    avxBuffer boundDynVbo;
    afxUnit boundDynVboBaseToBeFlushed;
    afxUnit boundDynVboRangeToBeFlushed;
    avxBuffer boundDynIbo;
    afxUnit boundDynIboBaseToBeFlushed;
    afxUnit boundDynIboRangeToBeFlushed;
    avxBuffer boundDynUbos[4][32];
    afxUnit boundDynUbosBaseToBeFlushed[4][32];
    afxUnit boundDynUbosRangeToBeFlushed[4][32];
};

AFX_DECLARE_STRUCT(_arxDdiRctx);
AFX_DECLARE_STRUCT(_arxIddRctx);

AFX_OBJECT(arxRenderContext)
{
    _arxDdiRctx const*  ddi;
    _arxIddRctx*        idd;
    void*               udd;
    afxString           tag;
    arxSimulation       sim;
    afxDrawSystem       dsys;
    afxBool             nonRhcs;
    avxClipSpaceDepth   clipSpaceDepth;
    afxReal             clipDepthEpsilon;
    arxScenario         scio;

    afxChain            commands;
    afxArena            cmdArena; // owned by dsys data for specific port

    afxPool visualMaterials;
    afxPool visualMeshes;
    afxPool visualModels;
    afxPool visualPuppets;

    afxBool inFrameGoingOn;
    afxUnit64 frameId;
    afxUnit frameIdx;
    afxUnit frameCnt;
    arxRenderFrame frames[3];
    afxUnit perSecFrame;

    avxBufferedPump dynVtxAlloc;
    avxBufferedPump dynIdxAlloc;
    avxBufferedPump dynUnfmAlloc;

    avxPipeline wirePip;
    avxPipeline wireBaryPip;
    afxReal wireThickness;
    avxColor wireCol;
    avxColor fillCol;

    avxShader codb;

    struct
    {
        arxModel            mdl;

        arxSkeleton         skl;
        afxUnit             maxBoneCnt;
        afxM4d const*       sklIwm;
        afxUnit             sklIwmStride;
        afxTransform const* sklLt;
        afxUnit             sklLtStride;
        afxUnit const*      sklPi;
        afxUnit             sklPiStride;
        afxUnit const*      sklLe;
        afxUnit             sklLeStride;
    } next;
};
#endif//_ARX_RENDER_CONTEXT_C

ARX afxClassConfig const _ARX_RCTX_CLASS_CONFIG;

#endif//ARX_RENDER_CONTEXT_DDK_H
