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

#ifndef AFX_MULTIPLICATION_H
#define AFX_MULTIPLICATION_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

//  --- post-multiply
//  m[0][i/0] * v[i/0], m[0][i/1] * v[i/1], m[0][i/2] * v[i/2], m[0][i/3] * v[i/3]

//  --- pre-multiply
//  v[i/0] * m[i/0][0], v[i/1] * m[i/1][0], v[i/2] * m[i/2][0], v[i/3] * m[i/3][0]

// Multiply (column) vector by matrix ([0][0], [1][0], [2][0], [3][0])
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

// Multiply (row) vector by matrix ([0][0], [0][1], [0][2], [0][3])
// With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).

AFXINL void     AfxV2dPostMultiplyM2d(afxV2d v, afxM2d const m, afxV2d const in); // m * v
AFXINL void     AfxV3dPostMultiplyM3d(afxV3d v, afxM3d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyM4d(afxV4d v, afxM4d const m, afxV4d const in); // m * v

AFXINL void     AfxV3dPostMultiplyLtm4d(afxV3d v, afxM4d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyM3d(afxV4d v, afxM3d const m, afxV4d const in); // m * v

AFXINL void     AfxV2dPostMultiplyAtm4d(afxV2d v, afxM4d const m, afxV2d const in); // m * v
AFXINL void     AfxV3dPostMultiplyAtm4d(afxV3d v, afxM4d const m, afxV3d const in); // m * v
AFXINL void     AfxV4dPostMultiplyAtm4d(afxV4d v, afxM4d const m, afxV4d const in); // m * v

AFXINL void     TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform);

AFXINL void     AfxV2dPreMultiplyM2d(afxV2d v, afxV2d const in, afxM2d const m); // v * m
AFXINL void     AfxV3dPreMultiplyM3d(afxV3d v, afxV3d const in, afxM3d const m); // v * m
AFXINL void     AfxV4dPreMultiplyM4d(afxV4d v, afxV4d const in, afxM4d const m); // v * m

AFXINL void     AfxV3dPreMultiplyLtm4d(afxV3d v, afxV3d const in, afxM4d const m); // v * m
AFXINL void     AfxV4dPreMultiplyM3d(afxV4d v, afxV4d const in, afxM3d const m); // v * m

AFXINL void     AfxV2dPreMultiplyAtm4d(afxV2d v, afxV2d const in, afxM4d const m); // v * m
AFXINL void     AfxV3dPreMultiplyAtm4d(afxV3d v, afxV3d const in, afxM4d const m); // v * m
AFXINL void     AfxV4dPreMultiplyAtm4d(afxV4d v, afxV4d const in, afxM4d const m); // v * m

////////////////////////////////////////////////////////////////////////////////
// QUATERNION (aka QWATERNION)                                                //
////////////////////////////////////////////////////////////////////////////////

/// Computes the product of two quaternions.
/// This multiplication is a quaternion multiplication. The way that quaternion multiplication is defined is one of the things that makes quaternions good at representing rotations.
/// The result represents the rotation A followed by the rotation B to be consistent with AfxMulplityM4d concatenation since this function is typically used to concatenate quaternions that represent rotations (i.e. it returns B*A).
/// q = Returns the product of two quaternions as B*A (which is the concatenation of a rotation A followed by the rotation B)
/// a = First quaternion.
/// b = Second quaternion.

// q = a * b
AFXINL void AfxQuatMultiply(afxQuat q, afxQuat const a, afxQuat const b);

////////////////////////////////////////////////////////////////////////////////
// MATRIX (aka QWATRIX)                                                       //
////////////////////////////////////////////////////////////////////////////////

/*
    The AfxM2dMultiply(), AfxM3dMultiply(), AfxM4dMultiply(), 
    AfxM3dMultiplyAtm(), AfxM4dMultiplyLtm() and AfxM4dMultiplyAtm() functions
    computes the product of two matrices as:

    column 0 * [ row 0, 1, 2, 3 ]
    column 1 * [ row 0, 1, 2, 3 ]
    column 2 * [ row 0, 1, 2, 3 ]
    column 3 * [ row 0, 1, 2, 3 ]
*/

AFXINL void     AfxM2dMultiply(afxM2d m, afxM2d const in, afxM2d const mul);
AFXINL void     AfxM3dMultiply(afxM3d m, afxM3d const in, afxM3d const mul);
AFXINL void     AfxM4dMultiply(afxM4d m, afxM4d const in, afxM4d const mul);

AFXINL void     AfxM3dMultiplyAtm(afxM3d m, afxM3d const in, afxM3d const mul); // 2x2 subset only
AFXINL void     AfxM4dMultiplyLtm(afxM4d m, afxM4d const in, afxM4d const mul); // 3x3 subset only
AFXINL void     AfxM4dMultiplyAtm(afxM4d m, afxM4d const in, afxM4d const mul); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

/*
    The AfxM2dMultiplyTransposed(), AfxM3dMultiplyTransposed(), AfxM4dMultiplyTransposed(), 
    AfxM3dMultiplyAtmTransposed(), AfxM4dMultiplyLtmTransposed(), AfxM4dMultiplyAtmTransposed() functions 
    computes the product of two matrices as:

    [ column 0, 1, 2, 3 ] * row 0
    [ column 0, 1, 2, 3 ] * row 1
    [ column 0, 1, 2, 3 ] * row 2
    [ column 0, 1, 2, 3 ] * row 3
*/

AFXINL void     AfxM2dMultiplyTransposed(afxM2d m, afxM2d const in, afxM2d const mul);
AFXINL void     AfxM3dMultiplyTransposed(afxM3d m, afxM3d const in, afxM3d const mul);
AFXINL void     AfxM4dMultiplyTransposed(afxM4d m, afxM4d const in, afxM4d const mul);

AFXINL void     AfxM3dMultiplyAtmTransposed(afxM3d m, afxM3d const in, afxM3d const mul); // 2x2 subset only
AFXINL void     AfxM4dMultiplyLtmTransposed(afxM4d m, afxM4d const in, afxM4d const mul); // 3x3 subset only of an affine transformation matrix
AFXINL void     AfxM4dMultiplyAtmTransposed(afxM4d m, afxM4d const in, afxM4d const mul); // 4x3 subset only (aka RenderWare (RwMatrix) matrix)

AFXINL void     TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix);

