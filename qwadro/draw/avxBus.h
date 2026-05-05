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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DRAWING DEVICE EXECUTION BUS                                      //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_BUS_H
#define AVX_BUS_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/base/afxUri.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxFormat.h"
#include "qwadro/draw/avxColor.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/draw/avxRasterization.h"

typedef enum avxBus
// Enumerant specifing a pipelining bus.
// In AVX, a pipelining bus is a distinct set of conductors carrying data and control signals within a graphics device, 
// to which pieces of pipeline state may be connected in parallel.
{
    // Specifies the rasterization/graphics bus.
    avxBus_GFX,
    // Specifies the compute bus.
    avxBus_PCX,
    // Specifies the ray tracing bus.
    avxBus_RTX,

    avxBus_TOTAL
} avxBus;

typedef enum avxBusStage
// Bitmask-based value specifying pipeline stages.
{
    // NIL,
    // Pseudo-stage indicating execution on the host of reads/writes of device memory. 
    // This stage is not invoked by any commands recorded in a draw context.
    avxBusStage_HOST        = AFX_BITMASK(0),

    // All operations performed by all commands supported on the queue it is used with.
    // This is a general "catch-all" stage that includes all commands in the pipeline.
    // Often used for full synchronization when you're not sure what stages are involved.
    avxBusStage_COMMANDS    = AFX_BITMASK(1),

    // First synchronization scope: Source of the synchronization (where an operation happens-before).
    // Second synchronization scope: Destination of the synchronization (where the synchronized operation is allowed to begin).
    // This is handled via pipeline stage masks and access masks, and the new synchronization APIs.
    // In the first scope (srcStageMask):
    //  - Represents "before any commands in the pipeline".
    //  - No commands are considered to be in-flight yet.
    //  - Effectively means "nothing"; so it’s equivalent to avxBusStage == NIL.
    //  - Used when no previous stages are relevant for the sync.
    // In the second scope (dstStageMask):
    //  - Represents "before any commands start".
    //  - Since synchronization ensures something happens before the dstStageMask, this means the command being synchronized to can start only after all previous commands complete.
    //  - With accessMask = 0, it becomes a full memory barrier, effectively like avxBusStage_COMMANDS (but with some nuance; see below).
    //  - So in this context, it acts like a full barrier when paired with accessMask = 0.
    avxBusStage_TOP         = AFX_BITMASK(2),
    
    // First synchronization scope (srcStageMask): Source of the sync; the stages you're waiting on.
    // Second synchronization scope (dstStageMask): Destination of the sync; the stages that must wait.
    // In the first scope (srcStageMask):
    //  - Represents after all commands have completed.
    //  - So, it's a way to say "wait for everything to be done".
    //  - With accessMask = 0, this is effectively equivalent to avxBusStage_COMMANDS with no accessMask.
    //  - Acts as a full barrier (waits for all previous commands, but doesn't make any memory visible unless access mask is used).
    // In the second scope(dstStageMask):
    //  - avxBusStage_BOTTOM is the last possible stage in the pipeline.
    //  - So there's nothing that can execute after it.
    //  - Specifying this as the destination stage means : "synchronize to... nothing".
    //  - Thus, it is equivalent to avxBusStage == NIL.
    //  - No actual synchronization occurs. Nothing is delayed.
    avxBusStage_BOTTOM      = AFX_BITMASK(3),
    
    // The execution of all copy commands.
    avxBusStage_COPY        = AFX_BITMASK(4),
    
    // The execution of all resolve commands.
    avxBusStage_RESOLVE     = AFX_BITMASK(5),

    // The execution of all blit commands.
    avxBusStage_BLIT        = AFX_BITMASK(6),
    
    // The execution of clear/fill commands, with the exception of AmxCmdClearCanvas.
    avxBusStage_CLEAR       = AFX_BITMASK(7),
    
    // The following commands: copies, blits, resolves, clears.
    avxBusStage_TRANSFER    = (avxBusStage_COPY | avxBusStage_RESOLVE | avxBusStage_BLIT | avxBusStage_CLEAR),

    // The execution of a compute shader.
    avxBusStage_COMPUTE     = AFX_BITMASK(8),

    // Where DrawIndirect*/DispatchIndirect*/TraceRaysIndirect* data structures are consumed.
    avxBusStage_DRAW_IND    = AFX_BITMASK(9),
    
    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IDX     = AFX_BITMASK(10),

    // The stage of the pipeline where vertex buffers are consumed.
    avxBusStage_VTX_ATTR    = AFX_BITMASK(11),

    // The stage of the pipeline where index buffers are consumed.
    avxBusStage_VTX_IN      = (avxBusStage_VTX_IDX | avxBusStage_VTX_ATTR),

    // The vertex shader stage.
    avxBusStage_VERTEX      = AFX_BITMASK(12),

    // The tessellation control shader stage.
    avxBusStage_DOMAIN      = AFX_BITMASK(13),

    // The tessellation evaluation shader stage.
    avxBusStage_HULL        = AFX_BITMASK(14),

    // The primitive (aka geometry) shader stage.
    avxBusStage_PRIM        = AFX_BITMASK(15),

    // All supported pre-rasterization stages, including vertex, tesselation, primitive and mesh shaders.
    avxBusStage_PRERASTER   = (avxBusStage_VERTEX | avxBusStage_DOMAIN | avxBusStage_HULL | avxBusStage_PRIM),

    // where early fragment tests (depth and stencil tests before fragment shading) are performed. 
    // This stage also includes draw scope load operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_EARLY_TESTS = AFX_BITMASK(16),

    // The fragment shader stage.
    avxBusStage_FRAGMENT    = AFX_BITMASK(17),

    // Where late fragment tests (depth and stencil tests after fragment shading) are performed. 
    // This stage also includes draw scope store operations for canvas' attached buffers with a depth/stencil format.
    avxBusStage_LATE_TESTS  = AFX_BITMASK(18),

    // After blending, where the final color values are output from the pipeline. This stage includes blending, 
    // logic operations, draw scope load and store operations for color buffers, draw scope multisample resolve operations, 
    // and AvxCmdClearCanvas.
    avxBusStage_COLOR_OUT   = AFX_BITMASK(19),
        
    // The execution of all graphics pipeline stages.
    avxBusStage_GRAPHICS    = ( avxBusStage_DRAW_IND | 
                                avxBusStage_VTX_IN |
                                avxBusStage_VERTEX |
                                avxBusStage_DOMAIN |
                                avxBusStage_HULL |
                                avxBusStage_PRIM |
                                avxBusStage_FRAGMENT |
                                avxBusStage_EARLY_TESTS |
                                avxBusStage_LATE_TESTS |
                                avxBusStage_COLOR_OUT),

    avxBusStage_VIDDEC      = AFX_BITMASK(20),

    avxBusStage_VIDENC      = AFX_BITMASK(21),
} avxBusStage;

