/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

// IMPORTANT: O módulo de matemática do Qwadro é matemática para programadores, não para matemáticos. 
// Nossos ângulos e suas contrapartes são representadas em "radians", não em "degrees". Funções para degrees são apenas utilitários de conveniência em conversão.
// Nossas rotações e orientações são representadas primariamente por "quaternion". Funções para axial rotation e Euler são apenas utilitários.
// É totalmente vedado adulterar os princípios gigachads e heterotops do Qwadro com essas tcholices dessas game engines randômicas do mundo por responder a vontade das pessoas de querer serem eternos subcolocados..

// IMPORTANT: This library WAS NOT made by a math guy. Most things here WERE NOT tested and there IS NOT any guarantee to me accurate.

// IMPORTANT: Qwadro uses their OWN matrix layout, which is a combined form of 3x3 matrix, 1x3 matrix and 4x1 matrix based.

// IMPORTANT: Qwadro is right handed.
// Left vs right-handed coordinate system.
// In a left-handed: the positive X, Y and Z axes point right, up and far/forward, respectively. Positive rotation is clockwise about the axis of rotation.
// In a right-handed: the positive X, Y and Z axes point right, up and near/backward (negative Z axis points far/forward), respectively. Positive rotation is counterclockwise about the axis of rotation.

// No Qwadro, usamos, como estilo de programação, muitos parênteses irrelevantes. 
// Eles existem para fins didáticos mesmo, facilitando a vida de pessoas que não conhecem o princípio de precedência de PEMDAS.

// AFX always targets a another object rather than 'self' to avoid using intermediate objects inside functions. It optimizes code when operations on a object type are grouped continously.
// That means "AfxNormalizeObject(self, to)" always should export to "to" instead of doing action in place.

// Qwadro uses radians as default angle measurement unit. They are natural and faster.

#ifndef AFX_MATH_DEFS_H
#define AFX_MATH_DEFS_H

#include <math.h>
#include "qwadro/afxDebug.h"
//#include "qwadro/afxSimd.h"
//#include <stdalign.h>

#ifdef AFX_OS_WINDOWS
/*
    __vectorcall is a Microsoft-specific calling convention introduced to pass vector types via registers,
    not the stack, improve performance for functions involving SIMD types, reduce overhead of copying 128-bit/256-bit vectors.
    It's part of MSVC's ABI, and also supported by Clang when targeting Windows (x86/x64) platforms.
*/
#   define AFXCALLV __vectorcall
#else
#   define AFXCALLV 
#endif

#define AFX_REAL(x_)   (x_)

#ifndef AFX_PI
#   define AFX_PI (3.14159265358979323846)
#endif
#ifndef AFX_EPSILON32
#   ifdef FLT_EPSILON
#       define AFX_EPSILON32 FLT_EPSILON
#   else
#       define AFX_EPSILON32 (1.192092896e-07f) // smallest such that ((1.0 + AFX_EPSILON32) != 1.0)
#   endif
#endif
#ifndef AFX_EPSILON64
#   ifdef DBL_EPSILON
#       define AFX_EPSILON64 DBL_EPSILON
#   else
#       define AFX_EPSILON64 (2.2204460492503131e-016) // smallest such that ((1.0 + AFX_EPSILON64) != 1.0)
#   endif
#endif
#ifndef AFX_EPSILON
#   define AFX_EPSILON AFX_EPSILON32
#endif

#define AFX_PI_OVER2        (AFX_PI / 2.0) // 0.0174532925

// The value of (PI / 180) is often used in converting degrees to radians, as there are "Pi" radians in 180 degrees.
#define AFX_PI_OVER_180     (AFX_PI / 180.0)
#define AFX_PI32_OVER_180   (0.0174532925f)
#define AFX_PI64_OVER_180   (0.017453292519943295)
// The value of (180 / PI) is commonly used for converting radians to degrees.
#define AFX_180_OVER_PI     (180.0 / AFX_PI)
#define AFX_180_OVER_PI32   (57.29578f)
#define AFX_180_OVER_PI64   (57.295779513082320)

// Converts degrees to radians.
 // The value returned by this function is (pi / 180) * degrees.
