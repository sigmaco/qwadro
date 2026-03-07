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

/*
    Qwadro uses its own matrix denominated as Qwatrix, it is the same matrix layout as do OpenGL. Forget about column-major vs row-major.
    Confusions around this subject is mostly from concepts came from mathemetics, not natural and pertaining to computer science.

    As OpenGL, Qwadro matrix has position in m[3][0], m[3][1], m[3][2], and m[3][3],
    then it is stored in columns laid out horizontally (aka "transposed"), what makes it looks like row-major order while it is in fact column-major.

    Column-major is thing emerged from mathematics. In programming, we are effectively working with rows, what is counterintuitive.

    In OpenGL, the convention for matrices is to store them in a column-major order,
    meaning that the matrix is stored column by column,
    but when you look at the matrix elements like m[3][0], m[3][1], m[3][2], and m[3][3],
    you're indexing into it as if it were a row-major matrix.
    It is so because C 2D-array stores each column laid out horizontally, aka "transposed".

    To clarify, row-major order means that the elements of each row are stored in contiguous memory locations,
    and column-major means that the elements of each column are stored in contiguous memory locations.
    Example, for a 4x4 matrix:

    In row-major order:
        00 ~ 16, 16 ~ 32, 32 ~ 48, 48 ~ 64

        m[0][0], m[0][1], m[0][2], m[0][3]
        m[1][0], m[1][1], m[1][2], m[1][3]
        m[2][0], m[2][1], m[2][2], m[2][3]
        m[3][0], m[3][1], m[3][2], m[3][3]

        Laid out in a C 2D array memory:

        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1]
        m[0][2], m[1][2], m[2][2], m[3][2]
        m[0][3], m[1][3], m[2][3], m[3][3]

    In column-major order (used by OpenGL):
        00 ~ 16, 16 ~ 32, 32 ~ 48, 48 ~ 64

        m[0][0], m[1][0], m[2][0], m[3][0]
        m[0][1], m[1][1], m[2][1], m[3][1]
        m[0][2], m[1][2], m[2][2], m[3][2]
        m[0][3], m[1][3], m[2][3], m[3][3]

        Laid out in a C 2D array memory:

        m[0][0], m[0][1], m[0][2], m[0][3]
        m[1][0], m[1][1], m[1][2], m[1][3]
        m[2][0], m[2][1], m[2][2], m[2][3]
        m[3][0], m[3][1], m[3][2], m[3][3]

    So, if OpenGL places values in m[3][0], m[3][1], m[3][2], and m[3][3],
    it implies that those values are being indexed from a matrix that is stored column-major.
    But if you are treating it as a typical array of arrays in code (e.g., C-style 2D array),
    that would look like row-major memory storage.
*/

/**
    RenderWare uses 4x3, row-major affine matrices.
    
    This object defines a RenderWare Matrix object.
    
    The Matrix is heavily used throughout the API and a full range of functions are provided, including: rotation, multiplication, concatenation, scaling, translation, creation, destruction, stream read/write functions and a number of access functions to access particular vector data within the matrix.
    The "Right vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'x' axis in a right-handed, xyz coordinate system. 
    The "Up vector" is one of the vectors in a RenderWare Graphics matrix. The up vector corresponds to the 'y' axis in a right-handed, xyz coordinate system. 
    The "at vector" is one of the vectors in a RenderWare Graphics matrix. It corresponds to the 'z' axis in a right-handed, xyz coordinate system. 
    RenderWare Graphics uses a right-handed co-ordinate system. Therefore with z increasing into the display screen, and y increasing upwards, the x co-ordinate system increases towards the left.
    RenderWare, such as DirectX, uses row vectors and ends up with the much more natural: result = input * local_to_object * object_to_world. Your input is in local space, it gets transformed into object space before finally ending up in world space. Clean, clear, and readable. If you instead multiply the two matrices together on their own, you get the clear local_to_world = local_to_object * object_to_world instead of the awkward local_to_world = object_to_world * local_to_object you would get with OpenGL shaders and column vectors.

    These translations are often called:

    Surge, translation along the longitudinal axis (forward or backwards)
    Sway, translation along the transverse axis (from side to side)
    Heave, translation along the vertical axis (to move up or down).

    The corresponding rotations are often called:

    roll, about the longitudinal axis
    pitch, about the transverse axis
    yaw, about the vertical axis.
*/

