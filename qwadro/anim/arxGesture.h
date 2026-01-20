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

#ifndef ARX_GESTURE_H
#define ARX_GESTURE_H

#include "qwadro/anim/arxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"

typedef enum arxGestureFlag
{
    arxGestureFlag_NONE
} arxGestureFlags;

AFX_DEFINE_STRUCT(arxVectorialGesture)
{
    afxUnit             seqKey;
    afxInt              dimension;
    arxCurve            value;
};

AFX_DEFINE_STRUCT(arxPivotalGesture)
{
    afxMask             flags;
    arxCurve            transmission;
    arxCurve            translation;
    arxCurve            transmutation;
};

AFX_DEFINE_STRUCT(arxGestureBlueprint)
{
    afxString32         id;
    afxUnit             vecCnt;
    afxString const*    vectors;
    afxUnit             pivotCnt;
    afxString const*    pivots;
    afxReal const*      pivotLodError;
    afxBool             incPivotLodError;
    afxTransform        displacement;

    void*               udd;
    afxString           tag;
};

ARX afxError ArxAssembleGestures
(
    arxScenario scio,
    afxUnit cnt,
    arxGestureBlueprint const blueprints[],
    arxGesture gestures[]
);

////////////////////////////////////////////////////////////////////////////////

ARX arxScenario ArxGetGestureHost
(
    arxGesture ges
);

ARX afxMask ArxGetGestureFlags
(
    arxGesture ges
);

ARX afxBool ArxGetGestureId
(
    arxGesture ges, 
    afxString* id
);

ARX void ArxComputeGestureDisplacement
(
    arxGesture ges, 
    afxM4d m
);

ARX afxBool ArxFindGestureVector
(
    arxGesture ges, 
    afxString const* seqId, 
    afxUnit* seqIdx
);

ARX afxBool ArxFindGestureTransform
(
    arxGesture ges, 
    afxString const* seqId, 
    afxUnit* seqIdx
);

ARX void ArxUpdateGestureVectors
(
    arxGesture ges, 
    afxUnit baseSeqIdx, 
    afxUnit seqCnt, 
    arxVectorialGesture const vectors[], 
    afxUnit fetchRate
);

ARX void ArxUpdateGestureTransforms
(
    arxGesture ges, 
    afxUnit baseSeqIdx, 
    afxUnit seqCnt, 
    arxPivotalGesture const transforms[], 
    afxUnit fetchRate
);

#endif//ARX_GESTURE_H
