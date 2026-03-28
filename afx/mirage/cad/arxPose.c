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

#define _ARX_POSE_C
//#define _ARX_SKELETON_C
//#define _ARX_MODEL_C
//#define _ARX_ANIMATION_C
#include "../scene/arxIcd.h"

_ARX _arxPoseArticulation* _ArxPoseGetPaArray(arxPose pose, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, base, 1);
    return &pose->arts[base];
}

_ARX afxUnit ArxGetPoseCapacity(arxPose pose, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, base, 1);
    return pose->artCnt > base ? pose->artCnt - base : 0;
}

_ARX void ArxDescribePose(arxPose pose, arxPoseInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    info->artCnt = pose->artCnt;
    info->fillThreshold = pose->fillThreshold;
    info->tag = pose->tag;
    info->udd = pose->udd;
}

_ARX void ArxDescribePoseArticulations(arxPose pose, afxUnit base, afxUnit cnt, afxUnit const remap[], arxPoseArticulation infos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, base, cnt);
    AFX_ASSERT(infos);

    if (remap)
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit artIdx = base + remap[i];
            infos[i].cnt = pose->arts[artIdx].cnt;
            infos[i].traversalId = pose->arts[artIdx].traversalId;
            infos[i].weight = pose->arts[artIdx].weight;
            infos[i].xform = pose->arts[artIdx].xform;
        }
        return;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit artIdx = base + i;
        infos[i].cnt = pose->arts[artIdx].cnt;
        infos[i].traversalId = pose->arts[artIdx].traversalId;
        infos[i].weight = pose->arts[artIdx].weight;
        infos[i].xform = pose->arts[artIdx].xform;
    }
}

_ARX afxTransform* ArxGetPoseTransform(arxPose pose, afxUnit artIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, artIdx, 1);
    return &pose->arts[artIdx].xform;
}

_ARX void ArxCopyPose(arxPose pose, afxUnit toBaseIdx, arxPose from, afxUnit fromBaseIdx, afxUnit cnt, afxUnit const remap[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, toBaseIdx, cnt);
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &from);
    AFX_ASSERT_RANGE(from->artCnt, fromBaseIdx, cnt);

    if (!remap)
    {
        // non-remapped copy

        for (afxUnit i = 0; i < cnt; i++)
        {
            _arxPoseArticulation* dst = &pose->arts[toBaseIdx + i];
            _arxPoseArticulation const* src = &from->arts[fromBaseIdx + i];

            dst->weight = src->weight;
            dst->cnt = src->cnt;
            dst->traversalId = src->traversalId;
            AfxCopyTransform(&dst->xform, &src->xform);
        }
        return;
    }

    // remapped copy

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit artIdx = remap[i];
        afxUnit toIdx = toBaseIdx + artIdx;
        afxUnit fromIdx = fromBaseIdx + artIdx;
        _arxPoseArticulation* dst = &pose->arts[toIdx];
        _arxPoseArticulation const* src = &from->arts[fromIdx];

        dst->weight = src->weight;
        dst->cnt = src->cnt;
        dst->traversalId = src->traversalId;
        AfxCopyTransform(&dst->xform, &src->xform);
    }
}

_ARX void ArxComputeAttachmentWorldMatrix(arxPose pose, arxSkeleton skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m)
{
    // Should be compatible with void GetWorldMatrixFromLocalAttitude(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalAttitude, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)
    // void AfxSklGetWorldMatrixFromLocalAttitude(arxSkeleton skl, afxUnit jointIdx, arxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseBoneArray, afxUnit const* sparseBoneArrayReverse)
    // AfxSklGetWorldMatrixFromLocalAttitude(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, jntIdx, 1);
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT(m);

    afxUnit maxJntCnt = ArxGetSkeletonDepth(skl);
    afxUnit const* pi = _ArxSklGetPiArray(skl, 0);
    AfxM4dReset(m);

    for (afxUnit i = jntIdx; i != AFX_INVALID_INDEX; i = pi[i])
    {
        AFX_ASSERT_RANGE(maxJntCnt, i, 1);
        afxTransform* t = ArxGetPoseTransform(pose, sparseJntMapRev ? sparseJntMapRev[i] : i);
        afxM4d tmp, tmp2;
        AfxComputeCompositeTransformM4d(t, tmp);
        AfxM4dMultiplyAtm(tmp2, m, tmp);
        AfxM4dCopy(m, tmp2);
    }

    if (displace)
    {
        afxM4d tmp2;
        AfxM4dMultiplyAtm(tmp2, m, displace);
        AfxM4dCopy(m, tmp2);
    }
}

