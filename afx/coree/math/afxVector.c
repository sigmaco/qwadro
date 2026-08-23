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

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"

// Memory layout: row-major


////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

// Zero

_AFXINL afxV2d AfxV2dZero(void)
{
    afxError err = { 0 };
    return AFX_V2D_ZERO;
}

_AFXINL afxV3d AfxV3dZero(void)
{
    afxError err = { 0 };
    return AFX_V3D_ZERO;
}

_AFXINL afxV4d AfxV4dZero(void)
{
    afxError err = { 0 };
    return AFX_V4D_ZERO;
}

// MakeIdentity

_AFXINL afxV4d AfxV4dIdentity(void)
{
    afxError err = { 0 };
    return AFX_V4D_W;
}

// Fill

_AFXINL afxV2d AfxV2dFill(afxReal value)
{
    afxError err = { 0 };
    return AFX_V2D(value, value);
}

_AFXINL afxV3d AfxV3dFill(afxReal value)
{
    afxError err = { 0 };
    return AFX_V3D(value, value, value);
}

_AFXINL afxV4d AfxV4dFill(afxReal value)
{
    afxError err = { 0 };
    return AFX_V4D(value, value, value, value);
}

// Make

_AFXINL afxV2d AfxV2dMake(afxReal x, afxReal y)
{
    afxError err = { 0 };
    return AFX_V2D(x, y);
}

_AFXINL afxV3d AfxV3dMake(afxReal x, afxReal y, afxReal z)
{
    afxError err = { 0 };
    return AFX_V3D(x, y, z);
}

_AFXINL afxV4d AfxV4dMake(afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = { 0 };
    return AFX_V4D(x, y, z, w);
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

// IsIdentity

_AFXINL afxBool AfxV4dIsIdentity(afxV4d const v)
{
    return (v.x + v.y + v.z) == 0 && (v.w == 1);
}

// IsNearEqual

_AFXINL afxBool AfxV2dIsNearEqual(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return  (AFX_REAL(v.x < other.x + AFX_EPSILON && v.x > other.x - AFX_EPSILON)) +
            (AFX_REAL(v.y < other.y + AFX_EPSILON && v.y > other.y - AFX_EPSILON));
}

_AFXINL afxBool AfxV3dIsNearEqual(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return  (AFX_REAL(v.x < other.x + AFX_EPSILON && v.x > other.x - AFX_EPSILON)) +
            (AFX_REAL(v.y < other.y + AFX_EPSILON && v.y > other.y - AFX_EPSILON)) +
            (AFX_REAL(v.z < other.z + AFX_EPSILON && v.z > other.z - AFX_EPSILON));
}

_AFXINL afxBool AfxV4dIsNearEqual(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return  (AFX_REAL(v.x < other.x + AFX_EPSILON && v.x > other.x - AFX_EPSILON)) +
            (AFX_REAL(v.y < other.y + AFX_EPSILON && v.y > other.y - AFX_EPSILON)) +
            (AFX_REAL(v.z < other.z + AFX_EPSILON && v.z > other.z - AFX_EPSILON)) +
            (AFX_REAL(v.w < other.w + AFX_EPSILON && v.w > other.w - AFX_EPSILON));
}

// IsEqual

_AFXINL afxBool AfxV2dIsEqual(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x == other.x, v.y == other.y, 0, 0);
}

_AFXINL afxBool AfxV3dIsEqual(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x == other.x, v.y == other.y, v.z == other.z, 0);
}

_AFXINL afxBool AfxV4dIsEqual(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x == other.x, v.y == other.y, v.z == other.z, v.w == other.w);
}

// IsDiff

_AFXINL afxBool AfxV2dIsDiff(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x != other.x, v.y != other.y, 0, 0);
}

_AFXINL afxBool AfxV3dIsDiff(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x != other.x, v.y != other.y, v.z != other.z, 0);
}

_AFXINL afxBool AfxV4dIsDiff(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return AFX_WORD32(v.x != other.x, v.y != other.y, v.z != other.z, v.w != other.w);
}

// IsInBounds

_AFXINL afxBool AfxV2dIsInBounds(afxV2d const v, afxV2d const bounds)
{
    afxError err = { 0 };
    return (((v.x <= bounds.x && v.x >= -bounds.x) && (v.y <= bounds.y && v.y >= -bounds.y)) != 0);
}

