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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_ICD_H
#define AVX_ICD_H

#include "../afxSystemDDK.h"
#include "../afxIoDDK.h"

#ifdef _AFX_DEBUG
// hardcoded validation controls
#define AVX_VALIDATION_ENABLED TRUE
#define AVX_LIMIT_VALIDATION_ENABLED TRUE
#define AVX_FEATURE_VALIDATION_ENABLED TRUE
#endif

#include "afxDrawSystemDDK.h"
#include "afxDrawDeviceDDK.h"
#include "dpu/avxDpuTransference.h"
#include "dpu/avxDpuExecutor.h"

AFX_DECLARE_STRUCT(_avxIddIcd);

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDdiIcd);
#else
AFX_DEFINE_STRUCT(_avxDdiIcd)
{
    afxError(*cfgDsysCb)(afxModule, avxSystemConfig*);
    afxError(*acqDsysCb)(afxModule, avxSystemConfig const*, afxDrawSystem*);
    afxClass const*(*getDsysClsCb)(afxModule icd);
};
#endif

AFX_DEFINE_STRUCT(_avxImplementation)
{
    afxModule icd;
    afxClassConfig ddevCls;
    afxClassConfig dsysCls;
};

AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL];

AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd);
AVX afxClass const* _AvxIcdGetDsysClass(afxModule icd);

AVX afxError _AuxRegisterDisplays(afxModule icd, afxUnit cnt, afxDisplayConfig const cfg[], afxDisplay displays[]);
AVX afxError _AvxIcdRegisterDevices(afxModule icd, afxUnit cnt, _avxDdevReg const infos[], afxDrawDevice devices[]);
AVX afxError _AvxIcdImplement(afxSystem sys, _avxImplementation const* cfg);

AVX afxBool _AvxGetIcd(afxSystem sys, afxUnit icdIdx, afxModule* driver);

AFX afxError AfxGetAvx(afxUnit unit, afxModule* avxIcd);

AVX afxError _AvxIcdConfigureDsysSW(afxModule avxIcd, avxSystemConfig* cfg);

AVX afxError _AvxIcdEstablishDsysSW(afxModule avxIcd, avxSystemConfig const* cfg, afxDrawSystem* system);

AVX _avxDdiIcd const* _AvxGetDdi(afxModule avxIcd);

#endif//AVX_ICD_H
