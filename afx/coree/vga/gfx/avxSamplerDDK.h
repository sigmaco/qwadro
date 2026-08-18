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

#ifndef AVX_SAMPLER_DDK_H
#define AVX_SAMPLER_DDK_H

#include "../../afxSystemDDK.h"
#include "qwadro/vga/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxSampIdd);
AFX_DECLARE_STRUCT(_avxSampDdi);

#ifdef _AVX_SAMPLER_C
#ifdef _AVX_SAMPLER_IMPL
AFX_OBJECT(_avxSampler)
#else
AFX_OBJECT(avxSampler)
#endif
{
    _avxSampDdi const*  ddi;
    _avxSampIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxUnit32           crc;
    afxFlags            flags;
    avxSamplerConfig    cfg;
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_SAMP;

AVX afxError _AvxDsysSwConfigureSampCb(afxDrawSystem dsys, avxSamplerConfig* cfg);

AVX afxError _AvxDsysSwAcquireSampCb(afxDrawSystem dsys, afxUnit cnt, avxSamplerConfig const cfg[], avxSampler samplers[]);

#endif//AVX_SAMPLER_DDK_H
