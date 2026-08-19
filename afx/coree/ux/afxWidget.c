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

// This software is part of Advanced User Experience Extensions.

#define _AUX_UX_C
#define _AUX_WIDGET_C
#define _AUX_WINDOW_C
#include "auxIcd.h"

_AUX _auxGuiElem* _AuxWidPushGuiElem(afxWidget wid, afxUnit id, afxUnit siz, afxCmdId* elemId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    AFX_ASSERT(siz >= sizeof(_auxGuiElemHdr));

    _auxGuiElem* cmd = AfxRequestArena(&wid->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AfxMakeChain(&cmd->hdr.children, cmd);
    AfxPushLink(&cmd->hdr.parent, NIL);
    AFX_ASSERT(elemId);
    *elemId = AfxPushLink(&cmd->hdr.script, &wid->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

_AUX afxCmdId AfxGuiCommencePanel(afxWidget wid, afxPanelInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(CommencePanel), sizeof(cmd->CommencePanel), &elemId);
    AFX_ASSERT(cmd);
    cmd->CommencePanel.info = *info;

    //wid->inPanel = TRUE;

    AFX_ASSERT(!wid->currNode);
    wid->currNode = cmd;

    return 1;
}

_AUX afxCmdId AfxGuiConcludePanel(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(ConcludePanel), sizeof(cmd->ConcludePanel), &elemId);
    AFX_ASSERT(cmd);
    cmd->ConcludePanel.nothing = NIL;


    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(CommencePanel));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);
    //wid->inPanel = FALSE;

    return 0;
}

_AUX afxCmdId AfxGuiPushLayout(afxWidget wid, auxLayoutDirection dir)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PushLayout), sizeof(cmd->PushLayout), &elemId);
    AFX_ASSERT(cmd);
    cmd->PushLayout.dir = dir;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);
    wid->currNode = cmd;

    return elemId;
}

_AUX afxCmdId AfxGuiPopLayout(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    //afxCmdId elemId;
    //_auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PopLayout), sizeof(cmd->PopLayout), &elemId);
    //AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(PushLayout));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);

    return 0;
}

_AUX afxCmdId AfxGuiPushGroup(afxWidget wid, afxString const* caption)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PushGroup), sizeof(cmd->PushGroup), &elemId);
    AFX_ASSERT(cmd);
    cmd->PushGroup.caption = *caption;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);
    wid->currNode = cmd;

    return elemId;
}

_AUX afxCmdId AfxGuiPopGroup(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PopGroup), sizeof(cmd->PopGroup), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(PushGroup));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);

    return elemId;
}

_AUX afxCmdId AfxGuiButton(afxWidget wid, afxButtonInfo const* info, afxUnit* optionIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Button), sizeof(cmd->Button), &elemId);
    AFX_ASSERT(cmd);
    cmd->Button.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiLabel(afxWidget wid, afxLabelInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Label), sizeof(cmd->Label), &elemId);
    AFX_ASSERT(cmd);
    cmd->Label.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiTextInputInlined(afxWidget wid, afxTextInputInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(TextInputInlined), sizeof(cmd->TextInputInlined), &elemId);
    AFX_ASSERT(cmd);
    cmd->TextInputInlined.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiCheckbox(afxWidget wid, afxCheckboxInfo const* info, afxBool* checked)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Checkbox), sizeof(cmd->Checkbox), &elemId);
    AFX_ASSERT(cmd);
    cmd->Checkbox.info = *info;
    cmd->Checkbox.checked = checked;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiDisable(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Disable), sizeof(cmd->Disable), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiSeparator(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Separator), sizeof(cmd->Separator), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxError AfxResetWidget(afxWidget wid)
{
    afxError err = { 0 };

    return err;
}

_AUX afxError AfxDoWidgetInput(afxWidget wid)
{
    afxError err = { 0 };

    afxWindow wnd = AfxGetHost(wid);
    afxEnvironment env = AfxGetHost(wnd);

    return err;
}

_AUX afxError AfxTestWidget(afxWidget wid)
{
    afxError err = { 0 };

    return err;
}

_AUX afxError AfxLockWidget(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    
    // reset chain
    AfxMakeChain(&wid->commands, wid);
    // exhaust arena
    if (wid->cmdArena.cleanupCnt > 3)
        AfxExhaustArena(&wid->cmdArena);

    wid->currNode = NIL;

    return err;
}

_AUX afxError AfxUnlockWidget(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    wid->updateCb(wid, 0);

    return err;
}

_AUX afxError AfxDrawWidget(afxWidget wid, afxRect const* area, afxDrawContext dctx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    wid->renderCb(wid, area, dctx);
    return err;
}

_AUX afxError _AuxWidDtorCb(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    AfxDismantleArena(&wid->cmdArena);

    return err;
}

_AUX afxError _AuxWidCtorCb(afxWidget wid, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxWindow wnd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxWidgetConfig const* cfg = AFX_CAST(afxWidgetConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    afxSurface dout;
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
    afxDrawSystem dsys = AvxGetSurfaceHost(dout);


    AfxMakeArena(&wid->cmdArena, NIL, AfxHere());
    AfxMakeChain(&wid->commands, wid);

    return err;
}

_AUX afxClassConfig const _AUX_WID_CLASS_CONFIG =
{
    .fcc = afxFcc_WID,
    .name = "Widget",
    .desc = "Widget",
    .fixedSiz = sizeof(AFX_OBJECT(afxWidget)),
    .ctor = (void*)_AuxWidCtorCb,
    .dtor = (void*)_AuxWidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireWidgets(afxWindow wnd, afxUnit cnt, afxWidgetConfig const cfg[], afxWidget widgets[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxClass* cls = (afxClass*)_AuxWndGetWidClass(wnd);
    AFX_ASSERT_CLASS(cls, afxFcc_WID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)widgets, (void const*[]) { wnd, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}
