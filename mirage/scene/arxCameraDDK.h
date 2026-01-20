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

#ifndef ARX_CAMERA_DDK_H
#define ARX_CAMERA_DDK_H

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

#ifdef _ARX_CAMERA_C
#ifdef _ARX_CAMERA_IMPL
AFX_OBJECT(_arxCamera)
#else
AFX_OBJECT(arxCamera)
#endif
{
    afxCameraFlags      flags;
    afxReal             wpOverHp; // physical w/h
    afxReal             wrOverHr; // resolution w/h
    afxReal             wwOverHw; // window w/h
    afxReal             fovY;
    afxReal             nearClipPlane;
    afxReal             farClipPlane;
    afxReal             depthRangeEpsilon;
    avxClipSpaceDepth     depthRange;
    afxBool             useQuatOrient;
    afxQuat             orient;
    afxM3d              orientM3d;
    afxV3d              elevAzimRoll;
    afxV3d              pos;
    afxV3d              displace;
    afxM4d              v, iv; // inverse view
    afxM4d              p, ip; // inverse projection
    afxM4d              pv, ipv; // IP * IV
    afxBool             shouldSyncV;
    afxBool             shouldSyncP;

    arxNodular          nodu;

    afxBool     perspective; // is projection perspective or orthographic.
    afxFrustum  frustum;
};
#endif//_ARX_CAMERA_C

//AFX_STATIC_ASSERT(offsetof(arxCamera, focus % AFX_CAM_ALIGN == 0, "");

ARX afxClassConfig const _ARX_CAM_CLASS_CONFIG;

#endif//ARX_CAMERA_DDK_H
