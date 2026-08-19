/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   U S E R   E X P E R I E N C E   I N F R A S T R U C T U R E
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

#ifndef AUX_WINDOW_DDK_H
#define AUX_WINDOW_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DECLARE_STRUCT(_auxWndIdd);

#ifndef _AUX_UX_C
AFX_DECLARE_STRUCT(_auxWndDdi);
#else
AFX_DEFINE_STRUCT(_auxWndDdi)
{
    afxError(*evhCb)(afxWindow, auxEvent*);
    afxError(*damageCb)(afxWindow, afxRect const*);
    afxError(*redrawCb)(afxWindow, afxRect const*);
    afxError(*chIconCb)(afxWindow, avxRaster, avxRasterRegion const* rgn);
    afxError(*chCursCb)(afxWindow, avxRaster, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY);
    afxError(*adjustCb)(afxWindow, afxAnchor, afxRect*);
    void(*focus)(afxWindow);
    afxBool(*hasFocus)(afxWindow);
    afxUnit(*titleCb)(afxWindow);
};
#endif

#ifdef _AUX_WINDOW_C
#ifdef _AUX_WINDOW_IMPL
AFX_OBJECT(_auxWindow)
#else
AFX_OBJECT(afxWindow)
#endif
{
    _auxWndDdi const*   ddi;
    _auxWndIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxDesktop*         dwm;
    afxChain            classes;
    afxClass            widCls;

    // .adjusting is key to prevent recursion.
    afxBool             adjusting;
    afxReal             aspRatio;
    afxAnchor           anchor;
    afxRect             screenRc;
    afxSurface          frameDout;
    afxInt              framePos;
    afxRect             frameRc;
    afxRect             frameRcMin;
    afxRect             frameRcMax;
    afxRect             frameMargin; // used to make room for drop shadow
    afxUnit             frameMarginL;
    afxUnit             frameMarginT;
    afxUnit             frameMarginR;
    afxUnit             frameMarginB;
    afxRect             frameBorder; // used to place resize control on frame.
    afxSurface          surfaceDout;
    afxRect             surfacePadding; // used to align content area
    afxRect             surfaceRc; // aka client area
    
    avxRaster           iconFnt;
    avxRasterRegion     iconCrop;
    avxRaster           cursFnt;
    avxRasterRegion     cursCrop;
    afxString512        title;

    afxBool             alwaysOnTop;
    afxBool             active;
    afxBool             focused;
    afxBool             decorated;
    afxBool             floating;
    afxBool             resizable;
    afxBool             fullscreen;

    afxClock            startClock;
    afxClock            lastClock;

    afxV2d              cursPos;
    afxV2d              cursMove;
    afxV2d              cursPosNdc;
    afxV2d              cursMoveNdc;
    afxBool             cursConfined; // confined in window
    afxRect             cursConfinRect;
    afxBool             cursHidden; // visually disabled in window surface
    
    afxBool             redrawFrameRequested;
    afxBool             redrawSurfaceRequested;

    afxWidget           hoveredWidg;
    afxV2d              hoveredPoint;
    afxWidget           focusedWidg;
    afxWidget           grabbedWidg;
    afxV2d              grabPoint;
};
#endif//_AUX_WINDOW_C

AUX afxClassConfig const _AUX_WND_CLS_CFG;
AUX _auxWndDdi const _AUX_WND_DDI_SW;

AUX afxClass const* _AuxWndGetWidClass(afxWindow wnd);

AUX afxBool _AuxWidEmitEventCb(afxWidget wid, auxEvent *ev);
AUX afxBool _AuxWndSwEventHandlerCb(afxWindow wnd, auxEvent *ev);

AUX afxUnit _AfxWndSwFormatTitleCb(afxWindow wnd);
AUX afxError _AfxWndSwAdjustCb(afxWindow wnd, afxAnchor, afxRect* c);
AUX _auxWndDdi const _AUX_WND_DDI_SW;

AUX afxError _AfxWndSwChangeCursorCb(afxWindow wnd, avxRaster font, avxRasterRegion const* rgn, afxInt hotspotX, afxInt hotspotY);
AUX afxError _AfxWndSwChangeIconCb(afxWindow wnd, avxRaster font, avxRasterRegion const* rgn);

AUX afxError _AvxEnvSwConfigureWndCb(afxEnvironment env, afxWindowConfig* cfg, afxV2d const origin, afxV2d const extent);
AUX afxError _AuxEnvSwAcquireWndCb(afxEnvironment env, afxWindowConfig const* cfg, afxWindow* window);

#endif//AUX_WINDOW_DDK_H
