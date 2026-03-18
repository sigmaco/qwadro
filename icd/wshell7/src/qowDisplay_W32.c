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

/*
    IMPORTANT: Research about the following implementation.

    Due to historical development of VGA from a basic screen controller to a full sidecar-computer,
    I concluded draw devices were not first class citizen, so not having any distintion of display devices.

    Things like "DISPLAY1" are logical devices installed to represent a connection to monitor. 
    Think I link the display port itself instead of monitor. This is why we can only retrieve video mode setting of it.

    A draw device is a GPU as a side-computer able to do work.
    A display device is a display port controller able to manager a specially configurated framebuffer and presentations to it.
    A monitor is just a remote endpoint receiving the display device output.
    A adapter is usually it all together.
*/

#include "../../icd_tarzgl4/src/zglUtils.h"
#include "../../icd_tarzgl4/src/zglCommands.h"
#include "../../icd_tarzgl4/src/zglObjects.h"
#include "qowBase.h"
#include "qowVideo_W32.h"

//#define _AVOID_WGL_FUNCS 1

//#define USE_SCREEN_DC 1
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "setupapi.lib")

#define COBJMACROS
#include <windows.h>
#include <dxgi.h>
#include <initguid.h>  // Required to define DXGI IIDs
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "dxgi.lib")
// Manually define IID_IDXGIFactory (for C projects)
DEFINE_GUID(IID_IDXGIFactory, 0x7b7166ec, 0x21c7, 0x44ae, 0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69);

_QOW afxResult _QwoDpyIoctrlCb(afxDisplay dpy, afxUnit reqCode, va_list va)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case 0:
    {
        
        break;
    }
    case 1: // init
    {
        break;
    }
    case 2: // Terminates the device. The device must first be stopped and then closed (in that order). This function should be called before the system shuts down.
    {
        break;
    }
    case 3: // Opens the device. This function must only be called after the device has been initialized.
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (dsys)
        {

        }
        break;
    }
    case 4: // Closes the device. This function must only be called after the device has been stopped.
    {
        afxDrawSystem dsys = va_arg(va, afxDrawSystem);

        if (dsys)
        {

        }
        break;
    }
    default: break;
    }

    return rslt;
}

_QOW afxError _QowVduDtorCb(afxDisplayPort vdu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxDisplay dpy = AfxGetHost(vdu);

#if USE_DXGI_DISPLAY

    if (dpy->type == qowDisplayType_DXGI)
    {
        IDXGIOutput_Release(vdu->pDxgiOutput);
    }

#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY

    if (dpy->type == qowDisplayType_GDI)
    {
        
    }
#endif//USE_GDI_DISPLAY
    
    return err;
}

