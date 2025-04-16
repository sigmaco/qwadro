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

#include "qwadro/inc/afxQwadro.h"

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
    AvxConfigureDrawSystem(drawIcd, &dsyc);
    dsyc.exuCnt = 1;
    AvxEstablishDrawSystem(drawIcd, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Open a session

    afxUnit shIcd = 0;
    afxSession ses;
    afxSessionConfig scfg = { 0 };
    scfg.dsys = dsys; // integrate our draw system
    AfxAcquireSession(shIcd, &scfg, &ses);
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AfxOpenSession(ses, NIL, NIL, NIL);

    // Acquire a drawable surface

    afxWindow wnd;
    afxWindowConfig wrc = { 0 };
    wrc.dsys = dsys;
    AfxConfigureWindow(&wrc, NIL, AFX_V3D(0.5, 0.5, 1));
    //wrc.surface.bufFmt[0] = avxFormat_BGRA4un;
    AfxAcquireWindow(&wrc, &wnd);
    //AfxAdjustWindowFromNdc(wnd, NIL, AFX_V3D(0.5, 0.5, 1));

    afxDrawOutput dout;
    AfxGetWindowDrawOutput(wnd, FALSE, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    afxBool readyToRender = TRUE;

    while (1)
    {
        AfxPollInput(0, AFX_TIME_INFINITE);

        if (!AfxSystemIsExecuting())
            break;

        afxClock currClock;
        AfxGetClock(&currClock);
        afxReal64 ct = AfxGetClockSecondsElapsed(&startClock, &currClock);
        afxReal64 dt = AfxGetClockSecondsElapsed(&lastClock, &currClock);
        lastClock = currClock;

        if (!readyToRender)
            continue;

        afxUnit outBufIdx = 0;
        if (AvxRequestDrawOutputBuffer(dout, 0, &outBufIdx))
            continue;
        
        afxUnit portId = 0;
        afxUnit queIdx = 0;
        afxDrawContext dctx;
        if (AvxAcquireDrawContexts(dsys, portId, queIdx, 1, &dctx))
        {
            AfxThrowError();
            AvxRecycleDrawOutputBuffer(dout, outBufIdx);
            continue;
        }

        avxCanvas canv;
        avxRange canvWhd;
        AvxGetDrawOutputCanvas(dout, outBufIdx, &canv);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        canvWhd = AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO);

        avxDrawScope dps = { 0 };
        dps.canv = canv;
        dps.layerCnt = 1;
        dps.targetCnt = 1;
        dps.targets[0].clearValue.rgba[0] = AfxRandomReal2(0, 1);
        dps.targets[0].clearValue.rgba[1] = AfxRandomReal2(0, 1);
        dps.targets[0].clearValue.rgba[2] = AfxRandomReal2(0, 1);
        dps.targets[0].clearValue.rgba[3] = 1;
        dps.targets[0].loadOp = avxLoadOp_CLEAR;
        dps.targets[0].storeOp = avxStoreOp_STORE;
        dps.depth.clearValue.depth = 1.0;
        dps.depth.clearValue.stencil = 0;
        dps.depth.loadOp = avxLoadOp_CLEAR;
        dps.depth.storeOp = avxStoreOp_STORE;
        //dps.stencil = &ddt;

        dps.area = AVX_RECT(0, 0, canvWhd.w, canvWhd.h);
        AvxCmdCommenceDrawScope(dctx, &dps);

        avxViewport vp = AVX_VIEWPORT(0, 0, canvWhd.w, canvWhd.h, 0, 1);
        AvxCmdAdjustViewports(dctx, 0, 1, &vp);

        AvxCmdConcludeDrawScope(dctx);

        if (AvxCompileDrawCommands(dctx))
        {
            AfxThrowError();
            AvxRecycleDrawOutputBuffer(dout, outBufIdx);
            AfxDisposeObjects(1, &dctx);
            continue;
        }

        avxSubmission subm = { 0 };
        avxFence drawCompletedFence = NIL;
        if (AvxExecuteDrawCommands(dsys, &subm, 1, &dctx, drawCompletedFence))
        {
            AfxThrowError();
            AvxRecycleDrawOutputBuffer(dout, outBufIdx);
            AfxDisposeObjects(1, &dctx);
            continue;
        }

        //AfxWaitForDrawQueue(dsys, subm.exuIdx, subm.baseQueIdx, 0);

        avxPresentation pres = { 0 };
        if (AvxPresentDrawOutputs(dsys, &pres, NIL, 1, &dout, &outBufIdx, NIL))
        {
            AfxThrowError();
            AvxRecycleDrawOutputBuffer(dout, outBufIdx);
        }
        AfxDisposeObjects(1, &dctx);
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &ses);
    AfxDisposeObjects(1, &dsys);

    AfxDoSystemShutdown(0);
    AfxYield();

    return 0;
}
