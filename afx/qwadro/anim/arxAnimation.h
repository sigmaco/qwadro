#pragma once
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

#ifndef ARX_ANIMATION_H
#define ARX_ANIMATION_H

// Qwadro armazena animações em partes baseadas em quão muitos modelos estão envolvidos numa animação.
// Assim sendo, uma arxAnimation é a moção de um conjunto de arxModel's animando sobre tempo.
// A arxAnimation contém um ou mais arxGesture's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
// O arxGesture é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no akxSkeleton do correspondente arxModel.

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/anim/arxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/anim/arxGesture.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/anim/arxPuppet.h"
#include "qwadro/anim/arxPulley.h"

typedef enum arxAnimationFlag
{
    arxAnimationFlag_NONE
} arxAnimationFlags;

AFX_DEFINE_STRUCT(arxAnimationBlueprint)
{
    afxReal         dur;
    afxReal         timeStep;
    afxReal         oversampling;
    afxUnit         gesSlotCnt;
    arxGesture*     gestures;
    // TODO make gestures' names a animation property to ease portability of gestures.
    afxString32     id;

    void*           udd;
    afxString       tag;
};

ARX afxError ArxAssembleAnimations
(
    arxScenario scio,
    afxUnit cnt,
    arxAnimationBlueprint const blueprints[],
    arxAnimation animations[]
);

ARX afxError ArxLoadAnimation
(
    arxScenario scio,
    afxString const* urn,
    afxUri const* uri,
    arxAnimation* animation
);

ARX void ArxTransformAnimations
(
    afxM3d const ltm,
    afxM3d const iltm,
    afxReal linearTol,
    afxV4d const atv,
    afxReal affineTol,
    afxFlags flags,
    afxUnit cnt,
    arxAnimation animations[]
);

////////////////////////////////////////////////////////////////////////////////

ARX arxScenario ArxGetAnimationHost
(
    arxAnimation ani
);

ARX afxBool ArxGetAnimationUrn
(
    arxAnimation ani, 
    afxString* id
);

ARX afxBool ArxFindGesture
(
    arxAnimation ani, 
    afxString const* id, 
    afxUnit *gesIdx
);

ARX afxUnit ArxGetGestures
(
    arxAnimation ani, 
    afxUnit first, 
    afxUnit cnt, 
    arxGesture gestures[]
);

ARX afxError ArxRelinkGestures
(
    arxAnimation ani, 
    afxUnit baseSlot, 
    afxUnit slotCnt, 
    arxGesture gestures[]
);

ARX afxUnit ArxPerformAnimation
(
    arxAnimation ani, 
    afxReal startTime, 
    afxUnit iterCnt, 
    afxUnit cnt, 
    afxString const gestures[],
    arxPuppet puppets[]
);

ARX afxError ArxArchiveAnimation
(
    arxAnimation ani, 
    afxUri const* uri
);

#endif//ARX_ANIMATION_H
