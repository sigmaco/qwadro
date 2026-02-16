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

#ifndef ACX_ICD_H
#define ACX_ICD_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/pro/afxWarpSystem.h"
#include "spu/acxSpuExecutor.h"
#include "spu/acxSpuTransference.h"
#include "acxSystemDDK.h"
#include "acxDeviceDDK.h"

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
