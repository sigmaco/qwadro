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
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_MOTION_H
#define AMX_MOTION_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxCurve.h"
#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/base/afxFixedString.h"

typedef enum afxMotionFlag
{
    afxMotionFlag_NONE
} afxMotionFlags;

AFX_DEFINE_STRUCT(afxVectorialMotion)
{
    afxUnit             seqKey;
    afxInt              dimension;
    afxCurve            value;
};

AFX_DEFINE_STRUCT(afxPivotalMotion)
{
    afxMask             flags;
    afxCurve            transmission;
    afxCurve            translation;
    afxCurve            transmutation;
};

AFX_DEFINE_STRUCT(afxMotionBlueprint)
{
    afxString32         id;
    afxUnit             vecCnt;
    afxString const*    vectors;
    afxUnit             pivotCnt;
    afxString const*    pivots;
    afxReal const*      pivotLodError;
    afxBool             incPivotLodError;
    afxTransform        displacement;
};

AMX afxMask     AfxGetMotionFlags(afxMotion mot);

AMX afxBool     AfxGetMotionId(afxMotion mot, afxString* id);

AMX void        AfxComputeMotionDisplacement(afxMotion mot, afxM4d m);

AMX afxBool     AfxFindMotionVector(afxMotion mot, afxString const* seqId, afxUnit *seqIdx);
AMX afxBool     AfxFindMotionTransform(afxMotion mot, afxString const* seqId, afxUnit *seqIdx);

AMX void        AfxUpdateMotionVectors(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxVectorialMotion const vectors[], afxUnit fetchRate);
AMX void        AfxUpdateMotionTransforms(afxMotion mot, afxUnit baseSeqIdx, afxUnit seqCnt, afxPivotalMotion const transforms[], afxUnit fetchRate);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxAssembleMotions(afxSimulation sim, afxUnit cnt, afxMotionBlueprint const blueprints[], afxMotion motions[]);

#endif//AMX_MOTION_H
