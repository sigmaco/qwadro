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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#define _ARX_POSE_C
#define _ARX_PLACEMENT_C
#include "../sim/asxImplementation.h"
#include "../scene/arxScenarioDDK.h"


_ARX afxCmdId AsxCmdCopyPose(arxContext ctx, arxPose src, afxUnit from, arxPose dst, afxUnit base, afxUnit cnt)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    AFX_ASSERT(dst);
    AFX_ASSERT(src);

    AFX_ASSERT_RANGE(src->artCnt, from, cnt);
    AFX_ASSERT_RANGE(dst->artCnt, base, cnt);

    from = AFX_MIN(from, src->artCnt - 1);
    base = AFX_MIN(from, dst->artCnt - 1);
    cnt = AFX_MIN(cnt, src->artCnt - from);
    cnt = AFX_MIN(cnt, dst->artCnt - base);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(CopyPose), sizeof(cmd->CopyPose), &cmdId);
    AFX_ASSERT(cmd);

    cmd->CopyPose.src = src;
    cmd->CopyPose.dst = dst;
    cmd->CopyPose.cnt = cnt;
    cmd->CopyPose.base = base;
    cmd->CopyPose.from = from;

    return 0;
}

_ARX afxCmdId AsxCmdApplyRootMotionVectors(arxContext ctx, arxPose pose, afxV3d const translation, afxV3d const rotation)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ApplyRootMotionVectors), sizeof(cmd->ApplyRootMotionVectors), &cmdId);
    AFX_ASSERT(cmd);

    cmd->ApplyRootMotionVectors.pose = pose;
    AfxV3dCopy(cmd->ApplyRootMotionVectors.translation, translation);
    AfxV3dCopy(cmd->ApplyRootMotionVectors.rotation, rotation);

    return 0;
}

_ARX afxCmdId AsxCmdSamplePuppetAnimationsLODSparse(arxContext ctx, arxPuppet bod, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SamplePuppetAnimations), sizeof(cmd->SamplePuppetAnimations), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &bod);

    cmd->SamplePuppetAnimations.allowedErr = allowedErr;
    cmd->SamplePuppetAnimations.basePivotIdx = basePivotIdx;
    cmd->SamplePuppetAnimations.bod = bod;
    cmd->SamplePuppetAnimations.pivotCnt = pivotCnt;
    cmd->SamplePuppetAnimations.pose = pose;
    cmd->SamplePuppetAnimations.sparseBoneArray = sparseBoneArray;
}

_ARX afxCmdId AsxCmdSampleSinglePuppetAnimationLODSparse(arxContext ctx, arxPuppet bod, arxPulley moto, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose pose, afxReal allowedErr, afxUnit const* sparseBoneArray)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SampleSinglePuppetAnimation), sizeof(cmd->SampleSinglePuppetAnimation), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &bod);
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &moto);

    cmd->SampleSinglePuppetAnimation.allowedErr = allowedErr;
    cmd->SampleSinglePuppetAnimation.basePivotIdx = basePivotIdx;
    cmd->SampleSinglePuppetAnimation.bod = bod;
    cmd->SampleSinglePuppetAnimation.moto = moto;
    cmd->SampleSinglePuppetAnimation.pivotCnt = pivotCnt;
    cmd->SampleSinglePuppetAnimation.pose = pose;
    cmd->SampleSinglePuppetAnimation.sparseBoneArray = sparseBoneArray;
    
    return cmdId;
}

_ARX afxCmdId AsxCmdSamplePuppetAnimationsAcceleratedLOD(arxContext ctx, arxPuppet bod, afxUnit pivotCnt, afxM4d const displace, arxPose scratch, arxPosture plce, afxReal allowedErr)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(SamplePuppetAnimationsAccelerated), sizeof(cmd->SamplePuppetAnimationsAccelerated), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &bod);
    AFX_ASSERT(scratch);

    cmd->SamplePuppetAnimationsAccelerated.bod = bod;
    cmd->SamplePuppetAnimationsAccelerated.allowedErr = allowedErr;
    AfxM4dCopy(cmd->SamplePuppetAnimationsAccelerated.displace, displace ? displace : AFX_M4D_IDENTITY);
    cmd->SamplePuppetAnimationsAccelerated.pivotCnt = pivotCnt;
    cmd->SamplePuppetAnimationsAccelerated.plce = plce;
    cmd->SamplePuppetAnimationsAccelerated.scratch = scratch;
    
    return cmdId;
}

