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
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

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

_AVX afxError _AvxDctxCompileCb(afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    // AVX assumes the compilation when ICD does not take the front.
    
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    dctx->state = avxContextState_EXECUTABLE;

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

_AVX _avxDdiDctx const _AVX_DDI_DCTX =
{
    .prepare = _AvxDctxPrepareCb,
    .compile = _AvxDctxCompileCb,
    .recycle = _AvxDctxRecycleCb,
    .exhaust = _AvxDctxExhaustCb
};