_QOW afxError _QowVduCtorCb(afxDisplayPort vdu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxDisplay dpy = AFX_CAST(afxDisplay, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    afxDisplayPortConfig const* cfg = AFX_CAST(afxDisplayPortConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    if (_AUX_VDU_CLASS_CONFIG.ctor(vdu, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }

#if USE_DXGI_DISPLAY

    if (dpy->type == qowDisplayType_DXGI)
    {
        UINT outputIndex = cfg->portId;

        IDXGIOutput* pOutput = NULL;
        if (S_OK != (IDXGIAdapter_EnumOutputs(dpy->pDxgiAdapter, outputIndex, &pOutput)))
        {
            AfxThrowError();
        }

        DXGI_OUTPUT_DESC desc;
        IDXGIOutput_GetDesc(pOutput, &desc);

        MONITORINFOEXA moni = { 0 };
        moni.cbSize = sizeof(moni);
        GetMonitorInfoA(desc.Monitor, &moni);

        afxRect workArea = AFX_RECT(moni.rcWork.left, moni.rcWork.top, moni.rcWork.right, moni.rcWork.bottom);
        afxRect fullArea = AFX_RECT(moni.rcMonitor.left, moni.rcMonitor.top, moni.rcMonitor.right, moni.rcMonitor.bottom);
        vdu->m.fullArea = fullArea;
        vdu->m.workArea = workArea;

        vdu->dxgiDesc = desc;
        vdu->pDxgiOutput = pOutput;
        vdu->dxgiOutputIndex = outputIndex;

        vdu->m.prime = (moni.dwFlags & MONITORINFOF_PRIMARY);
    }

#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY

    if (dpy->type == qowDisplayType_GDI)
    {
        afxUnit portId = AfxGetObjectId(vdu);

        afxUnit iteratedCnt = 0;
        afxUnit monIdx = 0;
        DISPLAY_DEVICEA dd2 = { 0 };
        dd2.cb = sizeof(dd2);
        while (EnumDisplayDevicesA(dpy->ddinfo.DeviceName, monIdx, &dd2, NIL))
        {
            iteratedCnt++;

            if (monIdx == portId)
            {
                AfxReportComment("    #%u = %s (%s) 0x%x; %s (%s)", monIdx, dd2.DeviceString, dd2.DeviceName, dd2.StateFlags, dd2.DeviceID, dd2.DeviceKey);

                //ddm.dmSpecVersion = DM_SPECVERSION;
                // passing dd2.DeviceName always return nothing
                /*
                    If you get something like \\.\DISPLAY2 then you should be fine, but if you get something
                    like \\.\DISPLAY2\Monitor0 then you can't use it directly with EnumDisplaySettings because it expects the former.
                */

                vdu->ddminfo = dd2;

                afxUnit dmIdx = 0;
                DEVMODEA ddm = { 0 };
                ddm.dmSize = sizeof(ddm);
                while (EnumDisplaySettingsExA(dpy->ddinfo.DeviceName, ENUM_REGISTRY_SETTINGS + dmIdx, &ddm, NIL))
                {
                    if (dmIdx == 0) // register the ENUM_REGISTRY_SETTINGS version only.
                    {
                        AfxReportComment("    %ux%u:%u@%u --- %u, (%i, %i)%u, %u, 0x%x", ddm.dmPelsWidth, ddm.dmPelsHeight, ddm.dmBitsPerPel, ddm.dmDisplayFrequency, ddm.dmLogPixels, ddm.dmPosition.x, ddm.dmPosition.y, ddm.dmDisplayOrientation, ddm.dmDisplayFixedOutput, ddm.dmDisplayFlags);

                        afxDisplayPortConfig vdui = { 0 };
                        vdui.dimWh[0] = ddm.dmPelsWidth;
                        vdui.dimWh[1] = ddm.dmPelsHeight;
                        vdui.resWh[0] = ddm.dmPelsWidth;
                        vdui.resWh[1] = ddm.dmPelsHeight;

                        AfxMakeString(&vdui.name, 0, dd2.DeviceName, 0);
                        AfxMakeString(&vdui.label, 0, dd2.DeviceString, 0);

                        //dpy->ddinfo = dd;
                        //dpy->ddminfo = dd2;

                        afxDisplayMode modes[256];
                        afxUnit dmCnt = 0;// _ZglEnumerateDisplayModes(icd, monIdx, avxFormat_BGRA8un, 256, modes);

                        for (afxUnit i = 0; i < dmCnt; i++)
                        {
                            AfxReportMessage("fmt %d %dx%d@%d %x %x %i", modes[i].fmt, modes[i].width, modes[i].height, modes[i].freq, modes[i].scaling, modes[i].scanlineOrdering, modes[i].stereo);
                        }

                        avxFormat formats[256];
                        afxUnit fmtCnt = 0;// _ZglEnumerateDisplayFormats(icd, monIdx, 256, formats);

                        for (afxUnit i = 0; i < fmtCnt; i++)
                        {
                            avxFormatDescription fd;
                            AvxDescribeFormats(1, &formats[i], &fd);
                            AfxReportMessage("fmt %.*s", AfxPushString(&fd.tag));
                        }
                    }
                    dmIdx++;

                    break;
                }

                break;
            }
            monIdx++;
        }

        if (iteratedCnt == 0)
            AfxThrowError();

        HDC hDc = CreateDCA("DISPLAY", dpy->ddinfo.DeviceName, 0, NIL);

        // Width, in pixels, of the screen; or for printers, the width, in pixels, of the printable area of the page.
        // Height, in raster lines, of the screen; or for printers, the height, in pixels, of the printable area of the page.
        vdu->m.resWh[0] = GetDeviceCaps(hDc, HORZRES);
        vdu->m.resWh[1] = GetDeviceCaps(hDc, VERTRES);

        // Width and height, in millimeters, of the physical screen.
        vdu->m.dimWh[0] = GetDeviceCaps(hDc, HORZSIZE);
        vdu->m.dimWh[1] = GetDeviceCaps(hDc, VERTSIZE);

        int dpiX = GetDeviceCaps(hDc, LOGPIXELSX);
        int dpiY = GetDeviceCaps(hDc, LOGPIXELSY);
        vdu->m.dpi[0] = dpiX;
        vdu->m.dpi[1] = dpiY;

        {
            /*
                On Windows 7, monitor DPI is not actually distinct between monitors. Even though we can
                enumerate monitors with EnumDisplayMonitors(), get the monitor a window is on with MonitorFromWindow(),
                get display settings per monitor with EnumDisplaySettings() or GetDeviceCaps(),
                we still get the same DPI for all monitors unless we read EDID or registry hacks.
                Microshit do it intentionally.

                But we are pirates! And we will not surrender to it humiliation.
                We can get the physical size of a monitor and its resolution, and then calculate the physical DPI ourself.

                HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
                MONITORINFOEX mi = { sizeof(mi) };
                GetMonitorInfo(hMonitor, &mi);
                DEVMODE devMode = { 0 };
                devMode.dmSize = sizeof(devMode);
                EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

                Now we have devMode.dmPelsWidth and devMode.dmPelsHeight.
                Now we need to get the physical size of the monitor. This is the hard part; Win32 does not give us this directly.

                If hdc is NULL, this gives us the screen size in millimeters (as reported by the display driver).
                int widthMM = GetDeviceCaps(hdc, HORZSIZE);
                int heightMM = GetDeviceCaps(hdc, VERTSIZE);

                Then calculate DPI:
                float dpiX = devMode.dmPelsWidth / (widthMM / 25.4f); // 25.4 mm per inch
                float dpiY = devMode.dmPelsHeight / (heightMM / 25.4f);

                BUT: this data is often inaccurate or faked by drivers or WDDM as this part of API is highly virtualized.
                The OS may report 96 DPI regardless of the real hardware.

                We can compute "per-monitor DPI" manually on Windows 7 but it's an approximation.
                We are reverse-engineering what GetDpiForMonitor() does.
                It can work, but it relies on accurate data from OS, which is not guaranteed.

                Windows 7 itself does not use or expose this data for DPI scaling, so even if we do the math, our app is working independently of the OS DPI system.
            */
            float dpiX = vdu->m.resWh[0] / (vdu->m.dimWh[0] / 25.4f); // 25.4 mm per inch
            float dpiY = vdu->m.resWh[1] / (vdu->m.dimWh[1] / 25.4f);

            /*
                Windows often reports 96 DPI by default, regardless of the actual physical DPI.
                This is known as logical DPI, and it’s part of how Windows scales text and UI elements.
                It doesn't mean our monitor is actually 96 DPI.

                Just don't expect Windows to match it, because 96 DPI is a UI convention, not a measurement.
            */
#if 0
            AFX_ASSERT(dpy->m.dpi[0] == (afxUnit)dpiX);
            AFX_ASSERT(dpy->m.dpi[1] == (afxUnit)dpiY);
#endif
        }

        // Number of adjacent color bits for each pixel.
        // NOTE: When nIndex is BITSPIXEL and the device has 15bpp or 16bpp, the return value is 16.
        vdu->m.bpp = GetDeviceCaps(hDc, BITSPIXEL);

        // Number of color planes.
        vdu->m.planeCnt = GetDeviceCaps(hDc, PLANES);

        // Number of entries in the device's color table, if the device has a color depth of no more than 8 bits per pixel. For devices with greater color depths, -1 is returned.
        vdu->m.paletteSiz = GetDeviceCaps(hDc, NUMCOLORS);

        // Flag that indicates the clipping capabilities of the device. If the device can clip to a rectangle, it is 1. Otherwise, it is 0.
        vdu->m.clipCapable = GetDeviceCaps(hDc, CLIPCAPS);

        // For display devices: the current vertical refresh rate of the device, in cycles per second (Hz).
        // A vertical refresh rate value of 0 or 1 represents the display hardware's default refresh rate. 
        // This default rate is typically set by switches on a display card or computer motherboard, or by a configuration program that does not use display functions such as ChangeDisplaySettings.
        vdu->m.freq = GetDeviceCaps(hDc, VREFRESH);

        // Value that indicates the raster capabilities of the device, as shown in the following table.
        /*
            RC_BANDING	Requires banding support.
            RC_BITBLT	Capable of transferring bitmaps.
            RC_BITMAP64	Capable of supporting bitmaps larger than 64 KB.
            RC_DI_BITMAP	Capable of supporting the SetDIBits and GetDIBits functions.
            RC_DIBTODEV	Capable of supporting the SetDIBitsToDevice function.
            RC_FLOODFILL	Capable of performing flood fills.
            RC_PALETTE	Specifies a palette-based device.
            RC_SCALING	Capable of scaling.
            RC_STRETCHBLT	Capable of performing the StretchBlt function.
            RC_STRETCHDIB	Capable of performing the StretchDIBits function.
        */
        //dpy->m.rasCaps = GetDeviceCaps(hDc, RASTERCAPS);

        DeleteDC(hDc);
    }

#endif//USE_GDI_DISPLAY

    if (err)
    {
        if (_AUX_VDU_CLASS_CONFIG.dtor(dpy))
            AfxThrowError();
    }

    return err;
}

_QOW afxUnit _QowDpyQueryModesCb(afxDisplay dpy, afxUnit port, avxFormat fmt, afxUnit cnt, afxDisplayMode modes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    AFX_ASSERT(modes);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxDisplayPort vdu = AfxGetDisplayPort(dpy, port);
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

#if USE_DXGI_DISPLAY
    if (dpy->type == qowDisplayType_DXGI)
    {
        static DXGI_FORMAT const avxToDxgiFmtMap[] =
        {
            [avxFormat_UNDEFINED] = DXGI_FORMAT_UNKNOWN,
            [avxFormat_RGBA32f] = DXGI_FORMAT_R32G32B32A32_FLOAT,
            [avxFormat_RGBA32u] = DXGI_FORMAT_R32G32B32A32_UINT,
            [avxFormat_RGBA32i] = DXGI_FORMAT_R32G32B32A32_SINT,
            [avxFormat_RGB32f] = DXGI_FORMAT_R32G32B32_FLOAT,
            [avxFormat_RGB32u] = DXGI_FORMAT_R32G32B32_UINT,
            [avxFormat_RGB32i] = DXGI_FORMAT_R32G32B32_SINT,
            [avxFormat_RGBA16f] = DXGI_FORMAT_R16G16B16A16_FLOAT,
            [avxFormat_RGBA16un] = DXGI_FORMAT_R16G16B16A16_UNORM,
            [avxFormat_RGBA16u] = DXGI_FORMAT_R16G16B16A16_UINT,
            [avxFormat_RGBA16sn] = DXGI_FORMAT_R16G16B16A16_SNORM,
            [avxFormat_RGBA16i] = DXGI_FORMAT_R16G16B16A16_SINT,
            [avxFormat_RG32f] = DXGI_FORMAT_R32G32_FLOAT,
            [avxFormat_RG32u] = DXGI_FORMAT_R32G32_UINT,
            [avxFormat_RG32i] = DXGI_FORMAT_R32G32_SINT,
            [avxFormat_D32fS8u] = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
            [avxFormat_RGBA8un] = DXGI_FORMAT_R8G8B8A8_UNORM,
            [avxFormat_RGBA8v] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
            [avxFormat_RGBA8u] = DXGI_FORMAT_R8G8B8A8_UINT,
            [avxFormat_RGBA8sn] = DXGI_FORMAT_R8G8B8A8_SNORM,
            [avxFormat_RGBA8i] = DXGI_FORMAT_R8G8B8A8_SINT,
            [avxFormat_RG16f] = DXGI_FORMAT_R16G16_FLOAT,
            [avxFormat_RG16un] = DXGI_FORMAT_R16G16_UNORM,
            [avxFormat_RG16u] = DXGI_FORMAT_R16G16_UINT,
            [avxFormat_RG16sn] = DXGI_FORMAT_R16G16_SNORM,
            [avxFormat_RG16i] = DXGI_FORMAT_R16G16_SINT,
            [avxFormat_D32f] = DXGI_FORMAT_D32_FLOAT,
            [avxFormat_R32f] = DXGI_FORMAT_R32_FLOAT,
            [avxFormat_R32u] = DXGI_FORMAT_R32_UINT,
            [avxFormat_R32i] = DXGI_FORMAT_R32_SINT,
            [avxFormat_D24unS8u] = DXGI_FORMAT_D24_UNORM_S8_UINT,
            [avxFormat_RG8un] = DXGI_FORMAT_R8G8_UNORM,
            [avxFormat_RG8u] = DXGI_FORMAT_R8G8_UINT,
            [avxFormat_RG8sn] = DXGI_FORMAT_R8G8_SNORM,
            [avxFormat_RG8i] = DXGI_FORMAT_R8G8_SINT,
            [avxFormat_R16f] = DXGI_FORMAT_R16_FLOAT,
            [avxFormat_D16un] = DXGI_FORMAT_D16_UNORM,
            [avxFormat_R16un] = DXGI_FORMAT_R16_UNORM,
            [avxFormat_R16u] = DXGI_FORMAT_R16_UINT,
            [avxFormat_R16sn] = DXGI_FORMAT_R16_SNORM,
            [avxFormat_R16i] = DXGI_FORMAT_R16_SINT,
            [avxFormat_R8un] = DXGI_FORMAT_R8_UNORM,
            [avxFormat_R8u] = DXGI_FORMAT_R8_UINT,
            [avxFormat_R8sn] = DXGI_FORMAT_R8_SNORM,
            [avxFormat_R8i] = DXGI_FORMAT_R8_SINT,
            [avxFormat_E5BGR9uf] = DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
            [avxFormat_DXT1un] = DXGI_FORMAT_BC1_UNORM,
            [avxFormat_DXT1v] = DXGI_FORMAT_BC1_UNORM_SRGB,
            [avxFormat_DXT3un] = DXGI_FORMAT_BC2_UNORM,
            [avxFormat_DXT3v] = DXGI_FORMAT_BC2_UNORM_SRGB,
            [avxFormat_DXT5un] = DXGI_FORMAT_BC3_UNORM,
            [avxFormat_DXT5v] = DXGI_FORMAT_BC3_UNORM_SRGB,
            [avxFormat_RGTC1un] = DXGI_FORMAT_BC4_UNORM,
            [avxFormat_RGTC1sn] = DXGI_FORMAT_BC4_SNORM,
            [avxFormat_RGTC2un] = DXGI_FORMAT_BC5_UNORM,
            [avxFormat_RGTC2sn] = DXGI_FORMAT_BC5_SNORM,
            [avxFormat_B5G6R5un] = DXGI_FORMAT_B5G6R5_UNORM,
            [avxFormat_BGR5A1un] = DXGI_FORMAT_B5G5R5A1_UNORM,
            [avxFormat_BGRA8un] = DXGI_FORMAT_B8G8R8A8_UNORM,
            [avxFormat_BGR8un] = DXGI_FORMAT_B8G8R8X8_UNORM,
            [avxFormat_BGRA8v] = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
            [avxFormat_BGR8v] = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
            [avxFormat_BPTCuf] = DXGI_FORMAT_BC6H_UF16,
            [avxFormat_BPTCf] = DXGI_FORMAT_BC6H_SF16,
            [avxFormat_BPTCun] = DXGI_FORMAT_BC7_UNORM,
            [avxFormat_BPTCv] = DXGI_FORMAT_BC7_UNORM_SRGB,
            [avxFormat_BGRA4un] = DXGI_FORMAT_B4G4R4A4_UNORM,
        };

        DXGI_FORMAT dxgiFmt = avxToDxgiFmtMap[fmt];

        HRESULT hr;
        UINT numModes;
        // Get number of supported display modes for current format
        hr = IDXGIOutput_GetDisplayModeList(vdu->pDxgiOutput, dxgiFmt, 0, &numModes, NULL);
        if (SUCCEEDED(hr) && numModes > 0)
        {
            DXGI_MODE_DESC* pModes = (DXGI_MODE_DESC*)malloc(sizeof(DXGI_MODE_DESC) * numModes);
            if (pModes)
            {
                hr = IDXGIOutput_GetDisplayModeList(vdu->pDxgiOutput, dxgiFmt, 0, &numModes, pModes);
                if (SUCCEEDED(hr))
                {
                    for (UINT i = 0; i < numModes; ++i)
                    {
                        if (pModes[i].ScanlineOrdering == DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED &&
                            pModes[i].Scaling == DXGI_MODE_SCALING_UNSPECIFIED)
                        {
                            //break;
                        }

                        modes[i] = (afxDisplayMode) { 0 };
                        modes[i].fmt = fmt;
                        modes[i].freq = pModes[i].RefreshRate.Numerator;
                        modes[i].stereo = FALSE;
                        modes[i].width = pModes[i].Width;
                        modes[i].height = pModes[i].Height;
                    }
                }
            }
        }
        rslt = numModes;
    }
#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY

    if (dpy->type == qowDisplayType_GDI)
    {
        avxFormatDescription pfd;
        AvxDescribeFormats(1, &fmt, &pfd);
        AFX_ASSERT(pfd.rasCaps);

        HDC hScreenDc = GetDC(NIL);

        afxUnit k = 0;
        DEVMODEA ddm = { 0 };
        ddm.dmSize = sizeof(ddm);
        ddm.dmSpecVersion = DM_SPECVERSION;
        // passing monitor's dd2.DeviceName always returned nothing
        if (EnumDisplaySettingsExA(dpy->ddinfo.DeviceName, ENUM_REGISTRY_SETTINGS + k, &ddm, NIL))
        {
            int pxlAttrPairs[][2] =
            {
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },

                { WGL_COLOR_BITS_ARB, pfd.bpp - pfd.bpc[3] },
                { WGL_RED_BITS_ARB, pfd.bpc[0] },
                { WGL_GREEN_BITS_ARB, pfd.bpc[1] },
                { WGL_BLUE_BITS_ARB, pfd.bpc[2] },
                { WGL_ALPHA_BITS_ARB, pfd.bpc[3] },
                { WGL_ALPHA_SHIFT_ARB, pfd.swizzle[3] },
                { WGL_RED_SHIFT_ARB, pfd.swizzle[0] },
                { WGL_GREEN_SHIFT_ARB, pfd.swizzle[1] },
                { WGL_BLUE_SHIFT_ARB, pfd.swizzle[2] },
                { NIL, NIL },
            };

            int gdiPfi[256];
            unsigned int gdiPfdCnt = 0;
            if (wglChoosePixelFormatARB(hScreenDc, &pxlAttrPairs[0][0], NIL, 256, &gdiPfi, &gdiPfdCnt))
            {
                for (afxUnit j = 0; j < gdiPfdCnt; j++)
                {
                    afxDisplayMode* mode = &modes[j];

                    mode->fmt = avxFormat_BGRA8un;
                    //mode->bpp = ddm.dmBitsPerPel;
                    mode->freq = ddm.dmDisplayFrequency;
                    mode->width = ddm.dmPelsWidth;
                    mode->height = ddm.dmPelsHeight;
                    mode->scaling = NIL;

                    if (ddm.dmDisplayFixedOutput == DMDFO_DEFAULT)
                        mode->scaling = avxVideoScaling_ONE_TO_ONE;
                    else if (ddm.dmDisplayFixedOutput == DMDFO_CENTER)
                        mode->scaling = avxVideoScaling_ASPECT_RATIO;
                    else if (ddm.dmDisplayFixedOutput == DMDFO_STRETCH)
                        mode->scaling = avxVideoScaling_STRETCH;

                    if (!(ddm.dmDisplayFlags & DM_INTERLACED))
                        mode->scanlineOrdering = avxScanlining_PROGRESSIVE;

                    ++rslt;
                }
            }
        }

        ReleaseDC(NIL, hScreenDc);
    }
#endif//USE_GDI_DISPLAY

    return rslt;
}

_QOW afxError _QowDpyAskGammaControlCb(afxDisplay dpy, afxUnit port, afxGammaCapabilites* caps)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxDisplayPort vdu = AfxGetDisplayPort(dpy, port);
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

#if USE_DXGI_DISPLAY
    if (dpy->type == qowDisplayType_DXGI)
    {
        HRESULT hr;
        DXGI_GAMMA_CONTROL_CAPABILITIES dxgiCaps;
        hr = vdu->pDxgiOutput->lpVtbl->GetGammaControlCapabilities(vdu->pDxgiOutput, &dxgiCaps);

        caps->minConvertedValue = dxgiCaps.MinConvertedValue;
        caps->maxConvertedValue = dxgiCaps.MaxConvertedValue;
        caps->scaleAndOffsetSupported = dxgiCaps.ScaleAndOffsetSupported;
        caps->ctrlPntCnt = dxgiCaps.NumGammaControlPoints;
        for (afxUnit i = 0; i < caps->ctrlPntCnt; i++)
            caps->ctrlPntPos[i] = dxgiCaps.ControlPointPositions[i];
        
        return err;
    }
#endif//USE_DXGI_DISPLAY
    err = afxError_UNSUPPORTED;

    return err;
}

_QOW afxError _QowDpyGetGammaControlCb(afxDisplay dpy, afxUnit port, afxGammaCurve* desc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxDisplayPort vdu = AfxGetDisplayPort(dpy, port);
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

#if USE_DXGI_DISPLAY
    if (dpy->type == qowDisplayType_DXGI)
    {
        HRESULT hr;
        DXGI_GAMMA_CONTROL dxgiGamma;
        hr = vdu->pDxgiOutput->lpVtbl->GetGammaControl(vdu->pDxgiOutput, &dxgiGamma);

        AfxV3dSet(desc->offset, dxgiGamma.Offset.Red, dxgiGamma.Offset.Green, dxgiGamma.Offset.Blue);
        AfxV3dSet(desc->scale, dxgiGamma.Scale.Red, dxgiGamma.Scale.Green, dxgiGamma.Scale.Blue);

        for (afxUnit i = 0; i < 1025; i++)
            AfxV3dSet(desc->curve[i], dxgiGamma.GammaCurve[i].Red, dxgiGamma.GammaCurve[i].Green, dxgiGamma.GammaCurve[i].Blue);

        return err;
    }
#endif//USE_DXGI_DISPLAY
    err = afxError_UNSUPPORTED;

    return err;
}

_QOW afxError _QowDpySetGammaControlCb(afxDisplay dpy, afxUnit port, afxGammaCurve const* ctrl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxDisplayPort vdu = AfxGetDisplayPort(dpy, port);
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

#if USE_DXGI_DISPLAY
    if (dpy->type == qowDisplayType_DXGI)
    {
        DXGI_GAMMA_CONTROL dxgiGamma;
        dxgiGamma.Offset.Red = ctrl->offset[0];
        dxgiGamma.Offset.Green = ctrl->offset[1];
        dxgiGamma.Offset.Blue = ctrl->offset[2];
        dxgiGamma.Scale.Red = ctrl->scale[0];
        dxgiGamma.Scale.Green = ctrl->scale[1];
        dxgiGamma.Scale.Blue = ctrl->scale[2];

        for (afxUnit i = 0; i < 1025; i++)
        {
            dxgiGamma.GammaCurve[i].Red = ctrl->curve[i][0];
            dxgiGamma.GammaCurve[i].Green = ctrl->curve[i][1];
            dxgiGamma.GammaCurve[i].Blue = ctrl->curve[i][2];
        }

        HRESULT hr;
        hr = vdu->pDxgiOutput->lpVtbl->SetGammaControl(vdu->pDxgiOutput, &dxgiGamma);

        return err;
    }
#endif//USE_DXGI_DISPLAY
    err = afxError_UNSUPPORTED;

    return err;
}

_QOW afxError _QowDpyCaptureCb(afxDisplay dpy, afxUnit port, afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxDisplayPort vdu = AfxGetDisplayPort(dpy, port);
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

#if USE_DXGI_DISPLAY
    if (dpy->type == qowDisplayType_DXGI)
    {
        //vdu->pDxgiOutput->lpVtbl->GetDisplaySurfaceData(vdu->pDxgiOutput, );
    }
#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY
    if (dpy->type == qowDisplayType_GDI)
    {
        HDC hDc = CreateDCA("DISPLAY", dpy->ddinfo.DeviceName, 0, NIL);
        BitBlt(dout->hDC, dout->m.dstArea.x, dout->m.dstArea.y, dout->m.dstArea.w, dout->m.dstArea.h, hDc, 0, 0, SRCCOPY);
        DeleteDC(hDc);
    }
#endif//USE_GDI_DISPLAY

    return err;
}

_QOW _auxDdiDisp _QOW_DDI_DPY =
{
    .qryModeCb = _QowDpyQueryModesCb,
    .askGammaCtrlCb = _QowDpyAskGammaControlCb,
    .getGammaCtrlCb = _QowDpyGetGammaControlCb,
    .setGammaCtrlCb = _QowDpySetGammaControlCb,
    .captureCb = _QowDpyCaptureCb,
};

_QOW afxError _QowDpyDtorCb(afxDisplay dpy)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    AfxExhaustChainedClasses(&dpy->m.dev.classes);

#if USE_DXGI_DISPLAY

    if (dpy->type == qowDisplayType_DXGI)
    {
        IDXGIAdapter_Release(dpy->pDxgiAdapter);
        IDXGIFactory_Release(dpy->pDxgiFactory);
    }

#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY

    if (dpy->type == qowDisplayType_GDI)
    {

    }

#endif

    if (_AUX_DPY_CLASS_CONFIG.dtor(dpy))
        AfxThrowError();

    return err;
}

