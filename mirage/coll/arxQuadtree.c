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

#include "../scene/arxIcd.h"
#include "qwadro/coll/arxQuadtree.h"

_ARX void ArxQuadrantResetBounds(asxQuadrant* q, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    q->minX[idx] = AFX_R32_MAX;
    q->minY[idx] = AFX_R32_MAX;
    q->minZ[idx] = AFX_R32_MAX;
    q->maxX[idx] = -AFX_R32_MAX;
    q->maxY[idx] = -AFX_R32_MAX;
    q->maxZ[idx] = -AFX_R32_MAX;
}

_ARX void ArxQuadrantSet(asxQuadrant* q, afxBool isChanged)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    q->changed = isChanged;

    for (afxUnit i = 0; i < /* it is always a quadtree*/ 4; i++)
    {
        q->minX[i] = AFX_R32_MAX;
        q->minY[i] = AFX_R32_MAX;
        q->minZ[i] = AFX_R32_MAX;
        q->maxX[i] = -AFX_R32_MAX;
        q->maxY[i] = -AFX_R32_MAX;
        q->maxZ[i] = -AFX_R32_MAX;
        q->childId[i] = AFX_INVALID_INDEX;
    }
}

_ARX void ArxQuadrantSetChildBounds(asxQuadrant* q, afxUnit idx, afxBox const* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(bounds->min[0] >= -AFX_R32_MAX && bounds->min[0] <= AFX_R32_MAX
            && bounds->min[1] >= -AFX_R32_MAX && bounds->min[1] <= AFX_R32_MAX
            && bounds->min[2] >= -AFX_R32_MAX && bounds->min[2] <= AFX_R32_MAX
            && bounds->max[0] >= -AFX_R32_MAX && bounds->max[0] <= AFX_R32_MAX
            && bounds->max[1] >= -AFX_R32_MAX && bounds->max[1] <= AFX_R32_MAX
            && bounds->max[2] >= -AFX_R32_MAX && bounds->max[2] <= AFX_R32_MAX);

    // Intentionally reversed in order.
    // By setting the max first, we keep the bounds invalid for reading threads.
    q->maxZ[idx] = bounds->max[2];
    q->maxY[idx] = bounds->max[1];
    q->maxX[idx] = bounds->max[0];

    q->minZ[idx] = bounds->min[2];
    q->minY[idx] = bounds->min[1];
    q->minX[idx] = bounds->min[0];

}

_ARX void ArxQuadrantGetChildBounds(asxQuadrant* q, afxUnit idx, afxBox* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    // Read bounding box in order min -> max
    AfxMakeAabb(bounds, 2, (afxV3d[]) { { q->minX[idx], q->minY[idx], q->minZ[idx] },
                                        { q->maxX[idx], q->maxY[idx], q->maxZ[idx] } });
}

_ARX void ArxQuadrantGetBounds(asxQuadrant* q, afxBox* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    ArxQuadrantGetChildBounds(q, 0, bounds);

    for (afxUnit i = 1; i < /* it is always a quadtree*/ 4; i++)
    {
        afxBox aabb;
        ArxQuadrantGetChildBounds(q, i, &aabb);
        AfxEmboxAabbs(bounds, 1, &aabb);
    }
}

_ARX void ArxDeployQuadtree(asxQuadtree* tree, afxRect* bounds)
{
    //tree->root.bounds = *bounds;
    //tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < /* it is always a quadtree*/ 4; i++)
    {
        //tree->
        //tree->root.childId[i] = AFX_INVALID_INDEX;
    }

    AfxMakeChain(&tree->root.contents, tree);
    AfxSetUpPool(&tree->quadrants, sizeof(asxQuadtree), /* it is always a quadtree*/ 4, AFX_SIMD_ALIGNMENT);
}

