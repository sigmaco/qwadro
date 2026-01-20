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

//#define _ARX_DRAW_C
#define _ARX_MODEL_C
#define _ARX_MESH_LINKAGE_C
//#define _ARX_SKELETON_C
//#define _ARX_PLACEMENT_C
//#define _ARX_POSE_C
#include "../scene/arxIcd.h"

// IMPORTANT:
// Some models are not built from a mesh in the classic sense of the word. 
// Models structured out of Non-uniform rational basis spline (NURBS) also exit. 
// Technically you could build a model in many ways that do not involve a mesh in the classic sense of the word, such as atoms and Voxels. 
// When people speak of a mesh they normally refer to a triangle-based polygon mesh.

_ARX afxError _ArxMshlDtorCb(arxMeshLinkage mshl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSHL, 1, &mshl);

    arxMesh msh = mshl->msh;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    AfxDisposeObjects(1, &msh);

    if (!mshl->origSkl)
    {
        AFX_ASSERT(!(mshl->flags & arxMeshRigFlag_TRANSPLANTED));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &mshl->origSkl);

        if (mshl->flags & arxMeshRigFlag_TRANSPLANTED)
        {
            AFX_ASSERT(mshl->origSkl != mshl->mdl->skl);
            mshl->flags &= ~arxMeshRigFlag_TRANSPLANTED;
            AfxDisposeObjects(1, &mshl->origSkl);
        }
    }

    afxBool transplanted = mshl->origSkl && (mshl->origSkl != mshl->mdl->skl);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mshl->mtlCnt,
            .siz = sizeof(mshl->mtlMap[0]),
            .var = (void**)&mshl->mtlMap
        },
        {
            .cnt = mshl->biasCnt,
            .siz = sizeof(mshl->biasToJntMap[0]),
            .var = (void**)&mshl->biasToJntMap
        },
        {
            // different from biasCnt to allow it to reuse biasToJntMap when not transplanted.
            .cnt = transplanted ? mshl->biasCnt : 0,
            .siz = sizeof(mshl->biasFromJntMap[0]),
            .var = (void**)&mshl->biasFromJntMap
        }
    };

    if (AfxDeallocateInstanceData(mshl, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ARX afxError _ArxMshlCtorCb(arxMeshLinkage mshl, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSHL, 1, &mshl);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxModel mdl = args[1];
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(args[2]);
    arxMeshRigging const* rigi = AFX_CAST(arxMeshRigging const*, args[2]) + invokeNo;
    
    arxSkeletonInfo skli;
    arxSkeleton skl = mdl->skl;
    ArxDescribeSkeleton(skl, &skli);

    arxMesh msh = rigi->msh;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    arxSkeletonInfo origSkli;
    arxSkeleton origSkl = rigi->origSkl;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SKL, 1, &origSkl);
    afxBool transplanted = origSkl && (origSkl != mdl->skl);

    if (transplanted)
        ArxDescribeSkeleton(origSkl, &origSkli);

    arxMeshInfo mshi;
    ArxDescribeMesh(msh, &mshi);

    afxUnit mtlCnt = mshi.mtlCnt;
    afxUnit biasCnt = mshi.biasCnt;

    afxObjectStash stashes[] =
    {
        {
            .cnt = mtlCnt,
            .siz = sizeof(mshl->mtlMap[0]),
            .var = (void**)&mshl->mtlMap
        },
        {
            .cnt = biasCnt,
            .siz = sizeof(mshl->biasToJntMap[0]),
            .var = (void**)&mshl->biasToJntMap
        },
        {
            // different from biasCnt to allow it to reuse biasToJntMap when not transplanted.
            .cnt = transplanted ? biasCnt : 0,
            .siz = sizeof(mshl->biasFromJntMap[0]),
            .var = (void**)&mshl->biasFromJntMap
        }
    };

    if (AfxAllocateInstanceData(mshl, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    if (biasCnt)
    {
        afxString* biasTags = ArxGetMeshBiasTags(msh, 0);
        afxUnit biasMappedCnt = 0;

        if (biasCnt != (biasMappedCnt = ArxFindBones(skl, biasCnt, biasTags, mshl->biasToJntMap)))
        {
            //AfxLogAdvertence("%u biases mapped to joints from %u", biasMappedCnt, biasCnt);
        }

        if (!transplanted) mshl->biasFromJntMap = mshl->biasToJntMap;
        else
        {
            if (biasCnt != (biasMappedCnt = ArxFindBones(origSkl, biasCnt, biasTags, mshl->biasFromJntMap)))
            {
                //AfxLogAdvertence("%u joints to biases mapped from %u", biasMappedCnt, biasCnt);
            }
        }

        // on error, alloc's will be released at end
    }

    if (!err && mshi.mtlCnt)
    {
        afxString* materials = ArxGetMeshMaterials(msh, 0);
        afxUnit mappedMtlCnt = 0;

        if (mshi.mtlCnt != (mappedMtlCnt = ArxFindMaterials(mdl->mtd, mshi.mtlCnt, materials, mshl->mtlMap, NIL)))
        {

        }
    }

    if (!err)
    {
        AfxReacquireObjects(1, &msh);
        mshl->msh = msh;
        mshl->mdl = mdl;
        mshl->origSkl = origSkl;
        mshl->mtlCnt = mtlCnt;
        mshl->biasCnt = biasCnt;
        mshl->flags = NIL;

        if (transplanted)
        {
            AfxReacquireObjects(1, &origSkl);
            mshl->flags |= arxMeshRigFlag_TRANSPLANTED;
        }
    }

    if (err)
    {
        if (AfxDeallocateInstanceData(mshl, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    return err;
}

_ARX afxClassConfig const _ARX_MSHL_CLASS_CONFIG =
{
    .fcc = afxFcc_MSHL,
    .name = "MeshLinkage",
    .desc = "Mesh Linkage",
    .fixedSiz = sizeof(AFX_OBJECT(arxMeshLinkage)),
    .ctor = (void*)_ArxMshlCtorCb,
    .dtor = (void*)_ArxMshlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARXINL afxBool ArxGetModelUrn(arxModel mdl, afxString* id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(id);
    *id = mdl->urn;
    return mdl->urn.len;
}

_ARXINL arxSkeleton ArxGetSkeleton(arxModel mdl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    arxSkeleton skl = mdl->skl;
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return skl;
}

_ARXINL void ArxDescribeModel(arxModel mdl, arxModelInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    *info = (arxModelInfo const) { 0 };
    info->skl = mdl->skl;
    ArxDescribeSkeleton(mdl->skl, &info->skli);
    info->lodType = mdl->lodType;
    info->allowedLodErrFadingFactor = mdl->allowedLodErrFadingFactor;
    info->displace = mdl->displace;
    info->rigCnt = mdl->rigCnt;
    info->riggedMeshCnt = mdl->usedRigCnt;
    info->mtd = mdl->mtd;
}

// DISPLACEMENT

_ARXINL void ArxDisplaceModel(arxModel mdl, afxTransform const* t)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (!t)
        AfxResetTransform(&mdl->displace);
    else
        AfxCopyTransform(&mdl->displace, t);
}

_ARXINL void ArxGetModelDisplacement(arxModel mdl, afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(t);
    AfxCopyTransform(&mdl->displace, t);
}

_ARXINL void ArxComputeModelDisplacement(arxModel mdl, afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mdl->displace, m);
}

// MESH RIGGING

_ARX void ArxDescribeMeshRigs(arxModel mdl, afxUnit baseRigIdx, afxUnit cnt, arxMeshRigInfo info[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, cnt);
    AFX_ASSERT(info);

    // sanitize arguments
    baseRigIdx = AFX_MIN(baseRigIdx, mdl->rigCnt - 1);
    cnt = AFX_MIN(cnt, mdl->rigCnt - baseRigIdx);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit rigIdx = baseRigIdx + i;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
        arxMeshLinkage rig = mdl->rigs[rigIdx];

        if (!rig) *info = (arxMeshRigInfo) { 0 };
        else
        {
            info->flags = rig->flags;
            info->msh = rig->msh;
            info->biasToJntMap = rig->biasToJntMap;
            info->biasFromJntMap = rig->biasFromJntMap;
            info->mtlMap = rig->mtlMap;
            info->origSkl = rig->origSkl;
        }
    }
}

_ARXINL afxBool ArxIsRiggedMeshTransplanted(arxModel mdl, afxUnit rigIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    
    // sanitize arguments
    rigIdx = AFX_MIN(rigIdx, mdl->rigCnt - 1);

    afxBool transplanted = FALSE;
    arxMeshLinkage rig = mdl->rigs[rigIdx];

    if (rig)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MSHL, 1, &rig);
        transplanted = (rig->flags & arxMeshRigFlag_TRANSPLANTED);
    }
    return transplanted;
}

_ARXINL afxUnit ArxGetRiggedMeshes(arxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);
    
    // sanitize arguments
    baseRigIdx = AFX_MIN(baseRigIdx, mdl->rigCnt - 1);
    rigCnt = AFX_MIN(rigCnt, mdl->rigCnt - baseRigIdx);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < rigCnt; i++)
    {
        afxUnit rigIdx = baseRigIdx + i;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

        arxMeshLinkage rig = mdl->rigs[rigIdx];
        if (!rig) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MSHL, 1, &rig);

        arxMesh msh = rig->msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_ARX afxError ArxRigMeshes(arxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, arxMeshRigging const meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);

    arxScenario scio = AfxGetHost(mdl);

    arxSkeletonInfo skli;
    arxSkeleton skl = mdl->skl;
    ArxDescribeSkeleton(skl, &skli);

    for (afxUnit it = 0; it < rigCnt; it++)
    {
        arxMeshRigging const* rigi = meshes ? &meshes[it] : &(arxMeshRigging const) { 0 };

        arxSkeletonInfo origSkli;
        arxSkeleton origSkl = rigi->origSkl;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_SKL, 1, &origSkl);
        afxBool transplanted = origSkl && (origSkl != mdl->skl);

        if (transplanted)
            ArxDescribeSkeleton(origSkl, &origSkli);

        afxUnit rigIdx = baseRigIdx + it;
        arxMeshLinkage rig = mdl->rigs[rigIdx];
        arxMesh msh = rigi->msh;
        arxMesh mshCurr = NIL;

        if (rig)
        {
            mshCurr = rig->msh;

            if (msh == mshCurr)
                continue;
        }
        else if (!msh)
            continue;

        if (mshCurr) // detach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &mshCurr);
            AfxDisposeObjects(1, &rig);
            --mdl->usedRigCnt;
        }

        if (msh) // attach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            afxClass* mshlCls = (afxClass*)_ArxScioGetMshlClass(scio);
            AFX_ASSERT_CLASS(mshlCls, afxFcc_MSHL);

            if (AfxAcquireObjects(mshlCls, 1, (afxObject*)&rig, (void const*[]) { scio, mdl, (void*)rigi }))
                AfxThrowError();
            else
            {
                mdl->rigs[rigIdx] = rig;
                ++mdl->usedRigCnt;

                // SOME DEBUG LOGGING

                afxString s;
                ArxGetMeshUrn(msh, &s);
                AfxReportMessage("+-- rig #%u", rigIdx);
                AfxReportMessage("|   +-- msh <%.*s> %p", AfxPushString(&s), msh);
                AfxReportMessage("|   +-- biases-from/to-joints");
                for (afxUnit i = 0; i < rig->biasCnt; i++)
                {
                    s = *ArxGetMeshBiasTags(msh, i);

                    if (transplanted)
                        AfxReportMessage("|   |   +-- <%.*s> @ <%u#%.*s>(%u#'%.*s')", AfxPushString(&s), rig->biasFromJntMap[i], AfxPushString(_ArxSklGetIsArray(origSkl, rig->biasFromJntMap[i])), rig->biasToJntMap[i], AfxPushString(_ArxSklGetIsArray(skl, rig->biasToJntMap[i])));
                    else
                        AfxReportMessage("|   |   +-- <%.*s> @ %u#'%.*s'", AfxPushString(&s), rig->biasToJntMap[i], AfxPushString(_ArxSklGetIsArray(skl, rig->biasToJntMap[i])));
                }
            }
        }

        ++rigIdx;

        if (err)
        {
            // Pop previous successful mesh linkages

            for (afxUnit j = it; j-- > 0;)
            {
                AfxDisposeObjects(1, &mdl->rigs[j]);
                --mdl->usedRigCnt;
            }
            break;
        }
    }
    return err;
}

