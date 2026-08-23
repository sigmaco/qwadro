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

#include "qwadro/math/afxTrigonometry.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"

/*********************************************************************************
 * ANGLE AND TRIGONOMETRY FUNCTIONS                                              *
 *                                                                               *
 * Function parameters specified as angle are assumed to be in units of radians. *
 * In no case will any of these functions result in a divide by zero error.      *
 * If the divisor of a ratio is 0, then results will be undefined.               *
 *                                                                               *
 * These all operate component-wise. The description is per component.           *
 *********************************************************************************/

 // Converts degrees to radians.
 // The value returned by this function is (pi / 180) * degrees.

_AFXINL afxReal64 AfxRad(afxReal64 degrees)
{
    return degrees * (AFX_PI / 180.0);
}

_AFXINL afxReal AfxRadf(afxReal degrees)
{
    return degrees * (AFX_PI / 180.f);
}

// Converts radians to degrees.
// The value returned by this function is (180 / pi) * radians.

_AFXINL afxReal64 AfxDeg(afxReal64 radians)
{
    return radians * (180.0 / AFX_PI);
}

_AFXINL afxReal AfxDegf(afxReal radians)
{
    return radians * (180.f / AFX_PI);
}

// The standard trigonometric sine function.

_AFXINL afxReal64 AfxSin(afxReal64 radians)
{
    return sin(radians);
}

_AFXINL afxReal AfxSinf(afxReal radians)
{
    return sinf(radians);
}

// Sine

_AFXINL afxV2d AfxV2dSin(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxSinf(in.x),
                    AfxSinf(in.y));
}

_AFXINL afxV3d AfxV3dSin(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxSinf(in.x),
                    AfxSinf(in.y),
                    AfxSinf(in.z));
}

_AFXINL afxV4d AfxV4dSin(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxSinf(in.x),
                    AfxSinf(in.y),
                    AfxSinf(in.z),
                    AfxSinf(in.w));
}

// The standard trigonometric cosine function.

_AFXINL afxReal64 AfxCos(afxReal64 radians)
{
    return cos(radians);
}

_AFXINL afxReal AfxCosf(afxReal radians)
{
    return cosf(radians);
}

// Cosine

_AFXINL afxV2d AfxV2dCos(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxCosf(in.x),
                    AfxCosf(in.y));
}

_AFXINL afxV3d AfxV3dCos(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxCosf(in.x),
                    AfxCosf(in.y),
                    AfxCosf(in.z));
}

_AFXINL afxV4d AfxV4dCos(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxCosf(in.x),
                    AfxCosf(in.y),
                    AfxCosf(in.z),
                    AfxCosf(in.w));
}

// A combined cosine and sine function

_AFXINL void AfxCosSin(afxReal64 radians, afxReal64* cos, afxReal64* sin)
{
    *cos = AfxCos(radians);
    *sin = AfxSin(radians);
}

_AFXINL void AfxCosSinf(afxReal radians, afxReal* cos, afxReal* sin)
{
    *cos = AfxCosf(radians);
    *sin = AfxSinf(radians);
}

// The standard trigonometric tangent function.

_AFXINL afxReal64 AfxTan(afxReal64 radians)
{
    return tan(radians);
}

_AFXINL afxReal AfxTanf(afxReal radians)
{
    return tanf(radians);
}

// Tangent

_AFXINL afxV2d AfxV2dTan(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxTanf(in.x),
                    AfxTanf(in.y));
}

_AFXINL afxV3d AfxV3dTan(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxTanf(in.x),
                    AfxTanf(in.y),
                    AfxTanf(in.z));
}

_AFXINL afxV4d AfxV4dTan(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxTanf(in.x),
                    AfxTanf(in.y),
                    AfxTanf(in.z),
                    AfxTanf(in.w));
}

// Arc sine. Returns an angle whose sine is x.
// The range of values returned by this function is [-pi / 2, pi / 2].
// Results are undefined if |x| > 1.

_AFXINL afxReal64 AfxAsin(afxReal64 x)
{
    return asin(x);
}

_AFXINL afxReal AfxAsinf(afxReal x)
{
    return asinf(x);
}

// Arc cosine. Returns an angle whose cosine is x.
// The range of values returned by this function is [0, pi].
// Results are undefined if |x| > 1.

_AFXINL afxReal64 AfxAcos(afxReal64 x)
{
    return acos(x);
}

_AFXINL afxReal AfxAcosf(afxReal x)
{
    return acosf(x);
}

// Arc cosine

_AFXINL afxV2d AfxV2dAcos(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxAcosf(in.x),
                    AfxAcosf(in.y));
}

_AFXINL afxV3d AfxV3dAcos(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxAcosf(in.x),
                    AfxAcosf(in.y),
                    AfxAcosf(in.z));
}

