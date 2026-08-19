/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_CANVAS_DDK_H
#define AVX_CANVAS_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxCanvIdd);

AFX_DEFINE_STRUCT(_avxCanvDdi)
{
    afxError(*relink)(avxCanvas, afxBool, afxUnit, afxUnit, avxRaster[]);
    afxError(*readjust)(avxCanvas, afxWarp const);
};

#ifdef _AVX_CANVAS_C
AFX_DEFINE_STRUCT(_avxCanvasRig)
{
    avxRaster       ras; // the texture subresource that will be output to for this color attachment.
    avxRaster       resolve; // the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    avxFormat       fmt; // the format of the image that will be used for the attachment.
    avxRasterUsage  usage; // additional properties of the attachment.
    avxRasterFlags  flags;
    afxUnit         lodCnt; // the number of samples of the image.
};
#ifdef _AVX_CANVAS_IMPL
AFX_OBJECT(_avxCanvas)
#else
AFX_OBJECT(avxCanvas)
#endif
{
    _avxCanvDdi const*  ddi;
    _avxCanvIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxCanvasFlags      flags;
    avxRange            whdMin; // when a user-provided raster are attached
    avxRange            whdMax; // when a user-provided raster are attached
    avxRange            whd; // D is layered
    afxUnit             lodCnt; // multisampling level (2^lodCnt)
    afxUnit             linkedCnt;
    afxUnit             rigCnt; // immutable
    _avxCanvasRig*      rigs;
    afxFlags            ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxUnit             colorCnt;
    afxUnit             dsRigIdx[2];
};
#endif//_AVX_CANVAS_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_CANV;

AVX afxError _AvxDsysSwConfigureCanvCb(afxDrawSystem dsys, avxCanvasConfig* cfg);
AVX afxError _AvxDsysSwAcquireCanvCb(afxDrawSystem dsys, avxCanvasConfig const* cfg, afxUnit cnt, avxCanvas canvases[]);

#endif//AVX_CANVAS_DDK_H
