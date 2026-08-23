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

#define FRAMES_IN_FLIGHT 3

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

    // Boot up the Qwadro (if necessary)

    afxSystemConfig sysc = { 0 };
    AfxConfigureSystem(&sysc, NIL);
    AfxBootstrapSystem(&sysc);

    // Set up the draw system

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

    // Open a session

    afxModule auxIcd;
    AfxGetShell(0, &auxIcd);

    afxEnvironment env;
    afxEnvironmentConfig ecfg = { 0 };
    ecfg.dsys = dsys; // integrate our draw system
    AfxConfigureEnvironment(auxIcd, &ecfg);
    AfxEstablishEnvironment(auxIcd, &ecfg, &env);
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    // Acquire a drawable surface

    afxWindow wnd;
    afxWindowConfig wcfg = { 0 };
    wcfg.dout.dsys = dsys;
    //wcfg.dout.bins[0].fmt = avxFormat_BGRA4un;
    AfxConfigureWindow(env, &wcfg, AFX_V2D_ZERO, AFX_V2D(0.5, 0.5));
    AfxAcquireWindow(env, &wcfg, &wnd);
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

    afxSurface dout;
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Operation contexts

    afxUnit frameCap = AFX_CLAMP(wcfg.dout.latency, 1, FRAMES_IN_FLIGHT);

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    afxReal64 ft = 0;
    afxUnit fpsi = 0;
    afxUnit fps = 0;

    afxBool readyToRender = TRUE;

    while (1)
    {
        AfxDoUx(0, AFX_TIMEOUT_INFINITE);

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

        afxUnit outBufIdx = 0;
        if (AfxFailed(AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_IGNORED, NIL, NIL, &outBufIdx)))
        {
            AfxThrowError();
        }
        else
        {
            afxDrawContext dctx;
            avxContextConfig ctxi = { 0 };
            ctxi.caps = avxService_GFX;
            AvxAcquireDrawContexts(dsys, NIL, &ctxi, 1, &dctx);
            AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

            avxFence drawCompletedFence = NIL;
            afxBool presented = FALSE;
            afxBool compiled = FALSE;

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
                dps.targets[0].clearVal = AVX_COLOR_VALUE(AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), 1);
                dps.ds[0].storeOp = avxStoreOp_STORE;
                dps.ds[0].loadOp = avxLoadOp_CLEAR;
                dps.ds[0].clearVal = AVX_DEPTH_VALUE(1.0, 0);
                
                if (AfxSucceded(AvxCmdCommenceDrawScope(dctx, &dps)))
                {
                    avxViewport vp = AVX_VIEWPORT(0, 0, area.area.w, area.area.h, 0, 1);
                    AvxCmdAdjustViewports(dctx, 0, 1, &vp);

                    AvxCmdConcludeDrawScope(dctx);
                }

                if (AfxFailed(AvxCompileDrawCommands(dctx)))
                {
                    AfxThrowError();
                }
                else
                {
                    compiled = TRUE;

                    afxUnit dqueIdx;
                    avxSubmission subm = { 0 };
                    subm.dctx = dctx;
                    subm.signal = drawCompletedFence;

                    if (AfxFailed(AvxExecuteDrawCommands(dsys, 1, &subm, &dqueIdx)))
                    {
                        AfxThrowError();
                        AvxUnlockSurfaceBuffer(dout, outBufIdx);
                        continue;
                    }

                    //AvxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, subm.exuMask, dqueIdx);
                    //AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, subm.exuMask);
                }
            }

            AfxDisposeObjects(1, &dctx);

            if (compiled)
            {
                avxPresentation pres = { 0 };
                pres.wait = drawCompletedFence;
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
        }
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &env);
    AfxDisposeObjects(1, &dsys);

    AfxAbolishSystem(0);
    AfxYield();

    return 0;
}
