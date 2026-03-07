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
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxV3dRotate(afxV3d v, afxV3d const in, afxQuat const q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT(v);
    AFX_ASSERT(q != in);
    AFX_ASSERT(q != v);

    // Transform a vector using a rotation expressed as a unit quaternion

#if 0
    // Compatible with out = XMVector3Rotate(in, q)

    afxQuat b, c;
    afxQuat a = { in[0], in[1], in[2], 0 };
    AfxQuatConj(b, q);
    AfxQuatMultiply(c, b, a);
    afxQuat qv;
    AfxQuatMultiply(qv, c, q);
    AfxV3dCopy(v, qv);
#else

    // Compatible with NormalQuaternionTransform3(in/out, q)

    afxReal v2 = q[0] * q[0];
    afxReal v3 = q[1] * q[1];
    afxReal v4 = q[2] * q[2];
    afxReal v5 = q[1] * q[0];
    afxReal ac = q[2] * q[0];
    afxReal ad = q[3] * q[0];
    afxReal bc = q[2] * q[1];
    afxReal bd = q[1] * q[3];
    afxReal v10 = q[2] * q[3];
    afxReal v12 = in[0];
    afxReal v13 = in[2];
    afxReal z2 = in[1];
    afxReal x2 = v12 + v12;
    afxReal y2 = z2 + z2;
    afxReal Vector3a = v13 + v13;
    v[0] = (1.0 - (v4 + v3 + v4 + v3)) * v12 + (v5 - v10) * y2 + (bd + ac) * Vector3a;
    v[1] = (1.0 - (v4 + v2 + v4 + v2)) * z2 + (bc - ad) * Vector3a + (v10 + v5) * x2;
    v[2] = (1.0 - (v3 + v2 + v3 + v2)) * v13 + (ac - bd) * x2 + (bc + ad) * y2;
#endif
}

_AFXINL void AfxV3dRotateInv(afxV3d v, afxV3d const in, afxQuat const q)
{
    afxReal const vx = 2.0 * in[0];
    afxReal const vy = 2.0 * in[1];
    afxReal const vz = 2.0 * in[2];
    afxReal const w2 = q[3] * q[3] - 0.5;
    afxReal const dot2 = (q[0] * vx + q[1] * vy + q[2] * vz);
    AfxV3dSet(v, (vx * w2 - (q[1] * vz - q[2] * vy) * q[3] + q[0] * dot2),
        (vy * w2 - (q[2] * vx - q[0] * vz) * q[3] + q[1] * dot2),
        (vz * w2 - (q[0] * vy - q[1] * vx) * q[3] + q[2] * dot2));
}

// Assimilate

_AFXINL void AfxAssimilateAtv3d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
    }
}

_AFXINL void AfxAssimilateAtv4d(afxM3d const ltm, afxV4d const atv, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    // inspired on AfxAssimilateAtv3d
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyLtv4d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dAdd(out[i], atv, out[i]);
        out[i][3] = 1.f;
    }
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatRotationM3d(afxQuat q, afxM3d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);

            q[0] = (m[0][1] + m[1][0]) * inv4y;
            q[1] = fourYSqr * inv4y;
            q[2] = (m[1][2] + m[2][1]) * inv4y;
            q[3] = (m[2][0] - m[0][2]) * inv4y;
        }
    }
    else  // z^2 + w^2 >= x^2 + y^2
    {
        afxReal sum10 = m[1][1] + m[0][0];
        afxReal opr22 = 1.f + r22;

        if (sum10 <= 0.f)  // z^2 >= w^2
        {
            afxReal fourZSqr = opr22 - sum10;
            afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatRotationM4d(afxQuat q, afxM4d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(q, m);

    afxReal r22 = m[2][2];

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m[1][1] - m[0][0];
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);

            q[0] = fourXSqr * inv4x;
            q[1] = (m[0][1] + m[1][0]) * inv4x;
            q[2] = (m[0][2] + m[2][0]) * inv4x;
            q[3] = (m[1][2] - m[2][1]) * inv4x;
        }
        else  // y^2 >= x^2
        {
            afxReal fourYSqr = omr22 + dif10;
            afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);

            q[0] = (m[0][1] + m[1][0]) * inv4y;
            q[1] = fourYSqr * inv4y;
            q[2] = (m[1][2] + m[2][1]) * inv4y;
            q[3] = (m[2][0] - m[0][2]) * inv4y;
        }
    }
    else  // z^2 + w^2 >= x^2 + y^2
    {
        afxReal sum10 = m[1][1] + m[0][0];
        afxReal opr22 = 1.f + r22;

        if (sum10 <= 0.f)  // z^2 >= w^2
        {
            afxReal fourZSqr = opr22 - sum10;
            afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);

            q[0] = (m[0][2] + m[2][0]) * inv4z;
            q[1] = (m[1][2] + m[2][1]) * inv4z;
            q[2] = fourZSqr * inv4z;
            q[3] = (m[0][1] - m[1][0]) * inv4z;
        }
        else  // w^2 >= z^2
        {
            afxReal fourWSqr = opr22 + sum10;
            afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);

            q[0] = (m[1][2] - m[2][1]) * inv4w;
            q[1] = (m[2][0] - m[0][2]) * inv4w;
            q[2] = (m[0][1] - m[1][0]) * inv4w;
            q[3] = fourWSqr * inv4w;
        }
    }
}

