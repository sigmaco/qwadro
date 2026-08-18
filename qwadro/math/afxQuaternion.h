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

/**
    The Qwadro quaternion functions use an 4D vector to represent quaternions, 
    where the X, Y, and Z components are the vector part and the W component is the scalar part.

    afxQuat: A 4-dimensional vector representing a rotation.
    The vector represents a 4 dimensional complex number where multiplication of the basis elements is not commutative(multiplying i with j gives a different result than multiplying j with i).
    Multiplying quaternions reproduces rotation sequences. However quaternions need to be often renormalized, or else they suffer from precision issues.
    It can be used to perform SLERP(spherical - linear interpolation) between two rotations.
*/

#ifndef AFX_QUATERNION_H
#define AFX_QUATERNION_H

#include "qwadro/math/afxVector.h"

typedef enum afxQuatBlend
// The afxQuatBlend enum represents different blending modes for quaternions, which are often used for representing rotations in 3D space. 
// These blending modes allow for various ways of interpolating or combining rotations, depending on the specific use case, 
// such as animation blending, procedural animation, or adjusting poses.
{
    // Directly mix the quaternions as they are.
    // This blending mode uses a direct interpolation of the quaternions, meaning the input quaternions are blended as they are, 
    // without any modification. This is the most straightforward blending mode, where two or more quaternions are mixed together 
    // directly. This is useful when you just want to blend rotations in their raw form, such as when blending between different 
    // animation poses in a straightforward way.
    afxQuatBlend_DIRECT,

    // Mix input as its inverse product.
    // This mode blends the inverse of the input quaternion. Essentially, it blends the input quaternion with its inverse rotation.
    // This is useful when you want to perform an inverse blending operation, which can be helpful in situations like reverse 
    // animations, mirroring rotations, or undoing a transformation. This could be applied, for instance, in an inverse 
    // kinematics (IK) system or when calculating corrective transformations.
    afxQuatBlend_INV,

    // Use adjacency to the rest pose for blending.
    // In this mode, blending uses adjacency to the rest pose. It likely calculates a relative rotation from the current pose 
    // to the rest pose and blends between them. This mode is useful in skeletal animation systems, where you want to smoothly 
    // transition between the current pose and a rest pose (or a reference pose) of the character. The "adjacency" implies that 
    // the blending is based on how close or far the current pose is relative to the neutral rest pose of the skeleton.
    afxQuatBlend_ADJACENT,

    // Use brevity for adjacent blending.
    // This mode uses brevity(likely referring to an efficient or simplified blending technique) to accumulate rotations relative 
    // to the rest pose.This could involve applying a series of incremental transformations rather than blending between large 
    // rotation differences. This mode is potentially useful when you're accumulating small rotations (like blending incremental 
    // pose changes) over time, such as in procedural animation systems or when blending small corrective transformations over a 
    // longer period.
    afxQuatBlend_ACCUM_ADJACENT,
} afxQuatBlend;

#define AFX_QUAT(x, y, z, w) (afxQuat){ (x), (y), (z), (w) }
#define AFX_QUATERNION(x, y, z, w) AFX_QUAT((x), (y), (z), (w))

#define AFX_QUAT_ZERO AFX_QUAT(0, 0, 0, 0)

#define AFX_QUAT_IDENTITY AFX_QUAT(0, 0, 0, 1)

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

/// Redefine um quaternion ao estado de unit/identity.
/// Dado um quaternion (x, y, z, w), a função AfxQuatReset vai fazer o quaternion (0, 0, 0, 1).

AFXINL void AfxQuatReset(afxQuat q);

/// Preenche um quaternion com zeros.

AFXINL void AfxQuatZero(afxQuat q);

/// Define cada componente de um quaternion.

AFXINL void AfxQuatSet
(
    afxQuat q, 
    afxReal x, 
    afxReal y, 
    afxReal z, 
    afxReal w
);

/// Since rotation quaternions are normalized, there are only 3 degrees of freedom instead of 4. 
/// Thus you only really need to store 3 components of the quaternion, and the 4th can be calculated using the formula
/// We can compress a Quaternion down to three elements by making sure one of the them is greater than or equal to zero. We can then rebuild the missing element with this function.

AFXINL void AfxQuatReconstructV3d
(
    afxQuat q, 
    afxV3d const in
);

////////////////////////////////////////////////////////////////////////////////

/// Testa se um quaternion é um unit/identity quaternion.
/// q = Quaternion to test.

AFXINL afxBool  AfxQuatIsIdentity
(
    afxQuat const q
);

AFXINL afxBool AfxQuatIsNan(afxQuat const q);
AFXINL afxBool AfxQuatIsInfinite(afxQuat const q);

/// Em margem de erro escalar de Epsilon, testa se dois quaternions são equivalentes.
/// q = Quaternion to test.

AFXINL afxBool  AfxQuatIsEqual
(
    afxQuat const q, 
    afxQuat const other
);

AFXINL afxBool AfxQuatIsDifferent(afxQuat const q, afxQuat const other);

////////////////////////////////////////////////////////////////////////////////

/// Cambia (operação de troca) os dados entre dois quaternions.

AFXINL void AfxQuatSwap
(
    afxQuat q, 
    afxQuat other
);

/// Copia os dados de um outro quaternion.

