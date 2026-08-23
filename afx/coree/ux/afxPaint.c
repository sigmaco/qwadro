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

// This software is part of Advanced User Experience Extensions.

#include "qwadro/ux/afxPaint.h"

void ClumpRenderBoundingBox(afxDrawContext dctx, afxBox box, afxM4d const m)
{
    // based on RenderWare void ClumpRenderBoundingBox(RpClump *clump);

    avxImmVertex vtx[8];
    avxVertexIndex const indices[24] = { 0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
    
    for (int i = 0; i < 8; i++)
    {
        vtx[i].posn = AFX_V4D( i & 1 ? box.max.v[0] : box.min.v[0], i & 2 ? box.max.v[1] : box.min.v[1], i & 4 ? box.max.v[2] : box.min.v[2], 1);
        vtx[i].col = AVX_ARGB(255, 196, 196, 0);
    }

    // set flat shade mode
    // set no raster

    //AvxCmdTransformImmVertices(dctx, m, 8, vtx);
    //AvxCmdPushImmIndexedPrimitive(dctx, avxTopology_LINE_LIST, 24, indices);
    
    return;
}