_AFXINL void AfxQuatRotationAxial(afxQuat q, afxV3d const axis, afxReal phi)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(axis); // radians

    afxReal s, c;
    AfxCosSinf(phi * AFX_REAL(0.5), &c, &s);
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = c;
    AfxQuatNormalize(q, q); // reduz erros causados por AfxSinf() e AfxCosf().
}

_AFXINL void AfxQuatRotationEuler(afxQuat q, afxV3d const pitchYawRoll)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationRollPitchYawFromVector(FXMVECTOR Angles)
    /*
        WARNING:

        Qwadro assumes pitchYawRoll vector -> [pitch, yaw, roll], which typically means:
            pitch -> rotation around X-axis (elevation)
            yaw -> rotation around Y-axis (azimuth)
            roll -> rotation around Z-axis

        This is the standard aeronautical convention, and it matches Yaw-Pitch-Roll rotation order if applied as:
        Q = Q_roll * Q_pitch * Q_yaw

        This uses the well-established formula for composing a quaternion from Euler angles in Y-X-Z rotation order, assuming:
            First rotate about Y (azimuth)
            Then rotate about X (elevation)
            Then rotate about Z (roll)
        Which is exactly what works for a spherical camera system with orientation driven by azimuth, elevation, and roll.

        But DirectXMath (XMQuaternionRotationRollPitchYawFromVector) assumes a Roll-Pitch-Yaw (Z-X-Y) vector,
        and parameters are passed as:
        XMQuaternionRotationRollPitchYaw(roll, pitch, yaw)
        So if you're feeding [pitch, yaw, roll] into a DXMath function that expects [roll, pitch, yaw],
        you'll get completely wrong orientations.


    */

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(pitchYawRoll);

    afxReal const halfpitch = pitchYawRoll[0] * 0.5f; // rotation around X
    afxReal cp = AfxCosf(halfpitch);
    afxReal sp = AfxSinf(halfpitch);
    afxReal const halfyaw = pitchYawRoll[1] * 0.5f; // rotation around Y
    afxReal cy = AfxCosf(halfyaw);
    afxReal sy = AfxSinf(halfyaw);
    afxReal const halfroll = pitchYawRoll[2] * 0.5f; // rotation around Z
    afxReal cr = AfxCosf(halfroll);
    afxReal sr = AfxSinf(halfroll);

#if 0 // DXMath wrong way
    q[0] = cr * sp * cy + sr * cp * sy;
    q[1] = cr * cp * sy - sr * sp * cy;
    q[2] = sr * cp * cy - cr * sp * sy;
    q[3] = cr * cp * cy + sr * sp * sy;
#endif
    // Quaternion composition: Q = Q_yaw * Q_pitch * Q_roll
    q[0] = sp * cy * cr + cp * sy * sr; // x
    q[1] = cp * sy * cr - sp * cy * sr; // y
    q[2] = cp * cy * sr - sp * sy * cr; // z
    q[3] = cp * cy * cr + sp * sy * sr; // w
}