_ARXINL void ArxComputeRiggedMeshMatrices(arxModel mdl, afxUnit rigIdx, arxPosture plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[])
{
    // Should be compatible with BuildMeshBinding4x4Array(rig, wp, baseJnt, jntCnt, matrix4x4Array)

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AFX_MIN(rigIdx, mdl->rigCnt - 1);

    arxMeshLinkage rig = mdl->rigs[rigIdx];
    if (!rig) return;

    arxMesh msh = rig->msh;
    if (!msh) return;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    arxMeshInfo mshi;
    ArxDescribeMesh(msh, &mshi);

    AFX_ASSERT_RANGE(mshi.biasCnt, baseBiasIdx, biasCnt);
    // sanitize arguments
    baseBiasIdx = AFX_MIN(baseBiasIdx, mshi.biasCnt - 1);
    biasCnt = AFX_MIN(biasCnt, mshi.biasCnt - baseBiasIdx);

    afxM4d* w = _ArxPlceGetMmArray(plce, 0);
    afxM4d* iw = _ArxSklGetIwArray(rig->origSkl, 0);
    afxUnit const* biasToJntMap = rig->biasToJntMap;
    afxUnit const* biasFromJntMap = rig->biasFromJntMap;

    for (afxUnit i = 0; i < biasCnt; i++)
    {
        afxUnit biasIdx = baseBiasIdx + i;
        AfxM4dMultiplyAtm(matrices[i], iw[biasFromJntMap[i]], w[biasToJntMap[biasIdx]]);
    }
}