/// Affine matrix (RenderWare matrix) significa que somente o conjunto 4x3 é considerado. (aka. não é uma projective matrix). [0][3] = [1][3] = [2][3] = 0; [3][3] = 1;
/// Linear matrix significa que somente o conjunto 3x3 é considerado. (aka. não é uma translation matrix).
/// atm  = affine transformation matrix;
/// ltm  = linear transformation matrix;
/// iltm = inverse linear transformation matrix;
/// atv  = affine transformation vector; positions and points
/// ltv  = linear transformation vector; normals, directions and axis
/// iwm  = inverse world matrix;
/// wm   = world matrix;
/// m    = matrix;
/// mm   = model matrix;
/// ptm  = projective transformation matrix;
/// v    = vector

/// Entendendo um pouco da natureza das matrices.
/// Row-major and column major refere-se a forma como os valores de uma matrix são arranjados, onde:
///     Em row-major, os vetores são arranjos por cada row inteiro.      ( aX, aY, aZ, aW, bX, bY, bZ, bW, ...)
///     Em column-major, os valores são arranjos por cada column inteira ( aX, bX, cX, dX, aY, bY, cY, dY, ...).
/// "Transposição" refere-se a forma de interação com ao valores da matrix, isto é, se isto é pré-multiplicado para cada row ou pós-multiplicado para cada columna.
///     Em column-major, nós pós-multiplicamos um (column) vector (a direta) para cada columna da matrix (a esquerda), isto é, M[][c] x V[c].
///     Em row-major, nós pré-multiplicamos um (row) vector (a esquerda) para cada columna da matrix (a direita), isto é, V[r] x M[r][].
///     Uma pós-multiplicação de um (column) vector por uma column-major matrix deve ser igual de uma pré-multiplicação de (row) vector por uma row-major matrix.
/// "Handedness" refere-se a forma como um sistema de coordenada considera a orientação.
///     Em left-handed, um valor positivo ao eixo X representa o quão a "esquerda" algo está.
///     Em right-handed, um valor positivo ao eixo X representa o quão a "direita" algo está.

// Qwadro is designed to use right-handed coordinate system and counterclockwise (CCW) orientation.
// This choice has been intended to favor our right-handed defenders coming down our towers to defend our base.

#ifndef AFX_MATRIX_H
#define AFX_MATRIX_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/coll/afxPlane.h"

#define AFX_M2D(c0r0, c0r1, \
                c1r0, c1r1) \
    (afxM2d) {  { (c0r0), (c0r1) }, \
                { (c1r0), (c1r1) } }

#define AFX_M3D(c0r0, c0r1, c0r2, \
                c1r0, c1r1, c1r2, \
                c2r0, c2r1, c2r2) \
    (afxM3d) {  { (c0r0), (c0r1), (c0r2) }, \
                { (c1r0), (c1r1), (c1r2) }, \
                { (c2r0), (c2r1), (c2r2) } }

#define AFX_M4D(c0r0, c0r1, c0r2, c0r3, \
                c1r0, c1r1, c1r2, c1r3, \
                c2r0, c2r1, c2r2, c2r3, \
                c3r0, c3r1, c3r2, c3r3) \
    (afxM4d) {  { (c0r0), (c0r1), (c0r2), (c0r3) }, \
                { (c1r0), (c1r1), (c1r2), (c1r3) }, \
                { (c2r0), (c2r1), (c2r2), (c2r3) }, \
                { (c3r0), (c3r1), (c3r2), (c3r3) } }

#define AFX_M4D_LINEAR( c0r0, c0r1, c0r2, \
                        c1r0, c1r1, c1r2, \
                        c2r0, c2r1, c2r2) \
    AFX_M4D((c0r0), (c0r1), (c0r2), 0, \
            (c1r0), (c1r1), (c1r2), 0, \
            (c2r0), (c2r1), (c2r2), 0, \
            0, 0, 0, 1)