_ARX void ArxComputeAttachmentOffset(arxPose pose, arxSkeleton skl, afxUnit jntIdx, afxUnit const sparseJntMap[], afxUnit const sparseJntMapRev[], afxM4d const displace, afxM4d m)
{
    // void AfxSklGetSkeletonAttachmentOffset(arxSkeleton skl, afxUnit jntIdx, arxPose const pos, afxM4d const offset, afxM4d m, afxUnit const* sparseArtArray, afxUnit const* sparseArtArrayReverse)
    // AfxSklGetSkeletonAttachmentOffset(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_RANGE(pose->artCnt, jntIdx, 1);
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT(m);

    afxM4d tmp;
    ArxComputeAttachmentWorldMatrix(pose, skl, jntIdx, sparseJntMap, sparseJntMapRev, displace, tmp);
    AfxM4dInvertAtm(m, tmp); // invert it
}

_ARX void ArxRebuildRestPose(arxPose pose, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt)
{
    // Should be compatible with void BuildRestLocalAttitude(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalAttitude)
    // AfxSklComputeRestLocalAttitude(skl, baseJntIdx, jntCnt, atti);

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);

    AFX_ASSERT_RANGE(pose->artCnt, baseJntIdx, jntCnt);

    afxUnit maxJntCnt = ArxGetSkeletonDepth(skl);
    AFX_ASSERT_RANGE(maxJntCnt, baseJntIdx, jntCnt);

    afxTransform const* lt = _ArxSklGetLtArray(skl, 0);

    for (afxUnit i = 0; i < jntCnt; i++)
    {
        afxUnit jntIdx = baseJntIdx + i;
        AFX_ASSERT_RANGE(maxJntCnt, jntIdx, 1);
        _arxPoseArticulation* pa = &pose->arts[jntIdx];
        pa->cnt = 1;
        pa->weight = 1.0;
        AfxCopyTransform(&pa->xform, &lt[jntIdx]);
    }
}

_ARX void ArxApplyPoseRootMotionVectors(arxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    afxTransform* t = ArxGetPoseTransform(pose, 0);
    AfxV3dAdd(t->pv, t->pv, translation);

    /*
        Rather than just applying the rotation directly to the root joint, the function seems to compute the angular velocity as a quaternion, 
        which is a more robust and efficient way to handle rotational motion, especially for interpolating rotations in animation systems.
    */

    afxQuat rot;
    AfxQuatFromAngularVelocity(rot, rotation);
    AfxQuatMultiply(t->oq, rot, t->oq);
}