// SKELETON

_ARXINL void ArxQueryModelErrorTolerance(arxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    
    if (allowedErr > 0.0)
    {
        afxReal s = mdl->allowedLodErrFadingFactor * allowedErr;
        *allowedErrEnd = s;
        *allowedErrScaler = 1.0 / (allowedErr - s);
    }
    else
    {
        *allowedErrEnd = 0.0;
        *allowedErrScaler = 0.0;
    }
}

_ARX afxUnit ArxCountJoints(arxModel mdl, afxReal allowedErr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit rslt = 0;

    arxSkeletonInfo skli;
    ArxDescribeSkeleton(mdl->skl, &skli);

    if (allowedErr == 0.0 || !mdl->lodType) rslt = skli.boneCnt;
    else
    {
        afxReal errEnd, errScaler;
        ArxQueryModelErrorTolerance(mdl, allowedErr, &errEnd, &errScaler);

        afxUnit boneCnt = skli.boneCnt;
        afxReal const* lodError = skli.le;

        if (boneCnt >= 4)
        {
            while (lodError[rslt + 0] >= errEnd)
            {
                if (lodError[rslt + 1] < errEnd)
                {
                    ++rslt;
                    break;
                }

                if (lodError[rslt + 2] < errEnd)
                {
                    rslt += 2;
                    break;
                }

                if (lodError[rslt + 3] < errEnd)
                {
                    rslt += 3;
                    break;
                }

                rslt += 4;

                if (rslt >= boneCnt - 3)
                    break;
            }
        }

        for (; rslt < boneCnt; rslt++)
            if (lodError[rslt] < errEnd)
                break;
    }
    return rslt;
}

