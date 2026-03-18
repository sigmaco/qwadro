/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced RenderWare Extensions.

// Animatrix

#define _AFX_SIM_C
#define _ARX_PULLEY_C
#include "../scene/arxIcd.h"

_ARXINL void* ArxGetPulleyUdd(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    return pull->udd;
}

_ARXINL void ArxSetPulleyTimeOnly(afxReal timestep, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        pull->timing.currTime = timestep;
    }
}

_ARXINL afxBool ArxPulleyTerminationIsScheduled(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    return (pull->flags & arxPulleyFlag_KILL_ONCE_COMPLETE) == arxPulleyFlag_KILL_ONCE_COMPLETE;
}

_ARXINL afxBool ArxPulleyIsActive(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    return (pull->flags & arxPulleyFlag_ACTIVE) == arxPulleyFlag_ACTIVE;
}

_ARXINL afxBool ArxPulleyHasTerminated(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    return (pull->flags & arxPulleyFlag_KILL_ONCE_COMPLETE) && pull->timing.currTime >= pull->timing.killTime;
}

_ARXINL afxBool ArxPulleyHasEffect(arxPulley pull)
{
    return (pull->flags & arxPulleyFlag_ACTIVE) && pull->currWeight != 0.0;
}

_ARXINL void ArxEnablePulleyTerminationCheck(afxBool checkComplete, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        if (checkComplete)
            pull->flags |= arxPulleyFlag_KILL_ONCE_COMPLETE;
        else
            pull->flags &= (~(arxPulleyFlag_KILL_ONCE_COMPLETE));
    }
}

_ARXINL void ArxEnablePulleyIterationClamping(afxBool clamp, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        if (clamp)
            pull->flags |= arxPulleyFlag_FORCE_CLAMPLED_LOOPS;
        else
            pull->flags &= (~(arxPulleyFlag_FORCE_CLAMPLED_LOOPS));
    }
}

_ARXINL void ArxEnablePulleyEaseIn(afxBool easeIn, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        if (easeIn)
            pull->flags |= arxPulleyFlag_EASE_IN;
        else
            pull->flags &= (~(arxPulleyFlag_EASE_IN));
    }
}

_ARXINL void ArxEnablePulleyEaseOut(afxBool easeOut, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        if (easeOut)
            pull->flags |= arxPulleyFlag_EASE_OUT;
        else
            pull->flags &= (~(arxPulleyFlag_EASE_OUT));
    }
}

_ARXINL void ArxActivatePulley(afxBool active, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

        if (((pull->flags & arxPulleyFlag_ACTIVE) == arxPulleyFlag_ACTIVE) != active)
        {
            if (active)
                pull->flags |= arxPulleyFlag_ACTIVE;
            else
                pull->flags &= (~(arxPulleyFlag_ACTIVE));

            // we do not resort
        }
    }
}

_ARXINL void ArxGetPulleyTiming(arxPulley pull, arxPulleyTiming* timing)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    AFX_ASSERT(timing);
    *timing = pull->timing;
}

_ARXINL void ArxRebasePulleyTimes(afxReal timestep, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->timing.currTime += timestep;
        pull->timing.killTime += timestep;
        pull->timing.easeInStartTime += timestep;
        pull->timing.easeInEndTime += timestep;
        pull->timing.easeOutStartTime += timestep;
        pull->timing.easeOutEndTime += timestep;
    }
}

_ARXINL void ArxResetPulleyLocalTime(afxReal localTime, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->dtLocalTimePending = 0.0;
        pull->localTime = localTime;
    }
}

_ARXINL void ArxSelectPulleyIteration(afxInt currIterIdx, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->currIterIdx = currIterIdx;
    }
}

_ARXINL void ArxResetPulleyIterations(afxInt iterCnt, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->iterCnt = iterCnt;
    }
}

_ARXINL void ArxSetPulleySpeed(afxReal speed, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->speed = speed;
    }
}

_ARXINL void ArxSetPulleyWeight(afxReal weight, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        //afxReal currWeight = pull->currWeight;
        pull->currWeight = weight;
        // we do not resort
    }
}

_ARXINL void ArxStepPulleys(afxReal time, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        afxReal dt = time - pull->timing.currTime;
        pull->timing.currTime = time;
        pull->dtLocalTimePending += dt;
    }
}

_ARXINL afxBool ArxPulleyIsUnused(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    return 0 == AfxGetRefCount(pull); //AfxIsChainEmpty(&pull->puppets);
}

_ARXINL void ArxSchedulePulleyTermination(afxReal atSecs, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxPulley pull = pulleys[i];
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        pull->flags |= arxPulleyFlag_KILL_ONCE_COMPLETE;
        pull->timing.killTime = atSecs;
    }
}

