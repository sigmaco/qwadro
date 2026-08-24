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
#include "qwadro/coll/afxPlane.h"
#include "qwadro/afxStream.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"
#include "qwadro/math/afxTrigonometry.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxV3d AfxV3dRotate(afxV3d const in, afxQuat const q)
{
    afxError err = { 0 };

    // Transform a vector using a rotation expressed as a unit quaternion

#if 0
    // Compatible with out = XMVector3Rotate(in, q)

    afxQuat b, c;
    afxQuat a = { in.x, in.y, in.z, 0 };
    AfxQuatConj(b, q);
    AfxQuatMultiply(c, b, a);
    afxQuat qv;
    AfxQuatMultiply(qv, c, q);
    AfxV3dCopy(v, qv);
#else

    // Compatible with NormalQuaternionTransform3(in/out, q)

    afxReal v2 = q.x * q.x;
    afxReal v3 = q.y * q.y;
    afxReal v4 = q.z * q.z;
    afxReal v5 = q.y * q.x;
    afxReal ac = q.z * q.x;
    afxReal ad = q.w * q.x;
    afxReal bc = q.z * q.y;
    afxReal bd = q.y * q.w;
    afxReal v10 = q.z * q.w;
    afxReal v12 = in.x;
    afxReal v13 = in.z;
    afxReal z2 = in.y;
    afxReal x2 = v12 + v12;
    afxReal y2 = z2 + z2;
    afxReal Vector3a = v13 + v13;
    return AFX_V3D( (1.0 - (v4 + v3 + v4 + v3)) * v12 + (v5 - v10) * y2 + (bd + ac) * Vector3a,
                    (1.0 - (v4 + v2 + v4 + v2)) * z2 + (bc - ad) * Vector3a + (v10 + v5) * x2,
                    (1.0 - (v3 + v2 + v3 + v2)) * v13 + (ac - bd) * x2 + (bc + ad) * y2);
#endif
}

_AFXINL afxV3d AfxV3dRotateInv(afxV3d const in, afxQuat const q)
{
    afxReal const vx = 2.0 * in.x;
    afxReal const vy = 2.0 * in.y;
    afxReal const vz = 2.0 * in.z;
    afxReal const w2 = q.w * q.w - 0.5;
    afxReal const dot2 = (q.x * vx + q.y * vy + q.z * vz);
    AFX_V3D(    (vx * w2 - (q.y * vz - q.z * vy) * q.w + q.x * dot2),
                (vy * w2 - (q.z * vx - q.x * vz) * q.w + q.y * dot2),
                (vz * w2 - (q.x * vy - q.y * vx) * q.w + q.z * dot2));
}

// Assimilate

_AFXINL void AfxAssimilateAtv3d(afxM3d const ltm, afxV3d const atv, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        out[i] = AfxV3dAdd(atv, out[i]);
    }
}

_AFXINL void AfxAssimilateAtv4d(afxM3d const ltm, afxV3d const atv, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    // inspired on AfxAssimilateAtv3d
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyLtv4d(ltm, cnt, in, out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        out[i].v3 = AfxV3dAdd(atv, out[i].v3);
        out[i].w = 1.f;
    }
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatRotationM3d(afxM3d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };

    afxReal r22 = m.z.z;

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m.y.y - m.x.x;
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);
            return AFX_QUAT(fourXSqr * inv4x,
                            (m.x.y + m.y.x) * inv4x,
                            (m.x.z + m.z.x) * inv4x,
                            (m.y.z - m.z.y) * inv4x);
        }
        
        // y^2 >= x^2
        afxReal fourYSqr = omr22 + dif10;
        afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);
        return AFX_QUAT((m.x.y + m.y.x) * inv4y,
                        fourYSqr * inv4y,
                        (m.y.z + m.z.y) * inv4y,
                        (m.z.x - m.x.z) * inv4y);
    }
    
    // z^2 + w^2 >= x^2 + y^2
    afxReal sum10 = m.y.y + m.x.x;
    afxReal opr22 = 1.f + r22;

    if (sum10 <= 0.f)  // z^2 >= w^2
    {
        afxReal fourZSqr = opr22 - sum10;
        afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);
        return AFX_QUAT((m.x.z + m.z.x) * inv4z,
                        (m.y.z + m.z.y) * inv4z,
                        fourZSqr * inv4z,
                        (m.x.y - m.y.x) * inv4z);
    }
    
    // w^2 >= z^2
    afxReal fourWSqr = opr22 + sum10;
    afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);
    return AFX_QUAT((m.y.z - m.z.y) * inv4w,
                    (m.z.x - m.x.z) * inv4w,
                    (m.x.y - m.y.x) * inv4w,
                    fourWSqr * inv4w);
}

