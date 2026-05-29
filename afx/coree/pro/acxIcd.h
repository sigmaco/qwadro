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

#ifndef ACX_ICD_H
#define ACX_ICD_H

#include "../afxSystemDDK.h"
#include "qwadro/pro/afxWarpSystem.h"
#include "spu/acxSpuExecutor.h"
#include "spu/acxSpuTransference.h"
#include "afxWarpSystemDDK.h"
#include "afxWarpDeviceDDK.h"

AFX_DECLARE_STRUCT(_acxIddIcd);

#ifndef _ACX_WARP_C
AFX_DECLARE_STRUCT(_acxDdiIcd);
#else
AFX_DEFINE_STRUCT(_acxDdiIcd)
{
    afxError(*cfgSsysCb)(afxModule, acxSystemConfig*);
    afxError(*acqSsysCb)(afxModule, acxSystemConfig const*, afxWarpSystem*);
    afxClass const*(*getSsysClsCb)(afxModule icd);
};
#endif

AFX_DEFINE_STRUCT(_acxImplementation)
{
    afxClassConfig sdevCls;
    afxClassConfig ssysCls;
};

ACX afxClass const* _AcxIcdGetSdevClass(afxModule icd);
ACX afxClass const* _AcxIcdGetSsysClass(afxModule icd);

ACX afxError _AcxIcdImplement(afxModule icd, _acxImplementation const* cfg);

ACX afxBool _AcxGetIcd(afxSystem sys, afxUnit icdIdx, afxModule* driver);

AFX afxError AcxGetIcd(afxUnit unit, afxModule* acxIcd);

ACX afxError _AcxIcdConfigureSsysSW(afxModule acxIcd, acxSystemConfig* cfg);

ACX afxError _AcxIcdEstablishSsysSW(afxModule acxIcd, acxSystemConfig const* cfg, afxWarpSystem* system);

ACX _acxDdiIcd const* _AcxGetDdi(afxModule acxIcd);


#endif//ACX_ICD_H
