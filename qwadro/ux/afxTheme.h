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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experience Extensions.

#ifndef AUX_THEME_H
#define AUX_THEME_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/ux/afxUxDefs.h"

AFX_DEFINE_STRUCT(afxThemeInfo)
{
    afxModule mdle;
};

AUX afxError AfxAcquireThemes
(
    afxEnvironment env, 
    afxUnit cnt, 
    afxThemeInfo const info[], 
    afxFont fonts[]
);

AUX afxError AfxLoadThemes
(
    afxEnvironment env,
    afxUnit cnt, 
    afxUri const uri[], 
    afxTheme themes[]
);

#endif//AUX_THEME_H
