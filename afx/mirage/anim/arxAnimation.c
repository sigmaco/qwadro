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

#define _ARX_GESTURE_C
#define _ARX_ANIMATION_C
//#define _ARX_SKELETON_C
//#define _ARX_BODY_C
//#define _ARX_MODEL_C
//#define _ARX_PULLEY_C
#include "../scene/arxIcd.h"

////////////////////////////////////////////////////////////////////////////////

_ARX arxScenario ArxGetAnimationHost(arxAnimation ani)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    arxScenario scio = AfxGetHost(ani);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio;
}

_ARX afxBool ArxGetAnimationUrn(arxAnimation ani, afxString* id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    *id = ani->urn;
    return ani->urn.len;
}

_ARX afxBool _ArxAniFindGestureCb(arxAnimation ani, afxString const* id, afxUnit* gesIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    AFX_ASSERT(gesIdx);
    afxBool found = FALSE;
    return err;
}

_ARX afxBool ArxFindGesture(arxAnimation ani, afxString const* id, afxUnit* gesIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(id);
    AFX_ASSERT(gesIdx);
    afxBool found = FALSE;

    if (!(found = ani->ddi->findGestureCb(ani, id, gesIdx)))
    {
        AfxThrowError();
    }

    return found;
}

_ARX afxUnit ArxGetGestures(arxAnimation ani, afxUnit first, afxUnit cnt, arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->gesSlotCnt, first, cnt);

    first = AFX_MIN(first, ani->gesSlotCnt);
    cnt = AFX_MIN(cnt, ani->gesSlotCnt - first);

    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit slotIdx = first + i;
        AFX_ASSERT_RANGE(ani->gesSlotCnt, slotIdx, 1);
        arxLinkedGesture* slot = &ani->gesSlots[slotIdx];

        arxGesture ges = slot->ges;

        if (ges)
        {
            AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
            ++rslt;
        }
        gestures[i] = ges;
    }
    return rslt;
}

_ARX afxError _ArxAniRelinkGesturesCb(arxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->gesSlotCnt, baseSlot, slotCnt);

    baseSlot = AFX_MIN(baseSlot, ani->gesSlotCnt);
    slotCnt = AFX_MIN(slotCnt, ani->gesSlotCnt - baseSlot);

    return err;
}

_ARX afxError ArxRelinkGestures(arxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT_RANGE(ani->gesSlotCnt, baseSlot, slotCnt);

    baseSlot = AFX_MIN(baseSlot, ani->gesSlotCnt);
    slotCnt = AFX_MIN(slotCnt, ani->gesSlotCnt - baseSlot);

    if ((err = ani->ddi->relinkCb(ani, baseSlot, slotCnt, gestures)))
    {
        AfxThrowError();
    }

    return err;
}

_ARX afxUnit _ArxAniPerformCb(arxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxString const gestures[], arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(puppets);
    afxUnit rslt = 0;
    return rslt;
}

_ARX afxUnit ArxPerformAnimation(arxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxString const gestures[], arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    AFX_ASSERT(puppets);
    afxUnit rslt = 0;

    if (!(rslt = ani->ddi->performCb(ani, startTime, iterCnt, cnt, gestures, puppets)))
    {
        AfxThrowError();
    }

    return rslt;
}

_ARX _arxDdiAni const _ARX_DDI_ANI =
{
    .findGestureCb = _ArxAniFindGestureCb,
    .relinkCb = _ArxAniRelinkGesturesCb,
    .performCb = _ArxAniPerformCb,
    //.performInstCb = NIL/*_ArxAniPerformInstCb*/,
};

_ARX afxError _ArxAniDtorCb(arxAnimation ani)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);

    for (afxUnit i = ani->gesSlotCnt; i-- > 0;)
    {
        arxLinkedGesture* alm = &ani->gesSlots[i];

        if (alm->ges)
            AfxDisposeObjects(1, &alm->ges);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ani->gesSlotCnt,
            .siz = sizeof(ani->gesSlots[0]),
            .var = (void**)&ani->gesSlots
        }
    };

    AfxDeallocateInstanceData(ani, ARRAY_SIZE(stashes), stashes);

    return err;
}

_ARX afxError _ArxAniCtorCb(arxAnimation ani, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxAnimationBlueprint const* anib = args[1];
    anib += invokeNo;

    ani->ddi = &_ARX_DDI_ANI;
    ani->idd = NIL;
    ani->udd = anib->udd;
    ani->tag = anib->tag;

    if (!AfxCatalogStrings(_ArxScioGetAnimUrnStringBase(scio), 1, &anib->id.s, &ani->urn))
        AfxThrowError();

    ani->dur = anib->dur;
    ani->timeStep = anib->timeStep;
    ani->oversampling = anib->oversampling;

    afxUnit gesCnt = anib->gesSlotCnt;
    ani->gesSlotCnt = gesCnt;
    ani->gesSlots = NIL;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = ani->gesSlotCnt,
            .siz = sizeof(ani->gesSlots[0]),
            .var = (void**)&ani->gesSlots
        }
    };

    if (AfxAllocateInstanceData(ani, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    if (gesCnt)
    {
        AfxZero(ani->gesSlots, sizeof(ani->gesSlots[0]) * gesCnt);
        
        if (anib->gestures && ArxRelinkGestures(ani, 0, gesCnt, anib->gestures))
            AfxThrowError();
    }
    AFX_ASSERT(ani->gesSlotCnt == gesCnt);

    return err;
}

_ARX afxClassConfig const _ARX_ANI_CLASS_CONFIG =
{
    .fcc = afxFcc_ANI,
    .name = "Animation",
    .desc = "Full Motion Animation", // or Multi-body Animation
    .fixedSiz = sizeof(AFX_OBJECT(arxAnimation)),
    .ctor = (void*)_ArxAniCtorCb,
    .dtor = (void*)_ArxAniDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAssembleAnimations(arxScenario scio, afxUnit cnt, arxAnimationBlueprint const blueprints[], arxAnimation animations[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(animations);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetAniClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)animations, (void const*[]) { scio, blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ANI, cnt, animations);

    return err;
}

_ARX void ArxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, arxAnimation animations[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(animations);
    AfxThrowError();
    (void)affineTol;
    (void)linearTol;
    AfxThrowError();
}