_AFXINL afxBool AfxV3dIsInBounds(afxV3d const v, afxV3d const bounds)
{
    afxError err = { 0 };
    return (((v.x <= bounds.x && v.x >= -bounds.x) && (v.y <= bounds.y && v.y >= -bounds.y) && (v.z <= bounds.z && v.z >= -bounds.z)) != 0);
}

_AFXINL afxBool AfxV4dIsInBounds(afxV4d const v, afxV4d const bounds)
{
    afxError err = { 0 };
    return (((v.x <= bounds.x && v.x >= -bounds.x) && (v.y <= bounds.y && v.y >= -bounds.y) && (v.z <= bounds.z && v.z >= -bounds.z) && (v.w <= bounds.w && v.w >= -bounds.w)) != 0);
}

// IsLessOrEqual

_AFXINL afxBool AfxV2dIsLessOrEqual(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return (((v.x <= other.x) && (v.y <= other.y)) != 0);
}

_AFXINL afxBool AfxV3dIsLessOrEqual(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return (((v.x <= other.x) && (v.y <= other.y) && (v.z <= other.z)) != 0);
}

_AFXINL afxBool AfxV4dIsLessOrEqual(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return (((v.x <= other.x) && (v.y <= other.y) && (v.z <= other.z) && (v.w <= other.w)) != 0);
}

// IsLessThan

_AFXINL afxBool AfxV2dIsLessThan(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return (((v.x < other.x) && (v.y < other.y)) != 0);
}

_AFXINL afxBool AfxV3dIsLessThan(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return (((v.x < other.x) && (v.y < other.y) && (v.z < other.z)) != 0);
}

_AFXINL afxBool AfxV4dIsLessThan(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return (((v.x < other.x) && (v.y < other.y) && (v.z < other.z) && (v.w < other.w)) != 0);
}

// IsZero

_AFXINL afxBool AfxV2dIsZero(afxV2d const v)
{
    afxError err = { 0 };
    return ((v.x == 0) && (v.y == 0));
}

_AFXINL afxBool AfxV3dIsZero(afxV3d const v)
{
    afxError err = { 0 };
    return ((v.x == 0) && (v.y == 0) && (v.z == 0));
}

_AFXINL afxBool AfxV4dIsZero(afxV4d const v)
{
    afxError err = { 0 };
    return ((v.x == 0) && (v.y == 0) && (v.z == 0) && (v.w == 0));
}

// IsInfinite

_AFXINL afxBool AfxV2dIsInfinite(afxV2d const v)
{
    afxError err = { 0 };
    return  (AfxIsInfinite(v.x) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.y) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV3dIsInfinite(afxV3d const v)
{
    afxError err = { 0 };
    return  (AfxIsInfinite(v.x) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.y) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.z) ? 0xFFFFFFFFU : 0);
}

_AFXINL afxBool AfxV4dIsInfinite(afxV4d const v)
{
    afxError err = { 0 };
    return  (AfxIsInfinite(v.x) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.y) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.z) ? 0xFFFFFFFFU : 0) | 
            (AfxIsInfinite(v.w) ? 0xFFFFFFFFU : 0);
}

// IsNormalized

_AFXINL afxBool AfxV3dIsNormalized(afxV3d const v)
{
    // From Crytek

    afxError err = { 0 };
    afxReal f = AfxV3dMag(v);
    return f >= 0.95f && f <= 1.05f;
}

// IsNan

_AFXINL afxBool AfxV2dIsNan(afxV2d const v)
{
    afxError err = { 0 };
    return (AfxIsNan(v.x) || AfxIsNan(v.y));
}

_AFXINL afxBool AfxV3dIsNan(afxV3d const v)
{
    afxError err = { 0 };
    return (AfxIsNan(v.x) || AfxIsNan(v.y) || AfxIsNan(v.z));
}

_AFXINL afxBool AfxV4dIsNan(afxV4d const v)
{
    afxError err = { 0 };
    return (AfxIsNan(v.x) || AfxIsNan(v.y) || AfxIsNan(v.z) || AfxIsNan(v.w));
}

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL afxV3d AfxV3dFromV2d(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V3D(in.x, in.y, 0);
}