_AFXINL void AfxAssimilateQuat(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxQuat const in[], afxQuat out[])
{
    // Should be compatible with void InPlaceSimilarityTransformOrientation

    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal len = AfxQuatMag(in[i]);
        AfxQuatScale(out[i], in[i], 1.f / len);

        afxM3d rm, tmp;
        AfxM3dRotationQuat(rm, out[i]);
        AfxM3dMultiply(tmp, ltm, rm);
        AfxM3dMultiply(rm, tmp, iltm);
        AfxQuatRotationM3d(out[i], rm);

        AfxQuatScale(out[i], out[i], len);
    }
}

_AFXINL void AfxQuatExtractAxialRotation(afxQuat const q, afxV3d axis, afxReal *radians)
{
    // Should be compatible with void XMQuaternionToAxisAngle(XMVECTOR* pAxis, float* pAngle, FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(axis);
    AfxV3dCopy(axis, q);
    *radians = 2.f * AfxAcosf(q[3]);
}

_AFXINL void AfxQuatLookTo(afxQuat q, afxV3d const from, afxV3d const to)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/03/quaternion-tricks.html

    afxV3d h;
    AfxV3dAdd(h, from, to);
    AfxV3dNormalize(h, h);

    q[3] = AfxV3dDot(from, h);
    q[0] = from[1] * h[2] - from[2] * h[1];
    q[1] = from[2] * h[0] - from[0] * h[2];
    q[2] = from[0] * h[1] - from[1] * h[0];
}

_AFXINL void AfxQuatFromAngularVelocity(afxQuat q, afxV3d const vel)
{
    afxError err = { 0 };
    AFX_ASSERT(vel);
    AFX_ASSERT(q);

    // Compatible with AngularVelocityToQuaternion(q, rot)

    afxV4d n;
    afxReal mag = AfxV3dMag(vel);
    AfxV3dScale(n, vel, 1.0 / mag);
    afxReal halfSq = mag * 0.5f;
    AfxV3dScale(q, n, AfxSinf(halfSq));
    q[3] = AfxCosf(halfSq);
}

/*
    The AfxQuatIntegrate function uses exponential map to compute deltaq = exp(0.5 w dt).
    This is a closed-form integration.The code computes rotation magnitude and forms a quaternion exponential.
    This is the exact solution of the quaternion ODE for constant angular velocity during dt.

    Pros:
    Much more accurate
    Stable even for large dt
    No need to normalize each step

    Cons:
    Slightly slower
    More code

    This function should be prefered over EULER variant when:
        dt is moderate or irregular
        angular velocities are high
        you want numerically stable integration (e.g., rigid-body sims, spacecraft attitude)
        you want physically correct results regardless of step size
*/

_AFXINL void AfxQuatIntegrate(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT(omega);

    afxV3d theta;
    AfxV3dScale(theta, omega, dt * 0.5f);
    afxReal thetaMagSq = AfxV3dSq(theta);
    afxReal w, s;

    if (thetaMagSq * thetaMagSq / 24.f < AFX_EPSILON)
    {
        // small-angle Taylor series
        w = 1.0f - thetaMagSq * 0.5f;
        s = 1.0f - thetaMagSq / 6.0f;
    }
    else
    {
        afxReal thetaMag = AfxSqrt(thetaMagSq);
        w = AfxCos(thetaMag);
        s = AfxSinf(thetaMag) / thetaMag;
    }

    afxQuat deltaQ;
    // delta quaternion (rotation over this timestep)
    AfxV3dScale(deltaQ, theta, s);
    deltaQ[3] = w;

    // Integrate.
    AfxQuatMultiply(q, deltaQ, in);
}

/*
    The AfxQuatIntegrateEULER function for first-order (explicit Euler) quaternion integration.
    This uses the quaternion differential equation and integrates it using explicit Euler.
    This is the simplest possible method.

    Pros: fast, small code
    Cons: low accuracy for large rotations or large dt,
    not stable for high angular velocities (requires normalization).

    This method is accurate only when: |w|dt<<1

    This function is mathematically equivalent to AfxQuatIntegrate using exponential map for small dt.

    This function should be prefered over EXP-MAP variant when:
        dt is very small (e.g., physics running at 1000 Hz)
        angular velocities are small
        you want maximum speed
        some accuracy loss is acceptable
*/

