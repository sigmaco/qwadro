/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_MODEL_DDK_H
#define ARX_MODEL_DDK_H

#include "arxMeshDDK.h"
#include "arxSkeletonDDK.h"
#include "arxMaterialDDK.h"

AFX_DEFINE_STRUCT(_arxDdiMshl);
AFX_DEFINE_STRUCT(_arxIddMshl);

#ifdef _ARX_MESH_LINKAGE_C
#ifdef _ARX_MESH_LINKAGE_IMPL
AFX_OBJECT(_arxMeshLinkage)
#else
AFX_OBJECT(arxMeshLinkage)
#endif
// This new rig representation allow both model and mesh to access rigging info.
// Access to this info is important to let mesh instantiaion be bundled and to solve vertices' bias index indirection.
// Mesh juncture.
{
    _arxIddMshl*         idd;
    _arxDdiMshl const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    arxMesh         msh;
    arxModel        mdl; // arxModel.rigs;
    afxUnit         biasCnt;
    afxUnit*        biasToJntMap; // [arxMesh.biasCnt] --- to assembled skeleton.
    afxUnit*        biasFromJntMap; // [arxMesh.biasCnt] --- from original skeleton.
    arxSkeleton     origSkl; // original skeleton
    // TODO? Unify to jntMap, and use baseFromIdx and baseToIdx.

    afxUnit         mtlCnt;
    afxUnit*        mtlMap; // [arxMesh.mtlCnt] --- map into global list.
    arxMeshRigFlags flags;
    afxMask         renderMask;
};
#endif//_ARX_MESH_LINKAGE_C

AFX_DEFINE_STRUCT(_arxDdiMdl);
AFX_DEFINE_STRUCT(_arxIddMdl);

#ifdef _ARX_MODEL_C

#ifdef _ARX_MODEL_IMPL
AFX_OBJECT(_arxModel)
#else
AFX_OBJECT(arxModel)
#endif
{
    _arxIddMdl*         idd;
    _arxDdiMdl const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // In the past, skeleton (as arxSkeleton) was separated from model.
    // Mainly to simplify serialization, they were merged.
    // Anyway, there was a second arrangement aimed to optimized reutilization of skeleton,
    // where a arxModel would be a superclass of arxSkeleton. The arxModel would point to itself or other model as a skeleton.
    // Fragments of this approach can be observed in mesh rigs.

    arxSkeleton     skl;
    afxUnit         lodType;
    afxReal         allowedLodErrFadingFactor;
    afxTransform    displace;

    // Should we chain the rigs instead of array it?
    afxUnit         usedRigCnt;
    afxUnit         rigCnt;
    arxMeshLinkage* rigs;
    arxMtd          mtd;
    afxBox          aabb;
    arxModelFlags   flags;
    afxString       urn; // 32
};

#endif

ARX afxClassConfig const _ARX_MDL_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MSHL_CLASS_CONFIG;

#endif//ARX_MODEL_DDK_H
