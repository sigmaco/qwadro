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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_QUERY_POOL_DDK_H
#define AVX_QUERY_POOL_DDK_H

#include "../afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxQrypDdi);
AFX_DECLARE_STRUCT(_avxQrypIdd);

#ifdef _AVX_QUERY_POOL_C
#ifdef _AVX_QUERY_POOL_IMPL
AFX_OBJECT(_avxQueryPool)
#else
AFX_OBJECT(avxQueryPool)
#endif
{
    _avxQrypDdi const*  ddi;
    _avxQrypIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    avxQueryType        type;
    afxUnit             slotCnt;
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_QRYP;

AVX afxError _AvxDsysSW_ResetQrypCb(afxDrawSystem dsys, avxQueryPool qryp, afxUnit base, afxUnit cnt);
AVX afxResult _AvxDsysSW_GetQrypRsltCb(afxDrawSystem dsys, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride);

#endif//AVX_QUERY_POOL_DDK_H
