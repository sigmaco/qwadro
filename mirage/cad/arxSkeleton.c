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
#define _ARX_SKELETON_C
//#define _ARX_PLACEMENT_C
//#define _ARX_POSE_C
#include "../scene/arxIcd.h"

#define _ARX_DEBUG_ECHO_REPARENT_BONES 1

_ARX afxString* _ArxSklGetIsArray(arxSkeleton skl, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return &skl->boneId[base];
}

_ARX afxTransform* _ArxSklGetLtArray(arxSkeleton skl, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return &skl->boneLt[base];
}

_ARX afxM4d* _ArxSklGetIwArray(arxSkeleton skl, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return &skl->boneIw[base];
}

_ARX afxUnit* _ArxSklGetPiArray(arxSkeleton skl, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return &skl->bonePi[base];
}

_ARX afxReal* _ArxSklGetLeArray(arxSkeleton skl, afxUnit base)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return &skl->boneLe[base];
}

_ARXINL afxBool ArxGetSkeletonUrn(arxSkeleton skl, afxString* id)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT(id);
    *id = skl->urn;
    return skl->urn.len;
}

_ARXINL afxUnit ArxGetSkeletonDepth(arxSkeleton skl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    return skl->boneCnt;
}

_ARXINL void ArxDescribeSkeleton(arxSkeleton skl, arxSkeletonInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    *info = (arxSkeletonInfo const) { 0 };
    info->boneCnt = skl->boneCnt;
    info->bones = skl->boneId;
    info->pi = skl->bonePi;
    info->lt = skl->boneLt;
    info->iw = skl->boneIw;
    info->le = skl->boneLe;
}

// SKELETON

_ARX afxUnit ArxGetBoneParents(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, void* indices, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);
    AFX_ASSERT(indices);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    switch (stride)
    {
        case sizeof(afxUnit8) :
        {
            afxUnit8* dst = indices;
            for (afxUnit i = 0; i < boneCnt; i++)
            {
                afxUnit boneIdx = baseBone + i;
                dst[i] = skl->bonePi[boneIdx];
            }
            break;
        }
        case sizeof(afxUnit16) :
        {
            afxUnit16* dst = indices;
            for (afxUnit i = 0; i < boneCnt; i++)
            {
                afxUnit boneIdx = baseBone + i;
                dst[i] = skl->bonePi[boneIdx];
            }
            break;
        }
        case sizeof(afxUnit32) :
        {
            afxUnit32* dst = indices;
            for (afxUnit i = 0; i < boneCnt; i++)
            {
                afxUnit boneIdx = baseBone + i;
                dst[i] = skl->bonePi[boneIdx];
            }
            break;
        }
        default: AfxThrowError(); break;
    }
    return boneCnt;
}

_ARX afxError ArxReplaceBoneParents(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, void const* indices, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);
    //AFX_ASSERT(indices);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    if (!indices) for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        skl->bonePi[boneIdx] = AFX_INVALID_INDEX;
        return err;
    }
    
#if _ARX_DEBUG_ECHO_REPARENT_BONES
    AfxReportMessage("SKL<%p>ReparentBones(boneIdx<%u>, boneCnt<%u>, indices<%p>, stride<%u>)", skl, baseBone, boneCnt, indices, stride);
#endif

    switch (stride)
    {
    case sizeof(afxUnit8):
    {
        afxUnit8 const* src = indices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit parentIdx = src[i];
            AFX_ASSERT_OR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->boneCnt);
            afxUnit boneIdx = baseBone + i;
            skl->bonePi[boneIdx] = parentIdx;
#if _ARX_DEBUG_ECHO_REPARENT_BONES
            AfxReportMessage("    %3u <%.*s> %3u", boneIdx, AfxPushString(&skl->boneId[boneIdx]), parentIdx);
#endif
        }
        break;
    }
    case sizeof(afxUnit16):
    {
        afxUnit16 const* src = indices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit parentIdx = src[i];
            AFX_ASSERT_OR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->boneCnt);
            afxUnit boneIdx = baseBone + i;
            skl->bonePi[boneIdx] = parentIdx;