_QOW afxError _QowDpyCtorCb(afxDisplay dpy, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxDisplayConfig const* cfg = (afxDisplayConfig const *)(args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    afxClassConfig portClsCfg = _AUX_VDU_CLASS_CONFIG;
    portClsCfg.fixedSiz = sizeof(AFX_OBJECT(afxDisplayPort));
    portClsCfg.ctor = (void*)_QowVduCtorCb;
    portClsCfg.dtor = (void*)_QowVduDtorCb;

    if (_AUX_DPY_CLASS_CONFIG.ctor(dpy, (void*[]) { icd, (void*)cfg, /*args[2]*/&portClsCfg }, 0))
    {
        AfxThrowError();
        return err;
    }

    if (0 == AfxCompareString(&dpy->m.tag, 0, "dxgi", 0, FALSE))
    {
        dpy->type = qowDisplayType_DXGI;
    }
    else if(0 == AfxCompareString(&dpy->m.tag, 0, "gdi", 0, FALSE))
    {
        dpy->type = qowDisplayType_GDI;
    }
    else
    {
        AfxThrowError();
    }

#if USE_DXGI_DISPLAY

    if (dpy->type == qowDisplayType_DXGI)
    {
        IDXGIFactory* pFactory = NULL;
        HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory);
        if (FAILED(hr))
        {
            printf("Failed to create DXGIFactory: 0x%08x\n", hr);
            return -1;
        }
        else
        {
            IDXGIAdapter* pAdapter = NULL;
            UINT adapterIndex = AfxGetObjectId(dpy);
            if (S_OK != (IDXGIFactory_EnumAdapters(pFactory, adapterIndex, &pAdapter)))
            {
                AfxThrowError();
            }
            else
            {
                printf("Adapter %u:\n", adapterIndex);

                dpy->pDxgiFactory = pFactory;
                dpy->pDxgiAdapter = pAdapter;
                dpy->dxgiAdapterIndex = adapterIndex;

                UINT outputIndex = 0;
                IDXGIOutput* pOutput = NULL;
                while (SUCCEEDED(IDXGIAdapter_EnumOutputs(pAdapter, outputIndex, &pOutput)))
                {
                    DXGI_OUTPUT_DESC desc;
                    IDXGIOutput_GetDesc(pOutput, &desc);

                    MONITORINFOEXA moni = { 0 };
                    moni.cbSize = sizeof(moni);
                    GetMonitorInfoA(desc.Monitor, &moni);

                    afxUnit portIdx = outputIndex;

                    afxString s = { 0 };
                    afxString32 monName;
                    afxString128 monLabel;
                    AfxMakeString32(&monName, &s);
                    AfxMakeString128(&monLabel, &s);
                    AfxLoadStringAsUtf16(&monName.s, desc.DeviceName, 0);
                    AfxLoadStringAsUtf16(&monLabel.s, desc.DeviceName, 0);

                    afxDisplayPortConfig portCfg = { 0 };
                    portCfg.portId = portIdx;
                    portCfg.fullArea = AFX_RECT(moni.rcMonitor.left, moni.rcMonitor.top, moni.rcMonitor.bottom, moni.rcMonitor.right);
                    portCfg.workArea = AFX_RECT(moni.rcWork.left, moni.rcWork.top, moni.rcWork.bottom, moni.rcWork.right);
                    portCfg.name = monName.s;
                    portCfg.label = monLabel.s;

                    afxDisplayPort vdu;
                    if (_AuxRegisterDisplayPorts(dpy, 1, &portCfg, &vdu))
                    {
                        AfxThrowError();
                    }
                    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

                    //IDXGIOutput_Release(pOutput);
                    outputIndex++;
                }

                //IDXGIAdapter_Release(pAdapter);
                //adapterIndex++;

                if (err)
                {
                    AfxExhaustClass(&dpy->m.portCls);
                }
            }

            if (err)
            {
                IDXGIFactory_Release(pFactory);
            }
        }
    }

#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY
    if (dpy->type == qowDisplayType_GDI)
    {
        dpy->ddinfo = *(DISPLAY_DEVICEA*)cfg->udd;

        //HDC hScreenDc = GetDC(NULL);
        //EnumDisplayMonitors(hScreenDc, NULL, MonitorEnumProc2, (LPARAM)ddev);
        //ReleaseDC(NULL, hScreenDc);

        CHAR DeviceName[32];
        AfxStrcpy(DeviceName, dpy->m.name.buf);

        afxUnit monIdx = 0;
        DISPLAY_DEVICEA dd2 = { 0 };
        dd2.cb = sizeof(dd2);
        while (EnumDisplayDevicesA(DeviceName, monIdx, &dd2, NIL))
        {
            //AfxReportComment("    #%u = %s (%s) 0x%x; %s (%s)", monIdx, dd2.DeviceString, dd2.DeviceName, dd2.StateFlags, dd2.DeviceID, dd2.DeviceKey);

            //ddm.dmSpecVersion = DM_SPECVERSION;
            // passing dd2.DeviceName always return nothing
            /*
                If you get something like \\.\DISPLAY2 then you should be fine, but if you get something
                like \\.\DISPLAY2\Monitor0 then you can't use it directly with EnumDisplaySettings because it expects the former.
            */

            afxDisplayPort vdu = NIL;

            afxUnit dmIdx = 0;
            DEVMODEA ddm = { 0 };
            ddm.dmSize = sizeof(ddm);
            while (EnumDisplaySettingsExA(DeviceName, ENUM_REGISTRY_SETTINGS + dmIdx, &ddm, NIL))
            {
                if (dmIdx == 0) // register the ENUM_REGISTRY_SETTINGS version only.
                {
                    //AfxReportComment("    %ux%u:%u@%u --- %u, (%i, %i)%u, %u, 0x%x", ddm.dmPelsWidth, ddm.dmPelsHeight, ddm.dmBitsPerPel, ddm.dmDisplayFrequency, ddm.dmLogPixels, ddm.dmPosition.x, ddm.dmPosition.y, ddm.dmDisplayOrientation, ddm.dmDisplayFixedOutput, ddm.dmDisplayFlags);

                    afxDisplayPortConfig vdui = { 0 };
                    vdui.dimWh[0] = ddm.dmPelsWidth;
                    vdui.dimWh[1] = ddm.dmPelsHeight;
                    vdui.resWh[0] = ddm.dmPelsWidth;
                    vdui.resWh[1] = ddm.dmPelsHeight;

                    AfxMakeString(&vdui.name, 0, dd2.DeviceName, 0);
                    AfxMakeString(&vdui.label, 0, dd2.DeviceString, 0);

                    if (_AuxRegisterDisplayPorts(dpy, 1, &vdui, &vdu))
                    {
                        AfxThrowError();
                    }
                    //dpy->ddinfo = dd;
                    //dpy->ddminfo = dd2;

                    afxDisplayMode modes[256];
                    afxUnit dmCnt = 0;// _ZglEnumerateDisplayModes(icd, monIdx, avxFormat_BGRA8un, 256, modes);

                    for (afxUnit i = 0; i < dmCnt; i++)
                    {
                        AfxReportMessage("fmt %d %dx%d@%d %x %x %i", modes[i].fmt, modes[i].width, modes[i].height, modes[i].freq, modes[i].scaling, modes[i].scanlineOrdering, modes[i].stereo);
                    }

                    avxFormat formats[256];
                    afxUnit fmtCnt = 0;// _ZglEnumerateDisplayFormats(icd, monIdx, 256, formats);

                    for (afxUnit i = 0; i < fmtCnt; i++)
                    {
                        avxFormatDescription fd;
                        AvxDescribeFormats(1, &formats[i], &fd);
                        AfxReportMessage("fmt %.*s", AfxPushString(&fd.tag));
                    }
                    break;
                }
                dmIdx++;
            }
            monIdx++;
        }
    }
#endif//USE_GDI_DISPLAY

    if (err && _AUX_DPY_CLASS_CONFIG.dtor(dpy))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////


int ListDisplayPortsDxgi() {
    IDXGIFactory* pFactory = NULL;
    HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory);
    if (FAILED(hr)) {
        printf("Failed to create DXGIFactory: 0x%08x\n", hr);
        return -1;
    }

    IDXGIAdapter* pAdapter = NULL;
    UINT adapterIndex = 0;

    while (SUCCEEDED(IDXGIFactory_EnumAdapters(pFactory, adapterIndex, &pAdapter))) {
        printf("Adapter %u:\n", adapterIndex);

        IDXGIOutput* pOutput = NULL;
        UINT outputIndex = 0;

        while (SUCCEEDED(IDXGIAdapter_EnumOutputs(pAdapter, outputIndex, &pOutput))) {
            DXGI_OUTPUT_DESC desc;
            IDXGIOutput_GetDesc(pOutput, &desc);

            DXGI_MODE_DESC currentMode;
            UINT numModes = 0;

            // Get number of supported display modes for current format
            hr = IDXGIOutput_GetDisplayModeList(pOutput, DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, NULL);
            if (SUCCEEDED(hr) && numModes > 0) {
                DXGI_MODE_DESC* pModes = (DXGI_MODE_DESC*)malloc(sizeof(DXGI_MODE_DESC) * numModes);
                if (pModes) {
                    hr = IDXGIOutput_GetDisplayModeList(pOutput, DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, pModes);
                    if (SUCCEEDED(hr)) {
                        // Find the current display mode
                        for (UINT i = 0; i < numModes; ++i) {
                            if (pModes[i].ScanlineOrdering == DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED &&
                                pModes[i].Scaling == DXGI_MODE_SCALING_UNSPECIFIED) {
                                currentMode = pModes[i];
                                //break;
                            }
                            currentMode = pModes[i];


                            UINT refreshRate = 0;
                            if (currentMode.RefreshRate.Denominator != 0) {
                                refreshRate = currentMode.RefreshRate.Numerator / currentMode.RefreshRate.Denominator;
                            }

                            wprintf(L"  Monitor %u: %s\n", outputIndex, desc.DeviceName);
                            printf("    Resolution: %ux%u\n", currentMode.Width, currentMode.Height);
                            printf("    Refresh Rate: %u Hz\n", refreshRate);
                        }
                    }
                    free(pModes);
                }
            }

            IDXGIOutput_Release(pOutput);
            outputIndex++;
        }

        IDXGIAdapter_Release(pAdapter);
        adapterIndex++;
    }

    IDXGIFactory_Release(pFactory);
    return 0;
}