_AFXINL afxQuat AfxQuatRotationM4d(afxM4d const m)
{
    // Should be compatible with XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)

    afxError err = { 0 };

    afxReal r22 = m.z.z;

    if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
    {
        afxReal dif10 = m.y.y - m.x.x;
        afxReal omr22 = 1.f - r22;

        if (dif10 <= 0.f)  // x^2 >= y^2
        {
            afxReal fourXSqr = omr22 - dif10;
            afxReal inv4x = 0.5f / AfxSqrtf(fourXSqr);
            return AFX_QUAT(fourXSqr * inv4x,
                            (m.x.y + m.y.x) * inv4x,
                            (m.x.z + m.z.x) * inv4x,
                            (m.y.z - m.z.y) * inv4x);
        }

        // y^2 >= x^2
        afxReal fourYSqr = omr22 + dif10;
        afxReal inv4y = 0.5f / AfxSqrtf(fourYSqr);
        return AFX_QUAT((m.x.y + m.y.x) * inv4y,
                        fourYSqr * inv4y,
                        (m.y.z + m.z.y) * inv4y,
                        (m.z.x - m.x.z) * inv4y);
    }
    
    // z^2 + w^2 >= x^2 + y^2
    afxReal sum10 = m.y.y + m.x.x;
    afxReal opr22 = 1.f + r22;

    if (sum10 <= 0.f)  // z^2 >= w^2
    {
        afxReal fourZSqr = opr22 - sum10;
        afxReal inv4z = 0.5f / AfxSqrtf(fourZSqr);
        return AFX_QUAT((m.x.z + m.z.x) * inv4z,
                        (m.y.z + m.z.y) * inv4z,
                        fourZSqr * inv4z,
                        (m.x.y - m.y.x) * inv4z);
    }

    // w^2 >= z^2
    afxReal fourWSqr = opr22 + sum10;
    afxReal inv4w = 0.5f / AfxSqrtf(fourWSqr);
    return AFX_QUAT((m.y.z - m.z.y) * inv4w,
                    (m.z.x - m.x.z) * inv4w,
                    (m.x.y - m.y.x) * inv4w,
                    fourWSqr * inv4w);
}

_AFXINL afxQuat AfxQuatRotationAxial(afxV3d const axis, afxReal phi)
{
    afxError err = { 0 };
    //AFX_ASSERT(axis); // radians

    afxReal s, c;
    AfxCosSinf(phi * AFX_REAL(0.5), &c, &s);
    afxQuat q = AFX_QUAT( axis.x * s,
                        axis.y * s,
                        axis.z * s,
                        c);
    return AfxQuatNormalize(q, NIL); // reduz erros causados por AfxSinf() e AfxCosf().
}

_AFXINL afxQuat AfxQuatRotationEuler(afxV3d const pitchYawRoll)
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

    afxReal const halfpitch = pitchYawRoll.x * 0.5f; // rotation around X
    afxReal cp = AfxCosf(halfpitch);
    afxReal sp = AfxSinf(halfpitch);
    afxReal const halfyaw = pitchYawRoll.y * 0.5f; // rotation around Y
    afxReal cy = AfxCosf(halfyaw);
    afxReal sy = AfxSinf(halfyaw);
    afxReal const halfroll = pitchYawRoll.z * 0.5f; // rotation around Z
    afxReal cr = AfxCosf(halfroll);
    afxReal sr = AfxSinf(halfroll);

#if 0 // DXMath wrong way
    q.x = cr * sp * cy + sr * cp * sy;
    q.y = cr * cp * sy - sr * sp * cy;
    q.z = sr * cp * cy - cr * sp * sy;
    q.w = cr * cp * cy + sr * sp * sy;
#endif
    // Quaternion composition: Q = Q_yaw * Q_pitch * Q_roll
    return AFX_QUAT(sp * cy * cr + cp * sy * sr, // x
                    cp * sy * cr - sp * cy * sr, // y
                    cp * cy * sr - sp * sy * cr, // z
                    cp * cy * cr + sp * sy * sr); // w
}