#if _ARX_DEBUG_ECHO_REPARENT_BONES
            AfxReportMessage("    %3u <%.*s> %3u", boneIdx, AfxPushString(&skl->boneId[boneIdx]), parentIdx);
#endif
        }
        break;
    }
    case sizeof(afxUnit32):
    {
        afxUnit32 const* src = indices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit parentIdx = src[i];
            AFX_ASSERT_OR(parentIdx == AFX_INVALID_INDEX, parentIdx < skl->boneCnt);
            afxUnit boneIdx = baseBone + i;
            skl->bonePi[boneIdx] = parentIdx;
#if _ARX_DEBUG_ECHO_REPARENT_BONES
            AfxReportMessage("    %3u <%.*s> %3u", boneIdx, AfxPushString(&skl->boneId[boneIdx]), parentIdx);
#endif
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ARX afxUnit ArxGetBoneInversors(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, void* matrices, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);
    AFX_ASSERT(matrices);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    switch (stride)
    {
    case 0:
    case sizeof(afxM4d):
    {
        afxM4d* dst = matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxM4dCopy(dst[i], skl->boneIw[boneIdx]);
        }
        break;
    }
    case sizeof(afxM4r) :
    {
        afxM4r* dst = (afxM4r*)matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxV3dCopy(dst[i][0], skl->boneIw[boneIdx][0]);
            AfxV3dCopy(dst[i][1], skl->boneIw[boneIdx][1]);
            AfxV3dCopy(dst[i][2], skl->boneIw[boneIdx][2]);
            AfxV3dCopy(dst[i][3], skl->boneIw[boneIdx][3]);
        }
        break;
    }
    case sizeof(afxM3d) :
    {
        afxM3d* dst = (afxM3d*)matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxM3dCopyM4d(dst[i], skl->boneIw[boneIdx]);
        }
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return boneCnt;
}

_ARX afxError ArxReplaceBoneInversors(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, void const* matrices, afxUnit stride)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    if (!matrices) for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        AfxM4dReset(skl->boneIw[boneIdx]);
        return err;
    }
    
    switch (stride)
    {
    case 0:
    case sizeof(afxM4d) :
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxM4dCopy(skl->boneIw[boneIdx], src[i]);
        }
        break;
    }
    case sizeof(afxM4r) :
    {
        afxM4r const* src = (afxM4r const*)matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxV4dCopyAtv3d(skl->boneIw[boneIdx][0], src[i][0]);
            AfxV4dCopyAtv3d(skl->boneIw[boneIdx][1], src[i][1]);
            AfxV4dCopyAtv3d(skl->boneIw[boneIdx][2], src[i][2]);
            AfxV4dCopyAtv3d(skl->boneIw[boneIdx][3], src[i][3]);
        }
        break;
    }
    case sizeof(afxM3d) :
    {
        afxM3d const* src = (afxM3d const*)matrices;

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxM4dCopyM3d(skl->boneIw[boneIdx], src[i], AFX_V4D_IDENTITY);
        }
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return err;
}

_ARX afxUnit ArxGetBoneTransforms(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, afxTransform transforms[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);
    AFX_ASSERT(transforms);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        AfxCopyTransform(&transforms[i], &skl->boneLt[boneIdx]);
    }
    return boneCnt;
}

_ARX afxError ArxReplaceBoneTransforms(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, afxTransform const transforms[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    if (!transforms)
    {
        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            AfxResetTransform(&skl->boneLt[boneIdx]);
        }
    }
    else for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        AfxCopyTransform(&skl->boneLt[boneIdx], &transforms[i]);
    }
    return err;
}

_ARX afxUnit ArxGetBoneNegligence(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, afxReal lodErrors[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);
    AFX_ASSERT(lodErrors);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        lodErrors[i] = skl->boneLe[boneIdx];
    }
    return boneCnt;
}