/*
    The AfxM2dPostMultiplyV2d(), AfxM3dPostMultiplyV3d(), AfxM4dPostMultiplyV4d(),
    AfxM3dPostMultiplyLtv4d(), AfxM4dPostMultiplyLtv3d(),
    AfxM4dPostMultiplyAtv3d(), AfxM4dPostMultiplyAtv4d() functions
    computes the product of two matrices as:

    column [ 0, 1 ] * vector
*/

AFXINL void     AfxM2dPostMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxM3dPostMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPostMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM3dPostMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxM4dPostMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);

AFXINL void     AfxM4dPostMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPostMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

/*
    The AfxM2dPreMultiplyV2d(), AfxM3dPreMultiplyV3d(), AfxM4dPreMultiplyV4d(),
    AfxM3dPreMultiplyLtv4d(), AfxM4dPreMultiplyLtv3d(),
    AfxM4dPreMultiplyAtv3d(), AfxM4dPreMultiplyAtv4d() functions
    computes the product of two matrices as:

    vector * row [ 0, 1 ]
*/

AFXINL void     AfxM2dPreMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxM3dPreMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPreMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxM3dPreMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);
AFXINL void     AfxM4dPreMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPreMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxM4dPreMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[]);

/*
    The AfxM2dPostMultiplyV2dSerialized(), AfxM3dPostMultiplyV3dSerialized(), AfxM4dPostMultiplyV4dSerialized(),
    AfxM4dPostMultiplyAtv3dSerialized(), AfxM3dPostMultiplyLtv4dSerialized(), AfxM4dPostMultiplyAtv4dSerialized() functions
    computes, in a strided stream, the product of two matrices as:

    column [ 0, 1 ] * vector
*/

AFXINL void     AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxUnit cnt, afxV2d const in[], afxUnit inStride, afxV2d out[], afxUnit outStride);
AFXINL void     AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride);
AFXINL void     AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride);

AFXINL void     AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride);
AFXINL void     AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride);
AFXINL void     AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride);

#endif//AFX_MULTIPLICATION_H
