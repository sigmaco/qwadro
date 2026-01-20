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
#define _ARX_NODE_C
#define _ARX_PUPPET_C
#define _ARX_MOTIVE_C
#define _ARX_ANIMATION_C
#define _ARX_GESTURE_C
#define _ARX_SKELETON_C
#include "../scene/arxIcd.h"

_ARX afxError _ArxPuplDtorCb(arxPuppetLink pupl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUPL, 1, &pupl);


    return err;
}

_ARX afxError _ArxPuplCtorCb(arxPuppetLink pupl, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUPL, 1, &pupl);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(args[1]);
    _arxPuplAcq const* puplAcq = AFX_CAST(_arxPuplAcq const*, args[1]);
    arxPuppet pup = puplAcq->pup;
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    if (!puplAcq->rigIdxCnt)
    {
        pupl->rigIdx = puplAcq->mdli.rigCnt > invokeNo ? invokeNo : AFX_INVALID_INDEX;
    }
    else
    {
        pupl->rigIdx = puplAcq->rigIdxCnt > invokeNo ? puplAcq->rigIdxLut[invokeNo] : AFX_INVALID_INDEX;
    }



    return err;
}

_ARX afxClassConfig const _ARX_PUPL_CLASS_CONFIG =
{
    .fcc = afxFcc_PUPL,
    .name = "PuppetLink",
    .desc = "Puppet Link",
    .fixedSiz = sizeof(AFX_OBJECT(arxPuppetLink)),
    .ctor = (void*)_ArxPuplCtorCb,
    .dtor = (void*)_ArxPuplDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxBool ArxGetPuppetModel(arxPuppet pup, arxModel* model)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
    arxModel mdl = pup->mdl;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(model);
    *model = mdl;
    return (mdl != NIL);
}

_ARX afxBool ArxGetPuppetPose(arxPuppet pup, arxPose* pose, arxPosture* placement)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
    arxPose pos = pup->pose;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_POSE, 1, &pos);
    AFX_ASSERT(pose);
    *pose = pos;

    if (placement)
        *placement = pup->placement;

    return (pos != NIL);
}

_ARX afxUnit ArxCountPuppetMotives(arxPuppet pup)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
    return pup->motives.cnt;
}


_ARX void _ArxPupStepCb(arxPuppet pup, afxReal time)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
}

_ARX void ArxStepPuppetMotives(arxPuppet pup, afxReal time)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
    
    pup->ddi->stepCb(pup, time);
}

_ARX void _ArxPupPurgeCb(arxPuppet pup)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
}

_ARX void ArxPurgeTerminatedMotives(arxPuppet pup)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    pup->ddi->purgeTermMotvCb(pup);
}

_ARX void _ArxPupRecenterMotvClocksCb(arxPuppet pup, afxReal currClock)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
}

_ARX void ArxRecenterPuppetMotiveClocks(arxPuppet pup, afxReal currClock)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    pup->ddi->recenterMotvClocksCb(pup, currClock);
}

_ARX void _ArxPipCompMotVecsCb(arxPuppet pup, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
}

_ARX void ArxComputePuppetMotionVectors(arxPuppet pup, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation)
{
    // AfxGetBodyRootMotionVectors
    // AfxQueryBodyRootMotionVectors

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    pup->ddi->compMotVecCb(pup, secsElapsed, inverse, translation, rotation);
}

_ARX void ArxComputePuppetMotionMatrix(arxPuppet pup, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    afxV4d t, r;
    ArxComputePuppetMotionVectors(pup, secsElapsed, inverse, t, r);
    AfxM4dRigidMotion(m, mm, r, t);
}

_ARX void _ArxPupAccumAnimsCb(arxPose rslt, arxPuppet pup, afxUnit basePivotIdx, afxUnit pivotCnt, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);
}

_ARX void ArxAccumulatePuppetAnimations(arxPose rslt, arxPuppet pup, afxUnit basePivotIdx, afxUnit pivotCnt, afxReal allowedErr, afxUnit const sparseJntMap[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    pup->ddi->accumAnimsCb(rslt, pup, basePivotIdx, pivotCnt, allowedErr, sparseJntMap);
}

_ARX afxError ArxDetachPuppet(arxPuppet pup)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    ArxBreakNodulation(&pup->nodu);

    return err;
}

_ARX afxError ArxAttachPuppet(arxPuppet pup, arxNode nod, void(*sync)(arxNodular*), afxFlags dagFlags, afxMask dagMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    ArxMakeNodulation(&pup->nodu, nod, sync, dagFlags, dagMask);

    return err;
}

_ARX afxError _ArxRigModelToPuppet(arxPuppet pup, arxModel mdl, afxUnit rigIdxCnt, afxUnit const rigIdxLut[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    arxModel currMdl = pup->mdl;
    arxModelInfo mdli;

    if (currMdl != mdl)
    {
        if (currMdl)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &currMdl);

            // Dispose body parts.
            AfxDisposeObjects(pup->partCnt, pup->parts);

            // Dereference the model.
            AfxDisposeObjects(1, &currMdl);
            pup->mdl = NIL;
        }

        if (mdl)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
            AfxReacquireObjects(1, &mdl);
            pup->mdl = mdl;
        }
    }

    if (!mdl) return err;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    ArxDescribeModel(mdl, &mdli);

    arxScenario scio = AfxGetHost(pup);

    afxClass* puplCls = (afxClass*)_ArxScioGetPuplClass(scio);
    AFX_ASSERT_CLASS(puplCls, afxFcc_PUPL);

    afxUnit partToRigCnt = AFX_MIN(pup->partCnt, rigIdxCnt ? rigIdxCnt : mdli.rigCnt);

    _arxPuplAcq puplAcq = { 0 };
    puplAcq.pup = pup;
    puplAcq.mdl = mdl;
    puplAcq.mdli = mdli;
    puplAcq.rigIdxCnt = rigIdxCnt;
    puplAcq.partToRigCnt = partToRigCnt;

    if (AfxAcquireObjects(puplCls, partToRigCnt, (afxObject*)pup->parts, (void const*[]) { scio, (void*)&puplAcq }))
        AfxThrowError();

    return err;
}

