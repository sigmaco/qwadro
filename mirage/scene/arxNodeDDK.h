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

#ifndef ARX_NODE_DDK_H
#define ARX_NODE_DDK_H

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/scene/arxScenario.h"
#include "qwadro/iris/arxRenderContext.h"
#include "../cad/arxModelDDK.h"
//#include "arxAnimationImpl.h"
//#include "arxBodyImpl.h"
//#include "arxCurveImpl.h"
#include "../scene/arxBufferDDK.h"

#include "../anim/arxAnimationDDK.h"
#include "../anim/arxCurveDDK.h"
#include "../anim/arxPuppetDDK.h"

#ifdef _ARX_NODE_C
#ifdef _ARX_NODE_IMPL
AFX_OBJECT(_arxNode)
#else
AFX_OBJECT(arxNode)
#endif
{
    arxNodeFlag    flags;
    // Allocation-based children were just a overcomplicated way of doing it as it is ALWAYS a DAG (not double reference or reentrance allowed).
    arxNode         root;
    afxLink         parent;
    afxChain        children;
    afxChain        nodulars;

    // ARX
    // Unlike RenderWare, Qwadro uses a afxTransform to work with node transformation to avoid orthonormalizations required in RW.
    afxTransform t;
    // Like RenderWare, Qwadro uses two matrices but both are cached once it uses a afxTransform to do transformations.
    afxM4d w, m;
    afxBool dirty;
    //arxPosture plce;
    afxUnit ltaCnt;
    afxUnit taCnt;

    arxPose pose; // pose
    arxPuppet pup; // anim
    afxReal fillThreshold; // anim, weighted blend
    void *udd; // cb
    afxError(*sampleCb)(void*, afxReal, arxPose, afxUnit, afxUnit const*); // cb
    void(*setClockCb)(void*, afxReal time); // cb
    void(*motionVectorsCb)(void*, afxReal, afxReal*, afxReal*, afxBool); // cb
    afxReal weightNone; // crossfade
    afxReal weightAll; // crossfade
    arxTrackMask*trackMask; // comm, crossfade
    afxBool ownedTrackMask; // comm, crossfade
    arxModel skl; // weighted blend
    afxQuatBlend quatMode; // weighted blend

    arxNodeType type;
    afxReal weight; // comm

};
#endif//_ARX_NODE_C

ASX afxClassConfig const _ARX_NOD_CLASS_CONFIG;

#endif//ARX_NODE_DDK_H