_AFXINL void AfxAssimilateQuat(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxQuat const in[], afxQuat out[])
{
    // Should be compatible with void InPlaceSimilarityTransformOrientation

    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxQuat qi = in[i];
        afxReal len = AfxQuatMag(qi);
        afxQuat q = AfxQuatScale(qi, 1.f / len);

        afxM3d rm = AfxM3dRotationQuat(q);
        afxM3d tmp = AfxM3dMultiply(ltm, rm);
        rm = AfxM3dMultiply(tmp, iltm);
        q = AfxQuatRotationM3d(rm);
        out[i] = AfxQuatScale(q, len);
    }
}

_AFXINL afxV3d AfxQuatExtractAxialRotation(afxQuat const q, afxReal *radians)
{
    // Should be compatible with void XMQuaternionToAxisAngle(XMVECTOR* pAxis, float* pAngle, FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(radians);
    *radians = 2.f * AfxAcosf(q.w);
    return AfxV3dFromV4d(q);
}

_AFXINL afxQuat AfxQuatLookTo(afxV3d const from, afxV3d const to)
{
    // Should be compatible with physicsforgames.blogspot.com/2010/03/quaternion-tricks.html

    afxV3d h;
    h = AfxV3dAdd(from, to);
    h = AfxV3dNormalize(h, NIL);
    return AFX_QUAT(    from.y * h.z - from.z * h.y,
                        from.z * h.x - from.x * h.z,
                        from.x * h.y - from.y * h.x,
                        AfxV3dDot(from, h));
}

