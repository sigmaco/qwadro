/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_FORMAT_H
#define ASX_FORMAT_H

#include "qwadro/pro/afxStepDefs.h"

#define ASX_BASE_VIDEO_FORMAT 100

typedef enum asxFormat
// Sound sample formats.
{
    asxFormat_M32f = 1, // 32-bit float amplitude sample [-1.f, 1.f]
    asxFormat_S32f, // 32-bit float interleaved amplutide block [-1.f, 1.f]
    asxFormat_M32i, // 32-bit signed int amplitude sample [-2147483648, 2147483647]
    asxFormat_S32i, // 32-bit signed int interleaved amplutide block [-2147483648, 2147483647]
    asxFormat_M24i, // 24-bit signed int amplitude sample [-8388608, 8388607]
    asxFormat_S24i, // 24-bit signed int interleaved amplutide block [-8388608, 8388607]
    asxFormat_M16i, // 16-bit signed int amplitude sample [-32768, 32767]
    asxFormat_S16i, // 16-bit signed int interleaved amplutide block [-32768, 32767]
    asxFormat_M8i,  // 8-bit amplitude sample [0, 255]
    asxFormat_S8i,  // 8-bit interleaved amplutide block [0, 255]

    // Qwadro video signal formats and its correspondence in Vulkan.
    asxFormat_YVYU8_422 = ASX_BASE_VIDEO_FORMAT, // G8B8G8R8_422
    asxFormat_VYUY8_422, // B8G8R8G8_422
    asxFormat_YVU8_420P3, // G8_B8_R8_3PLANE_420
    asxFormat_YVU8_420P2, // G8_B8R8_2PLANE_420
    asxFormat_YVU8_422P3, // G8_B8_R8_3PLANE_422
    asxFormat_YVU8_422P2, // G8_B8R8_2PLANE_422
    asxFormat_YVU8_444P3, // G8_B8_R8_3PLANE_444
    asxFormat_Y10X6, // R10X6_PACK16
    asxFormat_UY10X6, // R10X6G10X6_2PACK16
    asxFormat_UYVA10X6, // R10X6G10X6B10X6A10X6_4PACK16
    asxFormat_YVYU10X6_422, // G10X6B10X6G10X6R10X6_422_4PACK16
    asxFormat_VYUY10X6_422, // B10X6G10X6R10X6G10X6_422_4PACK16
    asxFormat_YVU10X6_420P3, // G10X6_B10X6_R10X6_3PLANE_420_3PACK16
    asxFormat_YVU10X6_420P2, // G10X6_B10X6R10X6_2PLANE_420_3PACK16
    asxFormat_YVU10X6_422P3, // G10X6_B10X6_R10X6_3PLANE_422_3PACK16
    asxFormat_YVU10X6_422P2, // G10X6_B10X6R10X6_2PLANE_422_3PACK16
    asxFormat_YVU10X6_444P3, // G10X6_B10X6_R10X6_3PLANE_444_3PACK16
    asxFormat_Y12X4, // R12X4_PACK16
    asxFormat_UY12X4, // R12X4G12X4_2PACK16
    asxFormat_UYVA12X4, // R12X4G12X4B12X4A12X4_4PACK16
    asxFormat_YVYU12X4_422, // G12X4B12X4G12X4R12X4_422_4PACK16
    asxFormat_VYUY12X4_422, // B12X4G12X4R12X4G12X4_422_4PACK16
    asxFormat_YVU12X4_420P3, // G12X4_B12X4_R12X4_3PLANE_420_3PACK16
    asxFormat_YVU12X4_420P2, // G12X4_B12X4R12X4_2PLANE_420_3PACK16
    asxFormat_YVU12X4_422P3, // G12X4_B12X4_R12X4_3PLANE_422_3PACK16
    asxFormat_YVU12X4_422P2, // G12X4_B12X4R12X4_2PLANE_422_3PACK16
    asxFormat_YVU12X4_444P3, // G12X4_B12X4_R12X4_3PLANE_444_3PACK16
    asxFormat_YVYU16_422, // G16B16G16R16_422
    asxFormat_VYUY16_422, // B16G16R16G16_422
    asxFormat_YVU16_420P3, // G16_B16_R16_3PLANE_420
    asxFormat_YVU16_420P2, // G16_B16R16_2PLANE_420
    asxFormat_YVU16_422P3, // G16_B16_R16_3PLANE_422
    asxFormat_YVU16_422P2, // G16_B16R16_2PLANE_422
    asxFormat_YVU16_444P3, // G16_B16_R16_3PLANE_444
    asxFormat_YVU8_444P2, // G8_B8R8_2PLANE_444
    asxFormat_YVU10X6_444P2, // G10X6_B10X6R10X6_2PLANE_444_3PACK16
    asxFormat_YVU12X4_444P2, // G12X4_B12X4R12X4_2PLANE_444_3PACK16
    asxFormat_YVU16_444P2, // G16_B16R16_2PLANE_444

    asxFormat_TOTAL
} asxFormat;


typedef enum asxFormatType
{
    asxFormatType_UNK,
    // Usually padding.
    asxFormatType_UNUSED,
    // Unsigned normalized.
    asxFormatType_UN,
    // Signed normalized.
    asxFormatType_SN,
    // Unsigned int.
    asxFormatType_U,
    // Signed int.
    asxFormatType_I,
    // Single precision floating point; (32-bit)
    asxFormatType_F,
    // Signed scaled.
    asxFormatType_SS,
    // Unsigned scaled.
    asxFormatType_US,
    // Fixed point.
    asxFormatType_SFIXED,

    asxFormatType_TOTAL
} asxFormatType;