_ARX void ArxBuildRiggedMeshCompositeMatrices(arxModel mdl, afxUnit rigIdx, arxPosture plce, afxUnit cnt, afxM4d matrices[])
{
    // parametrized wrapper
    // void AfxSklBuildIndexedCompositeBuffer(arxSkeleton skl, arxPosture const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
    // AfxSklBuildIndexedCompositeBuffer(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    afxUnit totalJntCnt = ArxCountJoints(mdl, 0);
    AFX_ASSERT(totalJntCnt >= cnt);

    AFX_ASSERT(plce);
    afxUnit plceCap = ArxGetPostureCapacity(plce);
    AFX_ASSERT(plceCap >= cnt);
    AFX_ASSERT(matrices);

    arxMeshLinkage rig = mdl->rigs[rigIdx];
    if (!rig) return;
    arxMesh msh = rig->msh;

    if (!msh) return;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit const* biasToJntMap = rig->biasToJntMap;
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit jntIdx = biasToJntMap[i];
        AFX_ASSERT_RANGE(mdl->jntCnt, jntIdx, 1);
        AFX_ASSERT_RANGE(plce->matCnt, jntIdx, 1);
        BuildSingleCompositeFromWorldPose_Generic(mdl->jntIw[jntIdx], plce->world[jntIdx], matrices[i]);
    }
