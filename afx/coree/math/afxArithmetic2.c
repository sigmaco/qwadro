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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxArithmetic2.h"

////////////////////////////////////////////////////////////////////////////////
// EXTENDED ARITHMETICA                                                       //
////////////////////////////////////////////////////////////////////////////////

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL afxV2d AfxV2dMix(afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V2D( x.x * (1.f - t) + y.x * t,
                    x.y * (1.f - t) + y.y * t);
}

_AFXINL afxV3d AfxV3dMix(afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V3D( x.x * (1.f - t) + y.x * t,
                    x.y * (1.f - t) + y.y * t,
                    x.z * (1.f - t) + y.z * t);
}

_AFXINL afxV4d AfxV4dMix(afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V4D( x.x * (1.f - t) + y.x * t,
                    x.y * (1.f - t) + y.y * t,
                    x.z * (1.f - t) + y.z * t,
                    x.w * (1.f - t) + y.w * t);
}

// Lerp
// LERP is a method to interpolate linearly between two values. In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// v = x + t * (y - x)

_AFXINL afxV2d AfxV2dLerp(afxV2d const x, afxV2d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V2D( x.x + t * (y.x - x.x),
                    x.y + t * (y.y - x.y));
}

_AFXINL afxV3d AfxV3dLerp(afxV3d const x, afxV3d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V3D( x.x + t * (y.x - x.x),
                    x.y + t * (y.y - x.y),
                    x.z + t * (y.z - x.z));
}

_AFXINL afxV4d AfxV4dLerp(afxV4d const x, afxV4d const y, afxReal t)
{
    afxError err = { 0 };
    return AFX_V4D( x.x + t * (y.x - x.x),
                    x.y + t * (y.y - x.y),
                    x.z + t * (y.z - x.z),
                    x.w + t * (y.w - x.w));
}

// Square root

_AFXINL afxV2d AfxV2dSqrt(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxSqrtf(in.x),
                    AfxSqrtf(in.y));
}

_AFXINL afxV3d AfxV3dSqrt(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxSqrtf(in.x),
                    AfxSqrtf(in.y),
                    AfxSqrtf(in.z));
}

_AFXINL afxV4d AfxV4dSqrt(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxSqrtf(in.x),
                    AfxSqrtf(in.y),
                    AfxSqrtf(in.z),
                    AfxSqrtf(in.w));
}

// Reciprocal square root

_AFXINL afxV2d AfxV2dRsqrt(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxRsqrtf(in.x),
                    AfxRsqrtf(in.y));
}

_AFXINL afxV3d AfxV3dRsqrt(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxRsqrtf(in.x),
                    AfxRsqrtf(in.y),
                    AfxRsqrtf(in.z));
}

_AFXINL afxV4d AfxV4dRsqrt(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxRsqrtf(in.x),
                    AfxRsqrtf(in.y),
                    AfxRsqrtf(in.z),
                    AfxRsqrtf(in.w));
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

// Clamp

_AFXINL afxV2d AfxV2dClamp(afxV2d const in, afxV2d const min, afxV2d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(min.x <= max.x);
    AFX_ASSERT(min.y <= max.y);

    afxV2d v = AfxV2dMax(min, in);
    return AfxV2dMin(max, v);
}

_AFXINL afxV3d AfxV3dClamp(afxV3d const in, afxV3d const min, afxV3d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(min.x <= max.x);
    AFX_ASSERT(min.y <= max.y);
    AFX_ASSERT(min.z <= max.z);

    afxV3d v = AfxV3dMax(min, in);
    return AfxV3dMin(max, v);
}

_AFXINL afxV4d AfxV4dClamp(afxV4d const in, afxV4d const min, afxV4d const max)
{
    afxError err = { 0 };
    AFX_ASSERT(min.x <= max.x);
    AFX_ASSERT(min.y <= max.y);
    AFX_ASSERT(min.z <= max.z);
    AFX_ASSERT(min.w <= max.w);

    afxV4d v = AfxV4dMax(min, in);
    return AfxV4dMin(max, v);
}

// Truncate

_AFXINL afxV2d AfxV2dTruncate(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxIsNan(in.x) ? 0x7FC00000 : ((AFX_ABS(in.x) < 8388608.0f) ? (afxReal)(((afxInt32)(in.x))) : in.x),
                    AfxIsNan(in.y) ? 0x7FC00000 : ((AFX_ABS(in.y) < 8388608.0f) ? (afxReal)(((afxInt32)(in.y))) : in.y));
}

_AFXINL afxV3d AfxV3dTruncate(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxIsNan(in.x) ? 0x7FC00000 : ((AFX_ABS(in.x) < 8388608.0f) ? (afxReal)(((afxInt32)(in.x))) : in.x),
                    AfxIsNan(in.y) ? 0x7FC00000 : ((AFX_ABS(in.y) < 8388608.0f) ? (afxReal)(((afxInt32)(in.y))) : in.y),
                    AfxIsNan(in.z) ? 0x7FC00000 : ((AFX_ABS(in.z) < 8388608.0f) ? (afxReal)(((afxInt32)(in.z))) : in.z));
}

