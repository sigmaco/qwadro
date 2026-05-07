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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
//#define _AFX_IO_SYSTEM_C
//#define _AFX_IO_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_IO_CONTEXT_C
#include "afxSystemDDK.h"

_AFX _afxCmd* _AfxCtxPushCmd(afxIoContext ctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    AFX_ASSERT(siz >= sizeof(_afxCmdHdr));

    _afxCmd* cmd = AfxRequestArena(&ctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &ctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxCmdId AfxGetLastIoCommandId(afxIoContext ctx)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    afxCmdId cmdId = ctx->commands.cnt - 1;
    return cmdId;
}

_AFX afxCmdId AfxGetNextIoCommandId(afxIoContext ctx)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    afxCmdId cmdId = ctx->commands.cnt;
    return cmdId;
}

_AFX afxClass const* _AfxCtxGetCtxClass(afxIoContext ctx)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    afxClass const* cls = &ctx->ctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CTX);
    return cls;
}

_AFX afxContextState _AfxCtxGetStatus(afxIoContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return ctx->state;
}

_AFX afxMask AfxGetIoCommandPort(afxIoContext ctx, afxMask exuMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return exuMask ? (ctx->exuMask & exuMask) : ctx->exuMask;
}

_AFX afxAptitude AfxGetIoCommandAptitude(afxIoContext ctx, afxAptitude caps)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return caps ? (ctx->caps & caps) : ctx->caps;
}

_AFX afxIoContext AfxGetIoCommandPool(afxIoContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    afxIoContext pool = ctx->pool;
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &pool);
    return pool;
}

_AFX afxError AfxTraverseIoCommands(afxIoContext ctx, afxCmdId(*f)(void* udd, void* cmd), void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    return err;
}

_AFX afxError AfxPrepareIoCommands(afxIoContext ctx, afxBool purge, afxCmdFlags flags)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // ctx must not be in the PENDING state.
    if (ctx->state >= afxContextState_PENDING)
    {
        AFX_ASSERT(!(ctx->state >= afxContextState_PENDING));
        err = afxError_BUSY;
        return err;
    }

    AFX_ASSERT(ctx->ddi->prepare);
    afxError err2 = ctx->ddi->prepare(ctx, purge, flags);

    if (err2)
    {
        AfxThrowError();
        err = err2;
    }
    else
    {
        AFX_ASSERT(ctx->state == afxContextState_RECORDING);
        AFX_ASSERT(AfxIsChainEmpty(&ctx->commands));
    }

    return err;
}

_AFX afxError AfxCompileIoCommands(afxIoContext ctx)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // ctx must be in the RECORDING state.
    if (ctx->state != afxContextState_RECORDING)
    {
        AFX_ASSERT(ctx->state == afxContextState_RECORDING);
        err = afxError_NOT_READY;
        return err;
    }

    // If ctx is a prime context, there must not be an active IO scope.
    if (ctx->inIoScope)
    {
        if (ctx->cmdFlags & afxCmdFlag_INCURRENT)
        {
            AFX_ASSERT(!ctx->inIoScope || (ctx->cmdFlags & afxCmdFlag_INCURRENT));
            err = afxError_INSUFFICIENT;
            return err;
        }
    }

    // Any and every query activated during the recording of ctx must have been deactivated.
    // TODO: Do it.
    // If ctx is a aux context, there must not be any remaining BeginDebugUtilsLabelEXT command recorded to ctx that has not previously been ended by a call to EndDebugUtilsLabelEXT.
    // TODO: Do it.

    AFX_ASSERT(ctx->ddi->compile);
    afxError err2 = ctx->ddi->compile(ctx);

    if (err2)
    {
        AfxThrowError();
        err = err2;

        // If there was an error during recording, the application will be notified by an unsuccessful return 
        // code returned by AfxCompileIoCommands, and the IO context will be moved to the invalid state.

        AFX_ASSERT(ctx->state == afxContextState_INVALID);
    }
    else
    {
        // The IO context must have been in the recording state, and, if successful, is moved to the executable state.
        AFX_ASSERT(ctx->state == afxContextState_EXECUTABLE);
        //AFX_ASSERT(AfxIsChainEmpty(&ctx->commands));
    }
    return err;
}

_AFX afxError AfxExhaustIoContext(afxIoContext ctx, afxBool freeMem)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // ctx must be a pool context.
    if (ctx->pool == ctx)
    {
        AFX_ASSERT((ctx->pool == ctx));
        err = afxError_INVALID;
        return err;
    }

    AFX_ASSERT(ctx->ddi->exhaust);
    afxError err2 = ctx->ddi->exhaust(ctx, freeMem);

    if (err2)
    {
        AfxThrowError();
        err = err2;
    }
    else
    {
        //AFX_ASSERT(ctx->state == afxContextState_INITIAL);
        AFX_ASSERT(AfxIsChainEmpty(&ctx->commands));
    }
    return err;
}

