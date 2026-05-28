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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_SKELETON_DDK_H
#define ARX_SKELETON_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DEFINE_STRUCT(_arxSklDdi);
AFX_DEFINE_STRUCT(_arxSklIdd);

#ifdef _ARX_SKELETON_C

#ifdef _ARX_SKELETON_IMPL
AFX_OBJECT(_arxSkeleton)
#else
AFX_OBJECT(arxSkeleton)
#endif
{
    _arxSklDdi const*   ddi;
    _arxSklIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    // In the past, skeleton (as arxSkeleton) was separated from model.
    // Mainly to simplify serialization, they were merged.
    // Anyway, there was a second arrangement aimed to optimized reutilization of skeleton,
    // where a arxModel would be a superclass of arxSkeleton. The arxModel would point to itself or other model as a skeleton.
    // Fragments of this approach can be observed in mesh rigs.

    afxUnit         boneCnt;
    // array of per-joint parent indexes
    afxUnit*        bonePi; // [jntCnt]
    // array of per-joint local transforms
    afxTransform*   boneLt; // [jntCnt]
    // array of per-joint inverse world matrices
    afxM4d*         boneIw; // [jntCnt]
    // array of per-joint LOD error values
    afxReal*        boneLe; // [jntCnt]
    // array of per-joint identier strings.
    afxString*      boneId; // [jntCnt]
    // array of per-joint IDD
    void**          boneIdd; // [jntCnt]
    // array of per-joint UDD
    void**          boneUdd; // [jntCnt]

    afxBox          aabb;
    arxModelFlags   flags;
    afxString       urn; // 32
};

#endif

ARX afxClassConfig const _ARX_SKL_CLASS_CONFIG;

ARX afxString* _ArxSklGetIsArray(arxSkeleton skl, afxUnit base);
ARX afxTransform* _ArxSklGetLtArray(arxSkeleton skl, afxUnit base);
ARX afxM4d* _ArxSklGetIwArray(arxSkeleton skl, afxUnit base);
ARX afxUnit* _ArxSklGetPiArray(arxSkeleton skl, afxUnit base);
ARX afxReal* _ArxSklGetLeArray(arxSkeleton skl, afxUnit base);

#endif//ARX_SKELETON_DDK_H
