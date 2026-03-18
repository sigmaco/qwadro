/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions.

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AFX_DEVICE_C
#define _ACX_WARP_C
#define _ACX_WARP_DEVICE_C
#include "acxIcd.h"

_ACX afxString const acxSignature = AFX_STRING(
    "      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
    "     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
    "     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
    "     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
    "     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
    "     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
    "      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
    "                                                                               \n"
    "      Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E      \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_ACX afxClass const* _AcxIcdGetSdevClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ACX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
    return cls;
}

_ACX afxClass const* _AcxIcdGetSsysClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ACX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.ssysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
    return cls;
}

_ACX afxError _AcxIcdImplement(afxModule icd, _acxImplementation const* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    //AFX_ASSERT((mdevCls && msysCls));

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    clsCfg = cfg->sdevCls.fcc ? cfg->sdevCls : _ACX_SDEV_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SDEV);
    AFX_ASSERT(clsCfg.fixedSiz >= _ACX_SDEV_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SDEV) ||
        (_ACX_SDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        return err;
    }
    else if (AfxMountClass(&icd->icd.sdevCls, (afxClass*)_AfxSysGetDevClass(sys), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        return err;
    }

    clsCfg = cfg->ssysCls.fcc ? cfg->ssysCls : _ACX_SSYS_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SSYS);
    AFX_ASSERT(clsCfg.fixedSiz >= _ACX_SSYS_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SSYS) ||
        (_ACX_SSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.sdevCls);
        return err;
    }
    else if (AfxMountClass(&icd->icd.ssysCls, NIL, &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        AfxDismountClass(&icd->icd.sdevCls);
        return err;
    }

    AfxPushLink(&icd->icd.acx, &sys->acxIcdChain);
    icd->flags |= afxModuleFlag_ACX;

    if (err)
    {
        AfxDismountClass(&icd->icd.sdevCls);
        AfxDismountClass(&icd->icd.ssysCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ACX));

    return err;
}

_ACX afxBool _AcxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = { 0 };
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->acxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->acxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.acx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ACX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_ACX afxError acxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig sdevClsCfg = _ACX_SDEV_CLASS_CONFIG;
    afxClassConfig ssysClsCfg = _ACX_SSYS_CLASS_CONFIG;

    _acxImplementation mimpl = { 0 };
    mimpl.sdevCls = _ACX_SDEV_CLASS_CONFIG;
    mimpl.ssysCls = _ACX_SSYS_CLASS_CONFIG;

    if (_AcxIcdImplement(icd, &mimpl))
    {
        AfxThrowError();
        return err;
    }

    static acxFeatures features = { 0 };

    // In Qwadro AFX, I used to name things based on bad experiences solving issues.
    // Vaio has been named after a bad experience making way for interop with a mix system.
    // Targa has been named after a bad experience handling graphical operations with images originating from left/bottom side.
    // Kurwa has been named after a bad experience when implementing curve-based motion in simulation framework.

    _acxSdevReg sdevInfos[] =
    {
        {
            .dev.urn = AFX_STRING("spu-dma"),
            .dev.type = afxDeviceType_PRAGMA,

            .features = features,

            .capabilities = acxAptitude_DMA,
            .minQueCnt = 2,
            .maxQueCnt = 16,
        },
    };

    afxWarpDevice sdevices[ARRAY_SIZE(sdevInfos)];

    if (_AcxIcdRegisterDevices(icd, ARRAY_SIZE(sdevInfos), sdevInfos, sdevices))
    {
        AfxThrowError();
    }

    return err;
}