typedef enum avxBusAccess
{
    avxBusAccess_NONE,
    // All read accesses, always valid in any access mask, and is treated as equivalent 
    // to setting all READ access flags that are valid where it is used.
    avxBusAccess_MEMORY_R = AFX_BITMASK(0),
    // All write accesses, always valid in any access mask, and is treated as equivalent 
    // to setting all WRITE access flags that are valid where it is used.
    avxBusAccess_MEMORY_W = AFX_BITMASK(1),
    // read access to indirect command data read as part of an indirect build, trace, drawing or 
    // dispatching command. Such access occurs in the DRAW_INDIRECT pipeline stage.
    avxBusAccess_INDIRECT_R = AFX_BITMASK(2),
    // read access to an index buffer as part of an indexed drawing command. 
    // Such access occurs in the VERTEX_INPUT pipeline stage.
    avxBusAccess_INDEX_R = AFX_BITMASK(3),
    // read access to a vertex buffer as part of a drawing command. 
    // Such access occurs in the VERTEX_INPUT_BIT pipeline stage.
    avxBusAccess_VERTEX_R = AFX_BITMASK(4),
    // read access to a uniform buffer in any shader pipeline stage.
    avxBusAccess_UNIFORM_R = AFX_BITMASK(5),
    // read access to an input attachment within a render pass during subpass shading or fragment shading.
    avxBusAccess_INPUT_ATTACH_R = AFX_BITMASK(6),
    // read access to a uniform texel buffer, sampled image, storage buffer, physical storage buffer, 
    // shader binding table, storage texel buffer, or storage image in any shader pipeline stage.
    avxBusAccess_SHADER_R = AFX_BITMASK(7),
    // write access to a storage buffer, physical storage buffer, storage texel buffer, or storage image in any shader pipeline stage.
    avxBusAccess_SHADER_W = AFX_BITMASK(8),
    avxBusAccess_SHADER = (avxBusAccess_SHADER_R | avxBusAccess_SHADER_W),

    // read access to a color attachment, such as via blending (other than advanced blend operations), 
    // logic operations or certain render pass load operations in the COLOR_OUT pipeline stage or via 
    // fragment shader tile image reads in the FRAGMENT_SHADER pipeline stage.
    avxBusAccess_COLOR_OUT_R = AFX_BITMASK(9),
    // write access to a color, resolve, or depth/stencil resolve attachment during a render pass or via 
    // certain render pass load and store operations. Such access occurs in the COLOR_OUT pipeline stage.
    avxBusAccess_COLOR_OUT_W = AFX_BITMASK(10),
    avxBusAccess_COLOR_OUT = (avxBusAccess_COLOR_OUT_R | avxBusAccess_COLOR_OUT_W),

    // read access to a depth/stencil attachment, via depth or stencil operations or certain render pass load 
    // operations in the EARLY_TESTS or LATE_TESTS pipeline stages or via fragment shader tile image reads 
    // in the FRAGMENT_SHADER pipeline stage.
    avxBusAccess_DEPTH_OUT_R = AFX_BITMASK(11),
    // write access to a depth/stencil attachment, via depth or stencil operations or certain render pass load and store operations. 
    // Such access occurs in the EARLY_TESTS or LATE_TESTS pipeline stages.
    avxBusAccess_DEPTH_OUT_W = AFX_BITMASK(12),
    avxBusAccess_DEPTH_OUT = (avxBusAccess_DEPTH_OUT_R | avxBusAccess_DEPTH_OUT_W),

    // read access to an image or buffer in a copy operation. Such access occurs in ALL_TRANSFER pipeline stage.
    avxBusAccess_TRANSFER_R = AFX_BITMASK(13),
    // write access to an image or buffer in a clear or copy operation. Such access occurs in the ALL_TRANSFER_BIT pipeline stage.
    avxBusAccess_TRANSFER_W = AFX_BITMASK(14),
    avxBusAccess_TRANSFER = (avxBusAccess_TRANSFER_R | avxBusAccess_TRANSFER_W),

    // read access by a host operation. Accesses of this type are not performed through a resource, but directly on memory. 
    // Such access occurs in the HOST pipeline stage.
    avxBusAccess_HOST_R = AFX_BITMASK(15),
    // write access by a host operation. Accesses of this type are not performed through a resource, but directly on memory. 
    // Such access occurs in the HOST pipeline stage.
    avxBusAccess_HOST_W = AFX_BITMASK(16),
    avxBusAccess_HOST = (avxBusAccess_HOST_R | avxBusAccess_HOST_W)
} avxBusAccess;

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(avxDispatchIndirect)
/// Structure specifying a indirect dispatching command.
{
    afxUnit32   w, h, d; // the number of warps (local workgroups) to dispatch in the X, Y and Z dimensions.
};