_AFXINL afxQuat AfxQuatFromAngularVelocity(afxV3d const vel)
{
    afxError err = { 0 };
    // Compatible with AngularVelocityToQuaternion(q, rot)

    afxV3d n;
    afxReal mag = AfxV3dMag(vel);
    n = AfxV3dScale(vel, 1.0 / mag);
    afxReal halfSq = mag * 0.5f;
    afxQuat q = AfxV4dFromV3d(AfxV3dScale(n, AfxSinf(halfSq)));
    q.w = AfxCosf(halfSq);
    return q;
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

_AFXINL afxQuat AfxQuatIntegrate(afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };

    afxV3d theta = AfxV3dScale(omega, dt * 0.5f);
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

    // delta quaternion (rotation over this timestep)
    afxQuat deltaQ = AfxV4dFromV3d(AfxV3dScale(theta, s));
    deltaQ.w = w;

    // Integrate.
    return AfxQuatMultiply(deltaQ, in);
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

_AFXINL afxQuat AfxQuatIntegrateEULER(afxQuat const in, afxV3d const omega, afxReal dt)
{
    afxError err = { 0 };

    afxQuat wq = AfxQuatMake(omega.x, omega.y, omega.z, 0);
    // dq = 0.5 * (wq * q)
    afxQuat dq = AfxQuatMultiply(wq, in);
    dq = AfxQuatScale(dq, 0.5);

    // integrate
    afxQuat q = AfxV4dMads(q, dq, dt);

    // renormalize
    return AfxQuatNormalize(q, NIL);
}

// BATCH OPS FOR QUATERNION

_AFXINL void AfxQuatRotateV3d(afxQuat const q, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dRotate(in[i], q);
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// MakeScaling

_AFXINL afxM3d AfxM3dScaling(afxV3d const scale)
{
    afxError err = { 0 };
    return AfxM3dDiagonal(scale.x, scale.y, scale.z);
}

_AFXINL afxM4d AfxM4dScaling(afxV3d const scale)
{
    afxError err = { 0 };
    return AfxM4dDiagonal(scale.x, scale.y, scale.z, 1.0);
}

// MakeRotation

_AFXINL afxM3d AfxM3dRotationQuat(afxQuat const q)
{
    // Inspired in AfxM4dRotationQuat

    afxError err = { 0 };

    afxReal qx = q.x, qxx = qx * qx;
    afxReal qy = q.y, qyy = qy * qy;
    afxReal qz = q.z, qzz = qz * qz;
    afxReal qw = q.w;

    afxM3d m;
    m.x.x = 1.f - 2.f * qyy - 2.f * qzz;
    m.x.y = 2.f * qx * qy + 2.f * qz * qw;
    m.x.z = 2.f * qx * qz - 2.f * qy * qw;

    m.y.x = 2.f * qx * qy - 2.f * qz * qw;
    m.y.y = 1.f - 2.f * qxx - 2.f * qzz;
    m.y.z = 2.f * qy * qz + 2.f * qx * qw;

    m.z.x = 2.f * qx * qz + 2.f * qy * qw;
    m.z.y = 2.f * qy * qz - 2.f * qx * qw;
    m.z.z = 1.f - 2.f * qxx - 2.f * qyy;
    return m;
}

_AFXINL afxM4d AfxM4dRotationQuat(afxQuat const q)
{
    // Should be compatible with XMMATRIX XMMatrixRotationQuaternion(FXMVECTOR Quaternion)

    afxError err = { 0 };

    afxReal qx = q.x, qxx = qx * qx;
    afxReal qy = q.y, qyy = qy * qy;
    afxReal qz = q.z, qzz = qz * qz;
    afxReal qw = q.w;

    afxM4d m;
    m.x.x = 1.f - 2.f * qyy - 2.f * qzz;
    m.x.y = 2.f * qx * qy + 2.f * qz * qw;
    m.x.z = 2.f * qx * qz - 2.f * qy * qw;
    m.x.w = 0.f;

    m.y.x = 2.f * qx * qy - 2.f * qz * qw;
    m.y.y = 1.f - 2.f * qxx - 2.f * qzz;
    m.y.z = 2.f * qy * qz + 2.f * qx * qw;
    m.y.w = 0.f;

    m.z.x = 2.f * qx * qz + 2.f * qy * qw;
    m.z.y = 2.f * qy * qz - 2.f * qx * qw;
    m.z.z = 1.f - 2.f * qxx - 2.f * qyy;
    m.z.w = 0.f;

    m.w.x = 0.f;
    m.w.y = 0.f;
    m.w.z = 0.f;
    m.w.w = 1.0f;
    return m;
}

// MakeRotation_Euler
//

_AFXINL afxM4d AfxM4dRotationEuler(afxReal pitch, afxReal yaw, afxReal roll)
{
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    // Should be compatible with XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)

    afxError err = { 0 };

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

    afxM4d m;
    m.x.x = rc * yc + rs * ps * ys;
    m.x.y = rs * pc;
    m.x.z = rs * ps * yc - rc * ys;
    m.x.w = 0.f;

    m.y.x = rc * ps * ys - rs * yc;
    m.y.y = rc * pc;
    m.y.z = rs * ys + rc * ps * yc;
    m.y.w = 0.f;

    m.z.x = pc * ys;
    m.z.y =-ps;
    m.z.z = pc * yc;
    m.z.w = 0.f;

    m.w.x = 0.f;
    m.w.y = 0.f;
    m.w.z = 0.f;
    m.w.w = 1.f;
    return m;
}

// uncertain origin

// MakeRotation_Axial
// Memory layout: looks like it is row-major

_AFXINL afxM3d AfxM3dRotationAxis(afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    axis2 = AfxV3dNormalize(axis, NIL);

    afxM3d m;
    m.x.x = (axis2.x * axis2.x * cosValue) + cosTheta;
    m.x.y = (axis2.x * axis2.y * cosValue) + (axis2.z * sinTheta);
    m.x.z = (axis2.x * axis2.z * cosValue) - (axis2.y * sinTheta);

    m.y.x = (axis2.y * axis2.x * cosValue) - (axis2.z * sinTheta);
    m.y.y = (axis2.y * axis2.y * cosValue) + cosTheta;
    m.y.z = (axis2.y * axis2.z * cosValue) + (axis2.x * sinTheta);

    m.z.x = (axis2.z * axis2.x * cosValue) + (axis2.y * sinTheta);
    m.z.y = (axis2.z * axis2.y * cosValue) - (axis2.x * sinTheta);
    m.z.z = (axis2.z * axis2.z * cosValue) + cosTheta;
    return m;
}

_AFXINL afxM4d AfxM4dRotationAxis(afxV3d const axis, afxReal /*theta*/radians)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxV3dIsZero(axis));
    AFX_ASSERT(!AfxV3dIsInfinite(axis));

    afxReal cosTheta, sinTheta;
    AfxCosSinf(radians, &cosTheta, &sinTheta);
    afxReal cosValue = 1.0f - cosTheta;
    afxV3d axis2;
    axis2 = AfxV3dNormalize(axis, NIL);

    afxM4d m;
    m.x.x = (axis2.x * axis2.x * cosValue) + cosTheta;
    m.x.y = (axis2.x * axis2.y * cosValue) + (axis2.z * sinTheta);
    m.x.z = (axis2.x * axis2.z * cosValue) - (axis2.y * sinTheta);
    m.x.w = AFX_REAL(0);

    m.y.x = (axis2.y * axis2.x * cosValue) - (axis2.z * sinTheta);
    m.y.y = (axis2.y * axis2.y * cosValue) + cosTheta;
    m.y.z = (axis2.y * axis2.z * cosValue) + (axis2.x * sinTheta);
    m.y.w = AFX_REAL(0);

    m.z.x = (axis2.z * axis2.x * cosValue) + (axis2.y * sinTheta);
    m.z.y = (axis2.z * axis2.y * cosValue) - (axis2.x * sinTheta);
    m.z.z = (axis2.z * axis2.z * cosValue) + cosTheta;
    m.z.w = AFX_REAL(0);

    m.w.x = AFX_REAL(0);
    m.w.y = AFX_REAL(0);
    m.w.z = AFX_REAL(0);
    m.w.w = AFX_REAL(1);
    return m;
}

