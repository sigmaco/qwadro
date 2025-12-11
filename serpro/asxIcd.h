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

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_ICD_H
#define ASX_ICD_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/pro/afxStepSystem.h"
#include "asxSpuExecutor.h"
#include "asxSpuTransference.h"
#include "asxSystemDDK.h"
#include "asxDeviceDDK.h"

AFX_DEFINE_STRUCT(_asxImplementation)
{
    afxClassConfig sdevCls;
    afxClassConfig ssysCls;
};

ASX afxBool _AsxGetIcd(afxUnit icdIdx, afxModule* driver);

ASX afxClass const* _AsxIcdGetSdevClass(afxModule icd);
ASX afxClass const* _AsxIcdGetSsysClass(afxModule icd);



ASX afxError _AsxIcdImplement(afxModule icd, _asxImplementation const* cfg);

#endif//ASX_ICD_H
