/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced User Experience Extensions.

#ifndef AUX_FONT_H
#define AUX_FONT_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxUxDefs.h"

AFX_DEFINE_STRUCT(afxFontConfig)
{
    afxString       face;
    afxUnit         ascent;
    afxUnit         descent;
    afxUnit         lineGap;
    afxRect         bounds;
    
    afxDrawSystem   dsys;
    afxBool         isDevFont;
    afxUnit         typeBufCap;
};

AUX afxError AfxAcquireFonts
(
    afxEnvironment env,
    afxUnit cnt,
    afxFontConfig const cfg[],
    afxFont fonts[]
);

////////////////////////////////////////////////////////////////////////////////

AFX afxError AfxTranscribe(afxDrawContext dctx, afxFont fnt, afxRect const* area, afxUnit col, afxUnit row, afxString const* text);

AFX afxError AfxFlushDeviceFont(afxFont fnt, afxDrawContext dctx, avxViewport const* vp);

#endif//AUX_FONT_H
