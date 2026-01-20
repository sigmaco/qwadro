/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experience Extensions.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_DISPLAY_DDK_H
#define AUX_DISPLAY_DDK_H

#include "qwadro/ux/afxUxDefs.h"
#include "../coree/exec/afxSystemDDK.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DECLARE_STRUCT(_auxIddDisp);
AFX_DEFINE_STRUCT(_auxDdiDisp)
{
    afxUnit(*qryModeCb)(afxDisplay, afxUnit port, avxFormat, afxUnit cnt, afxDisplayMode[]);
    afxError(*askGammaCtrlCb)(afxDisplay, afxUnit port, afxGammaCapabilites*);
    afxError(*getGammaCtrlCb)(afxDisplay, afxUnit port, afxGammaCurve*);
    afxError (*setGammaCtrlCb)(afxDisplay, afxUnit port, afxGammaCurve const*);
    afxError(*captureCb)(afxDisplay, afxUnit port, afxSurface);
};

#ifdef _AUX_DISPLAY_C
#ifdef _AUX_DISPLAY_IMPL
AFX_OBJECT(_auxDisplayPort)
#else
AFX_OBJECT(afxDisplayPort)
#endif
{
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // desktop coordinates.
    afxRect workArea;
    afxRect fullArea;
    // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit dimWh[2];
    // the physical, native, or preferred resolution of the display.
    afxUnit resWh[2];
    afxUnit dpi[2];
    // transforms are supported by this display.
    avxVideoTransform supportedXforms;
    // can re-arrange the planes on this display in any order relative to each other?
    afxBool planeReorder;
    // can submit persistent present operations on swapchains created against this display?
    afxBool persistentContent;
    afxBool prime;

    afxUnit bpp;
    afxUnit freq;
    afxUnit planeCnt;
    afxUnit paletteSiz;
    afxBool clipCapable;

    // the name of the display.
    afxString32     name;
    // the friendly name of the display.
    afxString128    label;
};

#ifdef _AUX_DISPLAY_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_auxDisplay)
#else
AFX_OBJECT(afxDisplay)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _auxIddDisp*        idd;
    _auxDdiDisp const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxClass            portCls;

    // the name of the display.
    afxString32     name;
    // the friendly name of the display.
    afxString128    label;
    
};
#endif//_AUX_DISPLAY_C

AUX afxClassConfig const _AUX_DPY_CLASS_CONFIG;
AUX afxClassConfig const _AUX_VDU_CLASS_CONFIG;

AUX afxClass const* _AuxDpyGetVduClass(afxDisplay dpy);
AUX afxError _AuxRegisterDisplayPorts(afxDisplay dpy, afxUnit cnt, afxDisplayPortConfig const cfg[], afxDisplayPort ports[]);

#endif//AUX_DISPLAY_DDK_H
