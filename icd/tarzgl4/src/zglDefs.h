/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ZGL_DEFS_H
#define ZGL_DEFS_H

//#define DISABLE_SHARED_HARDWARE_CONTEXT

#define _AVX_DRAW_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

//#define _AFX_MODULE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_DEVICE_IMPL
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_BRIDGE_IMPL
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_QUEUE_IMPL
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_SYSTEM_IMPL
#define _AVX_SURFACE_C
#define _AVX_SURFACE_IMPL
#define _AUX_DISPLAY_C
#define _AUX_DISPLAY_IMPL
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_INPUT_IMPL
#define _AVX_PIPELINE_C
#define _AVX_PIPELINE_IMPL
#define _AVX_BUFFER_C
#define _AVX_BUFFER_IMPL
#define _AVX_RASTER_C
#define _AVX_RASTER_IMPL
#define _AVX_RASTERIZER_C
#define _AVX_RASTERIZER_IMPL
#define _AVX_VERTEX_INPUT_C
#define _AVX_VERTEX_DECL_IMPL
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_CONTEXT_IMPL
#define _AVX_SAMPLER_C
#define _AVX_SAMPLER_IMPL
#define _AVX_SHADER_C
#define _AVX_SHADER_IMPL
#define _AVX_CANVAS_C
#define _AVX_CANVAS_IMPL
#define _AVX_LIGATURE_C
#define _AVX_BIND_SCHEMA_IMPL
#define _AVX_FENCE_C
#define _AVX_FENCE_IMPL
#define _AFX_SEMAPHORE_C
#define _AFX_SEMAPHORE_IMPL
#define _AVX_QUERY_POOL_C
#define _AVX_QUERY_POOL_IMPL

#include "zglInterop_WGL.h"

#define _ZGL_MAX_PUSH_CONSTANTS_SIZE 128

#define _ZGL_MAX_DOUT_PER_PRESENTATION 4
#define _ZGL_MAX_DSCR_PER_SUBMISSION 4
#define _ZGL_MAX_OBJ_PER_STREAMING 4
#define _ZGL_MAX_DQUE_PER_DCTX 4
#define _ZGL_MAX_SWAPCHAIN_CAP 3
#define _ZGL_PUSH_SWAPS 3
#define _ZGL_VAO_SWAPS 3
#define _ZGL_FBO_SWAPS 3
#define _ZGL_PSO_SWAPS 3
#define _ZGL_MAX_BUF_PER_DOUT 3
#define _ZGL_MAX_SET_PER_LIGA AVX_MAX_LIGAMENT_SETS
#define _ZGL_MAX_BIND_PER_SET AVX_MAX_LIGAMENTS
#define _ZGL_MAX_STAGE_PER_PIP 8
//#define _ZGL_MAX_VBO_PER_BIND 8

#define ZGL_MAX_VERTEX_ATTRIBS 16
#define ZGL_MAX_VERTEX_ATTRIB_BINDINGS 16
#define ZGL_MAX_VERTEX_ATTRIB_STRIDE 4096
#define ZGL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 4096

#define ZGL_MIN_MAP_BUFFER_ALIGNMENT 64

#define ZGL_MAX_VIEWPORTS 8
#define ZGL_VIEWPORT_BOUNDS_RANGE 0
#define ZGL_MAX_VIEWPORT_DIMS 0
#define ZGL_MAX_CANVAS_WIDTH AFX_U16_MAX
#define ZGL_MAX_CANVAS_HEIGHT AFX_U16_MAX
#define ZGL_MAX_CANVAS_LAYERS AFX_U16_MAX
#define ZGL_DEPTH_RANGE


#define ZGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 80
#define ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT (ZGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1)
#define ZGL_COPY_READ_RASTER_SLOT (ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT - 1)
#define ZGL_COPY_WRITE_RASTER_SLOT (ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT - 2)

#define _ZGL_MAX_VAO_PER_TIME 8
//#define _ZGL_MAX_VP_PER_SET 8
//#define _ZGL_MAX_SCISSOR_PER_SET 8
//#define _ZGL_MAX_INSTREAM_PER_SET 8
//#define _ZGL_MAX_RASTER_SURF_PER_CANV 8
#define ZGL_MAX_COLOR_ATTACHMENTS 8
#define _ZGL_MAX_SURF_PER_CANV ZGL_MAX_COLOR_ATTACHMENTS + 2

// force usage of recent functions.
#define FORCE_GL_GENERIC_FUNCS 1 // we also attempt to use specific functions as a medium of hinting for the driver.
#define ZGL_DONT_USE_ROBUST_ACCESS 1

