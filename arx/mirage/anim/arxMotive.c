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
#define _ARX_PULLEY_C
#define _ARX_MOTIVE_C
#define _ARX_ANIMATION_C
#define _ARX_GESTURE_C
#define _ARX_BODY_C
#define _ASX_SIMULATION_C
#include "../scene/arxIcd.h"

_ARX _arxDdiMotv const _ARX_DDI_MOTV =
{

};

_ARX afxError _ArxMotvDtorCb(arxMotive motv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MOTV, 1, &motv);

    return err;
}

_ARX afxError _ArxMotvCtorCb(arxMotive motv, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MOTV, 1, &motv);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    // args[1];
    arxPulley moto = args[2];
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &moto);
    AFX_ASSERT(args[3]);
    arxTrackTarget const* target = AFX_CAST(arxTrackTarget const*, args[3]) + invokeNo;

    motv->ddi = &_ARX_DDI_MOTV;
    motv->idd = NIL;
    motv->udd = NIL;
    motv->tag = (afxString) { 0 };

    return err;
}

_ARX afxClassConfig const _ARX_MOTV_CLASS_CONFIG =
{
    .fcc = afxFcc_MOTV,
    .name = "Motive",
    .desc = "Motive",
    .fixedSiz = sizeof(AFX_OBJECT(arxMotive)),
    .ctor = (void*)_ArxMotvCtorCb,
    .dtor = (void*)_ArxMotvDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError _ArxScioAcquireMotives(arxScenario scio, afxUnit cnt, arxTrackTarget const targets[], arxMotive motives[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(targets);
    AFX_ASSERT(motives);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetMotvClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MOTV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)motives, (void const*[]) { scio, targets
    }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MOTV, cnt, motives);

    return err;
}
