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

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_BUS_EXT_H
#define AVX_BUS_EXT_H

#include "qwadro/draw/avxBus.h"

/*
    The AvxCmdBindShadersEXT() method directly binds individual shader objects to specific shader stages in the current draw context.
    You are not binding a full pipeline here; you are binding individual shaders. Each one is separately bound.
    This provides a programmable, flexible, OpenGL-like shader binding model instead of requiring all stages to be packaged into a monolithic pipeline.

*/

AVX afxError AvxCmdBindShadersEXT
(
    afxDrawContext dctx,

    // The number of the stages to change.
    afxUnit cnt,

    // An array of values specifying the stage for each shader.
    avxShaderType const stages[],

    // An array of avxShader handles describing the shader object to be bound.
    avxShader shaders[]
);



//////////////////
// EXPERIMENTS  //
//////////////////

AVX afxError AvxCmdBindFontSIG(afxDrawContext dctx, afxUnit first, afxUnit cnt, /*afxTypography*/void* typ[], avxPipeline pip[], avxSampler smp[], avxRaster ras[]);
AVX afxError AvxCmdStampDebug(afxDrawContext dctx, afxM4d const v, afxV2d const at, afxString const* caption);

/*
    void vkCmdBindDescriptorBuffersEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    bufferCount,
    const VkDescriptorBufferBindingInfoEXT*     pBindingInfos);

    vkCmdBindDescriptorBuffersEXT causes any offsets previously set by vkCmdSetDescriptorBufferOffsetsEXT that use the bindings numbered
    [0.. bufferCount-1] to be no longer valid for subsequent bound pipeline commands. Any previously bound buffers at binding points
    greater than or equal to bufferCount are unbound.

    void vkCmdSetDescriptorBufferOffsetsEXT(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    setCount,
    const uint32_t*                             pBufferIndices,
    const VkDeviceSize*                         pOffsets);

    The vkCmdSetDescriptorBufferOffsetsEXT() binds setCount pairs of descriptor buffers, specified by indices into the binding points bound using
    vkCmdBindDescriptorBuffersEXT, and buffer offsets to set numbers [firstSet..firstSet+descriptorSetCount-1] for subsequent bound
    pipeline commands set by pipelineBindPoint. Set [firstSet + i] is bound to the descriptor buffer at binding pBufferIndices[i] at an
    offset of pOffsets[i]. Any bindings that were previously applied via these sets, or calls to vkCmdBindDescriptorSets, are no longer valid.
*/

/*
    The AvxCmdBindArgumentBuffersSIGMA() command binds argument buffers to a draw context. It causes any offsets previously set by
    AvxCmdPushUniformsSIGMA() that use the bindings numbered [ 0, @cnt - 1 ] to be no longer valid for subsequent bound pipeline
    commands. Any previously bound buffers at binding points greater than or equal to @cnt are unbound.
*/

AVX afxError AvxCmdBindArgumentBuffersSIGMA
(
    afxDrawContext  dctx,

    // the first buffer slot.
    afxUnit bufIdx,

    // the number of buffers.
    afxUnit cnt,

    // an array of avxBuffer objects.
    avxBufferedMap buffers[]
);

AVX afxError AvxCmdPushUniformsSIGMA
(
    afxDrawContext dctx,

    avxBus bus,

    afxUnit set,

    afxUnit binding,

    void const* data,

    afxUnit dataSiz
);


#endif//AVX_BUS_EXT_H
