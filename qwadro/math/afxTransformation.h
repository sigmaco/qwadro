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

#ifndef AFX_TRANSFORMATION_H
#define AFX_TRANSFORMATION_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxArithmetic.h"
#include "qwadro/math/afxArithmetic2.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

/// Rotates a 3D vector using a quaternion.
/// v = Returns the rotated 3D vector.
/// in = 3D vector to rotate.
/// rot = Quaternion that describes the rotation to apply to the vector.

AFXINL afxV3d AfxV3dRotate(afxV3d const in, afxQuat const q);

AFXINL afxV3d AfxV3dRotateInv(afxV3d const in, afxQuat const q);

// Similarity transform

AFXINL void     AfxAssimilateAtv3d
(
    afxM3d const ltm, 
    afxV3d const atv, 
    afxUnit cnt, 
    afxV3d const in[], 
    afxV3d out[]
); // make similarity transformation on afxV3d-based position.

AFXINL void     AfxAssimilateAtv4d
(
    afxM3d const ltm, 
    afxV3d const atv, 
    afxUnit cnt, 
    afxV4d const in[], 
    afxV4d out[]
); // make similarity transformation on afxV3d-based position.

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

/// Computes a rotation quaternion from a rotation matrix.
/// This function only uses the upper 3x3 portion of the matrix. 
/// Note if the input matrix contains scales, shears, or other non-rotation transformations in the upper 3x3 matrix, then the output of this function is ill-defined.
/// q = Returns the rotation quaternion.
/// m = Rotation matrix.

// aka get matrix rotation

AFXINL afxQuat AfxQuatRotationM3d
(
    afxM3d const m
);

AFXINL afxQuat AfxQuatRotationM4d
(
    afxM4d const m
);

/// Computes a rotation quaternion about an axis.
/// q = Returns the rotation quaternion.
/// axis = 3D vector describing the axis of rotation.
/// radians = Angle of rotation in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.

// makes a quaternion rotation that will rotate around the given axis by the specified angle. The axis must be a normalized vector.

AFXINL afxQuat AfxQuatRotationAxial
(
    afxV3d const axis,
    afxReal phi // angle
);

/// Computes a rotation quaternion based on the pitch, yaw, and roll (Euler angles).
/// Angles are measured clockwise when looking along the rotation axis toward the origin. This is a left-handed coordinate system. To use right-handed coordinates, negate all three angles.
/// The order of transformations is roll first, then pitch, then yaw.The rotations are all applied in the global coordinate frame.
/// This function takes x-axis, y-axis, z-axis angles as input parameters. 
/// The assignment of the labels pitch to the x-axis, yaw to the y-axis, and roll to the z-axis is a common one for computer graphics and games as it matches typical 'view' coordinate systems. 
/// There are of course other ways to assign those labels when using other coordinate systems (i.e. roll could be the x-axis, pitch the y-axis, and yaw the z-axis).
/// q = Returns the rotation quaternion.
/// pitchYawRoll = 3D vector containing the Euler angles in the order x-axis (pitch), then y-axis (yaw), and then z-axis (roll).

AFXINL afxQuat AfxQuatRotationEuler
(
    afxV3d const pitchYawRoll
);

/// Computes an axis and angle of rotation about that axis for a given quaternion.
/// q = Quaternion to measure.
/// axis = Address of a 3D vector describing the axis of rotation for the quaternion Q.
/// radians = Address of float describing the radian angle of rotation for the quaternion Q.

AFXINL afxV3d AfxQuatExtractAxialRotation
(
    afxQuat const q, 
    afxReal *radians
); // extracts an axis/angle representation to this quaternion rotation.

AFXINL void AfxAssimilateQuat
(
    afxM3d const ltm, 
    afxM3d const iltm, 
    afxUnit cnt, 
    afxQuat const in[], 
    afxQuat out[]
); // make similarity transformation on afxQuat-based orientation.

/// How to find a rotation which takes some initial vector into some final vector?
/// There are several variants of this problem, including "I am pointing my gun at Bob, and I would like to point my gun at Jim." 
/// I have seen this problem solved in many different ways, many times involving inverse trig functions. We are going to solve this problem with quaternions - and it's really easy!
/// Here is a function that will give you the rotation quaternion that will rotate some initial vector into some final vector

AFXINL afxQuat AfxQuatLookTo
(
    afxV3d const from, 
    afxV3d const to
);

AFXINL afxQuat AfxQuatFromAngularVelocity
(
    afxV3d const vel
);

/// Updating the dynamical state of a rigid body is referred to as integration. 
/// If you represent the orientation of this body with a quaternion, you will need to know how to update it. 
/// Here is a sample function for integrating a quaternion with a given angular velocity and time step.

