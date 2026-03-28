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
#include "qwadro/coll/arxOctree.h"

#if 0
ASX void AfxDeployOctree(arxOctree* tree, afxBox* bounds)
{
    tree->root.aabb = *bounds;
    tree->root.parent = NIL;

    for (afxUnit i = 0; i < /* it is always a octree*/ 8; i++)
        tree->root.child[i] = NIL;

    AfxMakeChain(&tree->root.contents, tree);
    AfxSetUpPool(&tree->octants, sizeof(arxOctant), /* it is always a octree*/ 8, AFX_SIMD_ALIGNMENT);
}

ASX afxError AfxSubdivideOctree(arxOctant* node)
// Subdivide the node into 8 octants
{
    afxError err = { 0 };
    AFX_ASSERT(node);

    if (node->child[0] != NIL)
    {
        AfxThrowError();
        return err;
    }

    afxReal midX = (node->aabb.min[0] + node->aabb.max[0]) / 2;
    afxReal midY = (node->aabb.min[1] + node->aabb.max[1]) / 2;
    afxReal midZ = (node->aabb.min[2] + node->aabb.max[2]) / 2;

    afxBox bounds[8];
    for (afxUnit i = 0; i < 8; i++)
    {
        if (i & 1) bounds[i].min[0] = midX;
        else bounds[i].max[0] = midX;
        if (i & 2) bounds[i].min[1] = midY;
        else bounds[i].max[1] = midY;
        if (i & 4) bounds[i].min[2] = midZ;
        else bounds[i].max[2] = midZ;
    }

    // Create the 8 new children with updated bounding boxes
    for (afxUnit i = 0; i < 8; i++)
    {
        afxSize childIdx;
        arxOctant* child = NIL;
        if (!(AfxRequestPoolUnits(&tree->octants, 1, &childIdx, &child)))
        {
            AfxThrowError();

            for (afxUnit j = i; j-- > 0;)
            {
                childIdx = j;
                // TODO: Fix this shit
                AfxReclaimPoolUnits(&tree->octants, 1, &child);
                node->child[j] = NIL;
            }
            break;
        }

        child->aabb = bounds[i];

        child->parent = node;
        for (afxUnit j = 0; j < 8; j++)
            child->child[j] = NIL;

        AfxMakeChain(&child->contents, child);

        node->child[i] = childIdx;
    }
    return err;
}

// Function to insert an AABB into the Octree
void AfxOctreeInsertAabbs(arxOctant* node, afxBox* box)
{
    afxError err = { 0 };

    // expand the tree if it does not fit.
    AfxEmboxAabbs(&tree->root.aabb, 1, box);

    AFX_ASSERT(node);

    // Check if the AABB intersects with the node's bounding box. No intersection, don't insert.
    if (!AfxDoesAabbInclude(&node->aabb, 1, box))
        return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->child[0] == NIL)
        if (AfxSubdivideOctree(node))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        arxOctant* child = node->child[i];

        if (AfxDoesAabbInclude(&child->aabb, 1, box))
        {
            AfxOctreeInsertAabbs(child, box);  // Insert into the appropriate child
            inserted = TRUE;
        }
    }

    // If the AABB is completely inside the node (and doesn't span children),
    // you could store it at the node's level (depending on your use case).
    // For now, we don't store data at the node level in this example.

    AFX_ASSERT(inserted);
}

void AfxOctreeInsertAtv3d(arxOctant* node, afxV3d p)
// Insert a point into the Octree
{
    afxError err = { 0 };

    // If the point is outside the bounding box, do not insert
    if (!AfxDoesAabbIncludeAtv3d(&node->aabb, 1, p)) return;

    // If the node doesn't have children, check if it's time to subdivide
    if (node->child[0] == NIL)
        if (AfxSubdivideOctree(node))
            AfxThrowError();

    // Insert the AABB into the children or store it in the node itself
    afxBool inserted = FALSE;
    for (afxUnit i = 0; i < 8; i++)
    {
        arxOctant* child = node->child[i];
        
        if (AfxDoesAabbIncludeAtv3d(&child->aabb, 1, p))
        {
            AfxOctreeInsertAabbs(child, p);  // Insert into the appropriate child
            inserted = TRUE;
            break;
        }
    }

    AFX_ASSERT(inserted);
}
#endif