#define AFX_M4D_AFFINE( c0r0, c0r1, c0r2, \
                        c1r0, c1r1, c1r2, \
                        c2r0, c2r1, c2r2, \
                        c3r0, c3r1, c3r2) \
    AFX_M4D((c0r0), (c0r1), (c0r2), 0, \
            (c1r0), (c1r1), (c1r2), 0, \
            (c2r0), (c2r1), (c2r2), 0, \
            (c3r0), (c3r1), (c3r2), 1)

#define AFX_M2D_DIAGONAL(c0r0, c1r1) \
    AFX_M2D((c0r0), 0, \
            0, (c1r1))

#define AFX_M3D_DIAGONAL(c0r0, c1r1, c2r2) \
    AFX_M3D((c0r0), 0, 0, \
            0, (c1r1), 0, \
            0, 0, (c2r2))

#define AFX_M4D_DIAGONAL(c0r0, c1r1, c2r2, c3r3) \
    AFX_M4D((c0r0), 0, 0, 0, \
            0, (c1r1), 0, 0, \
            0, 0, (c2r2), 0, \
            0, 0, 0, (c3r3))

#define AFX_M2D_IDENTITY AFX_M2D_DIAGONAL(1, 1)
#define AFX_M3D_IDENTITY AFX_M3D_DIAGONAL(1, 1, 1)
#define AFX_M4D_IDENTITY AFX_M4D_DIAGONAL(1, 1, 1, 1)

#define AFX_M2D_ZERO AFX_M2D(0, 0, 0, 0)
#define AFX_M3D_ZERO AFX_M3D(0, 0, 0, 0, 0, 0, 0, 0, 0)
#define AFX_M4D_ZERO AFX_M4D(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

AFXINL void     AfxM2dZero(afxM2d m);
AFXINL void     AfxM3dZero(afxM3d m);
AFXINL void     AfxM4dZero(afxM4d m);

AFXINL void     AfxM2dReset(afxM2d m);
AFXINL void     AfxM3dReset(afxM3d m);
AFXINL void     AfxM4dReset(afxM4d m);

AFXINL void     AfxM2dSet(afxM2d m, afxV2d const cx, afxV2d const cy);
AFXINL void     AfxM3dSet(afxM3d m, afxV3d const cx, afxV3d const cy, afxV3d const cz);
AFXINL void     AfxM4dSet(afxM4d m, afxV4d const cx, afxV4d const cy, afxV4d const cz, afxV4d const cw);
AFXINL void     AfxM4dSetLinear(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw);
AFXINL void     AfxM4dSetAffine(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw);

AFXINL void     AfxM2dDiagonal(afxM2d m, afxReal xx, afxReal yy);
AFXINL void     AfxM3dDiagonal(afxM3d m, afxReal xx, afxReal yy, afxReal zz);
AFXINL void     AfxM4dDiagonal(afxM4d m, afxReal xx, afxReal yy, afxReal zz, afxReal ww);

AFXINL void     AfxM2dSetTransposed(afxM2d m, afxV2d const rx, afxV2d const ry); // Be carefull using it in Qwadro
AFXINL void     AfxM3dSetTransposed(afxM3d m, afxV3d const rx, afxV3d const ry, afxV3d const rz); // Be carefull using it in Qwadro
AFXINL void     AfxM4dSetTransposed(afxM4d m, afxV4d const rx, afxV4d const ry, afxV4d const rz, afxV4d const rw);  // Be carefull using it in Qwadro
AFXINL void     AfxM4dSetLinearTransposed(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw);
AFXINL void     AfxM4dSetAffineTransposed(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw);

AFXINL void     AfxM4dEnsureLinear(afxM4d m); // make affine and zero translation.
AFXINL void     AfxM4dEnsureAffine(afxM4d m); // make affine.

AFXINL afxBool  AfxM2dIsIdentity(afxM2d const m);
AFXINL afxBool  AfxM3dIsIdentity(afxM3d const m);
AFXINL afxBool  AfxM4dIsIdentity(afxM4d const m);

