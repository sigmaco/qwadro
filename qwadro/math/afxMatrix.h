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
    (afxM2d) {  .m = { { (c0r0), (c0r1) }, \
                { (c1r0), (c1r1) } } }

#define AFX_M3D(c0r0, c0r1, c0r2, \
                c1r0, c1r1, c1r2, \
                c2r0, c2r1, c2r2) \
    (afxM3d) {  .m = { { (c0r0), (c0r1), (c0r2) }, \
                { (c1r0), (c1r1), (c1r2) }, \
                { (c2r0), (c2r1), (c2r2) } } }

#define AFX_M4D(c0r0, c0r1, c0r2, c0r3, \
                c1r0, c1r1, c1r2, c1r3, \
                c2r0, c2r1, c2r2, c2r3, \
                c3r0, c3r1, c3r2, c3r3) \
    (afxM4d) {  .m = { { (c0r0), (c0r1), (c0r2), (c0r3) }, \
                  { (c1r0), (c1r1), (c1r2), (c1r3) }, \
                  { (c2r0), (c2r1), (c2r2), (c2r3) }, \
                  { (c3r0), (c3r1), (c3r2), (c3r3) } } }

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

AFXINL afxM2d     AfxM2dZero(void);
AFXINL afxM3d     AfxM3dZero(void);
AFXINL afxM4d     AfxM4dZero(void);

AFXINL afxM2d     AfxM2dIdentity(void);
AFXINL afxM3d     AfxM3dIdentity(void);
AFXINL afxM4d     AfxM4dIdentity(void);

AFXINL afxM2d     AfxM2dMake(afxV2d const cx, afxV2d const cy);
AFXINL afxM3d     AfxM3dMake(afxV3d const cx, afxV3d const cy, afxV3d const cz);
AFXINL afxM4d     AfxM4dMake(afxV4d const cx, afxV4d const cy, afxV4d const cz, afxV4d const cw);
AFXINL afxM4d     AfxM4dMakeLtm(afxV3d const cx, afxV3d const cy, afxV3d const cz);
AFXINL afxM4d     AfxM4dMakeAtm(afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw);

AFXINL afxM2d     AfxM2dMakeTransposed(afxV2d const rx, afxV2d const ry); // Be carefull using it in Qwadro
AFXINL afxM3d     AfxM3dMakeTransposed(afxV3d const rx, afxV3d const ry, afxV3d const rz); // Be carefull using it in Qwadro
AFXINL afxM4d     AfxM4dMakeTransposed(afxV4d const rx, afxV4d const ry, afxV4d const rz, afxV4d const rw);  // Be carefull using it in Qwadro
AFXINL afxM4d     AfxM4dMakeAtmTransposed(afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw);

AFXINL afxM2d     AfxM2dDiagonal(afxReal xx, afxReal yy);
AFXINL afxM3d     AfxM3dDiagonal(afxReal xx, afxReal yy, afxReal zz);
AFXINL afxM4d     AfxM4dDiagonal(afxReal xx, afxReal yy, afxReal zz, afxReal ww);

AFXINL afxBool  AfxM2dIsIdentity(afxM2d const m);
AFXINL afxBool  AfxM3dIsIdentity(afxM3d const m);
AFXINL afxBool  AfxM4dIsIdentity(afxM4d const m);

AFXINL afxM3d     AfxM3dFromM2d(afxM2d const in);
AFXINL afxM4d     AfxM4dFromM2d(afxM2d const in);
AFXINL afxM2d     AfxM2dFromM3d(afxM3d const in);
AFXINL afxM4d     AfxM4dFromM3d(afxM3d const ltm, afxV4d const atv);
AFXINL afxM4d     AfxM4dFromLtm(afxM4d const in); // copy only the 3x3
AFXINL afxM4d     AfxM4dFromAtm(afxM4d const in); // copy only the 4x3
AFXINL afxM3d     AfxM3dFromM4d(afxM4d const in);

AFXINL afxM43d     AfxM43dFromM3d(afxM3d const in);
AFXINL afxM43d     AfxM43dFromM4d(afxM4d const in);
AFXINL afxM4d       AfxM4dFromM43d(afxM43d const in);

AFXINL afxM2d     AfxM2dTranspose(afxM2d const in);
AFXINL afxM3d     AfxM3dTranspose(afxM3d const in);
AFXINL afxM4d     AfxM4dTranspose(afxM4d const in);
AFXINL afxM4d     AfxM4dTransposeAtm(afxM4d const in); // only consider 4x3; ignore W components.
AFXINL afxM3d     AfxM3dTransposeM4d(afxM4d const in); // ltm will be transposed
AFXINL afxM4d     AfxM4dTransposeLtm(afxM4d const in); // only consider 3x3; ignore W components and the W row.
AFXINL afxM4d     AfxM4dTransposeM3d(afxM3d const ltm, afxV4d const atv);

AFXINL afxM3d  AfxM3dInvert(afxM3d const in, afxReal* determinant); // im = inverse of m
AFXINL afxM4d  AfxM4dInvert(afxM4d const in, afxReal* determinant); // im = inverse of m
AFXINL afxM4d  AfxM4dInvertAtm(afxM4d const in, afxReal* determinant); // im = inverse of m

// Computes the determinant of a matrix.
AFXINL afxReal  AfxM2dDet(afxM2d const m);
AFXINL afxReal  AfxM3dDet(afxM3d const m);
AFXINL afxReal  AfxM4dDet(afxM4d const m);

AFXINL afxM2d     AfxM2dAdd(afxM2d const a, afxM2d const b); // m = a + b
AFXINL afxM3d     AfxM3dAdd(afxM3d const a, afxM3d const b); // m = a + b
AFXINL afxM4d     AfxM4dAdd(afxM4d const a, afxM4d const b); // m = a + b

AFXINL afxM2d     AfxM2dSub(afxM2d const a, afxM2d const b); // m = a - b
AFXINL afxM3d     AfxM3dSub(afxM3d const a, afxM3d const b); // m = a - b
AFXINL afxM4d     AfxM4dSub(afxM4d const a, afxM4d const b); // m = a - b

AFXINL afxM2d     AfxM2dDiff(afxM2d const a, afxM2d const b); // m = a - b
AFXINL afxM3d     AfxM3dDiff(afxM3d const a, afxM3d const b); // m = a - b
AFXINL afxM4d     AfxM4dDiff(afxM4d const a, afxM4d const b); // m = a - b

AFXINL afxM2d     AfxM2dScale(afxM2d const in, afxReal scale);
AFXINL afxM3d     AfxM3dScale(afxM3d const in, afxReal scale);
AFXINL afxM4d     AfxM4dScale(afxM4d const in, afxReal scale);

AFXINL afxM2d     AfxM2dMads(afxM2d const add, afxM2d const mul, afxReal scale);
AFXINL afxM3d     AfxM3dMads(afxM3d const add, afxM3d const mul, afxReal scale);
AFXINL afxM4d     AfxM4dMads(afxM4d const add, afxM4d const mul, afxReal scale);

// DOFs

AFXINL afxM3d AfxM3dDofX(afxM3d const m);

AFXINL afxM3d AfxM3dDofY(afxM3d const m);

AFXINL afxM3d AfxM3dDofZ(afxM3d const m);

AFXINL afxM3d AfxM3dDofXY(afxM3d const m);

AFXINL afxM3d AfxM3dDofXZ(afxM3d const m);

AFXINL afxM3d AfxM3dDofYZ(afxM3d const m);


#endif//AFX_MATRIX_H
