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

#define _ARX_PLACEMENT_C
//#define _ARX_POSE_C
//#define _ARX_MODEL_C
#include "arxIcd.h"

_ARX afxUnit ArxGetPostureCapacity(arxPosture plce)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    return plce->matCnt;
}

_ARX afxM4d* _ArxPlceGetMmArray(arxPosture plce, afxUnit base)
{
    // AfxPostureGetWorldMatrixArray

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, base, 1);
    return &plce->mm[base];
}

_ARX afxM4d* _ArxPlceGetDeltaArray(arxPosture plce, afxUnit base)
{
    // AfxPostureGetCompositeMatrixArray

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, base, 1);
    return &plce->delta[base];
}

_ARX afxM4d* ArxGetPostureDeltas(arxPosture plce, afxUnit baseArtIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->delta[baseArtIdx];
}

_ARX afxM4d* ArxGetPostureMatrices(arxPosture plce, afxUnit baseArtIdx)
{
    //float (*__cdecl GetPosture4x4(const world_pose *Posture, int BoneIndex))[4]
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseArtIdx, 1);
    return &plce->mm[baseArtIdx];
}

/*
    The "world matrix" in the world buffer is not the mesh/world-space transform.
    It is the current pose matrix of each bone in model space.

    The inverse world matrix stored in the skeleton is actually the inverse bind-pose matrix.

    So the composite matrix is doing:
    composite = current_bone_pose * inverse_bind_pose

    This is exactly what we want for skinning.

    //

    When we "build the world buffer", the system walks the hierarchy and produces:
    WorldPose[bone] = LocalPose[bone] * WorldPose[parent]

    Despite the name, this is not world-space in the engine sense. This is model-space/skeleton-space.

    It means that root bone is in model space. Child bones are accumulated relative to the root.

    So "world" here really means "fully accumulated pose".

    This naming is historical and confusing.
*/

_ARX void ArxProducePosture(arxPosture plce, arxPose pose, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxM4d const displace, afxBool skipDeltas)
// Suggested signature: (arxPosture plce, afxBool skipDeltas, afxM4d const displace, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, arxPose pose, afxUnit baseReqJnt, afxUnit reqJntCnt).
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    afxUnit totalJntCnt = ArxGetSkeletonDepth(skl);
    AFX_ASSERT_RANGE(totalJntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);

    if (!displace)
        displace = AFX_M4D_IDENTITY;

    afxM4d const* iw = _ArxSklGetIwArray(skl, 0);
    afxUnit const* pi = _ArxSklGetPiArray(skl, 0);
    afxTransform const* lt = _ArxSklGetLtArray(skl, 0);
    afxM4d* mm = plce->mm;
    afxM4d* delta = plce->delta;

    if (!pose)
    {
        // compute the rest pose whether a attitude is not specified.
        // rest pose is computed for every joint ignoring baseReqJnt/reqJntCnt;
        // rest pose always compute composite delta matrices.
        // Should be compatible with void BuildRestPosture(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)
        // AfxSklComputeRestPosture(skl, baseJntIdx, jntCnt, offset, pose);

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            afxUnit jntIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

            afxUnit parentIdx = pi[jntIdx];
            if (parentIdx != AFX_INVALID_INDEX)
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);

                if (delta) // delta is always computed if present for rest pose.
                {
                    BWP_Dispatch(&lt[jntIdx], iw[jntIdx], delta[jntIdx], mm[jntIdx], mm[parentIdx]);
                }
                else
                {
                    BWPNC_Dispatch(&lt[jntIdx], mm[parentIdx], mm[jntIdx]);
                }
            }
            else
            {
                if (delta) // delta is always computed if present for rest pose.
                {
                    BWP_Dispatch(&lt[jntIdx], iw[jntIdx], delta[jntIdx], mm[jntIdx], displace);
                }
                else
                {
                    BWPNC_Dispatch(&lt[jntIdx], displace, mm[jntIdx]);
                }
            }
        }
        // Exit.
        return;
    }

    afxUnit poseCap = ArxGetPoseCapacity(pose, 0);
    AFX_ASSERT_RANGE(poseCap, baseJntIdx, jntCnt);
    _arxPoseArticulation const* pa = _ArxPoseGetPaArray(pose, 0);

    if (!delta || skipDeltas)
    {
        // Should be compatible with void BuildPostureNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
        // AfxSklComputePostureNoCompositeLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            afxUnit jntIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

            afxTransform const *t = &pa[jntIdx].xform;

            if (reqJntCnt <= 0)
                t = &lt[jntIdx];

            afxUnit parentIdx = pi[jntIdx];
            if (parentIdx != AFX_INVALID_INDEX)
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                BWPNC_Dispatch(t, mm[parentIdx], mm[jntIdx]);
            }
            else
            {
                BWPNC_Dispatch(t, displace, mm[jntIdx]);
            }

            --reqJntCnt;
        }
        // Exit.
        return;
    }

    // Should be compatible with void BuildPostureLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
    // AfxSklComputePostureLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);
    AFX_ASSERT(delta);

    for (afxUnit i = 0; i < jntCnt; i++)
    {
        afxUnit jntIdx = baseJntIdx + i;
        AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

        afxTransform const *t = &pa[jntIdx].xform;

        if (reqJntCnt <= 0)
            t = &lt[jntIdx];

        afxUnit parentIdx = pi[jntIdx];
        if (parentIdx != AFX_INVALID_INDEX)
        {
            AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
            BWP_Dispatch(t, iw[jntIdx], delta[jntIdx], mm[jntIdx], mm[parentIdx]);
        }
        else
        {
            BWP_Dispatch(t, iw[jntIdx], delta[jntIdx], mm[jntIdx], displace);
        }

        --reqJntCnt;
    }
}

