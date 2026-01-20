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

// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_FORMAT_H
#define ACX_FORMAT_H

#include "qwadro/pro/afxStepDefs.h"

#define ACX_BASE_VIDEO_FORMAT 100

typedef enum acxFormat
// Sound sample formats.
{
    acxFormat_M32f = 1, // 32-bit float amplitude sample [-1.f, 1.f]
    acxFormat_S32f, // 32-bit float interleaved amplutide block [-1.f, 1.f]
    acxFormat_M32i, // 32-bit signed int amplitude sample [-2147483648, 2147483647]
    acxFormat_S32i, // 32-bit signed int interleaved amplutide block [-2147483648, 2147483647]
    acxFormat_M24i, // 24-bit signed int amplitude sample [-8388608, 8388607]
    acxFormat_S24i, // 24-bit signed int interleaved amplutide block [-8388608, 8388607]
    acxFormat_M16i, // 16-bit signed int amplitude sample [-32768, 32767]
    acxFormat_S16i, // 16-bit signed int interleaved amplutide block [-32768, 32767]
    acxFormat_M8i,  // 8-bit amplitude sample [0, 255]
    acxFormat_S8i,  // 8-bit interleaved amplutide block [0, 255]

    // Qwadro video signal formats and its correspondence in Vulkan.
    acxFormat_YVYU8_422 = ACX_BASE_VIDEO_FORMAT, // G8B8G8R8_422
    acxFormat_VYUY8_422, // B8G8R8G8_422
    acxFormat_YVU8_420P3, // G8_B8_R8_3PLANE_420
    acxFormat_YVU8_420P2, // G8_B8R8_2PLANE_420
    acxFormat_YVU8_422P3, // G8_B8_R8_3PLANE_422
    acxFormat_YVU8_422P2, // G8_B8R8_2PLANE_422
    acxFormat_YVU8_444P3, // G8_B8_R8_3PLANE_444
    acxFormat_Y10X6, // R10X6_PACK16
    acxFormat_UY10X6, // R10X6G10X6_2PACK16
    acxFormat_UYVA10X6, // R10X6G10X6B10X6A10X6_4PACK16
    acxFormat_YVYU10X6_422, // G10X6B10X6G10X6R10X6_422_4PACK16
    acxFormat_VYUY10X6_422, // B10X6G10X6R10X6G10X6_422_4PACK16
    acxFormat_YVU10X6_420P3, // G10X6_B10X6_R10X6_3PLANE_420_3PACK16
    acxFormat_YVU10X6_420P2, // G10X6_B10X6R10X6_2PLANE_420_3PACK16
    acxFormat_YVU10X6_422P3, // G10X6_B10X6_R10X6_3PLANE_422_3PACK16
    acxFormat_YVU10X6_422P2, // G10X6_B10X6R10X6_2PLANE_422_3PACK16
    acxFormat_YVU10X6_444P3, // G10X6_B10X6_R10X6_3PLANE_444_3PACK16
    acxFormat_Y12X4, // R12X4_PACK16
    acxFormat_UY12X4, // R12X4G12X4_2PACK16
    acxFormat_UYVA12X4, // R12X4G12X4B12X4A12X4_4PACK16
    acxFormat_YVYU12X4_422, // G12X4B12X4G12X4R12X4_422_4PACK16
    acxFormat_VYUY12X4_422, // B12X4G12X4R12X4G12X4_422_4PACK16
    acxFormat_YVU12X4_420P3, // G12X4_B12X4_R12X4_3PLANE_420_3PACK16
    acxFormat_YVU12X4_420P2, // G12X4_B12X4R12X4_2PLANE_420_3PACK16
    acxFormat_YVU12X4_422P3, // G12X4_B12X4_R12X4_3PLANE_422_3PACK16
    acxFormat_YVU12X4_422P2, // G12X4_B12X4R12X4_2PLANE_422_3PACK16
    acxFormat_YVU12X4_444P3, // G12X4_B12X4_R12X4_3PLANE_444_3PACK16
    acxFormat_YVYU16_422, // G16B16G16R16_422
    acxFormat_VYUY16_422, // B16G16R16G16_422
    acxFormat_YVU16_420P3, // G16_B16_R16_3PLANE_420
    acxFormat_YVU16_420P2, // G16_B16R16_2PLANE_420
    acxFormat_YVU16_422P3, // G16_B16_R16_3PLANE_422
    acxFormat_YVU16_422P2, // G16_B16R16_2PLANE_422
    acxFormat_YVU16_444P3, // G16_B16_R16_3PLANE_444
    acxFormat_YVU8_444P2, // G8_B8R8_2PLANE_444
    acxFormat_YVU10X6_444P2, // G10X6_B10X6R10X6_2PLANE_444_3PACK16
    acxFormat_YVU12X4_444P2, // G12X4_B12X4R12X4_2PLANE_444_3PACK16
    acxFormat_YVU16_444P2, // G16_B16R16_2PLANE_444

    acxFormat_TOTAL
} acxFormat;