#define FORCE_ARG_REBIND TRUE
//#define FORCE_VBO_REBIND TRUE
//#define FORCE_IBO_REBIND TRUE
// unbind GL buffers at end of each function; to avoid stalling.
#define _ALWAYS_UNBIND_GL_BUF TRUE
 //#define COHERENT_PUSHABLES
#define _USE_BUF_RW_SLOT TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_OPS TRUE
#if _USE_RW_SLOTS_ON_BUFFER_OPS
#define _USE_RW_SLOTS_ON_BUFFER_COPY TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_FILL TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_DUMP TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_UPDATE TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_DOWNLOAD TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_UPLOAD TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_RW_SLOTS_ON_BUFFER_MAPPING TRUE // what about VBO and IBOS without a bound VAO?
#endif//_USE_RW_SLOTS_ON_BUFFER_OPS
#define _USE_IO_SLOTS_ON_BUFFER_OPS TRUE
#if _USE_IO_SLOTS_ON_BUFFER_OPS
#define _USE_IO_SLOTS_ON_BUFFER_COPY TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_FILL TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_DUMP TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_UPDATE TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_DOWNLOAD TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_UPLOAD TRUE // what about VBO and IBOS without a bound VAO?
#define _USE_IO_SLOTS_ON_BUFFER_MAPPING TRUE // what about VBO and IBOS without a bound VAO?
#endif//_USE_IO_SLOTS_ON_BUFFER_OPS

// unbind GL textures at end of each function; to avoid issues with layout transition.
#define _ALWAYS_UNBIND_GL_TEX TRUE
// allow avxRaster to be backed as renderbuffer when it is only drawable.
//#define RENDERBUFFER_ENABLED TRUE
// generate a FBO at call site to do blit instead of using common shared FBOs.
#define _USE_GEN_FBO_ON_RASTER_BLIT TRUE
// generate a FBO at call site to do resolve instead of using common shared FBOs.
#define _USE_GEN_FBO_ON_RASTER_RESOLVE TRUE
// generate a FBO at call site to do clear instead of using common shared FBOs.
#define _USE_GEN_FBO_ON_RASTER_CLEAR TRUE

typedef enum zglUpdateFlags
{
    ZGL_UPD_FLAG_DEVICE_FLUSH   = AFX_BITMASK(0), // flush from host to device
    ZGL_UPD_FLAG_HOST_FLUSH     = AFX_BITMASK(1), // flush from device to host
    ZGL_UPD_FLAG_DEVICE_INST    = AFX_BITMASK(2), // (re)instantiate on device
    ZGL_UPD_FLAG_HOST_INST      = AFX_BITMASK(3), // (re)instantiate on host
    
    ZGL_UPD_FLAG_HOST           = (ZGL_UPD_FLAG_HOST_INST | ZGL_UPD_FLAG_HOST_FLUSH),
    ZGL_UPD_FLAG_DEVICE         = (ZGL_UPD_FLAG_DEVICE_INST | ZGL_UPD_FLAG_DEVICE_FLUSH),
} zglUpdateFlags;

AFX_DEFINE_UNION(zglLegoData) // A GPUBindGroupEntry describes a single resource to be bound in a GPUBindGroup, and has the following members:
{
    struct
    {
        avxBuffer           buf; // The GPUBuffer to bind.
        afxUnit              offset; // The offset, in bytes, from the beginning of buffer to the beginning of the range exposed to the shader by the buffer binding.
        afxUnit              range; // The size, in bytes, of the buffer binding. If undefined, specifies the range starting at offset and ending at the end of buffer.
    };
    struct
    {
        avxSampler          smp;
        avxRaster          tex;
    };
};

typedef struct _DrawPipelineResource
{
    avxShaderType stages;
    avxShaderParam resType;
    //afxShaerBaseResType baseType;
    //VkAccessFlags access;
    afxUnit8 set;
    afxUnit8 binding;
    afxUnit8 location;
    afxUnit8 inputAttachmentIndex;
    afxUnit8 vecSize;
    afxUnit8 arraySize;
    afxUnit offset;
    afxUnit size;
    afxChar name[16];
} _DrawPipelineResource;

typedef enum zglBindFlags
{
    zglBindFlag_BIND = AFX_BITMASK(0),
    zglBindFlag_SYNC = AFX_BITMASK(1),
    zglBindFlag_KEEP = AFX_BITMASK(2)
}zglBindFlags;

#endif//ZGL_DEFS_H
