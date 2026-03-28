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

#ifndef ARX_INSTANCED_ANIMATION_DDK_H
#define ARX_INSTANCED_ANIMATION_DDK_H

#include "arxAnimationDDK.h"

typedef enum arxCachedGestureXformFlag
{
    arxCachedGestureXformFlag_Identity = 0x0,

    arxCachedGestureXformFlag_TranslationIsConstant = 0x1,
    arxCachedGestureXformFlag_TranslationIsKeyframed = 0x2,
    arxCachedGestureXformFlag_TranslationIsGeneral = 0x3,
    arxCachedGestureXformFlag_TranslationMask = 0x3,

    arxCachedGestureXformFlag_TransmissionIsConstant = 0x4,
    arxCachedGestureXformFlag_TransmissionIsKeyframed = 0x8,
    arxCachedGestureXformFlag_TransmissionIsGeneral = 0xC,
    arxCachedGestureXformFlag_TransmissionMask = 0xC,

    arxCachedGestureXformFlag_TransmutationIsConstant = 0x10,
    arxCachedGestureXformFlag_TransmutationIsKeyframed = 0x20,
    arxCachedGestureXformFlag_TransmutationIsGeneral = 0x30,
    arxCachedGestureXformFlag_TransmutationMask = 0x30,
} arxCachedGestureXformFlags;

AFX_DEFINE_STRUCT(arxCachedGestureTransform)
{
    afxUnit          srcTrackIdx; // short arxPivotalGesture
    afxQuatBlend quatMode; // char 0x02
    arxCachedGestureXformFlags flags; // char 0x03
    arxPivotalGesture const* srcTrack;
    void(*sampler)(const arxSampleContext *, const arxPivotalGesture *, arxCachedGestureTransform *, const afxTransform *, const float *, const float *, float *, float *);
    afxReal         lodErr;
    afxTransform    lodTransform;
};

AFX_DEFINE_HANDLE(arxInstancedAnimation);

AFX_DEFINE_STRUCT(arxInstancedAnimationId)
{
    arxAnimation Animation;
    int gesIdx;
    const char *TrackPattern;
    const char *BonePattern;
    arxModel OnModel;
    arxModel FromBasis;
    arxModel ToBasis;
};

AFX_DEFINE_STRUCT(_arxDdiAnik)
{
    int a;
};

AFX_DECLARE_STRUCT(_arxIddAnik);

#ifdef _ARX_INSTANCED_ANIMATION_C
#ifdef _ARX_INSTANCED_ANIMATION_IMPL
AFX_OBJECT(_arxInstancedAnimation)
#else
AFX_OBJECT(arxInstancedAnimation)
#endif
{
    arxInstancedAnimationId ID;

    void *RebasingMemory; // 28
    afxUnit TrackBindingCount; // 32
    arxCachedGestureTransform *TrackBindings; // 36 // one for each skeleton pivot
    arxInstancedAnimation Left;
    arxInstancedAnimation Right;
    int UsedBy;
    arxInstancedAnimation PreviousUnused;
    arxInstancedAnimation NextUnused;


    _arxIddAnik*         idd;
    _arxDdiAnik const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};
#endif//_ARX_INSTANCED_ANIMATION_C

AFX_DEFINE_STRUCT(arxTrackTarget) // draft
{
    arxInstancedAnimation Binding;
    arxInstancedAnimationId BindingID; // size = 28
    arxPuppet OnInstance; // 32
    arxAccumOp AccumulationMode; // 36
    float LODCopyScaler; // 40
    arxTrackMask *TrackMask; // 44
    arxTrackMask *ModelMask; // 48
};

ARX afxClassConfig const _ARX_ANIK_CLASS_CONFIG;

#endif//ARX_INSTANCED_ANIMATION_DDK_H
