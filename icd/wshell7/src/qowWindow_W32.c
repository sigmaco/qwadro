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

#include "qowBase.h"
#pragma comment(lib,"dwmapi.lib")

extern afxKey const Scan1MakeToQwadroDereferenceMap[afxKey_TOTAL];

_QOWINL void _QowRectFromAfx(RECT* wrc, afxRect const* rc)
{
    wrc->left = rc->x;
    wrc->top = rc->y;
    wrc->right = rc->w + rc->x;
    wrc->bottom = rc->h + rc->y;
}

_QOWINL void _QowRectFromW32(afxRect* rc, RECT const* wrc)
{
    rc->x = wrc->left;
    rc->y = wrc->top;
    rc->w = wrc->right - wrc->left;
    rc->h = wrc->bottom - wrc->top;
}

_QOWINL HWND _QowWndGetHandleWin32(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return wnd->hWnd;
}

_QOW void CalcWindowValuesW32(HWND window, afxInt* out_extra_width, afxInt32* out_extra_height)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *out_extra_width = (r.right - r.left) - (c.right - c.left);
    *out_extra_height = (r.bottom - r.top) - (c.bottom - c.top);
}

_QOW void CalcWindowMarginsW32(HWND window, afxUnit* left, afxUnit* top, afxUnit* right, afxUnit* bottom)
{
    RECT r, c;
    GetWindowRect(window, &r);
    GetClientRect(window, &c);
    *left = c.left - r.left;
    *top = c.top - r.top;
    *right = r.right - c.right;
    *bottom = r.bottom - c.bottom;
}

// Returns the difference between window bounds and client area, in pixels
_QOW void GetWindowFrameMargins(HWND hwnd, afxInt32* left, afxInt32* top, afxInt32* right, afxInt32* bottom)
{
    RECT windowRect;
    RECT clientRect;
    POINT clientTopLeft = { 0, 0 };
    POINT clientBottomRight;

    // Get the full window rect (including frame, title bar, etc.)
    GetWindowRect(hwnd, &windowRect);

    // Get the client area size (in client coordinates)
    GetClientRect(hwnd, &clientRect);

    // Convert client area top-left to screen coordinates
    clientBottomRight.x = clientRect.right;
    clientBottomRight.y = clientRect.bottom;

    ClientToScreen(hwnd, &clientTopLeft);
    ClientToScreen(hwnd, &clientBottomRight);

    *left = windowRect.left - clientTopLeft.x;
    *top = windowRect.top - clientTopLeft.y;
    *right = (windowRect.right - windowRect.left) - (clientBottomRight.x - clientTopLeft.x);
    *bottom = (windowRect.bottom - windowRect.top) - (clientBottomRight.y - clientTopLeft.y);
}

#if 0
_QOW HICON _AuxCreateWin32Icon(afxTarga const* tga, afxUnit xHotspot, afxUnit yHotspot, afxBool icon)
// Creates an RGBA icon or cursor
{
    afxError err = { 0 };
    HICON handle = NIL;

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = tga->whd[0];
    bi.bV5Height = -((LONG)tga->whd[1]);
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5AlphaMask = 0xff000000; // ARGB
    bi.bV5RedMask   = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask  = 0x000000ff;

    afxByte* dst = NIL;
    HDC dc = GetDC(NULL);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&dst, NULL, (DWORD)0);
    ReleaseDC(NULL, dc);

    if (!color) AfxThrowError();
    else
    {
        HBITMAP mask;

        if (!(mask = CreateBitmap(tga->whd[0], tga->whd[1], 1, 1, NULL))) AfxThrowError();
        else
        {
            afxByte const* src = tga->data.src;

            for (afxUnit i = 0; i < tga->whd[0] * tga->whd[1]; i++)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = src[3];
                dst += 4;
                src += 4;
            }

            ICONINFO ii = { 0 };
            ii.fIcon = icon;
            ii.xHotspot = xHotspot;
            ii.yHotspot = yHotspot;
            ii.hbmMask = mask;
            ii.hbmColor = color;

            if (!(handle = CreateIconIndirect(&ii)))
                AfxThrowError();

            DeleteObject(mask);
        }
        DeleteObject(color);
    }
    return handle;
}
#endif

_QOW afxError _AuxExtractWin32Cursor(HCURSOR cursor, int* width, int* height, unsigned* pixels_abgr, int* hotspot_x, int* hotspot_y)
{
    ICONINFO info;
    GetIconInfo(cursor, &info);
    BOOL bw_cursor = (info.hbmColor == NULL);

    BITMAP bmpinfo;
    memset(&bmpinfo, 0, sizeof(bmpinfo));
    if (bw_cursor && GetObject(info.hbmMask, sizeof(BITMAP), &bmpinfo) == 0)
    {
        DestroyCursor(cursor);
        DeleteObject(info.hbmColor);
        DeleteObject(info.hbmMask);
        return FALSE;
    }
    if (!bw_cursor && GetObject(info.hbmColor, sizeof(BITMAP), &bmpinfo) == 0)
    {
        DestroyCursor(cursor);
        DeleteObject(info.hbmColor);
        DeleteObject(info.hbmMask);
        return FALSE;
    }
    int pointer_width = bmpinfo.bmWidth;
    int pointer_height = (bmpinfo.bmHeight >= 0 ? bmpinfo.bmHeight : -bmpinfo.bmHeight) / (bw_cursor ? 2 : 1);

    if (width) *width = pointer_width;
    if (height) *height = pointer_height;
    if (hotspot_x) *hotspot_x = (int)info.xHotspot;
    if (hotspot_y) *hotspot_y = (int)info.yHotspot;
    if (!pixels_abgr)
    {
        DestroyCursor(cursor);
        DeleteObject(info.hbmColor);
        DeleteObject(info.hbmMask);
        return TRUE;
    }

    BITMAPINFOHEADER bmi;
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = bmpinfo.bmWidth;
    bmi.biHeight = -bmpinfo.bmHeight;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;
    HDC hdc = GetDC(NULL);
    if (GetDIBits(hdc, bw_cursor ? info.hbmMask : info.hbmColor, 0, (UINT)bmpinfo.bmHeight, pixels_abgr,
        (BITMAPINFO*)&bmi, DIB_RGB_COLORS) != bmpinfo.bmHeight)
    {
        DestroyCursor(cursor);
        DeleteObject(info.hbmColor);
        DeleteObject(info.hbmMask);
        ReleaseDC(NULL, hdc);
        return FALSE;
    }
    ReleaseDC(NULL, hdc);
    
    /*
        This code snippet processes an image of a pointer (a mouse cursor) and modifies its pixel data 
        depending on whether the pointer is a black-and-white cursor (bw_cursor is true) or a color cursor (bw_cursor is false). 
        It operates on a 1D array of ABGR pixels named pixels_abgr.
    */

    if (bw_cursor)
    {
        /*
            This case deals with a monochrome (black-and-white) cursor, typically represented using two bitmaps:
                The first pointer_height rows contain the AND mask (transparency).
                The next pointer_height rows contain the XOR mask (color).
            
            This essentially converts the two-mask format into a grayscale ARGB pixel, using:
                Alpha = inverse of AND mask
                RGB = grayscale from XOR mask
        */
        for (int y = 0; y < pointer_height; ++y)
        {
            for (int x = 0; x < pointer_width; ++x)
            {
                unsigned c = pixels_abgr[x + pointer_width * y]; // AND mask
                unsigned m = pixels_abgr[x + pointer_width * (pointer_height + y)]; // XOR mask
                unsigned a = 255 - (c & 0xff); // Create alpha from AND mask (c): 0 = opaque, 255 = transparent
                unsigned g = m & 0xff; // Use XOR mask as grayscale intensity
                pixels_abgr[x + pointer_width * y] = (a << 24) | (g << 16) | (g << 8) | g;
            }
        }
    }
    else
    {
        for (int y = 0; y < pointer_height; ++y)
        {
            for (int x = 0; x < pointer_width; ++x)
            {
                unsigned c = pixels_abgr[x + pointer_width * y];
                unsigned a = (c >> 24) & 0xff;
                unsigned r = (c >> 16) & 0xff;
                unsigned g = (c >> 8) & 0xff;
                unsigned b = (c) & 0xff;
                pixels_abgr[x + pointer_width * y] = (a << 24) | (b << 16) | (g << 8) | r;
            }
        }
    }

    DeleteObject(info.hbmColor);
    DeleteObject(info.hbmMask);
    DestroyCursor(cursor);
    return TRUE;
}