_ARX void ArxCommencePoseAccumulation(arxPose pose, afxUnit baseArtIdx, afxUnit artCnt, afxUnit const jntMap[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    ++pose->traversalId;
}

_ARX void ArxConcludePoseAccumulation(arxPose pose, afxUnit baseArtIdx, afxUnit artCnt, arxModel skl, afxReal allowedErr, afxUnit const jntMap[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    afxReal AllowedErrorScaler;
    afxReal AllowedErrorEnd;

    arxModelInfo mdli;
    ArxDescribeModel(skl, &mdli);

    if (mdli.lodType)
    {
        ArxQueryModelErrorTolerance(skl, allowedErr, &AllowedErrorEnd, &AllowedErrorScaler); // TODO insert allowed error in skeleton due it be skl-dependent?
    }
    else
    {
        allowedErr = 0.0;
        AllowedErrorEnd = 0.0;
        AllowedErrorScaler = 0.0;
    }

    for (afxUnit artIdx = baseArtIdx; artIdx < artCnt; artIdx++)
    {
        afxUnit jointIdx = (jntMap) ? jntMap[artIdx] : artIdx;
        _arxPoseArticulation* pa = &pose->arts[artIdx];
        afxTransform const* local = &mdli.skli.lt[jointIdx];

        if (pa->traversalId != pose->traversalId)
            pa->xform = *local;
        else
        {
            if (pa->weight < pose->fillThreshold)
            {
                ArxAccumulateLocalTransform(pose, artIdx, jointIdx, pose->fillThreshold - pa->weight, skl, afxQuatBlend_ACCUM_ADJACENT, local);
            }

            afxReal s = (allowedErr - mdli.skli.le[jointIdx]) * AllowedErrorScaler;

            if (s > 0.0)
            {
                if (s >= 0.99000001)
                {
                    pa->weight = 1.0;
                    pa->cnt = 1;
                    pa->xform = mdli.skli.lt[jointIdx];
                }
                else
                {
                    ArxAccumulateLocalTransform(pose, artIdx, jointIdx, pa->weight / (1.0 - s) * s, skl, afxQuatBlend_ADJACENT, local);
                }
            }

            afxReal wc = 1.0;

            if (pa->cnt != 1 || pa->weight != wc)
            {
                afxReal s2 = 1.0 / pa->weight;
                AfxV3dScale(pa->xform.pv, pa->xform.pv, s2);
                AfxM3dScale(pa->xform.ssm, pa->xform.ssm, s2);
            }

            afxReal sq2, sq = AfxV4dSq(pa->xform.oq);

            if (sq > 1.1 || sq < 0.89999998)
                sq2 = AfxRsqrtf(sq);
            else
                sq2 = (3.0 - sq) * (12.0 - (3.0 - sq) * (3.0 - sq) * sq) * 0.0625;

            AfxV4dScale(pa->xform.oq, pa->xform.oq, sq2);
        }
    }
}

_ARX void ArxAccumulateLocalTransform(arxPose pose, afxUnit artIdx, afxUnit sklJntIdx, afxReal weight, arxModel skl, afxQuatBlend blendOp, afxTransform const* t)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);

    afxReal orientWeight;

    _arxPoseArticulation* pa = &pose->arts[artIdx];

    afxTransform sklLt;
    ArxGetBoneTransforms(ArxGetSkeleton(skl), sklJntIdx, 1, &sklLt);

    switch (blendOp)
    {
    case afxQuatBlend_DIRECT: orientWeight = weight; break;
    case afxQuatBlend_INV: orientWeight = -weight; break;
    case afxQuatBlend_ADJACENT: orientWeight = (AfxV4dSq(sklLt.oq) >= 0.0) ? weight : (-weight); break;
    case afxQuatBlend_ACCUM_ADJACENT:
    {
        afxReal dot;

        if (pa->traversalId == pose->traversalId)
            dot = AfxQuatDot(pa->xform.oq, t->oq);
        else
            dot = AfxQuatDot(sklLt.oq, t->oq);

        orientWeight = (dot >= 0.0) ? weight : (-weight);
        break;
    }
    default: AfxThrowError(); break;
    }

    if (pa->traversalId == pose->traversalId)
    {
        pa->xform.flags |= t->flags;
        AfxV3dMads(pa->xform.pv, pa->xform.pv, t->pv, weight);
        AfxV3dMads(pa->xform.oq, pa->xform.oq, t->oq, orientWeight);
        AfxM3dMads(pa->xform.ssm, pa->xform.ssm, t->ssm, weight);
        pa->weight += weight;
        ++pa->cnt;
    }
    else
    {
        if (weight == 1.0) AfxCopyTransform(&pa->xform, t);
        else
        {
            pa->xform.flags = t->flags;
            AfxV3dScale(pa->xform.pv, t->pv, weight);
            AfxM3dScale(pa->xform.ssm, t->ssm, weight);
        }
        AfxQuatScale(pa->xform.oq, t->oq, orientWeight);
        pa->weight = weight;
        pa->cnt = 1;
        pa->traversalId = pose->traversalId;
    }
}

_ARX afxReal GetTrackMaskBoneWeight(arxTrackMask* mask, afxUnit jntIdx)
{
    double result; // st7

    if (jntIdx >= mask->boneCnt)
        result = mask->defWeight;
    else
        result = mask->boneWeights[jntIdx];
    return result;
}

