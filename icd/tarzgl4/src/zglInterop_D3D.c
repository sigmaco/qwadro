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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <assert.h>

#define COBJMACROS
#define INITGUID
#include <intrin.h>
#include <windows.h>
#include <d3d11.h>
#include <d3d9.h>
#include <dxgi1_2.h>
#include <dxgi1_6.h>

#define _AFX_CORE_C
//#define _AFX_MODULE_C
//#define _AFX_ICD_C
#define _AVX_DRAW_C
#define _AFX_THREAD_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DRIVER_C
#define _AFX_MODULE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_DRIVER_C

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"
#include "qwadro/inc/afxQwadro.h"
//#pragma comment(lib, "opengl32")

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dxgi.lib")

#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)
#define AssertHR(hr) Assert(SUCCEEDED(hr))
#if 0
static GLuint colorRbuf;
static GLuint fbuf;

static HANDLE dxDevice;

static ID3D11Device* device;
static ID3D11DeviceContext* context;
static IDXGISwapChain1* swapChain;

static HWND temp;
static HDC tempdc;
static HGLRC temprc;

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
    if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
    {
        Assert(0);
    }
}

static void Create(HWND window)
{
    // GL context on temporary window, no drawing will happen to this window
    {
        temp = CreateWindowA("STATIC", "temp", WS_OVERLAPPED,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL, NULL);
        Assert(temp);

        tempdc = GetDC(temp);
        Assert(tempdc);

        PIXELFORMATDESCRIPTOR pfd =
        {
            .nSize = sizeof(pfd),
            .nVersion = 1,
            .dwFlags = PFD_SUPPORT_OPENGL,
            .iPixelType = PFD_TYPE_RGBA,
            .iLayerType = PFD_MAIN_PLANE,
        };

        int format = ChoosePixelFormat(tempdc, &pfd);
        Assert(format);

        DescribePixelFormat(tempdc, format, sizeof(pfd), &pfd);
        BOOL set = SetPixelFormat(tempdc, format, &pfd);
        Assert(set);

        temprc = wglCreateContext(tempdc);
        Assert(temprc);

        BOOL make = wglMakeCurrent(tempdc, temprc);
        Assert(make);

        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (void*)wglGetProcAddress("wglCreateContextAttribsARB");

        int attrib[] =
        {
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0,
        };

        HGLRC newrc = wglCreateContextAttribsARB(tempdc, NULL, attrib);
        Assert(newrc);

        make = wglMakeCurrent(tempdc, newrc);
        Assert(make);

        wglDeleteContext(temprc);
        temprc = newrc;

        PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = (void*)wglGetProcAddress("glDebugMessageCallback");
        glDebugMessageCallback(DebugCallback, 0);

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    // create D3D11 device & context
    {
        UINT flags = D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
        HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, levels, ARRAYSIZE(levels), D3D11_SDK_VERSION, &device, NULL, &context);
        AssertHR(hr);
    }

    // enable VERY USEFUL debug break on API errors
    {
        ID3D11InfoQueue* info;
        ID3D11Device_QueryInterface(device, &IID_ID3D11InfoQueue, (void**)&info);
        ID3D11InfoQueue_SetBreakOnSeverity(info, D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        ID3D11InfoQueue_SetBreakOnSeverity(info, D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
        ID3D11InfoQueue_Release(info);
    }

    // create DXGI swap chain
    {
        IDXGIFactory2* factory;
        HRESULT hr = CreateDXGIFactory(&IID_IDXGIFactory2, &factory);
        AssertHR(hr);

        DXGI_SWAP_CHAIN_DESC1 desc =
        {
            .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
            .SampleDesc = { 1, 0 },
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = 2,
            .Scaling = DXGI_SCALING_NONE,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        };

        hr = IDXGIFactory2_CreateSwapChainForHwnd(factory, (IUnknown*)device, window, &desc, NULL, NULL, &swapChain);
        AssertHR(hr);
        IDXGIFactory_Release(factory);
    }

    dxDevice = wglDXOpenDeviceNV(device);
    Assert(dxDevice);

    glGenRenderbuffers(1, &colorRbuf);
    glGenFramebuffers(1, &fbuf);

    glBindFramebuffer(GL_FRAMEBUFFER, fbuf);
}

static void Destroy()
{
    ID3D11DeviceContext_ClearState(context);

    glDeleteFramebuffers(1, &fbuf);
    glDeleteRenderbuffers(1, &colorRbuf);

    wglDXCloseDeviceNV(dxDevice);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(temprc);
    ReleaseDC(temp, tempdc);

    ID3D11DeviceContext_ClearState(context);
    ID3D11DeviceContext_Release(context);
    ID3D11Device_Release(device);
    IDXGISwapChain_Release(swapChain);
}

static void Resize(int width, int height)
{
    ID3D11DeviceContext_ClearState(context);

    HRESULT hr = IDXGISwapChain_ResizeBuffers(swapChain, 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    AssertHR(hr);

    D3D11_VIEWPORT view =
    {
        .TopLeftX = 0.f,
        .TopLeftY = 0.f,
        .Width = (float)width,
        .Height = (float)height,
        .MinDepth = 0.f,
        .MaxDepth = 1.f,
    };
    ID3D11DeviceContext_RSSetViewports(context, 1, &view);

    glViewport(0, 0, width, height);
}

int main1()
{
    WNDCLASSA wc =
    {
        .lpfnWndProc = 0,
        .lpszClassName = "DXGL",
    };

    ATOM atom = RegisterClassA(&wc);
    Assert(atom);

    HWND window = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_NOREDIRECTIONBITMAP, wc.lpszClassName, "DXGL", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
    Assert(window);

    int running = 1;
    for (;;)
    {
        MSG msg;
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = 0;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if (!running)
        {
            break;
        }

        HANDLE dxColor;
        ID3D11RenderTargetView* colorView;
        {
            ID3D11Texture2D* colorBuffer;
            HRESULT hr = IDXGISwapChain_GetBuffer(swapChain, 0, &IID_ID3D11Texture2D, &colorBuffer);
            AssertHR(hr);

            hr = ID3D11Device_CreateRenderTargetView(device, (void*)colorBuffer, NULL, &colorView);
            AssertHR(hr);

            dxColor = wglDXRegisterObjectNV(dxDevice, colorBuffer, colorRbuf, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
            Assert(dxColor);

            ID3D11Texture2D_Release(colorBuffer);
        }

        // render with D3D
        {
            FLOAT cornflowerBlue[] = { 100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.f };
            ID3D11DeviceContext_OMSetRenderTargets(context, 1, &colorView, NIL);
            ID3D11DeviceContext_ClearRenderTargetView(context, colorView, cornflowerBlue);
        }

        HANDLE dxObjects[] = { dxColor };
        wglDXLockObjectsNV(dxDevice, _countof(dxObjects), dxObjects);

        // render with GL
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbuf);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbuf);
#if 0
            glBegin(GL_TRIANGLES);
            glColor3f(1, 0, 0);
            glVertex2f(0.f, -0.5f);
            glColor3f(0, 1, 0);
            glVertex2f(0.5f, 0.5f);
            glColor3f(0, 0, 1);
            glVertex2f(-0.5f, 0.5f);
            glEnd();
#endif
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        wglDXUnlockObjectsNV(dxDevice, _countof(dxObjects), dxObjects);

        wglDXUnregisterObjectNV(dxDevice, dxColor);

        ID3D11RenderTargetView_Release(colorView);

        HRESULT hr = IDXGISwapChain_Present(swapChain, 1, 0);
        Assert(SUCCEEDED(hr));
    }
}
#endif


ZGL afxError checkForResizeDxgi(zglDpu* dpu, afxSurface dout);

_ZGL afxError setObjectsLockedDxgi(zglDpu* dpu, afxSurface dout, afxBool locked)
{
    afxError err = { 0 };
    afxDrawDevice ddev = AvxGetSurfaceHost(dout);

    if (dout->dxgi.mRenderbufferBufferHandle == NIL)
    {
        AFX_ASSERT(dout->dxgi.mTextureHandle == NIL);
        return err;
    }

    HANDLE resources[] = {
        dout->dxgi.mRenderbufferBufferHandle,
        dout->dxgi.mTextureHandle,
    };

    GLint count = (dout->dxgi.mTextureHandle != NIL) ? 2 : 1;

    if (locked)
    {
        if (!wglDXLockObjectsNV(dout->dxgi.d3d11DevForGl, count, resources))
        {
            AfxThrowError();
        }
    }
    else
    {
        if (!wglDXUnlockObjectsNV(dout->dxgi.d3d11DevForGl, count, resources))
        {
            AfxThrowError();
        }
    }
    return err;
}

_ZGL afxError swapDxgi(zglDpu* dpu, afxSurface dout)
{
    afxError err;
    glVmt const* gl = dpu->gl;

    gl->Flush();
    setObjectsLockedDxgi(dpu, dout, FALSE);
    HRESULT result = IDXGISwapChain_Present(dout->dxgi.mSwapChain, dout->dxgi.mSwapInterval, 0);
    dout->dxgi.mFirstSwap = FALSE;
    setObjectsLockedDxgi(dpu, dout, TRUE);

    if (FAILED(result))
    {
        AfxThrowError();
        return err;
    }
    return checkForResizeDxgi(dpu, dout);
}

_ZGL afxError postSubBufferDxgi(zglDpu* dpu, afxSurface dout, afxRect const* rc)
{
    afxError err;
    glVmt const* gl = dpu->gl;

    AFX_ASSERT(rc->w > 0 && rc->h > 0);
    AFX_ASSERT(dout->dxgi.mSwapChain1 != NIL);
    gl->Flush();
    setObjectsLockedDxgi(dpu, dout, FALSE);
    HRESULT result = S_OK;

    if (dout->dxgi.mFirstSwap)
    {
        result = IDXGISwapChain1_Present(dout->dxgi.mSwapChain1, dout->dxgi.mSwapInterval, 0);
        dout->dxgi.mFirstSwap = FALSE;
    }
    else
    {
        RECT rect = { (LONG)(rc->x), (LONG)(dout->m.ccfg.whd.h - rc->y - rc->h), (LONG)(rc->x + rc->w), (LONG)(dout->m.ccfg.whd.h - rc->y) };
        DXGI_PRESENT_PARAMETERS params = { 1, &rect, NIL, NIL };
        result = IDXGISwapChain1_Present1(dout->dxgi.mSwapChain1, dout->dxgi.mSwapInterval, 0, &params);
    }

    setObjectsLockedDxgi(dpu, dout, TRUE);

    if (FAILED(result))
    {
        AfxThrowError();
        return err;
    }
    return checkForResizeDxgi(dpu, dout);
}

_ZGL afxError bindTexImageDxgi(zglDpu* dpu, afxSurface dout, avxRaster texture, GLint buffer)
{
    afxError err;
    afxDrawDevice ddev = AvxGetSurfaceHost(dout);

    AFX_ASSERT(dout->dxgi.mTextureHandle == NIL);
    GLuint textureID = texture->glHandle;
    ID3D11Texture2D *colorBuffer = NIL;
    HRESULT result = IDXGISwapChain_GetBuffer(dout->dxgi.mSwapChain, 0, &IID_ID3D11Texture2D, (void**)(&colorBuffer));

    if (FAILED(result))
    {
        AfxThrowError();
        return err;
    }

    dout->dxgi.mTextureHandle = wglDXRegisterObjectNV(dout->dxgi.d3d11DevForGl, colorBuffer, textureID, GL_TEXTURE_2D, WGL_ACCESS_READ_WRITE_NV);
    ID3D11Texture2D_Release(colorBuffer);

    if (dout->dxgi.mTextureHandle == NIL)
    {
        AfxThrowError();
        return err;
    }

    if (!wglDXLockObjectsNV(dout->dxgi.d3d11DevForGl, 1, &dout->dxgi.mTextureHandle))
    {
        wglDXUnregisterObjectNV(dout->dxgi.d3d11DevForGl, dout->dxgi.mTextureHandle);
        dout->dxgi.mTextureHandle = NIL;
        AfxThrowError();
        return err;
    }

    dout->dxgi.mTextureID = textureID;

    return err;
}

_ZGL afxError createSwapchainDxgi(zglDpu* dpu, afxSurface dout);

_ZGL afxError checkForResizeDxgi(zglDpu* dpu, afxSurface dout)
{
    afxError err = { 0 };

    RECT rect;

    if (!GetClientRect(dout->hWnd, &rect))
    {
        AfxThrowError();
        return err;
    }

    size_t newWidth = rect.right - rect.left;
    size_t newHeight = rect.bottom - rect.top;

    //if (newWidth != dout->idd->dxgi.mWidth || newHeight != dout->idd->dxgi.mHeight)
    {
        //dout->idd->dxgi.mWidth = newWidth;
        //dout->idd->dxgi.mHeight = newHeight;
        // TODO(geofflang): Handle resize by resizing the swap chain instead of re-creating it.

        if (createSwapchainDxgi(dpu, dout))
            AfxThrowError();
    }
    return err;
}

_ZGL IDXGIFactory *GetDXGIFactoryFromDevice(ID3D11Device *device)
{
    afxError err = { 0 };

    IDXGIDevice *dxgiDevice = NIL;
    HRESULT result = ID3D11Device_QueryInterface(device, &IID_IDXGIDevice, (void**)(&dxgiDevice));

    if (FAILED(result))
    {
        return NIL;
    }

    IDXGIAdapter *dxgiAdapter = NIL;
    result = IDXGIDevice_GetParent(dxgiDevice, &IID_IDXGIAdapter, (void**)(&dxgiAdapter));
    IDXGIDevice_Release(dxgiDevice);

    if (FAILED(result))
    {
        return NIL;
    }

    IDXGIFactory *dxgiFactory = NIL;
    result = IDXGIAdapter_GetParent(dxgiAdapter, &IID_IDXGIFactory, (void**)(&dxgiFactory));
    IDXGIAdapter_Release(dxgiAdapter);

    if (FAILED(result))
    {
        return NIL;
    }
    return dxgiFactory;
}

_ZGL afxError createSwapchainDxgi(zglDpu* dpu, afxSurface dout)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    afxDrawDevice ddev = AvxGetSurfaceHost(dout);

    if (setObjectsLockedDxgi(dpu, dout, FALSE)) AfxThrowError();
    else
    {
        if (dout->dxgi.mRenderbufferBufferHandle)
        {
            wglDXUnregisterObjectNV(dout->dxgi.d3d11DevForGl, dout->dxgi.mRenderbufferBufferHandle);
            dout->dxgi.mRenderbufferBufferHandle = NIL;
        }
        // If this surface is bound to a texture, unregister it.
        afxBool hadBoundSurface = (dout->dxgi.mTextureHandle != NIL);

        if (hadBoundSurface)
        {
            wglDXUnregisterObjectNV(dout->dxgi.d3d11DevForGl, dout->dxgi.mTextureHandle);
            dout->dxgi.mTextureHandle = NIL;
        }
        IDXGIFactory *dxgiFactory = GetDXGIFactoryFromDevice(dout->dxgi.d3d11Dev);

        if (dxgiFactory == NIL)
        {
            AfxThrowError();
            return err;
        }

        IDXGIFactory2 *dxgiFactory2 = NIL;
        HRESULT result = IDXGIFactory_QueryInterface(dxgiFactory, &IID_IDXGIFactory2, (void**)(&dxgiFactory2));

        if (SUCCEEDED(result))
        {
            AFX_ASSERT(dxgiFactory2 != NIL);
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.BufferCount = 1;
            swapChainDesc.Format = dout->dxgi.mSwapChainFormat;
            swapChainDesc.Width = (UINT)(dout->m.ccfg.whd.w);
            swapChainDesc.Height = (UINT)(dout->m.ccfg.whd.h);
            swapChainDesc.Format = dout->dxgi.mSwapChainFormat;
            swapChainDesc.Stereo = FALSE;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_BACK_BUFFER;
            swapChainDesc.BufferCount = 1;
            swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            swapChainDesc.Flags = dout->dxgi.mSwapChainFlags;
            result = IDXGIFactory2_CreateSwapChainForHwnd(dxgiFactory2, dout->dxgi.d3d11Dev, dout->hWnd, &swapChainDesc, NIL, NIL, &dout->dxgi.mSwapChain1);

            IDXGIFactory2_Release(dxgiFactory2);
            IDXGIFactory_Release(dxgiFactory);

            if (FAILED(result))
            {
                AfxThrowError();
            }
            dout->dxgi.mSwapChain = dout->dxgi.mSwapChain1;
            IDXGISwapChain_AddRef(dout->dxgi.mSwapChain);
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = 1;
            swapChainDesc.BufferDesc.Format = dout->dxgi.mSwapChainFormat;
            swapChainDesc.BufferDesc.Width = (UINT)(dout->m.ccfg.whd.w);
            swapChainDesc.BufferDesc.Height = (UINT)(dout->m.ccfg.whd.h);
            swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_BACK_BUFFER;
            swapChainDesc.Flags = dout->dxgi.mSwapChainFlags;
            swapChainDesc.OutputWindow = dout->hWnd;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            result = IDXGIFactory_CreateSwapChain(dxgiFactory, dout->dxgi.d3d11Dev, &swapChainDesc, &dout->dxgi.mSwapChain);
            IDXGIFactory_Release(dxgiFactory);

            if (FAILED(result))
            {
                AfxThrowError();
            }
        }

        ID3D11Texture2D *colorBuffer = NIL;
        result = IDXGISwapChain_GetBuffer(dout->dxgi.mSwapChain, 0, &IID_ID3D11Texture2D, (void**)(&colorBuffer));

        if (FAILED(result))
        {
            AfxThrowError();
        }

        gl->BindRenderbuffer(GL_RENDERBUFFER, dout->dxgi.mColorRenderbufferID); // state mgr
        dout->dxgi.mRenderbufferBufferHandle = wglDXRegisterObjectNV(dout->dxgi.d3d11DevForGl, colorBuffer, dout->dxgi.mColorRenderbufferID, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
        ID3D11Texture2D_Release(colorBuffer);

        if (dout->dxgi.mRenderbufferBufferHandle == NIL)
        {
            AfxThrowError();
        }

        // Rebind the surface to the texture if needed.
        if (hadBoundSurface)
        {
            dout->dxgi.mTextureHandle = wglDXRegisterObjectNV(dout->dxgi.d3d11DevForGl, colorBuffer, dout->dxgi.mTextureID, GL_TEXTURE_2D, WGL_ACCESS_READ_WRITE_NV);

            if (dout->dxgi.mTextureHandle == NIL)
            {
                AfxThrowError();
            }
        }

        setObjectsLockedDxgi(dpu, dout, TRUE);

        if (err)
        {
            return err;
        }

        dout->dxgi.mFirstSwap = TRUE;
    }
}



