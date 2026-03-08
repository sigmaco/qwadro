/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "zglDefs.h"
#include "zglCommands.h"
#include "zglObjects.h"
#include "zglUtils.h"

/*
Well-defined sized internal formats glTexStorage

GL_R8	GL_RED	8
GL_R8_SNORM	GL_RED	s8
GL_R16	GL_RED	16
GL_R16_SNORM	GL_RED	s16
GL_RG8	GL_RG	8	8
GL_RG8_SNORM	GL_RG	s8	s8
GL_RG16	GL_RG	16	16
GL_RG16_SNORM	GL_RG	s16	s16
GL_R3_G3_B2	GL_RGB	3	3	2
GL_RGB4	GL_RGB	4	4	4
GL_RGB5	GL_RGB	5	5	5
GL_RGB8	GL_RGB	8	8	8
GL_RGB8_SNORM	GL_RGB	s8	s8	s8
GL_RGB10	GL_RGB	10	10	10
GL_RGB12	GL_RGB	12	12	12
GL_RGB16_SNORM	GL_RGB	16	16	16
GL_RGBA2	GL_RGB	2	2	2	2
GL_RGBA4	GL_RGB	4	4	4	4
GL_RGB5_A1	GL_RGBA	5	5	5	1
GL_RGBA8	GL_RGBA	8	8	8	8
GL_RGBA8_SNORM	GL_RGBA	s8	s8	s8	s8
GL_RGB10_A2	GL_RGBA	10	10	10	2
GL_RGB10_A2UI	GL_RGBA	ui10	ui10	ui10	ui2
GL_RGBA12	GL_RGBA	12	12	12	12
GL_RGBA16	GL_RGBA	16	16	16	16
GL_SRGB8	GL_RGB	8	8	8
GL_SRGB8_ALPHA8	GL_RGBA	8	8	8	8
GL_R16F	GL_RED	f16
GL_RG16F	GL_RG	f16	f16
GL_RGB16F	GL_RGB	f16	f16	f16
GL_RGBA16F	GL_RGBA	f16	f16	f16	f16
GL_R32F	GL_RED	f32
GL_RG32F	GL_RG	f32	f32
GL_RGB32F	GL_RGB	f32	f32	f32
GL_RGBA32F	GL_RGBA	f32	f32	f32	f32
GL_R11F_G11F_B10F	GL_RGB	f11	f11	f10
GL_RGB9_E5	GL_RGB	9	9	9		5
GL_R8I	GL_RED	i8
GL_R8UI	GL_RED	ui8
GL_R16I	GL_RED	i16
GL_R16UI	GL_RED	ui16
GL_R32I	GL_RED	i32
GL_R32UI	GL_RED	ui32
GL_RG8I	GL_RG	i8	i8
GL_RG8UI	GL_RG	ui8	ui8
GL_RG16I	GL_RG	i16	i16
GL_RG16UI	GL_RG	ui16	ui16
GL_RG32I	GL_RG	i32	i32
GL_RG32UI	GL_RG	ui32	ui32
GL_RGB8I	GL_RGB	i8	i8	i8
GL_RGB8UI	GL_RGB	ui8	ui8	ui8
GL_RGB16I	GL_RGB	i16	i16	i16
GL_RGB16UI	GL_RGB	ui16	ui16	ui16
GL_RGB32I	GL_RGB	i32	i32	i32
GL_RGB32UI	GL_RGB	ui32	ui32	ui32
GL_RGBA8I	GL_RGBA	i8	i8	i8	i8
GL_RGBA8UI	GL_RGBA	ui8	ui8	ui8	ui8
GL_RGBA16I	GL_RGBA	i16	i16	i16	i16
GL_RGBA16UI	GL_RGBA	ui16	ui16	ui16	ui16
GL_RGBA32I	GL_RGBA	i32	i32	i32	i32
GL_RGBA32UI	GL_RGBA	ui32	ui32	ui32	ui32

*/

#if 0
_ZGL void ZglToGlFormat(avxFormat pf, GLint *fmt, GLenum *layout, GLenum *type)
{
    static struct { GLint intFmt; GLenum layout, type; } const v[] =
    {
        {   NIL,                    NIL,                NIL                     },

        {   GL_R8,                  GL_RED,             GL_UNSIGNED_BYTE        },
        {   GL_RG8,                 GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                GL_RGB,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,               GL_RGBA,            GL_UNSIGNED_BYTE        },

        {   GL_RG8,                 GL_RG,              GL_UNSIGNED_BYTE        },
        {   GL_RGB8,                GL_BGR,             GL_UNSIGNED_BYTE        },
        {   GL_RGBA8,               GL_BGRA,            GL_UNSIGNED_BYTE        },

        {   GL_R32F,                GL_RED,             GL_FLOAT                },
        {   GL_RG32F,               GL_RG,              GL_FLOAT                },
        {   GL_RGB32F,              GL_RGB,             GL_FLOAT                },
        {   GL_RGBA32F,             GL_RGBA,            GL_FLOAT                },

        {   GL_SRGB8,               GL_RGB,             GL_UNSIGNED_BYTE            },
        {   GL_SRGB8_ALPHA8,        GL_RGBA,            GL_UNSIGNED_BYTE            },
        {   GL_RGB9_E5,             GL_RGB,             GL_UNSIGNED_INT_5_9_9_9_REV },

        {   GL_RGBA4,               GL_RGBA,            GL_UNSIGNED_SHORT_4_4_4_4   },
        {   GL_RGB5_A1,             GL_RGBA,            GL_UNSIGNED_SHORT_5_5_5_1   },
        {   GL_RGB565,              GL_RGB,             GL_UNSIGNED_SHORT_5_6_5     },
        {   GL_RGB10_A2,            GL_RGBA,            GL_UNSIGNED_INT_10_10_10_2  },

        {   GL_STENCIL_INDEX8,      GL_STENCIL_INDEX,   GL_UNSIGNED_BYTE        },
        {   GL_DEPTH_COMPONENT16,   GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT       },
        {   GL_DEPTH_COMPONENT24,   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT         },
        {   GL_DEPTH_COMPONENT32,   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT         },
        {   GL_DEPTH_COMPONENT32F,  GL_DEPTH_COMPONENT, GL_FLOAT                },
        {   GL_DEPTH24_STENCIL8,    GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8    },
        {   GL_DEPTH32F_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8    },

        {  GL_COMPRESSED_RGB_S3TC_DXT1_EXT , GL_COMPRESSED_RGB_S3TC_DXT1_EXT , GL_COMPRESSED_RGB_S3TC_DXT1_EXT  },
        {  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT },
        {  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT },
        {  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT },
    };

    afxError err = { 0 };
    AFX_ASSERT(v[avxFormat_D32F].intFmt == GL_DEPTH_COMPONENT32F);
    AFX_ASSERT(v[avxFormat_D32].intFmt == GL_DEPTH_COMPONENT32);
    AFX_ASSERT(v[avxFormat_D24].intFmt == GL_DEPTH_COMPONENT24);
    AFX_ASSERT(v[avxFormat_D24S8].intFmt == GL_DEPTH24_STENCIL8);
    AFX_ASSERT(v[avxFormat_D32FS8].intFmt == GL_DEPTH32F_STENCIL8);

    *fmt = v[pf].intFmt;
    *layout = v[pf].layout;
    *type = v[pf].type;
}
#endif

static struct { GLint intFmt; GLenum layout, type; } vf[avxFormat_TOTAL] = { 0 };
#define MAKE_FMT(fmt, glFmt, glLayout, glType) vf[fmt].intFmt = glFmt, vf[fmt].layout = glLayout, vf[fmt].type = glType

