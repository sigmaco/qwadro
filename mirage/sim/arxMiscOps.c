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


_ARX afxCmdId AsxCmdBindMemory(arxContext ctx, afxUnit slot, void* ptr, afxSize siz)
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BindMemory), sizeof(cmd->BindMemory), &cmdId);
    AFX_ASSERT(cmd);

    cmd->BindMemory.slot = slot;
    cmd->BindMemory.ptr = ptr;
    cmd->BindMemory.siz = siz;

    return cmdId;
}

_ARX afxCmdId AsxCmdBindBuffers(arxContext ctx, afxUnit baseSlot, afxUnit cnt, avxBuffer buffers[], afxUnit const offsets[], afxUnit const ranges[])
{
    afxError err = { 0 };
    // ctx must be a valid arxContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_CTX, 1, &ctx);
    // ctx must be in the recording state.
    AFX_ASSERT(ctx->state == asxContextState_RECORDING);

    afxCmdId cmdId;
    asxCmd* cmd = _AsxCtxPushCmd(ctx, ASX_GET_STD_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.items[0])), &cmdId);
    AFX_ASSERT(cmd);

    cmd->BindBuffers.baseSlot = baseSlot;
    cmd->BindBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];
        afxUnit offset = offsets ? offsets[i] : 0;
        afxUnit range = ranges ? ranges[i] : 0;

        if (buf)
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufCap, offset, range);

            offset = AFX_MIN(offset, bufCap - 1);
            range = AFX_MIN(range ? range : bufCap - offset, bufCap - offset);

        }
        else
        {
            AFX_ASSERT(offsets && offsets[i]);
            AFX_ASSERT(ranges && ranges[i]);
            AFX_ASSERT(offset);
            AFX_ASSERT(range);
        }
        cmd->BindBuffers.items[i].buf = buf;
        cmd->BindBuffers.items[i].offset = offset;
        cmd->BindBuffers.items[i].range = range;
    }
    return cmdId;
}
