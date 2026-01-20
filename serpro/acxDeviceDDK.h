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

// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_DEVICE_DDK_H
#define ACX_DEVICE_DDK_H

#include "../coree/exec/afxSystemDDK.h"
#include "qwadro/pro/afxStepSystem.h"
#include "acxSystemDDK.h"

AFX_DEFINE_STRUCT(_acxSdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    acxAptitude         capabilities;
    acxFeatures         features;
    acxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};

AFX_DECLARE_STRUCT(_acxIddSdev);
AFX_DEFINE_STRUCT(_acxDdiSdev)
{
    int a;
};

#ifdef _ACX_STEP_DEVICE_C
#ifdef _ACX_STEP_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_acxStepDevice)
#else
AFX_OBJECT(afxStepDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _acxDdiSdev const*  ddi;
    _acxIddSdev*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxAcceleration     acceleration;
    acxAptitude         capabilities;
    acxFeatures         features;
    acxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion
};
#endif//_ACX_STEP_DEVICE_C

ACX afxClassConfig const _ACX_SDEV_CLASS_CONFIG;

ACX void*           _AcxSdevGetIdd(afxStepDevice sdev);

ACX acxLimits const* _AcxSdevGetLimits(afxStepDevice sdev);

ACX afxError _AcxIcdRegisterDevices(afxModule icd, afxUnit cnt, _acxSdevReg const infos[], afxStepDevice devices[]);

#endif//ACX_DEVICE_DDK_H