#define AFX_RADIANS(degrees_) ((degrees_) * AFX_PI_OVER_180)

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.
#define AFX_DEGREES(radians_) ((radians_) * AFX_180_OVER_PI)

//typedef  afxUnit afxWarp[3];

#ifndef MFX_ALIGN_ALL
AFX_DEFINE_STRUCT(afxV2d)
#else
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxV2d)
#endif
{
    union
    {
        afxReal v[2];
        struct { afxReal x, y; };
    };
};

#ifndef MFX_ALIGN_ALL
AFX_DEFINE_STRUCT(afxV3d)
#else
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxV3d)
#endif
{
    union
    {
        afxReal v[3];
        struct { afxReal x, y, z; };
        struct { afxV2d v2; afxReal v2z; };
        struct { afxV2d xy; afxReal xy_z; };
    };
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxV4d)
{
    union
    {
        afxReal v[4];
        struct { afxReal x, y, z, w; };
        struct { afxV3d v3; afxReal v3w; };
        struct { afxV3d xyz; afxReal xyz_w; };
        struct { afxV2d v2; afxV2d v2zw; };
        struct { afxV2d xy; afxV2d zw; };
    };
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxM2d)
// 2D Linear Transform (2x2 Matrix)
{
    union
    {
        afxReal m[2][2]; // 2 arrays of 2 floats
        struct { afxV2d x, y; };
        afxV2d v2[2]; // 2 arrays of 2 floats
        afxV4d v4; // 1 arrays of 4 floats
    };
};

#ifndef MFX_ALIGN_ALL
AFX_DEFINE_STRUCT(afxM3d)
#else
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxM3d)
#endif
// 3D Linear Transform (3x3 Matrix)
{
    union
    {
        afxReal m[3][3]; // 3 arrays of 3 floats
        struct { afxV3d x, y, z; };
        afxV3d v3[3]; // 3 arrays of 3 floats
    };
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxM34d)
// A 3x4 matrix has 3 rows and 4 columns.
{
    union
    {
        afxReal m[3][4]; // 3 arrays of 4 floats
        struct { afxV4d x, y, z; };
        afxV4d v4[3]; // 3 arrays of 4 floats
    };
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxM4d)
// 4x4
{
    union
    {
        afxReal m[4][4]; // 4 arrays of 4 floats
        struct { afxV4d x, y, z, w; };
        afxV4d v4[4]; // 4 arrays of 4 floats
    };
};

#ifndef MFX_ALIGN_ALL
AFX_DEFINE_STRUCT(afxM43d)
#else
AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxM43d)
#endif
// A 4x3 matrix has 4 rows and 3 columns.
{
    union
    {
        afxReal m[4][3]; // 4 arrays of 3 floats
        struct { afxV3d x, y, z, w; };
        afxV3d v3[4]; // 4 arrays of 3 floats
    };
};

typedef afxV4d  AFX_SIMD afxVector;
typedef afxV4d  AFX_SIMD afxRotor; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxQuat; // 0,1,2 = imaginary, 3 = real
typedef afxV4d  AFX_SIMD afxPoint;
typedef afxM4d  AFX_SIMD afxMatrix;
typedef afxQuat AFX_SIMD afxQuaternion;

#ifdef MFX_ALIGN_ALL
AFX_STATIC_ASSERT(__alignof(afxV2d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxV3d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxM3d) == AFX_SIMD_ALIGNMENT, "");
#endif
// always aligned
AFX_STATIC_ASSERT(__alignof(afxM4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxV4d) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxQuat) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxRotor) == AFX_SIMD_ALIGNMENT, "");
AFX_STATIC_ASSERT(__alignof(afxMatrix) == AFX_SIMD_ALIGNMENT, "");

#define MFX_USE_EXPERIMENTAL
#define MFX_USE_RW_MATH // use RenderWare over Qwadro conventions
//#define MFX_ALIGN_ALL // force SIMD alignment to afxV3d and other types.

AFXINL afxV2d         AfxV2dNdc(afxV2d const b, afxV2d const total);
AFXINL afxV2d         AfxV2dUnndc(afxV2d const b, afxV2d const total);

#endif//AFX_MATH_DEFS_H
