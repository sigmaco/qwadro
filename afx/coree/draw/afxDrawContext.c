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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

/*
    While programming I find myself thinking whether I am a program programmed to program programs in this program.
    — Veryzon
*/

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX _avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(siz >= sizeof(_avxCmdHdr));

    _avxCmd* cmd = AfxRequestArena(&dctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &dctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxCmdId AvxGetLastCommandId(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    afxCmdId cmdId = dctx->commands.cnt - 1;
    return cmdId;
}

_AVX afxCmdId AvxGetNextCommandId(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    afxCmdId cmdId = dctx->commands.cnt;
    return cmdId;
}

_AVX afxClass const* _AvxDctxGetDctxClass(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    afxClass const* cls = &dctx->dctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
    return cls;
}

_AVX avxContextState _AvxDctxGetStatus(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return dctx->state;
}

_AVX afxMask AvxGetCommandPort(afxDrawContext dctx, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return exuMask ? (dctx->exuMask & exuMask) : dctx->exuMask;
}

_AVX avxService AvxGetCommandAptitude(afxDrawContext dctx, avxService caps)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    return caps ? (dctx->caps & caps) : dctx->caps;
}

_AVX afxDrawContext AvxGetCommandPool(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    afxDrawContext pool = dctx->pool;
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
    return pool;
}

_AVX afxError AvxTraverseDrawCommands(afxDrawContext dctx, afxCmdId(*f)(void* udd, void* cmd), void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    return err;
}

_AVX afxError _AvxDctxPrepareCb(afxDrawContext dctx, afxBool purge, avxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&dctx->cmdArena);
    }
#else
    AfxExhaustArena(&dctx->cmdArena);
#endif
    AfxMakeChain(&dctx->commands, dctx);

    dctx->inDrawScope = FALSE;
    dctx->inVideoCoding = FALSE;

    dctx->state = avxContextState_RECORDING;
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);

    return err;
}

_AVX afxError AvxPrepareDrawCommands(afxDrawContext dctx, afxBool purge, avxCmdFlags flags)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // dctx must not be in the PENDING state.
    if ((dctx->state >= avxContextState_PENDING)
        && (dctx->state != avxContextState_INVALID))
    {
        AFX_ASSERT(!(dctx->state >= avxContextState_PENDING));
        err = afxError_BUSY;
        return err;
    }

    AFX_ASSERT(dctx->ddi->prepare);
    afxError err2 = dctx->ddi->prepare(dctx, purge, flags);

    if (err2)
    {
        AfxThrowError();
        err = err2;
    }
    else
    {
        AFX_ASSERT(dctx->state == avxContextState_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }

    return err;
}

_AVX afxError _AvxDctxCompileCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    dctx->state = avxContextState_EXECUTABLE;

    return err;
}

_AVX afxError AvxCompileDrawCommands(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // dctx must be in the RECORDING state.
    if (dctx->state != avxContextState_RECORDING)
    {
        AFX_ASSERT(dctx->state == avxContextState_RECORDING);
        err = afxError_NOT_READY;
        return err;
    }

    // If dctx is a prime context, there must not be an active draw scope.
    if (dctx->inDrawScope)
    {
        if (dctx->cmdFlags & avxCmdFlag_INCURRENT)
        {
            AFX_ASSERT(!dctx->inDrawScope || (dctx->cmdFlags & avxCmdFlag_INCURRENT));
            err = afxError_INSUFFICIENT;
            return err;
        }
    }

    // Any and every query activated during the recording of dctx must have been deactivated.
    // TODO: Do it.
    // If dctx is a aux context, there must not be any remaining BeginDebugUtilsLabelEXT command recorded to dctx that has not previously been ended by a call to EndDebugUtilsLabelEXT.
    // TODO: Do it.

    AFX_ASSERT(dctx->ddi->compile);
    afxError err2 = dctx->ddi->compile(dctx);

    if (err2)
    {
        AfxThrowError();
        err = err2;

        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileDrawCommands, and the draw context will be moved to the invalid state.

        AFX_ASSERT(dctx->state == avxContextState_INVALID);
    }
    else
    {
        // The draw context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(dctx->state == avxContextState_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }
    return err;
}

_AVX afxError _AvxDctxExhaustCb(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    /*
        vkResetCommandPool does not free command buffers. It invalidates all command buffers allocated from that pool,
        meaning any recorded commands are discarded and the buffers return to the initial state (just allocated, but empty).
        They are not freed; they still exist, and we can re-record commands into them.
    */

    while (1)
    {
        afxUnit leftCnt = 0;
        afxDrawContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&dctx->dctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != avxContextState_RECORDING);
            AFX_ASSERT(aux->state != avxContextState_PENDING);
#if 0
            while (AfxLoadAtom32(&aux->submCnt))
            {
                AfxYield();
            }
#else
            if (AfxLoadAtom32(&aux->submCnt))
            {
                ++leftCnt;
                continue;
            }
#endif
            if (AvxPrepareDrawCommands(aux, freeMem, NIL))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AvxPrepareDrawCommands(dctx, freeMem, NIL))
        AfxThrowError();

    if (dctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(dctx->objsToBeDisposed.pop, dctx->objsToBeDisposed.items);

        AfxEmptyArray(&dctx->objsToBeDisposed, !freeMem, FALSE);
    }

    if (freeMem)
    {
        AfxExhaustArena(&dctx->cmdArena);
        //AfxExhaustPool(&dctx->batches, !freeMem);
    }

    return err;
}

