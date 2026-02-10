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


AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, asxQuadrant)
{
    // SIMD friendly, data-driven
    afxReal     minX[4];
    afxReal     minY[4];
    afxReal     minZ[4];
    afxReal     maxX[4];
    afxReal     maxY[4];
    afxReal     maxZ[4];
    afxUnit     childId[4];
    afxUnit     parentId;
    afxBool     changed;
    afxChain    contents;
};

AFX_DEFINE_STRUCT(asxQuadtree)
{
    asxQuadrant root;
    afxPool     quadrants;
};


#endif//ASX_QUADTREE_H