_AFXINL afxM4d AfxM4dRotationX(afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationX(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    
    afxM4d m = AFX_M4D_IDENTITY;
    m.y.y = co;
    m.y.z = si;
    m.z.y = -si;
    m.z.z = co;
    return m;
}

_AFXINL afxM4d AfxM4dRotationY(afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationY(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);
    

    afxM4d m = AFX_M4D_IDENTITY;
    m.x.x = co;
    m.x.z = -si;
    m.z.x = si;
    m.z.z = co;
    return m;
}

_AFXINL afxM4d AfxM4dRotationZ(afxReal angle)
{
    // Should be compatible with XMMATRIX XMMatrixRotationZ(float Angle)

    afxReal co, si;
    AfxCosSinf(angle, &co, &si);

    afxM4d m = AFX_M4D_IDENTITY;
    m.x.x = co;
    m.x.y = si;
    m.y.x = -si;
    m.y.y = co;
    return m;
}

// Translate

_AFXINL afxM4d AfxM4dTranslation(afxV3d const translation)
{
    afxError err = { 0 };

    afxM4d m = AFX_M4D_IDENTITY;
    m.w.x = translation.x;
    m.w.y = translation.y;
    m.w.z = translation.z;
    return m;
}

// Decompose

_AFXINL afxBool AfxM3dPolarDecompose(afxM3d const m, afxReal tol, afxM3d* rm, afxM3d* ssm)
{
    // Should be compatible with char PolarDecompose(const float *Source3x3Init, float Tolerance, float *QInit, float *SInit)

    afxM3d ZMirror = AfxM3dIdentity();
    ZMirror.z.z = -1.0;

    // 0 1 2
    // 3 4 5
    // 6 7 8

    afxM3d rm2;

    if (AfxM3dDet(m) < 0.0)
    {
        rm2 = AfxM3dMultiply(m, ZMirror);
    }
    else
    {
        rm2 = m;
    }

    afxUnit i = 0;
    afxResult rslt = TRUE;

    afxM3d ssm2;

    while (1)
    {
        ssm2 = AfxM3dInvert(rm2, NIL);
        ssm2 = AfxM3dAdd(ssm2, rm2);

        ssm2.x = AfxV3dScale(ssm2.x, 0.5);
        ssm2.y = AfxV3dScale(ssm2.y, 0.5);
        ssm2.z = AfxV3dScale(ssm2.z, 0.5);

        afxM3d ssMinR = AfxM3dSub(ssm2, rm2);

        rm2 = ssm2;

        if (fabs(ssMinR.x.x) + fabs(ssMinR.x.y) + fabs(ssMinR.x.z) + 
            fabs(ssMinR.y.x) + fabs(ssMinR.y.y) + fabs(ssMinR.y.z) + 
            fabs(ssMinR.z.x) + fabs(ssMinR.z.y) + fabs(ssMinR.z.z) < tol)
            break;

        if (++i >= 1000)
        {
            rslt = FALSE;
            break;
        }
    }
    rm2 = TransposeMatrixMultiply3x3(rm2, ssm2, m);
    *rm = rm2;
    *ssm = ssm2;
    return rslt;
}

// Compose

_AFXINL afxM4d AfxM4dComposeTransformation(afxV3d const scalPivot, afxQuat const scalOrient, afxV3d const scaling, afxV3d const rotPivot, afxQuat const rotQuat, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixTransformation(FXMVECTOR ScalingOrigin, FXMVECTOR ScalingOrientationQuaternion, FXMVECTOR Scaling, GXMVECTOR RotationOrigin, HXMVECTOR RotationQuaternion, HXMVECTOR Translation)

    afxError err = { 0 };

    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxV3d negScaOrigin = AfxV3dNeg(scalPivot);

    afxM4d scalOriginI = AFX_M4D_IDENTITY;
    scalOriginI.w = AfxV4dFromV3d(negScaOrigin);
    
    afxM4d scalOrientM = AfxM4dRotationQuat(scalOrient);
    afxM4d scalOrientT = AfxM4dTranspose(scalOrientM);

    afxM4d scalM = AfxM4dScaling(scaling);
    afxM4d rotM = AfxM4dRotationQuat(rotQuat);

    afxM4d t = AfxM4dMultiply(scalOriginI, scalOrientT);
    afxM4d m = AfxM4dMultiply(t, scalM);
    t = AfxM4dMultiply(m, scalOrientM);
    t.w = AfxV4dFromAtv3d(AfxV3dAdd(AfxV3dFromV4d(t.w), scalPivot));
    t.w = AfxV4dFromAtv3d(AfxV3dSub(AfxV3dFromV4d(t.w), rotPivot));
    m = AfxM4dMultiply(t, rotM);
    m.w = AfxV4dFromAtv3d(AfxV3dAdd(AfxV3dFromV4d(m.w), rotPivot));
    m.w = AfxV4dFromAtv3d(AfxV3dAdd(AfxV3dFromV4d(m.w), translation));
    return m;
}

_AFXINL afxM4d AfxM4dComposeAffineTransformation(afxV3d const scale, afxV3d const rotPivot, afxQuat const rot, afxV3d const translation)
{
    // Should be compatible with XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)

    afxError err = { 0 };

    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    afxM4d sm = AfxM4dScaling(scale);
    afxM4d rm = AfxM4dRotationQuat(rot);
    sm.w = AfxV4dFromV3d(AfxV3dSub(AfxV3dFromV4d(sm.w), rotPivot));
    afxM4d m = AfxM4dMultiply(sm, rm);
    m.w = AfxV4dFromV3d(AfxV3dAdd(AfxV3dFromV4d(m.w), rotPivot));
    m.w = AfxV4dFromV3d(AfxV3dAdd(AfxV3dFromV4d(m.w), translation));
}

// Assimilate

_AFXINL void AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxM3d const in[], afxM3d out[])
{
    // Should be compatible with void InPlaceSimilarityTransformScaleShear
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d tmp = AfxM3dMultiply(ltm, in[i]);
        out[i] = AfxM3dMultiply(tmp, iltm);
    }
}