_AFXINL afxV4d AfxV4dTruncate(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxIsNan(in.x) ? 0x7FC00000 : ((AFX_ABS(in.x) < 8388608.0f) ? (afxReal)(((afxInt32)(in.x))) : in.x),
                    AfxIsNan(in.y) ? 0x7FC00000 : ((AFX_ABS(in.y) < 8388608.0f) ? (afxReal)(((afxInt32)(in.y))) : in.y),
                    AfxIsNan(in.z) ? 0x7FC00000 : ((AFX_ABS(in.z) < 8388608.0f) ? (afxReal)(((afxInt32)(in.z))) : in.z),
                    AfxIsNan(in.w) ? 0x7FC00000 : ((AFX_ABS(in.w) < 8388608.0f) ? (afxReal)(((afxInt32)(in.w))) : in.w));
}

// Saturate

_AFXINL afxV2d AfxV2dSaturate(afxV2d const in)
{
    afxError err = { 0 };
    return AfxV2dClamp(in, AFX_V2D_ZERO, AFX_V2D_ONE);
}

_AFXINL afxV3d AfxV3dSaturate(afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = { 0 };
    return AfxV3dClamp(in, AFX_V3D_ZERO, AFX_V3D_ONE);
}

_AFXINL afxV4d AfxV4dSaturate(afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorSaturate(FXMVECTOR V)

    afxError err = { 0 };
    return AfxV4dClamp(in, AFX_V4D_ZERO, AFX_V4D_ONE);
}

// Mini

_AFXINL afxV2d AfxV2dMin(afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    return AFX_V2D( a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y);
}

_AFXINL afxV3d AfxV3dMin(afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    return AFX_V3D( a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y,
                    a.z < b.z ? a.z : b.z);
}

_AFXINL afxV4d AfxV4dMin(afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    return AFX_V4D( a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y,
                    a.z < b.z ? a.z : b.z,
                    a.w < b.w ? a.w : b.w);
}

// Maxi

_AFXINL afxV2d AfxV2dMax(afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    return AFX_V2D( a.x > b.x ? a.x : b.x,
                    a.y > b.y ? a.y : b.y);
}

_AFXINL afxV3d AfxV3dMax(afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    return AFX_V3D( a.x > b.x ? a.x : b.x,
                    a.y > b.y ? a.y : b.y,
                    a.z > b.z ? a.z : b.z);
}

_AFXINL afxV4d AfxV4dMax(afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    return AFX_V4D( a.x > b.x ? a.x : b.x,
                    a.y > b.y ? a.y : b.y,
                    a.z > b.z ? a.z : b.z,
                    a.w > b.w ? a.w : b.w);
}

// Abs

_AFXINL afxV2d AfxV2dAbs(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AFX_ABS(in.x),
                    AFX_ABS(in.y));
}

_AFXINL afxV3d AfxV3dAbs(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AFX_ABS(in.x),
                    AFX_ABS(in.y),
                    AFX_ABS(in.z));
}

_AFXINL afxV4d AfxV4dAbs(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AFX_ABS(in.x),
                    AFX_ABS(in.y),
                    AFX_ABS(in.z),
                    AFX_ABS(in.w));
}

// Ceil

_AFXINL afxV2d AfxV2dCeil(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxCeilf(in.x),
                    AfxCeilf(in.y));
}

_AFXINL afxV3d AfxV3dCeil(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxCeilf(in.x),
                    AfxCeilf(in.y),
                    AfxCeilf(in.z));
}

_AFXINL afxV4d AfxV4dCeil(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxCeilf(in.x),
                    AfxCeilf(in.y),
                    AfxCeilf(in.z),
                    AfxCeilf(in.w));
}

// Floor

_AFXINL afxV2d AfxV2dFloor(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxFloorf(in.x),
                    AfxFloorf(in.y));
}

_AFXINL afxV3d AfxV3dFloor(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxFloorf(in.x),
                    AfxFloorf(in.y),
                    AfxFloorf(in.z));
}

_AFXINL afxV4d AfxV4dFloor(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxFloorf(in.x),
                    AfxFloorf(in.y),
                    AfxFloorf(in.z),
                    AfxFloorf(in.w));
}

////////////////////////////////////////////////////////////////////////////////
// EXPONENTIAL ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * EXPONENTIAL FUNCTIONS                                               *
 *                                                                     *
 * These all operate component-wise. The description is per component. *
 ***********************************************************************/

// Returns the natural exponentiation of x; e^x.

_AFXINL afxReal64 AfxExp(afxReal64 s)
{
    return exp(s);
}

_AFXINL afxReal AfxExpf(afxReal s)
{
    return expf(s);
}

// Returns 2 raised to the x power; 2^x.

_AFXINL afxReal64 AfxExp2(afxReal64 s)
{
    return exp2(s);
}

