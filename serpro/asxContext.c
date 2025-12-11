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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ASX_STEP_C
#define _ASX_STEP_SYSTEM_C
#define _ASX_STEP_DEVICE_C
#define _ASX_STEP_CONTEXT_C
#include "asxIcd.h"

_ASX afxError _AsxSctxPushCmd(afxStepContext sctx, afxUnit id, afxUnit siz, afxCmdId* cmdId, _asxCmd** ppCmd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    AFX_ASSERT(siz >= sizeof(_asxCmdHdr));

    _asxCmd* cmd = AfxRequestArena(&sctx->cmdArena, siz, 1, NIL, 0);
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

_ASX afxClass const* _AsxSctxGetSctxClass(afxStepContext sctx)
{
    afxError err = { 0 };
    // sctx must be a valid afxStepContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    afxClass const* cls = &sctx->sctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCTX);
    return cls;
}

_ASX asxContextState _AsxSctxGetStatus(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->state;
}

_ASX afxMask AsxGetCommandPort(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->exuMask;
}

_ASX afxStepContext AsxGetCommandPool(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    return sctx->pool;
}

_ASX afxError AsxPrepareStepCommands(afxStepContext sctx, afxBool purge, asxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    // sctx must be a valid afxStepContext handle.
    // sctx must not be in the PENDING state.
    AFX_ASSERT(sctx->state != asxContextState_PENDING);

    AFX_ASSERT(sctx->ddi->prepare);
    if (sctx->ddi->prepare(sctx, purge, flags))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT(sctx->state == asxContextState_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
    }
    return err;
}

_ASX afxError AsxCompileStepCommands(afxStepContext sctx)
{
    afxError err = { 0 };
    // sctx must be a valid afxStepContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
    // sctx must be in the RECORDING state.
    AFX_ASSERT(sctx->state == asxContextState_RECORDING);

    AFX_ASSERT(sctx->ddi->compile);
    if (sctx->ddi->compile(sctx))
    {
        AfxThrowError();

        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileStepCommands, and the draw context will be moved to the invalid state.

        AFX_ASSERT(sctx->state == asxContextState_INVALID);
    }
    else
    {
        // The mix context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(sctx->state == asxContextState_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
    }
    return err;
}

_ASX afxError AsxExhaustStepContext(afxStepContext sctx, afxBool freeMem)
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

_ASX afxError _AsxSctxDtorCb(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    AsxExhaustStepContext(sctx, TRUE);

    AfxExhaustInterlockedQueue(&sctx->recycQue);

    AfxDismantleArena(&sctx->cmdArena);

    AfxDeregisterChainedClasses(&sctx->classes);

    return err;
}

_ASX afxError _AsxSctxCtorCb(afxStepContext sctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    afxStepSystem ssys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    afxStepContext pool = AFX_CAST(afxStepContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);
    AFX_ASSERT(args[2]);
    asxContextConfig const* info = AFX_CAST(asxContextConfig const*, args[2]);
    afxClassConfig const* pSctxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    sctx->ddi = &_ASX_SCTX_DDI;
    sctx->udd = info->udd;
    sctx->tag = info->tag;
    //sctx->devLimits = _AsxMsysGetLimits(ssys);
    //sctx->enabledFeatures = _AsxMsysGetReqFeatures(ssys);
    
    AfxMakeChain(&sctx->classes, sctx);

    if (!pool)
    {
        sctx->pool = sctx;

        sctx->caps = info->caps;
        sctx->exuMask = info->exuMask;

        afxClassConfig sctxClsCfg = pSctxClsCfg ? *pSctxClsCfg : _ASX_SCTX_CLASS_CONFIG;
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
    sctx->state = asxContextState_INITIAL;

    AfxMakeChain(&sctx->commands, sctx);
    AfxMakeArena(&sctx->cmdArena, NIL, AfxHere());

    return err;
}

_ASX afxClassConfig const _ASX_SCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_SCTX,
    .name = "StepContext",
    .desc = "ASX Operation Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepContext)),
    .ctor = (void*)_AsxSctxCtorCb,
    .dtor = (void*)_AsxSctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireStepContexts(afxStepSystem ssys, afxStepContext pool, asxContextConfig const* info, afxUnit cnt, afxStepContext contexts[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    // contexts must be a valid pointer to an array of @cnt * afxStepContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid asxContextConfig structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AsxSsysGetSctxClass(ssys);
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

    afxBool once = (info->cmdFlags & asxCmdFlag_ONCE);
    afxBool deferred = (info->cmdFlags & asxCmdFlag_DEFERRED);

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

            AsxPrepareStepCommands(aux, TRUE, info->cmdFlags);

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
        cls = (afxClass*)_AsxSctxGetSctxClass(pool);
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

_ASX afxError AsxRecycleStepContexts(afxBool freeRes, afxUnit cnt, afxStepContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxStepContext sctx = contexts[iter];
        if (!sctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);
        AFX_ASSERT(sctx->state != asxContextState_PENDING);

        AFX_ASSERT(sctx->ddi->recycle);
        if (sctx->ddi->recycle(sctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(sctx->state == asxContextState_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&sctx->commands));
        }
    }
    return err;
}

_ASX afxError AsxExecuteStepCommands(afxStepSystem ssys, afxUnit cnt, asxSubmission const submissions[])
{
    afxError err = { 0 };
    // ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        asxSubmission const* subm = &submissions[ctxIt];

        afxStepContext sctx = subm->sctx;
        if (!sctx)
        {
            AFX_ASSERT(subm->sctx);
            AfxThrowError();
            continue;
        }
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

        AFX_ASSERT(!(sctx->cmdFlags & asxCmdFlag_DEFERRED));

        if (sctx->state != asxContextState_EXECUTABLE)
        {
            if ((sctx->state == asxContextState_PENDING) ||
                (sctx->state == asxContextState_INTERNAL_EXECUTING))
            {
                if (!(sctx->cmdFlags & asxCmdFlag_SHARED))
                {
                    AFX_ASSERT(!(sctx->cmdFlags & asxCmdFlag_SHARED));
                    AfxThrowError();
                }
            }
        }

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxStepBridge dexu;
                if (!AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxStepQueue sque;
                    afxUnit nextQueIdx = 0;
                    while (AsxGetStepQueues(dexu, nextQueIdx, 1, &sque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);
                        ++nextQueIdx;

                        afxError err2 = _AsxSqueSubmitStepCommands(sque, 1, subm);

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