#else
    ArxProduceIndexedCompositeMatrices(plce, mdl->skl, cnt, biasToJntMap, FALSE, matrices);
#endif
}


_ARX afxError _ArxMdlDtorCb(arxModel mdl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (mdl->rigCnt)
        ArxRigMeshes(mdl, 0, mdl->rigCnt, NIL);

    AFX_ASSERT(mdl->usedRigCnt == 0);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdl->rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!mdl->rigs);

    arxMtd mtd = mdl->mtd;

    if (mtd)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
        AfxDisposeObjects(1, &mtd);
    }

    arxSkeleton skl = mdl->skl;
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AfxDisposeObjects(1, &skl);

    return err;
}

_ARX afxError _ArxMdlCtorCb(arxModel mdl, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxString const* urn = !args[1] ? &(afxString const) { 0 } : AFX_CAST(afxString const*, args[1]) + invokeNo;
    AFX_ASSERT(args[2]);
    arxModelBlueprint const* mdlb = AFX_CAST(arxModelBlueprint const*, args[2]) + invokeNo;

    afxTransform const* displacement = &mdlb->displace;

    if (!displacement->flags)
        AfxResetTransform(&mdl->displace);
    else
        AfxCopyTransform(&mdl->displace, displacement);

    AfxResetBoxes(1, &mdl->aabb, 0);

    afxUnit rigCnt = AFX_MAX(1, mdlb->rigCnt);
    
    mdl->udd = NIL;

    mdl->rigCnt = 0;
    mdl->rigs = NIL;

    afxUnit boneCnt = AFX_MAX(1, mdlb->boneCnt);
    afxUnit lodType = mdlb->lodType;
    mdl->lodType = lodType;

    mdl->allowedLodErrFadingFactor = /*_ArxScioGetAllowedLodErrorFadingFactor(scio);*/ 0.80000001;

    afxObjectStash stashes[] =
    {
        {
            .cnt = rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxAllocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(mdl->rigs);

    if (!AfxCatalogStrings(_ArxScioGetModelUrnStringBase(scio), 1, urn, &mdl->urn))
    {
        AfxThrowError();
    }

    if (!err)
    {
        arxSkeleton skl = mdlb->skl;

        if (skl)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
            AfxReacquireObjects(1, &skl);
        }
        else
        {
            arxSkeletonInfo skli = { 0 };
            skli.boneCnt = boneCnt;
            skli.bones = mdlb->bones;

            if (ArxAcquireSkeletons(scio, 1, &mdl->urn, &skli, &skl))
                AfxThrowError();
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
            }
        }

        if (!err)
        {
            mdl->skl = skl;

            AfxZero(mdl->rigs, rigCnt * sizeof(mdl->rigs[0]));
            mdl->rigCnt = rigCnt;

            AFX_ASSERT(mdl->rigCnt == rigCnt);

            arxMtd mtd = mdlb->mtd;
            
            if (mtd)
            {
                AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
                AfxReacquireObjects(1, &mtd);
            }
            else
            {
                if (ArxAcquireMtds(scio, 1, &mdl->urn, NIL, &mtd))
                    AfxThrowError();
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
                }
            }

            if (!err)
            {
                mdl->mtd = mtd;

                if (err)
                    AfxDisposeObjects(1, &mtd);
            }

            if (err)
                AfxDisposeObjects(1, &skl);
        }
    }

    if (err)
    {
        if (AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }
    else
    {
        afxString s;
        ArxGetModelUrn(mdl, &s);
        afxUnit boneCnt = ArxCountJoints(mdl, 0);
        AfxReportMessage("Skeletal model <%.*s> assembled at %p. %u mesh rigs for %u joints.", AfxPushString(&s), mdl, mdl->rigCnt, boneCnt);
    }
    return err;
}

