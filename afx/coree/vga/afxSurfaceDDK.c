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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AVX_DRAW_C
//#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_BRIDGE_C
//#define _AVX_DRAW_QUEUE_C
#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "icd/avxIcd.h"

_AVX afxError _AvxDquePresentSurfaces(afxDrawQueue dque, afxUnit cnt, avxPresentation const presentations[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxPresentation const* pres = &presentations[i];

        afxSurface dout = pres->dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        AfxIncAtom32(&dout->submCnt);

#if 0
        if (pres->waitOnDpu && pres->dout->ddi->presOnDpuCb)
        {
            _AvxDqueSubmitCallback(dque, (void*)pres->dout->ddi->presOnDpuCb, pres);
        }
        else
#endif
        {
            //AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
            //dout->presentingBufIdx = pres->bufIdx;
#if 0
            if (pres->waitOnDpu)
            {
                while (AvxWaitForFences(AfxGetHost(pres->dout), AFX_TIMEOUT_INFINITE, FALSE, 1, &pres->waitOnDpu, NIL))
                {
                    AfxYield();
                }
            }
#endif
            if (dout->ddi->presentCb && dout->ddi->presentCb(dque, pres))
                AfxThrowError();

            dout->lastPresentedBufIdx = (afxAtom32)pres->bufIdx;

            afxClock currClock;
            AfxGetClock(&currClock);
            ++dout->outNo;

            if ((1.0 <= AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)))
            {
                dout->outCntResetClock = currClock;
                dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
                dout->outNo = 0;

                afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
                afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
                dout->lastClock = currClock;

                if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
                {
                    //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
                }

                if (dout->endpointNotifyFn)
                {
                    dout->endpointNotifyFn(dout->endpointNotifyObj, pres->bufIdx);
                }
            }

            _AvxDoutSwUnlockBufCb(dout, pres->bufIdx);
        }

        AfxDecAtom32(&dout->submCnt);
    }
    return err;
}

_AVX afxError _AvxDqueScanSurfaces(afxDrawQueue dque, afxUnit cnt, avxCaption const captions[])
{
    afxError err = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxCaption const* cap = &captions[i];

        afxSurface dout = cap->dout;
        AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

        AfxIncAtom32(&dout->submCnt);
#if 0
        if (pres->waitOnDpu && pres->dout->ddi->presOnDpuCb)
        {
            _AvxDqueSubmitCallback(dque, (void*)pres->dout->ddi->presOnDpuCb, pres);
        }
        else
#endif
        {
            //AFX_ASSERT(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
            //dout->presentingBufIdx = pres->bufIdx;
#if 0
            if (pres->waitOnDpu)
            {
                while (AvxWaitForFences(AfxGetHost(pres->dout), AFX_TIMEOUT_INFINITE, FALSE, 1, &pres->waitOnDpu, NIL))
                {
                    AfxYield();
                }
            }
#endif
            if (dout->ddi->captureCb && dout->ddi->captureCb(dque, cap))
                AfxThrowError();

            dout->lastPresentedBufIdx = (afxAtom32)cap->bufIdx;

            afxClock currClock;
            AfxGetClock(&currClock);
            ++dout->outNo;

            if ((1.0 <= AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)))
            {
                dout->outCntResetClock = currClock;
                dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
                dout->outNo = 0;

                afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
                afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
                dout->lastClock = currClock;

                if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
                {
                    //AfxFormatWindowTitle(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
                }

                if (dout->endpointNotifyFn)
                {
                    dout->endpointNotifyFn(dout->endpointNotifyObj, cap->bufIdx);
                }
            }

            _AvxDoutSwUnlockBufCb(dout, cap->bufIdx);
        }

        AfxDecAtom32(&dout->submCnt);
    }
    return err;
}
