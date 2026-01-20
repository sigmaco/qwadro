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

// This software is part of Advanced Multimedia Extensions.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ACX_STEP_C
#define _ACX_STEP_SYSTEM_C
#define _ACX_STEP_DEVICE_C
#define _ACX_STEP_CONTEXT_C
#include "acxIcd.h"

_ACX afxError _AcxSctxPushCmd(afxStepContext sctx, afxUnit id, afxUnit siz, afxCmdId* cmdId, _acxCmd** ppCmd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    AFX_ASSERT(siz >= sizeof(_acxCmdHdr));

    _acxCmd* cmd = AfxRequestArena(&sctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &sctx->commands);
    AFX_ASSERT(ppCmd);
    *ppCmd = cmd;

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_ACX afxClass const* _AcxSctxGetSctxClass(afxStepContext sctx)
{
    afxError err = { 0 };
    // sctx must be a valid afxStepContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    afxClass const* cls = &sctx->sctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCTX);
    return cls;
}

_ACX acxContextState _AcxSctxGetStatus(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->state;
}

_ACX afxMask AcxGetCommandPort(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->exuMask;
}

_ACX afxStepContext AcxGetCommandPool(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->pool;
}

_ACX afxError AcxPrepareStepCommands(afxStepContext sctx, afxBool purge, acxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    // sctx must be a valid afxStepContext handle.
    // sctx must not be in the PENDING state.
    AFX_ASSERT(sctx->state != acxContextState_PENDING);

    AFX_ASSERT(sctx->ddi->prepare);
    if (sctx->ddi->prepare(sctx, purge, flags))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(sctx->state == acxContextState_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
    }
    return err;
}

_ACX afxError AcxCompileStepCommands(afxStepContext sctx)
{
    afxError err = { 0 };
    // sctx must be a valid afxStepContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    // sctx must be in the RECORDING state.
    AFX_ASSERT(sctx->state == acxContextState_RECORDING);

    AFX_ASSERT(sctx->ddi->compile);
    if (sctx->ddi->compile(sctx))
    {
        AfxThrowError();

        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileStepCommands, and the draw context will be moved to the invalid state.

        AFX_ASSERT(sctx->state == acxContextState_INVALID);
    }
    else
    {
        // The mix context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(sctx->state == acxContextState_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
    }
    return err;
}

_ACX afxError AcxExhaustStepContext(afxStepContext sctx, afxBool freeMem)
{
    afxError err = { 0 };
    // sctx must be a valid afxStepContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    // dctx must be a pool context.
    AFX_ASSERT(!sctx->pool || (sctx->pool == sctx));

    AFX_ASSERT(sctx->ddi->exhaust);
    if (sctx->ddi->exhaust(sctx, freeMem))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
    }
    return err;
}

_ACX afxError _AcxSctxDtorCb(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    AcxExhaustStepContext(sctx, TRUE);

    AfxExhaustInterlockedQueue(&sctx->recycQue);

    AfxDismantleArena(&sctx->cmdArena);

    AfxDeregisterChainedClasses(&sctx->classes);

    return err;
}

_ACX afxError _AcxSctxCtorCb(afxStepContext sctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    afxStepSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxStepContext pool = AFX_CAST(afxStepContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);
    AFX_ASSERT(args[2]);
    acxContextConfig const* info = AFX_CAST(acxContextConfig const*, args[2]);
    afxClassConfig const* pSctxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    sctx->ddi = &_ACX_SCTX_DDI;
    sctx->udd = info->udd;
    sctx->tag = info->tag;
    //sctx->devLimits = _AcxMsysGetLimits(ssys);
    //sctx->enabledFeatures = _AcxMsysGetReqFeatures(ssys);
    
    AfxMakeChain(&sctx->classes, sctx);

    if (!pool)
    {
        sctx->pool = sctx;

        sctx->caps = info->caps;
        sctx->exuMask = info->exuMask;

        afxClassConfig sctxClsCfg = pSctxClsCfg ? *pSctxClsCfg : _ACX_SCTX_CLASS_CONFIG;
        AFX_ASSERT(sctxClsCfg.fcc == afxFcc_SCTX);
        sctxClsCfg.unitsPerPage = AFX_OR(AFX_MIN(info->auxCnt, 32), sctxClsCfg.unitsPerPage);
        AfxMountClass(&sctx->sctxCls, NIL, &sctx->classes, &sctxClsCfg);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&sctx->recycQue, sizeof(afxStepContext), AFX_MAX(AFX_MIN(info->recycCap, 32), 1));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);

        // If there is a base, the base must be its own base.
        // Effectively, a aux context can only be acquired from a base context.
        // By allowing it to be itself, we allow the code to proceed normally avoid testing against NIL.
        if (pool->pool && (pool->pool != pool))
        {
            AfxThrowError();
            return err;
        }
        sctx->pool = pool;

        sctx->caps = (pool->caps & info->caps);
        sctx->exuMask = (pool->exuMask & info->exuMask);

        sctx->sctxCls = (afxClass) { 0 };

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&sctx->recycQue, sizeof(afxStepContext), 1);
    }

    sctx->flags = info->flags;
    sctx->cmdFlags = info->cmdFlags;
    sctx->state = acxContextState_INITIAL;

    AfxMakeChain(&sctx->commands, sctx);
    AfxMakeArena(&sctx->cmdArena, NIL, AfxHere());

    return err;
}