_QOW HICON _AuxCreateWin32IconFromRaster(avxRaster ras, avxRasterRegion const* rgn, afxBool cursor, afxUnit xHotspot, afxUnit yHotspot)
// Creates an RGBA icon or cursor
{
    afxError err = { 0 };
    HICON handle = NIL;

    AFX_ASSERT(ras);
    AFX_ASSERT(rgn);

    avxRange whd = rgn->whd;
    avxFormat fmt = AvxGetRasterFormat(ras);
    AFX_ASSERT(fmt == avxFormat_BGRA8un);

    BITMAPV5HEADER bi = { 0 };
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = whd.w;
    bi.bV5Height = -((LONG)whd.h);
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5AlphaMask = 0xff000000; // ARGB
    bi.bV5RedMask = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask = 0x000000ff;

    afxByte* dst = NIL;
    HDC dc = GetDC(NULL);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&dst, NULL, (DWORD)0);
    ReleaseDC(NULL, dc);

    if (!color) AfxThrowError();
    else
    {
        avxRasterIo iop = { 0 };
        iop.rgn = *rgn;
        //iop.rgn.whd = whd;
        iop.rowStride = whd.w * (bi.bV5BitCount / 8);
        
        if (AvxDumpRaster(ras, 1, &iop, dst, NIL, 0))
            AfxThrowError();

        afxDrawSystem dsys = AvxGetRasterHost(ras);
        AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, 0);

        HBITMAP mask;

        if (!(mask = CreateBitmap(whd.w, whd.h, 1, 1, NULL))) AfxThrowError();
        else
        {
            ICONINFO ii = { 0 };
            ii.fIcon = !cursor;
            ii.xHotspot = xHotspot;
            ii.yHotspot = yHotspot;
            ii.hbmMask = mask;
            ii.hbmColor = color;

            if (!(handle = CreateIconIndirect(&ii)))
                AfxThrowError();

            DeleteObject(mask);
        }
        DeleteObject(color);
    }
    return handle;
}