_ARX afxError ArxReplaceBoneNegligence(arxSkeleton skl, afxUnit baseBone, afxUnit boneCnt, afxReal const lodErrors[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT_RANGE(skl->boneCnt, baseBone, boneCnt);

    boneCnt = AFX_MIN(boneCnt, skl->boneCnt - baseBone);

    if (!lodErrors)
    {
        for (afxUnit i = 0; i < boneCnt; i++)
        {
            afxUnit boneIdx = baseBone + i;
            skl->boneLe[boneIdx] = -1.0;
        }
    }
    else for (afxUnit i = 0; i < boneCnt; i++)
    {
        afxUnit boneIdx = baseBone + i;
        skl->boneLe[boneIdx] = lodErrors[i];
    }
    return err;
}

_ARX afxUnit ArxFindBones(arxSkeleton skl, afxUnit boneCnt, afxString const ids[], afxUnit indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    AFX_ASSERT(indices);
    AFX_ASSERT(boneCnt);
    AFX_ASSERT(ids);
    afxUnit rslt = 0;
    afxUnit totalBoneCnt = skl->boneCnt;

    for (afxUnit j = 0; j < boneCnt; j++)
    {
        afxUnit jointIdx = AFX_INVALID_INDEX;

        for (afxUnit i = 0; i < totalBoneCnt; i++)
        {
            afxString jointTag = skl->boneId[i];

            if ((!jointTag.len && !ids[j].len) || 
                (jointTag.len && (AfxCompareStrings(&ids[j], 0, TRUE, 1, &jointTag, NIL))))
            {
                jointIdx = i;
                ++rslt;
                break;

            }
        }
        indices[j] = jointIdx;
    }
    return rslt;
}

_ARX afxError _ArxSklDtorCb(arxSkeleton skl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);

    afxObjectStash stashes[] =
    {
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->bonePi[0]),
            .var = (void**)&skl->bonePi
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneLt[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneLt
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneIw[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneIw
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneLe[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneLe
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneId[0]),
            .var = (void**)&skl->boneId
        }
    };

    if (AfxDeallocateInstanceData(skl, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!skl->bonePi);
    AFX_ASSERT(!skl->boneLe);
    AFX_ASSERT(!skl->boneId);
    AFX_ASSERT(!skl->boneLt);
    AFX_ASSERT(!skl->boneIw);

    return err;
}

_ARX afxError _ArxSklCtorCb(arxSkeleton skl, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxString const* urn = !args[1] ? &(afxString const) {0} : AFX_CAST(afxString const*, args[1]) + invokeNo;
    AFX_ASSERT(args[2]);
    arxSkeletonInfo const* skli = AFX_CAST(arxSkeletonInfo const*, args[2]) + invokeNo;

    skl->udd = NIL;

    if (!AfxCatalogStrings(_ArxScioGetModelUrnStringBase(scio), 1, urn, &skl->urn))
    {
        AfxThrowError();
        return err;
    }

    AfxResetBoxes(1, &skl->aabb, 0);

    afxUnit boneCnt = AFX_MAX(1, skli->boneCnt);
    skl->boneCnt = boneCnt;

    afxObjectStash stashes[] =
    {
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->bonePi[0]),
            .var = (void**)&skl->bonePi
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneLt[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneLt
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneIw[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneIw
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneLe[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&skl->boneLe
        },
        {
            .cnt = skl->boneCnt,
            .siz = sizeof(skl->boneId[0]),
            .var = (void**)&skl->boneId
        }
    };

    if (AfxAllocateInstanceData(skl, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        AFX_ASSERT(skl->bonePi);
        AFX_ASSERT(skl->boneLt);
        AFX_ASSERT(skl->boneIw);
        AFX_ASSERT(skl->boneLe);
        AFX_ASSERT(skl->boneId);

        // reset joint parent indices to avoid crash
        ArxReplaceBoneParents(skl, 0, boneCnt, NIL, 0);

        // reset other joint attributes

        ArxReplaceBoneInversors(skl, 0, boneCnt, NIL, 0);
        ArxReplaceBoneTransforms(skl, 0, boneCnt, NIL);
        ArxReplaceBoneNegligence(skl, 0, boneCnt, NIL);
        skl->boneIdd = NIL;
        skl->boneUdd = NIL;

        // reset or catalog joint tags' strings.

        if (!skli->bones) AfxResetStrings(boneCnt, skl->boneId);
        else if (!AfxCatalogStrings(_ArxScioGetPivotTagStringBase(scio), boneCnt, skli->bones, skl->boneId))
            AfxThrowError();

        AFX_ASSERT(skl->boneCnt == boneCnt);

        if (err && AfxDeallocateInstanceData(skl, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    if (!err)
    {
        afxString s;
        ArxGetSkeletonUrn(skl, &s);
        afxUnit boneCnt = skl->boneCnt;
        AfxReportMessage("Modelling skeleton <%.*s> acquired at %p with the %u joints:", AfxPushString(&s), skl, boneCnt);
        
        arxSkeletonInfo skli;
        ArxDescribeSkeleton(skl, &skli);

        for (afxUnit i = 0; i < boneCnt; i++)
        {
            s = skli.bones[i];
            AfxReportMessage("    %3u <%.*s> %3u", i, AfxPushString(&s), skli.pi[i]);
        }
    }
    return err;
}

_ARX afxClassConfig const _ARX_SKL_CLASS_CONFIG =
{
    .fcc = afxFcc_SKL,
    .name = "Skelton",
    .desc = "Modelling Skeleton",
    .fixedSiz = sizeof(AFX_OBJECT(arxSkeleton)),
    .ctor = (void*)_ArxSklCtorCb,
    .dtor = (void*)_ArxSklDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX void ArxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, arxSkeleton skeletons[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(skeletons);

    (void)flags;
    (void)atTol;
    (void)ltTol;

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        arxSkeleton skl = skeletons[mdlIdx];
        if (!skl) continue;        
        AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);

        //AfxTransformSkeletons(ltm, iltm, ltTol, atv, atTol, 1, &skl);
        {
            afxUnit boneCnt = skl->boneCnt;
            AfxAssimilateTransforms(ltm, iltm, atv, boneCnt, skl->boneLt, skl->boneLt);
            AfxAssimilateAtm4d(ltm, iltm, atv, boneCnt, skl->boneIw, skl->boneIw);
        }

    }
}

_ARX afxUnit ArxEnumerateSkeletons(arxScenario scio, afxUnit first, afxUnit cnt, arxSkeleton skeletons[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetSklClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_SKL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)skeletons);
}

_ARX afxError ArxAcquireSkeletons(arxScenario scio, afxUnit cnt, afxString const urns[], arxSkeletonInfo const infos[], arxSkeleton skeletons[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(skeletons);
    AFX_ASSERT(infos);
    AFX_ASSERT(urns);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetSklClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_SKL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)skeletons, (void const*[]) { scio, urns, infos }))
        AfxThrowError();

    return err;
}

_ARXINL afxBool RctxIdentifySklCb(arxSkeleton skl, void *udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKL, 1, &skl);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxSkeleton* skeletons;
        afxUnit foundCnt;
    } *udd2 = udd;

    afxUnit matchedIdx;
    if (AfxCompareStrings(&skl->urn, 0, FALSE, udd2->cnt, &udd2->ids[udd2->foundCnt], &matchedIdx))
    {
        udd2->skeletons[udd2->foundCnt] = skl;
        ++udd2->foundCnt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 1;
    }
    return 0;
}

_ARX afxBool ArxFindSkeletons(arxScenario scio, afxUnit cnt, afxString const ids[], arxSkeleton skeletons[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = _ArxScioGetMdlClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_SKL);
    struct
    {
        afxUnit cnt;
        afxString const* ids;
        arxSkeleton* skeletons;
        afxUnit foundCnt;
    } udd =
    {
        .cnt = cnt,
        .ids = ids,
        .skeletons = skeletons,
        .foundCnt = 0
    };
    AfxInvokeObjects(cls, (void*)RctxIdentifySklCb, &udd, 0, cnt);
    return udd.foundCnt;
}