#pragma pack(pop)

  //////////////////////////////////////////////////////////////////////////////
 //// REPROGRAMMING                                                        ////
//////////////////////////////////////////////////////////////////////////////

/*
    The AvxCmdBindPipeline() method binds a pipeline object to a draw context so that subsequent draw/dispatch
    commands use the pipeline's state until it is replaced or disrupted.
    By binding, the pipeline will typically sets the context with:
     - programmable shaders
     - fixed-function state
     - input assembly or vertex input state
     - rasterization/compute configuration
     - blending, depth/stencil, etc.
    Once bound, it defines how future graphics or compute commands behave.

    Pipeline state is sticky: it stays active until replaced.
    If you bind another pipeline with the same bus (e.g., another graphics pipeline), the previous one is overridden.
    Binding a shader object explicitly (if the API allows separate shader objects) may invalidate or override
    part of the pipeline's state, forcing a re-bind or reconfiguration.
    This ensures that draw commands know which pipeline they belong to.

    The affected bus depends on pipeline type. So, binding a graphics pipeline does not affect compute commands.
    And, binding a compute pipeline does not affect draw commands.
    If you bind a graphics pipeline, then call a compute dispatch, it has no effect.
    If you bind a compute pipeline, then issue a draw call, it is invalid or ignored.

    The avxVertexInput argument is only meaningful for rasterizer/graphics pipelines.
    It defines: vertex buffer formats, attribute layouts, stride and offset, and per-vertex/per-instance input classification.

    The expected dynamic states tell the draw context which pipeline states will be provided dynamically instead of coming from the compiled pipeline object.

    The API may allow separate shader objects (similar to OpenGL or Vulkan's shader object extension).
    It does mean that if a shader object replaces part of the pipeline (e.g., fragment shader),
    then the pipeline’s internal state is considered modified or invalidated.
    The next draw call may require a new pipeline or rebinding.
    This is a special case and indicates hybrid pipeline/shader binding rules.


*/

