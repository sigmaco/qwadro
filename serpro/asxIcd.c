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
// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AFX_DEVICE_C
#define _ASX_STEP_C
#define _ASX_STEP_DEVICE_C
#include "asxIcd.h"

_ASX afxString const asxSignature = AFX_STRING(
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

_ASX afxClass const* _AsxIcdGetSdevClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.sdevCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
    return cls;
}

_ASX afxClass const* _AsxIcdGetSsysClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.ssysCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SSYS);
    return cls;
}

_ASX afxError _AsxIcdImplement(afxModule icd, _asxImplementation const* cfg)
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

    clsCfg = cfg->sdevCls.fcc ? cfg->sdevCls : _ASX_SDEV_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SDEV);
    AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SDEV_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SDEV) ||
        (_ASX_SDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
        return err;
    }
    else if (AfxMountClass(&icd->icd.sdevCls, (afxClass*)_AfxSysGetDevClass(sys), &icd->classes, &clsCfg)) // require base*
    {
        AfxThrowError();
        return err;
    }

    clsCfg = cfg->ssysCls.fcc ? cfg->ssysCls : _ASX_SSYS_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SSYS);
    AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SSYS_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SSYS) ||
        (_ASX_SSYS_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
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

    AfxPushLink(&icd->icd.asx, &sys->asxIcdChain);
    icd->flags |= afxModuleFlag_ASX;

    if (err)
    {
        AfxDismountClass(&icd->icd.sdevCls);
        AfxDismountClass(&icd->icd.ssysCls);
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX));

    return err;
}

_ASX afxBool _AsxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = { 0 };
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule icd = NIL;
    while ((icdIdx < sys->asxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&sys->asxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.asx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_ASX afxError asxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig sdevClsCfg = _ASX_SDEV_CLASS_CONFIG;
    afxClassConfig ssysClsCfg = _ASX_SSYS_CLASS_CONFIG;

    _asxImplementation mimpl = { 0 };
    mimpl.sdevCls = _ASX_SDEV_CLASS_CONFIG;
    mimpl.ssysCls = _ASX_SSYS_CLASS_CONFIG;

    if (_AsxIcdImplement(icd, &mimpl))
    {
        AfxThrowError();
        return err;
    }

    static asxFeatures features = { 0 };

    // In Qwadro AFX, I used to name things based on bad experiences solving issues.
    // Vaio has been named after a bad experience making way for interop with a mix system.
    // Targa has been named after a bad experience handling graphical operations with images originating from left/bottom side.
    // Kurwa has been named after a bad experience when implementing curve-based motion in simulation framework.

    _asxSdevReg sdevInfos[] =
    {
        {
            .dev.urn = AFX_STRING("spu-dma"),
            .dev.type = afxDeviceType_PRAGMA,

            .features = features,

            .capabilities = asxAptitude_DMA,
            .minQueCnt = 2,
            .maxQueCnt = 16,
        },
    };

    afxStepDevice sdevices[ARRAY_SIZE(sdevInfos)];

    if (_AsxIcdRegisterDevices(icd, ARRAY_SIZE(sdevInfos), sdevInfos, sdevices))
    {
        AfxThrowError();
    }

    return err;
}