_ARX void ArxProducePosturePose(arxPosture plce, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxBool rigid, arxPose pose)
// Suggested signature: (arxPosture plce, afxBool rigid, afxM4d const displace, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, arxPose pose)
{
    // Computes the local pose from the world pose.
    // If rigid is argumented as TRUE, the local pose will not include any scale/shear.

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    afxUnit totalJntCnt = ArxGetSkeletonDepth(skl);
    AFX_ASSERT_RANGE(totalJntCnt, baseJntIdx, jntCnt);
    afxUnit poseCap = ArxGetPoseCapacity(pose, 0);
    AFX_ASSERT_RANGE(poseCap, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(plce);

    if (!displace)
        displace = AFX_M4D_IDENTITY;

    afxUnit const* pi = _ArxSklGetPiArray(skl, 0);
    afxM4d* mm = plce->mm;

    if (rigid)
    {
        //AfxSklLocalAttitudeFromPostureNoScale(skl, atti, pose, offset, baseJntIdx, jntCnt);
        // Should be compatible with void LocalAttitudeFromPostureNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jntIdx = baseJntIdx + i;

            afxM4d invParent;
            afxUnit parentIdx = pi[jntIdx];
            if (parentIdx != AFX_INVALID_INDEX)
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                AfxM4dInvertAtm(invParent, mm[parentIdx]);
            }
            else
            {
                AfxM4dInvertAtm(invParent, displace);
            }

            afxM4d lam;
            AfxM4dMultiplyAtm(lam, mm[jntIdx], invParent);
            afxTransform* t = ArxGetPoseTransform(pose, jntIdx);

            afxM3d rm;
            AfxM3dSet(rm, lam[0], lam[1], lam[2]);
            AfxQuatRotationM3d(t->oq, rm);
            AfxQuatNormalize(t->oq, t->oq);
            AfxV3dCopy(t->pv, lam[3]);
            AfxM3dReset(t->ssm);
            t->flags = afxTransformFlag_T | afxTransformFlag_R;
        }
    }
    else
    {
        // Should be compatible with void LocalAttitudeFromPosture(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)
        //AfxSklLocalAttitudeFromPosture(skl, atti, pose, offset, baseJntIdx, jntCnt);

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jntIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);

            afxM4d invParent;
            afxUnit parentIdx = pi[jntIdx];
            if (parentIdx != AFX_INVALID_INDEX)
            {
                AFX_ASSERT_RANGE(totalJntCnt, parentIdx, 1);
                AfxM4dInvertAtm(invParent, mm[parentIdx]);
            }
            else
            {
                AfxM4dInvertAtm(invParent, displace);
            }

            afxM4d lam;
            AfxM4dMultiplyAtm(lam, mm[jntIdx], invParent);

            afxM3d ltm;
            AfxM3dSet(ltm, lam[0], lam[1], lam[2]);

            afxM3d rm, ssm;
            if (!AfxM3dPolarDecompose(ltm, 0.0000099999997, rm, ssm))
                AfxReportError("Can't accurately decompose MAX transform Q");

            afxV3d pos;
            AfxV3dCopy(pos, lam[3]);

            afxQuat orien;
            AfxQuatRotationM3d(orien, rm);
            AfxQuatNormalize(orien, orien);

            afxTransform* t = ArxGetPoseTransform(pose, jntIdx);
            AfxMakeTransform(t, pos, orien, ssm, TRUE);
        }
    }
}

