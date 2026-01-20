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

#ifndef ARX_WIREFRAME_H
#define ARX_WIREFRAME_H

#include "qwadro/iris/arxIrisDefs.h"

/*
    Wireframe should support line lists, line strips and face-based (degenerate) lines.
*/

typedef enum arxSceneMode
{
    // Classic wireframe mode.
    // Draw real lines consuming avxTopology_LINE_LIST.
    arxSceneMode_WIRE_LINES = AFX_BITMASK(0),

    // Draw real lines consuming avxTopology_LINE_STRIP.
    arxSceneMode_WIRE_LINE_STRIP = AFX_BITMASK(1),

    // Draw faces' edges consuming avxTopology_TRI_LIST.
    arxSceneMode_WIRE_EDGES = AFX_BITMASK(2),

    // Draw faces' edges consuming avxTopology_TRI_STRIP.
    arxSceneMode_WIRE_EDGE_STRIP = AFX_BITMASK(3),

    // Modern wireframe mode.
    // Draw line-shaped faces consuming avxTopology_TRI_LIST with barycentrics.
    arxSceneMode_WIRE_FACES = AFX_BITMASK(4),

    // Classic solid mode; drawing faces.
    arxSceneMode_FACE = AFX_BITMASK(10),
    arxSceneMode_TEXTURED = AFX_BITMASK(11),
    arxSceneMode_COLORED = AFX_BITMASK(12),
} arxSceneMode;

AFX_DEFINE_STRUCT(arxWireframePush)
{
    afxM4d  uM; // transform matrix
    afxV3d  uWireColor; // = vec3(1.0, 1.0, 1.0);
    afxReal uWireThickness; // = 0.02;
    afxV3d  uFillColor; // = vec3(0.1, 0.1, 0.1);
};

AFX_DEFINE_STRUCT(arxWireVertex)
{
    afxV3d pos;
    afxV3d bary;
};

ARX afxError ArxLoadWireframePipeline(arxRenderContext rctx, arxSceneMode mode, afxUri const* uri, avxPipeline* pipeline);

ARX void SphereDebugDraw_barycentric(arxRenderContext rctx, afxM4d const m, afxReal radius);
ARX void SphereDebugDraw(arxRenderContext rctx, afxM4d const m, afxReal radius);
ARX void drawWireSphere(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs);
ARX void drawWireSphereLineStrip(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs);

ARX void drawWireBox(arxRenderContext rctx, afxM4d const m, afxV3d extents);
ARX void drawWireBox2(arxRenderContext rctx, afxV3d const center, afxQuat const rotation, afxV3d const halfExtents);

ARX afxError ArxSetWireframeConstants(arxRenderContext rctx, afxReal wireThickness, avxColor wireCol, avxColor fillCol);
ARX afxError ArxPushTransform(arxRenderContext rctx, afxM4d const m);

ARX void ArxDrawLine(arxRenderContext rctx, afxV3d origin, afxV3d target);

#endif//ARX_WIREFRAME_H
