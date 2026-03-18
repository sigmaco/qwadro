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



#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_4D_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "qowBase.h"
#define CLAY_IMPLEMENTATION
#include "../dep/clay/clay.h"

AFX_DEFINE_STRUCT(Wpu)
{
    struct nk_context *ctx;
};

QOW _auxGuiElemLut const cmdDevmt;

AUX afxCmdId AfxCmdCommencePanel(afxWidget wid, afxPanelInfo const* info);
AUX afxCmdId AfxCmdConcludePanel(afxWidget wid);

AUX afxCmdId AfxCmdPushLayout(afxWidget wid, auxLayoutDirection dir);
AUX afxCmdId AfxCmdPopLayout(afxWidget wid);

AUX afxCmdId AfxCmdPushGroup(afxWidget wid, afxString const* caption);
AUX afxCmdId AfxCmdPopGroup(afxWidget wid);

AUX afxCmdId AfxCmdButton(afxWidget wid, afxButtonInfo const* info, afxUnit* optionIdx);
AUX afxCmdId AfxCmdLabel(afxWidget wid, afxLabelInfo const* info);
AUX afxCmdId AfxCmdTextInputInlined(afxWidget wid, afxTextInputInfo const* info);
AUX afxCmdId AfxCmdCheckbox(afxWidget wid, afxCheckboxInfo const* info, afxBool* checked);

AUX afxCmdId AfxCmdDisable(afxWidget wid);
AUX afxCmdId AfxCmdSeparator(afxWidget wid);

QOW void _DecodeSubnodes(Wpu* dpu, _auxGuiElem* parent);

_QOW void _DecodeCmdLabel(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    nk_label(ctx, cmd->Label.info.caption.start, NK_TEXT_LEFT);
}

_QOW void _DecodeCmdButton(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    nk_button_label(ctx, cmd->Button.info.caption.start);
}

_QOW void _DecodeCmdCheckbox(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    struct nk_style_window_header s;
    nk_checkbox_text(ctx, cmd->Checkbox.info.caption.start, cmd->Checkbox.info.caption.len, cmd->Checkbox.checked);
}

_QOW void _DecodeCmdTextInputInlined(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    nk_edit_string(ctx, NK_EDIT_FIELD, cmd->TextInputInlined.info.caption.start, &cmd->TextInputInlined.info.caption.len, cmd->TextInputInlined.info.caption.cap, nk_filter_default);
}

_QOW void _DecodeCmdDisable(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    
}

_QOW void _DecodeCmdSeparator(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    
}

_QOW void _DecodeCmdPushGroup(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    afxUnit itemCnt = cmd->PushGroup.hdr.children.cnt;

    //nk_group_begin(ctx, cmd->PushGroup.caption.start, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER);

    //nk_layout_row_template_begin(ctx, 100);
    //nk_layout_row_template_push_variable(ctx, 0);

    _auxGuiElem* childCmdHdr;
    afxCmdId lastId = 0; // DBG
    AFX_ITERATE_CHAIN_B2F(childCmdHdr, hdr.parent, &cmd->hdr.children)
    {
        lastId = childCmdHdr->hdr.id;

        nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 1);
        nk_layout_row_push(ctx, 1);

        AFX_ASSERT(cmdDevmt.f[childCmdHdr->hdr.id]);
        cmdDevmt.f[childCmdHdr->hdr.id](dpu, childCmdHdr);

        nk_layout_row_end(ctx);
    }
    //nk_group_end(ctx);

    //nk_layout_row_template_end(ctx);
}

_QOW void _DecodeCmdPopGroup(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

}

_QOW void _DecodeCmdPushLayout(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    auxLayoutDirection dir = cmd->PushLayout.dir;
    afxUnit itemCnt = cmd->PushLayout.hdr.children.cnt;

    if (dir == auxLayoutDirection_VERTICAL)
    {
        _auxGuiElem* childCmdHdr;
        afxCmdId lastId = 0; // DBG
        AFX_ITERATE_CHAIN_B2F(childCmdHdr, hdr.parent, &cmd->hdr.children)
        {
            lastId = childCmdHdr->hdr.id;

            nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 1);
            nk_layout_row_push(ctx, 1.0);

            //nk_layout_row_dynamic(ctx, 30, 1);

            //nk_layout_row_template_begin(ctx, 100);
            //nk_layout_row_template_push_variable(ctx, 0);

            AFX_ASSERT(cmdDevmt.f[childCmdHdr->hdr.id]);
            cmdDevmt.f[childCmdHdr->hdr.id](dpu, childCmdHdr);

            nk_layout_row_end(ctx);

            //nk_layout_row_template_end(ctx);
        }
    }
    else if (dir == auxLayoutDirection_HORIZONTAL)
    {
        //nk_layout_row_dynamic(ctx, 0, itemCnt);
        nk_layout_row_begin(ctx, NK_DYNAMIC, 0, itemCnt);

        //nk_layout_row_template_begin(ctx, 100);

        _auxGuiElem* childCmdHdr;
        afxCmdId lastId = 0; // DBG
        AFX_ITERATE_CHAIN_B2F(childCmdHdr, hdr.parent, &cmd->hdr.children)
        {
            lastId = childCmdHdr->hdr.id;

            //nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_push(ctx, 1.0);

            AFX_ASSERT(cmdDevmt.f[childCmdHdr->hdr.id]);
            cmdDevmt.f[childCmdHdr->hdr.id](dpu, childCmdHdr);
        }

        nk_layout_row_end(ctx);
        //nk_layout_row_template_end(ctx);
    }
}