_QOW void ShakeWindow2(afxWindow wnd)
{

    HWND hwnd = wnd->hWnd;
    if (!IsWindow(hwnd)) return;

    RECT rect;
    GetWindowRect(hwnd, &rect);

    int x = rect.left;
    int y = rect.top;

    // Small shake offset
    const int shakeDelta = 10;
    const int shakeCount = 10;
    const int delay = 20; // milliseconds

    // Play a sound with it.
    MessageBeep(MB_ICONEXCLAMATION);
    // Flash the taskbar icon if the window isn't focused.
    FLASHWINFO fi = { sizeof(FLASHWINFO), hwnd, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
    FlashWindowEx(&fi);
    // Restore window if minimized.
    if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);

    for (int i = 0; i < shakeCount; i++)
    {
        int offset = (i % 2 == 0) ? shakeDelta : -shakeDelta;
        SetWindowPos(hwnd, NULL, x + offset, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        Sleep(delay);
    }

    // Return to original position
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

_QOW void ShakeWindow3(afxWindow wnd)
{

    HWND hwnd = wnd->hWnd;
    if (!IsWindow(hwnd)) return;

    RECT rect;
    GetWindowRect(hwnd, &rect);

    int originalX = rect.left;
    int originalY = rect.top;

    const int shakeDistance = 15;  // Larger offset = more visible
    const int shakeCount = 4;      // Fewer shakes = feels heavier
    const int delay = 50;          // Milliseconds between moves
    
    // Play a sound with it.
    MessageBeep(MB_ICONEXCLAMATION);
    // Flash the taskbar icon if the window isn't focused.
    FLASHWINFO fi = { sizeof(FLASHWINFO), hwnd, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
    FlashWindowEx(&fi);
    // Restore window if minimized.
    if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);

    for (int i = 0; i < shakeCount; ++i)
    {
        SetWindowPos(hwnd, NULL, originalX + shakeDistance, originalY, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        Sleep(delay);

        SetWindowPos(hwnd, NULL, originalX - shakeDistance, originalY, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        Sleep(delay);
    }

    // Return to original position
    SetWindowPos(hwnd, NULL, originalX, originalY, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}

_QOW void ShakeWindow(afxWindow wnd)
{

    HWND hwnd = wnd->hWnd;
    if (!IsWindow(hwnd)) return;

    RECT rect;
    GetWindowRect(hwnd, &rect);

    int originalX = rect.left;
    int originalY = rect.top;

    const int shakeDistance = 10;  // How far it moves (in both X and Y)
    const int shakeCount = 8;      // Number of shakes
    const int delay = 50;// 30;          // Delay between shakes (ms)

    // Play a sound with it.
    MessageBeep(MB_ICONEXCLAMATION);
    // Flash the taskbar icon if the window isn't focused.
    FLASHWINFO fi = { sizeof(FLASHWINFO), hwnd, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
    FlashWindowEx(&fi);
    // Restore window if minimized.
    if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);

    for (int i = 0; i < shakeCount; ++i)
    {
        int offsetX = ((i % 2 == 0) ? 1 : -1) * shakeDistance;
        int offsetY = ((i % 3 == 0) ? 1 : -1) * shakeDistance;

        SetWindowPos(hwnd, NULL,
            originalX + offsetX,
            originalY + offsetY,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        Sleep(delay);
    }

    // Return to original position
    SetWindowPos(hwnd, NULL, originalX, originalY, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

_QOW LRESULT WINAPI _QowWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = { 0 };
    afxWindow wnd = (afxWindow)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!wnd)
    {
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    afxEnvironment env = AfxGetHost(wnd);
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (wnd->m.redrawFrameRequested)
    {
        wnd->m.redrawFrameRequested = FALSE;
        SetWindowTextA(hWnd, wnd->m.title.buf);
        RedrawWindow(hWnd, NIL, NIL, RDW_FRAME);
    }

    if (wnd->m.redrawSurfaceRequested)
    {

    }

    afxByte buf[4096] = { 0 };
    afxUnit bufSiz = sizeof(buf);

    afxUnit32 tid = AfxGetTid();
    afxUnit32 doutTid = AfxGetObjectTid(wnd);
    AFX_ASSERT(doutTid == tid);
        
    AFX_ASSERT(wnd->hWnd == hWnd);
#if 0
    HDC dc = wnd->m.w32.hDc;

    if (dc)
    {
        SwapBuffers(dc); // deadlocks all
    }
#endif

    switch (message)
    {
    case WM_INPUT:
    case WM_INPUT_DEVICE_CHANGE:
    {
        MSG msg = { 0 };
        msg.hwnd = hWnd;
        msg.lParam = lParam;
        msg.message = message;
        msg.wParam = wParam;
        _QowProcessSystemInputMessageWin32(&msg, env, wnd);  // we need this at focus loss to gain a last chance to release all keys.

        // RIM_INPUT --- Input occurred while the application was in the foreground.
        // The application must call DefWindowProc so the system can perform cleanup.
        break;
    }
    case WM_SETCURSOR:
    {
        // When the cursor enters a window's client area, Windows sends WM_SETCURSOR.

        if (LOWORD(lParam) == HTCLIENT)
        {
            if (wnd->hCursor)
            {
                wnd->hCursorBkp = SetCursor(wnd->hCursor);
                return TRUE;
            }

            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.wnd = wnd;
            ev.id = auxEventId_CURS_ENTER;
            AfxEmitEvent(wnd, &ev.ev);
        }

        break;
    }
    case WM_MOUSEMOVE:
    {
        if (!wnd->trackingMouse)
        {
            TRACKMOUSEEVENT tme = { sizeof(tme) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            tme.dwHoverTime = HOVER_DEFAULT; // Or a custom time in ms

            TrackMouseEvent(&tme);
            wnd->trackingMouse = TRUE;
        }

        // handle mouse move here.

        POINTS points = MAKEPOINTS(lParam);

        afxV2d curr = { AFX_REAL(points.x), AFX_REAL(points.y) };

        AfxV2dSub(wnd->m.cursMove, wnd->m.cursPos, curr);
        AfxV2dCopy(wnd->m.cursPos, curr);

        afxV2d screen = { AFX_REAL(wnd->m.frameRc.w), AFX_REAL(wnd->m.frameRc.h) };

        AfxV2dNdc(wnd->m.cursPosNdc, wnd->m.cursPos, screen);
        AfxV2dNdc(wnd->m.cursMoveNdc, wnd->m.cursMove, screen);

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_CURS_MOTION;
        AfxEmitEvent(wnd, &ev.ev);

        //data2->breake = TRUE;
        return 0;//break;
    }
    case WM_MOUSELEAVE:
    {
        /*
            The mouse left the client area of the window specified in a prior call to TrackMouseEvent.
            All tracking requested by TrackMouseEvent is canceled when this message is generated.
            The application must call TrackMouseEvent when the mouse reenters its window if it requires further tracking of mouse hover behavior.
        */

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_CURS_LEAVE;
        AfxEmitEvent(wnd, &ev.ev);

        // Mouse left the window; reset tracking.
        wnd->trackingMouse = FALSE;

        return 0;//break;
    }
    case WM_MOUSEHOVER:
    {
        /*
            The mouse hovered over the client area of the window for the period of time specified in a prior call to TrackMouseEvent.
            Hover tracking stops when this message is generated. The application must call TrackMouseEvent again if it requires further
            tracking of mouse hover behavior.

            WM_MOUSEHOVER is a Windows message that gets sent when the mouse hovers over a window (or control) for a specified amount of time without moving. 
            It's not sent by default; you have to request it using the TrackMouseEvent() API.

            It is used to detect when the user pauses the mouse over a window or control (e.g., for tooltips, hover effects, UI animations, etc.).
            It gives you more precise control than just WM_MOUSEMOVE, which is sent constantly as the mouse moves.
        */

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_CURS_HOVER;
        AfxEmitEvent(wnd, &ev.ev);

        // Mouse hovered without moving
        //MessageBoxW(hWnd, L"Mouse hovered!", L"Info", MB_OK);
        wnd->trackingMouse = FALSE; // Reset so it can trigger again

        return 0;//break;
    }
#if 0
    case WM_NCMOUSELEAVE:
    {
        // The same meaning as WM_MOUSELEAVE except this is for the nonclient area of the window.

        auxEvent ev = { 0 };
        ev.id = auxEventId_CURS_OUT;
        AfxEmitEvent(wnd, &ev);

        break;
    }
    case WM_NCMOUSEHOVER:
    {
        // The same meaning as WM_MOUSEHOVER except this is for the nonclient area of the window.
        break;
    }
#endif
    case WM_SYSCOMMAND: // Intercept System Commands
    {
        switch (wParam & 0xfff0)
        {
        case SC_KEYMENU:
        {
            // User trying to access window menu (using ALT key)
            // Disable OS menu to avoid stall.
            return 0;
            //break;
        }
        case SC_SCREENSAVE: // screensaver trying to start
        case SC_MONITORPOWER: // monitor trying to enter powersave mode
        {
            if (wnd->m.fullscreen)
            {
                // Disallow screen saver and screen blanking in fullscreen mode.
                return 0;
            }            
            break;
        }
        }
        break;
    }
    case WM_CLOSE: // Did We Receive A Close Message?
    {
        // Sent as a signal that a window or an application should terminate.
        // wParam = This parameter is not used.
        // lParam = This parameter is not used.
        // If an application processes this message, it should return zero.

        if (wnd->m.fullscreen)
        {
            AfxTakeFullscreen(wnd, FALSE);
        }

        //PostQuitMessage(0); // Send A Quit Message
        ShowWindow(hWnd, SW_MINIMIZE);

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_CLOSE;
        AfxEmitEvent(wnd, &ev.ev);

        return 0; // handled. Also prevent process at end of this function
    }
    case WM_ACTIVATE: // Watch For Window Activate Message
    {
        if (!HIWORD(wParam)) // Check Minimization State
        {
            wnd->m.active = TRUE; // Program Is Active
        }
        else
        {
            wnd->m.active = FALSE; // Program Is No Longer Active
        }

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_ACTIVATE;
        AfxEmitEvent(wnd, &ev.ev);

        return 0; // Return To The Message Loop
    }
    case WM_SHOWWINDOW:
    {
        //UpdateWindow(dout->idd->wnd);
        break;
    }
    case WM_MOVING:
    {
        /*
            WM_MOVING (0x0216) is a Windows message sent while the user is dragging the window to move it, before the move is finalized.
            Sent repeatedly during a user-initiated window move operation.
            While the user drags the window frame to a new position.
            Your app to track or modify the window’s position in real-time as it moves.

            wParam: Not used.
            lParam: Pointer to a RECT structure representing the window's proposed new position in screen coordinates
            typedef struct tagRECT {
                LONG left;
                LONG top;
                LONG right;
                LONG bottom;
            } RECT;
            You can modify the RECT in lParam to constrain or snap the window’s position during the move.

            Typical Use Cases:
                Restricting the window from moving outside certain screen bounds,
                Snapping window edges to certain positions or grids,
                Tracking window position continuously while it’s being dragged,
                Implementing custom drag behaviors.

            Example:
            {
                RECT* pRect = (RECT*)lParam;

                // Example: restrict window from moving off the left edge of the screen
                if (pRect->left < 0)
                {
                    int width = pRect->right - pRect->left;
                    pRect->left = 0;
                    pRect->right = width;
                }

                return TRUE; // Indicate message was processed and RECT modified
            }
        */

        break;
    }
    case WM_MOVE:
    {
        /*
            WM_MOVE (0x0003) is a Windows message sent to a window when its position has changed (but not its size).
            Notifies the window that it has been moved to a new position. 
            Sent after the window’s position changes (e.g., user drags the window or a program moves it).

            wParam: Not used, always zero.

            lParam: Contains the new position of the window’s client area (relative to the screen), packed as:
                Low-order word (LOWORD(lParam)) = X-coordinate (left)
                High-order word (HIWORD(lParam)) = Y-coordinate (top

            Typical Use Cases:
                Tracking window position changes for saving/restoring window location,
                Updating UI elements that depend on window position,
                Triggering actions when the window moves (e.g., repositioning popups or related windows)

            Example:
            {
                int xPos = LOWORD(lParam);
                int yPos = HIWORD(lParam);

                // Respond to the new position here, e.g. update UI or save window position

                break;
            }
        */

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_MOVE;
        AfxEmitEvent(wnd, &ev.ev);

        break;
    }
    case WM_SIZING:
    {
        /*
            WM_SIZING (0x0214) is a Windows message sent to a window while the user is resizing the window. 
            It lets your application track or modify the resizing in real-time.

            Sent repeatedly while the user is dragging the window border or sizing handles,
            during a user-initiated resize operation (before the window size is finalized).

            wParam: Indicates which edge or corner is being dragged. Possible values:
                WMSZ_LEFT (1)	Left edge
                WMSZ_RIGHT (2)	Right edge
                WMSZ_TOP (3)	Top edge
                WMSZ_TOPLEFT (4)	Top-left corner
                WMSZ_TOPRIGHT (5)	Top-right corner
                WMSZ_BOTTOM (6)	Bottom edge
                WMSZ_BOTTOMLEFT (7)	Bottom-left corner
                WMSZ_BOTTOMRIGHT (8)	Bottom-right corner

            lParam: Pointer to a RECT structure that contains the proposed new window position.
            typedef struct tagRECT {
                LONG left;
                LONG top;
                LONG right;
                LONG bottom;
            } RECT;
            You can modify the RECT in lParam to change or constrain the size or position during the resizing.

            Use Cases:
                Enforcing minimum or maximum window sizes during resizing,
                Snapping window edges to a grid or other windows,
                Implementing custom resizing behavior (e.g., aspect ratio lock),
                Providing visual feedback during resizing.

            WM_SIZING is sent only when the user is interactively resizing the window by dragging its borders or corners. When? 
            During the resize operation, repeatedly as the user drags.
            WM_WINDOWPOSCHANGING is sent whenever the window’s size, position, or Z-order is about to change (not just user resizing, 
            but also programmatic calls like SetWindowPos). When? Before the size/position change takes effect.

            Example:
            {
                RECT* pRect = (RECT*)lParam;

                // Example: enforce minimum width and height of 300x200
                int minWidth = 300;
                int minHeight = 200;

                int width = pRect->right - pRect->left;
                int height = pRect->bottom - pRect->top;

                if (width < minWidth)
                    pRect->right = pRect->left + minWidth;
                if (height < minHeight)
                    pRect->bottom = pRect->top + minHeight;

                return TRUE; // Indicate message was processed
            }
        */

        // Correct dragged edges by aspect ratio. The aspect ratio should be provided by user.

        afxReal ratio = wnd->m.aspRatio;
        if (!ratio) break;

        RECT frame = { 0 };
        AdjustWindowRectEx(&frame, wnd->dwStyle, FALSE, wnd->dwExStyle);
        
        RECT* rcInOut = (RECT*)lParam;

        switch (/*edge*/wParam)
        {
        case WMSZ_RIGHT:
        case WMSZ_BOTTOMRIGHT:
        case WMSZ_LEFT:
        case WMSZ_BOTTOMLEFT:
        {
            rcInOut->bottom = rcInOut->top + (frame.bottom - frame.top) +
                (int)(((rcInOut->right - rcInOut->left) - (frame.right - frame.left)) / ratio);
            break;
        }
        case WMSZ_TOPRIGHT:
        case WMSZ_TOPLEFT:
        {
            rcInOut->top = rcInOut->bottom - (frame.bottom - frame.top) -
                (int)(((rcInOut->right - rcInOut->left) - (frame.right - frame.left)) / ratio);
            break;
        }
        case WMSZ_BOTTOM:
        case WMSZ_TOP:
        {
            rcInOut->right = rcInOut->left + (frame.right - frame.left) +
                (int)(((rcInOut->bottom - rcInOut->top) - (frame.bottom - frame.top)) * ratio);
            break;
        }
        default: break;
        }
#if 0
        if (!wnd->m.adjusting)
        {
            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.wnd = wnd;
            ev.id = auxEventId_EDGE;
            AfxEmitEvent(wnd, &ev);
        }
#endif
        break;
    }
    case WM_SIZE:
    {
        /*
            WM_SIZE (0x0005) is a Windows message that is sent after a window has been resized. 
            It lets your application know that the client area dimensions have changed, 
            so you can respond accordingly (e.g., resize child controls, update layouts, redraw content).

            wParam - Resize Type: Indicates the type of resizing that occurred. wParam values:
              SIZE_RESTORED   = 0 - The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
              SIZE_MINIMIZED  = 1 - The window has been minimized.
              SIZE_MAXIMIZED  = 2 - The window has been maximized.
              SIZE_MAXSHOW    = 3 - Message is sent to mem pop-up windows when some other window has been restored to its former size.
              SIZE_MAXHIDE    = 4 - Message is sent to mem pop-up windows when some other window is maximized.

            lParam – New Size: Packed with the new width and height of the client area:
            LOWORD(lParam) = new width
            HIWORD(lParam) = new height
            You can extract them in C like this:
            int width  = LOWORD(lParam);
            int height = HIWORD(lParam);

            Sent after the window's size has changed, whether by user action, programmatically via SetWindowPos, 
            or by the system (e.g., when maximizing).

            Unlike WM_WINDOWPOSCHANGED, this focuses specifically on the client area's size, which is what your app typically draws in.

            Common Use Cases:
                Resize or reposition child windows or controls,
                Update layout or graphics (e.g., adjust rendering buffers),
                Recalculate UI elements based on new dimensions

            Example:
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                switch (wParam)
                {
                    case SIZE_MINIMIZED:
                        // Handle minimize
                        break;
                    case SIZE_MAXIMIZED:
                    case SIZE_RESTORED:
                        // Handle normal or maximized resize
                        ResizeChildControls(hwnd, width, height);
                        break;
                }
                return 0;
            }
        */
#if 0
        //if (doutD->resizable)
        {

            afxRect cr = { 0 };
            // lParam já é client area, filha da puta.
            cr.w = LOWORD(lParam);
            cr.h = HIWORD(lParam);

            if (cr.w * cr.h) // don't set to zero
            {
                AfxAdjustWindow(wnd, NIL, &cr);
            }
        }
#endif
        afxRect cr = { 0 };
        cr.w = LOWORD(lParam);
        cr.h = HIWORD(lParam);
        wnd->m.surfaceRc = cr;

        // AdjustWindow does already emit this event.
        if (!wnd->m.adjusting)
        {
            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.wnd = wnd;
            ev.id = auxEventId_SIZE;
            AfxEmitEvent(wnd, &ev.ev);
        }
        break;
    }
    case WM_WINDOWPOSCHANGING:
    {
        /*
            WM_WINDOWPOSCHANGING (0x0046) is a Windows message sent before a window's size, position, or Z-order is changed. 
            It gives your application a chance to modify or prevent those changes.

            When your window receives WM_WINDOWPOSCHANGING, the lParam contains a pointer to a modifiable WINDOWPOS structure.

            lParam: Pointer to a WINDOWPOS structure:
            typedef struct tagWINDOWPOS {
              HWND hwnd;
              HWND hwndInsertAfter;
              int  x;       // proposed new X position
              int  y;       // proposed new Y position
              int  cx;      // proposed new width
              int  cy;      // proposed new height
              UINT flags;   // behavior flags (e.g., SWP_NOMOVE, SWP_NOSIZE, etc.)
            } WINDOWPOS;

            wParam: Unused (typically zero)

            Sent when the system or an app is about to move, size, or change the Z-order of a window.
            You can modify the WINDOWPOS structure in lParam to change or block those changes.
            It's called before WM_WINDOWPOSCHANGED, which is sent after the change.

            Related Flags in WINDOWPOS->flags. Some common ones:
                SWP_NOMOVE - Don't change position
                SWP_NOSIZE - Don't change size
                SWP_NOZORDER - Don't change Z-order
                SWP_NOREDRAW - Don't redraw after change,
            You can set these flags to suppress certain behaviors.

            We might use this to:
                Enforce minimum/maximum window size,
                Restrict movement to a certain area,
                Lock Z-order,
                Prevent resizing entirely.

            Example:
            {
                LPWINDOWPOS pos = (LPWINDOWPOS)lParam;

                // Prevent window from moving off-screen
                if (pos->x < 0) pos->x = 0;
                if (pos->y < 0) pos->y = 0;

                // Prevent resizing smaller than 300x200
                if (pos->cx < 300) pos->cx = 300;
                if (pos->cy < 200) pos->cy = 200;

                return 0;
            }
        */



        break; //return 0;
    }
    case WM_WINDOWPOSCHANGED:
    {
        /*
            WM_WINDOWPOSCHANGED (0x0047) is a Windows message used in the Windows API to notify a window that its size, 
            position, or Z-order has changed.

            When a window receives WM_WINDOWPOSCHANGED, the lParam contains a pointer to a WINDOWPOS structure.

            lParam: Pointer to a WINDOWPOS structure:
            typedef struct tagWINDOWPOS {
              HWND hwnd;
              HWND hwndInsertAfter;
              int  x;
              int  y;
              int  cx;
              int  cy;
              UINT flags;
            } WINDOWPOS;
            
            wParam: Not used (typically 0)

            Sent after the window's size, position, or Z-order has changed (as a result of a SetWindowPos or similar).
            This message is sent after WM_WINDOWPOSCHANGING, which is sent before the change happens.
            You typically handle this to adjust layout, invalidate areas, or respond to movement or resizing.

            If you want to prevent changes to size or position, handle WM_WINDOWPOSCHANGING instead.
            WM_WINDOWPOSCHANGED is not meant to veto changes --- it's purely informational

            Example:
            {
                LPWINDOWPOS pos = (LPWINDOWPOS)lParam;
                // You can inspect pos->x, pos->y, pos->cx, pos->cy, etc.
                // Useful to reposition child controls, etc.
                return 0;
            }
        */

#if 0
        {

            RECT wr = { 0 };
            GetClientRect(hWnd, &wr);
            afxRect cr = { wr.left, wr.top, wr.right, wr.bottom };

            if (cr.w * cr.h) // don't set to zero
            {
                AfxAdjustWindow(wnd, NIL, &cr);
            }
        }
#endif

#if 0
        if (wnd->m.fullscreen)
        {
            AfxTakeFullscreen(wnd, FALSE);
            AfxTakeFullscreen(wnd, TRUE);
        }
#endif

        WINDOWPOS* wp = (WINDOWPOS*)lParam;
        RECT wrc = { wp->x, wp->y, wp->cx, wp->cy };
        afxRect rc;
        _QowRectFromW32(&rc, &wrc);
        wnd->m.frameRc = rc;

        if (wnd->m.cursConfined)
        {
            afxRect r = wnd->m.cursConfinRect;
            AfxMakeWindowCursory(wnd, &r, FALSE);
            AfxMakeWindowCursory(wnd, &r, TRUE);
#if 0
            ClientToScreen(hwnd, (POINT*)&r);
            ClientToScreen(hwnd, ((POINT*)&r) + 1);
            ClipCursor(&r);
#endif
        }
        break;//return 0;
    } 
    case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
    {
        HDC dc;
        afxSurface dout = wnd->m.surfaceDout;
        AvxCallSurfaceEndpoint(dout, 0, &dc);
        avxRange const resolution = { GetDeviceCaps(dc, HORZRES), GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, PLANES) };
        afxReal64 physAspRatio = AfxFindPhysicalAspectRatio(GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));
        afxReal refreshRate = GetDeviceCaps(dc, VREFRESH);

        avxModeSetting mode = { 0 };
        AvxQuerySurfaceMode(dout, &mode);
        mode.refreshRate = refreshRate;
        mode.wpOverHp = physAspRatio;
        mode.resolution = resolution;
        AvxResetSurfaceMode(dout, &mode);

        afxDesktop* dwm = wnd->m.dwm;
        dwm->wpOverHp = physAspRatio;
        dwm->refreshRate = refreshRate;
        dwm->res.w = resolution.w;
        dwm->res.h = resolution.h;
        dwm->res.d = resolution.d;
        dwm->wrOverHr = (afxReal64)dwm->res.x / (afxReal64)dwm->res.y;

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_CHANGED;
        AfxEmitEvent(wnd, &ev.ev);

        break;
    }
    case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
    {
        // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
        // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
        
        afxInt32 mleft, mtop, mright, mbottom;
        GetWindowFrameMargins(wnd->hWnd, &mleft, &mtop, &mright, &mbottom);
        wnd->m.frameMarginL = mleft;
        wnd->m.frameMarginT = mtop;
        wnd->m.frameMarginR = mright;
        wnd->m.frameMarginB = mbottom;

        //wnd->dwStyle = GetWindowLongW(hWnd, GWL_STYLE);
        //wnd->dwExStyle = GetWindowLongW(hWnd, GWL_EXSTYLE);

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_STYLE;
        AfxEmitEvent(wnd, &ev.ev);

        break;
    }
#if 0
    case WM_DROPFILES: // Sent when the user drops a file on the window of an application that has registered itself as a recipient of dropped files.
    {
        // TODO build a kind of handler for draw input to deal with it.

        afxFileDrop fdrop;
        AFX_ZERO(&fdrop);

        POINT ppt;
        HDROP hDrop = (HDROP)wParam;
        DragQueryPoint(hDrop, &ppt);

        fdrop.x = ppt.x;
        fdrop.y = ppt.y;
        afxUnit cnt = DragQueryFileA(hDrop, 0xFFFFFFFF, NIL, NIL);

        AfxMakeArray(&fdrop.files, sizeof(afxChar const*), 0, NIL, 0);

        afxChar* name = NIL;
        afxUnit len = 0;
        afxUnit i;

        afxDrawSystem dsys;
        AvxGetSurfaceHost(dout, &dsys);
        AfxAssertType(dctxD, afxFcc_DSYS);
        afxMmu mmu = AfxGetDrawSystemMmu(dsys);
        AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &mmu);

        for (i = 0; i < cnt; i++)
        {
            len = DragQueryFileA(hDrop, i, NIL, 0);

            if (!(name = AfxAllocate(mmu, len + 1, 1, 0, AfxHere()))) AfxReportError("");
            else
            {
                DragQueryFileA(hDrop, i, name, len + 1);
                afxUnit arrIdx;
                void *arrel = AfxPushArrayUnit(&fdrop.files, &arrIdx);
                AfxCopy2(1, sizeof(name), name, arrel);
            }
        }

        //afxEvent ev;
        //AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &doutD->obj, &fdrop);
        //AfxNotifyObject(&doutD->obj, &ev);

        for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
        {
            AfxReportMessage("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
        }

        for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
        {
            AfxDeallocate(mmu, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
        }

        AfxEmptyArray(&fdrop.files, FALSE, FALSE);

        DragFinish(hDrop);
        //data2->breake = TRUE;
        //return 0;
        break;
    }
#endif
    case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
    {
        if (wnd->m.cursHidden)
        {
            ShowCursor(TRUE);
            env->m.cursHidden = FALSE;
        }
        
        if (wnd->m.cursConfined)
        {
            AFX_ASSERT(wnd == env->m.cursCapturedOn);
            afxBool liberated = !!ClipCursor(NULL);
            env->m.cursCapturedOn = NIL;
        }
#if 0
        if (wnd->m.fullscreen)
        {
            ShowWindow(wnd->hWnd, SW_MINIMIZE);
        }
#endif

        _AfxEnvFocusWindowCb(env, 0, NIL, NIL);

        //ShakeWindow(wnd);

        {
            // GAMBIARRA
            // we need this at focus loss to gain a last chance to release all keys.
            MSG msg = { 0 };
            msg.hwnd = hWnd;
            msg.message = message;
            msg.lParam = lParam;
            msg.wParam = wParam;
            _QowProcessSystemInputMessageWin32(&msg, env, wnd);
        }
        break;
    }
    case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
    {
        POINT cursorPos;
        if (GetCursorPos(&cursorPos))
        {
            // Convert the cursor position to the client area of the window
            ScreenToClient(hWnd, &cursorPos);
            // Get the window's client rectangle
            RECT clientRect;
            if (GetClientRect(hWnd, &clientRect))
            {
                // Check if the cursor is inside the client rectangle
                if (cursorPos.x >= clientRect.left && cursorPos.x <= clientRect.right &&
                    cursorPos.y >= clientRect.top && cursorPos.y <= clientRect.bottom)
                {
                    // Cursor is inside the window's client area
                    if (wnd->m.cursHidden)
                    {
                        ShowCursor(FALSE);
                        env->m.cursHidden = TRUE;
                    }
                    else if (wnd->m.cursConfined)
                    {
                        RECT cr;
                        GetClientRect(hWnd, &cr);
                        ClientToScreen(hWnd, (POINT*)&cr.left);
                        ClientToScreen(hWnd, (POINT*)&cr.right);
                        afxBool confined = !!ClipCursor(&cr);

                        if (confined)
                            env->m.cursCapturedOn = wnd;
                    }
                }
            }
        }

        _AfxEnvFocusWindowCb(env, 0, wnd, NIL);

        break;
    }
    case WM_PAINT:
    {
        // emit damage event to let the application do what ever it want.

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.wnd = wnd;
        ev.id = auxEventId_PAINT;
        AfxEmitEvent(wnd, &ev.ev);

        // Required?
#if !0
        ValidateRect(hWnd, NULL);
#endif
        // An application returns zero if it processes this message.
        return 0;
    }
    case WM_ERASEBKGND:
    {
        // Flicker is usually caused by interference via WM_ERASEBKGND. 
        // If you haven't already, intercept WM_ERASEBKGND and do nothing in the regions where you are displaying OpenGL content.
        // An application should return nonzero if it erases the background; otherwise, it should return zero.
        return 1;
    }
    default: break;
    }
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

_QOW afxError _QowWndChIconCb(afxWindow wnd, avxRaster ras, avxRasterRegion const* rgn)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    HICON hIcon = NIL;

    if (ras)
    {
        if (!(hIcon = _AuxCreateWin32IconFromRaster(ras, rgn, FALSE, 0, 0))) AfxThrowError();
        else
        {
            
            HWND hWnd = wnd->hWnd;
            //Change both icons to the same icon handle.
            SendMessageA(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            //This will ensure that the application icon gets changed too.
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            SendMessageA(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        }
    }

    if (!err)
    {
        if (wnd->hIcon)
            DestroyIcon(wnd->hIcon);

        wnd->hIcon = hIcon;
    }
    return err;
}

_QOW afxError _QowWndChCursCb(afxWindow wnd, avxRaster ras, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    HCURSOR hCursor = NIL;

    if (ras)
    {
        if (!(hCursor = (HCURSOR)_AuxCreateWin32IconFromRaster(ras, rgn, TRUE, hotspotX, hotspotY))) AfxThrowError();
        else
        {
            HWND hWnd = wnd->hWnd;
            //wnd->hCursorBkp = SetCursor(hCursor);
        }
    }

    if (!err)
    {
        if (wnd->hCursor)
            DestroyCursor(wnd->hCursor);

        wnd->hCursor = hCursor;
    }
    return err;
}

_QOW afxBool DoutNotifyOvy(afxWindow wnd, afxUnit bufIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    wnd->lastBufIdx = bufIdx;
    wnd->swap = 1;

    return 1;
}

_QOW afxError _QowWndDamageCb(afxWindow wnd, afxRect const* rc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    // Force a WM_PAIN event.

    RECT wrc;
    _QowRectFromAfx(&wrc, rc);
    InvalidateRect(wnd->hWnd, &wrc, FALSE);

    return err;
}

_QOW afxError _QowWndRedrawCb(afxWindow wnd, afxRect const* rc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    HDC dc = wnd->hSurfaceDc;
    
    if (dc)
    {
        if (wnd->swap)
        {
            wnd->swap = FALSE;
            //SwapBuffers(dc);
            //DwmFlush();

            afxUnit rate = 0;
            //AfxFormatWindowTitle(wnd, "%u --- Draw I/O System --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology --- Public Test Build", rate);
            SetWindowTextA(wnd->hWnd, AfxGetStringData(&wnd->m.title.s, 0));
        }
    }
    return err;
}

#if 0
_QOW afxBool AfxGetOnSurfaceScreenPosition(afxWindow wnd, afxUnit const screenPos[2], afxUnit surfPos[2])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(screenPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    POINT point = { .x = screenPos[0],.y = screenPos[1] };
    afxBool rslt = !!ScreenToClient(wnd->hWnd, &point);
    AFX_ASSERT(surfPos);
    surfPos[0] = point.x;
    surfPos[1] = point.y;
    return rslt;
}

_QOW afxBool AfxGetOnScreenSurfacePosition(afxWindow wnd, afxUnit const surfPos[2], afxUnit screenPos[2])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AFX_ASSERT(surfPos);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    POINT point = { .x = surfPos[0], .y = surfPos[1] };
    afxBool rslt = !!ClientToScreen(wnd->hWnd, &point);
    AFX_ASSERT(screenPos);
    screenPos[0] = point.x;
    screenPos[1] = point.y;
    return rslt;
}
#endif

afxRect AdjustAnchoredRect(afxRect const* r, afxUnit newW, afxUnit newH, afxAnchor anchor)
{
    /*
        Say you have a rect (x, y, w, h) and an anchor (e.g. CENTER | MIDDLE).
        Now, suppose you change the width and height (w', h').
        You want to produce a new rect (x', y', w', h') such that the anchored point stays still;
        for example, whatever pixel the anchor represents before the resize is the same after.
        That means your adjustment logic must shift x and/or y depending on what the anchor is.
    */
    afxRect out = *r;
    afxUnit dx = newW - r->w;
    afxUnit dy = newH - r->h;

    // Horizontal adjustment
    if (anchor & afxAnchor_CENTER)
        out.x -= dx / 2;
    else if (anchor & afxAnchor_RIGHT)
        out.x -= dx;
    // (LEFT = no change)

    // Vertical adjustment
    if (anchor & afxAnchor_MIDDLE)
        out.y -= dy / 2;
    else if (anchor & afxAnchor_BOTTOM)
        out.y -= dy;
    // (TOP = no change)

    out.w = newW;
    out.h = newH;
    return out;
}

afxRect ClipRectToAnchor(afxRect const* screen, afxRect const* win, afxAnchor anchor)
{
    // Clip a smaller rectangle (the window) out of a larger rectangle (the screen or monitor area), positioned according to an anchor.
    // Given the screen rect and the window’s size (w,h), compute its top-left (x,y) so 
    // that the chosen anchor point of the window coincides with the anchor point of the screen.

    afxRect out = *win;

    // Horizontal alignment
    //if ((anchor & afxAnchor_LEFT))
        //out.x = screen->x;
    //else 
    if ((anchor & afxAnchor_CENTER))
        out.x = screen->x + (screen->w - win->w) / 2;
    else if ((anchor & afxAnchor_RIGHT))
        out.x = screen->x + screen->w - win->w;

    // Vertical alignment
    //if ((anchor & afxAnchor_TOP))
        //out.y = screen->y;
    //else 
    if ((anchor & afxAnchor_MIDDLE))
        out.y = screen->y + (screen->h - win->h) / 2;
    else if ((anchor & afxAnchor_BOTTOM))
        out.y = screen->y + screen->h - win->h;

    return out;
}

afxRect AdjustRect(afxRect r, afxAnchor anchor, int dw, int dh)
{
    /*
        Anchoring is basically about where we consider the rectangle’s "pivot point" to be when we adjust it.

        For example:
        Anchor LEFT means adjustments happen with respect to the left edge; width grows to the right.
        Anchor CENTER means the rectangle expands or moves symmetrically around its center.
        Anchor RIGHT means width grows to the left (x decreases).
        Similarly for TOP/MIDDLE/BOTTOM vertically.

        That assumes:
        (x, y) is top-left corner (Win32-style coordinate system).
        w, h are positive.
        dw, dh are the desired deltas (positive to expand, negative to shrink).
    */

    // Adjust width
    if (anchor & afxAnchor_LEFT)
    {
        // Grow/shrink width to the right only
        r.w += dw;
    }
    else if (anchor & afxAnchor_CENTER)
    {
        // Grow/shrink symmetrically around center
        r.x -= dw / 2;
        r.w += dw;
    }
    else if (anchor & afxAnchor_RIGHT)
    {
        // Grow/shrink width to the left
        r.x -= dw;
        r.w += dw;
    }

    // Adjust height
    if (anchor & afxAnchor_TOP)
    {
        // Grow/shrink height downward
        r.h += dh;
    }
    else if (anchor & afxAnchor_MIDDLE)
    {
        // Grow/shrink symmetrically vertically
        r.y -= dh / 2;
        r.h += dh;
    }
    else if (anchor & afxAnchor_BOTTOM)
    {
        // Grow/shrink height upward
        r.y -= dh;
        r.h += dh;
    }

    return r;
}

afxRect PositionByAnchor(afxRect r, afxAnchor anchor, int px, int py)
{
    /*
        This is for placement instead of resizing.

        If we want to move the rectangle (not resize)
        If instead of resizing, we mean repositioning the rectangle based on an anchor point 
        (e.g., place this rect so its CENTER is at (px, py)), then we can do this:
    */
    switch (anchor)
    {
    case afxAnchor_LEFT | afxAnchor_TOP:
        r.x = px;
        r.y = py;
        break;

    case afxAnchor_CENTER | afxAnchor_MIDDLE:
        r.x = px - r.w / 2;
        r.y = py - r.h / 2;
        break;

    case afxAnchor_RIGHT | afxAnchor_BOTTOM:
        r.x = px - r.w;
        r.y = py - r.h;
        break;

    case afxAnchor_RIGHT | afxAnchor_TOP:
        r.x = px - r.w;
        r.y = py;
        break;

    case afxAnchor_LEFT | afxAnchor_BOTTOM:
        r.x = px;
        r.y = py - r.h;
        break;

    default:
        // Handle any combination
        if (anchor & afxAnchor_RIGHT) r.x = px - r.w;
        else if (anchor & afxAnchor_CENTER) r.x = px - r.w / 2;
        else r.x = px;

        if (anchor & afxAnchor_BOTTOM) r.y = py - r.h;
        else if (anchor & afxAnchor_MIDDLE) r.y = py - r.h / 2;
        else r.y = py;
    }
    return r;
}

_QOW afxRect _QowWndGetRectW32(afxWindow wnd, afxAnchor anchor)
{
    // Win32 Implementation Details

    afxRect rect = { 0, 0, 0, 0 };

    // Get client area size and position.

    RECT rcClient;
    if (!GetClientRect(wnd->hWnd, &rcClient))
        return rect;

    POINT origin = { rcClient.left, rcClient.top };
    ClientToScreen(wnd->hWnd, &origin);

    rect.w = rcClient.right - rcClient.left;
    rect.h = rcClient.bottom - rcClient.top;

    // Compute anchor-relative position.

    rect.x = origin.x;
    rect.y = origin.y;

    // Adjust coordinates according to anchor
    if (anchor & afxAnchor_CENTER)
        rect.x += rect.w / 2;
    else if (anchor & afxAnchor_RIGHT)
        rect.x += rect.w;

    if (anchor & afxAnchor_MIDDLE)
        rect.y += rect.h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        rect.y += rect.h;

    // This way, rect.x, rect.y represent the anchor point in screen space.

    // Wayland clients don't know their global position, so WE can only return 0.

    /*
        Usage example:

        afxRect rect;
        AfxGetWindowRect(win, afxAnchor_CENTER | afxAnchor_MIDDLE, &rect);
        rect.w += 200;
        rect.h += 100;
        AfxAdjustWindow(win, afxAnchor_CENTER | afxAnchor_MIDDLE, &rect);
    */

    return rect;
}

_QOW void _QowWndAdjustW32(afxWindow wnd, afxAnchor anchor, afxRect const* area)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    // Win32 Implementation Details

    // Interpret afxRect as client area target.

    afxRect rect = *area;

    RECT clientTarget = { 0, 0, rect.w, rect.h };
    AdjustWindowRectEx(&clientTarget, GetWindowLong(wnd->hWnd, GWL_STYLE),
        FALSE, GetWindowLong(wnd->hWnd, GWL_EXSTYLE));

    int totalW = clientTarget.right - clientTarget.left;
    int totalH = clientTarget.bottom - clientTarget.top;

    // Get current window geometry.

    RECT currentWin;
    GetWindowRect(wnd->hWnd, &currentWin);

    afxRect current = {
        currentWin.left,
        currentWin.top,
        currentWin.right - currentWin.left,
        currentWin.bottom - currentWin.top
    };

    // Compute anchor point.

    POINT anchorPt = { current.x, current.y };

    if (anchor & afxAnchor_CENTER)
        anchorPt.x = current.x + current.w / 2;
    else if (anchor & afxAnchor_RIGHT)
        anchorPt.x = current.x + current.w;

    if (anchor & afxAnchor_MIDDLE)
        anchorPt.y = current.y + current.h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        anchorPt.y = current.y + current.h;

    // Compute adjusted target rect.

    afxRect adjusted = rect;

    // If rect.x/y == 0, treat as "don't move" (Wayland-style)
    if (rect.x == 0 && rect.y == 0)
    {
        adjusted.x = current.x;
        adjusted.y = current.y;
    }
    else
    {
        // Align the target window so that the anchor remains fixed
        if (anchor & afxAnchor_RIGHT)
            adjusted.x = anchorPt.x - totalW;
        else if (anchor & afxAnchor_CENTER)
            adjusted.x = anchorPt.x - totalW / 2;

        if (anchor & afxAnchor_BOTTOM)
            adjusted.y = anchorPt.y - totalH;
        else if (anchor & afxAnchor_MIDDLE)
            adjusted.y = anchorPt.y - totalH / 2;
    }

    // Apply only if changed.

    if (current.x != adjusted.x || current.y != adjusted.y ||
        current.w != totalW || current.h != totalH)
    {
        SetWindowPos(wnd->hWnd, NULL,
            adjusted.x, adjusted.y,
            totalW, totalH,
            SWP_NOZORDER | SWP_NOACTIVATE);

    }

    wnd->m.frameRc.w = totalW;
    wnd->m.frameRc.h = totalH;
    wnd->m.frameRc.x += adjusted.x;
    wnd->m.frameRc.y += adjusted.y;
    wnd->m.surfaceRc = *area;
}

afxRect ResolveSurfaceRect(
    afxRect const* screen,
    afxRect const* desired,
    afxAnchor anchor,
    afxRect const* current)
{
    afxRect out = *desired;

    // If no anchor is specified, just preserve position
    if (anchor == 0)
    {
        out.x = current->x;
        out.y = current->y;
        return out;
    }

    // Compute the anchor points in both rects
    afxInt screenAx = screen->x;
    afxInt screenAy = screen->y;
    afxInt winAx = 0;
    afxInt winAy = 0;

    // Screen anchor point
    if (anchor & afxAnchor_LEFT)
        screenAx = screen->x;
    else if (anchor & afxAnchor_CENTER)
        screenAx = screen->x + screen->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        screenAx = screen->x + screen->w;

    if (anchor & afxAnchor_TOP)
        screenAy = screen->y;
    else if (anchor & afxAnchor_MIDDLE)
        screenAy = screen->y + screen->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        screenAy = screen->y + screen->h;

    // Window anchor point (relative to window's own rect)
    if (anchor & afxAnchor_LEFT)
        winAx = 0;
    else if (anchor & afxAnchor_CENTER)
        winAx = desired->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        winAx = desired->w;

    if (anchor & afxAnchor_TOP)
        winAy = 0;
    else if (anchor & afxAnchor_MIDDLE)
        winAy = desired->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        winAy = desired->h;

    // Position the window so its anchor point coincides with the screen's anchor point
    out.x = screenAx - winAx + desired->x;
    out.y = screenAy - winAy + desired->y;

    return out;
}

_QOW afxError _QowWndAdjustCb(afxWindow wnd, afxAnchor anchor, afxRect* area)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxRect areaT, areaL, areaTL, areaCM, areaB, areaBR;
    AfxGetWindowRect(wnd, afxAnchor_TOP, &areaT);
    AfxGetWindowRect(wnd, afxAnchor_LEFT, &areaL);
    AfxGetWindowRect(wnd, afxAnchor_TOP | afxAnchor_LEFT, &areaTL);
    AfxGetWindowRect(wnd, afxAnchor_BOTTOM, &areaB);
    AfxGetWindowRect(wnd, afxAnchor_BOTTOM | afxAnchor_RIGHT, &areaBR);
    AfxGetWindowRect(wnd, afxAnchor_CENTER | afxAnchor_MIDDLE, &areaCM);

    if (!anchor)
    {
        RECT winFrameRc, winClientRc;
        GetWindowRect(wnd->hWnd, &winFrameRc);
        GetClientRect(wnd->hWnd, &winClientRc);

        RECT frame = { area->x, area->y, area->w, area->h };
        AdjustWindowRectEx(&frame, GetWindowLong(wnd->hWnd, GWL_STYLE), FALSE, GetWindowLong(wnd->hWnd, GWL_EXSTYLE));
        frame.left = winFrameRc.left;
        frame.top = winFrameRc.top;

        SetWindowPos(wnd->hWnd, NULL,
            frame.left,
            frame.top,
            frame.right, frame.bottom,
            SWP_NOZORDER);

        wnd->m.frameRc.w = frame.right;
        wnd->m.frameRc.h = frame.bottom;
        wnd->m.frameRc.x = frame.left;
        wnd->m.frameRc.y = frame.top;
        area->x = 0;
        area->y = 0;
        wnd->m.surfaceRc = *area;
        return err;
    }

    afxRect rect = ResolveSurfaceRect(&wnd->m.screenRc, area, anchor, &wnd->m.frameRc);
    area->x = 0;
    area->y = 0;

    afxRect a = wnd->m.screenRc;
    //afxRect rect = AfxAdjustAnchoredRect(&a, area->w, area->h, anchor);

    RECT winFrameRc, winClientRc;
    GetWindowRect(wnd->hWnd, &winFrameRc);
    GetClientRect(wnd->hWnd, &winClientRc);

    afxUnit ml, mt, mr, mb;
    CalcWindowMarginsW32(wnd->hWnd, &ml, &mt, &mr, &mb);

    if (!ml)
        ml = wnd->m.frameMarginL;

    if (!mt)
        mt = wnd->m.frameMarginT;

    //afxRect rect = anchor ? AfxClipRectToAnchor(&wnd->m.screenRc, area, anchor) : wnd->m.frameRc;
    RECT frame = { 0, 0, rect.w, rect.h };
    AdjustWindowRectEx(&frame, GetWindowLong(wnd->hWnd, GWL_STYLE), FALSE, GetWindowLong(wnd->hWnd, GWL_EXSTYLE));
    frame.left = 0;
    frame.top = 0;

    if (!anchor || (anchor & afxAnchor_TOP))
        frame.top = 0;

    if (!anchor || (anchor & afxAnchor_LEFT))
        frame.left = 0;

    int fullW = frame.right - frame.left;
    int fullH = frame.bottom - frame.top;

    // This code is really a mess.
    // Wayland fucked me all ways possible.

    HWND hWndInsertAfter2 = wnd->m.alwaysOnTop ? HWND_TOPMOST : NIL;
    UINT swpFlags2 = 0;// SWP_NOOWNERZORDER | SWP_NOZORDER;

    SetWindowPos(wnd->hWnd, hWndInsertAfter2,
        frame.left,
        frame.top,
        frame.right, frame.bottom,
        SWP_NOZORDER);

    wnd->m.frameRc.w = fullW;
    wnd->m.frameRc.h = fullH;
    wnd->m.frameRc.x = frame.left;
    wnd->m.frameRc.y = frame.top;
    wnd->m.surfaceRc = *area;
    wnd->m.surfaceRc.x = 0;
    wnd->m.surfaceRc.y = 0;

    AfxGetWindowRect(wnd, afxAnchor_TOP, &areaT);
    AfxGetWindowRect(wnd, afxAnchor_LEFT, &areaL);
    AfxGetWindowRect(wnd, afxAnchor_TOP | afxAnchor_LEFT, &areaTL);
    AfxGetWindowRect(wnd, afxAnchor_BOTTOM, &areaB);
    AfxGetWindowRect(wnd, afxAnchor_BOTTOM | afxAnchor_RIGHT, &areaBR);
    AfxGetWindowRect(wnd, afxAnchor_CENTER | afxAnchor_MIDDLE, &areaCM);

    //_QowWndAdjustW32(wnd, anchor, area);
    return err;



    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    afxDesktop* dwm = wnd->m.dwm;

    HWND hWndInsertAfter = wnd->m.alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST;
    UINT swpFlags = 0;// SWP_NOOWNERZORDER | SWP_NOZORDER;

    {
        //AFX_ASSERT2(wnd->m.frameRc.whd.w > (afxUnit)rc->origin.x, wnd->m.frameRc.whd.h > (afxUnit)rc->origin.y);
        //AFX_ASSERT4(surface->w, wnd->m.frameRc.w > (afxUnit)area->w, surface->h, wnd->m.frameRc.h > (afxUnit)surface->h);

        afxRect rc2 = *area;
        rc2.x = AFX_MIN_CASTED(rc2.x, wnd->m.frameRc.w - 1, afxUnit);
        rc2.y = AFX_MIN_CASTED(rc2.y, wnd->m.frameRc.h - 1, afxUnit);
        rc2.w = AFX_MAX(1, rc2.w/*AFX_MIN(surface->w, wnd->m.frameRc.w)*/);
        rc2.h = AFX_MAX(1, rc2.h/*AFX_MIN(surface->h, wnd->m.frameRc.h)*/);

        if ((wnd->m.surfaceRc.x != rc2.x) ||
            (wnd->m.surfaceRc.y != rc2.y) ||
            (wnd->m.surfaceRc.w != rc2.w) ||
            (wnd->m.surfaceRc.h != rc2.h))
        {
            afxInt32 extraWndWidth, extraWndHeight;
            CalcWindowValuesW32(wnd->hWnd, &extraWndWidth, &extraWndHeight);

            AFX_ASSERT2(rc2.w, rc2.h);
            wnd->m.frameRc.w = rc2.w + extraWndWidth;
            wnd->m.frameRc.h = rc2.h + extraWndHeight;
            wnd->m.frameRc.x += rc2.x;
            wnd->m.frameRc.y += rc2.y;
            wnd->m.surfaceRc = rc2;

            if (!SetWindowPos(wnd->hWnd, hWndInsertAfter, wnd->m.frameRc.x, wnd->m.frameRc.y, wnd->m.frameRc.w, wnd->m.frameRc.h, swpFlags))
                AfxThrowError();
        }
    }
#if 0
    afxSurface dout = wnd->m.surfaceDout;    
    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        afxRect whd;
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->m.surfaceRc.w;
        whd.h = wnd->m.surfaceRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->m.fullscreen))
            AfxThrowError();
    }
    dout = wnd->m.frameDout;
    if (dout)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        afxRect whd;
        AvxGetSurfaceArea(dout, &whd);
        whd.w = wnd->m.frameRc.w;
        whd.h = wnd->m.frameRc.h;

        if (AvxAdjustSurface(dout, &whd, wnd->m.fullscreen))
            AfxThrowError();
    }
#endif
    return err;
}

_QOW afxBool _QowWndChangeVisibility(afxWindow wnd, afxBool visible)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return !!ShowWindow(wnd->hWnd, visible ? SW_SHOW : SW_HIDE);
}

_QOW afxUnit _QowWndFormatTitleCb(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));
    SetWindowTextA(wnd->hWnd, wnd->m.title.buf);
    return 0;
}

