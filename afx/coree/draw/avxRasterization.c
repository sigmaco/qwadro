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

// This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
#define _AVX_DRAW_CONTEXT_C
#include "avxIcd.h"

_AVX afxError AvxCmdSwitchRasterization(afxDrawContext dctx, afxBool disabled)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(disabled, FALSE, TRUE);
    disabled = AFX_CLAMP(disabled, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DisableRasterization), sizeof(cmd->DisableRasterization), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableRasterization.disable = disabled;
    return err;
}

_AVX afxError AvxCmdSetLineWidth(afxDrawContext dctx, afxReal lineWidth)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetLineWidth), sizeof(cmd->SetLineWidth), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetLineWidth.lineWidth = lineWidth;
    return err;
}

_AVX afxError AvxCmdEnableDepthBias(afxDrawContext dctx, afxBool enabled)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enabled, FALSE, TRUE);
    enabled = AFX_CLAMP(enabled, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthBias), sizeof(cmd->EnableDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBias.enable = enabled;
    return err;
}

_AVX afxError AvxCmdSetDepthBias(afxDrawContext dctx, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthBias), sizeof(cmd->SetDepthBias), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthBias.constFactor = constFactor;
    cmd->SetDepthBias.clamp = clamp;
    cmd->SetDepthBias.slopeFactor = slopeFactor;
    return err;
}

_AVX afxError AvxCmdEnableDepthTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AFX_CLAMP(enable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthTest), sizeof(cmd->EnableDepthTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthTest.enable = enable;
    return err;
}

_AVX afxError AvxCmdSetDepthComparator(afxDrawContext dctx, avxCompareOp op)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_BOUNDS(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    op = AFX_CLAMP(op, avxCompareOp_NEVER, avxCompareOp_ALWAYS);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthCompareOp), sizeof(cmd->SetDepthCompareOp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetDepthCompareOp.compareOp = op;
    return err;
}

_AVX afxError AvxCmdDisableDepthWrites(afxDrawContext dctx, afxBool disable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    AFX_ASSERT_RANGE(disable, FALSE, TRUE);
    disable = AFX_CLAMP(disable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(DisableDepthWrite), sizeof(cmd->DisableDepthWrite), &cmdId);
    AFX_ASSERT(cmd);
    cmd->DisableDepthWrite.disable = disable;
    return err;
}

_AVX afxError AvxCmdSwitchStencilTesting(afxDrawContext dctx, afxBool enable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableStencilTest), sizeof(cmd->EnableStencilTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableStencilTest.enable = enable;
    return err;
}

_AVX afxError AvxCmdSetStencilCompareMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask compareMask)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilCompareMask), sizeof(cmd->SetStencilCompareMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilCompareMask.faceMask = faceMask;
    cmd->SetStencilCompareMask.compareMask = compareMask;
    return err;
}

_AVX afxError AvxCmdSetStencilWriteMask(afxDrawContext dctx, avxFaceMask faceMask, afxMask writeMask)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilWriteMask), sizeof(cmd->SetStencilWriteMask), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilWriteMask.faceMask = faceMask;
    cmd->SetStencilWriteMask.writeMask = writeMask;
    return err;
}

_AVX afxError AvxCmdSetStencilReference(afxDrawContext dctx, avxFaceMask faceMask, afxUnit32 reference)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT(faceMask);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilReference), sizeof(cmd->SetStencilReference), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilReference.faceMask = faceMask;
    cmd->SetStencilReference.reference = reference;
    return err;
}

_AVX afxError AvxCmdSetStencilOp(afxDrawContext dctx, avxFaceMask faceMask, avxStencilOp failOp, avxStencilOp passOp, avxStencilOp depthFailOp, avxCompareOp compareOp)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetStencilOp), sizeof(cmd->SetStencilOp), &cmdId);
    AFX_ASSERT(cmd);
    cmd->SetStencilOp.faceMask = faceMask;
    cmd->SetStencilOp.failOp = failOp;
    cmd->SetStencilOp.passOp = passOp;
    cmd->SetStencilOp.depthFailOp = depthFailOp;
    cmd->SetStencilOp.compareOp = compareOp;
    return err;
}

_AVX afxError AvxCmdEnableDepthBounds(afxDrawContext dctx, afxBool enable)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    AFX_ASSERT_RANGE(enable, FALSE, TRUE);
    enable = AFX_CLAMP(enable, FALSE, TRUE);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(EnableDepthBoundsTest), sizeof(cmd->EnableDepthBoundsTest), &cmdId);
    AFX_ASSERT(cmd);
    cmd->EnableDepthBoundsTest.enable = enable;
    return err;
}

_AVX afxError AvxCmdDelimitDepthBounds(afxDrawContext dctx, afxV2d const bounds)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetDepthBounds), sizeof(cmd->SetDepthBounds), &cmdId);
    AFX_ASSERT(cmd);
    AfxV2dCopy(cmd->SetDepthBounds.bounds, bounds ? bounds : AFX_V2D(0, 1));
    return err;
}

