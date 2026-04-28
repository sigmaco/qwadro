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

// This software is part of Advanced User Experience Extensions.

#define _AUX_UX_C
#define _AUX_WINDOW_C
#include "auxIcd.h"

_AUX afxUnit _AfxWndFormatTitleCb(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    return 0;
}

_AUX afxError _AfxWndAdjustCb(afxWindow wnd, afxAnchor anchor, afxRect* c)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    if (c)
    {
        afxRect rc2 = *c;
        rc2.x += wnd->frameRc.x;
        rc2.y += wnd->frameRc.y;
        rc2.w = AFX_MAX(1, rc2.w);
        rc2.h = AFX_MAX(1, rc2.h);

        if ((wnd->surfaceRc.w != rc2.w) || (wnd->surfaceRc.h != rc2.h) ||
            (wnd->frameRc.x != rc2.x) || (wnd->frameRc.y != rc2.y))
        {
            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->surfaceRc.w = rc2.w;
            wnd->surfaceRc.h = rc2.h;
            wnd->frameRc.w = wnd->surfaceRc.w + wnd->frameMarginR + wnd->frameMarginL;
            wnd->frameRc.h = wnd->surfaceRc.h + wnd->frameMarginB + wnd->frameMarginT;
            wnd->frameRc.x = rc2.x;
            wnd->frameRc.y = rc2.y;
        }
    }
#if 0
    afxRect whd;
    afxSurface dout = wnd->surfaceDout;

    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->surfaceRc.w;
        whd.h = wnd->surfaceRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
            AfxThrowError();
    }
#endif
    return err;
}

_AUX afxBool _AuxWidEmitEventCb(afxWidget wid, auxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);



    auxEvent ev2 = *ev;
    AfxEmitEvent(wid, &ev2.ev);

    return FALSE;
}

_AUX afxBool _AuxWndEventHandlerSW(afxWindow wnd, auxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    
    switch (ev->id)
    {
    case auxEventId_KEY:
    {
        afxUnit seatId = ev->seat;

        if (AfxWasKeyPressed(seatId, afxKey_ESC))
        {
            if (wnd->cursConfined)
            {

            }
        }

        if (AfxIsKeyPressed(seatId, afxKey_LALT) || AfxIsKeyPressed(0, afxKey_RALT))
        {
            if (AfxWasKeyPressed(seatId, afxKey_PRINT))
            {
                if (wnd->surfaceDout)
                {
                    afxUri2048 uri;
                    AfxMakeUri2048(&uri, NIL);
                    AfxFormatUri(&uri.uri, "../tmp/ss-wnd%u-%u.tga", AfxGetObjectId((void*)wnd), AfxGetTimer());
                    AvxPrintSurfaceBuffer(wnd->surfaceDout, 0, NIL, &uri.uri, 0);
                }
            }
            else if (AfxWasKeyPressed(seatId, afxKey_F11))
            {
                AfxTakeFullscreen(wnd, !wnd->fullscreen);
            }
        }
        break;
    }
    case auxEventId_CLOSE:
    {
        break;
    }
    case auxEventId_MOVE:
    {
        break;
    }
    case auxEventId_SIZE:
    {
        afxSurface dout = wnd->surfaceDout;

        if (dout)
        {
            AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
            afxRect whd = wnd->surfaceRc;
            if (AvxAdjustSurface(dout, &whd, wnd->fullscreen))
                AfxThrowError();
        }
        break;
    }
    case auxEventId_PAINT:
    {
        AfxRedrawWindow(wnd, NIL);
        break;
    }
    }

    AfxInvokeObjects(&wnd->widCls, (void*)_AuxWidEmitEventCb, ev, 0, AFX_U32_MAX);

    return TRUE;
}

_AUX afxError _AfxWndChangeIconCb(afxWindow wnd, avxRaster font, avxRasterRegion const* rgn)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    // If the host platform doesn't offer custom icon support, we do with via draw system.
    avxRaster curr = wnd->iconFnt;

    if (curr != font)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            wnd->iconFnt = NIL;
        }

        if (font)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &font);
            AfxReacquireObjects(1, &font);
            wnd->iconFnt = font;
            wnd->iconCrop = *rgn;
        }
    }
    return err;
}

_AUX afxError _AfxWndChangeCursorCb(afxWindow wnd, avxRaster font, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    // If the host platform doesn't offer custom icon support, we do with via draw system.
    avxRaster curr = wnd->cursFnt;

    if (curr != font)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &curr);
            AfxDisposeObjects(1, &curr);
            wnd->cursFnt = NIL;
        }

        if (font)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &font);
            AfxReacquireObjects(1, &font);
            wnd->cursFnt = font;
            wnd->cursCrop = *rgn;
        }
    }
    return err;
}

_AUX _auxDdiWnd const _AUX_DDI_WND =
{
    .evhCb = _AuxWndEventHandlerSW,
    .adjustCb = _AfxWndAdjustCb,
    .titleCb = _AfxWndFormatTitleCb,
    .chIconCb = _AfxWndChangeIconCb,
    .chCursCb = _AfxWndChangeCursorCb,
};