_QOW _auxDdiWnd const _QOW_DDI_WND =
{
    .damageCb = _QowWndDamageCb,
    .redrawCb = _QowWndRedrawCb,
    .adjustCb = _QowWndAdjustCb,
    .chIconCb = _QowWndChIconCb,
    .titleCb = _QowWndFormatTitleCb,
};

_QOW afxError _QowWndDtorCb(afxWindow wnd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    AFX_ASSERT(AfxGetTid() == AfxGetObjectTid(wnd));

    AfxDeregisterChainedClasses(&wnd->m.classes);

    _AUX_WND_CLASS_CONFIG.dtor(wnd);

    //AfxDisposeObjects(1, &wnd->m.dout);

    AfxChangeWindowCursor(wnd, NIL, NIL, 0, 0); // detach any custom cursor
    AfxChangeWindowIcon(wnd, NIL, NIL); // detach any custom icon
    AFX_ASSERT(!wnd->hIcon);

    DragAcceptFiles(wnd->hWnd, FALSE);
    DestroyWindow(wnd->hWnd);

    return err;
}

_QOW afxError _QowWndCtorCb(afxWindow wnd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxWindowConfig const* wcfg = (afxWindowConfig const*)(args[1]) + invokeNo;
    
    if (!wcfg)
    {
        AfxThrowError();
        return err;
    }

    afxDisplay disp = wcfg->disp;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VDU, 1, &disp);
    afxUnit dispPort = wcfg->dport;

    afxClassConfig widClsCfg = _AUX_WID_CLASS_CONFIG;
    widClsCfg.fixedSiz = sizeof(AFX_OBJ(afxWidget));
    widClsCfg.ctor = (void*)_QowWidCtorCb;
    widClsCfg.dtor = (void*)_QowWidDtorCb;

    if (_AUX_WND_CLASS_CONFIG.ctor(wnd, (void*[]) { env, (void*)wcfg, (void*)&widClsCfg }, 0))
    {
        AfxThrowError();
        return err;
    }

    wnd->m.ddi = &_QOW_DDI_WND;

    DWORD dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // Window Style

    //dwExStyle |= WS_EX_CONTEXTHELP;
    //dwExStyle |= WS_EX_NOREDIRECTIONBITMAP;

    if (wnd->m.fullscreen)
    {
        dwExStyle |= WS_EX_TOPMOST;
        dwStyle |= WS_POPUP;
        //ShowCursor(FALSE);
    }
    else
    {
        if (wnd->m.floating)
            dwExStyle |= WS_EX_TOPMOST;

        dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX;

        if (!wnd->m.decorated) dwStyle |= WS_POPUP;
        else
        {
            dwStyle |= WS_CAPTION;

            if (wnd->m.resizable)
                dwStyle |= WS_MAXIMIZEBOX | WS_THICKFRAME;
        }
    }

    wnd->dwExStyle = dwExStyle;
    wnd->dwStyle = dwStyle;

    HWND hWnd = CreateWindowExA(dwExStyle, env->wndClss.lpszClassName, wnd->m.title.s.start, 
                                dwStyle, wcfg->x, wcfg->y, 1, 1, NIL, NIL, env->wndClss.hInstance, NIL);

    if (!hWnd) AfxThrowError();
    else
    {
        wnd->hIcon = NIL;
        wnd->hFrameDc = NIL;
        wnd->hSurfaceDc = GetDC(hWnd);
        wnd->hWnd = hWnd;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)wnd);


        afxInt32 mleft, mtop, mright, mbottom;
        GetWindowFrameMargins(wnd->hWnd, &mleft, &mtop, &mright, &mbottom);
        wnd->m.frameMarginL = mleft;
        wnd->m.frameMarginT = mtop;
        wnd->m.frameMarginR = mright;
        wnd->m.frameMarginB = mbottom;

        MONITORINFO mi = { .cbSize = sizeof(mi) };
        GetMonitorInfo(MonitorFromWindow(wnd->hWnd, MONITOR_DEFAULTTONEAREST), &mi);
        afxRect screenRc =
        {
            .x = mi.rcWork.left,
            .y = mi.rcWork.top,
            .w = mi.rcWork.right - mi.rcWork.left,
            .h = mi.rcWork.bottom - mi.rcWork.top
        };
        wnd->m.screenRc = screenRc;