_ARX void ArxModulatePose(arxPose pose, afxUnit toBaseArtIdx, arxPose composite, afxUnit fromBaseArtIdx, afxUnit artCnt, afxReal weightNone, afxReal weightAll, arxTrackMask* mask, afxUnit const jntRemap[])
{
    // Should be compatible with ModulationCompositeLocalPoseSparse(BasePose, weightNone, wieghtAll, Mask, CompositePose, jntRemap)
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &composite);

    AFX_ASSERT_RANGE(pose->artCnt, toBaseArtIdx, artCnt);
    AFX_ASSERT_RANGE(composite->artCnt, fromBaseArtIdx, artCnt);
    artCnt = AFX_MIN(AFX_MIN(artCnt, pose->artCnt), composite->artCnt);

    if (!mask) weightNone = weightAll;

    afxReal weightScale = weightAll - weightNone;
    if (!(AFX_ABS(weightScale) >= 0.001))
    {
        mask = 0;
        if (weightNone < 0.001) return;

        if (!(weightNone <= 0.99900001))
        {
            ArxCopyPose(pose, toBaseArtIdx, composite, 0, artCnt, NIL);
            return;
        }
    }

    for (afxUnit i = 0; i < artCnt; i++)
    {
        _arxPoseArticulation const* ts = &composite->arts[fromBaseArtIdx + i];
        _arxPoseArticulation* td = &pose->arts[toBaseArtIdx + i];

        afxReal weight = weightAll;

        if (mask)
        {
            afxReal f;
            if (jntRemap)
                f = GetTrackMaskBoneWeight(mask, jntRemap[i]);
            else
                f = GetTrackMaskBoneWeight(mask, i);

            weight = f * weightScale + weightNone;
        }

        td->xform.flags = ts->xform.flags | td->xform.flags;

        if (td->xform.flags & afxTransformFlag_T)
        {
            AfxV3dMix(td->xform.pv, td->xform.pv, ts->xform.pv, weight);
        }

        if (td->xform.flags & afxTransformFlag_R)
        {
            AfxV4dMix(td->xform.oq, td->xform.oq, ts->xform.oq, weight);
            AfxQuatNormalize(td->xform.oq, td->xform.oq);
        }

        if (td->xform.flags & afxTransformFlag_S)
        {
            AfxM3dMix(td->xform.ssm, td->xform.ssm, ts->xform.ssm, weight);
        }
    }
}

_ARX afxError _ArxPoseDtorCb(arxPose pose)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    afxObjectStash const stashes[] =
    {
        {
        .cnt = pose->artCnt,
        .siz = sizeof(pose->arts[0]),
        .var = (void**)&pose->arts
        }
    };

    if (AfxDeallocateInstanceData(pose, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_ARX afxError _ArxPoseCtorCb(arxPose pose, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_POSE, 1, &pose);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxPoseInfo const* cfg = ((arxPoseInfo const*)(args[1])) + invokeNo;
    
    pose->tag = cfg->tag;
    pose->udd = cfg->udd;

    pose->fillThreshold = (cfg->fillThreshold == 0) ? 0.2 : cfg->fillThreshold;
    pose->traversalId = 0;

    pose->arts = NIL;
    pose->artCnt = AFX_MAX(1, cfg->artCnt);

    afxObjectStash const stashes[]=
    {
        {
        .cnt = pose->artCnt,
        .siz = sizeof(pose->arts[0]),
        .var = (void**)&pose->arts
        }
    };
    if (AfxAllocateInstanceData(pose, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(pose->arts);
    
    // need to be zeroed because of demilitarized traversalId comparisons.
    AfxZero(pose->arts, sizeof(pose->arts[0]) * pose->artCnt);

    return err;
}

_ARX afxClassConfig const _ARX_POSE_CLASS_CONFIG =
{
    .fcc = afxFcc_POSE,
    .name = "Pose",
    .desc = "Skeletal Pose",
    .fixedSiz = sizeof(AFX_OBJECT(arxPose)),
    .ctor = (void*)_ArxPoseCtorCb,
    .dtor = (void*)_ArxPoseDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquirePoses(arxScenario scio, afxUnit cnt, arxPoseInfo const cfgs[], arxPose poses[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetPoseClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);

    AFX_ASSERT(poses);
    if (AfxAcquireObjects(cls, cnt, (afxObject*)poses, (void const*[]) { scio, cfgs }))
        AfxThrowError();


    AFX_ASSERT_OBJECTS(afxFcc_POSE, cnt, poses);

    return err;
}
