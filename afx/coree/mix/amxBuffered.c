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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#define _AMX_MIX_C
#define _AMX_BUFFER_C
#include "amxIcd.h"

_AMX afxError AmxMakeBufferedRing(amxBufferedRing* rng, amxBuffer mbuf, afxSize bufBase, afxUnit bufRange, afxUnit blockSiz, afxUnit blockAlign, afxUnit rounds)
{
    // Small buffered ring utility.
    // Reuse memory across frames (no new buffer creation).
    // Efficient for dynamic scenes and many objects.
    // Prevents CPU-GPU sync stalls and fragmentation.

    afxError err = { 0 };
    AFX_ASSERT(rng);
    *rng = (amxBufferedRing) { 0 };

    rng->mbuf = mbuf;
    rng->mbufBase = bufBase;
    rng->mbufRange = bufRange;

    if (mbuf)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);
        rng->basePtr = AvxGetBufferMap(mbuf, bufRange, bufRange);

        if (!rng->basePtr)
        {
            AfxReportError("avxBuffer %p [ %u, %u ] not mapped", mbuf, bufBase, bufRange);
            AfxThrowError();
        }
    }
    else
    {
        rng->basePtr = (void*)bufBase;

        if (!rng->basePtr)
        {
            AfxReportError("avxBuffer %p [ %u, %u ] not mapped", mbuf, bufBase, bufRange);
            AfxThrowError();
        }
    }

    rng->currOffset = 0;

    rng->blockAlign = AFX_ALIGN_SIZE(AFX_MIN(AMX_BUFFER_ALIGNMENT, blockAlign), AMX_BUFFER_ALIGNMENT);
    rng->blockSiz = AFX_MAX(rng->blockAlign, AMX_ALIGN_BUFFERED(blockSiz));
    rng->blockCnt = rng->mbufRange / rng->blockSiz;

    // Use triple buffering (rounds = 3) to rotate through buffer regions.
    rng->rounds = AFX_MAX(1, rounds);

    return err;
}

_AMX afxSize AmxCycleBufferedRing(amxBufferedRing* rng)
{
    afxError err = { 0 };
    AFX_ASSERT(rng);

    rng->currOffset = rng->mbufBase + (rng->currOffset + rng->blockSiz * rng->blockCnt / rng->rounds) % rng->mbufRange;

    return rng->currOffset;
}

_AMX void* AmxAdvanceBufferedRing(amxBufferedRing* rng, afxUnit reqSiz, afxSize* pOffset, afxUnit* pRange)
{
    afxError err = { 0 };
    AFX_ASSERT(rng);

    reqSiz = AFX_ALIGN_SIZE(reqSiz, rng->blockSiz);

    if (rng->currOffset + reqSiz > rng->mbufRange)
    {
        // Wrap around
        rng->currOffset = 0;
    }

    void* blockPtr = &rng->basePtr[rng->currOffset];

    // Bind for shader access
    // You now write directly into blockPtr and the shader sees the bound block via layout(std140, binding = %).
    if (pOffset)
        *pOffset = rng->currOffset;

    if (pRange)
        *pRange = reqSiz;

    rng->currOffset += reqSiz;

    return blockPtr;
}
