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

#ifndef ARX_ANIMATION_DDK_H
#define ARX_ANIMATION_DDK_H

#include "arxGestureDDK.h"

AFX_DEFINE_STRUCT(_arxDdiAni)
{
    afxBool(*findGestureCb)(arxAnimation, afxString const* id, afxUnit* gesIdx);
    afxError(*relinkCb)(arxAnimation, afxUnit baseSlot, afxUnit slotCnt, arxGesture[]);
    afxUnit(*performCb)(arxAnimation, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxString const gestures[], arxPuppet[]);
    //afxUnit(*performInstCb)(arxAnimation, afxReal startTime, afxUnit iterCnt, arxInstancedAnimation, afxUnit cnt, afxString const gestures[], arxPuppet[]);
};

AFX_DECLARE_STRUCT(_arxIddAni);

#ifdef _ARX_ANIMATION_C

AFX_DEFINE_STRUCT(arxLinkedGesture)
{
    arxGesture           ges;
    /*
    // --- identifier
    arxAnimation Animation;
    int SourceTrackGroupIndex;
    const char *TrackPattern;
    const char *BonePattern;
    const arxModel *OnModel;
    arxModel *FromBasis;
    arxModel *ToBasis;

    // ... binding
    void *RebasingMemory;
    int trackCnt;
    arxCachedGestureTransform *tracks;
    */


};

#ifdef _ARX_ANIMATION_IMPL
AFX_OBJECT(_arxAnimation)
#else
AFX_OBJECT(arxAnimation)
#endif
{
    arxAnimationFlags   flags;
    afxUnit             gesSlotCnt;
    arxLinkedGesture*    gesSlots;

    // @timeStep é o intervalo após oversampling.
    // Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma animação foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    // Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    // Tanto @timeStep quanto @oversampling não são usadas pelo Qwadro.
    // Estão aqui de forma informativa preservando conhecimento sobre a intenção original do autor.
    afxReal             dur; // especifica quão demorado, em segundos, a animação leva para executar a completação.
    afxReal             timeStep; // especifica quão muitos segundos há entre cada frame.
    afxReal             oversampling; // 1.f

    afxString           urn;

    _arxIddAni*         idd;
    _arxDdiAni const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};
#endif

ARX afxClassConfig const _ARX_ANI_CLASS_CONFIG;

#endif//ARX_ANIMATION_DDK_H
