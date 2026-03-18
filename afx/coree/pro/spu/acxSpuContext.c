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
// This software is part of Advanced Multimedia Extensions.

#define _ACX_WARP_C
#define _ACX_WARP_BRIDGE_C
#define _ACX_WARP_CONTEXT_C
#define _ACX_BUFFER_C
#include "../acxIcd.h"

_ACX _acxCmdLut _ACX_SPU_CMD_VMT =
{

};

_ACX afxError _AcxSpuRollWarpContexts(acxSpu* spu, afxWarpContext sctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCTX, 1, &sctx);

    switch (sctx->state)
    {
    case acxContextState_PENDING:
    {
        AfxIncAtom32(&sctx->submCnt);
        sctx->state = acxContextState_INTERNAL_EXECUTING;
        break;
    }
    case acxContextState_INTERNAL_EXECUTING:
    {
        AFX_ASSERT((sctx->cmdFlags & acxCmdFlag_SHARED));
        AfxIncAtom32(&sctx->submCnt);
        break;
    }
    default:
    {
        AFX_ASSERT( (sctx->state == acxContextState_PENDING) ||
                    (sctx->state == acxContextState_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
    }
    }

    afxWarpBridge sexu = spu->sexu;
    _acxCmdLut const* cmdVmt = /*spu->cmdVmt*/ &_ACX_SPU_CMD_VMT;

    _acxCmd* cmdHdr;
    afxCmdId lastId = 0; // DBG
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &sctx->commands)
    {
#ifdef _AFX_DEBUG
        lastId = cmdHdr->hdr.id;
#endif
        if (cmdHdr->hdr.id == NIL)
            break;

#if 0
        if (sctx->state != acxContextState_PENDING)
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
    case acxContextState_INTERNAL_EXECUTING:
    {
        if (0 == AfxDecAtom32(&sctx->submCnt))
        {
            if (sctx->cmdFlags & acxCmdFlag_ONCE)
            {
                sctx->state = acxContextState_INVALID;
                AcxPrepareWarpCommands(sctx, FALSE, NIL);
            }
            else
            {
                sctx->state = acxContextState_EXECUTABLE;
            }
        }
        else
        {
            AFX_ASSERT((sctx->cmdFlags & acxCmdFlag_SHARED));
        }
        break;
    }
    default:
    {
        AFX_ASSERT((sctx->state == acxContextState_INTERNAL_EXECUTING));
        AfxThrowError();
        return err;
}
    }
    return err;
}

