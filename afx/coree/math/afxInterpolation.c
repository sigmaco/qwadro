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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxTrigonometry.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/afxStream.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

_AFXINL afxV2d AfxV2dSlerp(afxV2d x, afxV2d y, afxReal t)
{
    x = AfxV2dNormalize(x, NIL);
    y = AfxV2dNormalize(y, NIL);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV2dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV2d tmp =
    {
        y.x - x.x * dotProd,
        y.y - x.y * dotProd
    };
    tmp = AfxV2dNormalize(tmp, NIL);

    return AFX_V2D( x.x * thetaCos + tmp.x * thetaSin,
                    x.y * thetaCos + tmp.y * thetaSin);
}

_AFXINL afxV3d AfxV3dSlerp(afxV3d x, afxV3d y, afxReal t)
{
    x = AfxV3dNormalize(x, NIL);
    y = AfxV3dNormalize(y, NIL);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV3dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV3d tmp =
    {
        y.x - x.x * dotProd,
        y.y - x.y * dotProd,
        y.z - x.z * dotProd
    };
    tmp = AfxV3dNormalize(tmp, NIL);

    return AFX_V3D( x.x * thetaCos + tmp.x * thetaSin,
                    x.y * thetaCos + tmp.y * thetaSin,
                    x.z * thetaCos + tmp.z * thetaSin);
}

_AFXINL afxV4d AfxV4dSlerp(afxV4d x, afxV4d y, afxReal t)
{
    x = AfxV4dNormalize(x, NIL);
    y = AfxV4dNormalize(y, NIL);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV4dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV4d tmp =
    {
        y.x - x.x * dotProd,
        y.y - x.y * dotProd,
        y.z - x.z * dotProd,
        y.w - x.w * dotProd
    };
    tmp = AfxV4dNormalize(tmp, NIL);

    return AFX_V4D( x.x * thetaCos + tmp.x * thetaSin,
                    x.y * thetaCos + tmp.y * thetaSin,
                    x.z * thetaCos + tmp.z * thetaSin,
                    x.w * thetaCos + tmp.w * thetaSin);
}

// Hermite

_AFXINL afxV2d AfxV2dHermite(afxV2d const posA, afxV2d const tanA, afxV2d const posB, afxV2d const tanB, afxReal t)
{
    afxError err = { 0 };

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, ta, pb, tb;
    pa = AfxV2dFill(2.f * t3 - 3.f * t2 + 1.f);
    ta = AfxV2dFill(t3 - 2.f * t2 + t);
    pb = AfxV2dFill(-2.f * t3 + 3.f * t2);
    tb = AfxV2dFill(t3 - t2);

    afxV2d v = AfxV2dMultiply(pa, posA);
    v = AfxV2dMad(v, ta, ta);
    v = AfxV2dMad(v, pb, pb);
    return AfxV2dMad(v, tb, tb);
}

_AFXINL afxV3d AfxV3dHermite(afxV3d const posA, afxV3d const tanA, afxV3d const posB, afxV3d const tanB, afxReal t)
{
    afxError err = { 0 };
    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, ta, pb, tb;
    pa = AfxV3dFill(2.f * t3 - 3.f * t2 + 1.f);
    ta = AfxV3dFill(t3 - 2.f * t2 + t);
    pb = AfxV3dFill(-2.f * t3 + 3.f * t2);
    tb = AfxV3dFill(t3 - t2);

    afxV3d v = AfxV3dMultiply(pa, posA);
    v = AfxV3dMad(v, ta, ta);
    v = AfxV3dMad(v, pb, pb);
    return AfxV3dMad(v, tb, tb);
}

_AFXINL afxV4d AfxV4dHermite(afxV4d const posA, afxV4d const tanA, afxV4d const posB, afxV4d const tanB, afxReal t)
{
    afxError err = { 0 };
    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, ta, pb, tb;
    pa = AfxV4dFill(2.f * t3 - 3.f * t2 + 1.f);
    ta = AfxV4dFill(t3 - 2.f * t2 + t);
    pb = AfxV4dFill(-2.f * t3 + 3.f * t2);
    tb = AfxV4dFill(t3 - t2);

    afxV4d v = AfxV4dMultiply(pa, posA);
    v = AfxV4dMad(v, ta, ta);
    v = AfxV4dMad(v, pb, pb);
    return AfxV4dMad(v, tb, tb);
}

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
/// The geometry matrix.

// CatmullRom

