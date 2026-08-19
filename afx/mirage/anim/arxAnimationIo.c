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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
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

AFX_DEFINE_STRUCT(FMA_GES_VEC_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED VECTORIAL GESION HEADER
{
    afxUnit32       seqKey;
    afxInt32        dimension;
    afxUnit32       valueCurIdx;
};

AFX_DEFINE_STRUCT(FMA_GES_PVT_HDR)
// SIGMA/ENGITECH, FULL MOTION ANIMATION, SERIALIZED 4D PIVOTAL GESION HEADER
{
    afxUnit32       flags;
    afxUnit32       transmissionCurIdx;
    afxUnit32       translationCurIdx;
    afxUnit32       transmutationCurIdx;
};

AFX_DEFINE_STRUCT(FMA_GES_HDR)
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
    afxUnit32       rootGesPvtFlags;
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
// SIGMA/ENGITECH, FULL GESION ANIMATION, SERIALIZED ANIMATION HEADER
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
    afxUnit32       gesCnt;
    afxUnit32       gesIdBase;
    afxUnit32       gesInfoBase;
};

#pragma pack(pop)

_ARX afxError AfxSerializeCurves(afxStream out, afxUnit cnt, arxCurve curves[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_CUR, cnt, curves);
    afxUnit curIdx = 0;

    FMA_CUR_HDR* curHdrs;
    if (AfxAllocate(AfxHere(), cnt * sizeof(curHdrs[0]), 0, (void**)&curHdrs))
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

    AfxDeallocate(AfxHere(), (void**)&curHdrs);

    return err;
}

