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
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"
#include "qwadro/math/afxTrigonometry.h"

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatIdentity(void)
{
    afxError err = { 0 };
    return AFX_QUAT_IDENTITY;
}

_AFXINL afxQuat AfxQuatZero(void)
{
    afxError err = { 0 };
    return AFX_QUAT_ZERO;
}

_AFXINL afxQuat AfxQuatMake(afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = { 0 };
    return AFX_QUAT(x, y, z, w);
}

_AFXINL afxQuat AfxQuatReconstructV3d(afxV3d const in)
{
    afxError err = { 0 };

    // Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
    // Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
    // We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. 
    // We can then rebuild the missing element with this function.

    afxReal t = 1.f - AfxV3dDot(in, in);
    return AFX_QUAT(in.x, in.y, in.z,
                    t < 0.f ? 0.f : -AfxSqrtf(t));
}

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxBool AfxQuatIsIdentity(afxQuat const q)
{
    return AfxV4dIsIdentity(q);
}

_AFXINL afxBool AfxQuatIsNan(afxQuat const q)
{
    afxError err = { 0 };
    return AfxV4dIsNan(q);
}

_AFXINL afxBool AfxQuatIsInfinite(afxQuat const q)
{
    afxError err = { 0 };
    return AfxV4dIsInfinite(q);
}

_AFXINL afxBool AfxQuatIsEqual(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    return AfxV4dIsEqual(q, other);
}

_AFXINL afxBool AfxQuatIsDifferent(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    return AfxV4dIsDiff(q, other);
}

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatNormalize(afxQuat const in, afxReal* length)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalize(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dNormalize(in, length);
}