_ARX _arxDdiPup const _ARX_PUP_DDI =
{
    .stepCb = _ArxPupStepCb,
    .purgeTermMotvCb = _ArxPupPurgeCb,
    .recenterMotvClocksCb = _ArxPupRecenterMotvClocksCb,
    .compMotVecCb = _ArxPipCompMotVecsCb,
    .accumAnimsCb = _ArxPupAccumAnimsCb,

    // temporarily here to advance works.
    .performManedPose = NIL,
};

_ARX afxError _ArxPupDtorCb(arxPuppet pup)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    arxMotive intk;
    AFX_ITERATE_CHAIN(intk, pup, &pup->motives)
    {
        //_ArxDestroyMotive(intk);
        AfxDisposeObjects(1, &intk);
    }

    ArxDetachPuppet(pup);

    _ArxRigModelToPuppet(pup, NIL, 0, NIL);

    if (pup->mdl)
        AfxDisposeObjects(1, &pup->mdl);

    if (pup->pose)
        AfxDisposeObjects(1, &pup->pose);

    if (pup->placement)
        AfxDisposeObjects(1, &pup->placement);

    if (pup->parts)
    {
        afxObjectStash bodpStash =
        {
            .cnt = pup->partCnt,
            .siz = sizeof(pup->parts[0]),
            .var = (void**)&pup->parts,
        };
        if (AfxDeallocateInstanceData(pup, 1, &bodpStash))
            AfxThrowError();
    }

    return err;
}

_ARX afxError _ArxPupCtorCb(arxPuppet pup, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &pup);

    arxScenario scio = args[0];
    arxModel mdl = args[1];

    pup->ddi = &_ARX_PUP_DDI;
    pup->idd = NIL;
    pup->udd = NIL;
    pup->tag = (afxString) { 0 };

    AfxMakeChain(&pup->motives, pup);
    pup->reserved0 = 0;
    pup->reserved1 = 0;

    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit jntCnt = ArxCountJoints(mdl, 0);

    arxModelInfo mdli;
    ArxDescribeModel(mdl, &mdli);

    afxObjectStash puplStash =
    {
        .cnt = mdli.rigCnt,
        .siz = sizeof(pup->parts[0]),
        .var = (void**)&pup->parts,
    };

    if (AfxAllocateInstanceData(pup, 1, &puplStash))
    {
        AfxThrowError();
    }

    pup->mdl = NIL;
    pup->partCnt = mdli.rigCnt;
    AFX_ASSERT(pup->parts);
    AfxZero(pup->parts, sizeof(pup->parts[0]) * pup->partCnt);
    
    _ArxRigModelToPuppet(pup, mdl, 0, NIL);

    pup->nodu = (arxNodular) { 0 };
    ArxAttachPuppet(pup, NIL, NIL, NIL, NIL);

    //arxScenario scio = ArxGetSimulationDrawInput(scio);

    arxPoseInfo posei = { 0 };
    posei.artCnt = jntCnt;
    ArxAcquirePoses(scio, 1, &posei, &pup->pose);
    ArxAcquirePostures(scio, 1, (afxUnit[]) { jntCnt }, NIL, &pup->placement);

    return err;
}

_ARX afxClassConfig const _ARX_PUP_CLASS_CONFIG =
{
    .fcc = afxFcc_PUP,
    .name = "Puppet",
    .desc = "Animable Model Puppet",
    .fixedSiz = sizeof(AFX_OBJECT(arxPuppet)),
    .ctor = (void*)_ArxPupCtorCb,
    .dtor = (void*)_ArxPupDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxUnit ArxEnumeratePuppets(arxScenario scio, afxUnit first, afxUnit cnt, arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &scio);
    afxClass const* cls = _ArxScioGetPupClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PUP);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)puppets);
}

_ARX afxUnit ArxInvokePuppets(arxScenario scio, afxUnit first, afxUnit cnt, afxBool(*f)(arxPuppet, void*), void *udd)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = _ArxScioGetPupClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PUP);
    return AfxInvokeObjects(cls, (void*)f, udd, first, cnt);
}

_ARX afxError ArxAcquirePuppets(arxScenario scio, arxModel mdl, afxUnit cnt, arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetPupClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PUP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)puppets, (void const*[]) { scio, mdl }))
    {
        AfxThrowError();
    }
    return err;
}

_ARX afxUnit ArxPerformManipulatedPose(arxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, arxTrackMask* modelMask, afxUnit cnt, arxPuppet puppets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_PUP, cnt, puppets);
    afxUnit rslt = 0;

    if ((rslt = puppets[0]->ddi->performManedPose(pose, startTime, duration, iterCnt, modelMask, cnt, puppets)))
    {
        AfxThrowError();
    }

    return rslt;
}