_ARX afxError AfxSerializeGestures(afxStream out, afxString* sdb, afxUnit cnt, arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_GES, cnt, gestures);
    afxUnit gesIdx = 0;

    FMA_GES_HDR* gesHdrs;
    if (AfxAllocate(AfxHere(), cnt * sizeof(gesHdrs[0]), 0, (void**)&gesHdrs))
        AfxThrowError();

    afxSize gesHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    if (AfxAdvanceStream(out, cnt * sizeof(gesHdrs[0])))
        AfxThrowError();

    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxGesture ges = gestures[gesIdx];

        FMA_GES_HDR gesHdr = { 0 };

        gesHdr.flags = ges->flags;
        gesHdr.pvtCnt = ges->pivotCnt;
        gesHdr.vecCnt = ges->vecCnt;
        gesHdr.displacement = ges->displacement;

        AfxV3dCopy(gesHdr.loopTranslation, ges->loopTranslation);

        if (ges->periodicLoop)
        {
            gesHdr.periodicLoopRadius = ges->periodicLoop->radius;
            gesHdr.periodicLoop_dAngle = ges->periodicLoop->dAngle;
            gesHdr.periodicLoop_dZ = ges->periodicLoop->dZ;
            AfxV3dCopy(gesHdr.periodicLoopBasisX, ges->periodicLoop->basisX);
            AfxV3dCopy(gesHdr.periodicLoopBasisY, ges->periodicLoop->basisY);
            AfxV3dCopy(gesHdr.periodicLoopAxis, ges->periodicLoop->axis);
        }

        if (ges->pivotCnt)
        {
            // write pivots' identifier strings.
            gesHdr.pvtDir = AfxAskStreamPosn(out);
            if (ArxWriteMappedStrings(out, sdb, ges->pivotCnt, ges->pivots))
                AfxThrowError();
        }

        if (ges->vecCnt)
        {
            // write vectors' identifier strings.
            gesHdr.vecDir = AfxAskStreamPosn(out);
            if (ArxWriteMappedStrings(out, sdb, ges->vecCnt, ges->vectors))
                AfxThrowError();
        }

        // make room for and prepare pivots and vectors

        afxUnit curIdx = 0;
        afxUnit totalCurCnt = (ges->pivotCnt * 3) + (ges->vecCnt * 1) + (ges->root ? 3 : 0);
        arxCurve* curves = NIL;
        FMA_GES_PVT_HDR* pvtHdrs = NIL;
        FMA_GES_VEC_HDR* vecHdrs = NIL;

        if (totalCurCnt)
        {
            if (AfxAllocate(AfxHere(), totalCurCnt * sizeof(curves[0]), 0, (void**)&curves))
                AfxThrowError();
        }

        if (ges->pivotCnt)
        {
            if (AfxAllocate(AfxHere(), ges->pivotCnt * sizeof(pvtHdrs[0]), 0, (void**)&pvtHdrs))
                AfxThrowError();

            // prepare pivots
            for (afxUnit i = 0; i < ges->pivotCnt; i++)
            {
                FMA_GES_PVT_HDR* hdr = &pvtHdrs[i];
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
            if (AfxAllocate(AfxHere(), ges->vecCnt * sizeof(vecHdrs[0]), 0, (void**)&vecHdrs))
                AfxThrowError();

            // prepare vectors
            for (afxUnit i = 0; i < ges->vecCnt; i++)
            {
                FMA_GES_VEC_HDR* hdr = &vecHdrs[i];
                hdr->seqKey = ges->vecCurve[i].seqKey;
                hdr->dimension = ges->vecCurve[i].dimension;
                hdr->valueCurIdx = curIdx++;
                curves[hdr->valueCurIdx] = ges->vecCurve[i].value;
            }
        }

        // prepare root curves
        if (ges->root)
        {
            gesHdr.rootGesPvtFlags = ges->root->flags;
            gesHdr.rootTransmissionCurIdx = curIdx++;
            curves[gesHdr.rootTransmissionCurIdx] = ges->root->transmission;
            gesHdr.rootTranslationCurIdx = curIdx++;
            curves[gesHdr.rootTranslationCurIdx] = ges->root->translation;
            gesHdr.rootTransmutationCurIdx = curIdx++;
            curves[gesHdr.rootTransmutationCurIdx] = ges->root->transmutation;
        }

        // write all curves
        gesHdr.curDir = AfxAskStreamPosn(out);
        AFX_ASSERT(curIdx == totalCurCnt);
        gesHdr.totalCurCnt = curIdx;
        if (AfxSerializeCurves(out, gesHdr.totalCurCnt, curves))
            AfxThrowError();

        AfxDeallocate(AfxHere(), (void**)&curves);

        // write pivots' headers

        if (ges->pivotCnt)
        {
            // write pivot LOD error

            if (ges->pivotLodError)
            {
                gesHdr.pvtLodErrDir = AfxAskStreamPosn(out);

                if (AfxWriteStream(out, ges->pivotCnt * sizeof(ges->pivotLodError[0]), 0, ges->pivotLodError))
                    AfxThrowError();
            }

            // go back, write pivot headers and return.

            // make room
            gesHdr.pvtInfoDir = AfxAskStreamPosn(out);
            //AfxAdvanceStream(out, sizeof(pvtHdrs[0]) * ges->pivotCnt);

            afxSize bkpPos = AfxAskStreamPosn(out);
            //AfxSeekStream(out, gesHdr.pvtInfoDir, afxSeekOrigin_BEGIN);
            AfxWriteStream(out, sizeof(pvtHdrs[0]) * ges->pivotCnt, 0, pvtHdrs);
            //AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

            AfxDeallocate(AfxHere(), (void**)&pvtHdrs);
        }

        // write vectors' headers

        if (ges->vecCnt)
        {
            // make room
            gesHdr.vecInfoDir = AfxAskStreamPosn(out);
            if (AfxAdvanceStream(out, sizeof(vecHdrs[0]) * ges->vecCnt))
                AfxThrowError();

            afxUnit bkpPos = AfxAskStreamPosn(out);
            if (AfxSeekStream(out, gesHdr.vecInfoDir, afxSeekOrigin_BEGIN))
                AfxThrowError();

            if (AfxWriteStream(out, sizeof(vecHdrs[0]) * ges->vecCnt, 0, vecHdrs))
                AfxThrowError();

            if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
                AfxThrowError();

            AfxDeallocate(AfxHere(), (void**)&vecHdrs);
        }

        gesHdrs[gesIdx] = gesHdr;

    } while (!err && (++gesIdx < cnt));

    // write out all gestures' headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    if (AfxSeekStream(out, gesHdrOffBkp, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxWriteStream(out, cnt * sizeof(gesHdrs[0]), 0, gesHdrs))
        AfxThrowError();

    AFX_ASSERT(endOfHdrs == AfxAskStreamPosn(out));

    if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
        AfxThrowError();

    AfxDeallocate(AfxHere(), (void**)&gesHdrs);

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
    aniHdr.gesCnt = ani->gesSlotCnt;
    aniHdr.dur = ani->dur;
    aniHdr.oversampling = ani->oversampling;
    aniHdr.timeStep = ani->timeStep;
    
    afxSize hdrPosBkp = AfxAskStreamPosn(out);
    if (AfxAdvanceStream(out, sizeof(aniHdr)))
        AfxThrowError();

    aniHdr.gesIdBase = AfxAskStreamPosn(out);

    afxUnit totalCurCnt = 0;
    afxUnit uniqueGesCnt = 0;
    arxGesture gestures[256];

    for (afxUnit i = 0; i < aniHdr.gesCnt; i++)
    {
        arxGesture ges = ani->gesSlots[i].ges;

        if (ges)
        {
            gestures[uniqueGesCnt++] = ges;
            ArxWriteMappedStrings(out, &sdb.s, 1, &ges->id);
        }
    }

    aniHdr.gesInfoBase = AfxAskStreamPosn(out);

    if (AfxSerializeGestures(out, &sdb.s, uniqueGesCnt, gestures))
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