AVX afxError AvxCmdBindPipeline
(
    // The draw context that the pipeline will be bound to. 
    afxDrawContext dctx,

    // The pipeline to be bound.
    avxPipeline pip,

    // The vertex input to be bound when the pipeline is a rasterizer.
    avxVertexInput vin,

    // Flags specifying expected dynamic states.
    afxFlags dynamics
);

  //////////////////////////////////////////////////////////////////////////////
 //// RESOURCING                                                           ////
//////////////////////////////////////////////////////////////////////////////

AVX afxError AvxCmdDeclareBarrier
(
    afxDrawContext dctx,

    avxBusStage dstStage,

    avxBusAccess dstAcc
);

/*
    The AvxCmdUseLigature() method binds a ligature object to a specified bus within a draw context.
    A ligature in this rendering framework is a resource binding layout.
    It packages multiple shader-accessible resource interfaces together into a single bindable unit.

    The name "ligature" matches the concept: it ties ("ligates") multiple resource bindings into one object.
    The word "ligature" is the latin term for "binding". The target audience of this API is latin-based speaker.

    Like pipeline binding or shader binding, ligature usage is sticky.
    That is, all subsequent draw/dispatch commands use the resources from the currently bound ligature until a different one is bound.
*/

AVX afxError AvxCmdUseLigature
(
    afxDrawContext dctx,

    // A value specifying to which bus the ligature will be bound.
    avxBus bus,

    // The ligature to be bound.
    avxLigature liga,

    // Reserved flags.
    afxFlags flags
);

/*
    The AvxCmdBindBuffers() method updates buffer descriptors inside a ligature bound to a bus,
    but does so directly inside the command buffer, rather than rebuilding the ligature externally.

    In other words, it updates (in-place) buffer bindings for a specific descriptor set and binding index (pin) located on a specific bus.
    This is a very flexible, runtime-friendly descriptor update mechanism.

    @maps is nullable. Doing so, it unbinds existing descriptors, or leaves them unchanged (depending on implementation),
    or resets them to a null resource.

    General compatibility rules:
    - Bound ligature (on bus) must have a layout matching the descriptor set and binding ranges being updated.
    - Descriptor type at set, pin must be compatible with buffer descriptors (uniform, storage, etc.).
    - cnt must not exceed the descriptor array length for that binding.
*/

