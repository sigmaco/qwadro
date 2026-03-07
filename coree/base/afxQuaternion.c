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

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatReset(afxQuat q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxQuatCopy(q, AFX_QUAT_IDENTITY);
}

_AFXINL void AfxQuatZero(afxQuat q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxQuatCopy(q, AFX_QUAT_ZERO);
}

_AFXINL void AfxQuatSet(afxQuat q, afxReal x, afxReal y, afxReal z, afxReal w)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AfxV4dSet(q, x, y, z, w);
}

_AFXINL void AfxQuatReconstructV3d(afxQuat q, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);

    // Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
    // Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
    // We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. 
    // We can then rebuild the missing element with this function.

    afxReal t = 1.f - AfxV3dDot(in, in);
    AfxV3dCopy(q, in);
    q[3] = t < 0.f ? 0.f : -AfxSqrtf(t);
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
    AFX_ASSERT(q);
    return AfxV4dIsNan(q);
}

_AFXINL afxBool AfxQuatIsInfinite(afxQuat const q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxV4dIsInfinite(q);
}

_AFXINL afxBool AfxQuatIsEqual(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    return AfxV4dIsEqual(q, other);
}

_AFXINL afxBool AfxQuatIsDifferent(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    return AfxV4dIsDiff(q, other);
}

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatSwap(afxQuat q, afxQuat other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
    AFX_ASSERT(q != other);
    AfxV4dSwap(q, other);
}

_AFXINL void AfxQuatCopy(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(q, in);
    AfxV4dCopy(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxReal AfxQuatNormalize(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalize(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    return AfxV4dNormalize(q, in);
}

_AFXINL afxReal AfxQuatNormalizeEstimated(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionNormalizeEst(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    return AfxV4dNormalizeEstimated(q, in);
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatAdd(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AfxV4dAdd(q, a, b);
}

_AFXINL void AfxQuatSub(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(b);
    AFX_ASSERT(a);
    AfxV4dSub(q, a, b);
}

_AFXINL void AfxQuatScale(afxQuat q, afxQuat const in, afxReal scalar)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dScale(q, in, scalar);
}

AFXINL void AfxQuatDiv(afxQuat q, afxQuat const in, afxReal dividend)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dDiv(q, in, (afxV4d) { dividend, dividend, dividend, dividend });
}

_AFXINL void AfxQuatMad(afxQuat q, afxQuat const add, afxQuat const mul, afxQuat const f)
{
    AfxV4dMad(q, add, mul, f);
}

_AFXINL void AfxQuatMads(afxQuat q, afxQuat const add, afxQuat const mul, afxReal lambda)
{
    AfxV4dMads(q, add, mul, lambda);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatConj(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(in);
    AfxV4dNegAffine(q, in);
}

_AFXINL void AfxQuatNeg(afxQuat q, afxQuat const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);
    AfxV4dNeg(q, in);
}

_AFXINL void AfxQuatInv(afxQuat q, afxQuat const in)
{
    // Should be compatible with XMVECTOR XMQuaternionInverse(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(q);

    afxReal norm = AfxQuatSq(in);

    if (AfxIsGreaterThanEpsilon(norm)) AfxQuatZero(q);
    else
    {
        AfxQuatConj(q, in);
        afxV4d const v = { norm, norm, norm, norm };
        AfxV4dDiv(q, q, v);
    }
}

_AFXINL void AfxQuatGetImaginaryPart(afxQuat const q, afxV3d v)
{
    return AfxV3dCopy(v, q);
}

_AFXINL void AfxQuatGetBasisVector0(afxQuat const q, afxV3d v)
{
    afxReal const x2 = q[0] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[3] * w2) - 1.0  + q[0] * x2,
                    ( q[2] * w2)        + q[1] * x2,
                    (-q[1] * w2)        + q[2] * x2);
}

_AFXINL void AfxQuatGetBasisVector1(afxQuat const q, afxV3d v)
{
    afxReal const y2 = q[1] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    (-q[2] * w2)        + q[0] * y2, 
                    ( q[3] * w2) - 1.0  + q[1] * y2, 
                    ( q[0] * w2)        + q[2] * y2);
}

