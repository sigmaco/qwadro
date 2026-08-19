/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Parallel Computing Extensions.

#ifndef ACX_WARP_DEFS_H
#define ACX_WARP_DEFS_H

#include "qwadro/afxSystem.h"
#include "qwadro/vga/afxDrawSystem.h"

#ifndef __e2serpro__
#   ifdef _DEBUG
#       define ACX DLLIMPORT extern 
#       define ACXINL DLLIMPORT EMBED
#   else
#       define ACX DLLIMPORT extern 
#       define ACXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _ACX DLLEXPORT
#       define ACX DLLEXPORT extern 
#       define _ACXINL DLLEXPORT INLINE
#       define ACXINL DLLEXPORT EMBED
#   else
#       define _ACX DLLEXPORT
#       define ACX DLLEXPORT extern 
#       define _ACXINL DLLEXPORT INLINE
#       define ACXINL DLLEXPORT EMBED
#   endif
#endif//__e2serpro__

AFX_DEFINE_HANDLE(afxWarpDevice);
AFX_DEFINE_HANDLE(afxWarpSystem);
AFX_DEFINE_HANDLE(afxWarpBridge);
AFX_DEFINE_HANDLE(afxWarpQueue);
AFX_DEFINE_HANDLE(afxWarpContext);
AFX_DEFINE_HANDLE(acxFence);
AFX_DEFINE_HANDLE(acxBuffer);

#endif//ACX_WARP_DEFS_H
