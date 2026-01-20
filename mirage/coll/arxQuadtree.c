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


ARX void AfxDeployQuadtree(asxQuadtree* tree, afxRect* bounds)
{
    tree->root.bounds = *bounds;
    tree->root.parentIdx = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < /* it is always a quadtree*/ 4; i++)
        tree->root.childIdx[i] = AFX_INVALID_INDEX;

    AfxMakeChain(&tree->root.contents, tree);
    AfxSetUpPool(&tree->quadrants, sizeof(asxQuadtree), /* it is always a quadtree*/ 4, AFX_SIMD_ALIGNMENT);
}

