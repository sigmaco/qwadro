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

#ifndef ARX_CURVE_DDK_H
#define ARX_CURVE_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DECLARE_STRUCT(_arxIddCur);
AFX_DEFINE_STRUCT(_arxDdiCur)
{
    void(*evalAtKnotCb)(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec);
    void(*updateCb)(arxCurve c, afxUnit knotCnt, afxUnit dimens, afxReal const* knotSrc, afxReal const* ctrlSrc);
    void(*extractCb)(arxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec);
    afxUnit(*findKnotCb)(arxCurve c, afxReal t, afxUnit baseIdx);
    void(*copyCb)(arxCurve c, arxCurve const src);
};

#ifdef _ARX_CURVE_C
#ifdef _ARX_CURVE_IMPL
AFX_OBJECT(_arxCurve)
#else
AFX_OBJECT(arxCurve)
#endif
{
    _arxDdiCur const*   ddi;
    _arxIddCur*         idd;
    void*               udd;
    afxString           tag;

    arxCurveFormat      fmt;
    afxUnit             degree;
    afxUnit             dimens;
    arxCurveFlags       flags;
    afxUnit             knotCnt;
    union
    {
        afxReal*        knots; // 32f
        afxReal         knot0;
    };
    afxUnit             ctrlCnt;
    union
    {
        afxV4d          ctrls4; // D4, Constant32f
        afxV3d          ctrls3;
        afxReal*        ctrls; // 32f
    };
};
#endif//_ARX_CURVE_C

ARX afxClassConfig const _ARX_CUR_CLASS_CONFIG;

#endif//ARX_CURVE_DDK_H