_AFXINL afxReal AfxExp2f(afxReal s)
{
    return exp2f(s);
}

_AFXINL afxReal64 AfxExp10(afxReal64 s)
{
    return AfxPow(10.0, s);
}

_AFXINL afxReal AfxExp10f(afxReal s)
{
    return AfxPowf(10.f, s);
}

// Exp

_AFXINL afxV2d AfxV2dExp(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxExpf(in.x),
                    AfxExpf(in.y));
}

_AFXINL afxV3d AfxV3dExp(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxExpf(in.x),
                    AfxExpf(in.y),
                    AfxExpf(in.z));
}

_AFXINL afxV4d AfxV4dExp(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxExpf(in.x),
                    AfxExpf(in.y),
                    AfxExpf(in.z),
                    AfxExpf(in.w));
}

// Exp2

_AFXINL afxV2d AfxV2dExp2(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxExp2f(in.x),
                    AfxExp2f(in.y));
}

_AFXINL afxV3d AfxV3dExp2(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxExp2f(in.x),
                    AfxExp2f(in.y),
                    AfxExp2f(in.z));
}

_AFXINL afxV4d AfxV4dExp2(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxExp2f(in.x),
                    AfxExp2f(in.y),
                    AfxExp2f(in.z),
                    AfxExp2f(in.w));
}

// Exp10

_AFXINL afxV2d AfxV2dExp10(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxExp10f(in.x),
                    AfxExp10f(in.y));
}

_AFXINL afxV3d AfxV3dExp10(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxExp10f(in.x),
                    AfxExp10f(in.y),
                    AfxExp10f(in.z));
}

_AFXINL afxV4d AfxV4dExp10(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxExp10f(in.x),
                    AfxExp10f(in.y),
                    AfxExp10f(in.z),
                    AfxExp10f(in.w));
}

// Returns x raised to the y power; x^y.
// Results are undefined if x < 0 or if x = 0 and y <= 0.

_AFXINL afxReal64 AfxPow(afxReal64 base, afxReal64 exp)
{
    return AfxExp(exp * AfxLog(base));
}

_AFXINL afxReal AfxPowf(afxReal base, afxReal exp)
{
    return AfxExpf(exp * AfxLogf(base));
}

// Pow

_AFXINL afxV2d AfxV2dPow(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxPowf(in.x, in.x),
                    AfxPowf(in.y, in.y));
}

_AFXINL afxV3d AfxV3dPow(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxPowf(in.x, in.x),
                    AfxPowf(in.y, in.y),
                    AfxPowf(in.z, in.z));
}

_AFXINL afxV4d AfxV4dPow(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxPowf(in.x, in.x),
                    AfxPowf(in.y, in.y),
                    AfxPowf(in.z, in.z),
                    AfxPowf(in.w, in.w));
}

////////////////////////////////////////////////////////////////////////////////
// LOGARITHMIC ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////


// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog(afxReal64 s)
{
    return log(s);
}

_AFXINL afxReal AfxLogf(afxReal s)
{
    return logf(s);
}

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

_AFXINL afxReal64 AfxLog2(afxReal64 s)
{
    return log2(s);
}

_AFXINL afxReal AfxLog2f(afxReal s)
{
    return log2f(s);
}

_AFXINL afxReal64 AfxLog10(afxReal64 s)
{
    return log10(s);
}

_AFXINL afxReal AfxLog10f(afxReal s)
{
    return log10f(s);
}

// Log

_AFXINL afxV2d AfxV2dLog(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxLogf(in.x),
                    AfxLogf(in.y));
}

_AFXINL afxV3d AfxV3dLog(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxLogf(in.x),
                    AfxLogf(in.y),
                    AfxLogf(in.z));
}

_AFXINL afxV4d AfxV4dLog(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxLogf(in.x),
                    AfxLogf(in.y),
                    AfxLogf(in.z),
                    AfxLogf(in.w));
}

// Log2

_AFXINL afxV2d AfxV2dLog2(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxLog2f(in.x),
                    AfxLog2f(in.y));
}

_AFXINL afxV3d AfxV3dLog2(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxLog2f(in.x),
                    AfxLog2f(in.y),
                    AfxLog2f(in.z));
}

_AFXINL afxV4d AfxV4dLog2(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxLog2f(in.x),
                    AfxLog2f(in.y),
                    AfxLog2f(in.z),
                    AfxLog2f(in.w));
}

// Log10

_AFXINL afxV2d AfxV2dLog10(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxLog10f(in.x),
                    AfxLog10f(in.y));
}

_AFXINL afxV3d AfxV3dLog10(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxLog10f(in.x),
                    AfxLog10f(in.y),
                    AfxLog10f(in.z));
}

_AFXINL afxV4d AfxV4dLog10(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxLog10f(in.x),
                    AfxLog10f(in.y),
                    AfxLog10f(in.z),
                    AfxLog10f(in.w));
}
