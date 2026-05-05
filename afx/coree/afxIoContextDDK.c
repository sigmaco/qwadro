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
#define _AFX_IO_C
//#define _AFX_IO_SYSTEM_C
//#define _AFX_IO_DEVICE_C
#define _AFX_IO_BRIDGE_C
#define _AFX_IO_QUEUE_C
#define _AFX_IO_CONTEXT_C
#include "../afxSystemDDK.h"

_AFX afxError AfxCmdCommenceDebugScope(afxIoContext ctx, afxString const* name, afxV4d const color)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);

    // Bump up the label.
    ++ctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _afxCmd* cmd = _AfxCtxPushCmd(ctx, _AFX_CMD_ID(CommenceDebugScope), sizeof(cmd->CommenceDebugScope), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->CommenceDebugScope.label, name);

    if (color)
        AfxCopyColor(cmd->CommenceDebugScope.color, color);
    else
        AfxResetColor(cmd->CommenceDebugScope.color);

    return err;
}

_AFX afxError AfxCmdConcludeDebugScope(afxIoContext ctx)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);

    // There must be an open debug scope.
    AFX_ASSERT(ctx->dbgUtilOpenLabelCnt > 0);
    --ctx->dbgUtilOpenLabelCnt;

    afxCmdId cmdId;
    _afxCmd* cmd = _AfxCtxPushCmd(ctx, _AFX_CMD_ID(ConcludeDebugScope), sizeof(cmd->ConcludeDebugScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDebugScope.nothing = NIL;
    return err;
}

_AFX afxError AfxCmdMarkDebugMilestone(afxIoContext ctx, afxString const* name, afxV4d const color)
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);

    afxCmdId cmdId;
    _afxCmd* cmd = _AfxCtxPushCmd(ctx, _AFX_CMD_ID(MarkDebugMilestone), sizeof(cmd->MarkDebugMilestone), &cmdId);
    AFX_ASSERT(cmd);

    AfxMakeString2048(&cmd->MarkDebugMilestone.label, name);

    if (color)
        AfxCopyColor(cmd->MarkDebugMilestone.color, color);
    else
        AfxResetColor(cmd->MarkDebugMilestone.color);

    return err;
}

_AFX afxError AfxCmdExecuteCommands(afxIoContext ctx, afxUnit cnt, afxIoContext auxs[])
{
    afxError err = { 0 };
    // ctx must be a valid afxIoContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);

    AFX_ASSERT(cnt);

    afxCmdId cmdId;
    _afxCmd* cmd = _AfxCtxPushCmd(ctx, _AFX_CMD_ID(ExecuteCommands), sizeof(cmd->ExecuteCommands) + (cnt * sizeof(cmd->ExecuteCommands.contexts[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ExecuteCommands.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        cmd->ExecuteCommands.contexts[i].ctx = auxs[i];
    }
    return err;
}

_AFX afxError _AfxCtxExhaustCb(afxIoContext ctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    /*
        vkResetCommandPool does not free command buffers. It invalidates all command buffers allocated from that pool, 
        meaning any recorded commands are discarded and the buffers return to the initial state (just allocated, but empty).
        They are not freed; they still exist, and we can re-record commands into them.
    */

    while (1)
    {
        afxUnit leftCnt = 0;
        afxIoContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&ctx->ctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != afxContextState_RECORDING);
            AFX_ASSERT(aux->state != afxContextState_PENDING);
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
            if (AfxPrepareIoCommands(aux, freeMem, NIL))
                AfxThrowError();
        }

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AfxPrepareIoCommands(ctx, freeMem, NIL))
        AfxThrowError();

    if (ctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(ctx->objsToBeDisposed.pop, ctx->objsToBeDisposed.items);

        AfxEmptyArray(&ctx->objsToBeDisposed, !freeMem, FALSE);
    }

    if (freeMem)
    {
        AfxExhaustArena(&ctx->cmdArena);
        //AfxExhaustPool(&ctx->batches, !freeMem);
    }

    return err;
}

_AFX afxError _AfxCtxPrepareCb(afxIoContext ctx, afxBool purge, afxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // AFX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&ctx->cmdArena);
    }
#else
    AfxExhaustArena(&ctx->cmdArena);
#endif
    AfxMakeChain(&ctx->commands, ctx);

    ctx->inIoScope = FALSE;
    ctx->inVideoCoding = FALSE;

    ctx->state = afxContextState_RECORDING;
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);

    return err;
}

_AFX afxError _AfxCtxCompileCb(afxIoContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // AFX assumes the compilation when ICD does not take the front.
    
    AFX_ASSERT(ctx->state == afxContextState_RECORDING);
    ctx->state = afxContextState_EXECUTABLE;

    return err;
}

_AFX afxError _AfxCtxRecycleCb(afxIoContext ctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // AFX assumes the compilation when ICD does not take the front.

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&ctx->submCnt))
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
        _afxCmd* cmd;
        AFX_ITERATE_CHAIN(_afxCmd, cmd, hdr.script, &cmdb->commands)
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
    if (3 > ctx->cmdbRecycChain.cnt)
    {
        AfxPushLink(&cmdb->recyc, &ctx->cmdbRecycChain);
        recycled = TRUE;
    }
    // If could not enqueue for recyclage, destroy it.
#endif

#if 0
    if (!recycled)
    {
        //AfxExhaustArena(&cmdb->cmdArenaCompiled);
        AfxDismantleArena(&cmdb->cmdArenaCompiled);

        if (AfxReclaimPoolUnits(&ctx->batches, AfxHere(), 1, (void**)&cmdb))
        {
            AfxThrowError();
        }
        //AfxExhaustPool(&ctx->batches, FALSE);
    }
#endif

    AfxPrepareIoCommands(ctx, freeRes, NIL);

    afxIoContext pool = AfxGetCommandPool(ctx);
    // Command pool can not be recycled by itself.

    if (pool != ctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &ctx);
    }

    return err;
}

_AFX _afxDdiCtx const _AFX_DDI_CTX =
{
    .prepare = _AfxCtxPrepareCb,
    .compile = _AfxCtxCompileCb,
    .recycle = _AfxCtxRecycleCb,
    .exhaust = _AfxCtxExhaustCb
};
