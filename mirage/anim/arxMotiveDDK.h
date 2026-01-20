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

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_MOTIVE_DDK_H
#define ARX_MOTIVE_DDK_H

#include "arxInstancedAnimationDDK.h"
#include "arxPulleyDDK.h"

AFX_DECLARE_STRUCT(arxControlledAnimation);
AFX_DECLARE_STRUCT(arxControlledPose);

AFX_DECLARE_STRUCT(arxMotiveCallbacks);

typedef struct controlled_pose
{
    arxPose Attitude; // 0
    const arxTrackMask *ModelMask; // 4
} controlled_pose;

typedef struct controlled_animation
{
    arxInstancedAnimation Binding; // 0
    arxAccumOp AccumulationMode; // 4
    const arxTrackMask *TrackMask; // 8
    const arxTrackMask *ModelMask; // 12
} controlled_animation;

AFX_DECLARE_STRUCT(_arxIddMotv);
AFX_DEFINE_STRUCT(_arxDdiMotv)
{
    void(*PoseAccumulateBindingState)(arxPose, arxMotive, afxUnit, afxUnit, afxReal, afxUnit const*);
    void(*AnimationAccumulateBindingState)(arxPose, arxMotive, afxUnit, afxUnit, afxReal, const afxUnit *);
    void(*AnimationAccumulateLoopTransform)(arxMotive, afxReal, afxReal*, afxV3d, afxV3d, afxBool);
    void(*AnimationBuildDirect)(arxMotive, afxUnit, afxM4d const, arxPosture, afxReal);
    void(*PoseBuildDirect)(arxMotive, afxUnit, afxM4d const, arxPosture);
};

#ifdef _ARX_MOTIVE_C
#ifdef _ARX_MOTIVE_IMPL
AFX_OBJECT(_arxMotive)
#else
AFX_OBJECT(arxMotive)
#endif
{
    arxPulley            moto;
    afxLink          pup;
    arxMotiveCallbacks const *callbacks;
    union
    {
        controlled_animation ca;
        controlled_pose      cp;
    };
    afxBool isAnim;//void*               reservedPtr;

    _arxIddMotv*         idd;
    _arxDdiMotv const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};
#endif//_ARX_MOTIVE_C

AFX_DEFINE_STRUCT(arxMotiveCallbacks)
{
    struct controlled_animation*(*GetControlledAnimation)(arxMotive*);
    struct controlled_pose*(*GetControlledAttitude)(arxMotive*);
    afxBool(*InitializeBindingState)(arxMotive*, void*);
    void(*AccumulateBindingState)(arxMotive*, afxUnit, afxUnit, arxPose*, afxReal, afxUnit const*);
    void(*BuildBindingDirect)(arxMotive*, afxUnit, afxReal const*, arxPosture*, afxReal);
    void(*AccumulateLoopTransform)(arxMotive*, afxReal, afxReal*, afxReal*, afxReal*, afxBool);
    void(*CleanupBindingState)(arxMotive*);
};

ARX afxClassConfig const _ARX_MOTV_CLASS_CONFIG;

#endif//ARX_MOTIVE_DDK_H