_AFXINL afxQuat AfxQuatNormalizeEstimated(afxQuat const in, afxReal* length)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalizeEst(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dNormalizeEstimated(in, length);
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatAdd(afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    return AfxV4dAdd(a, b);
}

_AFXINL afxQuat AfxQuatSub(afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    return AfxV4dSub(a, b);
}

_AFXINL afxQuat AfxQuatScale(afxQuat const in, afxReal scalar)
{
    afxError err = { 0 };
    return AfxV4dScale(in, scalar);
}

AFXINL afxQuat AfxQuatDiv(afxQuat const in, afxReal dividend)
{
    afxError err = { 0 };
    return AfxV4dDiv(in, (afxV4d) { dividend, dividend, dividend, dividend });
}

_AFXINL afxQuat AfxQuatMad(afxQuat const add, afxQuat const mul, afxQuat const f)
{
    return AfxV4dMad(add, mul, f);
}

_AFXINL afxQuat AfxQuatMads(afxQuat const add, afxQuat const mul, afxReal lambda)
{
    return AfxV4dMads(add, mul, lambda);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatConj(afxQuat const in)
{
    afxError err = { 0 };
    return AfxV4dNegAffine(in);
}

_AFXINL afxQuat AfxQuatNeg(afxQuat const in)
{
    afxError err = { 0 };
    return AfxV4dNeg(in);
}

_AFXINL afxQuat AfxQuatInv(afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionInverse(FXMVECTOR Q)

    afxError err = { 0 };

    afxReal norm = AfxQuatSq(in);

    if (AfxIsGreaterThanEpsilon(norm))
        return AFX_QUAT_ZERO;

    afxQuat conj = AfxQuatConj(in);
    afxV4d const v = { norm, norm, norm, norm };
    return AfxV4dDiv(conj, v);
}

_AFXINL afxV3d AfxQuatGetImaginaryPart(afxQuat const q)
{
    return AFX_V3D(q.x, q.y, q.z);
}

_AFXINL afxV3d AfxQuatGetBasisVector0(afxQuat const q)
{
    afxReal const x2 = q.x * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( ( q.w * w2) - 1.0  + q.x * x2,
                    ( q.z * w2)        + q.y * x2,
                    (-q.y * w2)        + q.z * x2);
}

_AFXINL afxV3d AfxQuatGetBasisVector1(afxQuat const q)
{
    afxReal const y2 = q.y * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( (-q.z * w2)        + q.x * y2,
                    ( q.w * w2) - 1.0  + q.y * y2, 
                    ( q.x * w2)        + q.z * y2);
}

_AFXINL afxV3d AfxQuatGetBasisVector2(afxQuat const q)
{
    afxReal const z2 = q.z * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( ( q.y * w2)        + q.x * z2,
                    (-q.x * w2)        + q.y * z2, 
                    ( q.w * w2) - 1.0  + q.z * z2);
}

_AFXINL afxV3d AfxQuatGetInvBasisVector0(afxQuat const q)
{
    afxReal const x2 = q.x * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( ( q.w * w2) - 1.0  + q.x * x2,
                    (-q.z * w2)        + q.y * x2, 
                    ( q.y * w2)        + q.z * x2);
}

_AFXINL afxV3d AfxQuatGetInvBasisVector1(afxQuat const q)
{
    afxReal const y2 = q.y * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( ( q.z * w2)        + q.x * y2,
                    ( q.w * w2) - 1.0  + q.y * y2, 
                    (-q.x * w2)        + q.z * y2);
}

_AFXINL afxV3d AfxQuatGetInvBasisVector2(afxQuat const q)
{
    afxReal const z2 = q.z * 2.0;
    afxReal const w2 = q.w * 2.0;
    return AFX_V3D( (-q.y * w2)        + q.x * z2, 
                    ( q.x * w2)        + q.y * z2, 
                    ( q.w * w2) - 1.0  + q.z * z2);
}

_AFXINL afxReal AfxQuatDot(afxQuat const q, afxQuat const other)
{
    // Should be compatible with  XMVECTOR XMQuaternionDot(FXMVECTOR Q1, FXMVECTOR Q2)

    afxError err = { 0 };
    return AfxV4dDot(q, other);
}

_AFXINL afxReal AfxQuatSq(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLengthSq(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dSq(q);
}

_AFXINL afxReal AfxQuatMag(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionLength(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dMag(q);
}

_AFXINL afxReal AfxQuatMagRecip(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionReciprocalLength(FXMVECTOR Q)

    afxError err = { 0 };
    return AfxV4dMagRecip(q);
}

_AFXINL afxReal AfxQuatAngle(afxQuat const q)
{
    afxError err = { 0 };
    return AfxAcosf(q.w) * 2.0;
}

_AFXINL afxReal AfxQuatAngle2(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    return AfxAcosf(AfxQuatDot(q, other)) * 2.0;
}

_AFXINL afxQuat AfxQuatDofX(afxQuat const q)
{
    return AFX_QUAT(q.v[0], 0, 0, q.v[3]);
}

_AFXINL afxQuat AfxQuatDofY(afxQuat const q)
{
    return AFX_QUAT(0, q.v[1], 0, q.v[3]);
}

_AFXINL afxQuat AfxQuatDofZ(afxQuat const q)
{
    return AFX_QUAT(0, 0, q.v[2], q.v[3]);
}

_AFXINL afxQuat AfxQuatDofXY(afxQuat const q)
{
    return AFX_QUAT(q.v[0], q.v[1], 0, q.v[3]);
}

_AFXINL afxQuat AfxQuatDofXZ(afxQuat const q)
{
    return AFX_QUAT(q.v[0], 0, q.v[2], q.v[3]);
}

_AFXINL afxQuat AfxQuatDofYZ(afxQuat const q)
{
    return AFX_QUAT(0, q.v[1], q.v[2], q.v[3]);
}

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatTangentM3d(afxM3d const tbn)
{
    afxError err = { 0 };

    afxQuat q = AfxQuatRotationM3d(tbn);
    q = AfxQuatNormalize(q, NIL);

    // Make sure QTangent is always positive

    if (q.w < 0)
        q.w = -q.w;

    // yosoygames.com.ar/wp/2018/03/vertex-formats-part-1-compression/

#if !0
    // As Crytek found out, this code is faulty when using SNORM. 
    // This is because when qTangent.w is exactly 0, there is no negative 0 for SNORM. 
    // Crytek’s solution was to add a bias, so that the sign of the bitangent is negative when q.w < bias, instead of testing q.w < 0. 
    // This means that q.w can never be 0. It’s a small precision sacrifice to solve an edge case.

    // Because '-0' sign information is lost when using integers, we need to apply a "bias"; while making sure the Quaternion stays normalized.
    // ** Also our shaders assume qTangent.w is never 0. **

    afxUnit const BITS = 16;
    afxReal const bias = 1.f / (2 ^ (BITS - 1) - 1);

    if (q.w < bias)
    {
        afxReal normFactor = AfxSqrtf(1.f - bias * bias);
        q.x * normFactor;
        q.y * normFactor;
        q.z * normFactor;
        q.w = bias;
    }
#endif

    //If it's reflected, then make sure .w is negative.
    afxV3d naturalBinormal = AfxV3dCross(tbn.x, tbn.z);

    if (AfxV3dDot(naturalBinormal, tbn.y) <= 0)
        q = AfxQuatNeg(q);
}

_AFXINL afxQuat AfxQuatTangentFrame(afxV3d const normal, afxV3d const tangent, afxV3d const bitangent)
{
    afxError err = { 0 };
    afxM3d tbn;
    tbn = AfxM3dMake(normal, tangent, bitangent);
    return AfxQuatTangentM3d(tbn);
}

// Utils

_AFXINL void AfxEnforceQuaternionContinuity(afxUnit cnt, afxQuat q[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(q);
    
    // Compatible with EnsureQuaternionContinuity(cnt, q)

    afxQuat last = { 0.0, 0.0, 0.0, 0.0 };

    for (afxUnit i = cnt; i-- > 0;)
    {
        afxQuat t = q[i];

        if (AfxQuatDot(t, last) < 0.0)
            t = AfxQuatNeg(t);

        last = t;
        q[i] = t;
    }
}
