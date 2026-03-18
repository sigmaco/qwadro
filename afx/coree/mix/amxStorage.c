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

// This software is part of Advanced Multimedia Extensions.

// It is hard to invent something when there is nothing to be copied.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AMX_MIX_C
//#define _AMX_SOUNDSCAPE_C
//#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_MIX_INPUT_C
//#define _AMX_SINK_C
#include "amxIcd.h"

_AMX afxError _AmxMsysTransferCb_SW(afxMixSystem msys, amxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask msysIoExuMask = msys->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = msys->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (msysIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(msysIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && msys->dedIoExuMask && (!(msys->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (msys->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & msys->dedIoExuMask))
                    {
                        if (!(msys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (msys->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxMixBridge mexu;
            if (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AmxMexuTransferMixMemory(mexu, ctrl, opCnt, ops);

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

_AMX afxError _AmxMsysRemapBuffersCb_SW(afxMixSystem msys, afxBool unmap, afxUnit cnt, _amxBufferRemapping const maps[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AmxMsysGetIoExuMask(msys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxMixBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuRemapBuffers(mexu, unmap, cnt, maps);
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
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuRemapBuffers(mexu, unmap, cnt, maps);
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

_AMX afxError _AmxMsysCohereMappedBuffersCb_SW(afxMixSystem msys, afxBool discard, afxUnit cnt, amxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AmxMsysGetIoExuMask(msys, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxMixBridge mexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuCohereMappedBuffers(mexu, discard, cnt, maps);
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
        exuCnt = AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AmxChooseMixBridges(msys, AFX_INVALID_INDEX, amxAptitude_DMA, ioExuMask, exuIdx++, 1, &mexu))
        {
            queErr = _AmxMexuCohereMappedBuffers(mexu, discard, cnt, maps);
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
