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

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ASX_IMPL___SIMULATION_H
#define ASX_IMPL___SIMULATION_H

#include "qwadro/sim/arxSimulation.h"
#include "../scene/arxScenarioDDK.h"
#include "../anim/arxAnimationDDK.h"

#define ASX_MAX_SIM_BRIDGE_PER_CONTEXT 16

AFX_DEFINE_STRUCT(_acxSimulationAcquisition)
{
    afxUnit             bridgeCnt;
    arxEngineFeatures   requirements;
    afxUnit             extensionCnt;
    afxString const*    extensions;
    void*               udd;

    afxDrawSystem       dsys;
    arxScenario        scio;

    afxClassConfig const* sexuClsCfg;

};

#ifdef _ASX_SIMULATION_C
#ifdef _ASX_SIMULATION_IMPL
AFX_OBJECT(_acxSimulation)
#else
AFX_OBJECT(arxSimulation)
#endif
{
    afxBool         running;

    afxUnit         bridgeCnt;
    arxSimBridge*   bridges;
    arxEngineFeatures requirements;

    afxChain        classes;
    afxClass        sexuCls;

    afxClass        shapCls;


    afxSize         stepNum, stepsPerSecond;
    afxSize         stepTime, swapTime;
    afxReal         stepDeltaTime;

    arxMotive       globalPulleyInterlinks;

    afxError(*waitCb)(arxSimulation,afxTime);
    afxError(*stepCb)(arxSimulation, afxReal);

    afxDrawSystem  dsys;
    arxScenario    scio;

    struct smt* Smt;
    struct smt2* Smt2;

    struct _afxSimIdd* idd;
    void* udd; // user-defined data
};
#endif//_ASX_SIMULATION_C

ASX afxClass const* _AsxGetShapClass(arxSimulation sim);


ASX afxClassConfig const _ASX_SIM_CLASS_CONFIG;

typedef struct smt
{
    void(*SampleTrackUUULocalAtTime0)(arxPivotalGesture const* SourceTrack, afxTransform *Result);
    void(*SampleTrackUUULocal)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, afxTransform *Result);
    void(*SampleTrackUUU)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackIII)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCII)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCCI)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackCAI)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAII)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackACI)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackAAI)(const arxSampleContext *Context, const arxPivotalGesture*SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*Samplers[3][3][3])(const arxSampleContext *, const arxPivotalGesture *, arxCachedGestureTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackSSS)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*(*GetTrackSamplerFor)(const arxPivotalGesture *Track))(const arxSampleContext *, const arxPivotalGesture *, arxCachedGestureTransform *, const afxTransform *, const afxM4d, const afxM4d, afxM4d, afxM4d);
    void(*SampleTrackUUUAtTime0)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult);
    void(*SampleTrackUUUBlendWithTime0)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4, const afxM4d ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount);
    void(*SampleTrackIIU)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, const afxTransform *RestTransform, const afxM4d InverseWorld4x4Aliased, const afxM4d ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased);
    void(*SampleTrackPOLocal)(const arxSampleContext *Context, const arxPivotalGesture *SourceTrack, arxCachedGestureTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation);
} smt;

typedef struct smt2
{
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxUnit Degree, afxUnit Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, arxCurve prev, arxCurve c, arxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const arxSampleContext *Context, arxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

#endif//ASX_IMPL___SIMULATION_H
