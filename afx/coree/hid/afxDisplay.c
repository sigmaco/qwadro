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

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of Advanced User Experience Extensions.

#define _AVX_DRAW_C
#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AUX_DISPLAY_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "../draw/avxIcd.h"
#include "../ux/auxIcd.h"

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

_AUX afxError _AuxVduDtorCb(afxDisplayPort vdu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);



    return err;
}

_AUX afxError _AuxVduCtorCb(afxDisplayPort vdu, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxDisplay dpy = AFX_CAST(afxDisplay, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    afxDisplayPortConfig const* cfg = AFX_CAST(afxDisplayPortConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    vdu->tag = cfg->tag;
    vdu->udd = cfg->udd;

    vdu->dimWh[0] = cfg->dimWh[0];
    vdu->dimWh[1] = cfg->dimWh[1];
    vdu->resWh[0] = cfg->resWh[0];
    vdu->resWh[1] = cfg->resWh[1];
    vdu->fullArea = cfg->fullArea;
    vdu->workArea = cfg->workArea;
    vdu->persistentContent = cfg->persistentContent;
    vdu->planeReorder = cfg->planeReorder;
    vdu->supportedXforms = cfg->supportedXforms;
    vdu->prime = FALSE;

    vdu->dpi[0] = cfg->dpi[0];
    vdu->dpi[1] = cfg->dpi[1];

    AfxMakeString32(&vdu->name, &cfg->name);
    AfxMakeString128(&vdu->label, &cfg->label);

    return err;
}

_AUX afxClassConfig const _AUX_VDU_CLASS_CONFIG =
{
    .fcc = afxFcc_VDU,
    .name = "DisplayPort",
    .desc = "Video Display Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxDisplayPort)),
    .ctor = (void*)_AuxVduCtorCb,
    .dtor = (void*)_AuxVduDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxClass const* _AuxDpyGetVduClass(afxDisplay dpy)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    afxClass const* cls = &dpy->portCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    return cls;
}

_AUX afxError _AuxRegisterDisplayPorts(afxDisplay dpy, afxUnit cnt, afxDisplayPortConfig const cfg[], afxDisplayPort ports[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    AFX_ASSERT(ports);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AuxDpyGetVduClass(dpy);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ports, (void const*[]) { dpy, cfg }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VDU, cnt, ports);
    }
    return err;
}

_AUX afxDisplayPort AfxGetDisplayPort(afxDisplay dpy, afxUnit port)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    afxDisplayPort vdu = NIL;
    if (_AfxEnumerateObjectsUnlocked(_AuxDpyGetVduClass(dpy), FALSE, port, 1, (afxObject*)&vdu))
    {
        AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);
    }
    return vdu;
}

_AUX afxError AfxQueryGammaControlCapabilites(afxDisplay dpy, afxUnit port, afxGammaCapabilites* caps)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    
    if (!dpy->ddi->askGammaCtrlCb)
    {
        err = afxError_NOT_IMPLEMENTED;
    }
    else if (afxError_UNSUPPORTED == (err = dpy->ddi->askGammaCtrlCb(dpy, port, caps)))
    {
        *caps = (afxGammaCapabilites) { 0 };
    }
    return err;
}

_AUX afxError AfxDescribeGammaControl(afxDisplay dpy, afxUnit port, afxGammaCurve* desc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    if (!dpy->ddi->getGammaCtrlCb)
    {
        err = afxError_NOT_IMPLEMENTED;
    }
    else if (afxError_UNSUPPORTED == (err = dpy->ddi->getGammaCtrlCb(dpy, port, desc)))
    {
        *desc = (afxGammaCurve) { 0 };
    }
    return err;
}

_AUX afxError AfxControlGamma(afxDisplay dpy, afxUnit port, afxGammaCurve const* ctrl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    if (!dpy->ddi->setGammaCtrlCb)
    {
        err = afxError_NOT_IMPLEMENTED;
    }
    else if ((err = dpy->ddi->setGammaCtrlCb(dpy, port, ctrl)))
    {
        AfxThrowError();
    }
    return err;
}

_AUX afxUnit AfxQueryDisplayModes(afxDisplay dpy, afxUnit port, avxFormat fmt, afxUnit cnt, afxDisplayMode modes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);
    AFX_ASSERT(modes);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    rslt = dpy->ddi->qryModeCb(dpy, port, fmt, cnt, modes);

    return rslt;
}

_AUX afxError AfxCopyBackDisplayBuffer(afxDisplay dpy, afxUnit port, afxSurface dout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    if (dpy->ddi->captureCb(dpy, port, dout))
        AfxThrowError();

    return err;
}

_AUX afxError _AuxDpyDtorCb(afxDisplay dpy)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    AfxExhaustChainedClasses(&dpy->dev.classes);

    _AFX_DEV_CLASS_CONFIG.dtor(&dpy->dev);

    return err;
}

_AUX afxError _AuxDpyCtorCb(afxDisplay dpy, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DPY, 1, &dpy);

    afxModule icd = AFX_CAST(afxModule, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxDisplayConfig const* cfg = AFX_CAST(afxDisplayConfig const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    afxClassConfig const* pPortClsCfg = args[2];

    if (_AFX_DEV_CLASS_CONFIG.ctor(&dpy->dev, (void*[]) { icd, (void*)&cfg->dev }, 0))
    {
        AfxThrowError();
        return err;
    }
    
    dpy->tag = cfg->tag;
    dpy->udd = cfg->udd;

    AfxMakeString32(&dpy->name, &cfg->name);
    AfxMakeString128(&dpy->label, &cfg->label);

    afxClassConfig portClsCfg = pPortClsCfg ? *pPortClsCfg : _AUX_VDU_CLASS_CONFIG;

    AfxMountClass(&dpy->portCls, NIL, &dpy->dev.classes, &portClsCfg);

    return err;
}

_AUX afxClassConfig const _AUX_DPY_CLASS_CONFIG =
{
    .fcc = afxFcc_DPY,
    .name = "Display",
    .desc = "Video Display Device",
    .fixedSiz = sizeof(AFX_OBJECT(afxDisplay)),
    .ctor = (void*)_AuxDpyCtorCb,
    .dtor = (void*)_AuxDpyDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError _AuxRegisterDisplays(afxModule icd, afxUnit cnt, afxDisplayConfig const cfg[], afxDisplay displays[])
{
    afxError err = { 0 };
    AFX_ASSERT(displays);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);
    
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AUX));

    afxClass* cls = (afxClass*)_AuxIcdGetDpyClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_DPY);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)displays, (void const*[]) { icd, cfg, &_AUX_VDU_CLASS_CONFIG }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DPY, cnt, displays);

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
    AFX_ASSERT_CLASS(cls, afxFcc_DPY);
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)displays);
    // @displays must be an array of valid afxDisplay handles.
    AFX_ASSERT_OBJECTS(afxFcc_DPY, rslt, displays);
    return rslt;
}