_ARXINL void ArxQueryPulleyState(arxPulley pull, arxPulleyState* state)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

    afxReal localTimeRaw, localTimeClamped;
    localTimeRaw = ArxQueryPulleyLocalTime(pull, &localTimeClamped);
    afxReal speed = pull->speed;
    afxInt iterCnt = state->iterCnt;
    afxInt currIterIdx = pull->currIterIdx;
    afxReal localDur = pull->localDur;

    AFX_ASSERT(state);
    state->active = (pull->flags & arxPulleyFlag_ACTIVE) == arxPulleyFlag_ACTIVE;
    state->speed = speed;
    state->iterCnt = iterCnt;
    state->currIterIdx = currIterIdx;
    state->currWeight = pull->currWeight;
    state->effectiveWeight = state->active ? ArxGetPulleyEaseCurveMultiplier(pull) * pull->currWeight : 0.f;
    state->localTimeRaw = localTimeRaw;
    state->localTimeClamped = localTimeClamped;
    state->localDur = localDur;

    // determine iteration state
    if (pull->flags & arxPulleyFlag_FORCE_CLAMPLED_LOOPS)
    {
        state->overflow = FALSE;
        state->underflow = FALSE;
    }
    else if (iterCnt)
    {
        state->underflow = currIterIdx > 0;
        state->overflow = currIterIdx < iterCnt - 1;
    }
    else
    {
        state->overflow = TRUE;
        state->underflow = TRUE;
    }

    // determine duration
    
    state->dur = 1.9140881e38;
    state->durLeft = 1.9140881e38;

    if (iterCnt)
    {
        if (speed > 0.0)
        {
            state->dur = (localDur / speed) * iterCnt;

            if (currIterIdx < iterCnt)
                state->durLeft = (afxReal)(iterCnt - currIterIdx - 1) * (1.0 / speed * localDur) + (localDur - localTimeRaw) * (1.0 / speed);
            else
                state->durLeft = 0.0;
        }
        else if (speed < 0.0)
        {
            state->dur = -(localDur / speed) * iterCnt;

            if (pull->currIterIdx < 0)
                state->durLeft = 0.0;
            else
                state->durLeft = -(1.0 / speed * localDur) * (afxReal)currIterIdx - 1.0 / speed * localTimeRaw;
        }
    }
}

_ARXINL afxReal ArxPulleyEaseIn(arxPulley pull, afxReal Duration, afxBool fromCurrent)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxReal currTime = pull->timing.currTime;
    afxReal multiplier = fromCurrent ? ArxGetPulleyEaseCurveMultiplier(pull) : 0.0;    
    ArxEnablePulleyEaseIn(TRUE, 1, &pull);
    ArxEnablePulleyEaseOut(FALSE, 1, &pull);
    afxReal fromCurrenta = currTime + Duration;
    ArxSetPulleyEaseInCurve(pull, currTime, fromCurrenta, multiplier, multiplier, 1.0, 1.0);
    return fromCurrenta;
}

_ARXINL afxReal ArxPulleyEaseOut(arxPulley pull, afxReal duration)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxReal currTime = pull->timing.currTime;
    afxReal multiplier = ArxGetPulleyEaseCurveMultiplier(pull);
    ArxEnablePulleyEaseIn(FALSE, 1, &pull);
    ArxEnablePulleyEaseOut(TRUE, 1, &pull);
    afxReal durationa = currTime + duration;
    ArxSetPulleyEaseOutCurve(pull, currTime, durationa, multiplier, multiplier, 0.0, 0.0);
    return durationa;
}

// Modulus local clock and dependents

_ARXINL void _AfxPulleyModulusLocalTime(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxReal t = pull->localTime / pull->localDur;
    afxInt currIterIdx = pull->currIterIdx;
    afxInt i = currIterIdx + (afxInt)t;

    if (pull->localTime < 0.0)
        --i;

    afxInt iterCnt = pull->iterCnt;

    if (iterCnt)
    {
        if (i >= 0)
        {
            if (i >= iterCnt)
                i = iterCnt - 1;
        }
        else i = 0;
    }

    afxInt j = i - currIterIdx;

    if (j)
    {
        pull->currIterIdx = j + currIterIdx;
        pull->localTime = pull->localTime - (afxReal64)j * pull->localDur;
    }
}

_ARXINL void ArxSetPulleyTargetState(arxPulley pull, afxReal currGlobalTime, afxReal targetGlobalTime, afxReal localTime, afxInt loopIndex)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxReal t = (targetGlobalTime - currGlobalTime) * pull->speed;
    pull->dtLocalTimePending = 0.0;
    pull->timing.currTime = currGlobalTime;
    pull->currIterIdx = loopIndex;
    pull->localTime = localTime - t;
    _AfxPulleyModulusLocalTime(pull);
}