typedef enum asxFormatFlag
// Bitmasl flags specifying characteristics of a data format.
{
    // Format represents RGBA-like color data.
    asxFormatFlag_COLOR = AFX_BITMASK(0),
    // Format represents rasterization depth data.
    asxFormatFlag_DEPTH = AFX_BITMASK(1),
    // Format represents rasterization stencil data.
    asxFormatFlag_STENCIL = AFX_BITMASK(2),
    // Format represents luminance (grayscale) information.
    asxFormatFlag_LUMA = AFX_BITMASK(3),
    // Format represents chrominance (color difference) info.
    asxFormatFlag_CHROMA = AFX_BITMASK(4),
    // Format represents non-linear color data in sRGB color space;
    asxFormatFlag_sRGB = AFX_BITMASK(5),
    // Format represents block-compressed texel (e.g., DXT, ASTC).
    asxFormatFlag_BC = AFX_BITMASK(6),
    // Format supports mipmaps.
    asxFormatFlag_MIP = AFX_BITMASK(7),
    // Format represents normalized data (e.g., unsigned/signed normalized).
    asxFormatFlag_NORM = AFX_BITMASK(8),

    // Format has more than one wave with samples entangled in storage.
    asxFormatFlag_TANGLED   = AFX_BITMASK(9),
    // Format has more than one wave used to produce spatial perception.
    asxFormatFlag_STEREO    = AFX_BITMASK(10)
} asxFormatFlags;

// 153 big fishes.

typedef enum asxFormatUsage
// Bitmask flags specifying capabilities (or possible usages) of a data format,
// essentially, how a format can be used, in contrast to asxFormatFlags, which describes what a format represents.
{
    // Can be used as a source for copy operations.
    asxFormatUsage_COPY_SRC = AFX_BITMASK(0),
    // Can be used as a destination for copy operations.
    asxFormatUsage_COPY_DST = AFX_BITMASK(1),
    // Can be used as source and/or destination for copy operations.
    asxFormatUsage_COPY = (asxFormatUsage_COPY_SRC | asxFormatUsage_COPY_DST),

    // Can be used as blit (filtering) source for blit operations.
    asxFormatUsage_BLIT_SRC = AFX_BITMASK(2),
    // Can be used as blit destination for blit operations.
    asxFormatUsage_BLIT_DST = AFX_BITMASK(3),
    // Can be used as source and/or destination for blit operations.
    asxFormatUsage_BLIT = (asxFormatUsage_BLIT_SRC | asxFormatUsage_BLIT_DST),

    // Can be (attached to a mixer and) used as mixage target.
    asxFormatUsage_MIX = AFX_BITMASK(4),
    // Can be used as mixage target using blending operations.
    asxFormatUsage_BLEND = AFX_BITMASK(5),
    // Can be used to be mixed into and composed onto for mixing operations.
    asxFormatUsage_COMPOSE = (asxFormatUsage_MIX | asxFormatUsage_BLEND),

    // Can be used as a (read-only) sampled image/texture.
    asxFormatUsage_TEXTURE = AFX_BITMASK(6),
    // Can be used by shader to fetch, load and/or store image.
    asxFormatUsage_STORAGE = AFX_BITMASK(7),
    // Can be used by shader to fetch from a (read-only) tensor buffer; e.g. texel buffer.
    asxFormatUsage_FETCH = AFX_BITMASK(8),
    // Can be used to fetch, load and/or store from a (RW) tensor buffer; e.g. storage texel buffer.
    asxFormatUsage_TENSOR = AFX_BITMASK(9),
    // Can be used to stream vertices; e.g. vertex buffer.
    asxFormatUsage_VERTEX = AFX_BITMASK(10),
} asxFormatUsage;

AFX_DEFINE_STRUCT(asxFormatDescription)
// Structure describing a data format; how data is laid out, interpreted, and how it can be used.
{
    // Total bits per sample. For compressed formats, this might be per block.
    afxUnit32       bps;
    // Bytes per sample (total size per element, including padding).
    afxUnit32       stride;
    // Number of actual components (1–4); e.g., 3 for RGB, 4 for RGBA.
    afxUnit32       compCnt;
    // Type of each component (e.g., UINT8, FLOAT32, etc.).
    asxFormatType   type[4];
    // Mapping of RGBA channels (e.g., {0,1,2,3} for identity).
    afxUnit32       swizzle[4];
    // Default values for missing components (e.g., Alpha = 1).
    afxUnit32       defaults[4];
    // Whether each component is normalized to [0,1] or [-1,1].
    afxBool         isNormalized[4];
    // To-float scale factor (e.g., for unnormalized integers).
    afxReal         tof[4];
    // Bits per component.
    afxUnit32       bpc[4];
    // Block-compression width/height (e.g., {4,4} for BC/DXT formats).
    afxUnit32       bcWh[2];
    // What the format represents (e.g., COLOR, DEPTH, etc.).
    asxFormatFlags  flags;
    // Capabilities as a sampled audio.
    asxFormatUsage  audCaps;
    // Capabilities as a buffer.
    asxFormatUsage  bufCaps;
    // Human-readable name (e.g., "RGBA8_UNORM").
    afxString       tag;
};

ASX void AsxDescribeFormats(afxUnit cnt, asxFormat const formats[], asxFormatDescription pfd[]);

#endif//ASX_FORMAT_H
