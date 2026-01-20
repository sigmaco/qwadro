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

#ifndef ARX_PUPPET_DDK_H
#define ARX_PUPPET_DDK_H

#include "qwadro/scene/arxScenario.h"

/*
    Body
    Animation
        Linked Motion
            Motion
            Skeleton
*/

AFX_DEFINE_STRUCT(_arxPuplAcq)
{
    arxPuppet pup;
    arxModel mdl;
    arxModelInfo mdli;
    afxUnit partToRigCnt;
    afxUnit rigIdxCnt;
    afxUnit const* rigIdxLut;
};

#ifdef _ARX_PUPPET_C
#ifdef _ARX_PUPPET_IMPL
AFX_OBJECT(_arxPuppetLink)
#else
AFX_OBJECT(arxPuppetLink)
#endif
{
    // based on RpAtomic
    // one per mesh rigged to model
#if 0
    // In RW, RpAtomic must be individual because it is bound directly to the DAG as a generic discrete object.
    arxNodular          nodr;
#endif
    // In RW, there is one RpAtomic per mesh, which is a dynamic object storing the state of a RpGeometry.
    // RpGeometry would be a arxMesh. All RpAtomic's collectively would be a arxPuppet.
    afxUnit             rigIdx;

    // based on RpInterpolator
    arxMeshMorphing     morph;

    // In RW, there are two bounding spheres for interpolated geometry, one in object space and one in world space.
    afxBox              bb; // can change when morphing.
    afxSphere           bs; // can change when morphing.

    afxFlags            flags;
    afxMask             mask;

    // In RW, RpAtomic has a RwResEntry caching the dynamic state of a RpAtomic; RpGeometry has its own for static data.
    arxRenderableCache  cache;


    /*
        RpAtomic have a render callback, a render frame counter, and a render pipeline.

        RpAtomic have a link to only one RpClump nesting it in a group of related RpAtomic's, RpLight's and RwCamera's.
        As RpClump chains RpAtomic, RpLight and RwCamera, which are DAG entities, RpClump is considered a dynamic entity.
        RpClump is also used for providing callback function for frustum culling.
    */
};
#endif

AFX_DEFINE_STRUCT(_arxPupAcq)
{
    afxUnit partCnt;
    arxModel mdl;
    arxModelInfo mdli;
    afxUnit partToRigCnt;
    afxUnit rigIdxCnt;
    afxUnit const* rigIdxLut;
};

AFX_DEFINE_STRUCT(_arxDdiPup)
{
    void(*stepCb)(arxPuppet, afxReal time);
    void(*purgeTermMotvCb)(arxPuppet);
    void(*recenterMotvClocksCb)(arxPuppet, afxReal currClock);
    void(*compMotVecCb)(arxPuppet, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);
    void(*accumAnimsCb)(arxPose rslt, arxPuppet pup, afxUnit basePivotIdx, afxUnit pivotCnt, afxReal allowedErr, afxUnit const sparseJntMap[]);

    // temporarily here to advance works.
    afxUnit(*performManedPose)(arxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, arxTrackMask* modelMask, afxUnit cnt, arxPuppet puppets[]);
};

AFX_DECLARE_STRUCT(_arxIddPup);

#ifdef _ARX_PUPPET_C
#ifdef _ARX_PUPPET_IMPL
AFX_OBJECT(_arxPuppet)
#else
AFX_OBJECT(arxPuppet)
#endif
{
    _arxDdiPup const*   ddi;
    _arxIddPup*         idd;
    void*               udd;
    afxString           tag;

    arxModel            mdl;
    afxUnit             partCnt;
    arxPuppetLink*      parts;
    afxChain            motives;
    void*               userData[4];
    afxUnit32           reserved0,
                        reserved1;

    // based on RpAtomic
    // In RW, RpAtomic must be individual because it is bound directly to the DAG as a generic discrete object.
    arxNodular          nodu;
    // Noudular has been moved from part to here due to difficults with animation framework. 
    // It does not allow more than one displacement, for example.
    // Also the animation baking function takes an entire puppet.
    
    arxPose             pose;
    arxPosture        placement;
};
#endif//_ARX_PUPPET_C


ARX afxClassConfig const _ARX_PUP_CLASS_CONFIG;
ARX afxClassConfig const _ARX_PUPL_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MOTV_CLASS_CONFIG;
ARX afxClassConfig const _ARX_ANIK_CLASS_CONFIG;

#endif//ARX_PUPPET_DDK_H
