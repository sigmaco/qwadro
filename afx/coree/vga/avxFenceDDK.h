/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_FENCE_DDK_H
#define AVX_FENCE_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/vga/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxFencIdd);

#ifdef _AVX_FENCE_C

AFX_DEFINE_STRUCT(_avxFencDdi)
{
    afxError(*waitCb)(avxFence, afxUnit64 value, afxUnit64 timeout);
    afxError(*resetCb)(avxFence);
    afxError(*signalCb)(avxFence, afxUnit64 value);
    afxUnit64(*evalCb)(avxFence);
};

#ifdef _AVX_FENCE_IMPL
AFX_OBJECT(_avxFence)
#else
AFX_OBJECT(avxFence)
#endif
{
    _avxFencDdi const*  ddi;
    _avxFencIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxFenceFlags       flags;
    afxAtom32           signaled;
    afxAtom64           value;
    afxAtom32           signalQueuedCnt;
    afxAtom32           waitQueuedCnt;
};
#endif//_AVX_FENCE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_FENC;


AVX afxError _AvxDsysSW_WaitForFencesCb(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[]);

AVX afxUnit64 _AvxFencSW_GetValueCb(avxFence fenc);
AVX afxError _AvxFencSW_SignalCb(avxFence fenc, afxUnit64 value);
AVX afxError _AvxFencSW_WaitCb(avxFence fenc, afxUnit64 value, afxUnit64 timeout);

#endif//AVX_FENCE_DDK_H
