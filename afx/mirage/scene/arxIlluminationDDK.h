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

#ifndef ARX_ILLUMINATION_DDK_H
#define ARX_ILLUMINATION_DDK_H

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


#ifdef _ARX_LIGHT_C
AFX_OBJECT(akxLight)
{
    afxReal     distFadeBegin; // 40.f
    // A luz será suavemente esvanecida afora quando longe da câmera ativa iniciando de distFadeBegin.
    // Isto atua como uma forma de LOD. A luz será esmaecida afora sobre distFadeBegin + distFadeLen, após a qual será coligida e não enviada a shader.

    afxReal     distFadeLen; // 10.f
    // Distância sobre a qual a luz e sua sombra esmaecem. A energia da luz e a opacidade da sombra são progressivamente reduzidas sobre esta distância e é completamente invisível ao fim.

    afxReal     distFadeShadow; // 50.f
    // A distância da câmera na qual a sombra da luz corta (em unidades 3D).

    afxReal     angularDist; // 0.f

    afxUnit32    bakeMode; // 2
    avxColor    color; // [1, 1, 1, 1]
    afxMask     cullMask; // 0xFFFFFFFF
    afxReal     energy; // 1.f
    afxReal     indirectEnergy; // 1.f
    afxReal     intensLumens;
    afxReal     intensLux;
    avxRaster  projector;
    afxReal     size; // 0.f
    afxReal     specular; // 0.5f
    afxReal     temperature;
    afxReal     volFogEnergy; // 1.f
    afxReal     shadowBias; // 0.1f
    afxReal     shadowBlur; // 1.f
    afxReal     shadowNormalBias; // 2.f
    afxReal     shadowOpacity; // 1.f
    afxReal     shadowTransBias; // 0.05f

    akxLightType            type;
    union
    {
        struct
        {
            afxReal attenuation; // 1.f
            afxReal range; // 5.f
            // shadow mode
            afxReal shadowNormalBias; // 1.0
        } omni;
        struct
        {
            afxReal shadowBias; // 0.03f
            afxReal shadowNormalBias; // 1.f
            afxReal angle; // 45.f
            afxReal angleAttenuation; // 1.f
            afxReal range; // 5.f
        } spot;
        struct
        {
            afxBool shadowBlendSplits; // false
            afxReal shadowFadeStart; // 0.8
            afxReal shadowMaxDist; // 100.f
            // shadow mode
            afxReal shadowPancakeSiz; // 20.f
            afxReal shadowSplit1; // 0.1f
            afxReal shadowSplit2; // 0.2f
            afxReal shadowSplit3; // 0.5f
            // sky mode
        } dir;
    };
};
#endif//_ARX_LIGHT_C

ARX afxClassConfig const _ARX_LIT_CLASS_CONFIG;

#endif//ARX_ILLUMINATION_DDK_H