AVX afxError AvxCmdBindBuffers
(
    afxDrawContext dctx,

    // A value specifying to which bus the ligature will be bound.
    avxBus bus,

    // The destination descriptor set to update.
    afxUnit set,

    // The descriptor binding within that set.
    afxUnit pin,

    // The number of descriptors to update. 
    afxUnit cnt,

    // An nullable array of avxBufferedMap structures to be used to update the set.
    avxBufferedMap const maps[]
);

/*
    The AvxCmdBindRasters() method updates raster/image descriptors in a descriptor set of a ligature associated with a bus.
    In simple terms, it binds @cnt raster objects to the descriptor binding starting at (set, pin) in the ligature bound to bus.

    @rasters is nullable. Doing so, it unbinds existing images, or clears descriptors to "null texture".
    You would typically only pass NULL when intentionally invalidating a descriptor.

    Compatibility is layout-dependent. The ligature's descriptor layout determines:
     - whether this binding expects sampled images, storage images, input attachments, etc.
     - how many descriptors it supports
     - their array sizes and types

    AvxCmdBindRasters must obey that layout; exactly like Vulkan's descriptor compatibility rules.
*/

AVX afxError AvxCmdBindRasters
(
    afxDrawContext dctx,

    // A value specifying to which bus the ligature will be bound.
    avxBus bus,

    // The destination descriptor set to update.
    afxUnit set,

    // The descriptor binding within that set.
    afxUnit pin,

    // The number of descriptors to update. 
    afxUnit cnt,

    // An nullable array of avxRaster handles to be used to update the set.
    avxRaster const rasters[]
);

/*
    The AvxCmdBindSamplers() method updates sampler descriptors inside a descriptor set of a ligature bound to a bus.
    In simple terms, it writes @cnt sampler handles into the descriptor binding (set, pin) of the ligature bound to @bus.

    @samplers is nullable. Doing so, you unbind (reset) the sampler descriptor(s), or set them to a null/default sampler.
    This is useful for disabling sampling or forcing a resource rebind.

    Samplers and rasters are paired. Ligatures have parallel binding slots for them.
    The descriptor set layout in the active ligature must define whether the binding is a sampler-binding.
    The number (cnt) must match the descriptor array size. And the shader must expect a sampler at that binding location.
    Otherwise, the descriptor update is invalid.
*/

AVX afxError AvxCmdBindSamplers
(
    afxDrawContext dctx,

    // A value specifying to which bus the ligature will be bound.
    avxBus bus,

    // The destination descriptor set to update.
    afxUnit set,

    // The descriptor binding within that set.
    afxUnit pin,

    // The number of descriptors to update. 
    afxUnit cnt,

    // An nullable array of avxSampler handles to be used to update the set.
    avxSampler const samplers[]
);

/*
    The AvxCmdPushConstants() method copies a small block of immediate CPU-side data directly into the draw context's "push constant" memory region.
    In other words, it updates a small piece of fast, low-latency uniform data that the currently bound pipeline/shaders can read.
    These values can then be accessed by shaders during subsequent draw/dispatch commands without creating or updating buffers or descriptors.

    Push constants are ultra-fast per-command data.
    Push constants are:
     - Very small (typically a few hundred bytes total)
     - Extremely cheap to update
     - Meant for "scalar" or tiny structured data
     - Intended for per-draw / per-dispatch parameters

    They avoid the overhead of:
     - CPU-side buffer allocations
     - descriptor updates
     - memory barriers

    offset + siz specifies byte ranges in the push constant buffer
    This command gives complete byte-level control:
     - offset --- where in the push-constant region the data begins
     - siz --- number of bytes to copy
     - data --- pointer to the source memory

    This matches Vulkan, which allows partial updates of a larger push-constant block.

    They must match shader/pipeline push-constant layout
    The pipeline or shader object defines:
     - which stages can read push constants
     - the allowed size
     - the internal layout (struct layout)
    If @siz exceeds the pipeline's defined push-constant size,
    @offset + @siz goes out of bounds, or the shader expects a different layout,
    the behavior is invalid. Just like Vulkan, validation rules apply.

    Push constants persist until replaced.
    Once pushed, the data stays active until modified again or until the next pipeline/shader change resets them according to your backend rules.
    Usually, changing pipelines does not erase push constants; it only changes which subset of them is visible.

    Extremely cheap for small, high-frequency updates.
    Unlike buffer-based uniform updates:
     - No descriptor sets
     - No ligature rebinding
     - No memory allocations
     - No synchronization required
    This makes AvxCmdPushConstants ideal for rapidly changing parameters.
*/

