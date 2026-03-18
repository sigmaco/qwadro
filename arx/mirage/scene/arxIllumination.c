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

#define _AFX_SIM_C
#define _ARX_LIGHT_C
#include "arxIcd.h"

_ARX afxError _ArxLitDtorCb(akxLight lit)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LIT, 1, &lit);

    return err;
}

_ARX afxError _ArxLitCtorCb(akxLight lit, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LIT, 1, &lit);

    arxScenario scio = args[0];
    akxLightType type = *(akxLightType*)args[1];
    lit->type = (type) ? type : akxLightType_SPOT;
    
    lit->distFadeBegin = 40.f;
    lit->distFadeLen = 10.f;
    lit->distFadeShadow = 50.f;
    lit->angularDist = 0.f;
    lit->bakeMode = 2;
    AvxMakeColor(lit->color, 1, 1, 1, 1);
    lit->cullMask = 0xFFFFFFFF;
    lit->energy = 1.f;
    lit->indirectEnergy = 1.f;
    lit->projector = NIL;
    lit->size = 0.f;
    lit->specular = 0.5f;    
    lit->volFogEnergy = 1.f;
    lit->shadowBias = 0.1f;
    lit->shadowBlur = 1.f;
    lit->shadowNormalBias = 2.f;
    lit->shadowOpacity = 1.f;
    lit->shadowTransBias = 0.05f;

    switch(type)
    {
    case akxLightType_OMNI:
        lit->omni.attenuation = 1.f;
        lit->omni.range = 5.f;
        lit->omni.shadowNormalBias = 1.f;
        break;
    case akxLightType_SPOT:
        lit->spot.shadowBias = 0.03f;
        lit->spot.shadowNormalBias = 1.f;
        lit->spot.angle = 45.f;
        lit->spot.angleAttenuation = 1.f;
        lit->spot.range = 5.f;
        break;
    case akxLightType_DIRECTIONAL:
        lit->dir.shadowBlendSplits = FALSE;
        lit->dir.shadowFadeStart = 0.8;
        lit->dir.shadowMaxDist = 100.f;
        lit->dir.shadowPancakeSiz = 20.f;
        lit->dir.shadowSplit1 = 0.1f;
        lit->dir.shadowSplit2 = 0.2f;
        lit->dir.shadowSplit3 = 0.5f;
        break;
    default: AfxThrowError(); break;
    };
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireDirectionalLights(arxScenario scio, afxUnit cnt, afxUnit uid[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_ArxScioGetLitClass(scio), 1, (afxObject*)&lit, (void const*[]) { scio, (akxLightType[]) { akxLightType_DIRECTIONAL } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ARX afxError ArxAcquireOmniLights(arxScenario scio, afxUnit cnt, afxUnit uid[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_ArxScioGetLitClass(scio), 1, (afxObject*)&lit, (void const*[]) { scio, (akxLightType[]) { akxLightType_OMNI } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ARX afxError ArxAcquireSpotLights(arxScenario scio, afxUnit cnt, afxUnit uid[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    akxLight lit;

    for (afxUnit i = 0; i < cnt; i++)
        if (AfxAcquireObjects((afxClass *)_ArxScioGetLitClass(scio), 1, (afxObject*)&lit, (void const*[]) { scio, (akxLightType[]) { akxLightType_SPOT } })) AfxThrowError();
        else
            uid[i] = AfxGetObjectId(lit);

    return err;
}

_ARX afxClassConfig const _ARX_LIT_CLASS_CONFIG =
{
    .fcc = afxFcc_LIT,
    .name = "Light",
    .fixedSiz = sizeof(AFX_OBJECT(akxLight)),
    .ctor = (void*)_ArxLitCtorCb,
    .dtor = (void*)_ArxLitDtorCb
};