_ARX void ArxProduceCompositeMatrices(arxPosture plce, arxSkeleton skl, afxUnit baseJnt, afxUnit cnt, afxBool /*3x4*/transposed, afxM4d matrices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    AFX_ASSERT_RANGE(plce->matCnt, baseJnt, cnt);
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    afxUnit totalJntCnt = ArxGetSkeletonDepth(skl);
    AFX_ASSERT_RANGE(totalJntCnt, baseJnt, cnt);
    AFX_ASSERT(matrices);

    afxM4d const* iw = _ArxSklGetIwArray(skl, 0);
    afxM4d const* mm = plce->mm;

    if (transposed)
    {
        // Should be compatible with BuildCompositeBufferTransposed(skl, firstBone, cnt, wp, compositeBuffer)

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = baseJnt + i;
            BuildSingleCompositeFromWorldPoseTranspose_Generic(iw[jntIdx], mm[jntIdx], matrices[i]);
        }
    }
    else
    {
        // Should be compatible with BuildCompositeBuffer(skl, firstBone, cnt, wp, compositeBuffer)

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = baseJnt + i;
            BuildSingleCompositeFromWorldPose_Generic(iw[jntIdx], mm[jntIdx], matrices[i]);
        }
    }
}

_ARX void ArxProduceIndexedCompositeMatrices(arxPosture plce, arxSkeleton skl, afxUnit cnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxM4d matrices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);
    afxUnit totalMatCnt = plce->matCnt;
    AFX_ASSERT_RANGE(totalMatCnt, 0, cnt);
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    afxUnit totalJntCnt = ArxGetSkeletonDepth(skl);
    AFX_ASSERT_RANGE(totalJntCnt, 0, cnt);
    AFX_ASSERT(matrices);
    AFX_ASSERT(jntMap);

    afxM4d const* iw = _ArxSklGetIwArray(skl, 0);
    afxM4d const* mm = plce->mm;

    if (transposed)
    {
        // void AfxSklBuildIndexedCompositeBufferTransposed(arxSkeleton skl, arxPosture const posb, afxUnit const* indices, afxUnit idxCnt, afxReal buffer[][3][4])
        // AfxSklBuildIndexedCompositeBufferTransposed(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = jntMap[i];
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);
            AFX_ASSERT_RANGE(totalMatCnt, jntIdx, 1);
            BuildSingleCompositeFromWorldPoseTranspose_Generic(iw[jntIdx], mm[jntIdx], matrices[i]);
        }
    }
    else
    {
        // void AfxSklBuildIndexedCompositeBuffer(arxSkeleton skl, arxPosture const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
        // AfxSklBuildIndexedCompositeBuffer(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

        // joints used to be biasToJointMap of model's mesh rig.

        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit jntIdx = jntMap[i];
            AFX_ASSERT_RANGE(totalJntCnt, jntIdx, 1);
            AFX_ASSERT_RANGE(totalMatCnt, jntIdx, 1);
            BuildSingleCompositeFromWorldPose_Generic(iw[jntIdx], mm[jntIdx], matrices[i]);
        }
    }
}

_ARX afxError _ArxPosbDtorCb(arxPosture plce)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    if (plce->delta)
        AfxDeallocate((void**)&plce->delta, AfxHere());

    if (plce->mm)
        AfxDeallocate((void**)&plce->mm, AfxHere());

    return err;
}

_ARX afxError _ArxPosbCtorCb(arxPosture plce, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PLCE, 1, &plce);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxUnit artCnt = *(((afxUnit const*)(args[1])) + invokeNo);
    afxBool excludeComposite = args[2] ? *(((afxBool const*)(args[2])) + invokeNo) : TRUE;
    
    plce->matCnt = artCnt;
    AfxAllocate(plce->matCnt * sizeof(plce->mm[0]), 0, AfxHere(), (void**)&plce->mm);
    plce->delta = NIL;
    
    if ((!excludeComposite) && plce->matCnt)
        AfxAllocate(plce->matCnt * sizeof(plce->delta[0]), 0, AfxHere(), (void**)&plce->delta);

    return err;
}

_ARX afxClassConfig const _ARX_PLCE_CLASS_CONFIG =
{
    .fcc = afxFcc_PLCE,
    .name = "Posture",
    .desc = "Skeletal Posture Buffer",
    .fixedSiz = sizeof(AFX_OBJECT(arxPosture)),
    .ctor = (void*)_ArxPosbCtorCb,
    .dtor = (void*)_ArxPosbDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquirePostures(arxScenario scio, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeComposite[], arxPosture placements[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetPlceClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)placements, (void const*[]) { scio, artCnt, excludeComposite }))
        AfxThrowError();

    return err;
}