AVX afxError AvxCmdPushConstants
(
    afxDrawContext dctx,

    // The offset where in the push-constant region the data begins.
    afxUnit offset,

    // The number of bytes to copy.
    afxUnit siz,

    // The pointer to the source memory.
    void const* data
);

  //////////////////////////////////////////////////////////////////////////////
 //// DISPATCH/COMPUTING                                                   ////
//////////////////////////////////////////////////////////////////////////////

AVX afxError AvxCmdDispatch
/// Dispatch compute work items.
(
    afxDrawContext dctx,

    // the number of local workgroups to dispatch in the X dimension.
    afxUnit w,

    // the number of local workgroups to dispatch in the Y dimension.
    afxUnit h,

    // the number of local workgroups to dispatch in the Z dimension.
    afxUnit d
);

AVX afxError AvxCmdDispatchIndirect
/// Dispatch compute work items with indirect parameters.
(
    afxDrawContext dctx,

    // the buffer containing dispatch parameters.
    avxBuffer buf,

    // the byte offset into buffer where parameters begin.
    afxUnit32 offset
);

  //////////////////////////////////////////////////////////////////////////////
 //// QUERYING                                                             ////
//////////////////////////////////////////////////////////////////////////////

AVX afxError AvxCmdBeginQuery
/// Begin a query. After beginning a query, that query is considered active within the draw context it was called in until that same query is ended.
(
    afxDrawContext dctx,

    // the query pool that will manage the results of the query.
    avxQueryPool pool,

    // the index within the query pool that will contain the results.
    afxUnit slot,

    // specifies the precision of occlusion queries.
    afxBool precise
);

AVX afxError AvxCmdEndQuery
/// Ends a query. After ending a query, that query is marked as available.
(
    afxDrawContext dctx,

    // the pool that is managing the results of the query.
    avxQueryPool pool,

    // the index within the query pool where the result is stored.
    afxUnit slot
);

AVX afxError AvxCmdCopyQueryResults
/// Copy the results of queries in a query pool to a buffer.
(
    afxDrawContext dctx,

    // the query pool managing the queries containing the desired results.
    avxQueryPool pool,

    // the initial query index.
    afxUnit baseSlot,

    // the number of queries. @baseQuery and @queryCnt together define a range of queries.
    afxUnit slotCnt,

    // a avxBuffer object that will receive the results of the copy command.
    avxBuffer buf,

    // an offset into dstBuffer.
    afxSize offset,

    // the stride in bytes between results for individual queries within dstBuffer.
    afxSize stride,

    avxQueryResultFlags flags
);

AVX afxError AvxCmdResetQueries
/// Reset queries in a query pool. When executed on a queue, this command sets the status of query indices [baseQuery, baseQuery + queryCnt - 1] to unavailable.
(
    afxDrawContext dctx,

    // the pool managing the queries being reset.
    avxQueryPool pool,

    // the initial query index to reset.
    afxUnit baseSlot,

    // the number of queries to reset.
    afxUnit slotCnt
);

AVX afxError AvxCmdQueryTimestamp
/// Write a device timestamp into a query object. When AvxCmdQueryTimestamp is submitted to a queue, it defines an execution dependency on commands that were submitted before it, and writes a timestamp to a query pool.
(
    afxDrawContext dctx,

    avxQueryPool pool,

    // the query that will contain the timestamp.
    afxUnit slot,

    // the stage of the pipeline.
    avxBusStage stage
);

#endif//AVX_BUS_H
