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

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_BUFFER_H
#define ARX_BUFFER_H

#include "qwadro/iris/arxIrisDefs.h"

typedef enum arxFormat
{
    arxFormat_DaKeyframes32f, // Da, Keyframes32f
    arxFormat_DaK32fC32f, // Da, K32f, C32f
    arxFormat_DaIdentity, // Da, Identity
    arxFormat_DaConstant32f, // Da, Constant32f
    arxFormat_D3Constant32f, // D3, Constant32f
    arxFormat_D4Constant32f, // D4, Constant32f
    // still unsupported
    arxFormat_DaK16uC16u, // Da, K16u, C16u
    arxFormat_DaK8uC8u, // Da, K8u, C8u
    arxFormat_D4nK16uC15u, // D4n, K16u, C15u
    arxFormat_D4nK8uC7u, // D4n, K8u, C7u
    arxFormat_D3K16uC16u, // D3, K16u, C16u
    arxFormat_D3K8uC8u, // D3, K8u, C8u
    arxFormat_D9i1K16uC16u, // D9i1, K16u, C16u    
    arxFormat_D9i3K16uC16u, // D9i3, K16u, C16u
    arxFormat_D9i1K8uC8u, // D9i1, K8u, C8u
    arxFormat_D9i3K8uC8u, // D9i3, K8u, C8u
    arxFormat_D3i1K32fC32f, // D3i1, K32f, C32f
    arxFormat_D3i1K16uC16u, // D3i1, K16u, C16u
    arxFormat_D3i1K8uC8u // D3i1, K8u, C8u    

    // Let curve formats at first to be retrocompatible with old code.



} arxFormat;

typedef enum arxBufferUsage
{
    arxBufferUsage_VERTEX = AFX_BITMASK(0), // used to hold mesh triangulation vertices
    arxBufferUsage_INDEX = AFX_BITMASK(1), // used to hold mesh triangulation indices
    arxBufferUsage_DRAW = AFX_BITMASK(2), // used to be drawn by draw system
    arxBufferUsage_CURVE = AFX_BITMASK(3), // used to hold geometric curves
    arxBufferUsage_MATRIX = AFX_BITMASK(4)
} arxBufferUsage;

typedef enum arxBufferFlag
{
    arxBufferFlag_R = AFX_BITMASK(0),
    arxBufferFlag_W = AFX_BITMASK(1),
    arxBufferFlag_X = AFX_BITMASK(2),
    arxBufferFlag_RW = (arxBufferFlag_R | arxBufferFlag_W),
    arxBufferFlag_WX = (arxBufferFlag_W | arxBufferFlag_X),
    arxBufferFlag_RWX = (arxBufferFlag_R | arxBufferFlag_W | arxBufferFlag_X),
    arxBufferFlag_ACCESS = (arxBufferFlag_R | arxBufferFlag_W | arxBufferFlag_X)
} arxBufferFlags;

AFX_DEFINE_STRUCT(arxBufferInfo)
{
    // The size in bytes of the buffer to be created.
    afxUnit         size;
    // A bitmask specifying allowed usages of the buffer.
    arxBufferUsage  usage;
    // A bitmask specifying additional parameters of the buffer.
    arxBufferFlags  flags;
    // A user-defined data.
    void*           udd;
    afxString       tag;

    // A bitmask of device bridges that will access this buffer.
    afxMask         sharingMask;
    // A arxBuffer on which the view will be created.
    arxBuffer       base;
    // An offset in bytes from the base address of the buffer.
    afxSize         from;
    // A arxFormat describing the format of the data elements in the buffer.
    arxFormat       fmt; // used for TENSOR buffers.

    // Should this buffer be mapped at creation?
    afxBool         mapped;
    // Possibly initial data, to avoid boilerplate with inconvenient mappings.
    void*           data;
    afxUnit         dataSiz;
};

AFX_DEFINE_STRUCT(arxBufferCopy)
{
    // The starting offset in bytes from the start of srcBuffer.
    afxSize         srcOffset;
    // The starting offset in bytes from the start of dstBuffer.
    afxSize         dstOffset;
    // The number of bytes to copy.
    afxUnit         range;
};

AFX_DEFINE_STRUCT(arxBufferIo)
{
    // The starting offset in bytes from the start of srcBuffer.
    afxSize         srcOffset;
    // The starting offset in bytes from the start of dstBuffer.
    afxSize         dstOffset;
    // [!] only if supported by device, else case it must be 1.
    afxUnit         srcStride;
    // [!] only if supported by device, else case it must be 1.
    afxUnit         dstStride;
    // The number of rows to stream in/out.
    afxUnit         rowCnt;
};

ARX arxScenario ArxGetBufferHost
(
    arxBuffer rbuf
);

ARX void* ArxGetBufferUdd
(
    arxBuffer rbuf
);

ARX afxUnit ArxGetBufferCapacity
(
    arxBuffer rbuf, 
    afxUnit from
);

ARX arxBufferUsage ArxGetBufferUsage
(
    arxBuffer rbuf, 
    arxBufferUsage usage
);

ARX arxBufferFlags ArxGetBufferFlags
(
    arxBuffer rbuf, 
    arxBufferFlags mask
);

ARX void* ArxGetBufferData
(
    arxBuffer rbuf, 
    afxSize from
);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxAcquireBuffers
(
    arxScenario scio, 
    afxUnit cnt, 
    arxBufferInfo const infos[], 
    arxBuffer buffers[]
);

#endif//ARX_BUFFER_H
