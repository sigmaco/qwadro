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

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_TERRAIN_DDK_H
#define ARX_TERRAIN_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DEFINE_STRUCT(_arxDdiTer);
AFX_DEFINE_STRUCT(_arxIddTer);

#ifdef _ARX_TERRAIN_C
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, _arxTerrSec)
{
    afxUnit     gridNode, gridNodeX, gridNodeZ;

    afxUnit     vtxIdxCnt;
    afxUnit     vtxCnt;
    afxUnit     quadCnt;
    afxUnit     vtxStartX;
    afxUnit     vtxStartZ;
    afxUnit     vtxPerRow;
    afxUnit     vtxPerCol;

    afxUnit     visuReqCnt;
    afxUnit     visuVtxCnt;
    afxV3d*     visuVtxPos;
    afxUnit     visuVtxIdxCnt;
    afxUnit16*  visuVtxMap;

    afxSphere   bsph;
    afxBox      aabb;
    afxV4d      centre;
    afxUnit     collTriCnt;
    afxV3d*     collTriEdgeNrm; // [collTriCnt * 3]
    afxUnit     collVtxCnt;
    afxV3d*     collVtx;
    afxV3d*     collVtxTan;
    afxV3d*     collVtxBit;
    afxV3d*     collVtxNrm;
    afxV3d*     collVtxRgb;

    arxMesh     msh;

    avxBuffer   vbo;
    afxUnit     vboBase;
    afxUnit     vboRange;
    afxUnit     vboStride;

    avxBuffer   ibo;
    afxUnit     iboBase;
    afxUnit     iboRange;
    afxUnit     iboStride;

};

AFX_OBJECT(arxTerrain)
{
    _arxIddTer*         idd;
    _arxDdiTer const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxDrawSystem dsys;
    avxBuffer   vbo;
    avxBuffer   ibo;
    avxBuffer   dbgLinesVbo;
    afxUnit     dbgLinesVboRange;
    afxUnit     dbgLinesVboStride;
    avxBuffer   dbgLinesIbo;
    afxUnit     dbgLinesIboRange;
    afxUnit     dbgLinesIboStride;

    avxPipeline dbgAabbPip;
    avxPipeline meshPip;
    avxRaster   texd;
    avxRaster   texn;

    afxUnit     width; // terrain width
    afxUnit     depth; // terrain depth
    afxUnit     secWidth; // sector width
    afxUnit     secDepth; // sector depth
    afxUnit     rowSecCnt; // row-sectors
    afxUnit     sliceSecCnt; // slice-sectors
    afxReal     heightScale;
    afxUri      heightmap;

    afxUnit     secCnt;
    _arxTerrSec*sectors;
    afxBox*     secAabb;

};
#endif//_ARX_TERRAIN_C

ARX afxClassConfig const _ARX_TER_CLASS_CONFIG;

#endif//ARX_TERRAIN_DDK_H
