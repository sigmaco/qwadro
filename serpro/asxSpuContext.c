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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _ASX_STEP_C
#define _ASX_STEP_BRIDGE_C
#define _ASX_STEP_CONTEXT_C
#define _ASX_BUFFER_C
#include "asxIcd.h"

_ASX _asxCmdLut _ASX_SPU_CMD_VMT =
{

};

_ASX afxError _AsxSpuRollStepContexts(asxSpu* spu, afxStepContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    switch (sctx->state)
    {
    case asxContextState_PENDING:
    {
        AfxIncAtom32(&sctx->submCnt);
        sctx->state = asxContextState_INTERNAL_EXECUTING;
        break;
    }
    case asxContextState_INTERNAL_EXECUTING:
    {
        AFX_ASSERT((sctx->cmdFlags & asxCmdFlag_SHARED));
        AfxIncAtom32(&sctx->submCnt);
        break;
    }
    default:
    {
        AFX_ASSERT( (sctx->state == asxContextState_PENDING) ||
                    (sctx->state == asxContextState_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
    }
    }

    afxStepBridge sexu = spu->sexu;
    _asxCmdLut const* cmdVmt = /*spu->cmdVmt*/ &_ASX_SPU_CMD_VMT;

    _asxCmd* cmdHdr;
    afxCmdId lastId = 0; // DBG
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &sctx->commands)
    {
#ifdef _AFX_DEBUG
        lastId = cmdHdr->hdr.id;
#endif
        if (cmdHdr->hdr.id == NIL)
            break;

#if 0
        if (sctx->state != asxContextState_PENDING)
        {
            AfxThrowError();
            break;
        }
#endif
        if (!cmdVmt->f[cmdHdr->hdr.id])
        {
            AFX_ASSERT(cmdVmt->f[cmdHdr->hdr.id]);
        }
        else
        {
            cmdVmt->f[cmdHdr->hdr.id](spu, cmdHdr);
        }
    }

    switch (sctx->state)
    {
    case asxContextState_INTERNAL_EXECUTING:
    {
        if (0 == AfxDecAtom32(&sctx->submCnt))
        {
            if (sctx->cmdFlags & asxCmdFlag_ONCE)
            {
                sctx->state = asxContextState_INVALID;
                AsxPrepareStepCommands(sctx, FALSE, NIL);
            }
            else
            {
                sctx->state = asxContextState_EXECUTABLE;
            }
        }
        else
        {
            AFX_ASSERT((sctx->cmdFlags & asxCmdFlag_SHARED));
        }
        break;
    }
    default:
    {
        AFX_ASSERT((sctx->state == asxContextState_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
}
    }
    return err;
}