_AFXINL afxV3d AfxV3dFromAtv2d(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V3D(in.x, in.y, 1);
}

_AFXINL afxV3d AfxV3dFromV4d(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V3D(in.x, in.y, in.z);
}

_AFXINL afxV4d AfxV4dFromV2d(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V4D(in.x, in.y, 0, 0);
}

_AFXINL afxV4d AfxV4dFromV3d(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V4D(in.x, in.y, in.z, 0);
}

_AFXINL afxV4d AfxV4dFromAtv3d(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V4D(in.x, in.y, in.z, 1);
}

// Normalize

_AFXINL afxV2d AfxV2dNormalize(afxV2d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV2dMag(in);
    if (len > 0) len = 1.f / len;
    if (length) *length = len;

    return AfxV2dScale(in, len);
}

_AFXINL afxV3d AfxV3dNormalize(afxV3d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV3dMag(in);
    if (len > 0) len = 1.f / len;
    if (length) *length = len;

    return AfxV3dScale(in, len);
}

_AFXINL afxV4d AfxV4dNormalize(afxV4d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV4dMag(in);
    if (len > 0) len = 1.f / len;
    if (length) *length = len;

    return AfxV4dScale(in, len);
}

// NormalizeV*d

_AFXINL afxV3d AfxV3dNormalizeFromV4d(afxV4d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV4dMag(in);
    if (len > 0) len = 1.f / len;
    if (length) *length = len;

    return AfxV3dScale(AfxV3dFromV4d(in), len);
}

_AFXINL afxV4d AfxV4dNormalizeFromV3d(afxV3d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV3dMag(in);
    if (len > 0) len = 1.f / len;
    if (length) *length = len;

    return AfxV4dScale(AfxV4dFromV3d(in), len);
}

// NormalizeEst

_AFXINL afxV2d AfxV2dNormalizeEstimated(afxV2d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV2dMagRecip(in);
    if (length) *length = len;

    return AfxV2dScale(in, len);
}

_AFXINL afxV3d AfxV3dNormalizeEstimated(afxV3d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV3dMagRecip(in);
    if (length) *length = len;

    return AfxV3dScale(in, len);
}

_AFXINL afxV4d AfxV4dNormalizeEstimated(afxV4d const in, afxReal* length)
{
    afxError err = { 0 };

    afxReal len = AfxV4dMagRecip(in);
    if (length) *length = len;

    return AfxV4dScale(in, len);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Sum

_AFXINL afxReal AfxV2dSum(afxV2d const v)
{
    afxError err = { 0 };
    return v.x + v.y;
}

_AFXINL afxReal AfxV3dSum(afxV3d const v)
{
    afxError err = { 0 };
    return v.x + v.y + v.z;
}

_AFXINL afxReal AfxV4dSum(afxV4d const v)
{
    afxError err = { 0 };
    return v.x + v.y + v.z + v.w;
}

// Dot

_AFXINL afxReal AfxV2dDot(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return  (v.x * other.x) + 
            (v.y * other.y);
}

_AFXINL afxReal AfxV3dDot(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return  (v.x * other.x) + 
            (v.y * other.y) + 
            (v.z * other.z);
}

_AFXINL afxReal AfxV4dDot(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return  (v.x * other.x) + 
            (v.y * other.y) + 
            (v.z * other.z) + 
            (v.w * other.w);
}

// Dist
// Function to calculate the distance between two points

_AFXINL afxReal AfxV2dDist(afxV2d const v, afxV2d const other)
{
    afxError err = { 0 };
    return AfxSqrtf(
                    (v.x - other.x) * (v.x - other.x) +
                    (v.y - other.y) * (v.y - other.y));
}

_AFXINL afxReal AfxV3dDist(afxV3d const v, afxV3d const other)
{
    afxError err = { 0 };
    return AfxSqrtf(
                    (v.x - other.x) * (v.x - other.x) + 
                    (v.y - other.y) * (v.y - other.y) + 
                    (v.z - other.z) * (v.z - other.z));
}

_AFXINL afxReal AfxV4dDist(afxV4d const v, afxV4d const other)
{
    afxError err = { 0 };
    return AfxSqrtf(
                    (v.x - other.x) * (v.x - other.x) +
                    (v.y - other.y) * (v.y - other.y) +
                    (v.z - other.z) * (v.z - other.z) +
                    (v.w - other.w) * (v.w - other.w));
}

// Cross

_AFXINL afxV2d AfxV2dCross(afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };

    // [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]    
    return AFX_V2D_S((a.x * b.y) - (a.y * b.x));
}

