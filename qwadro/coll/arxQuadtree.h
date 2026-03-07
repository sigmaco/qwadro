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

// This file is part of Advanced RenderWare Extensions.

#ifndef ASX_QUADTREE_H
#define ASX_QUADTREE_H

#include "qwadro/base/afxObject.h"
#include "qwadro/base/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/coll/afxBox.h"
#include "qwadro/coll/afxSphere.h"
#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxPosture.h"

#define AFX_QUADTREE_QUADRANTS 4
#define AFX_INVALID_QUADRANT AFX_INVALID_INDEX

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, asxQuadrant)
{
    // SIMD friendly, data-driven
    afxReal     minX[AFX_QUADTREE_QUADRANTS];
    afxReal     minY[AFX_QUADTREE_QUADRANTS];
    afxReal     minZ[AFX_QUADTREE_QUADRANTS];
    afxReal     maxX[AFX_QUADTREE_QUADRANTS];
    afxReal     maxY[AFX_QUADTREE_QUADRANTS];
    afxReal     maxZ[AFX_QUADTREE_QUADRANTS];
    afxUnit     childId[AFX_QUADTREE_QUADRANTS];
    afxUnit     parentId;
    afxBool     changed;
    afxChain    contents;
    afxUnit     contents2[AFX_QUADTREE_QUADRANTS * 4]; // 16
    //afxUnit     contents3[];
};

AFX_DEFINE_STRUCT(asxQuadtree)
{
    asxQuadrant root;
    afxPool     quadrants;
};

ARX void ArxDeployQuadtree(asxQuadtree* tree, afxRect* bounds);

ARX void ArxQuadrantResetBounds(asxQuadrant* q, afxUnit idx);

ARX void ArxQuadrantSet(asxQuadrant* q, afxBool isChanged);

ARX void ArxQuadrantMergeChildBounds(asxQuadrant* q, afxUnit idx, afxBox const* bounds);

ARX void ArxQuadrantSetChildBounds(asxQuadrant* q, afxUnit idx, afxBox const* bounds);

ARX void ArxQuadrantGetChildBounds(asxQuadrant* q, afxUnit idx, afxBox* bounds);

ARX void ArxQuadrantGetBounds(asxQuadrant* q, afxBox* bounds);

#endif//ASX_QUADTREE_H
