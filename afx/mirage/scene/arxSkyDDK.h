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

#ifndef ARX_SKY_DDK_H
#define ARX_SKY_DDK_H

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/scene/arxScenario.h"
#include "qwadro/iris/arxRenderContext.h"
#include "../cad/arxModelDDK.h"
//#include "arxAnimationImpl.h"
//#include "arxBodyImpl.h"
//#include "arxCurveImpl.h"
#include "../scene/arxBufferDDK.h"
#include "../scene/arxCameraDDK.h"
#include "../scene/arxNodeDDK.h"

#include "../anim/arxAnimationDDK.h"
#include "../anim/arxCurveDDK.h"
#include "../anim/arxPuppetDDK.h"


#ifdef _ARX_SKY_C

AFX_OBJECT(arxSky)
{
    arxScenario    scio;
    arxSkyType      skyType;
    arxMesh         skyMsh;
    avxColor        apexCol;
    avxColor        centreCol;
    struct
    {
        avxBuffer       cube;
        avxRaster       cubemap;
        afxReal         cubemapColorIntensity;
        afxV3d          fogColor;
        afxReal         fogUpper;
        afxReal         fogLower;
        avxColor        emissiveColor;
        avxColor        ambientColor;
        afxReal         currRot;
        afxReal         rotSpeed;
        afxV3d          rotPivot;
        afxQuat         rotQuat;
        afxM4d          rotMtx;
        arxTechnique skyDtec;
        avxVertexInput  skyVin;
        avxSampler      smp;
    }               sky;
};

#endif//_ARX_SKY_C

ARX afxClassConfig const _ARX_SKY_CLASS_CONFIG;

#endif//ARX_SKY_DDK_H
