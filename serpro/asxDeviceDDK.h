/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_DEVICE_DDK_H
#define ASX_DEVICE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/pro/afxStepSystem.h"
#include "asxSystemDDK.h"

AFX_DEFINE_STRUCT(_asxSdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    asxAptitude         capabilities;
    asxFeatures         features;
    asxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};

AFX_DECLARE_STRUCT(_asxIddSdev);
AFX_DEFINE_STRUCT(_asxDdiSdev)
{
    int a;
};

#ifdef _ASX_STEP_DEVICE_C
#ifdef _ASX_STEP_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_asxStepDevice)
#else
AFX_OBJECT(afxStepDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _asxDdiSdev const*  ddi;
    _asxIddSdev*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxAcceleration     acceleration;
    asxAptitude         capabilities;
    asxFeatures         features;
    asxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion
};
#endif//_ASX_STEP_DEVICE_C

ASX afxClassConfig const _ASX_SDEV_CLASS_CONFIG;

ASX void*           _AsxSdevGetIdd(afxStepDevice sdev);

ASX asxLimits const* _AsxSdevGetLimits(afxStepDevice sdev);

ASX afxError _AsxIcdRegisterDevices(afxModule icd, afxUnit cnt, _asxSdevReg const infos[], afxStepDevice devices[]);

#endif//ASX_DEVICE_DDK_H