// MODEL OPS

_ARX afxCmdId AsxCmdDisplaceModel(arxContext ctx, arxModel mdl, afxTransform const* t)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(DisplaceModel), sizeof(cmd->DisplaceModel), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(t);
    //AFX_ASSERT(bufIdx);

    cmd->DisplaceModel.mdl = mdl;
    cmd->DisplaceModel.displacement = t ? *t : AFX_TRANSFORM_IDENTITY;

    return cmdId;
}

_ARX afxCmdId AsxCmdBuildRiggedMeshCompositeMatrices(arxContext ctx, arxModel mdl, afxUnit rigIdx, arxPosture const plce, afxUnit cnt, afxUnit bufIdx, afxSize offset)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildRiggedMeshCompositeMatrices), sizeof(cmd->BuildRiggedMeshCompositeMatrices), &cmdId);
    AFX_ASSERT(cmd);

    arxModelInfo mdli;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    ArxDescribeModel(mdl, &mdli);
    AFX_ASSERT_RANGE(mdli.rigCnt, rigIdx, 1);

    AFX_ASSERT(mdli.skli.boneCnt >= cnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= cnt);
    //AFX_ASSERT(bufIdx);

    cmd->BuildRiggedMeshCompositeMatrices.mdl = mdl;
    cmd->BuildRiggedMeshCompositeMatrices.plce = plce;
    cmd->BuildRiggedMeshCompositeMatrices.rigIdx = rigIdx;
    cmd->BuildRiggedMeshCompositeMatrices.cnt = cnt;
    cmd->BuildRiggedMeshCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildRiggedMeshCompositeMatrices.bufOffset = offset;

    return cmdId;
}

// SKELETON OPS

_ARX afxCmdId AsxCmdReparentJoints(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit idxSiz, void const* idxData)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ReparentJoints), sizeof(cmd->ReparentJoints) + (jntCnt * idxSiz), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(idxData);
    AFX_ASSERT(idxSiz);

    cmd->ReparentJoints.skl = skl;
    cmd->ReparentJoints.baseJntIdx = baseJntIdx;
    cmd->ReparentJoints.jntCnt = jntCnt;
    cmd->ReparentJoints.idxSiz = idxSiz;
    AfxCopy(cmd->ReparentJoints.idxData, idxData, jntCnt * idxSiz);

    return cmdId;
}

_ARX afxCmdId AsxCmdResetJointTransforms(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxTransform const transforms[])
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointTransforms), sizeof(cmd->ResetJointTransforms) + (jntCnt * sizeof(cmd->ResetJointTransforms.transforms[0])), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(transforms);
    
    cmd->ResetJointTransforms.skl = skl;
    cmd->ResetJointTransforms.baseJntIdx = baseJntIdx;
    cmd->ResetJointTransforms.jntCnt = jntCnt;

    for (afxUnit i = 0; i < jntCnt; i++)
        AfxCopyTransform(&cmd->ResetJointTransforms.transforms[i], &transforms[i]);

    return cmdId;
}

_ARX afxCmdId AsxCmdResetJointInversors(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit mtxSiz, void const* matrices)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointInversors), sizeof(cmd->ResetJointInversors) + (jntCnt * mtxSiz), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(matrices);
    AFX_ASSERT(mtxSiz);

    cmd->ResetJointInversors.skl = skl;
    cmd->ResetJointInversors.baseJntIdx = baseJntIdx;
    cmd->ResetJointInversors.jntCnt = jntCnt;
    cmd->ResetJointInversors.mtxSiz = mtxSiz;
    AfxCopy(cmd->ResetJointInversors.mtxData, matrices, jntCnt * mtxSiz);

    return cmdId;
}

_ARX afxCmdId AsxCmdResetJointLodErrors(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxReal const lodErrors[])
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(ResetJointLodErrors), sizeof(cmd->ResetJointLodErrors) + (jntCnt * sizeof(cmd->ResetJointLodErrors.lodErrors[0])), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(lodErrors);

    cmd->ResetJointLodErrors.skl = skl;
    cmd->ResetJointLodErrors.baseJntIdx = baseJntIdx;
    cmd->ResetJointLodErrors.jntCnt = jntCnt;
    AfxCopy(cmd->ResetJointLodErrors.lodErrors, lodErrors, jntCnt * sizeof(lodErrors[0]));

    return cmdId;
}

