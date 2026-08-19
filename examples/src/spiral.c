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
 *                                 Public Test Build
 *                             (c) 2017 SIGMA FEDERATION
 *                      Open sourced under the Qwadro License.
 */

#include "qwadro/afxQwadro.h"

//#define WAIT_FOR_FRAME TRUE
#define MAX_FRAMES_IN_FLIGHT 3

#ifdef AFX_OS_WIN
#ifdef AFX_OS_WIN64
#ifdef _AFX_DEBUG
#pragma comment(lib, "../../qwd/w64d/e2coree")
#else
#pragma comment(lib, "../../qwd/w64/e2coree")
#endif
#else
#ifdef _AFX_DEBUG
#pragma comment(lib, "../../qwd/w32d/e2coree")
#else
#pragma comment(lib, "../../qwd/w32/e2coree")
#endif
#endif
#endif

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;

    /*
        This code initializes a graphics system, sets up a draw context, and renders a series of lines in polar coordinates. 
        It uses low-level drawing commands and manages various graphical resources like buffers, environments, and contexts.
    */

    // Boot up the Qwadro (if necessary)

    afxSystemConfig sysc = { 0 };
    AfxConfigureSystem(&sysc, NIL);
    AfxBootstrapSystem(&sysc);

    /*
        System and Draw System Setup:

        Initializes and configures the graphics system (afxSystemConfig).

        Configures and establishes a drawing system (afxDrawSystem), 
        setting up graphics capabilities and acceleration settings.
    */

    afxModule avxIcd;
    AvxGetIcd(0, &avxIcd);

    afxDrawSystem dsys;
    avxSystemConfig dsyc = { 0 };
    dsyc.caps = avxService_GFX;
    dsyc.accel = afxAcceleration_DPU;
    dsyc.exuCnt = 1;
    AvxConfigureDrawSystem(avxIcd, &dsyc);
    AvxEstablishDrawSystem(avxIcd, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    /*
        Window and Surface Setup

        Sets up a window environment to display the graphics (afxWindowConfig).
    */

    afxModule auxIcd;
    AfxGetShell(0, &auxIcd);

    afxEnvironment env;
    afxEnvironmentConfig ecfg = { 0 };
    ecfg.dsys = dsys; // integrate our draw system
    AfxConfigureEnvironment(auxIcd, &ecfg);
    AfxEstablishEnvironment(auxIcd, &ecfg, &env);
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxWindow wnd;
    afxSurface dout;
    afxWindowConfig wcfg = { 0 };
    wcfg.dout.dsys = dsys;
    //wcfg.dout.bins[0].fmt = avxFormat_BGRA4un;
    wcfg.dout.latency = MAX_FRAMES_IN_FLIGHT;
    AfxConfigureWindow(env, &wcfg, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxAcquireWindow(env, &wcfg, &wnd);
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    /*
        Acquires a drawable surface (afxSurface), 
        which is where the graphical content will be rendered.
    */

    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    /*
        Draw Contexts Setup :

        Configures and acquires drawing contexts(afxDrawContext), 
        which represent the specific drawing settings for each frame.
    */

    afxUnit frameCap = AFX_CLAMP(wcfg.dout.latency, 1, MAX_FRAMES_IN_FLIGHT);

    afxUnit frameBufIndices[MAX_FRAMES_IN_FLIGHT] = { 0 };

    afxDrawContext drawContexts[MAX_FRAMES_IN_FLIGHT];
    avxContextConfig ctxi = { 0 };
    ctxi.caps = avxService_GFX;
    AvxAcquireDrawContexts(dsys, NIL, &ctxi, frameCap, drawContexts);
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, frameCap, drawContexts);

    avxFence frameReadySems[MAX_FRAMES_IN_FLIGHT];
    avxFence frameCompleteSems[MAX_FRAMES_IN_FLIGHT];
    avxFenceInfo frameSemsInfos[MAX_FRAMES_IN_FLIGHT] = { 0 };
    frameSemsInfos[0].flags = avxFenceFlag_PROGRESSIVE;
    frameSemsInfos[1].flags = avxFenceFlag_PROGRESSIVE;
    frameSemsInfos[2].flags = avxFenceFlag_PROGRESSIVE;
    AvxAcquireFences(dsys, frameCap, frameSemsInfos, frameReadySems);
    AvxAcquireFences(dsys, frameCap, frameSemsInfos, frameCompleteSems);

    avxFence sema;
    avxFenceInfo semi = { 0 };
    semi.flags = avxFenceFlag_PROGRESSIVE;
    AvxAcquireFences(dsys, 1, &semi, &sema);

    arxRenderContext rctx;
    arxRenderConfiguration rcfg = { 0 };
    rcfg.dsys = dsys;
    ArxConfigureRenderContext(NIL, &rcfg);
    ArxAcquireRenderContext(NIL, &rcfg, &rctx);

    /*
        Main Rendering Loop:

        In a continuous loop, the code checks if the system is still executing.

        It calculates the time difference between frames, prepares the drawing context, and renders the scene.

        It uses polar coordinates to draw lines by converting them to Cartesian coordinates and then drawing the lines between the converted points.

        Each line is drawn with varying radial distances and angles, creating some form of graphical pattern (likely spiraling or rotating).
    */

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    afxBool readyToRender = TRUE;

    afxReal a = -1;

    afxReal64 ft = 0;
    afxUnit fpsi = 0;
    afxUnit fps = 0;

    afxUnit64 nextFrameId = 0;

    while (1)
    {
        if (!AfxSystemIsExecuting())
            break;

        afxClock currClock;
        AfxGetClock(&currClock);
        afxReal64 ct = AfxGetSecondsElapsed(&startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&lastClock, &currClock);
        lastClock = currClock;

        if (!readyToRender)
            continue;

        if (ct - ft >= 1.0)
        {
            fps = fpsi;
            fpsi = 0;
            ft = ct;
        }
        ++fpsi;

#if WAIT_FOR_FRAME
        if (nextFrameId > frameCap)
        {
            AvxWaitForFence(sema, nextFrameId - frameCap, AFX_TIMEOUT_INFINITE);
        }
#endif

        /*
            Frame and Scene Management:

            Begins and ends frames and scenes using Arx functions, tied to some rendering pipeline.

            The loop also appears to be controlling the rendering state, adjusting viewports, and submitting draw commands for execution.
        */

        afxUnit outBufIdx = 0;
        if (AfxFailed(AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_IGNORED, NIL, NIL, &outBufIdx)))
        {
            //continue;
            AfxDoUx(0, AFX_TIMEOUT_INFINITE);
        }
        else
        {
#if WAIT_FOR_FRAME
            afxDrawContext dctx = drawContexts[outBufIdx];
#else
            avxContextConfig dci = { 0 };
            dci.caps = avxService_GFX;
            afxDrawContext dctx;
            AvxAcquireDrawContexts(dsys, NIL, &dci, 1, &dctx);
#endif
            avxFence drawCompletedFence = NIL;
            avxFence waitReadyBufFence = NIL;
            afxBool compiled = FALSE;
            afxBool presented = FALSE;


            if (AfxFailed(AvxPrepareDrawCommands(dctx, FALSE, NIL)))
            {
                AfxThrowError();
            }
            else
            {
                avxCanvas canv;
                afxLayeredRect area;
                AvxGetSurfaceCanvas(dout, outBufIdx, &canv, &area);
                AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

                avxDrawScope dps = { 0 };
                dps.canv = canv;
                dps.bounds = area;
                dps.targetCnt = 1;
                dps.targets[0].storeOp = avxStoreOp_STORE;
                dps.targets[0].loadOp = avxLoadOp_CLEAR;
                dps.targets[0].clearVal = AVX_COLOR_VALUE(0, 0, 0, 1);
                dps.ds[0].storeOp = avxStoreOp_STORE;
                dps.ds[0].loadOp = avxLoadOp_CLEAR;
                dps.ds[0].clearVal = AVX_DEPTH_VALUE(1.0, 0);
                if (AfxFailed(AvxCmdCommenceDrawScope(dctx, &dps)))
                {

                }
                else
                {
                    avxViewport vp = AVX_VIEWPORT(0, 0, area.area.w, area.area.h, 0, 1);
                    AvxCmdAdjustViewports(dctx, 0, 1, &vp);

                    a += 0.005;

                    //screen.setRadialGradient(0, 0, 100, "rgb(255,85,0)", "rgb(0,170,255)");

                    ArxBeginFrame(rctx, NIL);
                    ArxBeginScene(rctx, NIL, dctx);
                    ArxAdvanceSceneLayer(rctx, arxSceneMode_WIRE_LINES);

                    //ArxUseCamera(rctx, cam, &area.area);

                    arxViewConstants vc = { 0 };
                    ArxComputeOrthographicMatrices(rctx, AFX_V2D(area.area.w, area.area.h), 0.001, 2.0, vc.p, vc.ip);
                    AfxM4dCopy(vc.pv, vc.p);
                    AfxM4dReset(vc.v);

                    ArxUpdateViewConstants(rctx, &vc);

                    /*
                        Code to draw a line between two points in polar coordinates. The r1, r2, a1, and a2 variables represent
                        the radial distances and angles (in radians) for the two points. We're then converting those polar coordinates
                        to Cartesian coordinates by multiplying the radial distances by the cosine and sine of the respective angles.

                        r1 * cos(a1) and r1 * sin(a1) convert the first point's polar coordinates (r1, a1) to Cartesian coordinates (x1, y1).

                        Similarly, r2 * AfxCos(a2) and r2 * AfxSin(a2) convert the second point's polar coordinates (r2, a2) to Cartesian coordinates (x2, y2).

                        drawLine(x1, y1, x2, y2) then draws a line between the two points (x1, y1) and (x2, y2) on the screen.
                    */

                    ArxSetWireframeConstants(rctx, 10, AVX_COLOR(0, 0.5, 0.5, 1), AVX_COLOR(1, 1, 1, 1));

                    afxM4d ssm;
                    AfxM4dScaling(ssm, AFX_V3D(2, 2, 1));
                    ArxPushTransform(rctx, ssm);

#if !0
                    for (afxUnit i = 0; i < 200; i++)
                    {
                        afxReal r1 = 100 - i / 2;
                        afxReal r2 = 100 - (i + 1) / 2;
                        afxReal a1 = i * a / 10;
                        afxReal a2 = (i + 1) * a / 10;
                        ArxDrawLine(rctx, AFX_V3D(r1 * AfxCos(a1), r1 * AfxSin(a1), 0), AFX_V3D(r2 * AfxCos(a2), r2 * AfxSin(a2), 0));
                    }
#else
                    afxV3d points[200*2];
                    for (afxUnit i = 0; i < 200; i++)
                    {
                        afxReal r1 = 100 - i / 2;
                        afxReal r2 = 100 - (i + 1) / 2;
                        afxReal a1 = i * a / 10;
                        afxReal a2 = (i + 1) * a / 10;
                        AfxV3dCopy(points[i * 2], AFX_V3D(r1 * AfxCos(a1), r1 * AfxSin(a1), 0));
                        AfxV3dCopy(points[i * 2 + 1], AFX_V3D(r2 * AfxCos(a2), r2 * AfxSin(a2), 0));
                    }
                    ArxDrawStrippedLines(rctx, 200*2, points, 3);
#endif

                    ArxEndScene(rctx, 0);
                    ArxEndFrame(rctx, NIL, NIL);
                    ArxAdvanceFrame(rctx, NIL);

                    AvxCmdConcludeDrawScope(dctx);
                }

                if (AfxFailed(AvxCompileDrawCommands(dctx)))
                {
                    AfxThrowError();
                }
                else
                {
                    compiled = TRUE;

#if DONOT_FENCE_COMPLETION
                    avxFence drawCompletedFence = NIL;
#else
                    drawCompletedFence = frameCompleteSems[outBufIdx];
#endif
#if DO_NOT_FENCE_READY_BUFFER
                    avxFence waitReadyBufFence = NIL;
#else
                    waitReadyBufFence = frameReadySems[outBufIdx];
#endif

                    avxSubmission subm = { 0 };
                    subm.dctx = dctx;
                    //subm.wait = sema;
                    //subm.waitValue = nextFrameId - frameCap;
#if WAIT_FOR_FRAME
                    subm.signal = sema;
                    subm.signalValue = nextFrameId++;
#endif

                    if (AfxFailed(AvxExecuteDrawCommands(dsys, 1, &subm, NIL)))
                    {
                        AfxThrowError();
                    }

                    /*
                        Synchronization:

                        After preparing the drawing commands, it synchronizes with the graphics hardware to ensure the surface
                        buffer is updated correctly and waits for completion before moving to the next frame.
                    */

                    //AfxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, subm.exuIdx);
                    //AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, subm.exuMask);
                }                
            }

#ifndef WAIT_FOR_FRAME
            AfxDisposeObjects(1, &dctx);
#endif

            // Will block infinitely on surface resizing due to swaplock.
            //AfxDoUx(0, AFX_TIMEOUT_INFINITE);

            if (compiled)
            {
                avxPresentation pres = { 0 };
                //pres.wait = subm.signal;
                //pres.waitValue = subm.signalValue;
                pres.dout = dout;
                pres.bufIdx = outBufIdx;

                if (AfxFailed(AvxPresentSurfaces(dsys, 1, &pres, NIL)))
                {
                    AfxThrowError();
                }
                else
                {
                    presented = TRUE;
                }

                AfxFormatWindowTitle(wnd, "FPS %u %u", fps, 0);
            }

            if (!presented)
            {
                AvxUnlockSurfaceBuffer(dout, outBufIdx);
            }

            AfxDoUx(0, AFX_TIMEOUT_INFINITE);
        }
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &env);
    AfxDisposeObjects(1, &dsys);

    AfxAbolishSystem(0);
    AfxYield();

    return 0;
}
