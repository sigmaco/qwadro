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

// This software is part of Advanced Multimedia Extensions.

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SYSTEM_C

#define _ACX_STEP_C
#define _ACX_STEP_DEVICE_C
#define _ACX_STEP_SYSTEM_C
#include "acxIcd.h"

AFX afxChain* _AfxGetSystemClassChain(void);

////////////////////////////////////////////////////////////////////////////////
// DEVICE HANDLING                                                            //
////////////////////////////////////////////////////////////////////////////////

_ACX afxBool AcxIsStepDevicePrompt(afxStepDevice sdev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    return sdev->dev.serving;
}

_ACX void AcxQueryStepDeviceLimits(afxStepDevice sdev, acxLimits* limits)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT(limits);
    *limits = sdev->limits;
}

_ACX void AcxQueryStepDeviceFeatures(afxStepDevice sdev, acxFeatures* features)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT(features);
    *features = sdev->features;
}

_ACX void AcxQueryStepCapabilities(afxStepDevice sdev, acxDeviceInfo* caps)
{
    afxError err = { 0 };
    // @sdev must be a valid afxStepDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    //AFX_ASSERT_RANGE(sdev->portCnt, basePortIdx, portCnt);
    //AFX_ASSERT(caps);
    afxUnit rslt = 0;

    if (caps)
    {
        caps->acceleration = sdev->acceleration;
        caps->capabilities = sdev->capabilities;
        caps->maxQueCnt = sdev->maxQueCnt;
        caps->minQueCnt = sdev->minQueCnt;
        caps->udd = sdev->udd;
        caps->tag = sdev->tag;
    }
}

_ACX afxBool AcxIsStepDeviceAcceptable(afxStepDevice sdev, acxFeatures const* features, acxLimits const* limits)
{
    afxError err = { 0 };
    // @sdev must be a valid afxStepDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);
    AFX_ASSERT(limits);
    AFX_ASSERT(features);
    afxBool rslt = TRUE;

    if (features)
    {
        
    }

    if (limits)
    {
        
    }
    return rslt;
}

_ACX _acxDdiSdev const _ACX_DDI_SDEV =
{
    0
};

_ACX afxError _AcxSdevDtorCb(afxStepDevice sdev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

    AfxDeregisterChainedClasses(&sdev->dev.classes);

    AfxCallDevice(&sdev->dev, 2, NIL); // terminate

    AFX_ASSERT(!sdev->idd);
    AfxDeregisterChainedClasses(&sdev->dev.classes);


    return err;
}

_ACX afxError _AcxSdevCtorCb(afxStepDevice sdev, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _acxSdevReg const* info = ((_acxSdevReg const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&sdev->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    sdev->ddi = &_ACX_DDI_SDEV;
    sdev->idd = NIL;
    sdev->udd = info->udd;
    sdev->tag = info->tag;

    sdev->limits = info->limits;
    sdev->features = info->features;

    sdev->acceleration = info->acceleration;
    sdev->capabilities = info->capabilities;

    sdev->maxQueCnt = info->maxQueCnt;
    sdev->minQueCnt = info->minQueCnt;

    AfxMakeString128(&sdev->desc, NIL);
    AfxMakeString8(&sdev->urn, NIL);

    if (AfxCallDevice(&sdev->dev, afxFcc_SSYS, NIL)) AfxThrowError();
    else
    {
        if (err)
        {
            AfxDeregisterChainedClasses(&sdev->dev.classes);
        }
    }

    if (err)
        _AFX_DEV_CLASS_CONFIG.dtor(&sdev->dev);

    return err;
}

_ACX afxClassConfig const _ACX_SDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_SDEV,
    .name = "StepDevice",
    .desc = "Step Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxStepDevice)),
    .ctor = (void*)_AcxSdevCtorCb,
    .dtor = (void*)_AcxSdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_ACX afxUnit AcxInvokeStepDevices(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxStepDevice), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));
    
    afxClass const* cls = _AcxIcdGetSdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
    
    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_ACX afxUnit AcxEvokeStepDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxStepDevice), afxUnit cnt, afxStepDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));

    afxClass const* cls = _AcxIcdGetSdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);

    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, rslt, devices);

    return rslt;
}

_ACX afxUnit AcxEnumerateStepDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxStepDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));
    
    afxClass const* cls = _AcxIcdGetSdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);
    
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
    AFX_ASSERT_OBJECTS(afxFcc_SDEV, rslt, devices);

    return rslt;
}

_ACX afxUnit AcxChooseStepDevices(afxUnit icd, acxDeviceInfo const* caps, acxFeatures const* features, acxLimits const* limits, afxUnit maxCnt, afxUnit sdevId[])
{
    afxError err = { 0 };
    //AFX_ASSERT(limits);
    //AFX_ASSERT(features);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AcxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_ACX) == (afxModuleFlag_ICD | afxModuleFlag_ACX));

    afxClass const* cls = _AcxIcdGetSdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_SDEV);

    afxStepDevice sdev;
    for (afxUnit i = 0; TRUE; i++)
    {
        if (!AfxEnumerateObjects(cls, i, 1, (afxObject*)&sdev))
            break;
        AFX_ASSERT_OBJECTS(afxFcc_SDEV, 1, &sdev);

        if (features || limits)
            if (!AcxIsStepDeviceAcceptable(sdev, features, limits))
                continue;

        if (caps)
        {
            if ((sdev->capabilities & caps->capabilities) != caps->capabilities)
                continue;

            if ((sdev->acceleration & caps->acceleration) != caps->acceleration)
                continue;

            if (sdev->minQueCnt < caps->minQueCnt)
                continue;

            if (sdev->maxQueCnt < caps->maxQueCnt)
                continue;
        }

        {
            sdevId[rslt] = i;
            ++rslt;

            if (maxCnt > rslt)
                break;
        }
    }
    return rslt;
}

_ACX afxError _AcxIcdRegisterDevices(afxModule icd, afxUnit cnt, _acxSdevReg const infos[], afxStepDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_ACX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AcxIcdGetSdevClass(icd);
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