_ARX afxClassConfig const _ARX_MDL_CLASS_CONFIG =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .desc = "Animable Skeletal Model",
    .fixedSiz = sizeof(AFX_OBJECT(arxModel)),
    .ctor = (void*)_ArxMdlCtorCb,
    .dtor = (void*)_ArxMdlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX void ArxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(models);

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxModel mdl = models[mdlIdx];
        if (!mdl) continue;        
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

        ArxTransformSkeletons(ltm, iltm, ltTol, atv, atTol, NIL, 1, &mdli.skl);
        
        arxMesh meshes[64];
        afxUnit batchSiz = 64;
        afxUnit totalMshCnt = mdli.rigCnt;
        afxUnit batches = totalMshCnt / batchSiz;
        afxUnit remaining = totalMshCnt % batchSiz;
        afxUnit baseSlotIdx = 0;
        afxUnit mshCnt = 0;

        for (afxUnit j = 0; j < batches; j++)
        {
            mshCnt = ArxGetRiggedMeshes(mdl, baseSlotIdx, batchSiz, meshes);
            baseSlotIdx += mshCnt;

            // WARNING: What to do if mesh is shared among other models of strange asset?
            ArxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        if (remaining)
        {
            mshCnt = ArxGetRiggedMeshes(mdl, baseSlotIdx, remaining, meshes);
            // WARNING: What to do if mesh is shared among other models of strange asset?
            ArxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl->displace, &mdl->displace);
    }
}

_ARX afxUnit ArxEnumerateModels(arxScenario scio, afxUnit first, afxUnit cnt, arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetMdlClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)models);
}

_ARX afxError ArxAssembleModels(arxScenario scio, afxUnit cnt, afxString const urns[], arxModelBlueprint const blueprints[], arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(models);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetMdlClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)models, (void const*[]) { scio, urns, blueprints }))
        AfxThrowError();

    return err;
}

_ARXINL afxBool RctxIdentifyMdlCb(arxModel mdl, void *udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxModel* models;
        afxUnit foundCnt;
    } *udd2 = udd;

    afxUnit matchedIdx;
    if (AfxCompareStrings(&mdl->urn, 0, FALSE, udd2->cnt, &udd2->ids[udd2->foundCnt], &matchedIdx))
    {
        udd2->models[udd2->foundCnt] = mdl;
        ++udd2->foundCnt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 1;
    }
    return 0;
}

_ARX afxBool ArxFindModels(arxScenario scio, afxUnit cnt, afxString const ids[], arxModel models[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetMdlClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxModel* models;
        afxUnit foundCnt;
    } udd =
    {
        .cnt = cnt,
        .ids = ids,
        .models = models,
        .foundCnt = 0
    };
    AfxInvokeObjects(cls, (void*)RctxIdentifyMdlCb, &udd, 0, cnt);
    return udd.foundCnt;
}