_ACX afxClassConfig const _ACX_SCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_SCTX,
    .name = "StepContext",
    .desc = "ACX Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepContext)),
    .ctor = (void*)_AcxSctxCtorCb,
    .dtor = (void*)_AcxSctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ACX afxError AcxAcquireStepContexts(afxStepSystem ssys, afxStepContext pool, acxContextConfig const* info, afxUnit cnt, afxStepContext contexts[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    // contexts must be a valid pointer to an array of @cnt * afxStepContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid acxContextConfig structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AcxSsysGetSctxClass(ssys);
        AFX_ASSERT_CLASS(cls, afxFcc_SCTX);
        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { ssys, pool, info }))
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_SCTX, cnt, contexts);
        return err;
    }

    // If @pool is not NIL, @pool must be a valid afxStepContext handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);

    if (pool->pool != pool)
    {
        AFX_ASSERT(pool->pool != pool);
        AfxThrowError();
        return err;
    }

    afxBool once = (info->cmdFlags & acxCmdFlag_ONCE);
    afxBool deferred = (info->cmdFlags & acxCmdFlag_DEFERRED);

    afxUnit recycCnt = 0;
    while (1)
    {
        // Firstly, try recycling contexts.

        afxStepContext aux;
        if (AfxPopInterlockedQueue(&pool->recycQue, &aux))
        {
            AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &aux);
            AFX_ASSERT(aux->submCnt == 0);
            AFX_ASSERT(aux->submQueMask == 0);

            AcxPrepareStepCommands(aux, TRUE, info->cmdFlags);

            AfxMakeChain(&aux->commands, aux);

            AFX_ASSERT(AfxLoadAtom32(&aux->submCnt) == 0);
            aux->submCnt = 0;
            aux->submQueMask = NIL;

            aux->flags = info->flags ? (pool->flags & info->flags) : pool->flags;
            aux->cmdFlags = info->cmdFlags ? (pool->cmdFlags & info->cmdFlags) : pool->cmdFlags;
            aux->exuMask = info->exuMask ? (pool->exuMask & info->exuMask) : pool->exuMask;
            aux->caps = info->caps ? (pool->caps & info->caps) : pool->caps;

            contexts[recycCnt] = aux;

            ++recycCnt;
            if (recycCnt >= cnt)
                break;
        }
    }

    afxUnit remainCnt = (cnt - recycCnt);
    if (remainCnt)
    {
        cls = (afxClass*)_AcxSctxGetSctxClass(pool);
        AFX_ASSERT_CLASS(cls, afxFcc_SCTX);
        if (AfxAcquireObjects(cls, remainCnt, (afxObject*)&contexts[recycCnt], (void const*[]) { ssys, pool, info }))
        {
            AfxThrowError();

            // If acquisition failed, dispose all recycled contexts.
            // TODO: Recycle it.
            AfxDisposeObjects(recycCnt, contexts);
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_SCTX, cnt, contexts);
    return err;
}

_ACX afxError AcxRecycleStepContexts(afxBool freeRes, afxUnit cnt, afxStepContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxStepContext sctx = contexts[iter];
        if (!sctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
        AFX_ASSERT(sctx->state != acxContextState_PENDING);

        AFX_ASSERT(sctx->ddi->recycle);
        if (sctx->ddi->recycle(sctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(sctx->state == acxContextState_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
        }
    }
    return err;
}

_ACX afxError AcxExecuteStepCommands(afxStepSystem ssys, afxUnit cnt, acxSubmission const submissions[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        acxSubmission const* subm = &submissions[ctxIt];

        afxStepContext sctx = subm->sctx;
        if (!sctx)
        {
            AFX_ASSERT(subm->sctx);
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

        AFX_ASSERT(!(sctx->cmdFlags & acxCmdFlag_DEFERRED));

        if (sctx->state != acxContextState_EXECUTABLE)
        {
            if ((sctx->state == acxContextState_PENDING) ||
                (sctx->state == acxContextState_INTERNAL_EXECUTING))
            {
                if (!(sctx->cmdFlags & acxCmdFlag_SHARED))
                {
                    AFX_ASSERT(!(sctx->cmdFlags & acxCmdFlag_SHARED));
                    AfxThrowError();
                }
            }
        }

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AcxChooseStepBridges(ssys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxStepBridge dexu;
                if (!AcxChooseStepBridges(ssys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxStepQueue sque;
                    afxUnit nextQueIdx = 0;
                    while (AcxGetStepQueues(dexu, nextQueIdx, 1, &sque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
                        ++nextQueIdx;

                        afxError err2 = _AcxSqueSubmitStepCommands(sque, 1, subm);

                        if (!err2)
                        {
                            queued = TRUE;
                            break; // while --- get queue
                        }

                        if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                            break; // while --- get queue

                        AfxThrowError();
                    }
                    if (err || queued) break; // while --- find queues
                }
                if (err || queued) break; // for --- iterate bridge
            }
            if (err || queued) break; // while --- find bridges
        }
    }
    return err;
}
