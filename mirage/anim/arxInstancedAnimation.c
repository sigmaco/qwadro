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
#define _ARX_ANIMATION_C
#define _ARX_INSTANCED_ANIMATION_C
#define _ARX_GESTURE_C
#define _ARX_BODY_C
#define _ASX_SIMULATION_C
#include "../scene/arxIcd.h"

_ARX _arxDdiAnik const _ARX_DDI_ANIK =
{
    0
};

_ARX afxError _ArxAnikDtorCb(arxInstancedAnimation anik)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANIK, 1, &anik);



    return err;
}

_ARX afxError _ArxAnikCtorCb(arxInstancedAnimation anik, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANIK, 1, &anik);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(args[1]);
    arxTrackTarget const* target = AFX_CAST(arxTrackTarget const*, args[1]) + invokeNo;

    anik->ddi = &_ARX_DDI_ANIK;
    anik->idd = NIL;
    anik->udd = NIL;
    anik->tag = (afxString) { 0 };

    return err;
}

_ARX afxClassConfig const _ARX_ANIK_CLASS_CONFIG =
{
    .fcc = afxFcc_ANIK,
    .name = "InstancedAnimation",
    .desc = "Instanced Animation",
    .fixedSiz = sizeof(AFX_OBJECT(arxInstancedAnimation)),
    .ctor = (void*)_ArxAnikCtorCb,
    .dtor = (void*)_ArxAnikDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError _ArxScioInstanceAnimations(arxScenario scio, afxUnit cnt, arxTrackTarget const targets[], arxInstancedAnimation instances[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(instances);
    AFX_ASSERT(targets);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetAnikClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_ANIK);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)instances, (void const*[]) { scio, targets }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ANIK, cnt, instances);

    return err;
}
