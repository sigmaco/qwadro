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

// This software is part of Advanced Multimedia Extensions & Experiments.

// It is hard to invent something when there is nothing to be copied.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _ASX_STEP_C
#define _ASX_STEP_SYSTEM_C
#include "asxIcd.h"

_ASX afxError _AsxSsysTransferCb_SW(afxStepSystem ssys, asxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask ssysIoExuMask = ssys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = ssys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (ssysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(ssysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && ssys->dedIoExuMask && (!(ssys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (ssys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & ssys->dedIoExuMask))
                    {
                        if (!(ssys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (ssys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxStepBridge mexu;
            if (!AsxGetStepBridges(ssys, exuIdx, 1, &mexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AsxSexuTransferStepMemory(mexu, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        if (err || queued) break; // while --- find bridges
    }
    return err;
}

_ASX afxError _AsxSsysRemapBuffersCb_SW(afxStepSystem ssys, afxBool unmap, afxUnit cnt, _asxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AsxSsysGetIoExuMask(ssys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxStepBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AsxSexuRemapBuffers(mexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AsxSexuRemapBuffers(mexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_ASX afxError _AsxSsysCohereMappedBuffersCb_SW(afxStepSystem ssys, afxBool discard, afxUnit cnt, asxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @ssys must be a valid afxStepSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SSYS, 1, &ssys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AsxSsysGetIoExuMask(ssys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxStepBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AsxSexuCohereMappedBuffers(mexu, discard, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AsxChooseStepBridges(ssys, AFX_INVALID_INDEX, asxAptitude_DMA, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AsxSexuCohereMappedBuffers(mexu, discard, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}
