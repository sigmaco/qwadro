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

_ACX afxError _AcxSctxExhaustCb(afxStepContext sctx, afxBool freeMem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    while (1)
    {
        afxUnit leftCnt = 0;
        afxStepContext aux;
        for (afxUnit i = 0; AfxEnumerateObjects(&sctx->sctxCls, i, 1, (void**)&aux); i++)
        {
            AFX_ASSERT(aux->state != acxContextState_RECORDING);
            AFX_ASSERT(aux->state != acxContextState_PENDING);
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
            if (AcxPrepareStepCommands(aux, freeMem, NIL))
                AfxThrowError();
}

        if (leftCnt)
            AfxYield();
        else
            break;
    }

    if (AcxPrepareStepCommands(sctx, freeMem, NIL))
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

_ACX afxError _AcxSctxPrepareCb(afxStepContext sctx, afxBool purge, acxCmdFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    // ACX assumes the compilation when ICD does not take the front.

#if 0
    if (purge)
    {
        AfxExhaustArena(&sctx->cmdArena);
    }
#else
    AfxExhaustArena(&sctx->cmdArena);
#endif
    AfxMakeChain(&sctx->commands, sctx);

    sctx->state = acxContextState_RECORDING;
    AFX_ASSERT(sctx->state == acxContextState_RECORDING);

    return err;
}

_ACX afxError _AcxSctxCompileCb(afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    // ACX assumes the compilation when ICD does not take the front.

    AFX_ASSERT(sctx->state == acxContextState_RECORDING);
    sctx->state = acxContextState_EXECUTABLE;

    return err;
}

_ACX afxError _AcxSctxRecycleCb(afxStepContext sctx, afxBool freeRes)
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

    AcxPrepareStepCommands(sctx, freeRes, NIL);

    afxStepContext pool = AcxGetCommandPool(sctx);
    // Command pool can not be recycled by itself.

    if (pool != sctx)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &pool);
        AfxPushInterlockedQueue(&pool->recycQue, &sctx);
    }

    return err;
}

_ACX _acxDdiSctx const _ACX_SCTX_DDI =
{
    .prepare = _AcxSctxPrepareCb,
    .compile = _AcxSctxCompileCb,
    .recycle = _AcxSctxRecycleCb,
    .exhaust = _AcxSctxExhaustCb
};
