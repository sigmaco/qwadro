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
//#define _ARX_SIMULATION_C
#define _ARX_GESTURE_C
#define _ARX_ANIMATION_C
//#define _ARX_SKELETON_C
//#define _ARX_BODY_C
//#define _ARX_MODEL_C
//#define _ARX_PULLEY_C
#include "../scene/arxIcd.h"

_ARX arxScenario ArxGetGestureHost(arxGesture ges)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    arxScenario scio = AfxGetHost(ges);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio;
}

_ARX afxMask ArxGetGestureFlags(arxGesture ges)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    return ges->flags;
}

_ARXINL void ArxComputeGestureDisplacement(arxGesture ges, afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&ges->displacement, m);
}

_ARXINL afxBool _ArxGesFindTransformCb(arxGesture ges, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool found = FALSE;
    return found;
}

_ARXINL afxBool ArxFindGestureTransform(arxGesture ges, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);

    afxBool found = FALSE;

    if (!(found = ges->ddi->findMatCb(ges, seqId, seqIdx)))
    {
        *seqIdx = AFX_INVALID_INDEX;
    }

    return found;
}

_ARXINL afxBool _ArxGesFindVectorCb(arxGesture ges, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);
    afxBool rslt = FALSE;
    return rslt;
}

_ARXINL afxBool ArxFindGestureVector(arxGesture ges, afxString const* seqId, afxUnit *seqIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(seqId);
    AFX_ASSERT(seqIdx);

    afxBool rslt = FALSE;
    
    if (!(rslt = ges->ddi->findVecCb(ges, seqId, seqIdx)))
    {
        *seqIdx = AFX_INVALID_INDEX;
    }

    return rslt;
}

_ARX void _ArxGesUpdateVectorsCb(arxGesture ges, afxUnit baseSeqIdx, afxUnit seqCnt, arxVectorialGesture const vectors[], afxUnit fetchRate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT_RANGE(ges->vecCnt, baseSeqIdx, seqCnt);
    AFX_ASSERT(vectors);
}

_ARX void ArxUpdateGestureVectors(arxGesture ges, afxUnit baseSeqIdx, afxUnit seqCnt, arxVectorialGesture const vectors[], afxUnit fetchRate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT_RANGE(ges->vecCnt, baseSeqIdx, seqCnt);
    AFX_ASSERT(vectors);
    ges->ddi->updVecCb(ges, baseSeqIdx, seqCnt, vectors, fetchRate);
}

_ARX void _ArxGesUpdateTransformsCb(arxGesture ges, afxUnit baseSeqIdx, afxUnit seqCnt, arxPivotalGesture const transforms[], afxUnit fetchRate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT_RANGE(ges->pivotCnt, baseSeqIdx, seqCnt);
    AFX_ASSERT(transforms);
}

_ARX void ArxUpdateGestureTransforms(arxGesture ges, afxUnit baseSeqIdx, afxUnit seqCnt, arxPivotalGesture const transforms[], afxUnit fetchRate)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT_RANGE(ges->pivotCnt, baseSeqIdx, seqCnt);
    AFX_ASSERT(transforms);
    ges->ddi->updMatCb(ges, baseSeqIdx, seqCnt, transforms, fetchRate);
}

_ARX afxBool ArxGetGestureId(arxGesture ges, afxString* id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);
    AFX_ASSERT(id);
    *id = ges->id;
    return ges->id.len;
}

_ARX _arxDdiGes const _ARX_DDI_GES =
{
    .findMatCb = _ArxGesFindTransformCb,
    .findVecCb = _ArxGesFindVectorCb,
    .updMatCb = _ArxGesUpdateTransformsCb,
    .updVecCb = _ArxGesUpdateVectorsCb,
};

_ARX afxError _ArxGesDtorCb(arxGesture ges)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);

    if (ges->pivots)
        AfxDeallocate((void**)&ges->pivots, AfxHere());

    if (ges->pivotLodError)
        AfxDeallocate((void**)&ges->pivotLodError, AfxHere());

    if (ges->pivotCurve)
    {
        for (afxUnit i = 0; i < ges->pivotCnt; i++)
        {
            arxPivotalGesture* t = &ges->pivotCurve[i];
            arxCurve curves[3] = { t->translation, t->transmission, t->transmutation};
            AfxDisposeObjects(3, curves);
        }
        AfxDeallocate((void**)&ges->pivotCurve, AfxHere());
    }

    if (ges->vectors)
        AfxDeallocate((void**)&ges->vectors, AfxHere());

    if (ges->vecCurve)
    {
        for (afxUnit i = 0; i < ges->vecCnt; i++)
        {
            arxVectorialGesture* v = &ges->vecCurve[i];
            AfxDisposeObjects(1, &v->value);
        }
        AfxDeallocate((void**)&ges->vecCurve, AfxHere());
    }

    return err;
}