_AFXINL void AfxQuatGetBasisVector2(afxQuat const q, afxV3d v)
{
    afxReal const z2 = q[2] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[1] * w2)        + q[0] * z2, 
                    (-q[0] * w2)        + q[1] * z2, 
                    ( q[3] * w2) - 1.0  + q[2] * z2);
}

_AFXINL void AfxQuatGetInvBasisVector0(afxQuat const q, afxV3d v)
{
    afxReal const x2 = q[0] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[3] * w2) - 1.0  + q[0] * x2, 
                    (-q[2] * w2)        + q[1] * x2, 
                    ( q[1] * w2)        + q[2] * x2);
}

_AFXINL void AfxQuatGetInvBasisVector1(afxQuat const q, afxV3d v)
{
    afxReal const y2 = q[1] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    ( q[2] * w2)        + q[0] * y2, 
                    ( q[3] * w2) - 1.0  + q[1] * y2, 
                    (-q[0] * w2)        + q[2] * y2);
}

_AFXINL void AfxQuatGetInvBasisVector2(afxQuat const q, afxV3d v)
{
    afxReal const z2 = q[2] * 2.0;
    afxReal const w2 = q[3] * 2.0;
    AfxV3dSet(v,    (-q[1] * w2)        + q[0] * z2, 
                    ( q[0] * w2)        + q[1] * z2, 
                    ( q[3] * w2) - 1.0  + q[2] * z2);
}

_AFXINL afxReal AfxQuatDot(afxQuat const q, afxQuat const other)
{
    // Should be compatible with  XMVECTOR XMQuaternionDot(FXMVECTOR Q1, FXMVECTOR Q2)

    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(other);
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
    AFX_ASSERT(q);
    return AfxV4dMag(q);
}

_AFXINL afxReal AfxQuatMagRecip(afxQuat const q)
{
    // Should be compatible with XMVECTOR XMQuaternionReciprocalLength(FXMVECTOR Q)

    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxV4dMagRecip(q);
}

_AFXINL afxReal AfxQuatAngle(afxQuat const q)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxAcosf(q[3]) * 2.0;
}

_AFXINL afxReal AfxQuatAngle2(afxQuat const q, afxQuat const other)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    return AfxAcosf(AfxQuatDot(q, other)) * 2.0;
}

////////////////////////////////////////////////////////////////////////////////
// Trigonometry                                                               //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatTangentM3d(afxQuat q, afxM3d const tbn)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(tbn);

    AfxQuatRotationM3d(q, tbn);
    AfxQuatNormalize(q, q);

    // Make sure QTangent is always positive

    if (q[3] < 0)
        q[3] = -q[3];

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

    if (q[3] < bias)
    {
        afxReal normFactor = AfxSqrtf(1.f - bias * bias);
        AfxV3dScale(q, q, normFactor);
        q[3] = bias;
    }
#endif

    //If it's reflected, then make sure .w is negative.
    afxV3d naturalBinormal;
    AfxV3dCross(naturalBinormal, tbn[0], tbn[2]);

    if (AfxV3dDot(naturalBinormal, tbn[1]) <= 0)
        AfxQuatNeg(q, q);
}

_AFXINL void AfxQuatTangentFrame(afxQuat q, afxV3d const normal, afxV3d const tangent, afxV3d const bitangent)
{
    afxError err = { 0 };
    AFX_ASSERT(q);
    AFX_ASSERT(normal);
    AFX_ASSERT(tangent);
    AFX_ASSERT(bitangent);

    afxM3d tbn;
    AfxM3dSet(tbn, normal, tangent, bitangent);
    AfxQuatTangentM3d(q, tbn);
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
        afxQuat t;
        AfxQuatCopy(t, q[i]);

        if (AfxQuatDot(t, last) < 0.0)
            AfxQuatNeg(t, t);

        AfxQuatCopy(last, t);
        AfxQuatCopy(q[i], t);
    }
}
