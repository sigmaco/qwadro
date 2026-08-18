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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced User Experience Extensions.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_ICD_H
#define AUX_ICD_H

#include "qwadro/ux/afxShell.h"
#include "afxEnvironmentDDK.h"
#include "../hid/afxHidDDK.h"
#include "../vga/dma/afxFontDDK.h"
#include "afxWidgetDDK.h"
#include "afxWindowDDK.h"
#include "afxShellDDK.h"
#include "../vga/afxDisplayDDK.h"

AFX_DECLARE_STRUCT(_auxIddIcd);

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxDdiIcd);
#else
AFX_DEFINE_STRUCT(_auxDdiIcd)
{
    afxError(*cfgEnvCb)(afxModule, afxEnvironmentConfig*);
    afxError(*acqEnvCb)(afxModule, afxEnvironmentConfig const*, afxEnvironment*);
    afxClass const*(*getEnvClsCb)(afxModule icd);
};
#endif

AFX_DEFINE_STRUCT(_auxImplementation)
{
    afxModule icd;
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

AUX afxError _AuxIcdImplement(afxSystem sys, _auxImplementation const* cfg);

AUX afxBool _AuxGetIcd(afxSystem sys, afxUnit icdIdx, afxModule* auxIcd);

AFX afxError AfxGetShell(afxUnit unit, afxModule* auxIcd);

AUX afxError _AuxIcdSwConfigureEnvCb(afxModule auxIcd, afxEnvironmentConfig* cfg);

AUX afxError _AuxIcdSwEstablishEnvCb(afxModule auxIcd, afxEnvironmentConfig const* cfg, afxEnvironment* environment);

AUX _auxDdiIcd const* _AuxGetDdi(afxModule auxIcd);

#endif//AUX_ICD_H
