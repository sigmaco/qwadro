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
    AfxConfigureSystem(&sysc);
    AfxBootstrapSystem(&sysc);

    // Set up the draw system

    afxUnit drawIcd = 0;
    afxDrawSystem dsys;
    afxDrawSystemConfig dsyc = { 0 };
    dsyc.caps = avxAptitude_GFX;
    dsyc.accel = afxAcceleration_DPU;
    dsyc.exuCnt = 1;
    AvxConfigureDrawSystem(drawIcd, &dsyc);
    AvxEstablishDrawSystem(drawIcd, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Open a session

    afxUnit shIcd = 0;
    afxEnvironment env;
    afxEnvironmentConfig ecfg = { 0 };
    ecfg.dsys = dsys; // integrate our draw system
    AfxConfigureEnvironment(shIcd, &ecfg);
    AfxAcquireEnvironment(shIcd, &ecfg, &env);
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AfxOpenEnvironment(env, NIL, NIL, NIL);

    // Acquire a drawable surface

    afxWindow wnd;
    afxSurface dout;
    afxWindowConfig wcfg = { 0 };
    wcfg.dsys = dsys;
    //wcfg.dout.ccfg.bins[0].fmt = avxFormat_BGRA4un;
    AfxConfigureWindow(env, &wcfg, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxAcquireWindow(env, &wcfg, &wnd);
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Operation contexts

    afxUnit frameCap = AFX_CLAMP(wcfg.dout.latency, 1, 3);

    afxDrawContext drawContexts[3];
    avxContextInfo ctxi = { 0 };
    ctxi.caps = avxAptitude_GFX;
    AvxAcquireDrawContexts(dsys, &ctxi, frameCap, drawContexts);
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, frameCap, drawContexts);

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

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

        afxUnit outBufIdx = 0;
        if (AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_NONE, NIL, NIL, &outBufIdx))
        {
            continue;
        }

        afxDrawContext dctx = drawContexts[outBufIdx];

        afxUnit batchId;
        if (AvxRecordDrawCommands(dctx, TRUE, FALSE, &batchId))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        afxLayeredRect area;
        avxCanvas canv;
        AvxGetSurfaceCanvas(dout, outBufIdx, &canv, &area);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

        avxDrawScope dps = { 0 };
        dps.canv = canv;
        dps.bounds = area;
        dps.targetCnt = 1;
        dps.targets[0].clearVal = AVX_COLOR_VALUE(AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), 1);
        dps.targets[0].loadOp = avxLoadOp_CLEAR;
        dps.targets[0].storeOp = avxStoreOp_STORE;
        dps.ds[0].clearVal.depth = 1.0;
        dps.ds[0].clearVal.stencil = 0;
        dps.ds[0].loadOp = avxLoadOp_CLEAR;
        dps.ds[0].storeOp = avxStoreOp_STORE;

        AvxCmdCommenceDrawScope(dctx, &dps);

        avxViewport vp = AVX_VIEWPORT(0, 0, area.area.w, area.area.h, 0, 1);
        AvxCmdAdjustViewports(dctx, 0, 1, &vp);

        AvxCmdConcludeDrawScope(dctx);

        if (AvxCompileDrawCommands(dctx, batchId))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        avxFence drawCompletedFence = NIL;
        avxSubmission subm = { 0 };
        subm.fence = drawCompletedFence;
        subm.dctx = dctx;
        subm.batchId = batchId;

        if (AvxExecuteDrawCommands(dsys, 1, &subm))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        //AfxWaitForDrawQueue(dsys, AFX_TIMEOUT_INFINITE, subm.exuIdx);

        avxPresentation pres = { 0 };
        pres.dout = dout;
        pres.bufIdx = outBufIdx;
        //pres.waitOnDpu = drawCompletedFence;

        if (AvxPresentSurfaces(dsys, 1, &pres))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
        }
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &env);
    AfxDisposeObjects(1, &dsys);

    AfxDoSystemShutdown(0);
    AfxYield();

    return 0;
}