_ARXINL afxReal ArxQueryPulleyLocalTime(arxPulley pull, afxReal* clamped)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxInt currIterIdx = pull->currIterIdx;
    afxInt iterCnt = pull->iterCnt;

    if (pull->dtLocalTimePending != 0.0)
    {
        // process pending local clock

        while (1)
        {
            afxReal t = pull->speed * pull->dtLocalTimePending + pull->localTime;
            pull->localTime = t;

            if (1) // [!]
            {
                if (iterCnt && (currIterIdx <= 0))
                    break; // avoid modulus
            }
            else
            {
                if (t < pull->localDur)
                    break; // avoid modulus

                if (iterCnt)
                {
                    if (currIterIdx >= (iterCnt - 1))
                        break; // avoid modulus
                }
            }
            _AfxPulleyModulusLocalTime(pull);
            break;
        }
        pull->dtLocalTimePending = 0.0; // reset pending local clock
    }
    
    afxReal localTime = pull->localTime;

    if (clamped)
    {
        if (localTime < 0.0)
            *clamped = 0.0;
        else if (localTime > pull->localDur)
            *clamped = pull->localDur;
        else
            *clamped = localTime;
    }
    return localTime;
}

_ARXINL void _PackEaseCurve(afxUnit *result, afxReal Af, afxReal Bf, afxReal Cf, afxReal Df)
{
    *result = ((afxInt)(Af * 255.0)) | ((((afxInt)(Bf * 255.0)) | ((((afxInt)(Cf * 255.0)) | ((afxInt)((afxInt)(Df * 255.0)) << 8)) << 8)) << 8);
}

_ARXINL void ArxSetPulleyEaseInCurve(arxPulley pull, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    pull->timing.easeInStartTime = StartSeconds;
    pull->timing.easeInEndTime = EndSeconds;
    _PackEaseCurve(&pull->easeInValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ARXINL void ArxSetPulleyEaseOutCurve(arxPulley pull, afxReal StartSeconds, afxReal EndSeconds, afxReal StartValue, afxReal StartTangent, afxReal EndTangent, afxReal EndValue)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    pull->timing.easeOutStartTime = StartSeconds;
    pull->timing.easeOutEndTime = EndSeconds;
    _PackEaseCurve(&pull->easeOutValues, StartValue, StartTangent, EndTangent, EndValue);
}

_ARXINL afxReal _ComputeEaseCurve(afxReal *Bezier, afxReal StartClock, afxReal CurrentClock, afxReal EndClock)
{
    afxReal v5; // st7

    afxReal v4 = EndClock - StartClock;

    if (v4 == 0.0)
    {
        if (CurrentClock < (double)StartClock)
            goto LABEL_3;
    LABEL_7:
        v5 = 1.0;
        return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
    }

    v5 = (CurrentClock - StartClock) / v4;

    if (v5 >= 0.0)
    {
        if (v5 <= 1.0)
            return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);

        goto LABEL_7;
    }

LABEL_3:
    v5 = 0.0;
    return ((1.0 - v5) * Bezier[1] + v5 * Bezier[2]) * (1.0 - v5) * v5 * 3.0 + v5 * Bezier[3] * v5 * v5 + (1.0 - v5) * Bezier[0] * (1.0 - v5) * (1.0 - v5);
}

_ARXINL void _UnpackEaseCurve(afxUnit32 value, afxV4d result)
{
    result[0] = (afxReal)(afxUnit8)value * 0.0039215689;
    result[1] = (afxReal)((afxUnit16)value >> 8) * 0.0039215689;
    result[2] = (afxReal)((value >> 16) & 0xFF) * 0.0039215689;
    result[3] = (afxReal)(value >> 24) * 0.0039215689;
}

