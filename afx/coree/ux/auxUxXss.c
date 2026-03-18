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

// This software is part of Advanced User Experience Extensions.

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_ENVIRONMENT_C
#define _AUX_WINDOW_C
#include "auxIcd.h"
#include "../xss/xss.h"

 AFX void GenAcqObj(xssVm vm);
 AFX void GenRelObj(void* data);
 AFX void XssPushError(xssVm vm, afxUnit slot, afxError err);
 AFX void XssPushBool(xssVm vm, afxUnit slot, afxBool val);
 AFX void XssPushReal(xssVm vm, afxUnit slot, afxReal val);
 AFX void XssPushReal64(xssVm vm, afxUnit slot, afxReal64 val);
 AFX void XssPushInt(xssVm vm, afxUnit slot, afxInt val);
 AFX void XssPushNat(xssVm vm, afxUnit slot, afxUnit val);
 AFX void XssPushSize(xssVm vm, afxUnit slot, afxSize val);
 AFX afxBool XssPullBool(xssVm vm, afxUnit slot);
 AFX afxReal XssPullReal(xssVm vm, afxUnit slot);
 AFX afxReal64 XssPullReal64(xssVm vm, afxUnit slot);
 AFX void XssPullReal3(xssVm vm, afxUnit slot, afxV3d v);
 AFX afxInt XssPullInt(xssVm vm, afxUnit slot);
 AFX afxUnit XssPullNat(xssVm vm, afxUnit slot);
 AFX afxSize XssPullSize(xssVm vm, afxUnit slot);
 AFX afxHandle XssPullHandle(xssVm vm, afxUnit slot);
 AFX void XssPushHandle(xssVm vm, afxUnit slot, afxHandle han);
 AFX void* XssPullInstance(xssVm vm, afxUnit slot);
 AFX void XssPushInstance(xssVm vm, afxUnit slot, void* obj);
 AFX void XssPullUri(xssVm vm, afxUnit slot, afxUri* uri);
 AFX void XssPullString(xssVm vm, afxUnit slot, afxString* s);
 AFX void XssUnhandle(xssVm vm, afxUnit cnt, afxHandle han[]);
 AFX void XssHandleEachCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[]);
 AFX afxUnit XssHandleAnyCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[]);
 AFX afxUnit XssHandleAnyVar(xssVm vm, afxString const* domain, afxUnit baseSlot, afxUnit cnt, afxString const name[], afxHandle handles[]);
 AFX afxError XssCall(xssVm vm, afxHandle method);

 void _XsEnvGetKeyPressure(xssVm vm)
 {
     afxUnit seat = XssPullNat(vm, 1);
     afxUnit code = XssPullNat(vm, 2);
     XssPushReal64(vm, 0, AfxGetKeyPressure(seat, code));
 }

 void _XsEnvIsMousePressed(xssVm vm)
 {
     afxUnit seat = XssPullNat(vm, 1);
     afxUnit code = XssPullNat(vm, 2);
     XssPushBool(vm, 0, AfxIsMousePressed(seat, code));
 }

 void _XsEnvGetMouseMotion(xssVm vm)
 {
     afxUnit seat = XssPullNat(vm, 1);
     afxV2d delta;
     AfxGetMouseMotion(seat, delta, NIL, NIL);
     XssPushReal(vm, 0, delta[0]);
     XssPushReal(vm, 1, delta[1]);
 }

 void _XsEnvGetMouseWheelDelta(xssVm vm)
 {
     //afxEnvironment env = XssPullInstance(vm, 0);
     afxUnit seat = XssPullNat(vm, 1);
     XssPushReal(vm, 0, AfxGetMouseWheelDelta(seat));
 }

 void _XsWndAcquire(xssVm vm)
 {
     afxEnvironment env = XssPullInstance(vm, 0);
     afxUnit x = XssPullNat(vm, 1);
     afxUnit y = XssPullNat(vm, 2);
     afxUnit w = XssPullNat(vm, 3);
     afxUnit h = XssPullNat(vm, 4);

     afxWindow wnd;
     afxWindowConfig wcfg = { 0 };
     wcfg.x = x;
     wcfg.y = y;
     wcfg.dout.ccfg.whd.w = w;
     wcfg.dout.ccfg.whd.h = h;
     AfxConfigureWindow(env, &wcfg, NIL, NIL);
     AfxAcquireWindow(env, &wcfg, &wnd);
     XssPushInstance(vm, 0, wnd);
 }

 void _XsWndDispose(xssVm vm)
 {
     afxWindow wnd = XssPullInstance(vm, 0);
     AfxDisposeObjects(1, &wnd);
 }

 void _XsWndGetSurface(xssVm vm)
 {
     afxWindow wnd = XssPullInstance(vm, 0);
     afxSurface dout;
     AfxGetWindowSurface(wnd, &dout);
     XssPushHandle(vm, 0, dout);
 }

 void _XsWndAdjust(xssVm vm)
 {
     afxWindow wnd = XssPullInstance(vm, 0);
     afxRect rc = { 0 };
     AfxAdjustWindow(wnd, NIL, 0, NIL, &rc);
 }

 void _XsWndSetCaption(xssVm vm)
 {
     afxWindow wnd = XssPullInstance(vm, 0);
     char const* s = xssGetSlotString(vm, 1);
     AfxFormatWindowTitle(wnd, "%s", s);
 }

_AUX xssConsoleCrateInfo envXssCrates[] =
{
{
    .name = AFX_STRING("ux"),
    .classCnt = 2,
    .classes = (xssConsoleClassInfo[])
    {
    {
        .name = AFX_STRING("Environment"),
        .methodCnt = 9,
        .methods = (xssConsoleMethodInfo[])
        {
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Establish()"),
            .method = &gActiveEnv
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Abolish"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetAvx"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetAmx"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetDisplay"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetKeyPressure(_,_)"),
            .method = _XsEnvGetKeyPressure
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("IsMousePressed(_,_)"),
            .method = _XsEnvIsMousePressed
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetMouseMotion(_)"),
            .method = _XsEnvGetMouseMotion
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetMouseWheel(_)"),
            .method = _XsEnvGetMouseWheelDelta
        },
        {
            0 
        }
        }
    },
    {
        .name = AFX_STRING("Window"),
        .methodCnt = 8,
        .methods = (xssConsoleMethodInfo[])
        {
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Acquire(_,_,_,_)"),
            .method = _XsWndAcquire
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Dispose(_)"),
            .method = _XsWndDispose
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("GetSurface(_)"),
            .method = _XsWndGetSurface
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Adjust(_,_,_,_,_)"),
            .method = _XsWndAdjust
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Caption(_,_)"),
            .method = _XsWndSetCaption
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("Redraw(_)"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("LoadIcon(_)"),
            .method = NIL
        },
        {
            .isStatic = TRUE,
            .signature = AFX_STRING("LoadCursor(_)"),
            .method = NIL
        },
        {
            0
        }
        }
    },
    {
        0
    }
    }
}
};

