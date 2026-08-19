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

 // This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "../icd/avxIcd.h"

_AVX afxError AvxCmdUseVertexInput(afxDrawContext dctx, avxVertexInput vin)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UseVertexInput), sizeof(cmd->UseVertexInput), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UseVertexInput.vin = vin;

    dctx->vertices.vin = vin;
    dctx->vertices.vinBindCmdId = cmdId;

    return err;
}

_AVX afxError AvxCmdBindVertexBuffers(afxDrawContext dctx, afxUnit basePin, afxUnit cnt, avxBufferedStream const streams[])
{
    afxError err = { 0 };

#if AVX_VALIDATION_ENABLED
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(streams);

#if AVX_LIMIT_VALIDATION_ENABLED
    /*
        @baseSlotIdx must be less than device limit 'maxVtxInSrcs'.

        The sum of @baseSlotIdx and @slotCnt must be less than or equal to the device limit 'maxVtxInSrcs'.
    */
    AFX_ASSERT_RANGE(dctx->devLimits->maxVtxInSrcs, basePin, cnt);
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindVertexBuffers), sizeof(cmd->BindVertexBuffers) + (cnt * sizeof(cmd->BindVertexBuffers.src[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindVertexBuffers.basePin = basePin;
    cmd->BindVertexBuffers.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedStream const* stream = &streams[i];

        avxBuffer buf = stream->buf;
        afxSize offset = stream->offset;
        afxUnit range = stream->range;
        afxUnit stride = stream->stride;

#if AVX_VALIDATION_ENABLED
        if (buf)
        {
            // If @buffers is not NIL, @buf must be a valid avxBuffer handle.
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            // All elements of @buffers must have been created with the avxBufferUsage_VERTEX flag.
            AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_VERTEX));

            // If @ranges is not NIL, all elements of @offsets must be less than the size of the corresponding element in @buffers.
            // If @ranges is not NIL, all elements of @offsets plus @ranges, where sizes is not zero, must be less than or equal to the size of the corresponding element in @buffers.
            afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufCap, offset, range);
            offset = AFX_MIN(offset, bufCap - 1);
            range = AFX_MIN(range ? range : bufCap, bufCap - offset);
            AFX_ASSERT(range);
        }
        else
        {
            // If an element of @buffers is NIL, then the corresponding element of @offsets must be zero.
            AFX_ASSERT(range == 0);
            AFX_ASSERT(offset == 0);
            //AFX_ASSERT(stride);
        }

#if AVX_LIMIT_VALIDATION_ENABLED
        // If @strides is not NIL each element of @strides must be less than or equal to the device limit 'maxVertexInputBindingStride'.
        AFX_ASSERT_RANGE(dctx->devLimits->maxVtxInSrcStride, 0, stride);
#endif
#endif
        cmd->BindVertexBuffers.src[i].buf = buf;
        cmd->BindVertexBuffers.src[i].offset = offset;
        cmd->BindVertexBuffers.src[i].range = range;
        cmd->BindVertexBuffers.src[i].stride = stride;

        dctx->vertices.streams[basePin + i].buf = buf;
        dctx->vertices.streams[basePin + i].offset = offset;
        dctx->vertices.streams[basePin + i].range = range;
        dctx->vertices.streams[basePin + i].stride = stride;
        dctx->vertices.vboBindCmdId[basePin + i] = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdBindIndexBuffer(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 range, afxUnit32 idxSiz)
{
    afxError err = { 0 };

    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // idxSiz must not be zero.
    AFX_ASSERT(idxSiz);
    if (idxSiz == 1)
    {
        // If idxSiz is 1 byte longe, the indexTypeUint8 feature must be enabled.
#if AVX_FEATURE_VALIDATION_ENABLED
        //AFX_ASSERT(dctx->enabledFeatures->idxSizUint8);
#endif
    }

    if (!buf)
    {
        AFX_ASSERT(!offset);
        AFX_ASSERT(!range);
        // If @buf is NIL, @offset must be zero.
        offset = 0;
        range = 0;
    }
    else
    {
        // If @buf is not NIL, buffer must be a valid avxBuffer handle.
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
        // @buf must have been acquired with the avxBufferUsage_INDEX flag.
        AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_INDEX));

        // @offset must be less than the size of buffer.
        afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
        // The sum of @offset and the base address of the @range of buffer, must be a multiple of the size of the type indicated by @idxSiz.
        // If @range is not WHOLE, @range must be a multiple of the size of the type indicated by @idxSiz.
        // If @range is not WHOLE, the sum of @offset and @range must be less than or equal to the size of buffer.
        AFX_ASSERT(offset % idxSiz == 0);
        AFX_ASSERT(range % idxSiz == 0);
        // @offset must be less than the size of buffer.
        AFX_ASSERT_RANGE(bufCap, offset, range);
        AFX_ASSERT(range);

        offset = AFX_MIN(offset, bufCap - 1);
        range = AFX_MIN(range ? range : bufCap, bufCap - offset);
    }

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindIndexBuffer), sizeof(cmd->BindIndexBuffer), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindIndexBuffer.buf = buf;
    cmd->BindIndexBuffer.offset = offset;
    cmd->BindIndexBuffer.range = range;
    cmd->BindIndexBuffer.idxSiz = idxSiz;

    dctx->vertices.ibo = buf;
    dctx->vertices.iboOffset = offset;
    dctx->vertices.iboRange = range;
    dctx->vertices.iboStride = idxSiz;
    dctx->vertices.iboBindCmdId = cmdId;

    return err;
}

