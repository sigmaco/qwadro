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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_ICD_H
#define AMX_ICD_H

#include "../afxSystemDDK.h"
#include "qwadro/mix/afxMixSystem.h"
#include "amxAudioDDK.h"
#include "afxMixContextDDK.h"
#include "mpu/amxMpuAudio.h"
#include "mpu/amxMpuExecutor.h"
#include "mpu/amxMpuContext.h"
#include "mpu/amxMpuSoundscape.h"
#include "mpu/amxMpuTransference.h"
#include "amxSoundscapeDDK.h"
#include "afxMixSystemDDK.h"
#include "afxSinkDDK.h"
#include "afxMixDeviceDDK.h"
#include "amxAudioDDK.h"
#include "amxTrackerDDK.h"

AFX_DECLARE_STRUCT(_amxIddIcd);

#ifndef _AMX_MIX_C
AFX_DECLARE_STRUCT(_amxDdiIcd);
#else
AFX_DEFINE_STRUCT(_amxDdiIcd)
{
    afxError(*cfgMsysCb)(afxModule, amxSystemConfig*);
    afxError(*acqMsysCb)(afxModule, amxSystemConfig const*, afxMixSystem*);
    afxClass const*(*getMsysClsCb)(afxModule icd);
};
#endif

AFX_DEFINE_STRUCT(_amxImplementation)
{
    afxModule icd;
    afxClassConfig mcdcCls;
    afxClassConfig mdevCls;
    afxClassConfig msysCls;
};

AFX_DEFINE_STRUCT(_amxCodecReg)
{

};

AMX afxBool _AmxGetIcd(afxSystem sys, afxUnit icdIdx, afxModule* driver);

AMX afxClass const* _AmxIcdGetAsiClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMdevClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMcdcClass(afxModule icd);
AMX afxClass const* _AmxIcdGetMsysClass(afxModule icd);

AFX afxError AmxGetIcd(afxUnit unit, afxModule* amxIcd);

AMX afxError _AmxIcdConfigureMsysSW(afxModule amxIcd, amxSystemConfig* cfg);

AMX afxError _AmxIcdEstablishMsysSW(afxModule amxIcd, amxSystemConfig const* cfg, afxMixSystem* system);

AMX _amxDdiIcd const* _AmxGetDdi(afxModule amxIcd);

AMX afxError _AmxIcdImplement(afxSystem sys, _amxImplementation const* cfg);
AMX afxError _AmxIcdRegisterCodecs(afxModule icd, afxUnit cnt, _amxCodecReg const infos[], amxCodec codecs[]);

AMX afxError _AmxIcdRegisterAsis(afxModule icd, afxUnit cnt, _amxMdevAcq const infos[], afxDevice devices[]);

#endif//AMX_ICD_H
