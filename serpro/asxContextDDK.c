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

_ASX afxError _AsxSctxExhaustCb(afxStepContext sctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    while (1)
    {
        afxUnit leftCnt = 0;
        afxStepContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&sctx->sctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != asxContextState_RECORDING);
            AFX_ASSERT(aux->state != asxContextState_PENDING);
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
            if (AsxPrepareStepCommands(aux, freeMem, NIL))
                AfxThrowError();
}

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AsxPrepareStepCommands(sctx, freeMem, NIL))
        AfxThrowError();
#if 0
    if (sctx->objsToBeDisposed.pop)
    {
        AfxDisposeObjects(sctx->objsToBeDisposed.pop, sctx->objsToBeDisposed.items);

        AfxEmptyArray(&sctx->objsToBeDisposed, !freeMem, FALSE);
    }
#endif

    if (freeMem)
    {
        AfxExhaustArena(&sctx->cmdArena);
        //AfxExhaustPool(&sctx->batches, !freeMem);
    }
    return err;
}

_ASX afxError _AsxSctxPrepareCb(afxStepContext sctx, afxBool purge, asxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    // ASX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&sctx->cmdArena);
    }
#else
    AfxExhaustArena(&sctx->cmdArena);
#endif
    AfxMakeChain(&sctx->commands, sctx);

    sctx->state = asxContextState_RECORDING;
    AFX_ASSERT(sctx->state == asxContextState_RECORDING);

    return err;
}

_ASX afxError _AsxSctxCompileCb(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    // ASX assumes the compilation when ICD does not take the front.

    AFX_ASSERT(sctx->state == asxContextState_RECORDING);
    sctx->state = asxContextState_EXECUTABLE;

    return err;
}

_ASX afxError _AsxSctxRecycleCb(afxStepContext sctx, afxBool freeRes)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    // Should wait or return?
    // On the next roll, it should be recycled anyway.
#if 0
    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }
#else
    if (AfxLoadAtom32(&sctx->submCnt))
    {
        AfxThrowError();
        return afxError_BUSY;
    }
#endif

    // There is some issues if it is called from MPU as there not a lock mechanism for arena and batches' pool.

    AsxPrepareStepCommands(sctx, freeRes, NIL);

    afxStepContext pool = AsxGetCommandPool(sctx);
    // Command pool can not be recycled by itself.

    if (pool != sctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &sctx);
    }

    return err;
}

_ASX _asxDdiSctx const _ASX_SCTX_DDI =
{
    .prepare = _AsxSctxPrepareCb,
    .compile = _AsxSctxCompileCb,
    .recycle = _AsxSctxRecycleCb,
    .exhaust = _AsxSctxExhaustCb
};
