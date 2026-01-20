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
#define _ARX_CURVE_C
#define _ARX_SIMULATION_C
#define _ARX_GESTURE_C
#define _ARX_ANIMATION_C
#define _ARX_SKELETON_C
#define _ARX_BODY_C
#define _ARX_MODEL_C
#define _ARX_PULLEY_C
#include "../scene/arxIcd.h"

ARX afxError ArxWriteMappedStrings(afxStream out, afxString* sdb, afxUnit cnt, afxString const strings[]);
ARX afxError ArxReadMappedStrings(afxStream in, afxString const* sdb, afxUnit cnt, afxString strings[]);

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(FMA_CUR_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED GEOMETRIC CURVE HEADER
{
    afxUnit16       flags;
    afxUnit8        fmt;
    afxUnit8        dimens;
    afxUnit32       degree;
    afxUnit32       knotCnt;
    afxUnit32       ctrlCnt;
    afxUnit32       knotDirOffset;
    afxUnit32       ctrlDirOffset;
};

AFX_DEFINE_STRUCT(FMA_MOT_VEC_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED VECTORIAL MOTION HEADER
{
    afxUnit32       seqKey;
    afxInt32        dimension;
    afxUnit32       valueCurIdx;
};

AFX_DEFINE_STRUCT(FMA_MOT_PVT_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED 4D PIVOTAL MOTION HEADER
{
    afxUnit32       flags;
    afxUnit32       transmissionCurIdx;
    afxUnit32       translationCurIdx;
    afxUnit32       transmutationCurIdx;
};

AFX_DEFINE_STRUCT(FMA_MOT_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED MOTION HEADER
{
    afxUnit32       flags;
    afxUnit32       totalCurCnt;
    afxUnit32       curDir;
    afxUnit32       pvtCnt; // circuit count
    afxUnit32       pvtDir; // circuit id compatible with skeleton joints.
    afxUnit32       pvtInfoDir;
    afxUnit32       pvtLodErrDir;
    afxUnit32       vecCnt;
    afxUnit32       vecDir;
    afxUnit32       vecInfoDir;
    afxTransform    displacement;
    afxUnit32       rootMotPvtFlags;
    afxUnit32       rootTransmissionCurIdx;
    afxUnit32       rootTranslationCurIdx;
    afxUnit32       rootTransmutationCurIdx;
    afxV3d          loopTranslation;
    afxReal32       periodicLoopRadius;
    afxReal32       periodicLoop_dAngle;
    afxReal32       periodicLoop_dZ;
    afxV3d          periodicLoopBasisX;
    afxV3d          periodicLoopBasisY;
    afxV3d          periodicLoopAxis;
};

AFX_DEFINE_STRUCT(FMA_ANI_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED ANIMATION HEADER
{
    afxUnit8        fcc[4];
    afxUnit32       hdrSiz;
    afxUnit32       hdrVer;
    afxUnit32       segCnt;
    afxUnit32       segStart;
    afxUnit32       sdbSiz;
    afxUnit32       sdbStart;

    afxM3d          basis;
    afxV3d          origin;
    afxReal32       unitsPerMeter;

    afxReal32       dur;
    afxReal32       timeStep;
    afxReal32      oversampling;

    afxUnit32       flags;
    afxUnit32       motCnt;
    afxUnit32       motIdBase;
    afxUnit32       motInfoBase;
};

#pragma pack(pop)

_ARX afxError AfxSerializeCurves(afxStream out, afxUnit cnt, arxCurve curves[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_CUR, cnt, curves);
    afxUnit curIdx = 0;

    FMA_CUR_HDR* curHdrs;
    if (AfxAllocate(cnt * sizeof(curHdrs[0]), 0, AfxHere(), (void**)&curHdrs))
        AfxThrowError();;

    afxSize curHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    if (AfxAdvanceStream(out, cnt * sizeof(curHdrs[0])))
        AfxThrowError();

    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxCurve cur = curves[curIdx];

        FMA_CUR_HDR* hdr = &curHdrs[curIdx];
        hdr->fmt = cur->fmt;
        hdr->dimens = cur->dimens;
        hdr->flags = cur->flags;
        hdr->knotCnt = cur->knotCnt;
        hdr->ctrlCnt = cur->ctrlCnt;
        hdr->degree = cur->degree;

        if (cur->knotCnt)
        {
            hdr->knotDirOffset = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, cur->knotCnt * sizeof(afxReal), 0, ArxGetCurveKnots(cur)))
                AfxThrowError();
        }

        if (cur->ctrlCnt)
        {
            hdr->ctrlDirOffset = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, cur->ctrlCnt * sizeof(afxReal), 0, ArxGetCurveControls(cur)))
                AfxThrowError();
        }

    } while (!err && (++curIdx < cnt));

    // write out all headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    if (AfxSeekStream(out, curHdrOffBkp, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxWriteStream(out, cnt * sizeof(curHdrs[0]), 0, curHdrs))
        AfxThrowError();

    if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
        AfxThrowError();

    AfxDeallocate((void**)&curHdrs, AfxHere());

    return err;
}