_AFXINL afxV4d AfxV4dAcos(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxAcosf(in.x),
                    AfxAcosf(in.y),
                    AfxAcosf(in.z),
                    AfxAcosf(in.w));
}

// Arc tangent. Returns an angle whose tangent is y_over_x.
// The range of values returned by this function is [-pi / 2, pi / 2].

_AFXINL afxReal64 AfxAtan(afxReal64 y_over_x)
{
    return atan(y_over_x);
}

_AFXINL afxReal AfxAtanf(afxReal y_over_x)
{
    return atanf(y_over_x);
}

// Arc tangent

_AFXINL afxV2d AfxV2dAtan(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( AfxAtanf(in.x),
                    AfxAtanf(in.y));
}

_AFXINL afxV3d AfxV3dAtan(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( AfxAtanf(in.x),
                    AfxAtanf(in.y),
                    AfxAtanf(in.z));
}

_AFXINL afxV4d AfxV4dAtan(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( AfxAtanf(in.x),
                    AfxAtanf(in.y),
                    AfxAtanf(in.z),
                    AfxAtanf(in.w));
}

// Arc tangent. Returns an angle whose tangent is y / x.
// The signs of x and y are used to determine what quadrant the angle is in.
// The range of values returned by this function is [-pi, pi].
// Results are undefined if xand y are both 0.

_AFXINL afxReal64 AfxAtan2(afxReal64 y, afxReal64 x)
{
    return atan2(y, x);
}

_AFXINL afxReal AfxAtan2f(afxReal y, afxReal x)
{
    return atan2f(y, x);
}







_AFXINL afxV3d AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxV3d t = AfxV3dSub(b, a);
    afxV3d t2 = AfxV3dSub(c, a);
    afxV3d n = AfxV3dCross(t, t2);
    return AfxV3dNormalize(n, NIL);
}

// BaryCentric

_AFXINL afxV2d AfxV2dBarycentric(afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // Adapted from DirectXMath. Works?

    afxV2d p10 = AfxV2dSub(b, a);
    afxV2d fv = AfxV2dFill(f);
    afxV2d p20 = AfxV2dSub(c, a);
    afxV2d gv = AfxV2dFill(g);
    afxV2d tmp = AfxV2dMad(a, p10, fv);
    return AfxV2dMad(tmp, p20, gv);
}

_AFXINL afxV3d AfxV3dBarycentric(afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV3d p10 = AfxV3dSub(b, a);
    afxV3d fv = AfxV3dFill(f);
    afxV3d p20 = AfxV3dSub(c, a);
    afxV3d gv = AfxV3dFill(g);
    afxV3d tmp = AfxV3dMad(a, p10, fv);
    return AfxV3dMad(tmp, p20, gv);
}

_AFXINL afxV4d AfxV4dBarycentric(afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV4d p10 = AfxV4dSub(b, a);
    afxV4d fv = AfxV4dFill(f);
    afxV4d p20 = AfxV4dSub(c, a);
    afxV4d gv = AfxV4dFill(g);
    afxV4d tmp = AfxV4dMad(a, p10, fv);
    return AfxV4dMad(tmp, p20, gv);
}

// BaryCentric2

_AFXINL afxV2d AfxV2dBarycentric2(afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV2d p10 = AfxV2dSub(b, a);
    afxV2d p20 = AfxV2dSub(c, a);
    afxV2d tmp = AfxV2dMad(a, p10, f);
    return AfxV2dMad(tmp, p20, g);
}

_AFXINL afxV3d AfxV3dBarycentric2(afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV3d p10 = AfxV3dSub(b, a);
    afxV3d p20 = AfxV3dSub(c, a);
    afxV3d tmp = AfxV3dMad(a, p10, f);
    return AfxV3dMad(tmp, p20, g);
}

_AFXINL afxV4d AfxV4dBarycentric2(afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV4d p10 = AfxV4dSub(b, a);
    afxV4d p20 = AfxV4dSub(c, a);
    afxV4d tmp = AfxV4dMad(a, p10, f);
    return AfxV4dMad(tmp, p20, g);
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatBarycentric(afxQuat const a, afxQuat const b, afxQuat const c, afxReal f, afxReal g)
{
    // Should be compatible with  XMVECTOR XMQuaternionBaryCentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float f, float g)

    afxError err = { 0 };

    afxReal s = f + g;

    if ((s < 0.00001f) && (s > -0.00001f))
        return a;

    afxQuat ab = AfxQuatSlerp(a, b, s);
    afxQuat ac = AfxQuatSlerp(a, c, s);
    return AfxQuatSlerp(ab, ac, g / s);
}
