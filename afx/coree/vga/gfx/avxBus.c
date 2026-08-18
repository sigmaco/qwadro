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
#define _AVX_QUERY_POOL_C
#define _AVX_DRAW_CONTEXT_C
#include "../icd/avxIcd.h"

_AVX afxError AvxCmdBindPipeline(afxDrawContext dctx, avxPipeline pip, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    // This command must not be recorded when transform feedback is active.
    AFX_ASSERT(!dctx->xfbActive);

    // pip must be a valid avxPipeline handle.
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    avxBus bus = AvxGetPipelineBus(pip);
    AFX_ASSERT(bus < avxBus_TOTAL);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindPipeline), sizeof(cmd->BindPipeline), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindPipeline.pip = pip;
    cmd->BindPipeline.bus = bus;
    cmd->BindPipeline.vin = vin;
    cmd->BindPipeline.dynamics = dynamics;
    cmd->BindPipeline.flags = NIL;

    dctx->pipelines[bus].pip = pip;
    dctx->pipelines[bus].useSepShaders = FALSE;
    dctx->pipelines[bus].vin = vin;
    dctx->pipelines[bus].dynFlags = dynamics;

    avxLigature liga = NIL;
    AvxGetPipelineLigature(pip, &liga);
    if (dctx->ligatures[bus].liga != liga)
    {
        dctx->ligatures[bus].liga = liga;
        dctx->ligatures[bus].ligaBindCmdId = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdUseLigature(afxDrawContext dctx, avxBus bus, avxLigature liga, afxFlags flags)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // liga must be a valid avxLigature handle.
    AFX_TRY_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

#if 0
    if (dctx->ligatures[bus].liga == liga)
        return dctx->ligatures[bus].bindCmdId;
#endif

    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(UseLigature), sizeof(cmd->UseLigature), &cmdId);
    AFX_ASSERT(cmd);
    cmd->UseLigature.liga = liga;
    cmd->UseLigature.bus = bus;
    cmd->UseLigature.flags = flags;

    if (dctx->ligatures[bus].liga != liga)
    {
        dctx->ligatures[bus].liga = liga;
        dctx->ligatures[bus].ligaBindCmdId = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdBindBuffers(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxBufferedMap const maps[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindBuffers), sizeof(cmd->BindBuffers) + (cnt * sizeof(cmd->BindBuffers.maps[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindBuffers.bus = bus;
    cmd->BindBuffers.set = set;
    cmd->BindBuffers.pin = pin;
    cmd->BindBuffers.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferedMap const* map = maps ? &maps[i] : &(avxBufferedMap const) { 0 };
        afxSize offset = map->offset;
        afxUnit range = map->range;
        avxBuffer buf = map->buf;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        avxBufferUsage usage;
        afxSize bufSiz;
        if (buf)
        {
            AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
            bufSiz = AvxGetBufferCapacity(buf, 0);
            AFX_ASSERT_RANGE(bufSiz, offset, range);
            usage = AvxGetBufferUsage(buf, NIL);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin + i, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_UNIFORM:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_UNIFORM);
                }
                break;
            }
            case avxShaderParam_STORAGE:
                //case avxShaderParam_BUFFER:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_STORAGE);
                }
                break;
            }
            case avxShaderParam_FETCH:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_FETCH);
                }
                break;
            }
            case avxShaderParam_TSBO:
            {
                if (buf)
                {
                    AFX_ASSERT(usage & avxBufferUsage_TENSOR);
                }
                break;
            }
            default:
            {
                AFX_ASSERT((usage & avxBufferUsage_UNIFORM) ||
                    (usage & avxBufferUsage_STORAGE) ||
                    (usage & avxBufferUsage_FETCH) ||
                    (usage & avxBufferUsage_TENSOR));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindBuffers.maps[i].buf = buf;
        cmd->BindBuffers.maps[i].offset = offset;
        cmd->BindBuffers.maps[i].range = range;

        dctx->ligatures[bus].bindings[set][pin].buf.buf = buf;
        dctx->ligatures[bus].bindings[set][pin].buf.offset = offset;
        dctx->ligatures[bus].bindings[set][pin].buf.range = range;
        dctx->ligatures[bus].bindings[set][pin].buf.bufBindCmdId = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdBindRasters(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxRaster const rasters[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindRasters), sizeof(cmd->BindRasters) + (cnt * sizeof(cmd->BindRasters.rasters[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindRasters.bus = bus;
    cmd->BindRasters.set = set;
    cmd->BindRasters.pin = pin;
    cmd->BindRasters.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRaster ras = rasters ? rasters[i] : NIL;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin + i, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_TEXTURE:
            {
                break;
            }
            case avxShaderParam_RASTER:
            {
                break;
            }
            case avxShaderParam_IMAGE:
            {
                break;
            }
            default:
            {
                AFX_ASSERT((desc.type == avxShaderParam_TEXTURE) ||
                    (desc.type == avxShaderParam_RASTER) ||
                    (desc.type == avxShaderParam_IMAGE));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindRasters.rasters[i] = ras;

        dctx->ligatures[bus].bindings[set][pin].img.ras = ras;
        dctx->ligatures[bus].bindings[set][pin].img.rasBindCmdId = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdBindSamplers(afxDrawContext dctx, avxBus bus, afxUnit set, afxUnit pin, afxUnit cnt, avxSampler const samplers[])
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindSamplers), sizeof(cmd->BindSamplers) + (cnt * sizeof(cmd->BindSamplers.samplers[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindSamplers.bus = bus;
    cmd->BindSamplers.set = set;
    cmd->BindSamplers.pin = pin;
    cmd->BindSamplers.cnt = cnt;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
    avxLigature liga = dctx->ligatures[bus].liga;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
#endif//_AVX_DEBUG_BINDING_COMMANDS

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxSampler samp = samplers ? samplers[i] : NIL;

#ifdef _AVX_DEBUG_BINDING_COMMANDS // DEBUG
        if (samp)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &samp);
        }

        avxLigament desc;
        if (AvxDescribeLigament(liga, set, pin, 1, &desc))
        {
            switch (desc.type)
            {
            case avxShaderParam_TEXTURE:
            {
                break;
            }
            case avxShaderParam_SAMPLER:
            {
                break;
            }
            default:
            {
                AFX_ASSERT((desc.type == avxShaderParam_TEXTURE) ||
                    (desc.type == avxShaderParam_SAMPLER));
                AfxThrowError();
                break;
            }
            }
        }
#endif//_AVX_DEBUG_BINDING_COMMANDS

        cmd->BindSamplers.samplers[i] = samp;

        dctx->ligatures[bus].bindings[set][pin].img.samp = samp;
        dctx->ligatures[bus].bindings[set][pin].img.sampBindCmdId = cmdId;
    }
    return err;
}

_AVX afxError AvxCmdPushConstants(afxDrawContext dctx, afxUnit offset, afxUnit siz, void const* value)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(offset % sizeof(afxUnit32) == 0);
    AFX_ASSERT(siz % sizeof(afxUnit32) == 0);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PushConstants), sizeof(cmd->PushConstants) + siz, &cmdId);
    AFX_ASSERT(cmd);
    cmd->PushConstants.offset = offset;
    cmd->PushConstants.siz = siz;
    AfxCopy(cmd->PushConstants.data, value, siz);

    return err;
}

_AVX afxError AvxCmdDeclareDependency(afxDrawContext dctx, avxBusStage dstStage, avxBusAccess dstAcc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PipelineBarrier), sizeof(cmd->PipelineBarrier), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PipelineBarrier.dstStage = dstStage;
    cmd->PipelineBarrier.dstAccess = dstAcc;
    return err;
}

_AVX afxError AvxCmdDeclareBarrier(afxDrawContext dctx, avxBusStage dstStage, avxBusAccess dstAcc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(PipelineBarrier), sizeof(cmd->PipelineBarrier), &cmdId);
    AFX_ASSERT(cmd);
    cmd->PipelineBarrier.dstStage = dstStage;
    cmd->PipelineBarrier.dstAccess = dstAcc;
    return err;
}

AVX afxError AvxCmdBeginQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit slot, afxBool precise)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BeginQuery), sizeof(cmd->BeginQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BeginQuery.pool = pool;
    cmd->BeginQuery.slot = slot;
    cmd->BeginQuery.precise = precise;
    return err;
}

AVX afxError AvxCmdEndQuery(afxDrawContext dctx, avxQueryPool pool, afxUnit slot)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EndQuery), sizeof(cmd->EndQuery), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EndQuery.pool = pool;
    cmd->EndQuery.slot = slot;
    return err;
}

AVX afxError AvxCmdCopyQueryResults(afxDrawContext dctx, avxQueryPool pool, afxUnit baseSlot, afxUnit slotCnt, avxBuffer buf, afxSize offset, afxSize stride, avxQueryResultFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, baseSlot, slotCnt);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), offset, stride);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CopyQueryResults), sizeof(cmd->CopyQueryResults), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CopyQueryResults.pool = pool;
    cmd->CopyQueryResults.baseSlot = baseSlot;
    cmd->CopyQueryResults.slotCnt = slotCnt;
    cmd->CopyQueryResults.buf = buf;
    cmd->CopyQueryResults.offset = offset;
    cmd->CopyQueryResults.stride = stride;
    cmd->CopyQueryResults.flags = flags;
    return err;
}

AVX afxError AvxCmdResetQueries(afxDrawContext dctx, avxQueryPool pool, afxUnit baseSlot, afxUnit slotCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, baseSlot, slotCnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ResetQueries), sizeof(cmd->ResetQueries), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ResetQueries.pool = pool;
    cmd->ResetQueries.baseSlot = baseSlot;
    cmd->ResetQueries.slotCnt = slotCnt;
    return err;
}

AVX afxError AvxCmdQueryTimestamp(afxDrawContext dctx, avxQueryPool pool, afxUnit slot, avxBusStage stage)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &pool);
    AFX_ASSERT_RANGE(pool->slotCnt, slot, 1);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(QueryTimestamp), sizeof(cmd->QueryTimestamp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->QueryTimestamp.pool = pool;
    cmd->QueryTimestamp.slot = slot;
    cmd->QueryTimestamp.stage = stage;
    return err;
}

_AVX afxError AvxCmdDispatch(afxDrawContext dctx, afxUnit w, afxUnit h, afxUnit d)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(Dispatch), sizeof(cmd->Dispatch), &cmdId);
    AFX_ASSERT(cmd);
    cmd->Dispatch.data.w = w;
    cmd->Dispatch.data.h = h;
    cmd->Dispatch.data.d = d;
    return err;
}

_AVX afxError AvxCmdDispatchIndirect(afxDrawContext dctx, avxBuffer buf, afxUnit32 offset)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(!dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    // @buf must be a valid avxBuffer handle.
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DispatchIndirect), sizeof(cmd->DispatchIndirect), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DispatchIndirect.buf = buf;
    cmd->DispatchIndirect.offset = offset;
    return err;
}