_ARX afxError AfxSerializeMotions(afxStream out, afxString* sdb, afxUnit cnt, arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_GES, cnt, gestures);
    afxUnit gesIdx = 0;

    FMA_MOT_HDR* motHdrs;
    if (AfxAllocate(cnt * sizeof(motHdrs[0]), 0, AfxHere(), (void**)&motHdrs))
        AfxThrowError();

    afxSize motHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    if (AfxAdvanceStream(out, cnt * sizeof(motHdrs[0])))
        AfxThrowError();

    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxGesture ges = gestures[gesIdx];

        FMA_MOT_HDR motHdr = { 0 };

        motHdr.flags = ges->flags;
        motHdr.pvtCnt = ges->pivotCnt;
        motHdr.vecCnt = ges->vecCnt;
        motHdr.displacement = ges->displacement;

        AfxV3dCopy(motHdr.loopTranslation, ges->loopTranslation);

        if (ges->periodicLoop)
        {
            motHdr.periodicLoopRadius = ges->periodicLoop->radius;
            motHdr.periodicLoop_dAngle = ges->periodicLoop->dAngle;
            motHdr.periodicLoop_dZ = ges->periodicLoop->dZ;
            AfxV3dCopy(motHdr.periodicLoopBasisX, ges->periodicLoop->basisX);
            AfxV3dCopy(motHdr.periodicLoopBasisY, ges->periodicLoop->basisY);
            AfxV3dCopy(motHdr.periodicLoopAxis, ges->periodicLoop->axis);
        }

        if (ges->pivotCnt)
        {
            // write pivots' identifier strings.
            motHdr.pvtDir = AfxAskStreamPosn(out);
            if (ArxWriteMappedStrings(out, sdb, ges->pivotCnt, ges->pivots))
                AfxThrowError();
        }

        if (ges->vecCnt)
        {
            // write vectors' identifier strings.
            motHdr.vecDir = AfxAskStreamPosn(out);
            if (ArxWriteMappedStrings(out, sdb, ges->vecCnt, ges->vectors))
                AfxThrowError();
        }

        // make room for and prepare pivots and vectors

        afxUnit curIdx = 0;
        afxUnit totalCurCnt = (ges->pivotCnt * 3) + (ges->vecCnt * 1) + (ges->root ? 3 : 0);
        arxCurve* curves = NIL;
        FMA_MOT_PVT_HDR* pvtHdrs = NIL;
        FMA_MOT_VEC_HDR* vecHdrs = NIL;

        if (totalCurCnt)
        {
            if (AfxAllocate(totalCurCnt * sizeof(curves[0]), 0, AfxHere(), (void**)&curves))
                AfxThrowError();
        }

        if (ges->pivotCnt)
        {
            if (AfxAllocate(ges->pivotCnt * sizeof(pvtHdrs[0]), 0, AfxHere(), (void**)&pvtHdrs))
                AfxThrowError();

            // prepare pivots
            for (afxUnit i = 0; i < ges->pivotCnt; i++)
            {
                FMA_MOT_PVT_HDR* hdr = &pvtHdrs[i];
                hdr->flags = ges->pivotCurve[i].flags;

                hdr->transmissionCurIdx = curIdx++;
                curves[hdr->transmissionCurIdx] = ges->pivotCurve[i].transmission;
                hdr->translationCurIdx = curIdx++;
                curves[hdr->translationCurIdx] = ges->pivotCurve[i].translation;
                hdr->transmutationCurIdx = curIdx++;
                curves[hdr->transmutationCurIdx] = ges->pivotCurve[i].transmutation;
            }
        }

        if (ges->vecCnt)
        {
            if (AfxAllocate(ges->vecCnt * sizeof(vecHdrs[0]), 0, AfxHere(), (void**)&vecHdrs))
                AfxThrowError();

            // prepare vectors
            for (afxUnit i = 0; i < ges->vecCnt; i++)
            {
                FMA_MOT_VEC_HDR* hdr = &vecHdrs[i];
                hdr->seqKey = ges->vecCurve[i].seqKey;
                hdr->dimension = ges->vecCurve[i].dimension;
                hdr->valueCurIdx = curIdx++;
                curves[hdr->valueCurIdx] = ges->vecCurve[i].value;
            }
        }

        // prepare root curves
        if (ges->root)
        {
            motHdr.rootMotPvtFlags = ges->root->flags;
            motHdr.rootTransmissionCurIdx = curIdx++;
            curves[motHdr.rootTransmissionCurIdx] = ges->root->transmission;
            motHdr.rootTranslationCurIdx = curIdx++;
            curves[motHdr.rootTranslationCurIdx] = ges->root->translation;
            motHdr.rootTransmutationCurIdx = curIdx++;
            curves[motHdr.rootTransmutationCurIdx] = ges->root->transmutation;
        }

        // write all curves
        motHdr.curDir = AfxAskStreamPosn(out);
        AFX_ASSERT(curIdx == totalCurCnt);
        motHdr.totalCurCnt = curIdx;
        if (AfxSerializeCurves(out, motHdr.totalCurCnt, curves))
            AfxThrowError();

        AfxDeallocate((void**)&curves, AfxHere());

        // write pivots' headers

        if (ges->pivotCnt)
        {
            // write pivot LOD error

            if (ges->pivotLodError)
            {
                motHdr.pvtLodErrDir = AfxAskStreamPosn(out);

                if (AfxWriteStream(out, ges->pivotCnt * sizeof(ges->pivotLodError[0]), 0, ges->pivotLodError))
                    AfxThrowError();
            }

            // go back, write pivot headers and return.

            // make room
            motHdr.pvtInfoDir = AfxAskStreamPosn(out);
            //AfxAdvanceStream(out, sizeof(pvtHdrs[0]) * ges->pivotCnt);

            afxSize bkpPos = AfxAskStreamPosn(out);
            //AfxSeekStream(out, motHdr.pvtInfoDir, afxSeekOrigin_BEGIN);
            AfxWriteStream(out, sizeof(pvtHdrs[0]) * ges->pivotCnt, 0, pvtHdrs);
            //AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

            AfxDeallocate((void**)&pvtHdrs, AfxHere());
        }

        // write vectors' headers

        if (ges->vecCnt)
        {
            // make room
            motHdr.vecInfoDir = AfxAskStreamPosn(out);
            if (AfxAdvanceStream(out, sizeof(vecHdrs[0]) * ges->vecCnt))
                AfxThrowError();

            afxUnit bkpPos = AfxAskStreamPosn(out);
            if (AfxSeekStream(out, motHdr.vecInfoDir, afxSeekOrigin_BEGIN))
                AfxThrowError();

            if (AfxWriteStream(out, sizeof(vecHdrs[0]) * ges->vecCnt, 0, vecHdrs))
                AfxThrowError();

            if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
                AfxThrowError();

            AfxDeallocate((void**)&vecHdrs, AfxHere());
        }

        motHdrs[gesIdx] = motHdr;

    } while (!err && (++gesIdx < cnt));

    // write out all gestures' headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    if (AfxSeekStream(out, motHdrOffBkp, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxWriteStream(out, cnt * sizeof(motHdrs[0]), 0, motHdrs))
        AfxThrowError();

    AFX_ASSERT(endOfHdrs == AfxAskStreamPosn(out));

    if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
        AfxThrowError();

    AfxDeallocate((void**)&motHdrs, AfxHere());

    return err;
}