_AFX afxError _AfxCtxDtorCb(afxIoContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    AfxExhaustIoContext(ctx, TRUE);

    AFX_ASSERT(ctx->objsToBeDisposed.pop == 0);
    AfxEmptyArray(&ctx->objsToBeDisposed, FALSE, FALSE);

    AfxExhaustInterlockedQueue(&ctx->recycQue);

    AfxDismantleArena(&ctx->cmdArena);

    AfxDeregisterChainedClasses(&ctx->classes);

    return err;
}

_AFX afxError _AfxCtxCtorCb(afxIoContext ctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    afxIoSystem iom = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxIoContext pool = AFX_CAST(afxIoContext, args[1]);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_CTX, 1, &pool);
    AFX_ASSERT(args[2]);
    afxContextConfig const* info = AFX_CAST(afxContextConfig const*, args[2]);
    afxClassConfig const* pCtxClsCfg = NIL;/* AFX_CAST(afxClassConfig const*, args[3]);*/

    ctx->ddi = &_AFX_DDI_CTX;
    ctx->udd = info->udd;
    ctx->tag = info->tag;
    ctx->devLimits = _AfxDsysGetLimits(iom);
    ctx->enabledFeatures = _AfxDsysGetReqFeatures(iom);

    AfxMakeChain(&ctx->classes, ctx);

    if (!pool)
    {
        ctx->pool = ctx;

        ctx->caps = info->caps;
        ctx->exuMask = info->exuMask;

        afxClassConfig ctxClsCfg = pCtxClsCfg ? *pCtxClsCfg : _AFX_CLASS_CONFIG_CTX;
        AFX_ASSERT(ctxClsCfg.fcc == afxFcc_CTX);
        ctxClsCfg.unitsPerPage = AFX_OR(AFX_MIN(info->auxCnt, 32), ctxClsCfg.unitsPerPage);
        AfxMountClass(&ctx->ctxCls, NIL, &ctx->classes, &ctxClsCfg);

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&ctx->recycQue, sizeof(afxIoContext), AFX_MAX(AFX_MIN(info->recycCap, 32), 1));
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &pool);
        
        // If there is a base, the base must be its own base.
        // Effectively, a aux context can only be acquired from a base context.
        // By allowing it to be itself, we allow the code to proceed normally avoid testing against NIL.
        if (pool->pool && (pool->pool != pool))
        {
            AfxThrowError();
            return err;
        }
        ctx->pool = pool;

        ctx->caps = (pool->caps & info->caps);
        ctx->exuMask = (pool->exuMask & info->exuMask);

        ctx->ctxCls = (afxClass) { 0 };

        // FIXME: Interlocked queue can not be zero.
        AfxMakeInterlockedQueue(&ctx->recycQue, sizeof(afxIoContext), 1);
    }

    ctx->flags = info->flags;
    ctx->cmdFlags = info->cmdFlags;
    ctx->state = afxContextState_INITIAL;

    AfxMakeChain(&ctx->commands, ctx);
    AfxMakeArena(&ctx->cmdArena, NIL, AfxHere());

    AfxMakeArray(&ctx->objsToBeDisposed, sizeof(afxObject), 4, NIL, 0);

    ctx->inVideoCoding = FALSE;
    ctx->inIoScope = FALSE;
    ctx->dbgUtilOpenLabelCnt = 0;

    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_CTX =
{
    .fcc = afxFcc_CTX,
    .name = "IoContext",
    .desc = "Device-Execution Context",
    .fixedSiz = sizeof(AFX_OBJECT(afxIoContext)),
    .unitsPerPage = 4,
    .ctor = (void*)_AfxCtxCtorCb,
    .dtor = (void*)_AfxCtxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireIoContexts(afxIoSystem iom, afxIoContext pool, afxContextConfig const* info, afxUnit cnt, afxIoContext contexts[])
{
    afxError err = { 0 };
    // iom must be a valid afxIoSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    // contexts must be a valid pointer to an array of @cnt * afxIoContext handles.
    AFX_ASSERT(contexts);
    // @info must be a valid pointer to a valid afxContextConfig structure.
    AFX_ASSERT(info);
    // @cnt must be greater than 0.
    AFX_ASSERT(cnt);

    AFX_ASSERT(32 >= info->auxCnt);
    AFX_ASSERT(32 >= info->recycCap);

    afxClass* cls;

    if (!pool)
    {
        cls = (afxClass*)_AfxDsysSW_GetCtxClassCb(iom);
        AFX_ASSERT_CLASS(cls, afxFcc_CTX);
        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { iom, pool, info }))
        {
            AfxThrowError();
            return err;
        }

        AFX_ASSERT_OBJECTS(afxFcc_CTX, cnt, contexts);
        return err;
    }

    // If @pool is not NIL, @pool must be a valid afxIoContext handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_CTX, 1, &pool);

    if (pool->pool != pool)
    {
        AFX_ASSERT(pool->pool != pool);
        AfxThrowError();
        err = afxError_INVALID_HANDLE;
        return err;
    }

    afxBool once = !(info->cmdFlags & afxCmdFlag_RECURRENT);
    afxBool deferred = (info->cmdFlags & afxCmdFlag_INCURRENT);

    afxUnit recycCnt = 0;
    while (1)
    {
        // Firstly, try recycling contexts.

        afxIoContext aux;
        if (AfxPopInterlockedQueue(&pool->recycQue, &aux))
        {
            AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &aux);
            AFX_ASSERT(aux->submCnt == 0);
            AFX_ASSERT(aux->submQueMask == 0);

            AfxPrepareIoCommands(aux, TRUE, info->cmdFlags);

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
        cls = (afxClass*)_AfxCtxGetCtxClass(pool);
        AFX_ASSERT_CLASS(cls, afxFcc_CTX);
        if (AfxAcquireObjects(cls, remainCnt, (afxObject*)&contexts[recycCnt], (void const*[]) { iom, pool, info }))
        {
            AfxThrowError();

            // If acquisition failed, dispose all recycled contexts.
            // TODO: Recycle it.
            AfxDisposeObjects(recycCnt, contexts);
            return err;
        }
    }

    AFX_ASSERT_OBJECTS(afxFcc_CTX, cnt, contexts);
    return err;
}