_AFXINL void AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV3d const atv, afxUnit cnt, afxM4d const in[], afxM4d out[])
{
    // Should be compatible with void InPlaceSimilarityTransform4x3
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxM3d ss = AfxM3dFromM4d(in[i]);
        afxV4d pos;
        AfxAssimilateAtv4d(ltm, atv, 1, &in[i].w, &pos);
        AfxAssimilateLtm3d(ltm, iltm, 1, &ss, &ss);
        out[i] = AfxM4dFromM3d(ss, pos);
    }
}

_AFXINL afxM4d AfxM4dRigidMotion(afxM4d const mm, afxV3d const rotation, afxV3d const translation)
{
    // AfxApplyRootMotionVectorsToMatrix

    /*
        It composes and applies a transformation matrix based on root motion (translation and rotation), 
        adjusting the provided model matrix (mm) to produce a new transformed matrix (m). 
        This is essential in animation systems when root bone motion needs to be applied to a character's world transform.
    */

    afxError err = { 0 };
    //AFX_ASSERT(mm); // model matrix
    
    afxM4d m;
    afxM4d upd, tmp;
    afxReal rl = AfxV3dMag(rotation);

    if (rl <= 0.0)
    {
        upd = AfxM4dTranslation(translation);
        tmp = mm;
        return AfxM4dMultiplyAtm(upd, tmp);
    }

    afxV3d scaledRot = AfxV3dScale(rotation, 1.f / rl);
    tmp = AfxM4dRotationAxis(scaledRot, rl);
    upd = AfxM4dTranspose(tmp);
    upd.w = AfxV4dFromV3d(translation);
    tmp = mm;
    return AfxM4dMultiplyAtm(upd, tmp);
}