_ARX afxError AfxSerializeAnimation(arxAnimation ani, afxStream out)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_ANI, 1, &ani);
    afxUnit mdlIdx = 0;

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    FMA_ANI_HDR aniHdr = { 0 };
    aniHdr.fcc[0] = 'k';
    aniHdr.fcc[1] = '4';
    aniHdr.fcc[2] = 'd';
    aniHdr.fcc[3] = '\0';
    aniHdr.hdrSiz = sizeof(aniHdr) - sizeof(urdMark);
    AfxM3dReset(aniHdr.basis);
    AfxV3dZero(aniHdr.origin);
    aniHdr.unitsPerMeter = 1.f;
    aniHdr.flags = ani->flags;
    aniHdr.motCnt = ani->gesSlotCnt;
    aniHdr.dur = ani->dur;
    aniHdr.oversampling = ani->oversampling;
    aniHdr.timeStep = ani->timeStep;
    
    afxSize hdrPosBkp = AfxAskStreamPosn(out);
    if (AfxAdvanceStream(out, sizeof(aniHdr)))
        AfxThrowError();

    aniHdr.motIdBase = AfxAskStreamPosn(out);

    afxUnit totalCurCnt = 0;
    afxUnit uniqueMotCnt = 0;
    arxGesture gestures[256];

    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        arxGesture ges = ani->gesSlots[i].ges;

        if (ges)
        {
            gestures[uniqueMotCnt++] = ges;
            ArxWriteMappedStrings(out, &sdb.s, 1, &ges->id);
        }
    }

    aniHdr.motInfoBase = AfxAskStreamPosn(out);

    if (AfxSerializeMotions(out, &sdb.s, uniqueMotCnt, gestures))
        AfxThrowError();

    {
        // write string database
        aniHdr.sdbStart = AfxAskStreamPosn(out);
        aniHdr.sdbSiz = sdb.s.len;

        if (AfxWriteStream(out, aniHdr.sdbSiz, 0, sdb.s.start))
        {
            AfxThrowError();
            return err;
        }
    }

    // write animation header
    aniHdr.hdrSiz = AfxAskStreamPosn(out) - hdrPosBkp - sizeof(urdMark);

    if (AfxWriteStreamAt(out, hdrPosBkp, sizeof(aniHdr), 0, &aniHdr))
        AfxThrowError();

    if (AfxAdvanceStream(out, aniHdr.hdrSiz))
        AfxThrowError();

    return err;
}