_ARX afxError ArxUploadCurves(arxScenario scio, afxArena* arena, afxString const* sdb, afxStream in, afxUnit curCnt, FMA_CUR_HDR const curHdrs[], afxString const urns[], arxCurve curves[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(curves);

    arxCurveInfo* curis = AfxRequestArena(arena, sizeof(curis[0]), curCnt, NIL, 0);

    for (afxUnit j = 0; j < curCnt; j++)
    {
        arxCurveInfo* curi = &curis[j];
        *curi = (arxCurveInfo) { 0 };
        FMA_CUR_HDR const* curHdr = &curHdrs[j];

        curi->knotCnt = curHdr->knotCnt;
        curi->degree = curHdr->degree;
        curi->fmt = curHdr->fmt;
        curi->dimens = curHdr->dimens;

        if (curHdr->ctrlCnt)
        {
            afxReal* ctrls = AfxRequestArena(arena, sizeof(ctrls[0]), curHdr->ctrlCnt, NIL, 0);
            if (AfxReadStreamAt(in, curHdr->ctrlDirOffset, curHdr->ctrlCnt * sizeof(ctrls[0]), 0, ctrls))
                AfxThrowError();

            curi->ctrls = ctrls;
        }

        if (curHdr->knotCnt)
        {
            afxReal* knots = AfxRequestArena(arena, sizeof(knots[0]), curHdr->knotCnt, NIL, 0);
            if (AfxReadStreamAt(in, curHdr->knotDirOffset, curHdr->knotCnt * sizeof(knots[0]), 0, knots))
                AfxThrowError();

            curi->knots = knots;
        }
    }

    if (ArxAcquireCurves(scio, curCnt, curis, curves))
        AfxThrowError();

#if 0
    for (afxUnit j = 0; j < gesHdr->totalCurCnt; j++)
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

    AfxReclaimArena(arena, curis, sizeof(curis[0]) * curCnt);

    return err;
}

_ARX afxError ArxUploadGestures(arxScenario scio, afxArena* arena, afxString const* sdb, afxStream in, afxUnit gesCnt, FMA_GES_HDR const gesHdrs[], afxString const urns[], arxGesture gestures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(gestures);

    arxGestureBlueprint* gesbs = AfxRequestArena(arena, sizeof(gesbs[0]), gesCnt, NIL, 1);

    for (afxUnit i = 0; i < gesCnt; i++)
    {
        FMA_GES_HDR const* gesHdr = &gesHdrs[i];
        arxGestureBlueprint* gesb = &gesbs[i];

        afxString* pivots = AfxRequestArena(arena, sizeof(pivots[0]), gesHdr->pvtCnt, NIL, 0);;
        afxString* vectors = gesHdr->vecCnt ? AfxRequestArena(arena, sizeof(vectors[0]), gesHdr->vecCnt, NIL, 0) : NIL;

        gesb->displacement = gesHdr->displacement;
        gesb->incPivotLodError = !!gesHdr->pvtLodErrDir;
        gesb->pivotCnt = gesHdr->pvtCnt;
        gesb->vecCnt = gesHdr->vecCnt;
        gesb->pivots = pivots;
        gesb->vectors = vectors;
        AfxMakeString32(&gesb->id, &urns[i]);

        if (gesb->pivotCnt)
        {
            AfxSeekStream(in, gesHdr->pvtDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, sdb, gesHdr->pvtCnt, pivots);
        }

        if (gesb->vecCnt)
        {
            AfxSeekStream(in, gesHdr->vecDir, afxSeekOrigin_BEGIN);
            ArxReadMappedStrings(in, sdb, gesHdr->vecCnt, vectors);
        }
    }

    if (ArxAssembleGestures(scio, gesCnt, gesbs, gestures))
        AfxThrowError();

    AfxReclaimArena(arena, gesbs, sizeof(gesbs) * gesCnt);

    for (afxUnit i = 0; i < gesCnt; i++)
    {
        FMA_GES_HDR const* gesHdr = &gesHdrs[i];

        AfxV3dCopy(gestures[i]->loopTranslation, gesHdr->loopTranslation);

        FMA_CUR_HDR* curHdrs = AfxRequestArena(arena, sizeof(curHdrs[0]), gesHdr->totalCurCnt, NIL, 0);

        arxCurve* curves = AfxRequestArena(arena, sizeof(curves[0]), gesHdr->totalCurCnt, NIL, 0);

        if (AfxReadStreamAt(in, gesHdr->curDir, gesHdr->totalCurCnt * sizeof(curHdrs[0]), 0, curHdrs))
            AfxThrowError();

        if (ArxUploadCurves(scio, arena, sdb, in, gesHdr->totalCurCnt, curHdrs, NIL, curves))
            AfxThrowError();

        AfxReclaimArena(arena, curHdrs, sizeof(curHdrs[0]) * gesHdr->totalCurCnt);

        FMA_GES_PVT_HDR* pvtHdrs = AfxRequestArena(arena, sizeof(pvtHdrs[0]), gesHdr->pvtCnt, NIL, 0);
        if (AfxReadStreamAt(in, gesHdr->pvtInfoDir, gesHdr->pvtCnt * sizeof(pvtHdrs[0]), 0, pvtHdrs))
            AfxThrowError();

        for (afxUnit j = 0; j < gesHdr->pvtCnt; j++)
        {
            FMA_GES_PVT_HDR* pvtHdr = &pvtHdrs[j];

            arxPivotalGesture mt = { 0 };
            mt.flags = pvtHdr->flags;
            mt.translation = curves[pvtHdr->translationCurIdx];
            mt.transmission = curves[pvtHdr->transmissionCurIdx];
            mt.transmutation = curves[pvtHdr->transmutationCurIdx];
            ArxUpdateGestureTransforms(gestures[i], j, 1, &mt, sizeof(mt));
        }

        if (gesHdr->pvtLodErrDir)
        {
            if (AfxReadStreamAt(in, gesHdr->pvtLodErrDir, gesHdr->pvtCnt * sizeof(gestures[i]->pivotLodError[0]), 0, gestures[i]->pivotLodError))
                AfxThrowError();
        }

        if (gesHdr->vecCnt)
        {
            FMA_GES_VEC_HDR* vecHdrs = AfxRequestArena(arena, sizeof(vecHdrs[0]), gesHdr->vecCnt, NIL, 0);
            if (AfxReadStreamAt(in, gesHdr->vecInfoDir, gesHdr->vecCnt * sizeof(vecHdrs[0]), 0, vecHdrs))
                AfxThrowError();

            for (afxUnit j = 0; j < gesHdr->vecCnt; j++)
            {
                FMA_GES_VEC_HDR* vecHdr = &vecHdrs[j];

                arxVectorialGesture mv = { 0 };
                mv.seqKey = vecHdr->seqKey;
                mv.dimension = vecHdr->dimension;
                mv.value = curves[vecHdr->valueCurIdx];
                ArxUpdateGestureVectors(gestures[i], j, 1, &mv, sizeof(mv));
            }
        }

        //AfxDisposeObjects(gesHdr->totalCurCnt, curves);

    }

    return err;
}

_ARX afxError ArxUploadAnimation(arxScenario scio, afxStream in, FMA_ANI_HDR const* aniHdr, afxString const* urn, arxAnimation* animation)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(animation);

    // Qwadro doesn't use afxChunkID_STRUCT

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxString strings[256];
    arxGesture gestures[256];
    arxModelFlags rigFlags[256];

    afxSize posBkp = AfxAskStreamPosn(in);
    if (AfxSeekStream(in, aniHdr->sdbStart, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxReadString(&sdb.s, in, aniHdr->sdbSiz))
        AfxThrowError();

    if (AfxSeekStream(in, aniHdr->gesIdBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (ArxReadMappedStrings(in, &sdb.s, aniHdr->gesCnt, strings))
        AfxThrowError();

    if (AfxSeekStream(in, aniHdr->gesInfoBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    FMA_GES_HDR* gesHdrs = AfxRequestArena(&arena, sizeof(gesHdrs[0]), aniHdr->gesCnt, NIL, 0);

    if (AfxReadStream(in, aniHdr->gesCnt * sizeof(gesHdrs[0]), 0, gesHdrs))
        AfxThrowError();

    if (ArxUploadGestures(scio, &arena, &sdb.s, in, aniHdr->gesCnt, gesHdrs, strings, gestures))
        AfxThrowError();

    arxAnimation ani;
    arxAnimationBlueprint anib = { 0 };
    anib.dur = aniHdr->dur;
    anib.timeStep = aniHdr->timeStep;
    anib.oversampling = aniHdr->oversampling;
    anib.gesSlotCnt = aniHdr->gesCnt;
    anib.gestures = gestures;
    AfxMakeString32(&anib.id, urn);
    if (ArxAssembleAnimations(scio, 1, &anib, &ani))
        AfxThrowError();

    //if (_ArxRelinkGestures(ani, i, 1, &gestures[i]))
        //AfxThrowError();

    AfxDisposeObjects(aniHdr->gesCnt, gestures);

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

    FMA_ANI_HDR aniHdr;

    if (AfxReadStream(iob, sizeof(aniHdr), 0, &aniHdr))
        AfxThrowError();

    if (ArxUploadAnimation(scio, iob, &aniHdr, urn, animation))
        AfxThrowError();

    AfxDisposeObjects(1, &iob);

    return err;
}
