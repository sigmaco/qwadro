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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_POSE_DDK_H
#define ARX_POSE_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DEFINE_STRUCT(_arxPoseArticle)
{
    afxReal         weight;
    afxUnit         cnt;
    afxTransform    xform;

    // The traversalId field in the _arxPoseArticle struct is indeed being used to track the current update state or 
    // traversal state of the articulations within a pose, and it may not necessarily match the traversalId of the arxPose 
    // object itself during different updates.
    afxUnit         traversalId;
};

AFX_DEFINE_STRUCT(_arxPoseDdi);
AFX_DEFINE_STRUCT(_arxPoseIdd);

/*
    The arxPose object is represented as a structured object in the system, encapsulating various data related to the pose, 
    including articulation information, user-defined data, and debugging information. This structure is part of an animation 
    or procedural system, where the pose represents the configuration of joints or bones at a given point in time.
*/

#ifdef _ARX_POSE_C
#ifdef _ARX_POSE_IMPL
AFX_OBJECT(_arxPose)
#else
AFX_OBJECT(arxPose)
#endif
{
    _arxPoseDdi const*  ddi;
    _arxPoseIdd*        idd;
    // A tag for debugging or identifying the pose.
    afxString           tag;
    // User-defined data, custom info for the pose.
    void*               udd;

    // Number of articulations (joints/bones) in the pose.
    afxUnit             artCnt;
    // Array of articulations (each representing a joint's transformation).
    _arxPoseArticle*    arts;
    // Threshold value for "filling" or "completeness" (likely for pose blending).
    afxReal             fillThreshold;
    // ID used to track traversal of articulations or pose updates.
    afxUnit             traversalId;
};
#endif//_ARX_POSE_C

AFX_DEFINE_STRUCT(_arxPlceDdi);
AFX_DEFINE_STRUCT(_arxPlceIdd);

#ifdef _ARX_PLACEMENT_C
#ifdef _ARX_PLACEMENT_IMPL
AFX_OBJECT(_arxPlacement)
#else
AFX_OBJECT(arxPosture)
#endif
{
    _arxPlceDdi const*  ddi;
    _arxPlceIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;
    afxUnit             matCnt;
    // The model matrix. The current accumulated pose matrix.
    afxM4d*             mm;
    // The delta array store the delta from a joint's rest pose to the world matrix.
    afxM4d*             delta;
};
#endif//_ARX_PLACEMENT_C

ARX afxClassConfig const _ARX_POSE_CLASS_CONFIG;
ARX afxClassConfig const _ARX_PLCE_CLASS_CONFIG;

// pose
ARX _arxPoseArticle* _ArxPoseGetPaArray(arxPose pose, afxUnit base);
// placement
ARX afxM4d* _ArxPlceGetMmArray(arxPosture plce, afxUnit base);
ARX afxM4d* _ArxPlceGetDeltaArray(arxPosture plce, afxUnit base);

#endif//ARX_POSE_DDK_H