_AFX afxError AfxRecycleIoContexts(afxBool freeRes, afxUnit cnt, afxIoContext contexts[])
{
    afxError err = { 0 };

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxIoContext ctx = contexts[iter];
        if (!ctx) continue;
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
        AFX_ASSERT(ctx->state != afxContextState_PENDING);

        AFX_ASSERT(ctx->ddi->recycle);
        if (ctx->ddi->recycle(ctx, freeRes))
        {
            AfxThrowError();
        }
        else
        {
            AFX_ASSERT(ctx->state == afxContextState_INITIAL);
            AFX_ASSERT(AfxIsChainEmpty(&ctx->commands));
        }
    }
    return err;
}

_AFX afxError AfxExecuteIoCommands(afxIoSystem iom, afxUnit cnt, afxSubmission const submissions[], afxUnit queueingMap[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIoSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(submissions);
    AFX_ASSERT(cnt);

    /*
        If any IO context submitted to this queue is in the executable state, it is moved to the pending state.
        Once execution of all submissions of a IO context complete, it moves from the pending state, back to the executable state.
        If a IO context was recorded with the ONCE flag, it instead moves back to the invalid state.
    */

    for (afxUnit ctxIt = 0; ctxIt < cnt; ctxIt++)
    {
        afxSubmission const* subm = &submissions[ctxIt];

        afxIoContext ctx = subm->ctx;

        if (!ctx)
        {
            AFX_ASSERT(subm->ctx);
            AfxThrowError();
            continue;
        }

        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

        AFX_ASSERT(!(ctx->cmdFlags & afxCmdFlag_INCURRENT));

        if (ctx->state != afxContextState_EXECUTABLE)
        {
            if ((ctx->state == afxContextState_PENDING) ||
                (ctx->state == afxContextState_INTERNAL_EXECUTING))
            {
                if (!(ctx->cmdFlags & afxCmdFlag_CONCURRENT))
                {
                    AFX_ASSERT(!(ctx->cmdFlags & afxCmdFlag_CONCURRENT));
                    AfxThrowError();
                    continue;
                }
            }
        }

        afxMask exuMask = subm->exuMask;
        afxUnit exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, NIL, exuMask, 0, 0, NIL);
        afxUnit nextExuIdx = AfxRandom2(0, exuCnt - 1);

        afxBool queued = FALSE;

        while (1)
        {
            for (afxUnit exuIter = nextExuIdx; exuIter < exuCnt; exuIter++)
            {
                afxIoBridge dexu;
                if (!AfxChooseIoBridges(iom, AFX_INVALID_INDEX, NIL, exuMask, exuIter, 1, &dexu))
                {
                    AfxThrowError();
                    continue;
                }

                nextExuIdx = 0;

                while (1)
                {
                    afxIoQueue dque;
                    afxUnit nextQueIdx = 0;
                    while (AfxGetIoQueues(dexu, nextQueIdx, 1, &dque))
                    {
                        AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &dque);
                        ++nextQueIdx;

                        afxError err2 = AfxSubmitIoCommands(dque, 1, subm);

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
