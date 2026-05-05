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

AFX_DEFINE_STRUCT(_acxImplementation)
{
    afxClassConfig sdevCls;
    afxClassConfig ssysCls;
};

ACX afxBool _AcxGetIcd(afxUnit icdIdx, afxModule* driver);

ACX afxClass const* _AcxIcdGetSdevClass(afxModule icd);
ACX afxClass const* _AcxIcdGetSsysClass(afxModule icd);



ACX afxError _AcxIcdImplement(afxModule icd, _acxImplementation const* cfg);

#endif//ACX_ICD_H
