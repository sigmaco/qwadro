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

#ifndef AVX_SHADER_DDK_H
#define AVX_SHADER_DDK_H

#include "../../afxSystemDDK.h"
#include "qwadro/vga/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxShadIdd);
AFX_DECLARE_STRUCT(_avxShadDdi);

#ifdef _AVX_SHADER_C

AFX_DEFINE_STRUCT(_avxShadBlock)
{
    afxString           tag;
    void*               udd;
    afxString32         name;
    avxShaderType       stage;

    afxUnit             verMajor;
    afxUnit             verMinor;
    afxBool             extended;
    afxByte*            code;
    afxUnit             codeLen;
    afxString           entry;

    afxString32         pushConstName;
    afxUnit             resDeclCnt;
    avxShaderResource*  resDecls;

    afxUnit             inCnt;
    avxShaderIoChannel* ins;
    afxUnit             outCnt;
    avxShaderIoChannel* outs;
    avxTopology         topology;
};

#ifdef _AVX_SHADER_IMPL
AFX_OBJECT(_avxShader)
#else
AFX_OBJECT(avxShader)
#endif
{
    _avxShadDdi const*  ddi;
    _avxShadIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    // Optional base URL for file lookup.
    afxUri128           url;
    // A pool of code units.
    afxPool             codes;
};
#endif//_AVX_SHADER_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_CODB;

AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode);

AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode);
AVX afxError _AvxScanGlScript(afxString const* code, afxArray* fIns, afxArray* fOuts, afxArray* fResources, afxString* pushConstsName);

AFX_DEFINE_STRUCT(_avxGlslConversor)
{
    afxBool dontInline;
    afxBool bindUniform;
    afxUnit pushBufLoc;
    afxBool pushViaUbo;
};

AVX afxError _AvxConvertToGlsl(_avxGlslConversor const* cfg, afxString const* src, afxArray* fCode);

#endif//AVX_SHADER_DDK_H