_ARXINL afxReal ArxGetPulleyEaseCurveMultiplier(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
    afxV4d easeInValues;
    _UnpackEaseCurve(pull->easeInValues, easeInValues);
    afxV4d easeOutValues;
    _UnpackEaseCurve(pull->easeOutValues, easeOutValues);

    afxReal result = 1.0;

    if (pull->timing.easeInStartTime >= pull->timing.easeOutStartTime && (pull->flags & arxPulleyFlag_EASE_OUT) && pull->flags & arxPulleyFlag_EASE_IN)
    {
        if (pull->timing.currTime > pull->timing.easeOutEndTime)
        {
            if (pull->timing.currTime > pull->timing.easeInStartTime)
            {
                if (pull->timing.currTime <= pull->timing.easeInEndTime)
                    result = _ComputeEaseCurve(easeInValues, pull->timing.easeInStartTime, pull->timing.currTime, pull->timing.easeInEndTime);
            }
            else
            {
                afxReal a = (pull->timing.currTime - pull->timing.easeOutEndTime) / (pull->timing.easeInStartTime - pull->timing.easeOutEndTime);
                result = (1.0 - a) * easeOutValues[3] + easeInValues[0] * (a);
            }
        }
        else
        {
            result = _ComputeEaseCurve(easeOutValues, pull->timing.easeOutStartTime, pull->timing.currTime, pull->timing.easeOutEndTime);
        }
    }
    else
    {
        if ((pull->flags & arxPulleyFlag_EASE_OUT) && pull->timing.currTime >= pull->timing.easeOutStartTime)
            result = _ComputeEaseCurve(easeOutValues, pull->timing.easeOutStartTime, pull->timing.currTime, pull->timing.easeOutEndTime);

        if ((pull->flags & arxPulleyFlag_EASE_IN) && pull->timing.currTime <= pull->timing.easeInEndTime)
            result = _ComputeEaseCurve(easeInValues, pull->timing.easeInStartTime, pull->timing.currTime, pull->timing.easeInEndTime);
    }
    return result;
}

_ARX afxError _ArxMotoDtorCb(arxPulley pull)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

    pull->flags &= 0xFFFFFFF8;
    
    return err;
}

_ARX afxError _ArxMotoCtorCb(arxPulley pull, void** args, afxUnit invokeNo)
{
    (void)invokeNo;

    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxPulleyConfig const* cfg = AFX_CAST(arxPulleyConfig const*, args[1]);

    pull->flags = arxPulleyFlag_ACTIVE;
    pull->timing.currTime = cfg->currTime;
    pull->dtLocalTimePending = 0.0;
    pull->localTime = 0.0;
    pull->speed = 1.0;
    pull->localDur = cfg->localDur;
    pull->currIterIdx = 0;
    pull->iterCnt = cfg->iterCnt;
    pull->timing.killTime = 0;
    pull->currWeight = 1;
    pull->timing.easeInStartTime = 0;
    pull->timing.easeInEndTime = 0;
    pull->easeInValues = (afxUnit)-65536;
    pull->timing.easeOutStartTime = 0;
    pull->timing.easeOutEndTime = 0;
    pull->easeOutValues = (afxUnit)0xFFFF;

    return err;
}

_ARX afxClassConfig const _ARX_MOTO_CLASS_CONFIG =
{
    .fcc = afxFcc_PULL,
    .name = "Pulley",
    .desc = "Locomotion Pulley",
    .fixedSiz = sizeof(AFX_OBJECT(arxPulley)),
    .ctor = (void*)_ArxMotoCtorCb,
    .dtor = (void*)_ArxMotoDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxBool ArxReleaseTerminatedPulleys(afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT(pulleys);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    arxPulley pull;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((pull = pulleys[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

            if (ArxPulleyHasTerminated(pull))
                if (AfxDisposeObjects(1, (void*[]) { pull }))
                    ++rslt;
        }
    }
    return rslt;
}

_ARX afxBool ArxReleaseUnusedPulleys(afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT(pulleys);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;
    arxPulley pull;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((pull = pulleys[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);

            if ((pull->flags & arxPulleyFlag_KILL_ONCE_UNUSED) && ArxPulleyIsUnused(pull))
                if (AfxDisposeObjects(1, &pull))
                    ++rslt;
        }
    }
    return rslt;
}

_ARX afxBool ArxReleaseOnceUnusedPulleys(afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT(pulleys);
    AFX_ASSERT(cnt);

    //afxBool rslt = 0;
    arxPulley pull;

    for (afxUnit i = cnt; i-- > 0;)
    {
        if ((pull = pulleys[i]))
        {
            AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
            pull->flags |= arxPulleyFlag_KILL_ONCE_UNUSED;
        }
    }
    return ArxReleaseUnusedPulleys(cnt, pulleys);
}

_ARX afxError ArxAcquirePulleys(arxScenario scio, arxPulleyConfig const* cfg, afxUnit cnt, arxPulley pulleys[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(cfg);

    afxClass *cls = (afxClass *)_ArxScioGetCapClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PULL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pulleys, (void const*[]) { scio, cfg }))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_PULL, cnt, pulleys);

    return err;
}

_ARX afxUnit ArxRecenterAllPulleyClocks(arxScenario scio, afxReal dCurrentClock)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxUnit i = 0;
    arxPulley pull;
    afxClass *cls = (afxClass *)_ArxScioGetCapClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_PULL);    
    while (!AfxEnumerateObjects(cls, i++, 1, (afxObject*)&pull))
    {
        AFX_ASSERT_OBJECTS(afxFcc_PULL, 1, &pull);
        ArxRebasePulleyTimes(dCurrentClock, 1, &pull);
    }
    return i;
}