_AFXINL void AfxQuatIntegrateEULER(afxQuat q, afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT(omega);

    afxQuat wq;
    AfxQuatSet(wq, omega[0], omega[1], omega[2], 0);
    // dq = 0.5 * (wq * q)
    afxQuat dq;
    AfxQuatMultiply(dq, wq, q);
    AfxQuatScale(dq, dq, 0.5);

    // integrate
    AfxV4dMads(q, q, dq, dt);

    // renormalize
    AfxQuatNormalize(q, q);
}

// BATCH OPS FOR QUATERNION

_AFXINL void AfxQuatRotateV3d(afxQuat const q, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);
    AFX_ASSERT(q != in[0]);
    AFX_ASSERT(q != out[0]);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dRotate(out[i], in[i], q);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// MakeScaling

_AFXINL void AfxM3dScaling(afxM3d m, afxV3d const scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT_DIFF(m, scale);
    AfxM3dDiagonal(m, scale[0], scale[1], scale[2]);
}

_AFXINL void AfxM4dScaling(afxM4d m, afxV3d const scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT_DIFF(m, scale);
    AfxM4dDiagonal(m, scale[0], scale[1], scale[2], 1.0);
}

// MakeRotation

_AFXINL void AfxM3dRotationQuat(afxM3d m, afxQuat const q)
{
    // Inspired in AfxM4dRotationQuat

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(q);
    AFX_ASSERT_DIFF(m, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    m[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    m[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    m[0][2] = 2.f * qx * qz - 2.f * qy * qw;

    m[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    m[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    m[1][2] = 2.f * qy * qz + 2.f * qx * qw;

    m[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    m[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    m[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
}

_AFXINL void AfxM4dRotationQuat(afxM4d m, afxQuat const q)
{
    // Should be compatible with XMMATRIX XMMatrixRotationQuaternion(FXMVECTOR Quaternion)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(q);
    AFX_ASSERT_DIFF(m, q);

    afxReal qx = q[0], qxx = qx * qx;
    afxReal qy = q[1], qyy = qy * qy;
    afxReal qz = q[2], qzz = qz * qz;
    afxReal qw = q[3];

    m[0][0] = 1.f - 2.f * qyy - 2.f * qzz;
    m[0][1] = 2.f * qx * qy + 2.f * qz * qw;
    m[0][2] = 2.f * qx * qz - 2.f * qy * qw;
    m[0][3] = 0.f;

    m[1][0] = 2.f * qx * qy - 2.f * qz * qw;
    m[1][1] = 1.f - 2.f * qxx - 2.f * qzz;
    m[1][2] = 2.f * qy * qz + 2.f * qx * qw;
    m[1][3] = 0.f;

    m[2][0] = 2.f * qx * qz + 2.f * qy * qw;
    m[2][1] = 2.f * qy * qz - 2.f * qx * qw;
    m[2][2] = 1.f - 2.f * qxx - 2.f * qyy;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.0f;
}

// MakeRotation_Euler
//

_AFXINL void AfxM4dRotationEuler(afxM4d m, afxReal pitch, afxReal yaw, afxReal roll)
{
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = { 0 };
    AFX_ASSERT(m);

    /*
        Angles are measured clockwise when looking along the rotation axis toward the origin. 
        This is a left-handed coordinate system. To use right-handed coordinates, negate all three angles.
    */
    pitch = -pitch;
    yaw = -yaw;
    roll = -roll;

    afxReal pc, ps, yc, ys, rc, rs;
    AfxCosSinf(pitch, &pc, &ps);
    AfxCosSinf(yaw, &yc, &ys);
    AfxCosSinf(roll, &rc, &rs);

    m[0][0] = rc * yc + rs * ps * ys;
    m[0][1] = rs * pc;
    m[0][2] = rs * ps * yc - rc * ys;
    m[0][3] = 0.f;

    m[1][0] = rc * ps * ys - rs * yc;
    m[1][1] = rc * pc;
    m[1][2] = rs * ys + rc * ps * yc;
    m[1][3] = 0.f;

    m[2][0] = pc * ys;
    m[2][1] =-ps;
    m[2][2] = pc * yc;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

// uncertain origin

// MakeRotation_Axial
// Memory layout: looks like it is row-major

_AFXINL void AfxM3dRotationAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(axis);
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis2, axis);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosTheta;
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * sinTheta);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * sinTheta);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * sinTheta);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosTheta;
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * sinTheta);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * sinTheta);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * sinTheta);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosTheta;
}

_AFXINL void AfxM4dRotationAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(axis);
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    AfxV3dNormalize(axis2, axis);

    m[0][0] = (axis2[0] * axis2[0] * cosValue) + cosTheta;
    m[0][1] = (axis2[0] * axis2[1] * cosValue) + (axis2[2] * sinTheta);
    m[0][2] = (axis2[0] * axis2[2] * cosValue) - (axis2[1] * sinTheta);
    m[0][3] = AFX_REAL(0);

    m[1][0] = (axis2[1] * axis2[0] * cosValue) - (axis2[2] * sinTheta);
    m[1][1] = (axis2[1] * axis2[1] * cosValue) + cosTheta;
    m[1][2] = (axis2[1] * axis2[2] * cosValue) + (axis2[0] * sinTheta);
    m[1][3] = AFX_REAL(0);

    m[2][0] = (axis2[2] * axis2[0] * cosValue) + (axis2[1] * sinTheta);
    m[2][1] = (axis2[2] * axis2[1] * cosValue) - (axis2[0] * sinTheta);
    m[2][2] = (axis2[2] * axis2[2] * cosValue) + cosTheta;
    m[2][3] = AFX_REAL(0);

    m[3][0] = AFX_REAL(0);
    m[3][1] = AFX_REAL(0);
    m[3][2] = AFX_REAL(0);
    m[3][3] = AFX_REAL(1);
}

