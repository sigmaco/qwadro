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

#ifndef AMX_ICD_H
#define AMX_ICD_H

#include "../coree/exec/afxSystemDDK.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioDDK.h"
#include "amxContextDDK.h"
#include "mpu/amxMpuAudio.h"
#include "mpu/amxMpuExecutor.h"
#include "mpu/amxMpuContext.h"
#include "mpu/amxMpuSoundscape.h"
#include "mpu/amxMpuTransference.h"
#include "amxSoundscapeDDK.h"
#include "amxSystemDDK.h"
#include "amxSinkDDK.h"
#include "amxDeviceDDK.h"
#include "amxAudioDDK.h"
#include "amxTrackerDDK.h"

AFX_DEFINE_STRUCT(_amxImplementation)
{
    afxClassConfig mcdcCls;
    afxClassConfig mdevCls;
    afxClassConfig msysCls;
};

AFX_DEFINE_STRUCT(_amxCodecReg)
{

};

AMX afxBool _AmxGetIcd(afxUnit icdIdx, afxModule* driver);

AMX afxClass const* _AmxIcdGetAsiClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMdevClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMcdcClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMsysClass(afxModule icd);



AMX afxError _AmxIcdImplement(afxModule icd, _amxImplementation const* cfg);
AMX afxError _AmxIcdRegisterCodecs(afxModule icd, afxUnit cnt, _amxCodecReg const infos[], amxCodec codecs[]);

AMX afxError _AmxIcdRegisterAsis(afxModule icd, afxUnit cnt, _amxMdevReg const infos[], afxDevice devices[]);

#endif//AMX_ICD_H