AFXINL afxQuat AfxQuatIntegrate
(
    afxQuat const in, 
    afxV3d const omega, 
    afxReal dt
);

/*
    The AfxQuatIntegrateEULER function for first-order (explicit Euler) quaternion integration.
*/

AFXINL afxQuat AfxQuatIntegrateEULER
(
    afxQuat const in, 
    afxV3d const omega, 
    afxReal dt
);


/// Rotates an array of 3D vector using a quaternion.
/// v = Returns the rotated 3D vector.
/// in = 3D vector to rotate.
/// rot = Quaternion that describes the rotation to apply to the vector.

AFXINL void AfxQuatRotateV3d
(
    afxQuat const q, 
    afxUnit cnt, 
    afxV3d const in[], 
    afxV3d out[]
);

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// Affine transformation matrix methods

// Builds a matrix that scales along the x-axis, y-axis, and z-axis.
AFXINL afxM3d AfxM3dScaling(afxV3d const scale);
AFXINL afxM4d AfxM4dScaling(afxV3d const scale);

// Builds a rotation matrix from a quaternion.
AFXINL afxM3d AfxM3dRotationQuat(afxQuat const q);
AFXINL afxM4d AfxM4dRotationQuat(afxQuat const q);

// Builds a matrix that rotates around an arbitrary axis.
AFXINL afxM3d AfxM3dRotationAxis(afxV3d const axis, afxReal /*theta*/radians);
AFXINL afxM4d AfxM4dRotationAxis(afxV3d const axis, afxReal /*theta*/radians);

// Builds a rotation matrix based on a given pitch, yaw, and roll (Euler angles).
AFXINL afxM4d AfxM4dRotationEuler(afxReal pitch, afxReal yaw, afxReal roll);

// Builds a matrix that rotates around the x-axis, y-axis or z-axis.
// Angles are measured clockwise when looking along the rotation axis toward the origin.
AFXINL afxM4d AfxM4dRotationX(afxReal angle);
AFXINL afxM4d AfxM4dRotationY(afxReal angle);
AFXINL afxM4d AfxM4dRotationZ(afxReal angle);

AFXINL afxM4d AfxM4dTranslation
// Builds a translation matrix from the specified vector describing the translations along the x-axis, y-axis, and z-axis.
(
    afxV3d const translation // vector describing the translations along the x-axis, y-axis, and z-axis.
);

// PolarDecompose

// Qwadro expects all animation tracks to be decomposed. 
// Position, orientation, and scale/shear are stored separately. 
// The exporters kick out data in this format, but if you're writing your own important or processing tool, you may only have matrices as input. 
// Decomposing an arbitrary 3x3 matrix into its rotational and scale/shear components can be difficult, so Qwadro provides a function for doing this: 

AFXINL afxBool AfxM3dPolarDecompose
(
    afxM3d const m, 
    afxReal tol, 
    afxM3d* rm, 
    afxM3d* ssm
);

// Compose

AFXINL afxM4d AfxM4dComposeTransformation
// Builds a transformation matrix.
(
    // Vector describing the center of the scaling.
    afxV3d const scalPivot,
    // Quaternion describing the orientation of the scaling.
    afxQuat const scalOrient,
    // Vector containing the scaling factors for the x-axis, y-axis, and z-axis.
    afxV3d const scaling,
    // Vector describing the center of the rotation.
    afxV3d const rotPivot,
    // Quaternion describing the rotation around the origin indicated by rotOrigin.
    afxQuat const rotQuat,
    // Vector describing the translations along the x-axis, y-axis, and z-axis.
    afxV3d const translation
);

AFXINL afxM4d AfxM4dComposeAffineTransformation
// Builds an affine transformation matrix.
(
    // Vector of scaling factors for each dimension.
    afxV3d const scale,
    // Point identifying the center of rotation.
    afxV3d const rotPivot,
    // Rotation factors.
    afxQuat const rot,
    // Translation offsets.
    afxV3d const translation
);

// Assimilate

AFXINL void AfxAssimilateLtm3d
(
    afxM3d const ltm, 
    afxM3d const iltm, 
    afxUnit cnt, 
    afxM3d const in[], 
    afxM3d out[]
); // make similarity transformation on afxM3d-based scale/shear.

AFXINL void AfxAssimilateAtm4d
(
    afxM3d const ltm, 
    afxM3d const iltm, 
    afxV3d const atv, 
    afxUnit cnt, 
    afxM4d const in[], 
    afxM4d out[]
);

AFXINL afxM4d AfxM4dRigidMotion
(
    afxM4d const mm, 
    afxV3d const rotation, 
    afxV3d const translation
);

#endif//AFX_TRANSFORMATION_H