#if 0
        if (!wnd->m.fullscreen)
        {
            HMENU hMenu = CreateMenu();
            HMENU hFileMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hFileMenu, "File");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/0, "New");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/1, "Open");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/2, "Save");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/3, "Import");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/4, "Export");
            AppendMenu(hFileMenu, MF_STRING, /*ID_FILE_EXIT*/5, "Quit");

            HMENU hEditMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hEditMenu, "Edit");

            HMENU hRenderMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hRenderMenu, "Render");
            AppendMenu(hRenderMenu, MF_STRING, /*ID_FILE_EXIT*/0, "Wireframe");

            HMENU hWindowMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hWindowMenu, "Window");
            AppendMenu(hWindowMenu, MF_STRING, /*ID_FILE_EXIT*/0, "Spawn new");
            AppendMenu(hWindowMenu, MF_STRING, /*ID_FILE_EXIT*/1, "Go fullscreen");
            AppendMenu(hWindowMenu, MF_STRING, /*ID_FILE_EXIT*/1, "Print screenshot");

            // Set the menu to the window
            SetMenu(hWnd, hMenu);
        }
#endif

        DragAcceptFiles(hWnd, TRUE);

        afxSurface dout;
        afxSurfaceConfig scfg = { 0 };
        scfg = wcfg->dout;
        scfg.doNotClip = FALSE;

        scfg.iop.endpointNotifyObj = wnd;
        scfg.iop.endpointNotifyFn = (void*)DoutNotifyOvy;
        scfg.iop.w32.hInst = env->wndClss.hInstance;
        scfg.iop.w32.hWnd = hWnd;

        if (AvxAcquireSurface(wcfg->dsys, &scfg, &dout)) AfxThrowError();
        else
        {
            wnd->m.surfaceDout = dout;

            afxRect rc = { .x = wcfg->x, .y = wcfg->y, .w = wcfg->dout.ccfg.whd.w, .h = wcfg->dout.ccfg.whd.h };
            rc.w = AFX_MAX(1, rc.w);;
            rc.h = AFX_MAX(1, rc.h);
            AfxAdjustWindow(wnd, disp, dispPort, wnd->m.anchor, &rc);
#if !0
            if (scfg.presentAlpha && (scfg.presentAlpha != avxVideoAlpha_OPAQUE))
            {
                DWM_BLURBEHIND bb = { 0 };
                bb.dwFlags = DWM_BB_ENABLE;
                bb.fEnable = TRUE;
                DwmEnableBlurBehindWindow(wnd->hWnd, &(bb)); // ausent on Windows XP
            }
#endif

            ShowWindow(hWnd, SHOW_OPENWINDOW);
        }

        if (err)
            DestroyWindow(wnd->hWnd);
    }

    if (err)
        _AUX_WND_CLASS_CONFIG.dtor(wnd);

    return err;
}