/*
    EnumDisplayDevicesA()

    To query all display devices in the current session, call this function in a loop, starting with iDevNum set to 0,
    and incrementing iDevNum until the function fails. To select all display devices in the desktop, use only the
    display devices that have the DISPLAY_DEVICE_ATTACHED_TO_DESKTOP flag in the DISPLAY_DEVICE structure.

    To get information on the display adapter, call EnumDisplayDevices with lpDevice set to NULL.
    For example, DISPLAY_DEVICE.DeviceString contains the adapter name.

    To obtain information on a display monitor, first call EnumDisplayDevices with lpDevice set to NULL.
    Then call EnumDisplayDevices with lpDevice set to DISPLAY_DEVICE.DeviceName from the first call to
    EnumDisplayDevices and with iDevNum set to zero. Then DISPLAY_DEVICE.DeviceString is the monitor name.

    To query all monitor devices associated with an adapter, call EnumDisplayDevices in a loop with lpDevice set
    to the adapter name, iDevNum set to start at 0, and iDevNum set to increment until the function fails.
    Note that DISPLAY_DEVICE.DeviceName changes with each call for monitor information, so you must save the adapter name.
    The function fails when there are no more monitors for the adapter.

*/

BOOL GetMonitorNameFromHMONITOR(HMONITOR hMonitor, char* outName, size_t outNameSize);