_ARX afxError ArxArchiveAnimation(arxAnimation ani, afxUri const* uri)
{
    afxError err = { 0 };
    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_WRITEABLE;
    info.usage = afxIoUsage_FILE;
    
    if (AfxAcquireStream(1, &info, &iob))
        AfxThrowError();

    if (AfxReopenFile(iob, uri, afxFileFlag_W))
        AfxThrowError();
    
    if (AfxSerializeAnimation(ani, iob))
        AfxThrowError();

    AfxDisposeObjects(1, &iob);

    return err;
}

_ARX afxError ArxUploadAnimation(arxScenario scio, afxString const* urn, afxStream in, arxAnimation* animation)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(animation);

    // Qwadro doesn't use afxChunkID_STRUCT

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

#pragma push(pack, 1)
    FMA_ANI_HDR aniHdr;
#pragma pop(pack)

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxString strings[256];
    arxGesture gestures[256];
    arxModelFlags rigFlags[256];

    if (AfxReadStream(in, sizeof(aniHdr), 0, &aniHdr))
        AfxThrowError();

    afxSize posBkp = AfxAskStreamPosn(in);
    if (AfxSeekStream(in, aniHdr.sdbStart, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxReadString(&sdb.s, in, aniHdr.sdbSiz))
        AfxThrowError();

    arxAnimation ani;
    arxAnimationBlueprint anib = { 0 };
    anib.dur = aniHdr.dur;
    anib.timeStep = aniHdr.timeStep;
    anib.oversampling = aniHdr.oversampling;
    anib.gesSlotCnt = aniHdr.motCnt;
    AfxMakeString32(&anib.id, urn);
    if (ArxAssembleAnimations(scio, 1, &anib, &ani))
        AfxThrowError();

    if (AfxSeekStream(in, aniHdr.motIdBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (ArxReadMappedStrings(in, &sdb.s, aniHdr.motCnt, strings))
        AfxThrowError();

    if (AfxSeekStream(in, aniHdr.motInfoBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    FMA_MOT_HDR* motHdrs = AfxRequestArena(&arena, sizeof(motHdrs[0]), aniHdr.motCnt, NIL, 0);

    if (AfxReadStream(in, aniHdr.motCnt * sizeof(motHdrs[0]), 0, motHdrs))
        AfxThrowError();

    arxGestureBlueprint* motbs = AfxRequestArena(&arena, sizeof(motbs[0]), aniHdr.motCnt, NIL, 1);

    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        FMA_MOT_HDR* motHdr = &motHdrs[i];
        arxGestureBlueprint* gesb = &motbs[i];

        afxString* pivots = AfxRequestArena(&arena, sizeof(pivots[0]), motHdr->pvtCnt, NIL, 0);;
        afxString* vectors = motHdr->vecCnt ? AfxRequestArena(&arena, sizeof(vectors[0]), motHdr->vecCnt, NIL, 0) : NIL;

        gesb->displacement = motHdr->displacement;
        gesb->incPivotLodError = !!motHdr->pvtLodErrDir;
        gesb->pivotCnt = motHdr->pvtCnt;
        gesb->vecCnt = motHdr->vecCnt;
        gesb->pivots = pivots;
        gesb->vectors = vectors;
        AfxMakeString32(&gesb->id, &strings[i]);

        if (gesb->pivotCnt)
        {
            AfxSeekStream(in, motHdr->pvtDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, &sdb.s, motHdr->pvtCnt, pivots);
        }

        if (gesb->vecCnt)
        {
            AfxSeekStream(in, motHdr->vecDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, &sdb.s, motHdr->vecCnt, vectors);
        }
    }

    if (ArxAssembleGestures(scio, aniHdr.motCnt, motbs, gestures))
        AfxThrowError();
    
    for (afxUnit i = 0; i < aniHdr.motCnt; i++)
    {
        FMA_MOT_HDR* motHdr = &motHdrs[i];

        AfxV3dCopy(gestures[i]->loopTranslation, motHdr->loopTranslation);

        FMA_CUR_HDR* curHdrs = AfxRequestArena(&arena, sizeof(curHdrs[0]), motHdr->totalCurCnt, NIL, 0);
        arxCurveInfo* curis = AfxRequestArena(&arena, sizeof(curis[0]), motHdr->totalCurCnt, NIL, 0);
        arxCurve curvesStatic[256];
        arxCurve* curves = curvesStatic;// AfxRequestArena(&arena, motHdr->totalCurCnt * sizeof(curves[0]));
        if (AfxReadStreamAt(in, motHdr->curDir, motHdr->totalCurCnt * sizeof(curHdrs[0]), 0, curHdrs))
            AfxThrowError();
        
        for (afxUnit j = 0; j < motHdr->totalCurCnt; j++)
        {
            arxCurveInfo* curi = &curis[j];
            *curi = (arxCurveInfo) { 0 };
            FMA_CUR_HDR* curHdr = &curHdrs[j];

            curi->knotCnt = curHdr->knotCnt;
            curi->degree = curHdr->degree;
            curi->fmt = curHdr->fmt;
            curi->dimens = curHdr->dimens;

            if (curHdr->ctrlCnt)
            {
                afxReal* ctrls = AfxRequestArena(&arena, sizeof(ctrls[0]), curHdr->ctrlCnt, NIL, 0);
                if (AfxReadStreamAt(in, curHdr->ctrlDirOffset, curHdr->ctrlCnt * sizeof(ctrls[0]), 0, ctrls))
                    AfxThrowError();

                curi->ctrls = ctrls;
            }

            if (curHdr->knotCnt)
            {
                afxReal* knots = AfxRequestArena(&arena, sizeof(knots[0]), curHdr->knotCnt, NIL, 0);
                if (AfxReadStreamAt(in, curHdr->knotDirOffset, curHdr->knotCnt * sizeof(knots[0]), 0, knots))
                    AfxThrowError();

                curi->knots = knots;
            }
        }

        if (ArxAcquireCurves(scio, motHdr->totalCurCnt, curis, curves))
            AfxThrowError();
#if 0
        for (afxUnit j = 0; j < motHdr->totalCurCnt; j++)
        {
            K4D_CUR_HDR* curHdr = &curHdrs[j];

            if (curHdr->ctrlCnt)
            {
                afxReal* ctrls = AfxRequestArena(&arena, curHdr->ctrlCnt * sizeof(ctrls[0]));
                AfxSeekStream(in, curHdr->ctrlDirOffset, afxSeekOrigin_BEGIN);
                AfxReadStream(in, curHdr->ctrlCnt * sizeof(ctrls[0]), 0, ctrls);
            }

            if (curHdr->knotCnt)
            {
                afxReal* knots = AfxRequestArena(&arena, curHdr->knotCnt * sizeof(knots[0]));
                AfxSeekStream(in, curHdr->knotDirOffset, afxSeekOrigin_BEGIN);
                AfxReadStream(in, curHdr->knotCnt * sizeof(knots[0]), 0, knots);
            }
        }
#endif
        FMA_MOT_PVT_HDR* pvtHdrs = AfxRequestArena(&arena, sizeof(pvtHdrs[0]), motHdr->pvtCnt, NIL, 0);
        if (AfxReadStreamAt(in, motHdr->pvtInfoDir, motHdr->pvtCnt * sizeof(pvtHdrs[0]), 0, pvtHdrs))
            AfxThrowError();

        for (afxUnit j = 0; j < motHdr->pvtCnt; j++)
        {
            FMA_MOT_PVT_HDR* pvtHdr = &pvtHdrs[j];

            arxPivotalGesture mt = { 0 };
            mt.flags = pvtHdr->flags;
            mt.translation = curves[pvtHdr->translationCurIdx];
            mt.transmission = curves[pvtHdr->transmissionCurIdx];
            mt.transmutation = curves[pvtHdr->transmutationCurIdx];
            ArxUpdateGestureTransforms(gestures[i], j, 1, &mt, sizeof(mt));
        }

        if (motHdr->pvtLodErrDir)
        {
            if (AfxReadStreamAt(in, motHdr->pvtLodErrDir, motHdr->pvtCnt * sizeof(gestures[i]->pivotLodError[0]), 0, gestures[i]->pivotLodError))
                AfxThrowError();
        }

        if (motHdr->vecCnt)
        {
            FMA_MOT_VEC_HDR* vecHdrs = AfxRequestArena(&arena, sizeof(vecHdrs[0]), motHdr->vecCnt, NIL, 0);
            if (AfxReadStreamAt(in, motHdr->vecInfoDir, motHdr->vecCnt * sizeof(vecHdrs[0]), 0, vecHdrs))
                AfxThrowError();

            for (afxUnit j = 0; j < motHdr->vecCnt; j++)
            {
                FMA_MOT_VEC_HDR* vecHdr = &vecHdrs[j];

                arxVectorialGesture mv = { 0 };
                mv.seqKey = vecHdr->seqKey;
                mv.dimension = vecHdr->dimension;
                mv.value = curves[vecHdr->valueCurIdx];
                ArxUpdateGestureVectors(gestures[i], j, 1, &mv, sizeof(mv));
            }
        }

        //AfxDisposeObjects(motHdr->totalCurCnt, curves);

        if (ArxRelinkGestures(ani, i, 1, &gestures[i]))
            AfxThrowError();
    }

    AfxDismantleArena(&arena);

    *animation = ani;

    return err;
}

_ARX afxError ArxLoadAnimation(arxScenario scio, afxString const* urn, afxUri const* uri, arxAnimation* animation)
{
    afxError err = { 0 };
    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_READABLE;
    info.usage = afxIoUsage_FILE;
    if (AfxAcquireStream(1, &info, &iob))
        AfxThrowError();

    if (AfxReopenFile(iob, uri, afxFileFlag_R))
        AfxThrowError();

    if (ArxUploadAnimation(scio, urn, iob, animation))
        AfxThrowError();

    AfxDisposeObjects(1, &iob);

    return err;
}
