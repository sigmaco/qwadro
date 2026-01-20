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

#define _ARX_SCENE_C
#define _ARX_SCENARIO_C
#define _ARX_NODE_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"

_ARX afxError ArxCmdRenderNode(arxRenderContext rctx, afxUnit cnt, arxNode nodes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    for (afxUnit nodIdx = 0; nodIdx < cnt; nodIdx++)
    {
        arxNode nod = nodes[nodIdx];
        if (!nod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

        if (nod->type == arxNodeType_Leaf_AnimBlend)
        {
            arxPuppet pup = nod->pup;
            if (!pup) continue;
            AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

            afxM4d w;
            ArxGetNodeMatrix(nod, w);
            ArxCmdRenderPuppets(rctx, w, 1, &pup);
        }
    }
    return err;
}