_AFXINL afxV2d AfxV2dCatmull(afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const d, afxReal t)
{
    afxError err = { 0 };
    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, pb, pc, pd;
    pa = AfxV2dFill((-t3 + 2.0f * t2 - t) * 0.5f);
    pb = AfxV2dFill((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    pc = AfxV2dFill((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    pd = AfxV2dFill((t3 - t2) * 0.5f);

    afxV2d v = AfxV2dMultiply(pa, a);
    v = AfxV2dMad(v, pb, b);
    v = AfxV2dMad(v, pc, c);
    return AfxV2dMad(v, pd, d);
}

_AFXINL afxV3d AfxV3dCatmull(afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const d, afxReal t)
{
    afxError err = { 0 };
    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, pb, pc, pd;
    pa = AfxV3dFill((-t3 + 2.0f * t2 - t) * 0.5f);
    pb = AfxV3dFill((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    pc = AfxV3dFill((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    pd = AfxV3dFill((t3 - t2) * 0.5f);

    afxV3d v = AfxV3dMultiply(pa, a);
    v = AfxV3dMad(v, pb, b);
    v = AfxV3dMad(v, pc, c);
    return AfxV3dMad(v, pd, d);
}

_AFXINL afxV4d AfxV4dCatmull(afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const d, afxReal t)
{
    afxError err = { 0 };
    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, pb, pc, pd;
    pa = AfxV4dFill((-t3 + 2.0f * t2 - t) * 0.5f);
    pb = AfxV4dFill((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    pc = AfxV4dFill((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    pd = AfxV4dFill((t3 - t2) * 0.5f);

    afxV4d v = AfxV4dMultiply(pa, a);
    v = AfxV4dMad(v, pb, b);
    v = AfxV4dMad(v, pc, c);
    return AfxV4dMad(v, pd, d);
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatLerp(afxQuat const a, afxQuat const b, afxReal percent)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/02/quaternions.html

    afxError err = { 0 };

    afxReal f = AFX_REAL(1) - percent;

    afxQuat q;

    if (AfxQuatDot(a, b) < 0.f)
    {
        q.w = f * a.w + percent * -b.w;
        q.x = f * a.x + percent * -b.x;
        q.y = f * a.y + percent * -b.y;
        q.z = f * a.z + percent * -b.z;
    }
    else
    {
        // mul & add
        q.w = f * a.w + percent * b.w;
        q.x = f * a.x + percent * b.x;
        q.y = f * a.y + percent * b.y;
        q.z = f * a.z + percent * b.z;
    }
    return AfxQuatNormalize(q, NIL);
}

_AFXINL afxQuat AfxQuatSlerp(afxQuat const a, afxQuat const b, afxReal percent)
{
    afxError err = { 0 };

    if (AfxRealIsEquivalent(percent, AFX_REAL(0)))
        return a;

    if (AfxRealIsEquivalent(percent, AFX_REAL(1)))
        return b;

    // if they are close q parallel, use LERP, This avoids div/0. At small angles, the slerp a lerp are the same.
    afxReal dot = AfxQuatDot(a, b);

    if (AfxRealIsEquivalent(dot, AFX_REAL(1)))
        return AfxQuatLerp(a, b, percent);

    afxQuat q;

    // if dot is negative, they are "pointing" away from one another, use the shortest arc instead (reverse end a start)
    // This has the effect of changing the direction of travel around the sphere beginning with "end" a going the b way around the sphere.

    if (dot < AFX_REAL(0))
    {
        afxQuat neg = AfxQuatNeg(a);
        q = AfxQuatSlerp(a, neg, percent);
        q = AfxQuatNeg(q);
    }
    else
    {
        // keep the dot product in the range that acos canv handle (shouldn't get here)
        dot = AfxClampd(dot, AFX_REAL(-1), AFX_REAL(1));
        afxReal theta = AfxAcosf(dot); // the angle between start a end in radians
        afxReal s = AfxSinf(theta), f1 = AfxSinf((AFX_REAL(1) - percent) * theta) / s, f2 = AfxSinf(percent * theta) / s; // compute negative a positive

        // mul & add
        q.w = f1 * a.w + f2 * b.w;
        q.x = f1 * a.x + f2 * b.x;
        q.y = f1 * a.y + f2 * b.y;
        q.z = f1 * a.z + f2 * b.z; 
    }
    return q;
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL afxM2d AfxM2dMix(afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };

    afxM2d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;

    return m;
}

_AFXINL afxM3d AfxM3dMix(afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };

    afxM3d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;
    m.m[0][2] = a.m[0][2] * (1.f - t) + b.m[0][2] * t;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;
    m.m[1][2] = a.m[1][2] * (1.f - t) + b.m[1][2] * t;

    m.m[2][0] = a.m[2][0] * (1.f - t) + b.m[2][0] * t;
    m.m[2][1] = a.m[2][1] * (1.f - t) + b.m[2][1] * t;
    m.m[2][2] = a.m[2][2] * (1.f - t) + b.m[2][2] * t;
}

_AFXINL afxM4d AfxM4dMix(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;
    m.m[0][2] = a.m[0][2] * (1.f - t) + b.m[0][2] * t;
    m.m[0][3] = a.m[0][3] * (1.f - t) + b.m[0][3] * t;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;
    m.m[1][2] = a.m[1][2] * (1.f - t) + b.m[1][2] * t;
    m.m[1][3] = a.m[1][3] * (1.f - t) + b.m[1][3] * t;

    m.m[2][0] = a.m[2][0] * (1.f - t) + b.m[2][0] * t;
    m.m[2][1] = a.m[2][1] * (1.f - t) + b.m[2][1] * t;
    m.m[2][2] = a.m[2][2] * (1.f - t) + b.m[2][2] * t;
    m.m[2][3] = a.m[2][3] * (1.f - t) + b.m[2][3] * t;

    m.m[3][0] = a.m[3][0] * (1.f - t) + b.m[3][0] * t;
    m.m[3][1] = a.m[3][1] * (1.f - t) + b.m[3][1] * t;
    m.m[3][2] = a.m[3][2] * (1.f - t) + b.m[3][2] * t;
    m.m[3][3] = a.m[3][3] * (1.f - t) + b.m[3][3] * t;
}

_AFXINL afxM3d AfxM3dMixAtm(afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };

    afxM3d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;
    m.m[0][2] = 0.f;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;
    m.m[1][2] = 0.f;

    m.m[2][0] = a.m[2][0] * (1.f - t) + b.m[2][0] * t;
    m.m[2][1] = a.m[2][1] * (1.f - t) + b.m[2][1] * t;
    m.m[2][2] = 1.f;
}

_AFXINL afxM4d AfxM4dMixAtm(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;
    m.m[0][2] = a.m[0][2] * (1.f - t) + b.m[0][2] * t;
    m.m[0][3] = 0.f;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;
    m.m[1][2] = a.m[1][2] * (1.f - t) + b.m[1][2] * t;
    m.m[1][3] = 0.f;

    m.m[2][0] = a.m[2][0] * (1.f - t) + b.m[2][0] * t;
    m.m[2][1] = a.m[2][1] * (1.f - t) + b.m[2][1] * t;
    m.m[2][2] = a.m[2][2] * (1.f - t) + b.m[2][2] * t;
    m.m[2][3] = 0.f;

    m.m[3][0] = a.m[3][0] * (1.f - t) + b.m[3][0] * t;
    m.m[3][1] = a.m[3][1] * (1.f - t) + b.m[3][1] * t;
    m.m[3][2] = a.m[3][2] * (1.f - t) + b.m[3][2] * t;
    m.m[3][3] = 1.f;
}

_AFXINL afxM4d AfxM4dMixLtm(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] * (1.f - t) + b.m[0][0] * t;
    m.m[0][1] = a.m[0][1] * (1.f - t) + b.m[0][1] * t;
    m.m[0][2] = a.m[0][2] * (1.f - t) + b.m[0][2] * t;
    m.m[0][3] = 0.f;

    m.m[1][0] = a.m[1][0] * (1.f - t) + b.m[1][0] * t;
    m.m[1][1] = a.m[1][1] * (1.f - t) + b.m[1][1] * t;
    m.m[1][2] = a.m[1][2] * (1.f - t) + b.m[1][2] * t;
    m.m[1][3] = 0.f;

    m.m[2][0] = a.m[2][0] * (1.f - t) + b.m[2][0] * t;
    m.m[2][1] = a.m[2][1] * (1.f - t) + b.m[2][1] * t;
    m.m[2][2] = a.m[2][2] * (1.f - t) + b.m[2][2] * t;
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
}

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

_AFXINL afxM2d AfxM2dLerp(afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };

    afxM2d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
}

_AFXINL afxM3d AfxM3dLerp(afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };

    afxM3d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);
    m.m[0][2] = a.m[0][2] + t * (b.m[0][2] - a.m[0][2]);

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
    m.m[1][2] = a.m[1][2] + t * (b.m[1][2] - a.m[1][2]);

    m.m[2][0] = a.m[2][0] + t * (b.m[2][0] - a.m[2][0]);
    m.m[2][1] = a.m[2][1] + t * (b.m[2][1] - a.m[2][1]);
    m.m[2][2] = a.m[2][2] + t * (b.m[2][2] - a.m[2][2]);
}

_AFXINL afxM4d AfxM4dLerp(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);
    m.m[0][2] = a.m[0][2] + t * (b.m[0][2] - a.m[0][2]);
    m.m[0][3] = a.m[0][3] + t * (b.m[0][3] - a.m[0][3]);

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
    m.m[1][2] = a.m[1][2] + t * (b.m[1][2] - a.m[1][2]);
    m.m[1][3] = a.m[1][3] + t * (b.m[1][3] - a.m[1][3]);

    m.m[2][0] = a.m[2][0] + t * (b.m[2][0] - a.m[2][0]);
    m.m[2][1] = a.m[2][1] + t * (b.m[2][1] - a.m[2][1]);
    m.m[2][2] = a.m[2][2] + t * (b.m[2][2] - a.m[2][2]);
    m.m[2][3] = a.m[2][3] + t * (b.m[2][3] - a.m[2][3]);

    m.m[3][0] = a.m[3][0] + t * (b.m[3][0] - a.m[3][0]);
    m.m[3][1] = a.m[3][1] + t * (b.m[3][1] - a.m[3][1]);
    m.m[3][2] = a.m[3][2] + t * (b.m[3][2] - a.m[3][2]);
    m.m[3][3] = a.m[3][3] + t * (b.m[3][3] - a.m[3][3]);
}

_AFXINL afxM3d AfxM3dLerpAtm(afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    
    afxM3d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);
    m.m[0][2] = 0.f;

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
    m.m[1][2] = 0.f;

    m.m[2][0] = a.m[2][0] + t * (b.m[2][0] - a.m[2][0]);
    m.m[2][1] = a.m[2][1] + t * (b.m[2][1] - a.m[2][1]);
    m.m[2][2] = 1.f;
}

_AFXINL afxM4d AfxM4dLerpAtm(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);
    m.m[0][2] = a.m[0][2] + t * (b.m[0][2] - a.m[0][2]);
    m.m[0][3] = 0.f;

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
    m.m[1][2] = a.m[1][2] + t * (b.m[1][2] - a.m[1][2]);
    m.m[1][3] = 0.f;

    m.m[2][0] = a.m[2][0] + t * (b.m[2][0] - a.m[2][0]);
    m.m[2][1] = a.m[2][1] + t * (b.m[2][1] - a.m[2][1]);
    m.m[2][2] = a.m[2][2] + t * (b.m[2][2] - a.m[2][2]);
    m.m[2][3] = 0.f;

    m.m[3][0] = a.m[3][0] + t * (b.m[3][0] - a.m[3][0]);
    m.m[3][1] = a.m[3][1] + t * (b.m[3][1] - a.m[3][1]);
    m.m[3][2] = a.m[3][2] + t * (b.m[3][2] - a.m[3][2]);
    m.m[3][3] = 1.f;
}

_AFXINL afxM4d AfxM4dLerpLtm(afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };

    afxM4d m;

    m.m[0][0] = a.m[0][0] + t * (b.m[0][0] - a.m[0][0]);
    m.m[0][1] = a.m[0][1] + t * (b.m[0][1] - a.m[0][1]);
    m.m[0][2] = a.m[0][2] + t * (b.m[0][2] - a.m[0][2]);
    m.m[0][3] = 0.f;

    m.m[1][0] = a.m[1][0] + t * (b.m[1][0] - a.m[1][0]);
    m.m[1][1] = a.m[1][1] + t * (b.m[1][1] - a.m[1][1]);
    m.m[1][2] = a.m[1][2] + t * (b.m[1][2] - a.m[1][2]);
    m.m[1][3] = 0.f;

    m.m[2][0] = a.m[2][0] + t * (b.m[2][0] - a.m[2][0]);
    m.m[2][1] = a.m[2][1] + t * (b.m[2][1] - a.m[2][1]);
    m.m[2][2] = a.m[2][2] + t * (b.m[2][2] - a.m[2][2]);
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
}
