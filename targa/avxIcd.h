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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_ICD_H
#define AVX_ICD_H

#include "../coree/exec/afxSystemDDK.h"
#include "../coree/io/afxIoDDK.h"

#ifdef _AFX_DEBUG
// hardcoded validation controls
#define AVX_VALIDATION_ENABLED TRUE
#define AVX_LIMIT_VALIDATION_ENABLED TRUE
#define AVX_FEATURE_VALIDATION_ENABLED TRUE
#endif

#include "avxSystemDDK.h"
#include "avxDeviceDDK.h"
#include "dpu/avxDpuTransference.h"
#include "dpu/avxDpuExecutor.h"

AFX_DEFINE_STRUCT(_avxImplementation)
{
    afxClassConfig ddevCls;
    afxClassConfig dsysCls;
};

AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd);
AVX afxClass const* _AvxIcdGetDsysClass(afxModule icd);

AVX afxError _AuxRegisterDisplays(afxModule icd, afxUnit cnt, afxDisplayConfig const cfg[], afxDisplay displays[]);
AVX afxError _AvxIcdRegisterDevices(afxModule icd, afxUnit cnt, _avxDdevReg const infos[], afxDrawDevice devices[]);
AVX afxError _AvxIcdImplement(afxModule icd, _avxImplementation const* cfg);

AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver);

AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL];

#endif//AVX_ICD_H
