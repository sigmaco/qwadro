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

#ifndef AFX_FENCE_DDK_H
#define AFX_FENCE_DDK_H

#include "afxSystemDDK.h"
#include "qwadro/afxIommu.h"

AFX_DECLARE_STRUCT(_afxFencIdd);

#ifdef _AFX_FENCE_C

AFX_DEFINE_STRUCT(_afxFencDdi)
{
    afxError(*waitCb)(afxFence, afxUnit64 value, afxUnit64 timeout);
    afxError(*resetCb)(afxFence);
    afxError(*signalCb)(afxFence, afxUnit64 value);
    afxUnit64(*evalCb)(afxFence);
};

#ifdef _AFX_FENCE_IMPL
AFX_OBJECT(_afxFence)
#else
AFX_OBJECT(afxFence)
#endif
{
    _afxFencDdi const*  ddi;
    _afxFencIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxFenceFlags       flags;
    afxAtom32           signaled;
    afxAtom64           value;
    afxAtom32           signalQueuedCnt;
    afxAtom32           waitQueuedCnt;
};
#endif//_AFX_FENCE_C

AFX afxClassConfig const _AFX_CLASS_CONFIG_FENC;


AFX afxError _AfxIom_WaitForFencesCbSW(afxIommu iom, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, afxFence const fences[], afxUnit64 const values[]);

AFX afxUnit64 _AfxFenc_GetValueCbSW(afxFence fenc);
AFX afxError _AfxFenc_SignalCbSW(afxFence fenc, afxUnit64 value);
AFX afxError _AfxFenc_WaitCbSW(afxFence fenc, afxUnit64 value, afxUnit64 timeout);

#endif//AFX_FENCE_DDK_H