BOOL GetMonitorNameFromHMONITOR(HMONITOR hMonitor, char* outName, size_t outNameSize)
{
    if (!hMonitor || !outName || outNameSize == 0)
        return FALSE;

    MONITORINFOEXA targetInfo = { 0 };
    targetInfo.cbSize = sizeof(targetInfo);

    if (!GetMonitorInfoA(hMonitor, (MONITORINFO*)&targetInfo))
        return FALSE;

    RECT targetRect = targetInfo.rcMonitor;

    DISPLAY_DEVICEA adapter = { 0 };
    adapter.cb = sizeof(adapter);

    // Loop over all display devices (video adapters)
    for (DWORD adapterIdx = 0; EnumDisplayDevicesA(NULL, adapterIdx, &adapter, 0); adapterIdx++) {
        DISPLAY_DEVICEA monitor = { 0 };
        monitor.cb = sizeof(monitor);

        // Loop over monitors attached to this adapter
        for (DWORD monitorIdx = 0; EnumDisplayDevicesA(adapter.DeviceName, monitorIdx, &monitor, 0); monitorIdx++) {
            DEVMODEA devMode = { 0 };
            devMode.dmSize = sizeof(devMode);

            if (!EnumDisplaySettingsExA(monitor.DeviceName, ENUM_CURRENT_SETTINGS, &devMode, 0))
                continue;

            RECT monitorRect = {
                devMode.dmPosition.x,
                devMode.dmPosition.y,
                devMode.dmPosition.x + (LONG)devMode.dmPelsWidth,
                devMode.dmPosition.y + (LONG)devMode.dmPelsHeight
            };

            // Compare this monitor's rect with the HMONITOR's rect
            if (EqualRect(&targetRect, &monitorRect)) {
                // Copy friendly name
                strncpy_s(outName, outNameSize, monitor.DeviceString, _TRUNCATE);
                return TRUE;
            }
        }
    }

    return FALSE;
}

