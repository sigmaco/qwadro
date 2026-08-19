/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // GEOMETRIC PRIMITIVE TRANSFORMATION PIPELINE OPERATIONS                   //
//////////////////////////////////////////////////////////////////////////////

// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_TRANSFORMATION_H
#define AVX_TRANSFORMATION_H

#include "qwadro/draw/avxViewport.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/avxRaster.h"
#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/draw/avxQueryPool.h"

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(avxDrawIndirect)
/// Structure specifying a indirect drawing command
{
    afxUnit32   vtxCnt; // is the number of vertices to draw.
    afxUnit32   instCnt; // is the number of instances to draw.
    afxUnit32   baseVtx; // is the index of the first vertex to draw.
    afxUnit32   baseInst; // is the instance ID of the first instance to draw.
};

AFX_DEFINE_STRUCT(avxDrawIndexedIndirect)
/// Structure specifying a indexed indirect drawing command
{
    afxUnit32   idxCnt; // is the number of vertices to draw.
    afxUnit32   instCnt; // is the number of instances to draw.
    afxUnit32   baseIdx;/// is the base index within the index buffer.
    afxInt32    vtxOffset; // is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit32   baseInst; // is the instance ID of the first instance to draw.
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////////
//// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

// Specify the vertex input layout for consequent vertex fetching for a draw context.

AVX afxError AvxCmdUseVertexInput
(
    afxDrawContext dctx,

    avxVertexInput vin
);

/*
    Bind vertex buffers to a draw context and dynamically set strides.
    The values taken from elements #i of @buf and @offset replace the current state for the vertex input binding @basePin + #i, for #i in [0, @cnt].
    The vertex input binding is updated to start at the offset indicated by @offset[#i] from the start of the buffer @buf[#i].
    If @range is not NIL then @range[#i] specifies the bound size of the vertex buffer starting from the corresponding elements of @buf[#i] plus @offset[#i].
    If @range[#i] is WHOLE_SIZE then the bound size is from @buf[#i] plus @offset[#i] to the end of the buffer @buf[#i].
    All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
    If the null descriptor feature is enabled, elements of @buf can be NIL, and can be used by the vertex shader.
    If a vertex input attribute is bound to a vertex input binding that is NIL, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

    This command also dynamically sets the byte strides between consecutive elements within buffer @buf[#i] to the corresponding @stride[#i] value when drawing using shader objects, or when the graphics pipeline is created without vertex input binding stride set.

    The ranges inside the buffers are not required, but they should be provided to help debugging buffer access and hinting the implementation about usage behavior.
*/

AVX afxError AvxCmdBindVertexBuffers
(
    afxDrawContext dctx,

    // The index of the first vertex input binding whose state is updated by the command.
    afxUnit basePin,

    // The number of vertex input bindings whose state is updated by the command.
    afxUnit cnt,

    // An array of info to set up the avxBuffer-backed streams.
    avxBufferedStream const streams[]
);

/*
    The AvxCmdBindIndexBuffer() command binds an index buffer to a draw context for use by consequent indexed drawing commands.

    The range inside the buffer is not required, but it should be provided to help debugging buffer access and hinting the implementation about usage behavior.
*/

AVX afxError AvxCmdBindIndexBuffer
(
    afxDrawContext dctx,

    // The buffer being bound.
    avxBuffer buf,

    // The starting offset in bytes within buffer used in index buffer address calculations.
    afxUnit32 offset,

    // The size in bytes of index data bound from buffer.
    afxUnit32 range,

    // A value specifying the size of the indices.
    afxUnit32 idxSiz
);

/// Draw primitives.
/// When the command is executed, primitives are assembled using the current primitive topology and @vtxCnt consecutive vertex indices with the first @vtxIdx value equal to @firstVtx. 
/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxError AvxCmdDraw
(
    afxDrawContext dctx,
    
    // is the number of vertices to draw.
    afxUnit vtxCnt,

    // is the number of instances to draw.
    afxUnit instCnt,

    // is the index of the first vertex to draw.
    afxUnit baseVtx,

    // is the instance ID of the first instance to draw.
    afxUnit baseInst
);

/// Draw primitives with indirect parameters.
/// AvxCmdDrawIndirect behaves similarly to AvxCmdDraw except that the parameters are read by the device from a buffer during execution.
/// @drawCount draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndirectCmd structures.
/// If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxError AvxCmdDrawIndirect
(
    afxDrawContext dctx,

    // is the buffer containing draw parameters.
    avxBuffer buf,

    // is the byte offset into @buf where parameters begin.
    afxUnit32 offset,

    // is the number of draws to execute, and can be zero.
    afxUnit32 drawCnt,

    // is the byte stride between successive sets of draw parameters.
    afxUnit32 stride
);

/// Draw primitives with indirect parameters and draw count.
/// AvxCmdDrawIndirect2 behaves similarly to AvxCmdDrawIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxError AvxCmdDrawIndirect2
(
    afxDrawContext dctx,

    // is the buffer containing draw parameters.
    avxBuffer buf,

    // is the byte offset into buffer where parameters begin.
    afxUnit32 offset,

    // is the buffer containing the draw count.
    avxBuffer cntBuf,

    // is the byte offset into @cntBuf where the draw count begins.
    afxUnit32 cntBufOff,

    // specifies the maximum number of draws that will be executed. 
    // The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32 maxDrawCnt,

    // is the byte stride between successive sets of draw parameters.
    afxUnit32 stride
);

/// Draw primitives with indexed vertices. 
/// When the command is executed, primitives are assembled using the current primitive topology and @idxCnt vertices whose indices are retrieved from the index buffer.
/// The index buffer is treated as an array of tightly packed unsigned integers of size defined by the @idxSiz parameter (of AvxCmdBindIndexBuffer) with which the buffer was bound.

/// The first vertex index is at an offset of @firstIdx × @idxSiz + @offset within the bound index buffer, where @offset is the offset specified by AvxCmdBindIndexBuffer, and @idxSize is the byte size of the type specified by @idxSiz.
/// Subsequent index values are retrieved from consecutive locations in the index buffer.
/// Indices are first compared to the primitive restart value, then zero extended to 32 bits (if the @idxSiz is 1 or 2) and have @vtxOff added to them, before being supplied as the @vtxIdx value.

/// The primitives are drawn @instCnt times with @instIdx starting with @firstInst and increasing sequentially for each instance.
/// The assembled primitives execute the bound graphics pipeline.

AVX afxError AvxCmdDrawIndexed
(
    afxDrawContext dctx,

    // is the number of vertices to draw.
    afxUnit idxCnt,

    // is the number of instances to draw.
    afxUnit instCnt,

    // is the base index within the index buffer.
    afxUnit baseIdx,

    // is the value added to the vertex index before indexing into the vertex buffer.
    afxUnit vtxOffset,

    // is the instance ID of the first instance to draw.
    afxUnit baseInst
);

/// Draw primitives with indirect parameters and indexed vertices.
/// AvxCmdDrawIndexedIndirect behaves similarly to AvxCmdDrawIndexed except that the parameters are read by the device from a buffer during execution.
/// @drawCnt draws are executed by the command, with parameters taken from @buf starting at @offset and increasing by @stride bytes for each successive draw.
/// The parameters of each draw are encoded in an array of afxDrawIndexedIndirectCmd structures. If @drawCnt is less than or equal to one, @stride is ignored.

AVX afxError AvxCmdDrawIndexedIndirect
(
    afxDrawContext dctx,

    // is the buffer containing draw parameters.
    avxBuffer buf,

    // is the byte offset into buffer where parameters begin.
    afxUnit32 offset,

    // is the number of draws to execute, and can be zero.
    afxUnit32 drawCnt,

    // is the byte stride between successive sets of draw parameters.
    afxUnit32 stride
);

/// Draw parameters with indirect parameters, indexed vertices, and draw count.
/// AvxCmdDrawIndexedIndirect2 behaves similarly to AvxCmdDrawIndexedIndirect except that the draw count is read by the device from a buffer during execution.
/// The command will read an unsigned 32-bit integer from @cntBuf located at @cntBufOff and use this as the draw count.

AVX afxError AvxCmdDrawIndexedIndirect2
(
    afxDrawContext dctx,

    // is the buffer containing draw parameters.
    avxBuffer buf,

    // is the byte offset into @buf where parameters begin.
    afxUnit32 offset,

    // is the buffer containing the draw count.
    avxBuffer cntBuf,

    // is the byte offset into @cntBuf where the draw count begins.
    afxUnit32 cntBufOff,

    // specifies the maximum number of draws that will be executed. 
    // The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
    afxUnit32 maxDrawCnt,

    // is the byte stride between successive sets of draw parameters.
    afxUnit32 stride
);

/*
    Set primitive topology state dynamically for a draw context.
    This command sets the primitive topology for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without primitive topology set.
*/

AVX afxError AvxCmdSetPrimitiveTopology
(
    afxDrawContext dctx,

    // The primitive topology to use for drawing.
    avxTopology topology
);

// Set the viewport dynamically for a draw context.
// This command sets the viewport transformation parameters state for subsequent drawing commands when the graphics pipeline is created without viewport set.

// The viewport parameters taken from element #i of @vp replace the current state for the viewport index @baseIdx + #i, for #i in[0, @cnt).

AVX afxError AvxCmdAdjustViewports
(
    afxDrawContext dctx,

    // The index of the first viewport whose parameters are updated by the command.
    afxUnit baseIdx,

     // The number of viewports whose parameters are updated by the command.
    afxUnit cnt,

     // An array of avxViewport structures specifying viewport parameters.
    avxViewport const viewports[]
);

// Set cull mode dynamically for a draw context.

AVX afxError AvxCmdAlterCullMode
(
    afxDrawContext dctx,

    // specifies the cull mode property to use for drawing.
    avxCullMode mode
);

// Set front face orientation dynamically for a draw context.

AVX afxError AvxCmdSwitchFrontFace
(
    afxDrawContext dctx,

    // Specifying if the front-facing triangle orientation to be used for culling is CW otherwise it is CCW.
    afxBool cw
);

#endif//AVX_TRANSFORMATION_H