_ZGL void ZglToGlFormat2(avxFormat pf, GLint *fmt, GLenum *layout, GLenum *type)
{
    static afxBool inited = FALSE;

    if (!inited)
    {
        inited = TRUE;

        //                              inverse of name
        //                              as a vector
        MAKE_FMT(avxFormat_RG4un,       GL_RGBA4,           GL_RG,      GL_UNSIGNED_SHORT_4_4_4_4);
        MAKE_FMT(avxFormat_RGBA4un,     GL_RGBA4,           GL_RGBA,    GL_UNSIGNED_SHORT_4_4_4_4_REV);
        MAKE_FMT(avxFormat_BGRA4un,     GL_RGBA4,           GL_RGBA,    GL_UNSIGNED_SHORT_4_4_4_4);
        MAKE_FMT(avxFormat_R5G6B5un,    GL_RGB565,          GL_RGB,     GL_UNSIGNED_SHORT_5_6_5_REV);
        MAKE_FMT(avxFormat_B5G6R5un,    GL_RGB565,          GL_RGB,     GL_UNSIGNED_SHORT_5_6_5);
        MAKE_FMT(avxFormat_RGB5A1un,    GL_RGB5_A1,         GL_RGBA,    GL_UNSIGNED_SHORT_5_5_5_1);
        MAKE_FMT(avxFormat_BGR5A1un,    GL_RGB5_A1,         GL_BGRA,    GL_UNSIGNED_SHORT_1_5_5_5_REV);
        MAKE_FMT(avxFormat_A1RGB5un,    GL_RGB5_A1,         GL_RGBA,    GL_UNSIGNED_SHORT_5_5_5_1);
    
        MAKE_FMT(avxFormat_R8un,        GL_R8,              GL_RED,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_R8sn,        GL_R8_SNORM,        GL_RED,     GL_BYTE);
        MAKE_FMT(avxFormat_R8us,        GL_R8,              GL_RED,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_R8ss,        GL_R8_SNORM,        GL_RED,     GL_BYTE);
        MAKE_FMT(avxFormat_R8u,         GL_R8UI,            GL_RED_INTEGER,GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_R8i,         GL_R8I,             GL_RED_INTEGER,GL_BYTE);
        MAKE_FMT(avxFormat_R8v,       GL_SRGB8,           GL_RED,     GL_UNSIGNED_BYTE);
        
        MAKE_FMT(avxFormat_RG8un,       GL_RG8,             GL_RG,      GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RG8sn,       GL_RG8_SNORM,       GL_RG,      GL_SHORT);
        MAKE_FMT(avxFormat_RG8us,       GL_RG8,             GL_RG,      GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RG8ss,       GL_RG8_SNORM,       GL_RG,      GL_SHORT);
        MAKE_FMT(avxFormat_RG8u,        GL_RG8UI,           GL_RG_INTEGER,GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RG8i,        GL_RG8I,            GL_RG_INTEGER,GL_SHORT);
        MAKE_FMT(avxFormat_RG8v,      GL_SRGB8,           GL_RG,      GL_UNSIGNED_SHORT);
        
        MAKE_FMT(avxFormat_RGB8un,      GL_RGB8,            GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8sn,      GL_RGB8_SNORM,      GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8us,      GL_RGB8,            GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8ss,      GL_RGB8_SNORM,      GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8u,       GL_RGB8I,           GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8i,       GL_RGB8UI,          GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGB8v,     GL_SRGB8,           GL_RGB,     GL_UNSIGNED_BYTE);
        
        MAKE_FMT(avxFormat_BGR8un,      GL_RGB8,            GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8sn,      GL_RGB8_SNORM,      GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8us,      GL_RGB8,            GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8ss,      GL_RGB8_SNORM,      GL_RGB,     GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8u,       GL_RGB8UI,          GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8i,       GL_RGB8I,           GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGR8v,     GL_SRGB8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        
        MAKE_FMT(avxFormat_RGBA8un,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8sn,     GL_RGBA8_SNORM,     GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8us,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8ss,     GL_RGBA8_SNORM,     GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8u,      GL_RGBA8UI,         GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8i,      GL_RGBA8I,          GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_RGBA8v,    GL_SRGB8_ALPHA8,    GL_RGBA,    GL_UNSIGNED_BYTE);
        
        // 32-bit float
        MAKE_FMT(avxFormat_R32f,        GL_R32F,            GL_RED,     GL_FLOAT);
        MAKE_FMT(avxFormat_RG32f,       GL_RG32F,           GL_RG,      GL_FLOAT);
        MAKE_FMT(avxFormat_RGB32f,      GL_RGB32F,          GL_RGB,     GL_FLOAT);
        MAKE_FMT(avxFormat_RGBA32f,     GL_RGBA32F,         GL_RGBA,    GL_FLOAT);
        // 32-bit signed int
        MAKE_FMT(avxFormat_R32i,        GL_R32I,            GL_RED,     GL_INT);
        MAKE_FMT(avxFormat_RG32i,       GL_RG32I,           GL_RG,      GL_INT);
        MAKE_FMT(avxFormat_RGB32i,      GL_RGB32I,          GL_RGB,     GL_INT);
        MAKE_FMT(avxFormat_RGBA32i,     GL_RGBA32I,         GL_RGBA,    GL_INT);
        // 32-bit unsigned int
        MAKE_FMT(avxFormat_R32u,        GL_R32UI,           GL_RED,     GL_UNSIGNED_INT);
        MAKE_FMT(avxFormat_RG32u,       GL_RG32UI,          GL_RG,      GL_UNSIGNED_INT);
        MAKE_FMT(avxFormat_RGB32u,      GL_RGB32UI,         GL_RGB,     GL_UNSIGNED_INT);
        MAKE_FMT(avxFormat_RGBA32u,     GL_RGBA32UI,        GL_RGBA,    GL_UNSIGNED_INT);
        // 16-bit signed int
        MAKE_FMT(avxFormat_R16i,        GL_R16I,            GL_RED,     GL_SHORT);
        MAKE_FMT(avxFormat_RG16i,       GL_RG16I,           GL_RG,      GL_SHORT);
        MAKE_FMT(avxFormat_RGB16i,      GL_RGB16I,          GL_RGB,     GL_SHORT);
        MAKE_FMT(avxFormat_RGBA16i,     GL_RGBA16I,         GL_RGBA,    GL_SHORT);
        // 16-bit unsigned int
        MAKE_FMT(avxFormat_R16u,        GL_R16UI,           GL_RED,     GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RG16u,       GL_RG16UI,          GL_RG,      GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RGB16u,      GL_RGB16UI,         GL_RGB,     GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_RGBA16u,     GL_RGBA16UI,        GL_RGBA,    GL_UNSIGNED_SHORT);


        MAKE_FMT(avxFormat_BGRA8un,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8sn,     GL_RGBA8_SNORM,     GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8us,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8ss,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8u,      GL_RGBA8UI,         GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8i,      GL_RGBA8I,          GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_BGRA8v,  GL_SRGB8_ALPHA8,    GL_RGBA,    GL_UNSIGNED_BYTE);

        MAKE_FMT(avxFormat_ABGR8un,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8sn,     GL_RGBA8_SNORM,     GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8us,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8ss,     GL_RGBA8,           GL_RGBA,    GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8u,      GL_RGBA8UI,         GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8i,      GL_RGBA8I,          GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_ABGR8v,  GL_SRGB8_ALPHA8,    GL_RGBA,    GL_UNSIGNED_BYTE);
        
        MAKE_FMT(avxFormat_A2RGB10un,   GL_RGB10_A2,        GL_BGRA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2RGB10sn,   GL_RGB10_A2,        GL_BGRA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2RGB10us,   GL_RGB10_A2,        GL_BGRA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2RGB10ss,   GL_RGB10_A2,        GL_BGRA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2RGB10u,    GL_RGB10_A2UI,      GL_BGRA_INTEGER,GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2RGB10i,    GL_RGB10_A2,        GL_BGRA_INTEGER,GL_UNSIGNED_INT_2_10_10_10_REV);
        
        MAKE_FMT(avxFormat_A2BGR10un,   GL_RGB10_A2,        GL_RGBA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2BGR10sn,   GL_RGB10_A2,        GL_RGBA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2BGR10us,   GL_RGB10_A2,        GL_RGBA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2BGR10ss,   GL_RGB10_A2,        GL_RGBA,    GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2BGR10u,    GL_RGB10_A2UI,      GL_RGBA_INTEGER,GL_UNSIGNED_INT_2_10_10_10_REV);
        MAKE_FMT(avxFormat_A2BGR10i,    GL_RGB10_A2,        GL_RGBA_INTEGER,GL_UNSIGNED_INT_2_10_10_10_REV);
    
        MAKE_FMT(avxFormat_B10GR11uf,   GL_R11F_G11F_B10F,  GL_RGBA,    GL_UNSIGNED_INT_10F_11F_11F_REV);
        MAKE_FMT(avxFormat_E5BGR9uf,    GL_RGB9_E5,         GL_RGBA,    GL_UNSIGNED_INT_5_9_9_9_REV);

        MAKE_FMT(avxFormat_D16un,       GL_DEPTH_COMPONENT16,   GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
        MAKE_FMT(avxFormat_X8D24un,       GL_DEPTH_COMPONENT24,   GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
        MAKE_FMT(avxFormat_D32f,        GL_DEPTH_COMPONENT32F,  GL_DEPTH_COMPONENT, GL_FLOAT);
        MAKE_FMT(avxFormat_S8u,         GL_STENCIL_INDEX8,      GL_STENCIL_INDEX,   GL_UNSIGNED_BYTE);
        MAKE_FMT(avxFormat_D16unS8u,       GL_DEPTH24_STENCIL8,    GL_DEPTH_STENCIL,   GL_UNSIGNED_INT);
        MAKE_FMT(avxFormat_D24unS8u,    GL_DEPTH24_STENCIL8,    GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8);
        MAKE_FMT(avxFormat_D32fS8u,     GL_DEPTH32F_STENCIL8,   GL_DEPTH_STENCIL,   GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
        
        MAKE_FMT(avxFormat_DXT1un,        GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT1v,   GL_COMPRESSED_SRGB_S3TC_DXT1_EXT, GL_COMPRESSED_SRGB_S3TC_DXT1_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT1A,       GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT1Av,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT3un,        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT3v,   GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT5un,        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_NONE);
        MAKE_FMT(avxFormat_DXT5v,   GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_NONE);
        MAKE_FMT(avxFormat_RGTC1un,       GL_COMPRESSED_RED_RGTC1, GL_COMPRESSED_RED_RGTC1, GL_NONE);
        MAKE_FMT(avxFormat_RGTC1sn,     GL_COMPRESSED_SIGNED_RED_RGTC1, GL_COMPRESSED_SIGNED_RED_RGTC1, GL_NONE);
        MAKE_FMT(avxFormat_RGTC2un,       GL_COMPRESSED_RG_RGTC2, GL_COMPRESSED_RG_RGTC2, GL_NONE);
        MAKE_FMT(avxFormat_RGTC2sn,     GL_COMPRESSED_SIGNED_RG_RGTC2, GL_COMPRESSED_SIGNED_RG_RGTC2, GL_NONE);
    }

    afxError err = { 0 };
    AFX_ASSERT(vf[avxFormat_D32f].intFmt == GL_DEPTH_COMPONENT32F);
    //AFX_ASSERT(vf[avxFormat_D32f].intFmt == GL_DEPTH_COMPONENT32);
    AFX_ASSERT(vf[avxFormat_X8D24un].intFmt == GL_DEPTH_COMPONENT24);
    AFX_ASSERT(vf[avxFormat_D24unS8u].intFmt == GL_DEPTH24_STENCIL8);
    AFX_ASSERT(vf[avxFormat_D32fS8u].intFmt == GL_DEPTH32F_STENCIL8);

    AFX_ASSERT(vf[avxFormat_RGBA4un].intFmt == GL_RGBA4);

    *fmt = vf[pf].intFmt;
    *layout = vf[pf].layout;
    *type = vf[pf].type;
}

_ZGL GLenum ZglToGlImageDimensions(afxUnit d)
{
    static GLenum const v[] =
    {
        NIL,
        GL_TEXTURE_1D,
        GL_TEXTURE_2D,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP,

        GL_INVALID_ENUM
    };

    return v[d];
}

_ZGL GLenum ZglToGlTexelFilterMode(avxTexelFilter tfm)
{
    static GLenum const v[] =
    {
        [avxTexelFilter_NEAREST] = GL_NEAREST,
        [avxTexelFilter_LINEAR] = GL_LINEAR,

        GL_INVALID_ENUM
    };

    return v[tfm];
}

_ZGL GLenum ZglToGlTexelFilterModeMipmapped(avxTexelFilter min, avxTexelFilter mip)
{
    static GLenum const nearest[] =
    {
        GL_NEAREST_MIPMAP_NEAREST, // - no filtering, sharp switching between mipmaps
        GL_NEAREST_MIPMAP_LINEAR, // - no filtering, smooth transition between mipmaps

        GL_INVALID_ENUM
    };

    static GLenum const linear[] =
    {
        GL_LINEAR_MIPMAP_NEAREST, //- filtering, sharp switching between mipmaps
        GL_LINEAR_MIPMAP_LINEAR,

        GL_INVALID_ENUM
    };

    return min == avxTexelFilter_LINEAR ? linear[mip] : nearest[mip];
}

_ZGL GLenum ZglToGlTexWrapMode(avxTexelWrap twm)
{
    static GLenum const v[] =
    {
        [avxTexelWrap_REPEAT] = GL_REPEAT,
        [avxTexelWrap_MIRROR] = GL_MIRRORED_REPEAT,
        [avxTexelWrap_EDGE] = GL_CLAMP_TO_EDGE,
        [avxTexelWrap_BORDER] = GL_CLAMP_TO_BORDER,
        [avxTexelWrap_REFLECT] = GL_MIRROR_CLAMP_TO_EDGE,

        GL_INVALID_ENUM
    };

    return v[twm];
}

_ZGL GLenum ZglToGlColorSwizzle(avxColorSwizzle swizzle)
{
    static GLenum const v[] =
    {
        [avxColorSwizzle_R] = GL_RED,
        [avxColorSwizzle_G] = GL_GREEN,
        [avxColorSwizzle_B] = GL_BLUE,
        [avxColorSwizzle_A] = GL_ALPHA,
        [avxColorSwizzle_ZERO] = GL_ZERO,
        [avxColorSwizzle_ONE] = GL_ONE,

        //GL_INVALID_ENUM
    };
    return v[swizzle];
}

_ZGL void ZglToGlColorSwizzling(avxSwizzling const *swizzling, GLenum *arrayedSwizzles)
{
    arrayedSwizzles[0] = ZglToGlColorSwizzle(swizzling->r);
    arrayedSwizzles[1] = ZglToGlColorSwizzle(swizzling->g);
    arrayedSwizzles[2] = ZglToGlColorSwizzle(swizzling->b);
    arrayedSwizzles[3] = ZglToGlColorSwizzle(swizzling->a);
}

_ZGL void ZglDetermineGlTargetInternalFormatType(avxRaster ras, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type)
{
    afxError err = { 0 };
    AFX_ASSERT(target);
    AFX_ASSERT(intFmt);
    AFX_ASSERT(fmt);
    AFX_ASSERT(type);
    afxResult cubemap = AvxGetRasterFlags(ras, avxRasterFlag_CUBEMAP);
    AFX_ASSERT(ras->m.whd.w); // always have at least one dimension.
    
    if (AvxGetRasterFlags(ras, avxRasterFlag_1D)) // Y
    {
        if (ras->m.whd.d > 1)
        {
            *target = GL_TEXTURE_1D_ARRAY;
        }
        else
        {
            *target = GL_TEXTURE_1D;
        }
    }
    else
    {
        // 2D or 2D array?

        if (!AvxGetRasterFlags(ras, avxRasterFlag_3D))
        {
            if (cubemap)
            {
                if (ras->m.whd.d > 6)
                {
                    *target = GL_TEXTURE_CUBE_MAP_ARRAY;
                }
                else
                {
                    *target = GL_TEXTURE_CUBE_MAP;
                }
            }
            else
            {
                if (ras->m.whd.d > 1)
                {
                    *target = GL_TEXTURE_2D_ARRAY;
                }
                else
                {
                    *target = GL_TEXTURE_2D;
                }
            }
        }
        else
        {
            *target = GL_TEXTURE_3D;
        }
    }

    ZglToGlFormat2(ras->m.fmt, intFmt, fmt, type);
}

_ZGL GLenum AfxToGlFrontFace(avxFrontFace ff)
{
    static GLenum const v[] =
    {
        [avxFrontFace_CCW] = GL_CCW,
        [avxFrontFace_CW] = GL_CW
    };

    return v[ff];
};

_ZGL GLenum AfxToGlCullMode(avxCullMode cm)
{
    afxError err;

    static GLenum const v[] =
    {
        [avxCullMode_NONE] = NIL,
        [avxCullMode_FRONT] = GL_FRONT,
        [avxCullMode_BACK] = GL_BACK,
        [avxCullMode_BOTH] = GL_FRONT_AND_BACK
    };
    AFX_ASSERT(v[avxCullMode_BACK] == GL_BACK);
    return v[cm];
};

_ZGL GLenum AfxToGlFillMode(avxFillMode pm)
{
    static GLenum const v[] =
    {
        [avxFillMode_FACE] = GL_FILL,
        [avxFillMode_LINE] = GL_LINE,
        [avxFillMode_POINT] = GL_POINT
    };

    return v[pm];
};

_ZGL GLenum ZglToGlCompareOp(avxCompareOp co)
{
    static GLenum const v[] =
    {
        [avxCompareOp_NEVER] = GL_NEVER,
        [avxCompareOp_LESS] = GL_LESS,
        [avxCompareOp_LEQUAL] = GL_LEQUAL,
        [avxCompareOp_EQUAL] = GL_EQUAL,
        [avxCompareOp_NOTEQUAL] = GL_NOTEQUAL,
        [avxCompareOp_GEQUAL] = GL_GEQUAL,
        [avxCompareOp_GREATER] = GL_GREATER,
        [avxCompareOp_ALWAYS] = GL_ALWAYS
    };

    return v[co];
};

_ZGL GLuint AfxToGlShaderStage(avxShaderType s)
{
    static GLuint const v[] =
    {
        NIL,
        [avxShaderType_VERTEX] = GL_VERTEX_SHADER,
        [avxShaderType_FRAGMENT] = GL_FRAGMENT_SHADER,
        [avxShaderType_PRIMITIVE] = GL_GEOMETRY_SHADER,
        [avxShaderType_TESS_CTRL] = GL_TESS_CONTROL_SHADER,
        [avxShaderType_TESS_EVAL] = GL_TESS_EVALUATION_SHADER,
        [avxShaderType_COMPUTE] = GL_COMPUTE_SHADER,
        
        NIL
    };

    return v[s];
};

_ZGL GLuint AfxToGlShaderStageBit(avxShaderType s)
{
    static GLuint const v[] =
    {
        NIL,
        [avxShaderType_VERTEX] = GL_VERTEX_SHADER_BIT,
        [avxShaderType_FRAGMENT] = GL_FRAGMENT_SHADER_BIT,
        [avxShaderType_PRIMITIVE] = GL_GEOMETRY_SHADER_BIT,
        [avxShaderType_TESS_CTRL] = GL_TESS_CONTROL_SHADER_BIT,
        [avxShaderType_TESS_EVAL] = GL_TESS_EVALUATION_SHADER_BIT,
        [avxShaderType_COMPUTE] = GL_COMPUTE_SHADER_BIT,
        NIL
    };

    return v[s];
};

_ZGL GLenum AfxToGlTopology(avxTopology pm)
{
    static GLuint const v[] =
    {
        [avxTopology_POINT_LIST] = GL_POINTS,

        [avxTopology_LINE_LIST] = GL_LINES,
        [avxTopology_LINE_STRIP] = GL_LINE_STRIP,

        [avxTopology_TRI_LIST] = GL_TRIANGLES,
        [avxTopology_TRI_STRIP] = GL_TRIANGLE_STRIP,
        [avxTopology_TRI_FAN] = GL_TRIANGLE_FAN,

        [avxTopology_LINE_LIST_ADJACENT] = GL_LINES_ADJACENCY,
        [avxTopology_LINE_STRIP_ADJACENT] = GL_LINE_STRIP_ADJACENCY,

        [avxTopology_TRI_LIST_ADJACENT] = GL_TRIANGLES_ADJACENCY,
        [avxTopology_TRI_STRIP_ADJACENT] = GL_TRIANGLE_STRIP_ADJACENCY,

        [avxTopology_PATCH_LIST] = GL_PATCHES
    };

    return v[pm];
};

_ZGL GLenum AfxToGlBufferUsage(avxBufferUsage usage)
{
    afxError err;
    static GLenum const v[] =
    {
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        GL_PIXEL_PACK_BUFFER,
        GL_PIXEL_UNPACK_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        GL_ARRAY_BUFFER,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_DRAW_INDIRECT_BUFFER
    };
    AfxThrowError();
    //errado
    return v[usage];
};

_ZGL GLenum AfxToGlStencilOp(avxStencilOp so)
{
    static GLenum const v[] =
    {
        [avxStencilOp_KEEP] = GL_KEEP,
        [avxStencilOp_ZERO] = GL_ZERO,
        [avxStencilOp_REPLACE] = GL_REPLACE,
        [avxStencilOp_INC_N_CLAMP] = GL_INCR,
        [avxStencilOp_DEC_N_CLAMP] = GL_DECR,
        [avxStencilOp_INV] = GL_INVERT,
        [avxStencilOp_INC_AND_WRAP] = GL_INCR_WRAP,
        [avxStencilOp_DEC_AND_WRAP] = GL_DECR_WRAP
    };

    return v[so];
}

_ZGL GLenum AfxToGlBlendOp(avxBlendOp f)
{
    static GLenum const v[] =
    {
        [avxBlendOp_ADD] = GL_FUNC_ADD,
        [avxBlendOp_SUB] = GL_FUNC_SUBTRACT,
        [avxBlendOp_REV_SUB] = GL_FUNC_REVERSE_SUBTRACT,
        [avxBlendOp_MIN] = GL_MIN,
        [avxBlendOp_MAX] = GL_MAX
    };

    return v[f];
}

_ZGL GLenum AfxToGlLogicOp(avxLogicOp f)
{
    afxError err;
    static GLenum const v[] =
    {
        [avxLogicOp_NOP] = GL_NOOP,
        [avxLogicOp_CLEAR] = GL_CLEAR,
        [avxLogicOp_SET] = GL_SET,
        [avxLogicOp_COPY] = GL_COPY,
        [avxLogicOp_COPY_INV] = GL_COPY_INVERTED,
        [avxLogicOp_INV] = GL_INVERT,
        [avxLogicOp_AND] = GL_AND,
        [avxLogicOp_NAND] = GL_NAND,
        [avxLogicOp_OR] = GL_OR,
        [avxLogicOp_NOR] = GL_NOR,
        [avxLogicOp_XOR] = GL_XOR,
        [avxLogicOp_EQUIV] = GL_EQUIV,
        [avxLogicOp_AND_REV] = GL_AND_REVERSE,
        [avxLogicOp_AND_INV] = GL_AND_INVERTED,
        [avxLogicOp_OR_REV] = GL_OR_REVERSE,
        [avxLogicOp_OR_INV] = GL_OR_INVERTED
    };
    AFX_ASSERT(v[avxLogicOp_NOP] == GL_NOOP);
    return v[f];
}

_ZGL GLenum AfxToGlBlendFactor(avxBlendFactor f)
{
    static GLenum const v[] =
    {
        [avxBlendFactor_ZERO] = GL_ZERO,
        [avxBlendFactor_ONE] = GL_ONE,
        [avxBlendFactor_SRC_RGB] = GL_SRC_COLOR,
        [avxBlendFactor_ONE_MINUS_SRC_RGB] = GL_ONE_MINUS_SRC_COLOR,
        [avxBlendFactor_DST_RGB] = GL_DST_COLOR,
        [avxBlendFactor_ONE_MINUS_DST_RGB] = GL_ONE_MINUS_DST_COLOR,
        [avxBlendFactor_SRC_A] = GL_SRC_ALPHA,
        [avxBlendFactor_ONE_MINUS_SRC_A] = GL_ONE_MINUS_SRC_ALPHA,
        [avxBlendFactor_DST_A] = GL_DST_ALPHA,
        [avxBlendFactor_ONE_MINUS_DST_A] = GL_ONE_MINUS_DST_ALPHA,
        [avxBlendFactor_CONST_RGB] = GL_CONSTANT_COLOR,
        [avxBlendFactor_ONE_MINUS_CONST_RGB] = GL_ONE_MINUS_CONSTANT_COLOR,
        [avxBlendFactor_CONST_A] = GL_CONSTANT_ALPHA,
        [avxBlendFactor_ONE_MINUS_CONST_A] = GL_ONE_MINUS_CONSTANT_ALPHA,
        [avxBlendFactor_SRC_A_SATURATE] = GL_SRC_ALPHA_SATURATE,
        [avxBlendFactor_SRC1_RGB] = GL_SRC1_COLOR,
        [avxBlendFactor_ONE_MINUS_SRC1_RGB] = GL_ONE_MINUS_SRC1_COLOR,
        [avxBlendFactor_SRC1_A] = GL_SRC1_ALPHA,
        [avxBlendFactor_ONE_MINUS_SRC1_A] = GL_ONE_MINUS_SRC1_ALPHA
    };

    return v[f];
}

#if 0
_ZGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxUnit32* stride)
{
    struct
    {
        GLint size;
        GLenum type;
        afxUnit stride;
    } fds[afxVertexFormat_TOTAL] =
    {
        [afxVertexFormat_R32] = { 1, GL_FLOAT, sizeof(afxReal32) },
        [afxVertexFormat_R32x2] = { 2, GL_FLOAT, sizeof(afxReal32) * 2 },
        [afxVertexFormat_R32x3] = { 3, GL_FLOAT, sizeof(afxReal32) * 3 },
        [afxVertexFormat_R32x4] = { 4, GL_FLOAT, sizeof(afxReal32) * 4 },

        [afxVertexFormat_R16] = { 1, GL_HALF_FLOAT, sizeof(afxReal16) },
        [afxVertexFormat_R16x2] = { 2, GL_HALF_FLOAT, sizeof(afxReal16) * 2 },
        [afxVertexFormat_R16x4] = { 4, GL_HALF_FLOAT, sizeof(afxReal16) * 4 },

        [afxVertexFormat_U32] = { 1, GL_UNSIGNED_INT, sizeof(afxUnit32) },
        [afxVertexFormat_U32x2] = { 2, GL_UNSIGNED_INT, sizeof(afxUnit32) * 2 },
        [afxVertexFormat_U32x4] = { 4, GL_UNSIGNED_INT, sizeof(afxUnit32) * 4 },

        [afxVertexFormat_U16] = { 1, GL_UNSIGNED_SHORT, sizeof(afxN16) },
        [afxVertexFormat_U16x2] = { 2, GL_UNSIGNED_SHORT, sizeof(afxN16) * 2 },
        [afxVertexFormat_U16x4] = { 4, GL_UNSIGNED_SHORT, sizeof(afxN16) * 4 },

        [afxVertexFormat_U8] = { 1, GL_UNSIGNED_BYTE, sizeof(afxUnit8) },
        [afxVertexFormat_U8x2] = { 2, GL_UNSIGNED_BYTE, sizeof(afxUnit8) * 2 },
        [afxVertexFormat_U8x4] = { 4, GL_UNSIGNED_BYTE, sizeof(afxUnit8) * 4 },

        [afxVertexFormat_I32] = { 1, GL_INT, sizeof(afxInt32) },
        [afxVertexFormat_I32x2] = { 2, GL_INT, sizeof(afxInt32) * 2 },
        [afxVertexFormat_I32x4] = { 4, GL_INT, sizeof(afxInt32) * 4 },

        [afxVertexFormat_I16] = { 1, GL_SHORT, sizeof(afxInt16) },
        [afxVertexFormat_I16x2] = { 2, GL_SHORT, sizeof(afxInt16) * 2 },
        [afxVertexFormat_I16x4] = { 4, GL_SHORT, sizeof(afxInt16) * 4 },

        [afxVertexFormat_I8] = { 1, GL_BYTE, sizeof(afxInt8) },
        [afxVertexFormat_I8x2] = { 2, GL_BYTE, sizeof(afxInt8) * 2 },
        [afxVertexFormat_I8x4] = { 4, GL_BYTE, sizeof(afxInt8) * 4 },
    };

    if (siz)
        *siz = fds[fmt].size;

    if (typ)
        *typ = fds[fmt].type;

    if (stride)
        *stride = fds[fmt].stride;

    //AFX_STATIC_ASSERT(afxVertexFormat_TOTAL == 5, "");

}
#endif

_ZGL void _ZglCopyTexSubImage(zglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxUnit opCnt, avxRasterCopy const ops[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    afxBool srcIs3d = FALSE;

    if (glSrcTarget == GL_TEXTURE_3D)
        srcIs3d = TRUE;

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc);

    for (afxUnit i = 0; i < opCnt; i++)
    {
        avxRasterCopy const* op = &ops[i];
        avxRasterRegion const* srcRgn = &op->src;
        //afxWarp const dstOrigin = op->dstOrigin;

        _ZglBindFboAttachment(gl, GL_READ_FRAMEBUFFER, NIL, GL_COLOR_ATTACHMENT0, glSrcTarget, glSrcHandle, op->src.lodIdx, op->src.origin.z, FALSE);
        gl->ReadBuffer(GL_COLOR_ATTACHMENT0);

        switch (glDstTarget)
        {
        case GL_TEXTURE_2D:
        {
            gl->CopyTexSubImage2D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_1D_ARRAY:
        {
            for (afxUnit i = op->src.whd.d; i < op->src.whd.d; i++)
            {
                gl->CopyTexSubImage2D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, i, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_1D:
        {
            gl->CopyTexSubImage1D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->src.origin.x, op->src.origin.y, op->src.whd.w);
            break;
        }
        case GL_TEXTURE_3D:
        {
            for (afxUnit i = op->dstOrigin.z; i < op->src.whd.d; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, i, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_2D_ARRAY:
        {
            for (afxUnit i = op->dstOrigin.z; i < op->src.whd.d; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, i, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        {
            for (afxUnit i = op->dstOrigin.z; i < op->src.whd.d; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, i, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP:
        {
            for (afxUnit i = op->dstOrigin.z; i < op->src.whd.d; i++)
            {
                gl->CopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_RECTANGLE:
        {
            AFX_ASSERT(op->dstLodIdx == 0);
            gl->CopyTexSubImage2D(glDstTarget, op->dstLodIdx, op->dstOrigin.x, op->dstOrigin.y, op->src.origin.x, op->src.origin.y, op->src.whd.w, op->src.whd.h); _ZglThrowErrorOccuried();
        };
        default:
            AfxThrowError();
            break;
        }
    }

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, NIL);
}

_ZGL afxError _ZglTexSubImage(glVmt const* gl, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src)
{
    afxError err = { 0 };

    switch (glTarget)
    {
    case GL_TEXTURE_2D:
    {
        gl->TexSubImage2D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->TexSubImage3D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    {
        for (afxUnit i = 0; i < rgn->whd.d; i++)
        {
            gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn->origin.z + i, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->TexSubImage1D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->whd.w, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->TexSubImage2D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.z, rgn->whd.w, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->TexSubImage3D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError _ZglTextureSubImage(glVmt const* gl, GLuint glHandle, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src)
{
    afxError err = { 0 };

    switch (glTarget)
    {
    case GL_TEXTURE_2D:
    {
        gl->TextureSubImage2D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_CUBE_MAP:
    {
        gl->TextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    {
        for (afxUnit i = 0; i < rgn->whd.d; i++)
        {
            gl->TextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z + i + (glTarget - GL_TEXTURE_CUBE_MAP_POSITIVE_X), rgn->whd.w, rgn->whd.h, 1, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->TextureSubImage1D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->whd.w, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->TextureSubImage2D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.z, rgn->whd.w, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->TextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, glType, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError _ZglCompressedTexSubImage(glVmt const* gl, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxUnit compressedSiz, afxAddress const src)
{
    afxError err = { 0 };

    switch (glTarget)
    {
    case GL_TEXTURE_2D:
    {
        gl->CompressedTexSubImage2D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->CompressedTexSubImage3D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    {
        for (afxUnit i = 0; i < rgn->whd.d; i++)
        {
            gl->CompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + rgn->origin.z + i, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->CompressedTexSubImage1D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->whd.w, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->CompressedTexSubImage2D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.z, rgn->whd.w, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->CompressedTexSubImage3D(glTarget, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError _ZglCompressedTextureSubImage(glVmt const* gl, GLuint glHandle, GLenum glTarget, avxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxUnit compressedSiz, afxAddress const src)
{
    afxError err = { 0 };

    switch (glTarget)
    {
    case GL_TEXTURE_2D:
    {
        gl->CompressedTextureSubImage2D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->whd.w, rgn->whd.h, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_CUBE_MAP:
    {
        gl->CompressedTextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
    {
        for (afxUnit i = 0; i < rgn->whd.d; i++)
        {
            gl->CompressedTextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z + i + (glTarget - GL_TEXTURE_CUBE_MAP_POSITIVE_X), rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->CompressedTextureSubImage1D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->whd.w, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->CompressedTextureSubImage2D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.z, rgn->whd.w, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    {
        gl->CompressedTextureSubImage3D(glHandle, rgn->lodIdx, rgn->origin.x, rgn->origin.y, rgn->origin.z, rgn->whd.w, rgn->whd.h, rgn->whd.d, glFmt, compressedSiz, (void const*)src); _ZglThrowErrorOccuried();
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError zglBindTextureUnit(glVmt const* gl, GLuint unit, GLenum target, GLuint texture)
{
    afxError err = { 0 };

    if (gl->BindTextureUnit)
    {
        gl->BindTextureUnit(unit, texture); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->ActiveTexture(GL_TEXTURE0 + unit); _ZglThrowErrorOccuried();
        gl->BindTexture(target, texture); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglCreateTextures(glVmt const* gl, GLenum target, GLsizei n, GLuint *textures)
{
    afxError err = { 0 };

    if (gl->CreateTextures)
    {
        gl->CreateTextures(target, n, textures); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->GenTextures(n, textures); _ZglThrowErrorOccuried();

        for (GLsizei i = 0; i < n; i++)
        {
            gl->BindTexture(target, textures[i]); _ZglThrowErrorOccuried();
            AFX_ASSERT(gl->IsTexture(textures[i]));
        }
    }
    return err;
}

_ZGL afxError zglDeleteTextures(glVmt const* gl, GLsizei n, GLuint *textures)
{
    afxError err = { 0 };
    gl->DeleteTextures(n, textures); _ZglThrowErrorOccuried();
    return err;
}

_ZGL afxError zglMakeTextureStorage(glVmt const* gl, GLenum target, GLuint texture, GLsizei levelsOrSamples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
    afxError err = { 0 };

    switch (target)
    {
    case GL_TEXTURE_2D:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->TextureStorage2D)
        {
            gl->TextureStorage2D(texture, levelsOrSamples, internalformat, width, height); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage2D(target, levelsOrSamples, internalformat, width, height); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_CUBE_MAP_ARRAY:
    case GL_TEXTURE_3D:
    {
        if (gl->TextureStorage3D)
        {
            gl->TextureStorage3D(texture, levelsOrSamples, internalformat, width, height, depth); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage3D(target, levelsOrSamples, internalformat, width, height, depth); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AFX_ASSERT(1 >= height);

        if (gl->TextureStorage2D)
        {
            gl->TextureStorage2D(texture, levelsOrSamples, internalformat, width, depth); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage2D(target, levelsOrSamples, internalformat, width, depth); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        AFX_ASSERT(1 >= height);
        AFX_ASSERT(1 >= depth);

        if (gl->TextureStorage1D)
        {
            gl->TextureStorage1D(texture, levelsOrSamples, internalformat, width); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage1D(target, levelsOrSamples, internalformat, width); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_MULTISAMPLE:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->TextureStorage2DMultisample)
        {
            gl->TextureStorage2DMultisample(texture, levelsOrSamples, internalformat, width, height, fixedsamplelocations); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage2DMultisample(target, levelsOrSamples, internalformat, width, height, fixedsamplelocations); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
    {
        if (gl->TextureStorage3DMultisample)
        {
            gl->TextureStorage3DMultisample(texture, levelsOrSamples, internalformat, width, height, depth, fixedsamplelocations); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, target, texture);
            gl->TexStorage3DMultisample(target, levelsOrSamples, internalformat, width, height, depth, fixedsamplelocations); _ZglThrowErrorOccuried();
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError zglCreateRenderbuffers(glVmt const* gl, GLsizei n, GLuint *renderbuffers)
{
    afxError err = { 0 };

    if (gl->CreateRenderbuffers)
    {
        gl->CreateRenderbuffers(n, renderbuffers); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->GenRenderbuffers(n, renderbuffers); _ZglThrowErrorOccuried();

        for (GLsizei i = 0; i < n; i++)
        {
            gl->BindRenderbuffer(GL_RENDERBUFFER, renderbuffers[i]); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglMakeRenderbufferStorage(glVmt const* gl, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    afxError err = { 0 };

    if (1 >= samples)
    {
        if (gl->NamedRenderbufferStorage)
        {
            gl->NamedRenderbufferStorage(renderbuffer, internalformat, width, height); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->BindRenderbuffer(GL_RENDERBUFFER, renderbuffer); _ZglThrowErrorOccuried();
            gl->RenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height); _ZglThrowErrorOccuried();
        }
    }
    else
    {
        if (gl->NamedRenderbufferStorageMultisample)
        {
            gl->NamedRenderbufferStorageMultisample(renderbuffer, samples, internalformat, width, height); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->BindRenderbuffer(GL_RENDERBUFFER, renderbuffer); _ZglThrowErrorOccuried();
            gl->RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglUpdateTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    afxError err = { 0 };
    
    gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    switch (target)
    {
    case GL_TEXTURE_2D:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->TextureSubImage2D)
        {
            gl->TextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_3D:
    {
        if (gl->TextureSubImage3D)
        {
            gl->TextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AFX_ASSERT(1 >= height);

        if (gl->TextureSubImage2D)
        {
            gl->TextureSubImage2D(texture, level, xoffset, zoffset, width, depth, format, type, pixels); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage2D(target, level, xoffset, zoffset, width, depth, format, type, pixels); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        AFX_ASSERT(1 >= height);
        AFX_ASSERT(1 >= depth);

        if (gl->TextureSubImage1D)
        {
            gl->TextureSubImage1D(texture, level, xoffset, width, format, type, pixels); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage1D(target, level, xoffset, width, format, type, pixels); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        break;
    }
    default: AfxThrowError(); break;
    }

    return err;
}

_ZGL afxError zglUpdateTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    switch (target)
    {
    case GL_TEXTURE_2D:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->CompressedTextureSubImage2D)
        {
            gl->CompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_3D:
    {
        if (gl->CompressedTextureSubImage3D)
        {
            gl->CompressedTextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AFX_ASSERT(1 >= height);

        if (gl->CompressedTextureSubImage2D)
        {
            gl->CompressedTextureSubImage2D(texture, level, xoffset, zoffset, width, depth, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage2D(target, level, xoffset, zoffset, width, depth, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        AFX_ASSERT(1 >= height);
        AFX_ASSERT(1 >= depth);

        if (gl->CompressedTextureSubImage1D)
        {
            gl->CompressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        break;
    }
    default: AfxThrowError(); break;
    }
    return err;
}

_ZGL afxError zglUnpackTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLuint buffer, GLsizei offset)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);

    switch (target)
    {
    case GL_TEXTURE_2D:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->TextureSubImage2D)
        {
            gl->TextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_3D:
    {
        if (gl->TextureSubImage3D)
        {
            gl->TextureSubImage3D(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AFX_ASSERT(1 >= height);

        if (gl->TextureSubImage2D)
        {
            gl->TextureSubImage2D(texture, level, xoffset, zoffset, width, depth, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage2D(target, level, xoffset, zoffset, width, depth, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        AFX_ASSERT(1 >= height);
        AFX_ASSERT(1 >= depth);

        if (gl->TextureSubImage1D)
        {
            gl->TextureSubImage1D(texture, level, xoffset, width, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage1D(target, level, xoffset, width, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        if (gl->TextureSubImage2D)
        {
            gl->TextureSubImage2D(texture, level, xoffset, zoffset, width, depth, format, type, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->TexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + zoffset, level, xoffset, yoffset, width, height, format, type, (void const*)offset); _ZglThrowErrorOccuried();
            AFX_ASSERT(1 >= depth);
        }
        break;
    }
    default:
        break;
    }

    //gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    return err;
}

_ZGL afxError zglUnpackTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLuint buffer, GLsizei offset)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);

    switch (target)
    {
    case GL_TEXTURE_2D:
    {
        AFX_ASSERT(1 >= depth);

        if (gl->CompressedTextureSubImage2D)
        {
            gl->CompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_3D:
    {
        if (gl->CompressedTextureSubImage3D)
        {
            gl->CompressedTextureSubImage3D(texture, level, xoffset, zoffset, zoffset, width, height, depth, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage3D(target, level, xoffset, zoffset, zoffset, width, height, depth, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    {
        AFX_ASSERT(1 >= height);

        if (gl->CompressedTextureSubImage2D)
        {
            gl->CompressedTextureSubImage2D(texture, level, xoffset, zoffset, width, depth, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage2D(target, level, xoffset, zoffset, width, depth, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_1D:
    {
        AFX_ASSERT(1 >= height);
        AFX_ASSERT(1 >= depth);

        if (gl->CompressedTextureSubImage1D)
        {
            gl->CompressedTextureSubImage1D(texture, level, xoffset, width, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    {
        if (gl->CompressedTextureSubImage2D)
        {
            gl->CompressedTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
        }
        else
        {
            zglBindTextureUnit(gl, ZGL_COPY_WRITE_RASTER_SLOT, target, texture);
            gl->CompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + zoffset, level, xoffset, yoffset, width, height, format, imageSize, (void const*)offset); _ZglThrowErrorOccuried();
            AFX_ASSERT(1 >= depth);
        }
        break;
    }
    default: AfxThrowError(); break;
    }
    //gl->BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    return err;
}

_ZGL afxError zglDumpTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    if (gl->GetTextureSubImage)
    {
        gl->GetTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, pixels); _ZglThrowErrorOccuried();
    }
    else if (gl->GetTextureImage)
    {
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetTextureImage(texture, level, format, type, bufSize, pixels); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else if (gl->GetnTexImage)
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetnTexImage(target, level, format, type, bufSize, pixels); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetTexImage(texture, level, format, type, pixels); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglDumpTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    if (gl->GetCompressedTextureSubImage)
    {
        gl->GetCompressedTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, pixels); _ZglThrowErrorOccuried();
    }
    else if (gl->GetnCompressedTexImage)
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetnCompressedTexImage(target, level, bufSize, pixels); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetCompressedTexImage(target, level, pixels); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglPackTextureSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, GLuint buffer, GLsizei offset)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_PACK_BUFFER, buffer);

    if (gl->GetTextureSubImage)
    {
        gl->GetTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, bufSize, (void*)offset); _ZglThrowErrorOccuried();
    }
    else if (gl->GetTextureImage)
    {
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetTextureImage(texture, level, format, type, bufSize, (void*)offset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else if (gl->GetnTexImage)
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetnTexImage(target, level, format, type, bufSize, (void*)offset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetTexImage(texture, level, format, type, (void*)offset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglPackTextureSubImageCompressed(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, GLuint buffer, GLsizei offset)
{
    afxError err = { 0 };

    gl->BindBuffer(GL_PIXEL_PACK_BUFFER, buffer);

    if (gl->GetCompressedTextureSubImage)
    {
        gl->GetCompressedTextureSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, bufSize, (void*)offset); _ZglThrowErrorOccuried();
    }
    else if (gl->GetnCompressedTexImage)
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetnCompressedTexImage(target, level, bufSize, (void*)offset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    else
    {
        zglBindTextureUnit(gl, ZGL_COPY_READ_RASTER_SLOT, target, texture);

        gl->PixelStorei(GL_PACK_SKIP_PIXELS, xoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, yoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, zoffset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, width); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, height); _ZglThrowErrorOccuried();
        gl->GetCompressedTexImage(target, level, (void*)offset); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_PIXELS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_ROWS, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_SKIP_IMAGES, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_ROW_LENGTH, 0); _ZglThrowErrorOccuried();
        gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, 0); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglClearTexImage(glVmt const* gl, GLuint texture, GLint level, GLenum format, GLenum type, const void *data)
{
    afxError err = { 0 };

    if (gl->ClearTexImage)
    {
        gl->ClearTexImage(texture, level, format, type, data); _ZglThrowErrorOccuried();
    }
    else
    {
        AfxThrowError();
    }
    return err;
}

_ZGL afxError zglClearTexSubImage(glVmt const* gl, GLenum target, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data)
{
    afxError err = { 0 };

    if (gl->ClearTexSubImage)
    {
        gl->ClearTexSubImage(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data); _ZglThrowErrorOccuried();
    }
    else
    {
        GLuint pbo, pbo2;
        zglCreateBuffers(gl, GL_PIXEL_UNPACK_BUFFER, 1, &pbo);
        zglCreateBuffers(gl, GL_PIXEL_PACK_BUFFER, 1, &pbo2);

        GLuint tmp;
        zglCreateTextures(gl, target, 1, &tmp);

        switch (target)
        {
        case GL_TEXTURE_2D:
        {
            zglMakeTextureStorage(gl, target, tmp, 1, GL_RGBA32F, 1, 1, 1, GL_FALSE);
            zglUpdateTextureSubImage(gl, target, tmp, 0, 0, 0, 0, 1, 1, 1, format, type, data);

            afxUnit bufSiz = AFX_ALIGN_SIZE(width, AVX_RASTER_ALIGNMENT) * height * depth * sizeof(avxClearValue);
            zglMakeBufferStorage(gl, GL_PIXEL_UNPACK_BUFFER, pbo, bufSiz, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
            afxUnit bufSiz2 = AFX_ALIGN_SIZE(sizeof(avxClearValue), AVX_RASTER_ALIGNMENT);
            zglMakeBufferStorage(gl, GL_PIXEL_PACK_BUFFER, pbo2, bufSiz2, NIL, GL_MAP_READ_BIT);

            zglPackTextureSubImage(gl, target, tmp, 0, 0, 0, 0, 1, 1, 1, format, type, bufSiz, pbo2, 0);
            void* p2;
            zglMapBuffer(gl, GL_PIXEL_PACK_BUFFER, pbo2, GL_MAP_READ_BIT, &p2);

            void* p;
            zglMapBuffer(gl, GL_PIXEL_UNPACK_BUFFER, pbo, GL_MAP_WRITE_BIT, &p);
            AfxStream(bufSiz / sizeof(avxClearValue), 0, sizeof(avxClearValue), p2, p);
            zglUnmapBuffer(gl, GL_PIXEL_UNPACK_BUFFER, pbo);
            zglUnmapBuffer(gl, GL_PIXEL_PACK_BUFFER, pbo2);

            zglUnpackTextureSubImage(gl, target, tmp, level, xoffset, yoffset, 0, width, height, 1, format, type, pbo, 0);

            break;
        }
        default: AfxThrowError(); break;
        }

        zglDeleteBuffers(gl, 1, &pbo);
        zglDeleteBuffers(gl, 1, &pbo2);
        zglDeleteTextures(gl, 1, &tmp);
    }
    return err;
}

_ZGL afxError zglBlitFramebuffer(glVmt const* gl, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    afxError err = { 0 };

    if (gl->BlitNamedFramebuffer)
    {
        gl->BlitNamedFramebuffer(readFramebuffer, drawFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer); _ZglThrowErrorOccuried();
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer); _ZglThrowErrorOccuried();
        gl->BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter); _ZglThrowErrorOccuried();
    }
    return err;
}


_ZGL afxError zglTextureBuffer(glVmt const* gl, GLuint texture, GLenum internalformat, GLuint buffer)
{
    afxError err = { 0 };

    if (gl->TextureBuffer)
    {
        gl->TextureBuffer(texture, internalformat, buffer); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindTexture(GL_TEXTURE_BUFFER, texture); _ZglThrowErrorOccuried();
        gl->TexBuffer(GL_TEXTURE_BUFFER, internalformat, buffer); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglTextureBufferRange(glVmt const* gl, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    afxError err = { 0 };

    if (gl->TextureBufferRange)
    {
        gl->TextureBufferRange(texture, internalformat, buffer, offset, size); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindTexture(GL_TEXTURE_BUFFER, texture); _ZglThrowErrorOccuried();
        gl->TexBufferRange(GL_TEXTURE_BUFFER, internalformat, buffer, offset, size); _ZglThrowErrorOccuried();
    }
    return err;
}


_ZGL afxError zglDeleteBuffers(glVmt const* gl, GLsizei n, GLuint *buffers)
{
    afxError err = { 0 };
    gl->DeleteBuffers(n, buffers); _ZglThrowErrorOccuried();
    return err;
}

_ZGL afxError zglCreateBuffers(glVmt const* gl, GLenum target, GLsizei n, GLuint *buffers)
{
    afxError err = { 0 };

    if (gl->CreateBuffers)
    {
        gl->CreateBuffers(n, buffers); _ZglThrowErrorOccuried();
    }
    else
    {
        /*
            glCreateBuffers returns n previously unused buffer names in buffers, 
            each representing a new buffer object initialized as if it had been bound to an unspecified target.

            WHAT TO DO???

            I mimic'ed CreateTextures()
        */

        gl->GenBuffers(n, buffers); _ZglThrowErrorOccuried();

        for (GLsizei i = 0; i < n; i++)
        {
            gl->BindBuffer(target, buffers[i]); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglBindBufferBase(glVmt const* gl, GLenum target, GLuint first, GLsizei count, const GLuint *buffers)
{
    afxError err = { 0 };

    if (gl->BindBuffersBase)
    {
        gl->BindBuffersBase(target, first, count, buffers); _ZglThrowErrorOccuried();
    }
    else
    {
        for (GLsizei i = 0; i < count; i++)
        {
            gl->BindBufferBase(target, first + i, buffers ? buffers[i] : NIL); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglBindBufferRange(glVmt const* gl, GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes)
{
    afxError err = { 0 };

    if (gl->BindBuffersRange)
    {
        gl->BindBuffersRange(target, first, count, buffers, offsets, sizes); _ZglThrowErrorOccuried();
    }
    else
    {
        for (GLsizei i = 0; i < count; i++)
        {
            gl->BindBufferRange(target, first + i, buffers ? buffers[i] : NIL, offsets ? offsets[i] : 0, sizes ? sizes[i] : 0); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglBindTextures(glVmt const* gl, GLuint first, GLsizei count, const GLenum* targets, const GLuint *textures)
{
    afxError err = { 0 };

    if (gl->BindBuffersBase)
    {
        gl->BindTextures(first, count, textures); _ZglThrowErrorOccuried();
    }
    else
    {
        for (GLsizei i = 0; i < count; i++)
        {
            zglBindTextureUnit(gl, first + i, targets[i], textures ? textures[i] : NIL); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglBindSamplers(glVmt const* gl, GLuint first, GLsizei count, const GLuint *samplers)
{
    afxError err = { 0 };

    if (gl->BindSamplers)
    {
        gl->BindSamplers(first, count, samplers); _ZglThrowErrorOccuried();
    }
    else
    {
        for (GLsizei i = 0; i < count; i++)
        {
            gl->BindSampler(first + i, samplers ? samplers[i] : NIL); _ZglThrowErrorOccuried();
        }
    }
    return err;
}

_ZGL afxError zglMakeBufferStorage(glVmt const* gl, GLenum target, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)
{
    afxError err = { 0 };

    if (gl->NamedBufferStorage)
    {
        gl->NamedBufferStorage(buffer, size, data, flags); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->BufferStorage(target, size, data, flags); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglUpdateBufferSubData(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)
{
    afxError err = { 0 };

    if (gl->NamedBufferSubData)
    {
        gl->NamedBufferSubData(buffer, offset, size, data); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->BufferSubData(target, offset, size, data); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglCopyNamedBufferSubData(glVmt const* gl, GLenum readTarget, GLuint readBuffer, GLenum writeTarget, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    afxError err = { 0 };

    if (gl->CopyNamedBufferSubData)
    {
        gl->CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(readTarget, readBuffer); _ZglThrowErrorOccuried();
        gl->BindBuffer(writeTarget, writeBuffer); _ZglThrowErrorOccuried();
        gl->CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglClearBufferData(glVmt const* gl, GLenum target, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data)
{
    afxError err = { 0 };

    if (gl->ClearNamedBufferData)
    {
        gl->ClearNamedBufferData(buffer, internalformat, format, type, data); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->ClearBufferData(target, internalformat, format, type, data); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglClearBufferSubData(glVmt const* gl, GLenum target, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data)
{
    afxError err = { 0 };

    if (gl->ClearNamedBufferSubData)
    {
        gl->ClearNamedBufferSubData(buffer, internalformat, offset, size, format, type, data); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->ClearBufferSubData(target, internalformat, offset, size, format, type, data); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglMapBuffer(glVmt const* gl, GLenum target, GLuint buffer, GLenum access, void** pp)
{
    afxError err = { 0 };
    void* p;

    if (gl->MapNamedBuffer)
    {
        p = gl->MapNamedBuffer(buffer, access); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        p = gl->MapBuffer(target, access); _ZglThrowErrorOccuried();
    }
    *pp = p;
    return err;
}

_ZGL afxError zglMapBufferRange(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access, void** pp)
{
    afxError err = { 0 };
    void* p;

    if (gl->MapNamedBufferRange)
    {
        p = gl->MapNamedBufferRange(buffer, offset, length, access); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        p = gl->MapNamedBufferRange(target, offset, length, access); _ZglThrowErrorOccuried();
    }
    *pp = p;
    return err;

}

_ZGL afxError zglUnmapBuffer(glVmt const* gl, GLenum target, GLuint buffer)
{
    afxError err = { 0 };

    if (gl->UnmapNamedBuffer)
    {
        gl->UnmapNamedBuffer(buffer); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->UnmapBuffer(target); _ZglThrowErrorOccuried();
    }
    return err;
}

_ZGL afxError zglFlushBufferRange(glVmt const* gl, GLenum target, GLuint buffer, GLintptr offset, GLsizeiptr length)
{
    afxError err = { 0 };

    if (gl->FlushMappedNamedBufferRange)
    {
        gl->FlushMappedNamedBufferRange(buffer, offset, length); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindBuffer(target, buffer); _ZglThrowErrorOccuried();
        gl->FlushMappedBufferRange(target, offset, length); _ZglThrowErrorOccuried();
    }
    return err;
}
