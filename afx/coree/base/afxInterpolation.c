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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/io/afxStream.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

_AFXINL void AfxV2dSlerp(afxV2d v, afxV2d x, afxV2d y, afxReal t)
{
    AfxV2dNormalize(x, x);
    AfxV2dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV2dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV2d tmp =
    {
        y[0] - x[0] * dotProd,
        y[1] - x[1] * dotProd
    };
    AfxV2dNormalize(tmp, tmp);

    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
}

_AFXINL void AfxV3dSlerp(afxV3d v, afxV3d x, afxV3d y, afxReal t)
{
    AfxV3dNormalize(x, x);
    AfxV3dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV3dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV3d tmp =
    {
        y[0] - x[0] * dotProd,
        y[1] - x[1] * dotProd,
        y[2] - x[2] * dotProd
    };
    AfxV3dNormalize(tmp, tmp);

    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
    v[2] = x[2] * thetaCos + tmp[2] * thetaSin;
}

_AFXINL void AfxV4dSlerp(afxV4d v, afxV4d x, afxV4d y, afxReal t)
{
    AfxV4dNormalize(x, x);
    AfxV4dNormalize(y, y);

    afxReal dotProd = AFX_MAX(AFX_MIN(AfxV4dDot(x, y), 1.0f), -1.0f);

    afxReal theta_0 = AfxAcos(dotProd);
    afxReal theta = theta_0 * t;
    afxReal thetaCos = AfxCos(theta);
    afxReal thetaSin = AfxSin(theta);

    afxV4d tmp =
    {
        y[0] - x[0] * dotProd,
        y[1] - x[1] * dotProd,
        y[2] - x[2] * dotProd,
        y[3] - x[3] * dotProd
    };
    AfxV4dNormalize(tmp, tmp);

    v[0] = x[0] * thetaCos + tmp[0] * thetaSin;
    v[1] = x[1] * thetaCos + tmp[1] * thetaSin;
    v[2] = x[2] * thetaCos + tmp[2] * thetaSin;
    v[3] = x[3] * thetaCos + tmp[3] * thetaSin;
}

// Hermite