_AVX afxError AvxExhaustDrawContext(afxDrawContext dctx, afxBool freeMem)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // dctx must be a pool context.
    if (dctx->pool == dctx)
    {
        AFX_ASSERT((dctx->pool == dctx));
        err = afxError_INVALID;
        return err;
    }

    AFX_ASSERT(dctx->ddi->exhaust);
    afxError err2 = dctx->ddi->exhaust(dctx, freeMem);

    if (err2)
    {
        AfxThrowError();
        err = err2;
    }
    else
    {
        //AFX_ASSERT(dctx->state == avxContextState_INITIAL);
        AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
    }
    return err;
}

_AVX afxError _AvxDctxRecycleCb(afxDrawContext dctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&dctx->submCnt))
    {
        AfxThrowError();
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from DPU as there not a lock mechanism for arena and batches' pool.

#if 0
    if (freeRes)
    {
        AFX_ASSERT(freeRes == TRUE);
        AfxExhaustArena(&cmdb->cmdArenaCompiled);
    }
    else
    {
        _avxCmd* cmd;
        AFX_ITERATE_CHAIN(_avxCmd, cmd, hdr.script, &cmdb->commands)
        {
            AfxPopLink(&cmd->hdr.script);
            AfxReclaimArena(&cmdb->cmdArenaCompiled, cmd, cmd->hdr.siz);
        }
    }
#else
    //AfxExhaustArena(&cmdb->cmdArenaCompiled);
#endif

    //AfxMakeChain(&cmdb->commands, cmdb);

    //afxBool recycled = FALSE;
#if 0
    if (3 > dctx->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &dctx->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

#if 0
    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&dctx->batches, AfxHere(), 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&dctx->batches, FALSE);
    }
#endif

    AvxPrepareDrawCommands(dctx, freeRes, NIL);

    afxDrawContext pool = AvxGetCommandPool(dctx);
    // Command pool can not be recycled by itself.

    if (pool != dctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &dctx);
    }

    return err;
}

_AVX _avxDctxDdi const _AVX_DDI_DCTX =
{
    .prepare = _AvxDctxPrepareCb,
    .compile = _AvxDctxCompileCb,
    .recycle = _AvxDctxRecycleCb,
    .exhaust = _AvxDctxExhaustCb
};

_AVX afxError _AvxDctxDtorCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AvxExhaustDrawContext(dctx, TRUE);

    AFX_ASSERT(dctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&dctx->objsToBeDisposed, FALSE, FALSE);

    AfxExhaustInterlockedQueue(&dctx->recycQue);

    AfxDismantleArena(&dctx->cmdArena);

    AfxDeregisterChainedClasses(&dctx->classes);

    return err;
}