_QOW void _DecodeCmdPopLayout(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;


}

_QOW void _DecodeCmdConcludePanel(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    nk_end(ctx);
}

_QOW void _DecodeCmdCommencePanel(Wpu* dpu, _auxGuiElem const* cmd)
{
    afxError err = { 0 };

    struct nk_context *ctx = dpu->ctx;

    afxRect bounds = cmd->CommencePanel.info.bounds;

    if (nk_begin(ctx, cmd->CommencePanel.info.caption.start, nk_rect(bounds.x, bounds.y, bounds.w, bounds.h),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        _DecodeSubnodes(dpu, cmd);

    }
    nk_end(ctx);
}

_QOW _auxGuiElemLut const cmdDevmt =
{
    .CommencePanel = (void*)_DecodeCmdCommencePanel,
    .ConcludePanel = (void*)_DecodeCmdConcludePanel,
    .PushLayout = (void*)_DecodeCmdPushLayout,
    .PopLayout = (void*)_DecodeCmdPopLayout,
    .PushGroup = (void*)_DecodeCmdPushGroup,
    .PopGroup = (void*)_DecodeCmdPopGroup,

    .Disable = (void*)_DecodeCmdDisable,
    .Separator = (void*)_DecodeCmdSeparator,
    
    .Label = (void*)_DecodeCmdLabel,
    .Button = (void*)_DecodeCmdButton,
    .Checkbox = (void*)_DecodeCmdCheckbox,
    .TextInputInlined = (void*)_DecodeCmdTextInputInlined,
};

_QOW void _DecodeSubnodes(Wpu* dpu, _auxGuiElem* parent)
{
    afxError err;
    afxCmdId lastId = 0; // DBG

    _auxGuiElem *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.parent, &parent->hdr.children)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        //if (cmdb->state != avxContextState_PENDING)
        {
            //AfxThrowError();
            //break;
        }

        AFX_ASSERT(cmdDevmt.f[cmdHdr->hdr.id]);
        cmdDevmt.f[cmdHdr->hdr.id](dpu, cmdHdr);
    }
}

_QOW afxError _QowWidUpdateCb(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    /* Input */
    nk_afx_new_frame(&wid->idd);

    struct nk_context *ctx = wid->ctx;
    Wpu wpu = { 0 };
    wpu.ctx = ctx;

    afxCmdId lastId = 0; // DBG

    _auxGuiElem *cmdHdr;
    AFX_ITERATE_CHAIN_B2F(cmdHdr, hdr.script, &wid->m.commands)
    {
        lastId = cmdHdr->hdr.id;

        if (cmdHdr->hdr.id == NIL/*ZGL_CMD_END*/)
        {
            break;
        }

        if (cmdHdr->hdr.id != _AUX_GUI_ELEM_ID(CommencePanel))
        {
            continue;
        }

        //if (cmdb->state != avxContextState_PENDING)
        {
            //AfxThrowError();
            //break;
        }

        AFX_ASSERT(cmdDevmt.f[cmdHdr->hdr.id]);
        cmdDevmt.f[cmdHdr->hdr.id](&wpu, cmdHdr);
    }

    return err;
}

_QOW afxError _QowWidRenderCb(afxWidget wid, afxRect const* area, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    wid->m.updateCb(wid, 0);

    nk_afx_render(&wid->idd, NK_ANTI_ALIASING_OFF, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER, dctx);

    return err;
}

_QOW afxError _QowWidImplAdjustLayout(afxWidget wid, afxUnit w, afxUnit h)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    Clay_SetLayoutDimensions((Clay_Dimensions) { w, h });

    return err;
}

_QOW afxError _QowWidDtorCb(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wid));

    nk_afx_shutdown(&wid->idd);

    _AUX_WID_CLASS_CONFIG.dtor(wid);

    return err;
}

_QOW afxError _QowWidCtorCb(afxWidget wid, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxWindow wnd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxWidgetConfig const* cfg = AFX_CAST(afxWidgetConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    if (_AUX_WID_CLASS_CONFIG.ctor(wid, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }
    
    struct nk_afx* idd = &wid->idd;

    AfxZero(idd, sizeof(*idd));

    struct nk_context *ctx;

    ctx = nk_afx_init(idd, wnd, NK_AFX_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {struct nk_font_atlas *atlas;
    nk_afx_font_stash_begin(idd, &atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_afx_font_stash_end(idd);
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &droid->handle);*/}

    wid->ctx = ctx;

    wid->m.renderCb = (void*)_QowWidRenderCb;
    wid->m.updateCb = (void*)_QowWidUpdateCb;

    return err;
}
