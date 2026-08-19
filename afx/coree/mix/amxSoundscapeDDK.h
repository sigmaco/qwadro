/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_SOUNDSCAPE_DDK_H
#define AMX_SOUNDSCAPE_DDK_H

#include "amxSoundDDK.h"

#ifdef _AMX_SOUNDSCAPE_C
#ifdef _AMX_SOUNDSCAPE_IMPL
AFX_OBJECT(_amxSoundscape)
#else
AFX_OBJECT(amxSoundscape)
#endif
{
    afxChain        classes;
    afxClass        sndCls;

    afxFlags        flags;
    afxUnit         ambiOrder;

    afxTransform    placement;
    afxV3d          velocity;

    afxMixContext   mix;
    afxUnit         baseSmixIdx;
    afxUnit         exuIdx; // renderer
};
#endif

AMX afxClass const* _AmxSndsGetSndClass(amxSoundscape snds);

#endif//AMX_SOUNDSCAPE_DDK_H