AFXINL void     AfxM2dCopy(afxM2d m, afxM2d const in);
AFXINL void     AfxM3dCopy(afxM3d m, afxM3d const in);
AFXINL void     AfxM4dCopy(afxM4d m, afxM4d const in); // copy every row and column.
AFXINL void     AfxM3dCopyM2d(afxM3d m, afxM2d const in);
AFXINL void     AfxM4dCopyM2d(afxM4d m, afxM2d const in);
AFXINL void     AfxM2dCopyM3d(afxM2d m, afxM3d const in);
AFXINL void     AfxM4dCopyM3d(afxM4d m, afxM3d const ltm, afxV4d const atv);
AFXINL void     AfxM4dCopyLtm(afxM4d m, afxM4d const in); // copy only the 3x3
AFXINL void     AfxM4dCopyAtm(afxM4d m, afxM4d const in); // copy only the 4x3
AFXINL void     AfxM3dCopyM4d(afxM3d m, afxM4d const in);

AFXINL void     AfxV3d4Copy(afxV3d4 m, afxV3d4 const in);
AFXINL void     AfxV3d4CopyM3d(afxV3d4 m, afxM3d const in);
AFXINL void     AfxV3d4CopyM4d(afxV3d4 m, afxM4d const in);

AFXINL void     AfxM2dCopyTransposed(afxM2d m, afxM2d const in);
AFXINL void     AfxM3dCopyTransposed(afxM3d m, afxM3d const in);
AFXINL void     AfxM4dCopyTransposed(afxM4d m, afxM4d const in);
AFXINL void     AfxM4dCopyAtmTransposed(afxM4d m, afxM4d const in); // only consider 4x3; ignore W components.
AFXINL void     AfxM3dCopyM4dTransposed(afxM3d m, afxM4d const in); // ltm will be transposed
AFXINL void     AfxM4dCopyLtmTransposed(afxM4d m, afxM4d const in); // only consider 3x3; ignore W components and the W row.
AFXINL void     AfxM4dCopyM3dTransposed(afxM4d m, afxM3d const ltm, afxV4d const atv);

AFXINL afxReal  AfxM3dInvert(afxM3d m, afxM3d const in); // im = inverse of m
AFXINL afxReal  AfxM4dInvert(afxM4d m, afxM4d const in); // im = inverse of m
AFXINL afxReal  AfxM4dInvertAtm(afxM4d m, afxM4d const in); // im = inverse of m

// Computes the determinant of a matrix.
AFXINL afxReal  AfxM2dDet(afxM2d const m);
AFXINL afxReal  AfxM3dDet(afxM3d const m);
AFXINL afxReal  AfxM4dDet(afxM4d const m);

AFXINL void     AfxM2dSwap(afxM2d m, afxM2d other);
AFXINL void     AfxM3dSwap(afxM3d m, afxM3d other);
AFXINL void     AfxM4dSwap(afxM4d m, afxM4d other);

AFXINL void     AfxM2dAdd(afxM2d m, afxM2d const a, afxM2d const b); // m = a + b
AFXINL void     AfxM3dAdd(afxM3d m, afxM3d const a, afxM3d const b); // m = a + b
AFXINL void     AfxM4dAdd(afxM4d m, afxM4d const a, afxM4d const b); // m = a + b

AFXINL void     AfxM2dSub(afxM2d m, afxM2d const a, afxM2d const b); // m = a - b
AFXINL void     AfxM3dSub(afxM3d m, afxM3d const a, afxM3d const b); // m = a - b
AFXINL void     AfxM4dSub(afxM4d m, afxM4d const a, afxM4d const b); // m = a - b

AFXINL void     AfxM2dDiff(afxM2d m, afxM2d const a, afxM2d const b); // m = a - b
AFXINL void     AfxM3dDiff(afxM3d m, afxM3d const a, afxM3d const b); // m = a - b
AFXINL void     AfxM4dDiff(afxM4d m, afxM4d const a, afxM4d const b); // m = a - b

AFXINL void     AfxM2dScale(afxM2d m, afxM2d const in, afxReal scale);
AFXINL void     AfxM3dScale(afxM3d m, afxM3d const in, afxReal scale);
AFXINL void     AfxM4dScale(afxM4d m, afxM4d const in, afxReal scale);