// Draw

_AVX afxError AvxCmdDraw(afxDrawContext dctx, afxUnit vtxCnt, afxUnit instCnt, afxUnit baseVtx, afxUnit baseInst)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(vtxCnt);
    //AFX_ASSERT(instCnt);

#if 0
#if _AFX_DEBUG
    AFX_ASSERT(dctx->vertices.vin);
    avxVertexLayout vil;
    AvxDescribeVertexLayout(dctx->vertices.vin, &vil);
    for (afxUnit i = 0; i < vil.binCnt; i++)
    {
        avxBuffer buf = dctx->vertices.streams[i].buf;
        afxUnit bufCap = AvxGetBufferCapacity(buf, 0);
        afxSize bufBase = dctx->vertices.streams[i].offset;
        afxUnit bufRange = dctx->vertices.streams[i].range;
        afxUnit bufStride = dctx->vertices.streams[i].stride;

        AFX_ASSERT_RANGE(bufCap, baseVtx * bufStride + bufBase, 1);
        AFX_ASSERT_RANGE(bufCap, bufBase, vtxCnt * bufStride);
    }
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(Draw), sizeof(cmd->Draw), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Draw.data.vtxCnt = vtxCnt;
    cmd->Draw.data.instCnt = instCnt;
    cmd->Draw.data.baseVtx = baseVtx;
    cmd->Draw.data.baseInst = baseInst;
    return err;
}

_AVX afxError AvxCmdDrawIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    AFX_ASSERT(drawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndirect), sizeof(cmd->DrawIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirect.buf = buf;
    cmd->DrawIndirect.drawCnt = drawCnt;
    cmd->DrawIndirect.offset = offset;
    cmd->DrawIndirect.stride = stride;
    return err;
}

_AVX afxError AvxCmdDrawIndirect2(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // cntBuf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(cntBuf);
    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndirect2), sizeof(cmd->DrawIndirect2), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndirect2.buf = buf;
    cmd->DrawIndirect2.cntBuf = cntBuf;
    cmd->DrawIndirect2.cntBufOff = cntBufOff;
    cmd->DrawIndirect2.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndirect2.offset = offset;
    cmd->DrawIndirect2.stride = stride;
    return err;
}

_AVX afxError AvxCmdDrawIndexed(afxDrawContext dctx, afxUnit idxCnt, afxUnit instCnt, afxUnit baseIdx, afxUnit vtxOffset, afxUnit baseInst)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(idxCnt);
    //AFX_ASSERT(instCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexed), sizeof(cmd->DrawIndexed), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexed.data.baseIdx = baseIdx;
    cmd->DrawIndexed.data.baseInst = baseInst;
    cmd->DrawIndexed.data.idxCnt = idxCnt;
    cmd->DrawIndexed.data.instCnt = instCnt;
    cmd->DrawIndexed.data.vtxOffset = vtxOffset;
    return err;
}

_AVX afxError AvxCmdDrawIndexedIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, afxUnit32 drawCnt, afxUnit32 stride)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    /*
        @buf must have been created with the avxBufferUsage_INDIRECT bit set.

        @offset must be a multiple of 4.

        If the multiDrawIndirect feature is not enabled,
            @drawCnt must be 0 or 1.

        @drawCnt must be less than or equal to device's maxDrawIndirectCnt.

        If @drawCnt is greater than 1,
            @stride must be a multiple of 4 and must be greater than or equal to sizeof(avxDrawIndexedIndirect).

        If @drawCnt is equal to 1,
            (offset + sizeof(avxDrawIndexedIndirect)) must be less than or equal to the size of @buf.

        If @drawCnt is greater than 1,
            (@stride * (@drawCnt - 1) + offset + sizeof(avxDrawIndexedIndirect)) must be less than or equal to the size of @buf.
    */

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT(AvxGetBufferUsage(buf, avxBufferUsage_INDIRECT));
    afxSize bufCap = AvxGetBufferCapacity(buf, 0);
    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(stride % sizeof(afxUnit32) == 0);
    AFX_ASSERT_RANGE(bufCap, offset, (stride * (drawCnt ? drawCnt - 1 : 0) + sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT(!stride || (stride >= sizeof(avxDrawIndexedIndirect)));
    AFX_ASSERT_RANGE(drawCnt, 0, dctx->devLimits->maxDrawIndirectCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexedIndirect), sizeof(cmd->DrawIndexedIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirect.buf = buf;
    cmd->DrawIndexedIndirect.drawCnt = drawCnt;
    cmd->DrawIndexedIndirect.offset = offset;
    cmd->DrawIndexedIndirect.stride = stride;
    return err;
}

_AVX afxError AvxCmdDrawIndexedIndirect2(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset, avxBuffer cntBuf, afxUnit32 cntBufOff, afxUnit32 maxDrawCnt, afxUnit32 stride)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called inside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    // cntBuf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &cntBuf);

    AFX_ASSERT(maxDrawCnt);
    AFX_ASSERT(stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DrawIndexedIndirect2), sizeof(cmd->DrawIndexedIndirect2), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DrawIndexedIndirect2.buf = buf;
    cmd->DrawIndexedIndirect2.cntBuf = cntBuf;
    cmd->DrawIndexedIndirect2.cntBufOff = cntBufOff;
    cmd->DrawIndexedIndirect2.maxDrawCnt = maxDrawCnt;
    cmd->DrawIndexedIndirect2.offset = offset;
    cmd->DrawIndexedIndirect2.stride = stride;
    return err;
}

_AVX afxError AvxCmdSetPrimitiveTopology(afxDrawContext dctx, avxTopology topology)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // topology must be a valid avxTopology value.
    AFX_ASSERT(topology < avxTopology_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetPrimitiveTopology), sizeof(cmd->SetPrimitiveTopology), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetPrimitiveTopology.topology = topology;
    return err;
}

_AVX afxError AvxCmdAdjustViewports(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, avxViewport const viewports[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // cnt must be greater than 0.
    AFX_ASSERT(cnt);
    // viewports must be a valid pointer to an array of cnt valid avxViewport structures.
    AFX_ASSERT(viewports);

#if AVX_VALIDATION_ENABLED
#if AVX_LIMIT_VALIDATION_ENABLED
    // The sum of @baseIdx and @cnt must be between 1 and the device limit 'maxVpCnt", inclusive.
    AFX_ASSERT_RANGE(dctx->devLimits->maxVpCnt, baseIdx, cnt);
    AFX_ASSERT_RANGE(AVX_MAX_VIEWPORTS, baseIdx, cnt);
#endif

#if AVX_FEATURE_VALIDATION_ENABLED
    if (!dctx->enabledFeatures->viewports)
    {
        /*
            If the multiViewport feature is not enabled,
                firstViewport must be 0.
            If the multiViewport feature is not enabled,
                viewportCount must be 1.
        */
        AFX_ASSERT(baseIdx == 0);
        AFX_ASSERT(cnt == 1);
    }
#endif
#endif

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(AdjustViewports), sizeof(cmd->AdjustViewports) + (cnt * sizeof(cmd->AdjustViewports.viewports[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustViewports.baseIdx = baseIdx;
    cmd->AdjustViewports.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxViewport const* vp = &viewports[i];
#if AVX_VALIDATION_ENABLED
#if AVX_LIMIT_VALIDATION_ENABLED
        AFX_ASSERT_RANGE(dctx->devLimits->maxVpDimensions[0], vp->origin[0], vp->extent[0]);
        AFX_ASSERT_RANGE(dctx->devLimits->maxVpDimensions[1], vp->origin[1], vp->extent[1]);
        AFX_ASSERT(dctx->devLimits->vpBoundsRange[0] <= vp->minDepth);
        AFX_ASSERT(dctx->devLimits->vpBoundsRange[1] >= vp->maxDepth);
#endif
#endif
        cmd->AdjustViewports.viewports[i] = *vp;
    }
    return err;
}

_AVX afxError AvxCmdAlterCullMode(afxDrawContext dctx, avxCullMode mode)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // mode must be a valid combination of avxCullMode values.
    AFX_ASSERT_BOUNDS(mode, avxCullMode_NONE, avxCullMode_BOTH);
    mode = AFX_CLAMP(mode, avxCullMode_NONE, avxCullMode_BOTH);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetCullMode), sizeof(cmd->SetCullMode), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetCullMode.mode = mode;
    return err;
}

_AVX afxError AvxCmdSwitchFrontFace(afxDrawContext dctx, afxBool cw)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(cw, FALSE, TRUE);
    cw = AFX_CLAMP(cw, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SwitchFrontFace), sizeof(cmd->SwitchFrontFace), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SwitchFrontFace.cw = cw;
    return err;
}
