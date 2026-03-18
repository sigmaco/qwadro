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
#include "arxIcd.h"

afxBool arxInited = FALSE;
_ARX afxModule e2mirageDll = NIL;
_ARX afxChain AFX_STATIC_CHAIN(arxIcdChain);

ARX afxError afxScmHook(afxModule mdle, afxManifest const* ini);

_ARX afxError _ArxAmendEcosystem(afxSystem sys)
{
    afxError err = { 0 };

    if (arxInited)
        return afxError_NONE;

    //AfxMakeChain(&arxIcdChain, NIL);
    arxInited = TRUE;

    afxUri uri;
    AfxMakeUri(&uri, 0, "e2mirage", 0);

    if (AfxAcquireModule(&uri, AFX_BITMASK(8), &e2mirageDll))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2mirageDll);

        //afxScmHook(e2mirageDll, NIL);
    }

    return err;
}

_ARX afxClass const* _ArxIcdGetScioClass(afxModule icd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ARX))
    {
        AfxThrowError();
        return NIL;
    }
    afxClass const* cls = &icd->icd.scioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCIO);
    return cls;
}

_ARX afxError _ArxIcdImplement(afxModule icd, afxClassConfig const* scioCls)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(scioCls);

    if (!AfxTestModule(icd, afxModuleFlag_ICD))
    {
        AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD));
        AfxThrowError();
        return NIL;
    }

    afxSystem sys;
    AfxGetSystem(&sys);

    afxClassConfig clsCfg = scioCls ? *scioCls : _ARX_SCIO_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_SCIO);
    AFX_ASSERT(clsCfg.fixedSiz >= _ARX_SCIO_CLASS_CONFIG.fixedSiz);
    if ((clsCfg.fcc != afxFcc_SCIO) ||
        (_ARX_SCIO_CLASS_CONFIG.fixedSiz > clsCfg.fixedSiz))
    {
        AfxThrowError();
    }
    else if (AfxMountClass(&icd->icd.scioCls, NIL, &icd->classes, &clsCfg))
    {
        AfxThrowError();
    }
    else
    {
        afxSystem sys;
        AfxGetSystem(&sys);
        AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
        AfxPushLink(&icd->icd.arx, &arxIcdChain);
        icd->flags |= afxModuleFlag_ARX;
    }

    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ARX));

    return err;
}

_ARX afxBool _ArxGetIcd(afxUnit icdIdx, afxModule* driver)
{
    afxError err = { 0 };
    afxBool found = FALSE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    if (_ArxAmendEcosystem(sys))
    {
        AfxThrowError();
        return FALSE;
    }

    afxModule icd = NIL;
    while ((icdIdx < arxIcdChain.cnt) && (icd = AFX_REBASE(AfxFindFirstLink(&arxIcdChain, icdIdx), AFX_OBJ(afxModule), icd.arx)))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

        if (AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ARX))
            found = TRUE;
        else
            icd = NIL;

        break;
    }
    AFX_ASSERT(driver);
    *driver = icd;
    return found;
}

_ARX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
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

    _AfxScanIcdManifests(sys, &AFX_STRING("ARX"), ini);

    return err;
}

#if 0
afxInt32 __stdcall DllMain(
    void* hinstDLL,  // handle to DLL module
    afxUnit32 fdwReason,     // reason for calling function
    void* lpvReserved)  // reserved
{
    afxError err = { 0 };
    

    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case 1:
    {
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.

        afxUri uri;
        AfxMakeUri(&uri, 0, "e2mirage", 0);
        
        if (AfxAcquireModule(&uri, AFX_BITMASK(8), &e2simDll))
        {
            //AfxThrowError();
            //err = 0;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2simDll);
            afxScmHook(e2simDll, NIL);
        }

        break;
    }
    case 2:
        // Do thread-specific initialization.
        break;

    case 3:
        // Do thread-specific cleanup.
        break;

    case 0:

        if (lpvReserved != NIL)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.

        AfxDisposeObjects(1, &e2simDll);

        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif
