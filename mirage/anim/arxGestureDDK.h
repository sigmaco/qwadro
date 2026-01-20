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

#ifndef ARX_GESTURE_DDK_H
#define ARX_GESTURE_DDK_H

//#include "afxSystemDDK.h"
#include "qwadro/scene/arxScenario.h"
#include "qwadro/sim/arxSimulation.h"

typedef enum arxTrackSampler
{
    arxTrackSampler_UUU, // uniform position, orientation and scale/shear.
    arxTrackSampler_III, // identity position, orientation and scale/shear.
    arxTrackSampler_CII, // constant position, identity orientation and scale/shear.
    arxTrackSampler_CCI, // constant position and orientation, identity scale/shear.
    arxTrackSampler_CAI, // constant position, accumulative orientation, identity scale/shear.
    arxTrackSampler_AII, // accumulative position, identity orientation and scale/shear.
    arxTrackSampler_ACI, // accumulative position, constant orientation, identity scale/shear.
    arxTrackSampler_AAI, // accumulative position and orientation, identity scale/shear.
    arxTrackSampler_IIU, // identity position, identity orientation, uniform scale/shear
    arxTrackSampler_SSS // static position, orientation and scale/shear.
} arxTrackSampler;

AFX_DEFINE_STRUCT(_arxDdiGes)
{
    afxBool(*findMatCb)(arxGesture, afxString const* seqId, afxUnit *seqIdx);
    afxBool(*findVecCb)(arxGesture, afxString const* seqId, afxUnit *seqIdx);
    void(*updMatCb)(arxGesture, afxUnit baseSeqIdx, afxUnit seqCnt, arxPivotalGesture const[], afxUnit fetchRate);
    void(*updVecCb)(arxGesture, afxUnit baseSeqIdx, afxUnit seqCnt, arxVectorialGesture const[], afxUnit fetchRate);
};

AFX_DECLARE_STRUCT(_arxIddGes);

#ifdef _ARX_GESTURE_C

AFX_DEFINE_STRUCT(akxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

#ifdef _ARX_GESTURE_IMPL
AFX_OBJECT(_arxGesture)
#else
AFX_OBJECT(arxGesture)
#endif
{
    _arxIddGes*         idd;
    _arxDdiGes const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // Gesture? Motion of body

    // Motion Geometry?
    // Slight inversion of "geometric motion", gives it a unique identity.
    // Feels more like a data structure or class name.
    // Less likely to be misinterpreted by search engines than "geometric motion".

    afxString           id;
    afxMask             flags;
    afxUnit             pivotCnt; // circuit count
    arxPivotalGesture*  pivotCurve; // uma arranjo de registros da moção de junta de arxSkeleton do correspondente arxModel.
    afxString*          pivots; // circuit id compatible with skeleton joints.
    //afxUnit              pivotLodErrorCnt;
    afxReal*            pivotLodError;
    afxUnit             vecCnt;
    arxVectorialGesture*vecCurve;
    afxString*          vectors;
    afxTransform        displacement;
    arxPivotalGesture*  root;
    afxV3d              loopTranslation;
    akxPeriodicLoop*    periodicLoop;

#if 0
    afxUnit             curveCnt;
    arxCurve*           curves;
    struct
    {
        afxUnit         baseCurIdx;
        afxUnit         curCnt;
    }*xforms;
    afxMask*            xformFlag;
    arxCurve*           deformation;
    arxCurve*           translation;
    arxCurve*           transmission;
    afxString*          articulation;
#endif
};
#endif//_ARX_GESTURE_C

ARX afxClassConfig const _ARX_GES_CLASS_CONFIG;

#endif//ARX_GESTURE_DDK_H