_ARX afxError _ArxGesCtorCb(arxGesture ges, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_GES, 1, &ges);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxGestureBlueprint const* gesb = args[1];
    gesb += invokeNo;

    ges->ddi = &_ARX_DDI_GES;
    ges->idd = NIL;
    ges->udd = gesb->udd;
    ges->tag = gesb->tag;

    if (!AfxCatalogStrings(_ArxScioGetAnimUrnStringBase(scio), 1, &gesb->id.s, &ges->id))
        AfxThrowError();

    afxUnit vecCnt = gesb->vecCnt;
    afxUnit pivotCnt = gesb->pivotCnt;

    if (vecCnt)
    {
        AfxAllocate(vecCnt * sizeof(ges->vecCurve[0]), 0, AfxHere(), (void**)&ges->vecCurve);
        AfxAllocate(vecCnt * sizeof(ges->vectors[0]), 0, AfxHere(), (void**)&ges->vectors);
    }

    if (pivotCnt)
    {
        AfxAllocate(pivotCnt * sizeof(ges->pivotCurve[0]), 0, AfxHere(), (void**)&ges->pivotCurve);
        AfxAllocate(pivotCnt * sizeof(ges->pivots[0]), 0, AfxHere(), (void**)&ges->pivots);

        if (gesb->incPivotLodError)
        {
            AfxAllocate(pivotCnt * sizeof(ges->pivotLodError[0]), 0, AfxHere(), (void**)&ges->pivotLodError);
        }
    }

    afxString s;

    if (!ges->vectors)
    {
        for (afxUnit i = 0; i < vecCnt; i++)
            AfxResetStrings(1, &ges->vectors[i]);
    }
    else if (!AfxCatalogStrings(_ArxScioGetAnimUrnStringBase(scio), vecCnt, gesb->vectors, ges->vectors)) // AfxRegisterMorphTags is temp
        AfxThrowError();

    if ((ges->vecCnt = vecCnt))
    {
        for (afxUnit i = 0; i < vecCnt; i++)
        {
            arxVectorialGesture* t = &ges->vecCurve[i];
        }

        afxString id;
        arxCurve value;
        afxUnit key;
        afxInt dim;
        //ArxUpdateGestureVectors(ges, 0, vecCnt, &id, &key, &dim, &value, 0);
    }

    if (!ges->pivots)
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
            AfxResetStrings(1, &ges->pivots[i]);
    }
    else if (!AfxCatalogStrings(_ArxScioGetAnimUrnStringBase(scio), pivotCnt, gesb->pivots, ges->pivots))
        AfxThrowError();

    if ((ges->pivotCnt = pivotCnt))
    {
        for (afxUnit i = 0; i < pivotCnt; i++)
        {
            arxPivotalGesture* t = &ges->pivotCurve[i];
            
        }

        afxString id;
        afxFlags flag;
        arxCurve orient, scale, pos;
        //ArxUpdateGestureTransforms(ges, 0, xformMotCnt, &id, &flag, &orient, &pos, &scale, 0);
    }

    if (ges->pivotLodError)
    {
        if (gesb->pivotLodError) for (afxUnit i = 0; i < pivotCnt; i++)
            ges->pivotLodError[i] = gesb->pivotLodError[i];
        else for (afxUnit i = 0; i < pivotCnt; i++)
            ges->pivotLodError[i] = 1.9140881e38;
    }

    AfxCopyTransform(&ges->displacement, &gesb->displacement);

    ges->flags = 2;
    ges->periodicLoop = 0;
    ges->root = NIL;
    AfxV3dZero(ges->loopTranslation);

#if !0
    // echo

    ArxGetGestureId(ges, &s);
    AfxReportMessage("Motion <%.*s> assembled. %p\n    Listing %u transform circuits:", AfxPushString(&s), ges, ges->pivotCnt);

    for (afxUnit i = 0; i < ges->pivotCnt; i++)
    {
        s = ges->pivots[i];
        AfxReportMessage("\t%u <%.*s> %x", i, AfxPushString(&s), ges->pivotCurve[i].flags);
    }
#endif

    return err;
}

_ARX afxClassConfig const _ARX_GES_CLASS_CONFIG =
{
    .fcc = afxFcc_GES,
    .name = "Motion",
    .desc = "Full Motion Gesture",
    .fixedSiz = sizeof(AFX_OBJECT(arxGesture)),
    .ctor = (void*)_ArxGesCtorCb,
    .dtor = (void*)_ArxGesDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAssembleGestures(arxScenario scio, afxUnit cnt, arxGestureBlueprint const blueprints[], arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetGesClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_GES);

    if (AfxAcquireObjects(cls, 1, (afxObject*)gestures, (void const*[]) { scio, blueprints }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_GES, cnt, gestures);

    return err;
}
