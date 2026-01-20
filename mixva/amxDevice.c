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

#define _AMX_MIX_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_INPUT_C
#define _AMX_SINK_C
#define _AMX_MIX_SYSTEM_C
#include "amxIcd.h"

extern afxClassConfig const _AMX_ASIO_CLASS_CONFIG;
AFX afxChain* _AfxGetSystemClassChain(void);

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE HANDLING                                                      //
////////////////////////////////////////////////////////////////////////////////

_AMX afxBool AmxIsMixDevicePrompt(afxMixDevice mdev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    return mdev->dev.serving;
}

_AMX void AmxQueryMixDeviceLimits(afxMixDevice mdev, amxLimits* limits)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    AFX_ASSERT(limits);
    *limits = mdev->limits;
}

_AMX void AmxQueryMixDeviceFeatures(afxMixDevice mdev, amxFeatures* features)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    AFX_ASSERT(features);
    *features = mdev->features;
}

_AMX void AmxQueryMixCapabilities(afxMixDevice mdev, amxDeviceInfo* caps)
{
    afxError err = { 0 };
    // @mdev must be a valid afxMixDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
    //AFX_ASSERT_RANGE(mdev->portCnt, basePortIdx, portCnt);
    //AFX_ASSERT(caps);
    afxUnit rslt = 0;

    if (caps)
    {
        caps->acceleration = mdev->acceleration;
        caps->capabilities = mdev->capabilities;
        caps->maxQueCnt = mdev->maxQueCnt;
        caps->minQueCnt = mdev->minQueCnt;
        caps->udd = mdev->udd;
        caps->tag = mdev->tag;
    }
}

_AMX afxBool AmxIsMixDeviceAcceptable(afxMixDevice mdev, amxFeatures const* features, amxLimits const* limits)
{
    afxError err = { 0 };
    // @mdev must be a valid afxMixDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);
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

_AMX _amxDdiMdev const _AMX_DDI_MDEV =
{
    0
};

_AMX afxError _AmxMdevDtorCb(afxMixDevice mdev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

    AfxDeregisterChainedClasses(&mdev->dev.classes);

    AfxCallDevice(&mdev->dev, 2, NIL); // terminate

    AFX_ASSERT(!mdev->idd);
    AfxDeregisterChainedClasses(&mdev->dev.classes);


    return err;
}

_AMX afxError _AmxMdevCtorCb(afxMixDevice mdev, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxMdevReg const* info = ((_amxMdevReg const *)args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&mdev->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    mdev->ddi = &_AMX_DDI_MDEV;
    mdev->idd = NIL;
    mdev->udd = info->udd;
    mdev->tag = info->tag;

    mdev->limits = info->limits;
    mdev->features = info->features;

    mdev->acceleration = info->acceleration;
    mdev->capabilities = info->capabilities;

    mdev->maxQueCnt = info->maxQueCnt;
    mdev->minQueCnt = info->minQueCnt;

    AfxMakeString128(&mdev->desc, NIL);
    AfxMakeString8(&mdev->urn, NIL);

    if (AfxCallDevice(&mdev->dev, afxFcc_MSYS, NIL)) AfxThrowError();
    else
    {
        if (err)
        {
            AfxDeregisterChainedClasses(&mdev->dev.classes);
        }
    }

    if (err)
        _AFX_DEV_CLASS_CONFIG.dtor(&mdev->dev);

    return err;
}

_AMX afxClassConfig const _AMX_MDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_MDEV,
    .name = "MixDevice",
    .desc = "Mix Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixDevice)),
    .ctor = (void*)_AmxMdevCtorCb,
    .dtor = (void*)_AmxMdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AmxInvokeMixDevices(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxMixDevice), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
    
    afxClass const* cls = _AmxIcdGetMdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    
    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_AMX afxUnit AmxEvokeMixDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*,afxMixDevice), afxUnit cnt, afxMixDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxClass const* cls = _AmxIcdGetMdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, rslt, devices);

    return rslt;
}

_AMX afxUnit AmxEnumerateMixDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxMixDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
    
    afxClass const* cls = _AmxIcdGetMdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);
    
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
    AFX_ASSERT_OBJECTS(afxFcc_MDEV, rslt, devices);

    return rslt;
}

_AMX afxUnit AmxChooseMixDevices(afxUnit icd, amxDeviceInfo const* caps, amxFeatures const* features, amxLimits const* limits, afxUnit maxCnt, afxUnit mdevId[])
{
    afxError err = { 0 };
    //AFX_ASSERT(limits);
    //AFX_ASSERT(features);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxClass const* cls = _AmxIcdGetMdevClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    afxMixDevice mdev;
    for (afxUnit i = 0; TRUE; i++)
    {
        if (!AfxEnumerateObjects(cls, i, 1, (afxObject*)&mdev))
            break;
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if (features || limits)
            if (!AmxIsMixDeviceAcceptable(mdev, features, limits))
                continue;

        if (caps)
        {
            if ((mdev->capabilities & caps->capabilities) != caps->capabilities)
                continue;

            if ((mdev->acceleration & caps->acceleration) != caps->acceleration)
                continue;

            if (mdev->minQueCnt < caps->minQueCnt)
                continue;

            if (mdev->maxQueCnt < caps->maxQueCnt)
                continue;
        }

        {
            mdevId[rslt] = i;
            ++rslt;

            if (maxCnt > rslt)
                break;
        }
    }
    return rslt;
}

_AMX afxError _AmxIcdRegisterDevices(afxModule icd, afxUnit cnt, _amxMdevReg const infos[], afxMixDevice devices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AMX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AmxIcdGetMdevClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_MDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, cnt, devices);
    }
    return err;
}