_AVX afxError AvxCmdSetBlendConstants(afxDrawContext dctx, afxV4d const blendConstants)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);
    
    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(SetBlendConstants), sizeof(cmd->SetBlendConstants), &cmdId);
    AFX_ASSERT(cmd);
    AfxV4dCopy(cmd->SetBlendConstants.blendContants, blendConstants ? blendConstants : AFX_V4D(1, 1, 1, 1));
    return err;
}

_AVX afxError AvxCmdAdjustScissors(afxDrawContext dctx, afxUnit baseIdx, afxUnit cnt, afxRect const rects[])
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

    // rects must be a valid pointer to an array of cnt valid afxRect structures.
    AFX_ASSERT(rects);
    AFX_ASSERT_RANGE(AVX_MAX_VIEWPORTS, baseIdx, cnt);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(AdjustScissors), sizeof(cmd->AdjustScissors) + (cnt * sizeof(cmd->AdjustScissors.rects[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->AdjustScissors.baseIdx = baseIdx;
    cmd->AdjustScissors.cnt = cnt;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRect const* rc = &rects[i];

        // The x and y members of offset member of any element of rects must be greater than or equal to 0.
        AFX_ASSERT(rc->x >= 0);
        AFX_ASSERT(rc->y >= 0);

        // Evaluation of (offset + extent) must not cause a signed integer addition overflow for any element of rects.
        AFX_ASSERT(rc->x + (afxInt)rc->w > 0);
        AFX_ASSERT(rc->y + (afxInt)rc->h > 0);

        cmd->AdjustScissors.rects[i] = *rc;
    }
    return err;
}

_AVXINL void AfxMinLayeredRect(afxLayeredRect* rc, afxLayeredRect const* a, afxLayeredRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    rc->area.x = AFX_MIN(a->area.x, b->area.x);
    rc->area.y = AFX_MIN(a->area.y, b->area.y);
    rc->area.w = AFX_MIN(a->area.w, (b->area.w - rc->area.x));
    rc->area.h = AFX_MIN(a->area.h, (b->area.h - rc->area.y));
    rc->baseLayer = AFX_MIN(a->baseLayer, b->baseLayer);
    rc->layerCnt = AFX_MIN(a->layerCnt, b->layerCnt);
}

_AVXINL void AfxMaxLayeredRect(afxLayeredRect* rc, afxLayeredRect const* a, afxLayeredRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    rc->area.x = AFX_MAX(a->area.x, b->area.x);
    rc->area.y = AFX_MAX(a->area.y, b->area.y);
    rc->area.w = AFX_MAX(a->area.w, (b->area.w - rc->area.x));
    rc->area.h = AFX_MAX(a->area.h, (b->area.h - rc->area.y));
    rc->baseLayer = AFX_MAX(a->baseLayer, b->baseLayer);
    rc->layerCnt = AFX_MAX(a->layerCnt, b->layerCnt);
}

_AVX afxError AvxCmdCommenceDrawScope(afxDrawContext dctx, avxDrawScope const* cfg)
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

    // cfg must be a valid pointer to a valid avxDrawScope structure.
    AFX_ASSERT(cfg);
    AFX_ASSERT(cfg->targets);
    avxDrawScope cfg2 = *cfg;
    avxCanvas canv = cfg->canv;

    if (canv)
    {
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        afxLayeredRect areaMax;
        AvxGetCanvasExtent(canv, NIL, &areaMax);
        AFX_ASSERT_RANGE(areaMax.area.w, cfg->bounds.area.x, cfg->bounds.area.w);
        AFX_ASSERT_RANGE(areaMax.area.h, cfg->bounds.area.y, cfg->bounds.area.h);
        AFX_ASSERT_RANGE(areaMax.layerCnt, cfg->bounds.baseLayer, cfg->bounds.layerCnt);

        cfg2.bounds.area.x = AFX_CLAMP(cfg->bounds.area.x, 0, (afxInt)areaMax.area.w);
        cfg2.bounds.area.y = AFX_CLAMP(cfg->bounds.area.y, 0, (afxInt)areaMax.area.h);
        cfg2.bounds.area.w = cfg->bounds.area.w ? AFX_CLAMP(cfg->bounds.area.w, 1, areaMax.area.w - cfg2.bounds.area.x) : areaMax.area.w;
        cfg2.bounds.area.h = cfg->bounds.area.h ? AFX_CLAMP(cfg->bounds.area.h, 1, areaMax.area.h - cfg2.bounds.area.y) : areaMax.area.h;
        cfg2.bounds.baseLayer = AFX_CLAMP(cfg->bounds.baseLayer, areaMax.baseLayer, areaMax.layerCnt - 1);
        cfg2.bounds.layerCnt = AFX_CLAMP(cfg->bounds.layerCnt, areaMax.baseLayer, areaMax.layerCnt);
    }

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(CommenceDrawScope), sizeof(cmd->CommenceDrawScope) + (cfg2.targetCnt * sizeof(cmd->CommenceDrawScope.targets[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->CommenceDrawScope.targetCnt = cfg2.targetCnt;
    cmd->CommenceDrawScope.canv = canv;
    cmd->CommenceDrawScope.bounds = cfg2.bounds;

    for (afxUnit i = 0; i < cfg2.targetCnt; i++)
    {
        AFX_ASSERT(cfg->targets);
        AFX_ASSERT_BOUNDS(cfg->targets[i].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg->targets[i].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.targets[i] = cfg->targets[i];
    }

    //if (!cfg2.depth) cmd->CommenceDrawScope.hasD = FALSE;
    //else
    {
        AFX_ASSERT_BOUNDS(cfg2.ds[0].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.ds[0].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.ds[0] = cfg2.ds[0];
        cmd->CommenceDrawScope.hasD = TRUE;
    }

    //if (!cfg2.stencil) cmd->CommenceDrawScope.hasS = FALSE;
    //else
    {
        AFX_ASSERT_BOUNDS(cfg2.ds[1].loadOp, avxLoadOp_LOAD, avxLoadOp_DONT_CARE);
        AFX_ASSERT_BOUNDS(cfg2.ds[1].storeOp, avxStoreOp_STORE, avxStoreOp_DONT_CARE);
        cmd->CommenceDrawScope.ds[1] = cfg2.ds[1];
        cmd->CommenceDrawScope.hasS = TRUE;
    }

    cmd->CommenceDrawScope.flags = cfg2.flags;
    cmd->CommenceDrawScope.dbgTag = cfg2.tag;

    dctx->inDrawScope = TRUE;
    dctx->inDrawScopeCmd = cmd;
    dctx->canv = canv;
    dctx->ccfg = cfg2;

    return err;
}

_AVX afxError AvxCmdConcludeDrawScope(afxDrawContext dctx)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ConcludeDrawScope), sizeof(cmd->ConcludeDrawScope), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ConcludeDrawScope.nothing = NIL;

    dctx->inDrawScope = FALSE;
    dctx->inDrawScopeCmd = cmd;
    dctx->canv = NIL;
    dctx->ccfg = (avxDrawScope) { 0 };

    return err;
}

_AVX afxError AvxCmdClearCanvas(afxDrawContext dctx, afxUnit bufCnt, afxUnit const bins[], avxClearValue const values[], afxUnit areaCnt, afxLayeredRect const areas[])
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

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(ClearCanvas), sizeof(cmd->ClearCanvas) + (areaCnt * sizeof(cmd->ClearCanvas.areas[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->ClearCanvas.bufCnt = bufCnt;
    cmd->ClearCanvas.areaCnt = areaCnt;

    for (afxUnit i = 0; i < bufCnt; i++)
    {
        cmd->ClearCanvas.bins[i] = bins ? bins[i] : i;
        cmd->ClearCanvas.values[i] = values[i];
    }

    for (afxUnit i = 0; i < areaCnt; i++)
    {
        AfxMinLayeredRect(&cmd->ClearCanvas.areas[i], &areas[i], &dctx->ccfg.bounds);
    }

    return err;
}

_AVX afxError AvxCmdBindDrawBuffersEXT(afxDrawContext dctx, avxCanvas canv, afxUnit bufCnt, afxUnit const bins[], avxRaster const buffers[])
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

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(BindDrawBuffersEXT), sizeof(cmd->BindDrawBuffersEXT) + (bufCnt * sizeof(cmd->BindDrawBuffersEXT.buffers[0])), &cmdId);
    AFX_ASSERT(cmd);
    cmd->BindDrawBuffersEXT.bufCnt = bufCnt;

    for (afxUnit i = 0; i < bufCnt; i++)
    {
        cmd->BindDrawBuffersEXT.bins[i] = bins ? bins[i] : i;
        cmd->BindDrawBuffersEXT.buffers[i] = buffers[i];
    }

    return err;
}

_AVX afxError AvxCmdAdvancePass(afxDrawContext dctx, afxBool useAuxScripts)
{
    afxError err = { 0 };
    // dctx must be a valid afxDrawContext handle.
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    // dctx must be in the recording state.
    AFX_ASSERT(dctx->state == avxContextState_RECORDING);
    // This command must only be called outside of a draw scope instance.
    AFX_ASSERT(dctx->inDrawScope);
    // This command must only be called outside of a video coding scope.
    AFX_ASSERT(!dctx->inVideoCoding);

    afxCmdId cmdId;
    _avxCmd* cmd = _AvxDctxPushCmd(dctx, _AVX_CMD_ID(NextPass), sizeof(cmd->NextPass), &cmdId);
    AFX_ASSERT(cmd);
    cmd->NextPass.useAuxContexts = !!useAuxScripts;
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