_AFXINL afxV3d AfxV3dCross(afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    return AFX_V3D( ((a.y * b.z) - (a.z * b.y)),
                    ((a.z * b.x) - (a.x * b.z)),
                    ((a.x * b.y) - (a.y * b.x)));
}

_AFXINL afxV4d AfxV4dCross(afxV4d const a, afxV4d const b, afxV4d const c)
{
    afxError err = { 0 };
    return  AFX_V4D(    ((((b.z * c.w) - (b.w * c.z)) * a.y) - (((b.y * c.w) - (b.w * c.y)) * a.z) + (((b.y * c.z) - (b.z * c.y)) * a.w)),
                        ((((b.w * c.z) - (b.z * c.w)) * a.x) - (((b.w * c.x) - (b.x * c.w)) * a.z) + (((b.z * c.x) - (b.x * c.z)) * a.w)),
                        ((((b.y * c.w) - (b.w * c.y)) * a.x) - (((b.x * c.w) - (b.w * c.x)) * a.y) + (((b.x * c.y) - (b.y * c.x)) * a.w)),
                        ((((b.z * c.y) - (b.y * c.z)) * a.x) - (((b.z * c.x) - (b.x * c.z)) * a.y) + (((b.y * c.x) - (b.x * c.y)) * a.z)));
}

// Norm

_AFXINL afxReal AfxV2dSq(afxV2d const v)
{
    afxError err = { 0 };
    return AfxV2dDot(v, v);
}

_AFXINL afxReal AfxV3dSq(afxV3d const v)
{
    afxError err = { 0 };
    return AfxV3dDot(v, v);
}

_AFXINL afxReal AfxV4dSq(afxV4d const v)
{
    afxError err = { 0 };
    return AfxV4dDot(v, v);
}

// Magnitude

_AFXINL afxReal AfxV2dMag(afxV2d const v)
{
    afxError err = { 0 };
    return AfxSqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMag(afxV3d const v)
{
    afxError err = { 0 };
    return AfxSqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMag(afxV4d const v)
{
    afxError err = { 0 };
    return AfxSqrtf(AfxV4dSq(v));
}

// MagnitudeRecip

_AFXINL afxReal AfxV2dMagRecip(afxV2d const v)
{
    afxError err = { 0 };
    return AfxRsqrtf(AfxV2dSq(v));
}

_AFXINL afxReal AfxV3dMagRecip(afxV3d const v)
{
    afxError err = { 0 };
    return AfxRsqrtf(AfxV3dSq(v));
}

_AFXINL afxReal AfxV4dMagRecip(afxV4d const v)
{
    afxError err = { 0 };
    return AfxRsqrtf(AfxV4dSq(v));
}

// AfxCopyArrayed

_AFXINL void AfxCopyArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = in[i];
}

_AFXINL void AfxCopyArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = in[i];
}

_AFXINL void AfxCopyArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(out);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(out, in);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = in[i];
}

// AfxNormalizeArrayed

_AFXINL void AfxNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV2dNormalize(in[i], NIL);
}

_AFXINL void AfxNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dNormalize(in[i], NIL);
}

_AFXINL void AfxNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dNormalize(in[i], NIL);
}

// ZeroOrNormalizeArrayed

_AFXINL void AfxZeroOrNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV2dMag(in[i]);

        if (len <= 0.0000099999997f)
            out[i] = AfxV2dZero();
        else
            out[i] = AfxV2dScale(in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV3dMag(in[i]);

        if (len <= 0.0000099999997f)
            out[i] = AfxV3dZero();
        else
            out[i] = AfxV3dScale(in[i], 1.f / len);
    }
}

_AFXINL void AfxZeroOrNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxV4dMag(in[i]);

        if (len <= 0.0000099999997f)
            out[i] = AfxV4dZero();
        else
            out[i] = AfxV4dScale(in[i], 1.f / len);
    }
}
