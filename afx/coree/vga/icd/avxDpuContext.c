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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_BUFFER_C
#include "../icd/avxIcd.h"

_AVX afxError _AvxDpuCmd_CopyBufferCb(avxDpu* dpu, _avxCmd* cmd)
{
    afxError err = { 0 };
    //cmd->CopyBuffer.
    return err;
}

_AVX _avxCmdLut const _AVX_DPU_CMD_VMT =
{
    .CopyBuffer = _AvxDpuCmd_CopyBufferCb
};

_AVX afxError _AvxDpuRollContext(avxDpu* dpu, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    switch (dctx->state)
    {
    case avxContextState_PENDING:
    {
        dctx->execExuMask |= AFX_BITMASK(dpu->exuIdx);
        dctx->state = avxContextState_INTERNAL_EXECUTING;
        break;
    }
    case avxContextState_INTERNAL_EXECUTING:
    {
        AFX_ASSERT(_AvxDctxIsConcurrent(dctx));
        dctx->execExuMask |= AFX_BITMASK(dpu->exuIdx);
        break;
    }
    default:
    {
        AFX_ASSERT( (dctx->state == avxContextState_PENDING) ||
                    (dctx->state == avxContextState_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
    }
    }

    afxDrawBridge dexu = dpu->dexu;
    _avxCmdLut const*cmdVmt = dexu->cmdVmt;
    afxCmdId lastId = 0; // DBG

    _avxCmd *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &dctx->commands)
    {
#ifdef _AFX_DEBUG
        lastId = cmdHdr->hdr.id;
#endif

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }
#if 0
        if (cmdb->state != avxContextState_PENDING)
        {
            AfxThrowError();
            break;
        }
#endif

        AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        cmdVmt->f[cmdHdr->hdr.id](dpu, cmdHdr);
    }

    dctx->execExuMask &= ~AFX_BITMASK(dpu->exuIdx);

    afxUnit submCnt = 0;
    if (0 == (submCnt = AfxAtomicDec32(&dctx->submCnt)))
    {
        if (!(_AvxDctxIsRecurrent(dctx)))
        {
            dctx->state = avxContextState_INVALID;
        }
        else
        {
            dctx->state = avxContextState_EXECUTABLE;
        }
    }
    else
    {
        AFX_ASSERT(_AvxDctxIsConcurrent(dctx));

        dctx->state = avxContextState_PENDING;
    }

    return err;
}
