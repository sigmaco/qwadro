/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experience Extensions.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ICD_H
#define AUX_ICD_H

#include "qwadro/ux/afxShell.h"
#include "auxEnvironmentDDK.h"
#include "../hid/auxHidDDK.h"
#include "auxFontDDK.h"
#include "auxWidgetDDK.h"
#include "auxWindowDDK.h"
#include "auxShellDDK.h"
#include "../hid/auxDisplayDDK.h"

AFX_DEFINE_STRUCT(_auxImplementation)
{
    afxClassConfig dpyCls;
    afxClassConfig envCls;
    afxError(*getInteropDoutCls)(afxDrawSystem dsys, afxString const* tool, afxClassConfig* cfg);
    afxError(*getInteropSinkCls)(afxMixSystem msys, afxString const* tool, afxClassConfig* cfg);
};

AUX afxClassConfig const _AUX_SSH_CLASS_CONFIG;

AUX afxError _AuxIcdGetInteropDpyClass(afxUnit icd, afxString const* tool, afxClassConfig* clsc);
AUX afxError _AuxIcdGetInteropDoutClass(afxDrawSystem dsys, afxString const* tool, afxClassConfig* clsc);
AUX afxError _AuxIcdGetInteropSinkClass(afxMixSystem msys, afxString const* tool, afxClassConfig* clsc);

AUX afxClass const* _AuxIcdGetHidClass(afxModule icd);
AUX afxClass const* _AuxIcdGetSshClass(afxModule icd);
AUX afxClass const* _AuxIcdGetDpyClass(afxModule icd);


AUX afxError _AuxRegisterHids(afxModule icd, afxUnit cnt, afxHidInfo const infos[], afxHid hids[]);
AUX afxError _AuxRegisterShells(afxModule icd, afxUnit cnt, _auxShellAcq const infos[], afxShell shells[]);

AUX afxError _AuxIcdImplement(afxModule icd, _auxImplementation const* cfg);

AUX afxBool _AuxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//AUX_ICD_H
