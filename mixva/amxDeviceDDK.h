/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_DEVICE_DDK_H
#define AMX_DEVICE_DDK_H

#include "../coree/exec/afxSystemDDK.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioDDK.h"
#include "amxContextDDK.h"
#include "mpu/amxMpuExecutor.h"
#include "amxSoundscapeDDK.h"
#include "amxSystemDDK.h"

AFX_DEFINE_STRUCT(_amxMdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    amxAptitude         capabilities;
    amxFeatures         features;
    amxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
};

AFX_DECLARE_STRUCT(_amxIddMdev);
AFX_DEFINE_STRUCT(_amxDdiMdev)
{

};

#ifdef _AMX_MIX_DEVICE_C
#ifdef _AMX_MIX_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_amxMixDevice)
#else
AFX_OBJECT(afxMixDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _amxDdiMdev const*  ddi;
    _amxIddMdev*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxAcceleration     acceleration;
    amxAptitude         capabilities;
    amxFeatures         features;
    amxLimits           limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion
};
#endif//_AMX_MIX_DEVICE_C

AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG;

AMX void*           _AmxMdevGetIdd(afxMixDevice mdev);

AMX amxLimits const* _AmxMdevGetLimits(afxMixDevice mdev);

AMX afxError _AmxIcdRegisterDevices(afxModule icd, afxUnit cnt, _amxMdevReg const infos[], afxMixDevice devices[]);

#endif//AMX_DEVICE_DDK_H
