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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_VERTEX_INPUT_DDK_H
#define AVX_VERTEX_INPUT_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddVin);
AFX_DECLARE_STRUCT(_avxDdiVin);

AFX_DEFINE_STRUCT(_avxVertexBin)
{
    // Binding index (matches the vertex buffer bound) which this stream takes its data from.
    afxUnit         pin;
    // Minimum stride (in bytes) between consecutive vertices.
    afxUnit32       minStride;
    // Instance rate divisor (0 = per-vertex, >0 = per-instance).
    afxUnit         instRate;
    // Index into 'attrs' where attributes for this fetch begin.
    afxUnit         baseAttrIdx;
    // Number of attributes sourced from this fetch.
    afxUnit         attrCnt;
    // Misc. flags (e.g., dynamic/static buffer, etc.).
    afxFlags        flags;
};

AFX_DEFINE_STRUCT(_avxVertexAttr)
// Describes a single vertex attribute (like position, normal, texcoord, etc.).
{
    // Location in shader (e.g., layout(location = x) in GLSL).
    afxUnit         location;
    // Format of the data (e.g., vec3 float, vec2 half, etc.).
    avxFormat       fmt;
    // Cached binding pin.
    afxUnit         pin;
    // A byte offset of this attribute relative to the start of an element in the bound vertex buffer.
    afxUnit32       offset;
    // Custom flags for optimization/metadata.
    afxFlags        flags;
};

#ifdef _AVX_VERTEX_INPUT_C
#ifdef _AVX_VERTEX_DECL_IMPL
AFX_OBJECT(_avxVertexInput)
#else
AFX_OBJECT(avxVertexInput)
#endif
{
    _avxDdiVin const*   ddi;
    _avxIddVin*         idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxFlags            flags;
    afxUnit             totalAttrCnt;
    _avxVertexAttr*     attrs;
    afxUnit             binCnt;
    _avxVertexBin*      bins;
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_VIN;

#endif//AVX_VERTEX_INPUT_DDK_H
