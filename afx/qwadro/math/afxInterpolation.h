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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_INTERPOLATION_H
#define AFX_INTERPOLATION_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

// SLERP
// Function to perform Spherical Linear Interpolation (SLERP) between two quaternions
// Slerp is a type of interpolation that smoothly interpolates between two vectors along the shortest path (on a sphere). 
// Unlike linear interpolation, it handles rotations and vectors that lie on the surface of a sphere.
// Rotating objects in 3D space, interpolating between two orientations (used in quaternions and rotation matrices).
// v(t) = ((sin((1 - t) * theta)) / sin(theta)) * x + ((sin(t) * theta) / (sin(theta))) * y

AFXINL void     AfxV2dSlerp(afxV2d v, afxV2d x, afxV2d y, afxReal t);
AFXINL void     AfxV3dSlerp(afxV3d v, afxV3d x, afxV3d y, afxReal t);
AFXINL void     AfxV4dSlerp(afxV4d v, afxV4d x, afxV4d y, afxReal t);

/// Hermite interpolation
/// v = (2 * t^3 - 3 * t^2 + 1) * posA + (t^3 - 2 * t^2 + t) * tanA + (-2 * t^3 + 3 * t^2) * posB + (t^3 - t^2) * tanB

AFXINL void     AfxHermiteV2d(afxV2d v, afxV2d const posA, afxV2d const tanA, afxV2d const posB, afxV2d const tanB, afxReal t);
AFXINL void     AfxHermiteV3d(afxV3d v, afxV3d const posA, afxV3d const tanA, afxV3d const posB, afxV3d const tanB, afxReal t);
AFXINL void     AfxHermiteV4d(afxV4d v, afxV4d const posA, afxV4d const tanA, afxV4d const posB, afxV4d const tanB, afxReal t);

/// Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each point Pi is calculated using the previous and next point on the spline, T(Pi + 1 - Pi - 1).
///
///     [ aX, aY, aZ, aW ] [  0  2  0  0 ] [  1  ]
///     [ bX, bY, bZ, bW ] [ -1  0  1  0 ] [  t  ]
///     [ cX, cY, cZ, cW ] [  2 -5  4 -1 ] [ t^2 ]
///     [ dX, dY, dZ, dW ] [ -1  3 -3  1 ] [ t^3 ] * 0.5 => v

/// v = ((-t^3 + 2 * t^2 - t) * a + (3 * t^3 - 5 * t^2 + 2) * b + (-3 * t^3 + 4 * t^2 + t) * c + (t^3 - t^2) * d) * 0.5

AFXINL void     AfxCatmullV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const d, afxReal t);
AFXINL void     AfxCatmullV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const d, afxReal t);
AFXINL void     AfxCatmullV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const d, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

/// Interpolating a quaternion is useful when smoothly varying between an initial and final rotation. 
/// Interpolation is good for finding arbitrary in-between values of rotation. This is employed especially in character animation systems. 
/// It is possible to interpolate rotation matrices, but the interpolated matrix may not be a size and shape preserving matrix. 
/// Needless to say, interpolating a quaternion is a bajillion times easier than interpolating rotation matrices.

/// There is one interesting property of quaternions that comes into play when dealing with interpolation. 
/// If a rotation can be represented by a quaternion q, then the quaterion - q also represents the same rotation. 
/// Why is that ? I'm not going to explain it right now, other than to say that it is connected to the very fabric of reality. 
/// (You think I'm joking but I'm not!) What you need to worry about is which one of these quaternions you are going to use.

/// To describe the difference between q and -q, consider that you turn a quarter turn to your left. 
/// Esentially this is the same as turning 3 / 4 turn to your right. 
/// One turn is the "short" turn and the other is the "long"one. 
/// When representing a static orientation it is irrelevant if a quaternion represents the short, or long path, because it just sits in the final position and you don't get to see the in-between values. 
/// However, when you are blending it surely does make a difference.

/// When blending between an initial and a final quaternion, there is some ambiguity as to if we are taking the "short" way or the "long" way. 
/// It seems like the right thing to always blend on the shortest path. Given the two input quaternions, it is possible to determine which way we are going to blend. 
/// You can check this by examining the sign of the 4D dot product of the inputs. If the sign is negative, then you know you are going to be blending the long way.

/// So, what do you do if you find out that you are blending the long way? Simply flip the sign on one of your input quaternions.remember q and -q represent the same rotation. 
/// Flipping the sign on one of your inputs will flip the sign of the 4D dot product.

/// Now that we have discussed that little tid-bit, let's move on to interpolation formulas.

/// There are a few different interpolation formulas, but two main ones:
///     NLerp is a linear interpolation of the components that is followed by a normalization of the interpolated quaternion, to ensure that it represents a rotation.
///     Slerp is a spherical interpolation which interpolates in a spherical space, rather than in the cartesian space of the coordinates. 
/// The interpolant of the slerp function moves at a constant motion, while the interpolant of the NLerp has some non-linear acceleration.

/// Heres the quick and dirty: Don't mess around with Slerp, even though you think it might be the more "correct" thing to do. It is too expensive, and has too many special cases that need to be considered. 
/// There are some complicated schemes that try to closely approximate the Slerp function, but it just isn't worth it. Just use NLerp. Especially for computationally strapped code.

AFXINL void AfxQuatLerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal percent);

/// Interpolates between two unit quaternions, using spherical linear interpolation.
/// When t is 0.0f, the function returns A. When t is 1.0f, the function returns B.
/// q = Returns the interpolated quaternion. If A and B are not unit quaternions, the resulting interpolation is undefined.
/// a = Unit quaternion to interpolate from.
/// b = Unit quaternion to interpolate to.
/// t = Interpolation control factor.

AFXINL void AfxQuatSlerp(afxQuat q, afxQuat const a, afxQuat const b, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// It is often used in shader programming and graphics.
// v = x * (1 - t) + y * t

AFXINL void     AfxM2dMix(afxM2d v, afxM2d const x, afxM2d const y, afxReal t);
AFXINL void     AfxM3dMix(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dMix(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

AFXINL void     AfxM3dMixAtm(afxM3d m, afxM3d const a, afxM3d const b, afxReal t);
AFXINL void     AfxM4dMixAtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t);
AFXINL void     AfxM4dMixLtm(afxM4d m, afxM4d const a, afxM4d const b, afxReal t);

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// x + t * (y - x)

AFXINL void     AfxM2dLerp(afxM2d v, afxM2d const x, afxM2d const y, afxReal t);
AFXINL void     AfxM3dLerp(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dLerp(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

AFXINL void     AfxM3dLerpAtm(afxM3d v, afxM3d const x, afxM3d const y, afxReal t);
AFXINL void     AfxM4dLerpAtm(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);
AFXINL void     AfxM4dLerpLtm(afxM4d v, afxM4d const x, afxM4d const y, afxReal t);

#endif//AFX_INTERPOLATION_H
