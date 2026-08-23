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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
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

    for (afxUnit i = 0; i < AFX_QUADTREE_QUADRANTS; i++)
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

_ARX void ArxQuadrantMergeChildBounds(asxQuadrant* q, afxUnit idx, afxBox const* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    q->maxZ[idx] = AFX_MIN(q->maxZ[idx], bounds->max.v[2]);
    q->maxY[idx] = AFX_MIN(q->maxY[idx], bounds->max.v[1]);
    q->maxX[idx] = AFX_MIN(q->maxX[idx], bounds->max.v[0]);

    q->minZ[idx] = AFX_MIN(q->minZ[idx], bounds->min.v[2]);
    q->minY[idx] = AFX_MIN(q->minY[idx], bounds->min.v[1]);
    q->minX[idx] = AFX_MIN(q->minX[idx], bounds->min.v[0]);

}

_ARX void ArxQuadrantSetChildBounds(asxQuadrant* q, afxUnit idx, afxBox const* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(bounds->min.v[0] >= -AFX_R32_MAX && bounds->min.v[0] <= AFX_R32_MAX
            && bounds->min.v[1] >= -AFX_R32_MAX && bounds->min.v[1] <= AFX_R32_MAX
            && bounds->min.v[2] >= -AFX_R32_MAX && bounds->min.v[2] <= AFX_R32_MAX
            && bounds->max.v[0] >= -AFX_R32_MAX && bounds->max.v[0] <= AFX_R32_MAX
            && bounds->max.v[1] >= -AFX_R32_MAX && bounds->max.v[1] <= AFX_R32_MAX
            && bounds->max.v[2] >= -AFX_R32_MAX && bounds->max.v[2] <= AFX_R32_MAX);

    // Intentionally reversed in order.
    // By setting the max first, we keep the bounds invalid for reading threads.
    q->maxZ[idx] = bounds->max.v[2];
    q->maxY[idx] = bounds->max.v[1];
    q->maxX[idx] = bounds->max.v[0];

    q->minZ[idx] = bounds->min.v[2];
    q->minY[idx] = bounds->min.v[1];
    q->minX[idx] = bounds->min.v[0];

}

_ARX afxBox ArxQuadrantGetChildBounds(asxQuadrant* q, afxUnit idx)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    // Read bounding box in order min -> max
    return AfxMakeAabb(2, (afxV3d[]) {  AFX_V3D(q->minX[idx], q->minY[idx], q->minZ[idx]),
                                        AFX_V3D(q->maxX[idx], q->maxY[idx], q->maxZ[idx]) });
}

_ARX afxBox ArxQuadrantGetBounds(asxQuadrant* q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);

    afxBox bounds = ArxQuadrantGetChildBounds(q, 0);

    for (afxUnit i = 1; i < AFX_QUADTREE_QUADRANTS; i++)
    {
        afxBox aabb = ArxQuadrantGetChildBounds(q, i);
        bounds = AfxEmboxAabbs(bounds, 1, &aabb);
    }
    return bounds;
}

////////////////////////////////////////////////////////////////////////////////

_ARX void ArxDeployQuadtree(asxQuadtree* tree, afxRect* bounds)
{
    afxError err = { 0 };
    AFX_ASSERT(tree);
    //tree->root.bounds = *bounds;
    //tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < AFX_QUADTREE_QUADRANTS; i++)
    {
        //tree->
        //tree->root.childId[i] = AFX_INVALID_INDEX;
    }

    ArxQuadrantSet(&tree->root, TRUE);
    AfxMakeChain(&tree->root.contents, tree);
    AfxSetUpPool(&tree->quadrants, sizeof(asxQuadtree), AFX_QUADTREE_QUADRANTS * 4, AFX_SIMD_ALIGNMENT);
}

_ARX afxError ArxExhaustQuadtree(asxQuadtree* tree)
{
    afxError err = { 0 };
    AFX_ASSERT(tree);
    AfxExhaustPool(&tree->quadrants, FALSE);
    return err;
}
