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

#ifndef ARX_PULLEY_H
#define ARX_PULLEY_H

// Servo
// Governor
// Regulator

// Terminology: Motive is an animating force or underlying purpose; intention;
// Terminology: Puppet is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.
// Terminology: Pulley is a wheel with a grooved rim around which a cord passes. It acts to change the direction of a force applied to the cord and is chiefly used (typically in combination) to raise heavy weights.

// There are two primary objects involved in the run-time animation layer : the arxPuppet and the arxPulley.
// Together, they represent the two ends of the animation process.
// The arxPuppet represents the state of a single model that may have some number of animations affecting it, 
// whereas the arxPulley represents the state of an animation that may be affecting some number of models.
// When your engine creates a new animating entity in the game world, you create a arxPuppet to accompany it.
// When you want to play an animation on such an instance, you create a arxPulley.

// Essential part of our machinima.

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/cad/arxPose.h"

AFX_DEFINE_STRUCT(arxPulleyTiming)
{
    afxReal         currTime;
    afxReal         killTime;
    afxReal         easeInStartTime;
    afxReal         easeInEndTime;
    afxReal         easeOutStartTime;
    afxReal         easeOutEndTime;
};

AFX_DEFINE_STRUCT(arxPulleyState)
{
    afxBool         active;
    afxReal         speed;
    afxInt          iterCnt;
    afxInt          currIterIdx;
    afxReal         currWeight;
    afxReal         effectiveWeight;
    afxBool         underflow; // Figure out what looping state the control is in
    afxBool         overflow; // Figure out what looping state the control is in
    afxReal         localTimeRaw;
    afxReal         localTimeClamped;
    afxReal         dur; // Find out how long this control plays for, total
    afxReal         durLeft; // Find out how long this control will continue to play for, accounting for however much time has already elapsed
    afxReal         localDur;
};

AFX_DEFINE_STRUCT(arxPulleyConfig)
{
    afxReal currTime;
    afxReal localDur;
    afxUnit iterCnt;
};

ARXINL void ArxQueryPulleyState
/// Query the current state of a motor execution.
(
    arxPulley pull,
    arxPulleyState* state
);

ARX afxReal ArxQueryPulleyLocalTime
/// Query the raw local clock and, optionally, compute a clamped value of it valid to be used to sample animation.
(
    arxPulley pull,
    afxReal* clamped // [0-localDur]
);

ARX void ArxGetPulleyTiming
/// Get internal clocks for a motor.
(
    arxPulley pull,
    arxPulleyTiming* timing
);

ARX afxBool ArxPulleyIsUnused
(
    arxPulley pull
);

// TERMINATION

ARX afxBool ArxPulleyHasTerminated
(
    arxPulley pull
);

ARX afxBool ArxPulleyTerminationIsScheduled
(
    arxPulley pull
);

ARX afxBool ArxPulleyIsActive
(
    arxPulley pull
);

ARX afxBool ArxPulleyHasEffect
(
    arxPulley pull
);

ARX void* ArxGetPulleyUdd
(
    arxPulley pull
);

ARX void ArxSetPulleyTargetState
(
    arxPulley pull, 
    afxReal currGlobalTime, 
    afxReal targetGlobalTime, 
    afxReal localTime, 
    afxInt currIterIdx
);

ARX void ArxSetPulleyEaseInCurve
(
    arxPulley pull, 
    afxReal startSeconds, 
    afxReal endSeconds, 
    afxReal startValue, 
    afxReal startTangent, 
    afxReal endTangent, 
    afxReal endValue
); // Ease the control in over the next two seconds

ARX void ArxSetPulleyEaseOutCurve
(
    arxPulley pull, 
    afxReal startSeconds, 
    afxReal endSeconds, 
    afxReal startValue, 
    afxReal startTangent, 
    afxReal endTangent, 
    afxReal endValue
); // In ten seconds, ease the control out over two seconds

ARX afxReal ArxPulleyEaseIn
(
    arxPulley pull, 
    afxReal duration, 
    afxBool fromCurrent
);

ARX afxReal ArxPulleyEaseOut
(
    arxPulley pull, 
    afxReal duration
);

ARX afxReal ArxGetPulleyEaseCurveMultiplier
(
    arxPulley pull
);

////////////////////////////////////////////////////////////////////////////////

/// Resets the raw local clock of a motor to the time passed in.

ARX void            ArxResetPulleyLocalTime(afxReal localTime, afxUnit cnt, arxPulley pulleys[]);

// Devido a limitação imposto pelo tipo afxReal32, os relógios de uma arxPulley não podem operar por mais que um dia e meio sem problemas relacionados a perda de precisão.
// É necessário recentralizar os relógios em algum momento após tal período de operação.

/// Rebase internal clocks with the passed into one.

ARX void            ArxRebasePulleyTimes(afxReal time, afxUnit cnt, arxPulley pulleys[]);

/// Update the current motor clock to a new one. // new absolute time

ARX void            ArxStepPulleys(afxReal /* NOT delta */ time, afxUnit cnt, arxPulley pulleys[]);

// deveria ser suspend, já que o motor já começa ativo

ARX void            ArxActivatePulley(afxBool active, afxUnit cnt, arxPulley pulleys[]);

ARX void            ArxSetPulleyWeight(afxReal weight, afxUnit cnt, arxPulley pulleys[]);

/// Sets the speed multiplier. 0.f = stop it, -1.f play it backwards, 0.5 half the playback, 2.0 doubles the playback, etc.

ARX void            ArxSetPulleySpeed(afxReal speed, afxUnit cnt, arxPulley pulleys[]);

ARX void            ArxSetPulleyTimeOnly(afxReal time, afxUnit cnt, arxPulley pulleys[]);

/// Resets the total of iterations allowed for the pulleys. 0 means infinite.

ARX void            ArxResetPulleyIterations(afxInt iterCnt, afxUnit cnt, arxPulley pulleys[]);

/// Selects the current iteration index.

ARX void            ArxSelectPulleyIteration(afxInt currIterIdx, afxUnit cnt, arxPulley pulleys[]);

ARX void            ArxEnablePulleyIterationClamping(afxBool clamp, afxUnit cnt, arxPulley pulleys[]);
ARX void            ArxEnablePulleyTerminationCheck(afxBool checkComplete, afxUnit cnt, arxPulley pulleys[]);
ARX void            ArxEnablePulleyEaseIn(afxBool easeIn, afxUnit cnt, arxPulley pulleys[]);
ARX void            ArxEnablePulleyEaseOut(afxBool easeOut, afxUnit cnt, arxPulley pulleys[]);

/// Schedules the self-termination when pulleys reaches the specified seconds.

ARX void            ArxSchedulePulleyTermination(afxReal atSecs, afxUnit cnt, arxPulley pulleys[]);

ARX afxBool         ArxReleaseUnusedPulleys(afxUnit cnt, arxPulley pulleys[]);
ARX afxBool         ArxReleaseOnceUnusedPulleys(afxUnit cnt, arxPulley pulleys[]);
ARX afxBool         ArxReleaseTerminatedPulleys(afxUnit cnt, arxPulley pulleys[]);

ARX afxError        ArxAcquirePulleys(arxScenario scio, arxPulleyConfig const* cfg, afxUnit cnt, arxPulley pulleys[]);

#endif//ARX_PULLEY_H