_AVX afxError _AvxDctxCtorCb(afxDrawContext dctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawContext pool = AFX_CAST(afxDrawContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
    AFX_ASSERT(args[2]);
    avxContextConfig const* info = AFX_CAST(avxContextConfig const*, args[2]);
    afxClassConfig const* pDctxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    dctx->ddi = &_AVX_DDI_DCTX;
    dctx->udd = info->udd;
    dctx->tag = info->tag;
    dctx->devLimits = _AvxDsysGetLimits(dsys);
    dctx->enabledFeatures = _AvxDsysGetReqFeatures(dsys);

    AfxMakeChain(&dctx->classes, dctx);

    if (!pool)
    {
        dctx->pool = dctx;

        dctx->caps = info->caps;
        dctx->exuMask = info->exuMask;

        afxClassConfig dctxClsCfg = pDctxClsCfg ? *pDctxClsCfg : _AVX_CLASS_CONFIG_DCTX;
        AFX_ASSERT(dctxClsCfg.fcc == afxFcc_DCTX);
        dctxClsCfg.unitsPerPage = AFX_OR(AFX_MIN(info->auxCnt, 32), dctxClsCfg.unitsPerPage);
        AfxMountClass(&dctx->dctxCls, NIL, &dctx->classes, &dctxClsCfg);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&dctx->recycQue, sizeof(afxDrawContext), AFX_MAX(AFX_MIN(info->recycCap, 32), 1));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);
        
        // If there is a base, the base must be its own base.
        // Effectively, a aux context can only be acquired from a base context.
        // By allowing it to be itself, we allow the code to proceed normally avoid testing against NIL.
        if (pool->pool && (pool->pool != pool))
        {
            AfxThrowError();
            return err;
        }
        dctx->pool = pool;

        dctx->caps = (pool->caps & info->caps);
        dctx->exuMask = (pool->exuMask & info->exuMask);

        dctx->dctxCls = (afxClass) { 0 };

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&dctx->recycQue, sizeof(afxDrawContext), 1);
    }

    dctx->flags = info->flags;
    dctx->cmdFlags = info->cmdFlags;
    dctx->state = avxContextState_INITIAL;

    AfxMakeChain(&dctx->commands, dctx);
    AfxMakeArena(&dctx->cmdArena, NIL, AfxHere());

    AfxMakeArray(&dctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    dctx->inVideoCoding = FALSE;
    dctx->inDrawScope = FALSE;
    dctx->dbgUtilOpenLabelCnt = 0;

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_DCTX =
{
    .fcc = afxFcc_DCTX,
    .name = "DrawContext",
    .desc = "Device-Execution Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AvxDctxCtorCb,
    .dtor = (void*)_AvxDctxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireDrawContexts(afxDrawSystem dsys, afxDrawContext pool, avxContextConfig const* info, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };
    // dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    // contexts must be a valid pointer to an array of @cnt * afxDrawContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid avxContextConfig structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AvxDsysSW_GetDctxClassCb(dsys);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { dsys, pool, info }))
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);
        return err;
    }

    // If @pool is not NIL, @pool must be a valid afxDrawContext handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_DCTX, 1, &pool);

    if (pool->pool != pool)
    {
        AFX_ASSERT(pool->pool != pool);
        AfxThrowError();
        err = afxError_INVALID_HANDLE;
        return err;
    }

    afxBool once = !(info->cmdFlags & avxCmdFlag_RECURRENT);
    afxBool deferred = (info->cmdFlags & avxCmdFlag_INCURRENT);

    afxUnit recycCnt = 0;
    while (1)
    {
        // Firstly, try recycling contexts.

        afxDrawContext aux;
        if (AfxPopInterlockedQueue(&pool->recycQue, &aux))
        {
            AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &aux);
            AFX_ASSERT(aux->submCnt == 0);
            AFX_ASSERT(aux->submQueMask == 0);

            AvxPrepareDrawCommands(aux, TRUE, info->cmdFlags);

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
        cls = (afxClass*)_AvxDctxGetDctxClass(pool);
        AFX_ASSERT_CLASS(cls, afxFcc_DCTX);
        if (AfxAcquireObjects(cls, remainCnt, (afxObject*)&contexts[recycCnt], (void const*[]) { dsys, pool, info }))
        {
            AfxThrowError();

            // If acquisition failed, dispose all recycled contexts.
            // TODO: Recycle it.
            AfxDisposeObjects(recycCnt, contexts);
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_DCTX, cnt, contexts);
    return err;
}