typedef enum acxFormatType
{
    acxFormatType_UNK,
    // Usually padding.
    acxFormatType_UNUSED,
    // Unsigned normalized.
    acxFormatType_UN,
    // Signed normalized.
    acxFormatType_SN,
    // Unsigned int.
    acxFormatType_U,
    // Signed int.
    acxFormatType_I,
    // Single precision floating point; (32-bit)
    acxFormatType_F,
    // Signed scaled.
    acxFormatType_SS,
    // Unsigned scaled.
    acxFormatType_US,
    // Fixed point.
    acxFormatType_SFIXED,

    acxFormatType_TOTAL
} acxFormatType;

typedef enum acxFormatFlag
// Bitmasl flags specifying characteristics of a data format.
{
    // Format represents RGBA-like color data.
    acxFormatFlag_COLOR = AFX_BITMASK(0),
    // Format represents rasterization depth data.
    acxFormatFlag_DEPTH = AFX_BITMASK(1),
    // Format represents rasterization stencil data.
    acxFormatFlag_STENCIL = AFX_BITMASK(2),
    // Format represents luminance (grayscale) information.
    acxFormatFlag_LUMA = AFX_BITMASK(3),
    // Format represents chrominance (color difference) info.
    acxFormatFlag_CHROMA = AFX_BITMASK(4),
    // Format represents non-linear color data in sRGB color space;
    acxFormatFlag_sRGB = AFX_BITMASK(5),
    // Format represents block-compressed texel (e.g., DXT, ASTC).
    acxFormatFlag_BC = AFX_BITMASK(6),
    // Format supports mipmaps.
    acxFormatFlag_MIP = AFX_BITMASK(7),
    // Format represents normalized data (e.g., unsigned/signed normalized).
    acxFormatFlag_NORM = AFX_BITMASK(8),

    // Format has more than one wave with samples entangled in storage.
    acxFormatFlag_TANGLED   = AFX_BITMASK(9),
    // Format has more than one wave used to produce spatial perception.
    acxFormatFlag_STEREO    = AFX_BITMASK(10)
} acxFormatFlags;

// 153 big fishes.

typedef enum acxFormatUsage
// Bitmask flags specifying capabilities (or possible usages) of a data format,
// essentially, how a format can be used, in contrast to acxFormatFlags, which describes what a format represents.
{
    // Can be used as a source for copy operations.
    acxFormatUsage_COPY_SRC = AFX_BITMASK(0),
    // Can be used as a destination for copy operations.
    acxFormatUsage_COPY_DST = AFX_BITMASK(1),
    // Can be used as source and/or destination for copy operations.
    acxFormatUsage_COPY = (acxFormatUsage_COPY_SRC | acxFormatUsage_COPY_DST),

    // Can be used as blit (filtering) source for blit operations.
    acxFormatUsage_BLIT_SRC = AFX_BITMASK(2),
    // Can be used as blit destination for blit operations.
    acxFormatUsage_BLIT_DST = AFX_BITMASK(3),
    // Can be used as source and/or destination for blit operations.
    acxFormatUsage_BLIT = (acxFormatUsage_BLIT_SRC | acxFormatUsage_BLIT_DST),

    // Can be (attached to a mixer and) used as mixage target.
    acxFormatUsage_MIX = AFX_BITMASK(4),
    // Can be used as mixage target using blending operations.
    acxFormatUsage_BLEND = AFX_BITMASK(5),
    // Can be used to be mixed into and composed onto for mixing operations.
    acxFormatUsage_COMPOSE = (acxFormatUsage_MIX | acxFormatUsage_BLEND),

    // Can be used as a (read-only) sampled image/texture.
    acxFormatUsage_TEXTURE = AFX_BITMASK(6),
    // Can be used by shader to fetch, load and/or store image.
    acxFormatUsage_STORAGE = AFX_BITMASK(7),
    // Can be used by shader to fetch from a (read-only) tensor buffer; e.g. texel buffer.
    acxFormatUsage_FETCH = AFX_BITMASK(8),
    // Can be used to fetch, load and/or store from a (RW) tensor buffer; e.g. storage texel buffer.
    acxFormatUsage_TENSOR = AFX_BITMASK(9),
    // Can be used to stream vertices; e.g. vertex buffer.
    acxFormatUsage_VERTEX = AFX_BITMASK(10),
} acxFormatUsage;

AFX_DEFINE_STRUCT(acxFormatDescription)
// Structure describing a data format; how data is laid out, interpreted, and how it can be used.
{
    // Total bits per sample. For compressed formats, this might be per block.
    afxUnit32       bps;
    // Bytes per sample (total size per element, including padding).
    afxUnit32       stride;
    // Number of actual components (1–4); e.g., 3 for RGB, 4 for RGBA.
    afxUnit32       compCnt;
    // Type of each component (e.g., UINT8, FLOAT32, etc.).
    acxFormatType   type[4];
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
    acxFormatFlags  flags;
    // Capabilities as a sampled audio.
    acxFormatUsage  audCaps;
    // Capabilities as a buffer.
    acxFormatUsage  bufCaps;
    // Human-readable name (e.g., "RGBA8_UNORM").
    afxString       tag;
};

ACX void AcxDescribeFormats(afxUnit cnt, acxFormat const formats[], acxFormatDescription pfd[]);

#endif//ACX_FORMAT_H
