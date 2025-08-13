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
    AvxConfigureDrawSystem(drawIcd, afxDrawCaps_DRAW, afxAcceleration_DPU, &dsyc);
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

    afxSurface dout;
    AfxGetWindowDrawOutput(wnd, FALSE, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    afxBool readyToRender = TRUE;

    while (1)
    {
        AfxPollInput(0, AFX_TIMEOUT_INFINITE);

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
        if (AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_IMMEDIATE, NIL, &outBufIdx, NIL, NIL))
            continue;
        
        afxUnit portId = 0;
        afxUnit queIdx = 0;
        afxDrawContext dctx;
        if (AvxAcquireDrawContexts(dsys, afxDrawCaps_DRAW, portId, TRUE, FALSE, 1, &dctx))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        avxCanvas canv;
        afxRect crc;
        AvxGetSurfaceCanvas(dout, outBufIdx, &canv, &crc);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

        avxDrawScope dps = { 0 };
        dps.canv = canv;
        dps.area = crc;
        dps.layerCnt = 1;
        dps.targetCnt = 1;
        dps.targets[0].clearVal = AVX_COLOR_VALUE(AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), AfxRandomReal2(0, 1), 1);
        dps.targets[0].loadOp = avxLoadOp_CLEAR;
        dps.targets[0].storeOp = avxStoreOp_STORE;
        dps.depth.clearVal.depth = 1.0;
        dps.depth.clearVal.stencil = 0;
        dps.depth.loadOp = avxLoadOp_CLEAR;
        dps.depth.storeOp = avxStoreOp_STORE;
        //dps.stencil = &ddt;

        AvxCmdCommenceDrawScope(dctx, &dps);

        avxViewport vp = AVX_VIEWPORT(0, 0, crc.w, crc.h, 0, 1);
        AvxCmdAdjustViewports(dctx, 0, 1, &vp);

        AvxCmdConcludeDrawScope(dctx);

        if (AvxCompileDrawCommands(dctx))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            AfxDisposeObjects(1, &dctx);
            continue;
        }

        avxSubmission subm = { 0 };
        avxFence drawCompletedFence = NIL;
        if (AvxExecuteDrawCommands(dsys, &subm, 1, &dctx, drawCompletedFence))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            AfxDisposeObjects(1, &dctx);
            continue;
        }

        //AfxWaitForDrawQueue(dsys, subm.exuIdx, subm.baseQueIdx, 0);

        avxPresentation pres = { 0 };
        if (AvxPresentSurfaces(dsys, &pres, NIL, 1, &dout, &outBufIdx, NIL))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
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
