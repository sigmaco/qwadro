/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ASX_IMPL___CONTEXT_H
#define ASX_IMPL___CONTEXT_H

#include "qwadro/sim/arxSimulation.h"

// A catalyst is an event or person causing a change.

typedef enum asxContextState
/// Each command buffer is always in one of the following states
{
    asxContextState_INITIAL,
    asxContextState_RECORDING,
    asxContextState_EXECUTABLE,
    asxContextState_PENDING,
    asxContextState_INVALID,

    asxContextState_INTERNAL_EXECUTING,
} asxContextState;

#ifdef _ASX_CONTEXT_C
#ifdef _ASX_CONTEXT_IMPL
AFX_OBJECT(_acxContext)
#else
AFX_OBJECT(arxContext)
#endif
{
    afxArena        cmdArena; // owned by dsys data for specific port
    afxChain        commands;
    asxContextState state;
    afxError        (*endCb)(arxContext);
    afxError        (*resetCb)(arxContext, afxBool freeMem, afxBool permanent);
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.

    afxUnit         portId;
    afxUnit         poolIdx;
    afxBool         disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    struct
    {
        arxPose pose;

        arxModel skl;
        afxReal allowedErr;
        afxReal allowedErrScaler;
        afxReal allowedErrEnd;

        afxM4d displacement;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit baseReqJntIdx;
        afxUnit reqJntCnt;
        afxUnit const* sparseJntMap;
    } pose;

    arxPosture plce;
    afxUnit baseXformIdx;
    afxUnit xformCnt;

};
#endif//_ASX_CONTEXT

AFX_DEFINE_STRUCT(_acxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(asxCmd)
{
    _acxCmdHdr hdr;
    struct
    {
        _acxCmdHdr hdr;

        arxPose src;
        afxUnit from;
        arxPose dst;
        afxUnit base;
        afxUnit cnt;
    } CopyPose;
    struct
    {
        _acxCmdHdr hdr;

        arxPose pose;
        afxV3d translation;
        afxV3d rotation;
    } ApplyRootMotionVectors;
    struct
    {
        _acxCmdHdr hdr;

        afxUnit baseSlot;
        afxUnit cnt;
        struct
        {
            avxBuffer buf;
            afxUnit offset;
            afxUnit range;
        } /*AFX_SIMD*/ items[];
    } BindBuffers;
    struct
    {
        _acxCmdHdr hdr;

        afxUnit     baseIdx;
        afxUnit     cnt;
        struct
        {
            arxCurve cur;
            afxUnit a;
            afxUnit b;
        }       items[];
    } BindCurves;
    struct
    {
        _acxCmdHdr hdr;

        afxUnit slot;
        void* ptr;
        afxUnit siz;
    } BindMemory;
    struct
    {
        _acxCmdHdr hdr;

        arxPuppet bod;
        afxUnit pivotCnt;
        afxM4d displace;
        arxPose scratch;
        arxPosture plce;
        afxReal allowedErr;
    } SamplePuppetAnimationsAccelerated;
    struct
    {
        _acxCmdHdr hdr;

        arxPuppet bod;
        arxPulley moto;
        afxUnit basePivotIdx;
        afxUnit pivotCnt;
        arxPose pose;
        afxReal allowedErr;
        afxUnit const* sparseBoneArray;
    } SampleSinglePuppetAnimation;
    struct
    {
        _acxCmdHdr hdr;

        arxPuppet bod;
        afxUnit basePivotIdx;
        afxUnit pivotCnt;
        arxPose pose;
        afxReal allowedErr;
        afxUnit const* sparseBoneArray;
    } SamplePuppetAnimations;
    struct
    {
        _acxCmdHdr hdr;

        arxModel mdl;
        afxTransform displacement;
    } DisplaceModel;
    struct
    {
        _acxCmdHdr hdr;

        arxModel mdl;
        afxUnit rigIdx;
        arxPosture plce;
        afxUnit cnt;
        afxUnit bufIdx;
        afxSize bufOffset;
    } BuildRiggedMeshCompositeMatrices;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxM4d displace;
        arxPosture plce;
        afxBool rigid;
        arxPose pose;
    } RebuildPose;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit idxSiz;
        afxByte idxData[];
    } ReparentJoints;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxTransform transforms[];
    } ResetJointTransforms;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit mtxSiz;
        afxByte mtxData[];
    } ResetJointInversors;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxReal lodErrors[];
    } ResetJointLodErrors;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        arxPose pose;
    } BuildRestPose;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        afxUnit baseJntIdx;
        afxUnit jntCnt;
        afxUnit baseReqJnt;
        afxUnit reqJntCnt;
        arxPose pose;
        afxM4d displace;
        afxBool noComposite;
        arxPosture plce;
    } BuildPlacement;
    struct
    {
        _acxCmdHdr hdr;

        afxUnit bufIdx;
        afxSize offset;
        arxSkeleton skl;
        arxPosture plce;
        afxUnit baseJnt;
        afxUnit jntCnt;
        afxBool /*3x4*/transposed;
    } BuildCompositeMatrices;
    struct
    {
        _acxCmdHdr hdr;

        arxSkeleton skl;
        arxPosture plce;
        afxUnit jntCnt;
        afxBool /*3x4*/transposed;
        afxUnit bufIdx;
        afxSize offset;
        afxUnit jntMap[];
    } BuildIndexedCompositeMatrices;
};

AFX_DEFINE_UNION(asxCmdList)
{
    struct
    {
        void* End; // end of context

        void* CopyPose;
        void* ApplyRootMotionVectors;
        void* BindMemory;
        void* BindBuffers;
        void* BindCurves;
        void* SamplePuppetAnimationsAccelerated;
        void* SampleSinglePuppetAnimation;
        void* SamplePuppetAnimations;
        void* DisplaceModel;
        void* BuildRiggedMeshCompositeMatrices;
        void* RebuildPose;
        void* ReparentJoints;
        void* ResetJointTransforms;
        void* ResetJointInversors;
        void* ResetJointLodErrors;
        void* BuildRestPose;
        void* BuildPlacement;
        void* BuildCompositeMatrices;
        void* BuildIndexedCompositeMatrices;
    };
    void(*f[])(void*, asxCmd const*);
};

#define ASX_GET_STD_CMD_ID(cmdName_) (offsetof(asxCmdList, cmdName_) / sizeof(void*))

ASX asxCmd* _AsxCtxPushCmd(arxContext, afxUnit id, afxUnit siz, afxCmdId* cmdId);
ASX afxError _AsxCtxResetCb(arxContext, afxBool freeMem, afxBool permanent);
ASX afxError _AsxCtxEndCb(arxContext);

ASX asxContextState _AsxGetCatalystStatus(arxContext ctx);

ASX afxClassConfig const _ASX_CTX_CLASS_CONFIG;

#endif//ASX_IMPL___CONTEXT_H
