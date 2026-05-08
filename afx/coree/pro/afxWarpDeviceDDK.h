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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Parallel Computing Extensions.

#ifndef ACX_DEVICE_DDK_H
#define ACX_DEVICE_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/pro/afxWarpSystem.h"
#include "afxWarpSystemDDK.h"

AFX_DEFINE_STRUCT(_acxSdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    acxService         capabilities;
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

#ifdef _ACX_WARP_DEVICE_C
#ifdef _ACX_WARP_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_acxStepDevice)
#else
AFX_OBJECT(afxWarpDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _acxDdiSdev const*  ddi;
    _acxIddSdev*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxAcceleration     acceleration;
    acxService         capabilities;
    acxFeatures         features;
    acxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion
};
#endif//_ACX_WARP_DEVICE_C

ACX afxClassConfig const _ACX_SDEV_CLASS_CONFIG;

ACX void*           _AcxSdevGetIdd(afxWarpDevice sdev);

ACX acxLimits const* _AcxSdevGetLimits(afxWarpDevice sdev);

ACX afxError _AcxIcdRegisterDevices(afxModule icd, afxUnit cnt, _acxSdevReg const infos[], afxWarpDevice devices[]);

#endif//ACX_DEVICE_DDK_H