_QOW afxUnit _ZglChooseDisplayModes(afxDrawDevice ddev, afxUnit port, afxDisplayMode const* expected, afxDisplayMode* closest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    afxUnit rslt = 0;

    DEVMODEA ddm = { 0 };

    afxUnit dmIdx = 0;
    while (EnumDisplaySettingsExA(ddev->dd.DeviceName, ENUM_REGISTRY_SETTINGS + dmIdx, &ddm, NIL))
    {
        ++dmIdx;
    }

    return rslt;
}

_QOW afxUnit _ZglEnumerateDisplayFormats(afxModule icd, afxUnit port, afxUnit cnt, avxFormat formats[])
{
    afxError err = { 0 };
    AFX_ASSERT(formats);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxDisplay dpy;
    if (!AfxEnumerateDisplays(AfxGetObjectId(icd), port, 1, &dpy))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

#if USE_SCREEN_DC
    HDC hDc = GetDC(NIL);
#else
    HDC hDc = CreateDCA("DISPLAY", dpy->ddinfo.DeviceName, 0, NIL);
#endif

    if (!hDc)
    {
        AfxThrowError();
        return err;
    }

#if 0
    for (afxUnit i = 0; i < avxFormat_TOTAL; i++)
    {
        avxFormat fmt = i;

        avxFormatDescription pfd;
        AvxDescribeFormat(fmt, &pfd);

        // Skip non-color drawable formats.
        if ((pfd.flags & (avxFormatFlag_DEPTH | avxFormatFlag_STENCIL | avxFormatFlag_BC | avxFormatFlag_LUMA)))
            continue;

        if ((pfd.flags & avxFormatFlag_COLOR) != avxFormatFlag_COLOR)
            continue;

        int pxlAttrPairs[30][2] =
        {
            { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
            { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
            { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
            { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
            { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },

            { WGL_COLOR_BITS_ARB, pfd.bpp - pfd.bpc[3] },
            { WGL_RED_BITS_ARB, pfd.bpc[0] },
            { WGL_GREEN_BITS_ARB, pfd.bpc[1] },
            { WGL_BLUE_BITS_ARB, pfd.bpc[2] },
            { WGL_ALPHA_BITS_ARB, pfd.bpc[3] },
            { WGL_ALPHA_SHIFT_ARB, pfd.swizzle[3] },
            { WGL_RED_SHIFT_ARB, pfd.swizzle[0] },
            { WGL_GREEN_SHIFT_ARB, pfd.swizzle[1] },
            { WGL_BLUE_SHIFT_ARB, pfd.swizzle[2] },
            { WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // WGL_COLORSPACE_LINEAR_EXT
            { NIL, NIL },
        };




        if ((pfd.flags & avxFormatFlag_sRGB) == avxFormatFlag_sRGB)
        {
            pxlAttrPairs[14][0] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
            pxlAttrPairs[14][1] = GL_TRUE;

            //pxlAttrPairs[14][0] = WGL_COLORSPACE_EXT;
            //pxlAttrPairs[14][1] = WGL_COLORSPACE_SRGB_EXT; // WGL_COLORSPACE_LINEAR_EXT
        }
        else
        {
            pxlAttrPairs[14][0] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
            pxlAttrPairs[14][1] = GL_FALSE;

            //pxlAttrPairs[14][0] = WGL_COLORSPACE_EXT;
            //pxlAttrPairs[14][1] = WGL_COLORSPACE_SRGB_EXT;
        }

        int pxlFmt = 0;
        afxUnit gdiPfdCnt = 0;
        if (wgl.ChoosePixelFormatARB(hDc, &pxlAttrPairs[0][0], NIL, 1, &pxlFmt, &gdiPfdCnt))
            //if (wglChooseBestPixelFormatSIG(hDc, &pxlAttrPairs[0][0], NIL, 1, &pxlFmt, &gdiPfdCnt))
        {
            PIXELFORMATDESCRIPTOR dcPfd = { 0 };
            dcPfd.nSize = sizeof(dcPfd);
            dcPfd.nVersion = 1;
            wgl.DescribePixelFormat(hDc, pxlFmt, sizeof(dcPfd), &dcPfd);

            if ((dcPfd.cBlueBits != pfd.bpc[2]) ||
                (dcPfd.cGreenBits != pfd.bpc[1]) ||
                (dcPfd.cRedBits != pfd.bpc[0]) ||
                (dcPfd.cAlphaBits != pfd.bpc[3]))
            {
                continue;
            }

            formats[rslt] = fmt;
            ++rslt;
        }
    }
#else

    DEVMODEA ddm = { 0 };
    ddm.dmSize = sizeof(ddm);
    if (!EnumDisplaySettingsExA(dpy->ddinfo.DeviceName, ENUM_REGISTRY_SETTINGS, &ddm, NIL))
    {
        AfxThrowError();
        return rslt;
    }

    afxUnit bpp = ddm.dmBitsPerPel;
    avxFormatDescription pfd = { 0 };
    pfd.flags |= avxFormatFlag_COLOR;
    pfd.rasCaps |= avxFormatUsage_BLIT | avxFormatUsage_DRAW;
    pfd.bpp = bpp;
    pfd.stride = bpp / AFX_BYTE_SIZE;
    pfd.compCnt = 3;
    //pfd.bpc[0] = bpp / pfd.compCnt;
    //pfd.bpc[1] = bpp / pfd.compCnt;
    //pfd.bpc[2] = bpp / pfd.compCnt;
    //pfd.bpc[3] = bpp / pfd.compCnt;
    pfd.swizzle[0] = avxColorSwizzle_B;
    pfd.swizzle[1] = avxColorSwizzle_G;
    pfd.swizzle[2] = avxColorSwizzle_R;
    pfd.swizzle[3] = avxColorSwizzle_A;
    afxUnit fmtCnt = AvxChooseFormats(&pfd, cnt, formats);

    //pfd.compCnt = 3;
    //fmtCnt += AvxChooseFormats(&pfd, cnt - fmtCnt, &formats[fmtCnt]);
    fmtCnt += 0; // break point

    for (afxUnit fmtIdx = 0; fmtIdx < fmtCnt; fmtIdx++)
    {
        pfd = (avxFormatDescription) { 0 };
        AvxDescribeFormats(1, &formats[fmtIdx], &pfd);

        int pxlAttrPairs[30][2] =
        {
            { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
            { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
            { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
            { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
            { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },

            { WGL_COLOR_BITS_ARB, pfd.bpp - pfd.bpc[3] },
            { WGL_RED_BITS_ARB, pfd.bpc[0] },
            { WGL_GREEN_BITS_ARB, pfd.bpc[1] },
            { WGL_BLUE_BITS_ARB, pfd.bpc[2] },
            { WGL_ALPHA_BITS_ARB, pfd.bpc[3] },
            { WGL_ALPHA_SHIFT_ARB, pfd.swizzle[3] },
            { WGL_RED_SHIFT_ARB, pfd.swizzle[0] },
            { WGL_GREEN_SHIFT_ARB, pfd.swizzle[1] },
            { WGL_BLUE_SHIFT_ARB, pfd.swizzle[2] },
            { WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // WGL_COLORSPACE_LINEAR_EXT
            { NIL, NIL },
        };

        int pxlFmt = 0;
        unsigned int gdiPfdCnt = 0;

        afxBool byWgl;
        if (wglChooseBestPixelFormatSIG(hDc, &pxlAttrPairs[0][0], NIL, 1, &pxlFmt, &gdiPfdCnt, &byWgl))
        {
            PIXELFORMATDESCRIPTOR dcPfd = { 0 };
            dcPfd.nSize = sizeof(dcPfd);
            dcPfd.nVersion = 1;
#if USE_SCREEN_DC
            wgl.DescribePixelFormat(hDc, pxlFmt, sizeof(dcPfd), &dcPfd);
#else
            DescribePixelFormat(hDc, pxlFmt, sizeof(dcPfd), &dcPfd);
#endif

            if ((dcPfd.cBlueBits != pfd.bpc[2]) ||
                (dcPfd.cGreenBits != pfd.bpc[1]) ||
                (dcPfd.cRedBits != pfd.bpc[0]) ||
                (dcPfd.cAlphaBits != pfd.bpc[3]))
            {
                afxUnit i = fmtCnt;

                if (fmtCnt > 1) while (i)
                    formats[i - 1] = formats[i], i--;

                --fmtCnt;
            }

            ++rslt;
        }
    }
#endif

#if USE_SCREEN_DC
    ReleaseDC(NIL, hDc);
#else
    DeleteDC(hDc);
#endif

    return fmtCnt;
}

#if 0
_QOW afxUnit _ZglEnumerateDisplayModes(afxModule icd, afxUnit port, avxFormat fmt, afxUnit cnt, afxDisplayMode modes[])
{
    afxError err = { 0 };
    AFX_ASSERT(modes);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxDisplay dpy;
    if (!AfxEnumerateDisplays(AfxGetObjectId(icd), port, 1, &dpy))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);
    AFX_ASSERT(pfd.rasCaps);

    HDC hScreenDc = GetDC(NIL);

    afxUnit k = 0;
    DEVMODEA ddm = { 0 };
    ddm.dmSize = sizeof(ddm);
    ddm.dmSpecVersion = DM_SPECVERSION;
    // passing monitor's dd2.DeviceName always returned nothing
    if (EnumDisplaySettingsExA(dpy->ddinfo.DeviceName, ENUM_REGISTRY_SETTINGS + k, &ddm, NIL))
    {
        int pxlAttrPairs[][2] =
        {
            { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
            { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
            { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
            { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
            { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },

            { WGL_COLOR_BITS_ARB, pfd.bpp - pfd.bpc[3] },
            { WGL_RED_BITS_ARB, pfd.bpc[0] },
            { WGL_GREEN_BITS_ARB, pfd.bpc[1] },
            { WGL_BLUE_BITS_ARB, pfd.bpc[2] },
            { WGL_ALPHA_BITS_ARB, pfd.bpc[3] },
            { WGL_ALPHA_SHIFT_ARB, pfd.swizzle[3] },
            { WGL_RED_SHIFT_ARB, pfd.swizzle[0] },
            { WGL_GREEN_SHIFT_ARB, pfd.swizzle[1] },
            { WGL_BLUE_SHIFT_ARB, pfd.swizzle[2] },
            { NIL, NIL },
        };

        int gdiPfi[256];
        unsigned int gdiPfdCnt = 0;
        if (wglChoosePixelFormatARB(hScreenDc, &pxlAttrPairs[0][0], NIL, 256, &gdiPfi, &gdiPfdCnt))
        {
            for (afxUnit j = 0; j < gdiPfdCnt; j++)
            {
                afxDisplayMode* mode = &modes[j];

                mode->fmt = avxFormat_BGRA8un;
                //mode->bpp = ddm.dmBitsPerPel;
                mode->freq = ddm.dmDisplayFrequency;
                mode->width = ddm.dmPelsWidth;
                mode->height = ddm.dmPelsHeight;
                mode->scaling = NIL;

                if (ddm.dmDisplayFixedOutput == DMDFO_DEFAULT)
                    mode->scaling = avxVideoScaling_ONE_TO_ONE;
                else if (ddm.dmDisplayFixedOutput == DMDFO_CENTER)
                    mode->scaling = avxVideoScaling_ASPECT_RATIO;
                else if (ddm.dmDisplayFixedOutput == DMDFO_STRETCH)
                    mode->scaling = avxVideoScaling_STRETCH;

                if (!(ddm.dmDisplayFlags & DM_INTERLACED))
                    mode->scanlineOrdering = avxScanlining_PROGRESSIVE;

                ++rslt;
            }
        }
    }

    ReleaseDC(NIL, hScreenDc);

    return rslt;
}
#endif

afxError RegisterPresentVdus(afxModule icd)
{
    afxError err;
#if 0
    // enumerates all display monitors (i.e., screens) that intersect with a given device context (DC) or a clipping rectangle.
    // Optional: drawing context (NULL = entire desktop).
    // Optional: restricts enumeration.
    // Callback function for each monitor.
    // User-defined data for callback.
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)icd);
#endif

#if 0
    ListDisplayPortsDxgi();
#endif

#if USE_DXGI_DISPLAY

    IDXGIFactory* pFactory = NULL;
    HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory, (void**)&pFactory);
    if (FAILED(hr))
    {
        printf("Failed to create DXGIFactory: 0x%08x\n", hr);
        return -1;
    }

    UINT adapterIndex = 0;
    IDXGIAdapter* pAdapter = NULL;
    while (SUCCEEDED(IDXGIFactory_EnumAdapters(pFactory, adapterIndex, &pAdapter)))
    {
        //printf("Adapter %u:\n", adapterIndex);

        static afxString const staticDxgiTag = AFX_STRING("dxgi");

        afxDisplay dpy;
        afxDisplayConfig dpi = { 0 };
        dpi.dev.type = afxDeviceType_DISPLAY;
        dpi.tag = staticDxgiTag;
        
        afxString32 s;
        AfxMakeString32(&s, NIL);
        AfxFormatString(&s.s, "dxgi%u", adapterIndex);
        dpi.dev.urn = s.s;

        if (_AuxRegisterDisplays(icd, 1, &dpi, &dpy))
        {
            AfxThrowError();
        }

        IDXGIAdapter_Release(pAdapter);
        adapterIndex++;
    }

    IDXGIFactory_Release(pFactory);

#endif//USE_DXGI_DISPLAY

#if USE_GDI_DISPLAY
    afxUnit ddIdx = 0;
    DISPLAY_DEVICEA dd = { 0 };
    dd.cb = sizeof(dd);
    while (EnumDisplayDevicesA(NULL, ddIdx, &dd, NIL))
    {
        //AfxReportComment("#%u = %s (%s) 0x%x; %s (%s)", ddIdx, dd.DeviceString, dd.DeviceName, dd.StateFlags, dd.DeviceID, dd.DeviceKey);

#if 0
        {
            HDC hGfxDc = CreateDCA("DISPLAY", dd.DeviceName, NULL, NULL);
            int pxlAttrPairs[][2] =
            {
                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB },
                { WGL_COLOR_BITS_ARB, AFX_MIN(24, GetDeviceCaps(hGfxDc, BITSPIXEL)) },
                { WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
                { NIL, NIL },
            };

            DWORD pfdFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SWAP_EXCHANGE | PFD_DIRECT3D_ACCELERATED;
            UINT formatCount;
            PIXELFORMATDESCRIPTOR dcPfd = { 0 };
            dcPfd.nSize = sizeof(dcPfd);
            dcPfd.dwFlags = pfdFlags;
            dcPfd.cColorBits = AFX_MIN(24, GetDeviceCaps(hGfxDc, BITSPIXEL));
            int dcPxlFmt = 0;

            dcPxlFmt = ChoosePixelFormat(hGfxDc, &dcPfd);
            DescribePixelFormat(hGfxDc, dcPxlFmt, sizeof(dcPfd), &dcPfd);
            AFX_ASSERT(dcPfd.dwFlags & pfdFlags);
            dcPxlFmt = ChoosePixelFormat(hGfxDc, &dcPfd);

            if (dcPxlFmt == 0)
                continue;

            //ddev->dd = dd;
#if 0
            DescribePixelFormat(hGfxDc, dcPxlFmt, sizeof(dcPfd), &dcPfd);

            AfxZero(&dcPfd, sizeof(dcPfd));
            dcPxlFmt = 0;

            if (!wgl.ChoosePixelFormatARB(hGfxDc, &pxlAttrPairs[0][0], NIL, 1, &dcPxlFmt, &formatCount)) AfxThrowError();
            else
            {
                AFX_ASSERT(dcPxlFmt);
                AFX_ASSERT(formatCount);
                AfxZero(&dcPfd, sizeof(dcPfd));

                wglDescribePixelFormatSIG(hGfxDc, dcPxlFmt, sizeof(dcPfd), &dcPfd);
                wglSetPixelFormatSIG(hGfxDc, dcPxlFmt, NULL, &dcPfd);

                AFX_ASSERT(dcPfd.dwFlags & pfdFlags);

            }
#endif
            DeleteDC(hGfxDc);
        }
#endif

        afxBool attachedToDesk = (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
        afxBool active = (dd.StateFlags & DISPLAY_DEVICE_ACTIVE);

        if (active)
        {
            static afxString const staticGdiTag = AFX_STRING("gdi");

            afxDisplay dpy = NIL;
            afxDisplayConfig dpi = { 0 };
            dpi.dev.type = afxDeviceType_DISPLAY;
            afxString slabel, sname;
            AfxMakeString(&slabel, 0, dd.DeviceString, 0);
            AfxMakeString(&sname, 0, dd.DeviceName, 0);
            dpi.name = sname;
            dpi.label = slabel;
            dpi.tag = staticGdiTag;
            dpi.udd = &dd;

            afxString32 s;
            AfxMakeString32(&s, NIL);
            AfxFormatString(&s.s, "gdi%u", ddIdx);
            dpi.dev.urn = s.s;

            if (_AuxRegisterDisplays(icd, 1, &dpi, &dpy))
            {
                AfxThrowError();
            }

            dpy->ddinfo = dd;
        }
        ddIdx++;
    }
#endif//USE_GDI_DISPLAY
}