_AFXINL void AfxHermiteV2d(afxV2d v, afxV2d const posA, afxV2d const tanA, afxV2d const posB, afxV2d const tanB, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(posA);
    AFX_ASSERT(tanA);
    AFX_ASSERT(posB);
    AFX_ASSERT(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, ta, pb, tb;
    AfxV2dFill(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxV2dFill(ta, t3 - 2.f * t2 + t);
    AfxV2dFill(pb, -2.f * t3 + 3.f * t2);
    AfxV2dFill(tb, t3 - t2);

    AfxV2dMultiply(v, pa, posA);
    AfxV2dMad(v, v, ta, ta);
    AfxV2dMad(v, v, pb, pb);
    AfxV2dMad(v, v, tb, tb);
}

_AFXINL void AfxHermiteV3d(afxV3d v, afxV3d const posA, afxV3d const tanA, afxV3d const posB, afxV3d const tanB, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(posA);
    AFX_ASSERT(tanA);
    AFX_ASSERT(posB);
    AFX_ASSERT(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, ta, pb, tb;
    AfxV3dFill(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxV3dFill(ta, t3 - 2.f * t2 + t);
    AfxV3dFill(pb, -2.f * t3 + 3.f * t2);
    AfxV3dFill(tb, t3 - t2);

    AfxV3dMultiply(v, pa, posA);
    AfxV3dMad(v, v, ta, ta);
    AfxV3dMad(v, v, pb, pb);
    AfxV3dMad(v, v, tb, tb);
}

_AFXINL void AfxHermiteV4d(afxV4d v, afxV4d const posA, afxV4d const tanA, afxV4d const posB, afxV4d const tanB, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(posA);
    AFX_ASSERT(tanA);
    AFX_ASSERT(posB);
    AFX_ASSERT(tanB);

    // Result = (2 * t^3 - 3 * t^2 + 1) * Pos0 + (t^3 - 2 * t^2 + t) * Tan0 + (-2 * t^3 + 3 * t^2) * Pos1 + (t^3 - t^2) * Tan1

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, ta, pb, tb;
    AfxV4dFill(pa, 2.f * t3 - 3.f * t2 + 1.f);
    AfxV4dFill(ta, t3 - 2.f * t2 + t);
    AfxV4dFill(pb, -2.f * t3 + 3.f * t2);
    AfxV4dFill(tb, t3 - t2);

    AfxV4dMultiply(v, pa, posA);
    AfxV4dMad(v, v, ta, ta);
    AfxV4dMad(v, v, pb, pb);
    AfxV4dMad(v, v, tb, tb);
}

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
/// The geometry matrix.

// CatmullRom

_AFXINL void AfxCatmullV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const d, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    AFX_ASSERT(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV2d pa, pb, pc, pd;
    AfxV2dFill(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxV2dFill(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxV2dFill(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxV2dFill(pd, (t3 - t2) * 0.5f);

    AfxV2dMultiply(v, pa, a);
    AfxV2dMad(v, v, pb, b);
    AfxV2dMad(v, v, pc, c);
    AfxV2dMad(v, v, pd, d);
}

_AFXINL void AfxCatmullV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const d, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    AFX_ASSERT(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV3d pa, pb, pc, pd;
    AfxV3dFill(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxV3dFill(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxV3dFill(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxV3dFill(pd, (t3 - t2) * 0.5f);

    AfxV3dMultiply(v, pa, a);
    AfxV3dMad(v, v, pb, b);
    AfxV3dMad(v, v, pc, c);
    AfxV3dMad(v, v, pd, d);
}

_AFXINL void AfxCatmullV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const d, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);
    AFX_ASSERT(d);

    // v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

    afxReal t2 = t * t, t3 = t * t2;
    afxV4d pa, pb, pc, pd;
    AfxV4dFill(pa, (-t3 + 2.0f * t2 - t) * 0.5f);
    AfxV4dFill(pb, (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
    AfxV4dFill(pc, (-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
    AfxV4dFill(pd, (t3 - t2) * 0.5f);

    AfxV4dMultiply(v, pa, a);
    AfxV4dMad(v, v, pb, b);
    AfxV4dMad(v, v, pc, c);
    AfxV4dMad(v, v, pd, d);
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatLerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/02/quaternions.html

    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q);

    afxReal f = AFX_REAL(1) - percent;

    if (AfxQuatDot(a, b) < 0.f)
    {
        q[3] = f * a[3] + percent * -b[3];
        q[0] = f * a[0] + percent * -b[0];
        q[1] = f * a[1] + percent * -b[1];
        q[2] = f * a[2] + percent * -b[2];
    }
    else
    {
        // mul & add
        q[3] = f * a[3] + percent * b[3];
        q[0] = f * a[0] + percent * b[0];
        q[1] = f * a[1] + percent * b[1];
        q[2] = f * a[2] + percent * b[2];
    }
    AfxQuatNormalize(q, q);
}

_AFXINL void AfxQuatSlerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q);

    if (AfxRealIsEquivalent(percent, AFX_REAL(0))) AfxQuatCopy(q, a);
    else
    {
        if (AfxRealIsEquivalent(percent, AFX_REAL(1))) AfxQuatCopy(q, b);
        else
        {
            // if they are close q parallel, use LERP, This avoids div/0. At small angles, the slerp a lerp are the same.
            afxReal dot = AfxQuatDot(a, b);

            if (AfxRealIsEquivalent(dot, AFX_REAL(1))) AfxQuatLerp(q, a, q, percent);
            else
            {
                // if dot is negative, they are "pointing" away from one another, use the shortest arc instead (reverse end a start)
                // This has the effect of changing the direction of travel around the sphere beginning with "end" a going the b way around the sphere.

                if (dot < AFX_REAL(0))
                {
                    afxQuat neg;
                    AfxQuatNeg(neg, a);
                    AfxQuatSlerp(q, a, neg, percent);
                    AfxQuatNeg(q, q);
                }
                else
                {
                    // keep the dot product in the range that acos canv handle (shouldn't get here)
                    dot = AfxClampd(dot, AFX_REAL(-1), AFX_REAL(1));
                    afxReal theta = AfxAcosf(dot); // the angle between start a end in radians
                    afxReal s = AfxSinf(theta), f1 = AfxSinf((AFX_REAL(1) - percent) * theta) / s, f2 = AfxSinf(percent * theta) / s; // compute negative a positive

                    // mul & add
                    q[3] = f1 * a[3] + f2 * b[3];
                    q[0] = f1 * a[0] + f2 * b[0];
                    q[1] = f1 * a[1] + f2 * b[1];
                    q[2] = f1 * a[2] + f2 * b[2];
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

_AFXINL void AfxM2dMix(afxM2d m, afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
}

_AFXINL void AfxM3dMix(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
}

_AFXINL void AfxM4dMix(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = a[0][3] * (1.f - t) + b[0][3] * t;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = a[1][3] * (1.f - t) + b[1][3] * t;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = a[2][3] * (1.f - t) + b[2][3] * t;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
    m[3][3] = a[3][3] * (1.f - t) + b[3][3] * t;
}

_AFXINL void AfxM3dMixAtm(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dMixAtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = 0.f;

    m[3][0] = a[3][0] * (1.f - t) + b[3][0] * t;
    m[3][1] = a[3][1] * (1.f - t) + b[3][1] * t;
    m[3][2] = a[3][2] * (1.f - t) + b[3][2] * t;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMixLtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] * (1.f - t) + b[0][0] * t;
    m[0][1] = a[0][1] * (1.f - t) + b[0][1] * t;
    m[0][2] = a[0][2] * (1.f - t) + b[0][2] * t;
    m[0][3] = 0.f;

    m[1][0] = a[1][0] * (1.f - t) + b[1][0] * t;
    m[1][1] = a[1][1] * (1.f - t) + b[1][1] * t;
    m[1][2] = a[1][2] * (1.f - t) + b[1][2] * t;
    m[1][3] = 0.f;

    m[2][0] = a[2][0] * (1.f - t) + b[2][0] * t;
    m[2][1] = a[2][1] * (1.f - t) + b[2][1] * t;
    m[2][2] = a[2][2] * (1.f - t) + b[2][2] * t;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

_AFXINL void AfxM2dLerp(afxM2d m, afxM2d const a, afxM2d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
}

_AFXINL void AfxM3dLerp(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
}

_AFXINL void AfxM4dLerp(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = a[0][3] + t * (b[0][3] - a[0][3]);

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = a[1][3] + t * (b[1][3] - a[1][3]);

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = a[2][3] + t * (b[2][3] - a[2][3]);

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
    m[3][3] = a[3][3] + t * (b[3][3] - a[3][3]);
}

_AFXINL void AfxM3dLerpAtm(afxM3d m, afxM3d const a, afxM3d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dLerpAtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = 0.f;

    m[3][0] = a[3][0] + t * (b[3][0] - a[3][0]);
    m[3][1] = a[3][1] + t * (b[3][1] - a[3][1]);
    m[3][2] = a[3][2] + t * (b[3][2] - a[3][2]);
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dLerpLtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, a, b);

    m[0][0] = a[0][0] + t * (b[0][0] - a[0][0]);
    m[0][1] = a[0][1] + t * (b[0][1] - a[0][1]);
    m[0][2] = a[0][2] + t * (b[0][2] - a[0][2]);
    m[0][3] = 0.f;

    m[1][0] = a[1][0] + t * (b[1][0] - a[1][0]);
    m[1][1] = a[1][1] + t * (b[1][1] - a[1][1]);
    m[1][2] = a[1][2] + t * (b[1][2] - a[1][2]);
    m[1][3] = 0.f;

    m[2][0] = a[2][0] + t * (b[2][0] - a[2][0]);
    m[2][1] = a[2][1] + t * (b[2][1] - a[2][1]);
    m[2][2] = a[2][2] + t * (b[2][2] - a[2][2]);
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}
