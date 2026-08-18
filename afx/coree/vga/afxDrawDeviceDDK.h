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

#ifndef AVX_DEVICE_DDK_H
#define AVX_DEVICE_DDK_H

#include "afxDrawBridgeDDK.h"

AFX_DEFINE_STRUCT(_avxDdevReg)
{
    afxDeviceInfo       dev;
    avxService          caps;
    afxAcceleration     accel;
    avxFeatures         features;
    avxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxBool             nonRhcs;
    avxClipSpaceDepth   clipSpaceDepth;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};

AFX_DECLARE_STRUCT(_avxDdevIdd);
AFX_DEFINE_STRUCT(_avxDdevDdi)
{
    afxError(*descFmtCb)(afxDrawDevice, afxUnit cnt, avxFormat const[], avxFormatDescription[]);
    afxBool(*isAcceptCb)(afxDrawDevice, avxFeatures const*, avxLimits const*);
};

#ifdef _AVX_DRAW_DEVICE_C
#ifdef _AVX_DRAW_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_avxDrawDevice)
#else
AFX_OBJECT(afxDrawDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _avxDdevDdi const*  ddi;
    _avxDdevIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxService          caps;
    afxAcceleration     accel;
    avxFeatures         features;
    avxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion

    afxBool             nonRhcs;
    avxClipSpaceDepth   clipSpaceDepth;

    avxFormatDescription const*pfds;

    afxChain            ineps;
};
#endif//_AVX_DRAW_DEVICE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_DDEV;

AVX void*           _AvxDdevGetIdd(afxDrawDevice ddev);

AVX avxLimits const* _AvxDdevGetLimits(afxDrawDevice ddev);

#endif//AVX_DEVICE_DDK_H