_ARX afxCmdId AsxCmdRebuildPose(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, arxPosture plce, afxBool rigid, arxPose pose)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(RebuildPose), sizeof(cmd->RebuildPose), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(pose->artCnt >= jntCnt);
    //AFX_ASSERT(bufIdx);

    cmd->RebuildPose.skl = skl;
    cmd->RebuildPose.plce = plce;
    cmd->RebuildPose.baseJntIdx = baseJntIdx;
    cmd->RebuildPose.jntCnt = jntCnt;
    cmd->RebuildPose.rigid = rigid;
    cmd->RebuildPose.pose = pose;

    if (displace)
        AfxM4dCopy(cmd->RebuildPose.displace, displace);
    else
        AfxM4dReset(cmd->RebuildPose.displace);

    return cmdId;
}

_ARX afxCmdId AsxCmdBuildRestPose(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, arxPose pose)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildRestPose), sizeof(cmd->BuildRestPose), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(pose);

    cmd->BuildRestPose.skl = skl;
    cmd->BuildRestPose.baseJntIdx = baseJntIdx;
    cmd->BuildRestPose.jntCnt = jntCnt;
    cmd->BuildRestPose.pose = pose;
    
    return cmdId;
}

_ARX afxCmdId AsxCmdBuildPlacement(arxContext ctx, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, arxPose pose, afxM4d const displace, afxBool noComposite, arxPosture plce)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildPlacement), sizeof(cmd->BuildPlacement), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJntIdx, jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(plce);

    cmd->BuildPlacement.skl = skl;
    cmd->BuildPlacement.baseJntIdx = baseJntIdx;
    cmd->BuildPlacement.jntCnt = jntCnt;
    cmd->BuildPlacement.baseReqJnt = baseReqJnt;
    cmd->BuildPlacement.reqJntCnt = reqJntCnt;
    cmd->BuildPlacement.pose = pose;
    cmd->BuildPlacement.noComposite = noComposite;
    cmd->BuildPlacement.plce = plce;

    if (displace)
        AfxM4dCopy(cmd->BuildPlacement.displace, displace);
    else
        AfxM4dReset(cmd->BuildPlacement.displace);

    return cmdId;
}

_ARX afxCmdId AsxCmdBuildCompositeMatrices(arxContext ctx, arxSkeleton skl, arxPosture plce, afxUnit baseJnt, afxUnit jntCnt, afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildCompositeMatrices), sizeof(cmd->BuildCompositeMatrices), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), baseJnt, jntCnt);
    AFX_ASSERT(plce);

    cmd->BuildCompositeMatrices.skl = skl;
    cmd->BuildCompositeMatrices.plce = plce;
    cmd->BuildCompositeMatrices.baseJnt = baseJnt;
    cmd->BuildCompositeMatrices.jntCnt = jntCnt;
    cmd->BuildCompositeMatrices.transposed = transposed;
    cmd->BuildCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildCompositeMatrices.offset = offset;

    return cmdId;
}

_ARX afxCmdId AsxCmdBuildIndexedCompositeMatrices(arxContext ctx, arxSkeleton skl, arxPosture plce, afxUnit jntCnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxUnit bufIdx, afxSize offset)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BuildIndexedCompositeMatrices), sizeof(cmd->BuildIndexedCompositeMatrices) + (jntCnt * sizeof(cmd->BuildIndexedCompositeMatrices.jntMap[0])), &cmdId);
    AFX_ASSERT(cmd);

    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(ArxGetSkeletonDepth(skl), 0, jntCnt);
    AFX_ASSERT(plce);

    cmd->BuildIndexedCompositeMatrices.skl = skl;
    cmd->BuildIndexedCompositeMatrices.plce = plce;
    cmd->BuildIndexedCompositeMatrices.jntCnt = jntCnt;
    cmd->BuildIndexedCompositeMatrices.transposed = transposed;
    cmd->BuildIndexedCompositeMatrices.bufIdx = bufIdx;
    cmd->BuildIndexedCompositeMatrices.offset = offset;

    for (afxUnit i = 0; i < jntCnt; i++)
        cmd->BuildIndexedCompositeMatrices.jntMap[i] = jntMap[i];

    return cmdId;
}