_AFXINL void AfxM4dRotationX(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationX(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[1][1] = co;
    m[1][2] = si;
    m[2][1] = -si;
    m[2][2] = co;
}

_AFXINL void AfxM4dRotationY(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationY(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[0][0] = co;
    m[0][2] = -si;
    m[2][0] = si;
    m[2][2] = co;
}

_AFXINL void AfxM4dRotationZ(afxM4d m, afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationZ(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    AfxM4dReset(m);
    m[0][0] = co;
    m[0][1] = si;
    m[1][0] = -si;
    m[1][1] = co;
}

// Translate

_AFXINL void AfxM4dTranslation(afxM4d m, afxV3d const translation)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(translation);
    AfxM4dReset(m);
    AfxV3dCopy(m[3], translation);    
}

// Decompose

_AFXINL afxBool AfxM3dPolarDecompose(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm)
{
    // Should be compatible with char PolarDecompose(const float *Source3x3Init, float Tolerance, float *QInit, float *SInit)

    afxM3d ZMirror;
    AfxM3dReset(ZMirror);
    ZMirror[2][2] = -1.0;

    // 0 1 2
    // 3 4 5
    // 6 7 8

    if (AfxM3dDet(m) < 0.0)
    {
        AfxM3dMultiply(rm, m, ZMirror);
    }
    else
    {
        AfxM3dCopy(rm, m);
    }

    afxUnit i = 0;
    afxResult rslt = TRUE;

    while (1)
    {
        AfxM3dInvert(ssm, rm);
        AfxM3dAdd(ssm, ssm, rm);

        AfxV3dScale(ssm[0], ssm[0], 0.5);
        AfxV3dScale(ssm[1], ssm[1], 0.5);
        AfxV3dScale(ssm[2], ssm[2], 0.5);

        afxM3d ssMinR;
        AfxM3dSub(ssMinR, ssm, rm);

        AfxM3dCopy(rm, ssm);

        if (fabs(ssMinR[0][0]) + fabs(ssMinR[0][1]) + fabs(ssMinR[0][2]) + fabs(ssMinR[1][0]) + fabs(ssMinR[1][1]) + fabs(ssMinR[1][2]) + fabs(ssMinR[2][0]) + fabs(ssMinR[2][1]) + fabs(ssMinR[2][2]) < tol)
            break;

        if (++i >= 1000)
        {
            rslt = FALSE;
            break;
        }
    }
    TransposeMatrixMultiply3x3(rm, ssm, m);
    return rslt;
}

// Compose

_AFXINL void AfxM4dComposeTransformation(afxM4d m, afxV3d const scalPivot, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotPivot, afxQuat const rotQuat, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixTransformation(FXMVECTOR ScalingOrigin, FXMVECTOR ScalingOrientationQuaternion, FXMVECTOR Scaling, GXMVECTOR RotationOrigin, HXMVECTOR RotationQuaternion, HXMVECTOR Translation)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scalPivot);
    AFX_ASSERT(scalOrient);
    AFX_ASSERT(scaling);
    AFX_ASSERT(rotPivot);
    AFX_ASSERT(rotQuat);
    AFX_ASSERT(translation);

    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxV3d negScaOrigin;
    AfxV3dNeg(negScaOrigin, scalPivot);

    afxM4d scalOriginI, scalOrientM, scalOrientT, scalM, rotM;
    AfxM4dReset(scalOriginI);
    AfxV3dCopy(scalOriginI[3], negScaOrigin);
    AfxM4dRotationQuat(scalOrientM, scalOrient);
    AfxM4dCopyTransposed(scalOrientT, scalOrientM);
    AfxM4dReset(scalM);
    AfxM4dScaling(scalM, scaling);
    AfxM4dRotationQuat(rotM, rotQuat);

    afxM4d t;
    AfxM4dMultiply(t, scalOriginI, scalOrientT);
    AfxM4dMultiply(m, t, scalM);
    AfxM4dMultiply(t, m, scalOrientM);
    AfxV3dAdd(t[3], t[3], scalPivot);
    AfxV3dSub(t[3], t[3], rotPivot);
    AfxM4dMultiply(m, t, rotM);
    AfxV3dAdd(m[3], m[3], rotPivot);
    AfxV3dAdd(m[3], m[3], translation);
}

_AFXINL void AfxM4dComposeAffineTransformation(afxM4d m, afxV3d const scale, afxV3d const rotPivot, afxQuat const rot, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)

    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(scale);
    AFX_ASSERT(rot);
    AFX_ASSERT(translation);

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d sm, rm, c;
    AfxM4dReset(sm);
    AfxM4dScaling(sm, scale);
    AfxM4dRotationQuat(rm, rot);
    AfxV3dSub(sm[3], sm[3], rotPivot);
    AfxM4dMultiply(m, sm, rm);
    AfxV3dAdd(m[3], m[3], rotPivot);
    AfxV3dAdd(m[3], m[3], translation);
}

