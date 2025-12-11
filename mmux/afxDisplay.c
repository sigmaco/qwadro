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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.
// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AVX_DRAW_C
#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AUX_DISPLAY_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "../targa/avxIcd.h"
#include "auxIcd.h"

_AUX afxReal64 AfxFindPhysicalAspectRatio(afxUnit screenWidth, afxUnit screenHeight)
{
    afxError err = { 0 };

    // Ensure screen dimensions are valid
    AFX_ASSERT(screenWidth);
    // Prevent division by zero
    if (screenHeight == 0)
    {
        AFX_ASSERT(screenHeight);
        //err = afxError_DIVISION_BY_ZERO; // Assuming we have an error code for this case
        return 0;  // Handle this as needed
    }

    // Calculate the aspect ratio (width divided by height)
    afxReal64 ratio;
    afxReal64 div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    // Determine the physical aspect ratio based on width-to-height ratio
    if (div <= 1.4)
        ratio = 1.33;    // Standard 4:3 aspect ratio
    else if (div <= 1.5)  // 5:4 is close to 1.25, but we can round it based on the width-to-height ratio
        ratio = 1.25;    // Standard 5:4 aspect ratio
    else if (div <= 1.6)
        ratio = 1.56;    // Standard 16:10 aspect ratio
    else if (div <= 1.7)
        ratio = 1.78;    // Standard 16:9 aspect ratio
    else if (div <= 2.0)
        ratio = 1.91;    // Standard 21:9 aspect ratio (ultrawide)
    else if (div <= 2.5)
        ratio = 2.33;    // Example for ultra-wide monitors
    else if (div <= 3.0)
        ratio = 3.00;    // Example for super-ultrawide displays (32:9)
    else
        ratio = 3.00;    // Catch-all for extreme aspect ratios

    return ratio;
}

_AUX afxError _AuxVduDtorCb(afxDisplay vdu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    _AFX_DEV_CLASS_CONFIG.dtor(&vdu->dev);

    return err;
}

_AUX afxError _AuxVduCtorCb(afxDisplay vdu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxModule icd = AFX_CAST(afxModule, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    avxDisplayInfo const* info = AFX_CAST(avxDisplayInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&vdu->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    vdu->dimWh[0] = info->dimWh[0];
    vdu->dimWh[1] = info->dimWh[1];
    vdu->resWh[0] = info->resWh[0];
    vdu->resWh[1] = info->resWh[1];
    vdu->dpi[0] = info->dpi[0];
    vdu->dpi[1] = info->dpi[1];
    vdu->persistentContent = info->persistentContent;
    vdu->planeReorder = info->planeReorder;
    vdu->supportedXforms = info->supportedXforms;
    AfxStrcpy(vdu->name, info->name);
    AfxStrcpy(vdu->label, info->label);

    vdu->portCnt = 1;
    vdu->ports[0] = (_auxDisplayPort) { 0 };
    vdu->ports[1] = (_auxDisplayPort) { 0 };

    return err;
}

_AUX afxClassConfig const _AUX_VDU_CLASS_CONFIG =
{
    .fcc = afxFcc_VDU,
    .name = "Display",
    .desc = "Video Display Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxDisplay)),
    .ctor = (void*)_AuxVduCtorCb,
    .dtor = (void*)_AuxVduDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError _AuxRegisterDisplays(afxModule icd, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[])
{
    afxError err = { 0 };
    AFX_ASSERT(displays);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);
    
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX));

    afxClass* cls = (afxClass*)_AuxIcdGetDpyClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)displays, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VDU, cnt, displays);

    }
    return err;
}

_AUX afxUnit AfxEnumerateDisplays(afxUnit icd, afxUnit first, afxUnit cnt, afxDisplay displays[])
{
    afxError err = { 0 };
    AFX_ASSERT(displays);
    AFX_ASSERT(cnt);

    afxModule drv;
    if (!_AuxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AUX));

    afxUnit rslt = 0;
    afxClass const* cls = _AuxIcdGetDpyClass(drv);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)displays);
    // @displays must be an array of valid afxDisplay handles.
    AFX_ASSERT_OBJECTS(afxFcc_VDU, rslt, displays);
    return rslt;
}