_AVX afxError AvxRecycleDrawContexts(afxBool freeRes, afxUnit cnt, afxDrawContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxDrawContext dctx = contexts[iter];
        if (!dctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
        AFX_ASSERT(dctx->state != avxContextState_PENDING);

        AFX_ASSERT(dctx->ddi->recycle);
        if (dctx->ddi->recycle(dctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(dctx->state == avxContextState_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&dctx->commands));
        }
    }
    return err;
}

_AVX afxError AvxExecuteDrawCommands(afxDrawSystem dsys, afxUnit cnt, avxSubmission const submissions[], afxUnit queueingMap[])
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    /*
        If any draw context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a draw context complete, it moves from the pending state, back to the executable state.
        If a draw context was recorded with the ONCE flag, it instead moves back to the invalid state.
    */

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        avxSubmission const* subm = &submissions[ctxIt];

        afxDrawContext dctx = subm->dctx;

        if (!dctx)
        {
            AFX_ASSERT(subm->dctx);
            AfxThrowError();
            continue;
        }

        AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

        AFX_ASSERT(!(dctx->cmdFlags & avxCmdFlag_INCURRENT));

        if (dctx->state != avxContextState_EXECUTABLE)
        {
            if ((dctx->state == avxContextState_PENDING) ||
                (dctx->state == avxContextState_INTERNAL_EXECUTING))
            {
                if (!(dctx->cmdFlags & avxCmdFlag_CONCURRENT))
                {
                    AFX_ASSERT(!(dctx->cmdFlags & avxCmdFlag_CONCURRENT));
                    AfxThrowError();
                    continue;
                }
            }
        }

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxDrawBridge dexu;
                if (!AvxChooseDrawBridges(dsys, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxDrawQueue dque;
                    afxUnit nextQueIdx = 0;
                    while (AvxGetDrawQueues(dexu, nextQueIdx, 1, &dque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_DQUE, 1, &dque);
                        ++nextQueIdx;

                        afxError err2 = AvxSubmitDrawCommands(dque, 1, subm);

                        if (!err2)
                        {
                            if (queueingMap)
                            {
                                queueingMap[ctxIt] = nextQueIdx - 1;
                            }

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

  //////////////////////////////////////////////////////////////////////////////
 // STANDARD DEBUGGING AND EXECUTION COMMANDS                                //
//////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxCmdCommenceDebugScope(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // Bump up the label.
    ++dctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CommenceDebugScope), sizeof(cmd->CommenceDebugScope), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->CommenceDebugScope.label, name);

    if (color)
        AvxCopyColor(cmd->CommenceDebugScope.color, color);
    else
        AvxResetColor(cmd->CommenceDebugScope.color);

    return err;
}

_AVX afxError AvxCmdConcludeDebugScope(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // There must be an open debug scope.
    AFX_ASSERT(dctx->dbgUtilOpenLabelCnt > 0);
    --dctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ConcludeDebugScope), sizeof(cmd->ConcludeDebugScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDebugScope.nothing = NIL;
    return err;
}

_AVX afxError AvxCmdMarkDebugMilestone(afxDrawContext dctx, afxString const* name, avxColor const color)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(MarkDebugMilestone), sizeof(cmd->MarkDebugMilestone), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->MarkDebugMilestone.label, name);

    if (color)
        AvxCopyColor(cmd->MarkDebugMilestone.color, color);
    else
        AvxResetColor(cmd->MarkDebugMilestone.color);

    return err;
}

_AVX afxError AvxCmdExecuteCommands(afxDrawContext dctx, afxUnit cnt, afxDrawContext auxs[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ExecuteCommands), sizeof(cmd->ExecuteCommands) + (cnt * sizeof(cmd->ExecuteCommands.contexts[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ExecuteCommands.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->ExecuteCommands.contexts[i].dctx = auxs[i];
    }
    return err;
}