// Assimilate

_AFXINL void AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxM3d const in[], afxM3d out[])
{
    // Should be compatible with void InPlaceSimilarityTransformScaleShear
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d tmp;
        AfxM3dMultiply(tmp, ltm, in[i]);
        AfxM3dMultiply(out[i], tmp, iltm);
    }
}

_AFXINL void AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxM4d const in[], afxM4d out[])
{
    // Should be compatible with void InPlaceSimilarityTransform4x3
    afxError err = { 0 };
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d ss;
        AfxM3dCopyM4d(ss, in[i]);
        afxV4d pos;
        AfxAssimilateAtv4d(ltm, atv, 1, &in[i][3], &pos);
        AfxAssimilateLtm3d(ltm, iltm, 1, &ss, &ss);
        AfxM4dCopyM3d(out[i], ss, pos);
    }
}

_AFXINL void AfxM4dRigidMotion(afxM4d m, afxM4d const mm, afxV3d const rotation, afxV3d const translation)
{
    // AfxApplyRootMotionVectorsToMatrix

    /*
        It composes and applies a transformation matrix based on root motion (translation and rotation), 
        adjusting the provided model matrix (mm) to produce a new transformed matrix (m). 
        This is essential in animation systems when root bone motion needs to be applied to a character's world transform.
    */

    afxError err = { 0 };
    AFX_ASSERT(translation);
    AFX_ASSERT(rotation);
    AFX_ASSERT(mm); // model matrix
    AFX_ASSERT(m);
    afxM4d upd, tmp;
    afxReal rl = AfxV3dMag(rotation);

    if (rl <= 0.0)
    {
        AfxM4dTranslation(upd, translation);
        AfxM4dCopy(tmp, mm);
        AfxM4dMultiplyAtm(m, upd, tmp);
    }
    else
    {
        afxV3d scaledRot;
        AfxV3dScale(scaledRot, rotation, 1.f / rl);
        AfxM4dRotationAxis(tmp, scaledRot, rl);
        AfxM4dCopyTransposed(upd, tmp);
        AfxV3dCopy(upd[3], translation);
        AfxM4dCopy(tmp, mm);
        AfxM4dMultiplyAtm(m, upd, tmp);
    }
}
