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

#ifndef ARX_LIGHT_H
#define ARX_LIGHT_H

#include "qwadro/iris/arxIrisDefs.h"

#define AFX_LIT_ALIGN AFX_SIMD_ALIGNMENT

// light rigs as in Doom

typedef enum
{
    // A light placed infinitely far away. It affects everything in the scene.
    akxLightType_DIRECTIONAL,

    // A light that shines equally in all directions from its location, affecting all objects within its Range.
    akxLightType_OMNI,

    // A light that shines everywhere within a cone defined by Spot Angle and Range. Only objects within this region are affected by the light.
    akxLightType_SPOT,

    // A light that shines in all directions to one side of a rectangular area of a plane. The rectangle is defined by the Width and Height properties.
    // Area lights are only available during lightmap baking and have no effect on objects at runtime.
    akxLightType_AREA
} akxLightType;

typedef enum akxLightFlag
{
    akxLightFlag_FADE_ENABLED   = AFX_BITMASK(0), // false
    akxLightFlag_NEGATIVE       = AFX_BITMASK(1), // false
    akxLightFlag_SHADOW_ENABLED = AFX_BITMASK(2), // false
    akxLightFlag_REV_CULL_FACE  = AFX_BITMASK(3), // false
} akxLightFlags;

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxAcquireDirectionalLights(arxScenario scio, afxUnit cnt, afxUnit uid[]);
ARX afxError    ArxAcquireOmniLights(arxScenario scio, afxUnit cnt, afxUnit uid[]);
ARX afxError    ArxAcquireSpotLights(arxScenario scio, afxUnit cnt, afxUnit uid[]);

#endif//ARX_LIGHT_H
