/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_CAPSTAN_DDK_H
#define ARX_CAPSTAN_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DECLARE_STRUCT(_arxDdiCap);
AFX_DECLARE_STRUCT(_arxIddCap);

#ifdef _ARX_PULLEY_C
typedef enum arxPulleyFlag
{
    arxPulleyFlag_ACTIVE = AFX_BITMASK(0),
    arxPulleyFlag_KILL_ONCE_COMPLETE = AFX_BITMASK(1),
    arxPulleyFlag_KILL_ONCE_UNUSED = AFX_BITMASK(2),
    arxPulleyFlag_EASE_IN = AFX_BITMASK(3),
    arxPulleyFlag_EASE_OUT = AFX_BITMASK(4),
    arxPulleyFlag_FORCE_CLAMPLED_LOOPS = AFX_BITMASK(5)
} arxPulleyFlags;

#ifdef _ARX_PULLEY_IMPL
AFX_OBJECT(_arxPulley)
#else
AFX_OBJECT(arxPulley)
#endif
{
    _arxIddCap*         idd;
    _arxDdiCap const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    arxPulleyFlags flags;
    afxReal         dtLocalTimePending;
    afxReal         localTime;
    afxReal         speed;
    afxReal         localDur;
    afxInt          currIterIdx;
    afxInt          iterCnt;
    afxReal         currWeight;
    arxPulleyTiming timing;
    afxUnit32       easeInValues;
    afxUnit32       easeOutValues;
};
#endif//_ARX_PULLEY_C

ARX afxClassConfig const _ARX_MOTO_CLASS_CONFIG;

#endif//ARX_CAPSTAN_DDK_H
