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

// This file is part of Advanced RenderWare Extensions.

#define _AFX_SYSTEM_C
#define _AFX_MODULE_C
#define _AFX_SIM_C
#include "asxImplementation.h"

AFX afxChain* _AfxGetSystemClassChain(void);

_ASX afxClass const* _AsxGetEngineClass(afxModule icd)
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

_ASX afxClass const* _AsxGetSimulationClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.simCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SIM);
    return cls;
}

_ASX afxError _AsxRegisterEngines(afxModule icd, afxUnit cnt, afxEngineInfo const infos[], arxEngine devices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ASX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AsxGetEngineClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, cnt, devices);
    }
    return err;
}

_ASX afxError _AsxImplementSimulation(afxModule icd, afxClassConfig const* sdevCls, afxClassConfig const* simCls)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT((sdevCls && simCls));

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxClassConfig clsCfg;

    afxSystem sys;
    AfxGetSystem(&sys);

    if (!err)
    {
        clsCfg = sdevCls ? *sdevCls : _ASX_SDEV_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SDEV);
        AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SDEV_CLASS_CONFIG.fixedSiz);
        if ((clsCfg.fcc != afxFcc_SDEV) ||
            (_ASX_SDEV_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
        {
            AfxThrowError();
        }
        else if (AfxMountClass(&icd->icd.rdevCls, (afxClass*)_AfxSysGetDevClass(sys), &icd->classes, &clsCfg)) // require base*
        {
            AfxThrowError();
        }
        else
        {
            clsCfg = simCls ? *simCls : _ASX_SIM_CLASS_CONFIG;
            AFX_ASSERT(clsCfg.fcc == afxFcc_SIM);
            AFX_ASSERT(clsCfg.fixedSiz >= _ASX_SIM_CLASS_CONFIG.fixedSiz);
            if ((clsCfg.fcc != afxFcc_SIM) ||
                (_ASX_SIM_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
            {
                AfxThrowError();
            }
            else if (AfxMountClass(&icd->icd.simCls, NIL, &icd->classes, &clsCfg)) // require base*
            {
                AfxThrowError();
            }
            else
            {
                afxSystem sys;
                AfxGetSystem(&sys);
                AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
                AfxPushLink(&icd->icd.arx, &sys->asxIcdChain);
                icd->flags |= afxModuleFlag_ASX;
            }
        }
    }

    if (err)
    {
        AfxDismountClass(&icd->icd.simCls);
        AfxDismountClass(&icd->icd.rdevCls);
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

_ASX afxError afxScmHook_(afxModule mdle, afxManifest const* ini)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    //AfxMountClass(&sys->asx.sdevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_ASX_SDEV_CLASS_CONFIG); // require base*

    if (!err)
    {
        //sys->asx.ready = TRUE;
    }

    return err;
}
