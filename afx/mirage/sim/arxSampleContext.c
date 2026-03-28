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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _ASX_ENGINE_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#include "../sim/asxImplementation.h"

_ARX asxCmd* _AsxCtxPushCmd(arxContext ctx, afxUnit id, afxUnit siz, afxCmdId* cmdId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    AFX_ASSERT(siz >= sizeof(_acxCmdHdr));

    asxCmd* cmd = AfxRequestArena(&ctx->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AFX_ASSERT(cmdId);
    *cmdId = AfxPushLink(&cmd->hdr.script, &ctx->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

////////////////////////////////////////////////////////////////////////////////

_ARX asxContextState _AsxGetCatalystStatus(arxContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return ctx->state;
}

_ARX afxUnit ArxGetCatalystPort(arxContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return ctx->portId;
}

_ARX afxUnit ArxGetCatalystPool(arxContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    return ctx->poolIdx;
}

_ARX afxError ArxCompileSimCommands(arxContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    // If there was an error during recording, the application will be notified by an unsuccessful return 
    // code returned by AfxEndCommandBuffer, and the command buffer will be moved to the invalid state.
    // The command buffer must have been in the recording state, and, if successful, is moved to the executable state.

    if (ctx->state != asxContextState_RECORDING)
    {
        AfxThrowError();
        return err;
    }
    else
    {
        if (ctx->endCb(ctx))
        {
            AfxThrowError();
            return err;
        }
        else
        {
            ctx->state = asxContextState_EXECUTABLE;
        }
    }
    return err;
}

_ARX afxError ArxRecycleCatalyst(arxContext ctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    arxSimQueue sque = AfxGetHost(ctx);
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    //afxUnit poolIdx = ctx->poolIdx;

    if (AfxTryLockFutex(&sque->cmdbReqLock, FALSE))
    {
        if (AfxPushQueue(&sque->cmdbRecycQue, &ctx))
        {
            AfxDisposeObjects(1, &ctx);
        }
        else
        {
            ctx->state = asxContextState_INVALID;
        }
        AfxUnlockFutex(&sque->cmdbReqLock, FALSE);
    }
    else
    {
        AfxDisposeObjects(1, &ctx);
    }
    return err;
}

_ARX afxError _AsxCtxDtorCb(arxContext ctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    while (AfxLoadAtom32(&ctx->submCnt))
    {
        AfxYield();
    }

    AFX_ASSERT(ctx->state != asxContextState_PENDING);

    //AfxRecycleCmdBuffers(TRUE, 1, &ctx);

    //if (ctx->base.vmt->dtor && ctx->base.vmt->dtor(ctx))
        //AfxThrowError();

    //AFX_ASSERT(!ctx->base.idd);

    AfxDismantleArena(&ctx->cmdArena);

    return err;
}

_ARX afxError _AsxCtxCtorCb(arxContext ctx, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

    arxSimQueue sque = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    //arxSimulation dsys = AfxGetSimBridgeContext(sexu);

    ctx->submCnt = 0;
    ctx->submQueMask = NIL;
    ctx->portId = AfxGetSampleQueuePort(sque);
    ctx->poolIdx = AfxGetObjectId(sque);

    ctx->disposable = TRUE;

    ctx->state = asxContextState_INITIAL;

    AfxMakeChain(&ctx->commands, ctx);
    AfxMakeArena(&ctx->cmdArena, NIL, AfxHere());

    ctx->endCb = _AsxCtxEndCb;
    ctx->resetCb = _AsxCtxResetCb;

    return err;
}

_ARX afxClassConfig const _ASX_CTX_CLASS_CONFIG =
{
    .fcc = afxFcc_CTX,
    .name = "Context",
    .desc = "Sample Context",
    .fixedSiz = sizeof(AFX_OBJECT(arxContext)),
    .ctor = (void*)_AsxCtxCtorCb,
    .dtor = (void*)_AsxCtxDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireSampleContexts(arxSimulation dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, arxContext batches[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &dsys);
    AFX_ASSERT(batches);
    AFX_ASSERT(cnt);

    arxSimBridge sexu;
    if (!ArxGetSimBridges(dsys, exuIdx, 1, &sexu))
    {
        AfxThrowError();
        return afxError_BAD_UNIT;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SEXU, 1, &sexu);

    arxSimQueue sque;
    if (!ArxGetSimQueues(sexu, queIdx, 1, &sque))
    {
        AfxThrowError();
        return afxError_BAD_UNIT;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SQUE, 1, &sque);

    if (!AfxTryLockFutex(&sque->cmdbReqLock, FALSE))
    {
        AfxThrowError();
        return afxError_TIMEOUT;
    }

    afxUnit cnt2 = 0;
    arxContext* unit;
    while ((unit = AfxPeekQueue(&sque->cmdbRecycQue)))
    {
        arxContext ctx = *unit;
        AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

        if (ctx->resetCb(ctx, TRUE, 0)) AfxThrowError();
        else
        {
            AFX_ASSERT(ctx->state == asxContextState_RECORDING);

            batches[cnt2] = ctx;
            ++cnt2;
        }

        if (err)
            AfxDisposeObjects(1, &ctx);

        AfxPopQueue(&sque->cmdbRecycQue);

        if (cnt2 == cnt)
            break;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CTX, cnt2, batches);

    AfxUnlockFutex(&sque->cmdbReqLock, FALSE);

    if (cnt2 < cnt)
    {
        afxUnit cnt3 = cnt - cnt2;

        afxClass* cls = (afxClass*)_AsxGetCatalystClass(sque);
        AFX_ASSERT_CLASS(cls, afxFcc_CTX);

        if (AfxAcquireObjects(cls, cnt3, (afxObject*)&batches[cnt2], (void const*[]) { sque }))
        {
            AfxThrowError();

            for (afxUnit j = cnt2; j-- > 0;)
                ArxRecycleCatalyst(batches[j], TRUE);

            cnt2 = 0;
        }
        else
        {
            for (afxUnit k = 0; k < cnt3; k++)
            {
                arxContext ctx = batches[cnt2 + k];
                AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);

                if (ctx->resetCb(ctx, TRUE, 0)) AfxThrowError();
                else
                {
                    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

                    batches[cnt2] = ctx;
                    ++cnt2;
                }
            }
            AFX_ASSERT_OBJECTS(afxFcc_CTX, cnt2, batches);
        }
    }
    return err;
}
