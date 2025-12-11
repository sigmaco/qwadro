/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ARX_BUFFERIZER_H
#define ARX_BUFFERIZER_H

#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxVertexInput.h"
#include "qwadro/render/arxRenderable.h"

AFX_DEFINE_HANDLE(arxBufferizer);

#ifdef _ARX_DRAW_C
#ifdef _ARX_VERTEX_BUFFER_C

#endif//_ARX_VERTEX_BUFFER_C
#endif//_ARX_DRAW_C

AFX_DEFINE_STRUCT(arxBufferizerInfo)
{
    afxUnit         bufCap;
    avxBufferFlags  access;
    avxBufferUsage  usage;
};

ARX afxError        ArxAcquireBufferizer(arxScenario scio, arxBufferizerInfo const* info, arxBufferizer* bufferizer);

ARX avxBuffer       ArxGetVertexBufferStorage(afxVertexBuffer vbuf);
ARX afxUnit         ArxGetVertexBufferUsage(afxVertexBuffer vbuf);
ARX avxVertexInput  ArxGetVertexBufferLayout(afxVertexBuffer vbuf);
ARX afxUnit         ArxGetVertexBufferCapacity(afxVertexBuffer vbuf);

#endif//ARX_BUFFERIZER_H