AFXINL void AfxQuatCopy
(
    afxQuat q, 
    afxQuat const in
);

////////////////////////////////////////////////////////////////////////////////

/// Normalizes a quaternion.
/// q = Returns the normalized form of in.
/// in = Quaternion to normalize.

AFXINL afxReal  AfxQuatNormalize
(
    afxQuat q, 
    afxQuat const in
);

/// Estimates the normalized version of a quaternion.
/// This estimation offer increased performance at the expense of reduced accuracy. 
/// So, it is appropriate for non-critical calculations where accuracy can be sacrificed for speed. The exact amount of lost accuracy and speed increase are platform dependent.
/// This function internally calls the AfxV4dNormalizeEstimated function.
/// q = An XMVECTOR union that is the estimate of the normalized version of a quaternion.
/// in = A quaternion for which to estimate the normalized version.

AFXINL afxReal  AfxQuatNormalizeEstimated
(
    afxQuat q, 
    afxQuat const in
);

////////////////////////////////////////////////////////////////////////////////

// q = a + b
AFXINL void AfxQuatAdd(afxQuat q, afxQuat const a, afxQuat const b);

// q = a - b
AFXINL void AfxQuatSub(afxQuat q, afxQuat const a, afxQuat const b);

// q = a * s
AFXINL void AfxQuatScale(afxQuat q, afxQuat const in, afxReal scalar);

// q = a / s
AFXINL void AfxQuatDiv(afxQuat q, afxQuat const in, afxReal dividend);

// q = add + (mul * f)
AFXINL void AfxQuatMad(afxQuat q, afxQuat const add, afxQuat const mul, afxQuat const f);

// q = add + (mul * lambda)
AFXINL void AfxQuatMads(afxQuat q, afxQuat const add, afxQuat const mul, afxReal lambda);

////////////////////////////////////////////////////////////////////////////////

AFXINL void AfxQuatGetImaginaryPart(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetBasisVector0(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetBasisVector1(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetBasisVector2(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetInvBasisVector0(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetInvBasisVector1(afxQuat const q, afxV3d v);

AFXINL void AfxQuatGetInvBasisVector2(afxQuat const q, afxV3d v);

/// Computes the dot product of two quaternions.
/// q = First quaternion.
/// other = Second quaternion.

AFXINL afxReal  AfxQuatDot(afxQuat const q, afxQuat const other);

/// Computes the square of the magnitude of a quaternion.
/// q = Quaternion to measure.

AFXINL afxReal  AfxQuatSq(afxQuat const q);

/// Computes the magnitude of a quaternion.
/// q = Quaternion to measure.

AFXINL afxReal  AfxQuatMag(afxQuat const q);

/// Computes the reciprocal of the magnitude of a quaternion.
/// q = Returns the reciprocal of the magnitude of in.
/// in = Quaternion to measure.

AFXINL afxReal  AfxQuatMagRecip(afxQuat const q);

/// Negativa todos os componentes de um quaternion.

AFXINL void AfxQuatNeg(afxQuat q, afxQuat const in);

/// Computes the conjugate of a quaternion.
/// Given a quaternion ( x, y, z, w), the AfxQuatConj function returns the quaternion (-x, -y, -z, w).
/// Use the AfxQuatNormalize function for any quaternion input that is not already normalized.
/// q = Returns the conjugate of in.
/// in = The quaternion to conjugate.

AFXINL void AfxQuatConj(afxQuat q, afxQuat const in);

/// Computes the inverse of a quaternion.
/// q = Returns the inverse of in.
/// in = Quaternion to invert.

AFXINL void AfxQuatInv(afxQuat q, afxQuat const in); // q = inverse of in

AFXINL afxReal AfxQuatAngle(afxQuat const q);

AFXINL afxReal AfxQuatAngle2(afxQuat const q, afxQuat const other);

////////////////////////////////////////////////////////////////////////////////

AFXINL void AfxQuatTangentM3d(afxQuat q, afxM3d const tbn);

AFXINL void AfxQuatTangentFrame(afxQuat q, afxV3d const normal, afxV3d const tangent, afxV3d const bitangent);

////////////////////////////////////////////////////////////////////////////////

/// Qwadro uses quaternions to represent any decomposed rotational quantity. 
/// On occasion, if you are doing some very low-level animation work, 
/// you may find yourself working with an array of quaternions from Qwadro (such as those that come out of the b-spline buffer construction routines). 
/// For interpolation purposes, it is important that all neighboring quaternions be "as close as possible" to eachother. 
/// Since all single-spin rotations can be represented by one of two different quaternions, 
/// this amounts to running through the quaternion buffer and selecting the sequence of quaternions that minimizes the distance between each successive quaternion while keep the rotational meaning the same. 
/// Qwadro makes it simple to do this:

AFXINL void AfxEnforceQuaternionContinuity(afxUnit cnt, afxQuat q[]);

/// Suffice to say, you shouldn't normally be calling AfxEnforceQuaternionContinuity on anything unless you know you need to, 
/// because any time you're using Qwadro's higher-level animation functions, 
/// it makes sure that all quaternion operations happen correctly. 
/// AfxEnforceQuaternionContinuity is only for situations where you are manually performing some animation operation.

#endif//AFX_QUATERNION_H