AFXINL void     AfxM2dMads(afxM2d m, afxM2d const add, afxM2d const mul, afxReal scale);
AFXINL void     AfxM3dMads(afxM3d m, afxM3d const add, afxM3d const mul, afxReal scale);
AFXINL void     AfxM4dMads(afxM4d m, afxM4d const add, afxM4d const mul, afxReal scale);

////////////////////////////////////////////////////////////////////////////////
// Affine transformation matrix methods                                       //
////////////////////////////////////////////////////////////////////////////////

// Builds a matrix that scales along the x-axis, y-axis, and z-axis.
AFXINL void         AfxM3dScaling(afxM3d m, afxV3d const scale);
AFXINL void         AfxM4dScaling(afxM4d m, afxV3d const scale);

// Builds a rotation matrix from a quaternion.
AFXINL void         AfxM3dRotationQuat(afxM3d m, afxQuat const q);
AFXINL void         AfxM4dRotationQuat(afxM4d m, afxQuat const q);

// Builds a matrix that rotates around an arbitrary axis.
AFXINL void         AfxM3dRotationAxis(afxM3d m, afxV3d const axis, afxReal /*theta*/radians);
AFXINL void         AfxM4dRotationAxis(afxM4d m, afxV3d const axis, afxReal /*theta*/radians);

// Builds a rotation matrix based on a given pitch, yaw, and roll (Euler angles).
AFXINL void         AfxM4dRotationEuler(afxM4d m, afxReal pitch, afxReal yaw, afxReal roll);

// Builds a matrix that rotates around the x-axis, y-axis or z-axis.
// Angles are measured clockwise when looking along the rotation axis toward the origin.
AFXINL void         AfxM4dRotationX(afxM4d m, afxReal angle);
AFXINL void         AfxM4dRotationY(afxM4d m, afxReal angle);
AFXINL void         AfxM4dRotationZ(afxM4d m, afxReal angle);

AFXINL void         AfxM4dTranslation
// Builds a translation matrix from the specified vector describing the translations along the x-axis, y-axis, and z-axis.
(
    afxM4d          m, 
    afxV3d const    translation // vector describing the translations along the x-axis, y-axis, and z-axis.
);

// PolarDecompose

// Qwadro expects all animation tracks to be decomposed. 
// Position, orientation, and scale/shear are stored separately. 
// The exporters kick out data in this format, but if you're writing your own important or processing tool, you may only have matrices as input. 
// Decomposing an arbitrary 3x3 matrix into its rotational and scale/shear components can be difficult, so Qwadro provides a function for doing this: 

AFXINL afxBool      AfxM3dPolarDecompose(afxM3d const m, afxReal tol, afxM3d rm, afxM3d ssm);

// Compose

AFXINL void         AfxM4dComposeTransformation
// Builds a transformation matrix.
(
    afxM4d          m, 
    // Vector describing the center of the scaling.
    afxV3d const    scalPivot,
    // Quaternion describing the orientation of the scaling.
    afxQuat const   scalOrient,
    // Vector containing the scaling factors for the x-axis, y-axis, and z-axis.
    afxV3d const    scaling,
    // Vector describing the center of the rotation.
    afxV3d const    rotPivot,
    // Quaternion describing the rotation around the origin indicated by rotOrigin.
    afxQuat const   rotQuat,
    // Vector describing the translations along the x-axis, y-axis, and z-axis.
    afxV3d const    translation
);

AFXINL void         AfxM4dComposeAffineTransformation
// Builds an affine transformation matrix.
(
    afxM4d          m, 
    // Vector of scaling factors for each dimension.
    afxV3d const    scale,
    // Point identifying the center of rotation.
    afxV3d const    rotPivot,
    // Rotation factors.
    afxQuat const   rot,
    // Translation offsets.
    afxV3d const    translation
);

// Assimilate

AFXINL void     AfxAssimilateLtm3d(afxM3d const ltm, afxM3d const iltm, afxUnit cnt, afxM3d const in[], afxM3d out[]); // make similarity transformation on afxM3d-based scale/shear.
AFXINL void     AfxAssimilateAtm4d(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxM4d const in[], afxM4d out[]);

AFXINL void     AfxM4dRigidMotion(afxM4d m, afxM4d const mm, afxV3d const rotation, afxV3d const translation);

#endif//AFX_MATRIX_H
