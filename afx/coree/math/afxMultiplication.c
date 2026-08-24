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
// MathFX files should be inline, but we put they in translation units to avoid recompilation.

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/afxStream.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

// PostMultiply
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

_AFXINL afxV2d AfxV2dPostMultiplyM2d(afxM2d const m, afxV2d const t)
{
    afxError err = { 0 };

    return AFX_V2D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]), // column 0 * vector
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1])); // column 1 * vector
}

_AFXINL afxV3d AfxV3dPostMultiplyM3d(afxM3d const m, afxV3d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V3D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2])); // column 2 * vector
}

_AFXINL afxV4d AfxV4dPostMultiplyM4d(afxM4d const m, afxV4d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V4D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]) + (m.m[0][3] * t.v[3]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]) + (m.m[1][3] * t.v[3]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2]) + (m.m[2][3] * t.v[3]), // column 2 * vector    
                    (m.m[3][0] * t.v[0]) + (m.m[3][1] * t.v[1]) + (m.m[3][2] * t.v[2]) + (m.m[3][3] * t.v[3])); // column 3 * vector
}

_AFXINL afxV3d AfxV3dPostMultiplyLtm4d(afxM4d const m, afxV3d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V3D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2])); // column 2 * vector
}

_AFXINL afxV4d AfxV4dPostMultiplyM3d(afxM3d const m, afxV4d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

    // Compatible with TransposeVectorTransform4x3(in/v, 0.0, m)

    return AFX_V4D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2]), // column 2 * vector
                    t.v[3]);
}

_AFXINL afxV3d TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform)
{
    afxError err = { 0 };
    AfxThrowError();
    // Compatible with TransposeVectorTransform4x3

    Dest.v[0] = Transform.m[0][0] * Dest.v[0] + Transform.m[0][1] * Dest.v[1] + Transform.m[0][2] * Dest.v[2] + D3 * Transform.m[3][0];
    Dest.v[1] = Transform.m[1][0] * Dest.v[0] + Transform.m[1][1] * Dest.v[1] + Transform.m[1][2] * Dest.v[2] + D3 * Transform.m[3][1];
    Dest.v[2] = Transform.m[2][0] * Dest.v[0] + Transform.m[2][1] * Dest.v[1] + Transform.m[2][2] * Dest.v[2] + D3 * Transform.m[3][2];

    return Dest;
}

_AFXINL afxV2d AfxV2dPostMultiplyAtm4d(afxM4d const m, afxV2d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V2D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2]) + (m.m[0][3]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2]) + (m.m[1][3])); // column 1 * vector
}

_AFXINL afxV3d AfxV3dPostMultiplyAtm4d(afxM4d const m, afxV3d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V3D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]) + (m.m[0][3]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]) + (m.m[1][3]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2]) + (m.m[2][3])); // column 2 * vector
}

_AFXINL afxV4d AfxV4dPostMultiplyAtm4d(afxM4d const m, afxV4d const t)
{
    afxError err = { 0 };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    return AFX_V4D( (m.m[0][0] * t.v[0]) + (m.m[0][1] * t.v[1]) + (m.m[0][2] * t.v[2]) + (m.m[0][3]), // column 0 * vector    
                    (m.m[1][0] * t.v[0]) + (m.m[1][1] * t.v[1]) + (m.m[1][2] * t.v[2]) + (m.m[1][3]), // column 1 * vector    
                    (m.m[2][0] * t.v[0]) + (m.m[2][1] * t.v[1]) + (m.m[2][2] * t.v[2]) + (m.m[2][3]), // column 2 * vector
                    1.f);
}

// PreMultiply

_AFXINL afxV2d AfxV2dPreMultiplyM2d(afxV2d const t, afxM2d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V2D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1])); // vector * row 1
}

_AFXINL afxV3d AfxV3dPreMultiplyM3d(afxV3d const t, afxM3d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V3D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]), // vector * row 1    
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2])); // vector * row 2
}

_AFXINL afxV4d AfxV4dPreMultiplyM4d(afxV4d const t, afxM4d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // Compatible with TransposeVectorTransform4x4(v/in, m)

    return AFX_V4D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]) + (t.v[3] * m.m[3][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]) + (t.v[3] * m.m[3][1]), // vector * row 1    
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2]) + (t.v[3] * m.m[3][2]), // vector * row 2    
                    (t.v[0] * m.m[0][3]) + (t.v[1] * m.m[1][3]) + (t.v[2] * m.m[2][3]) + (t.v[3] * m.m[3][3])); // vector * row 3
}

// TransformTransposedPoint/Normal/Affine

_AFXINL afxV3d AfxV3dPreMultiplyLtm4d(afxV3d const t, afxM4d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V3D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]), // vector * row 1    
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2])); // vector * row 2
}

_AFXINL afxV4d AfxV4dPreMultiplyM3d(afxV4d const t, afxM3d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V4D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]), // vector * row 1    
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2]), // vector * row 2
                    t.v[3]);
}

_AFXINL afxV2d AfxV2dPreMultiplyAtm4d(afxV2d const t, afxM4d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V2D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (m.m[2][0]) + (m.m[3][0]), // vector * row 0
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (m.m[2][1]) + (m.m[3][1])); // vector * row 1
}

_AFXINL afxV3d AfxV3dPreMultiplyAtm4d(afxV3d const t, afxM4d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V3D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]) + (m.m[3][0]), // vector * row 0    
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]) + (m.m[3][1]), // vector * row 1    
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2]) + (m.m[3][2])); // vector * row 2
}

_AFXINL afxV4d AfxV4dPreMultiplyAtm4d(afxV4d const t, afxM4d const m)
{
    afxError err = { 0 };

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    return AFX_V4D( (t.v[0] * m.m[0][0]) + (t.v[1] * m.m[1][0]) + (t.v[2] * m.m[2][0]) + (m.m[3][0]), // vector * row 0
                    (t.v[0] * m.m[0][1]) + (t.v[1] * m.m[1][1]) + (t.v[2] * m.m[2][1]) + (m.m[3][1]), // vector * row 1
                    (t.v[0] * m.m[0][2]) + (t.v[1] * m.m[1][2]) + (t.v[2] * m.m[2][2]) + (m.m[3][2]), // vector * row 2
                    1.f);
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxQuat AfxQuatMultiply(afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };

#if 0

    // Should be compatible with XMVECTOR XMQuaternionMultiply(FXMVECTOR Q1, FXMVECTOR Q2)

    // The result represents the rotation A followed by the rotation B to be consistent with 
    // MatrixMulplity concatenation since this function is typically used to concatenate quaternions that 
    // represent rotations (i.e. it returns B*A).

    // Returns the product B*A (which is the concatenation of a rotation A followed by the rotation B)
    
    afxQuat q;
    afxQuat const t = { b.v[0], b.v[1], b.v[2], b.v[3] };
    q.v[0] = (t.v[3] * a.v[0]) + (t.v[0] * a.v[3]) + (t.v[1] * a.v[2]) - (t.v[2] * a.v[1]);
    q.v[1] = (t.v[3] * a.v[1]) - (t.v[0] * a.v[2]) + (t.v[1] * a.v[3]) + (t.v[2] * a.v[0]);
    q.v[2] = (t.v[3] * a.v[2]) + (t.v[0] * a.v[1]) - (t.v[1] * a.v[0]) + (t.v[2] * a.v[3]);
    q.v[3] = (t.v[3] * a.v[3]) - (t.v[0] * a.v[0]) - (t.v[1] * a.v[1]) - (t.v[2] * a.v[2]);
    return q;
#else

    // Compatible with QuaternionMultiply4(q, a, b)

    afxQuat q;
    afxQuat const t = AFX_QUAT( b.v[0], b.v[1], b.v[2], b.v[3] );
    q.v[0] = (t.v[3] * a.v[0]) + ((t.v[0] * a.v[3]) + (t.v[2] * a.v[1]) - (t.v[1] * a.v[2]));
    q.v[1] = (t.v[1] * a.v[3]) + (t.v[0] * a.v[2]) - (t.v[2] * a.v[0]) + (t.v[3] * a.v[1]);
    q.v[2] = (t.v[3] * a.v[2]) + ((t.v[2] * a.v[3]) + ((t.v[1] * a.v[0]) - (t.v[0] * a.v[1])));
    q.v[3] = (t.v[3] * a.v[3]) - ((t.v[2] * a.v[2]) + (t.v[1] * a.v[1]) + (t.v[0] * a.v[0]));
    return q;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX                                                                     //
////////////////////////////////////////////////////////////////////////////////

// In post-multiplication, each component of the result, at in given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

// MultiplyTransposed

_AFXINL afxM2d AfxM2dMultiply(afxM2d const in, afxM2d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM2d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]); // colum 0 * row 0    
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]); // colum 0 * row 1
    
    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]); // colum 1 * row 0    
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]); // colum 1 * row 1
    return m;
}

_AFXINL afxM2d AfxM2dMultiplyTransposed(afxM2d const in, afxM2d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM2d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]); // colum 0 * row 0    
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]); // colum 1 * row 0
    
    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]); // colum 1 * row 1
    return m;
}

_AFXINL afxM3d AfxM3dMultiply(afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };

    // Compatible with MatrixMultiply3x3(m, in, mul)

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM3d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[0][2] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2

    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2

    m.m[2][0] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[2][1] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    return m;
}

_AFXINL afxM3d AfxM3dMultiplyTransposed(afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM3d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[0][2] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0

    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1

    m.m[2][0] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[2][1] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    return m;
}

_AFXINL afxM3d TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix)
{
    afxError err = { 0 };
    AfxThrowError();
    // Compatible with TransposeMatrixMultiply3x3(out, in, mul)

// 0 1 2, 3 4 5, 6 7 8

    // This function takes a already transposed matrix while AfxM3dMultiplyTransposed computes a new one. 

    IntoMatrix.m[0][0] = TransposedMatrix.m[0][0] * ByMatrix.m[0][0] + TransposedMatrix.m[1][0] * ByMatrix.m[1][0] + TransposedMatrix.m[2][0] * ByMatrix.m[2][0];
    IntoMatrix.m[0][1] = TransposedMatrix.m[0][0] * ByMatrix.m[0][1] + TransposedMatrix.m[1][0] * ByMatrix.m[1][1] + TransposedMatrix.m[2][0] * ByMatrix.m[2][1];
    IntoMatrix.m[0][2] = TransposedMatrix.m[0][0] * ByMatrix.m[0][2] + TransposedMatrix.m[1][0] * ByMatrix.m[1][2] + TransposedMatrix.m[2][0] * ByMatrix.m[2][2];

    IntoMatrix.m[1][0] = TransposedMatrix.m[0][1] * ByMatrix.m[0][0] + TransposedMatrix.m[1][1] * ByMatrix.m[1][0] + TransposedMatrix.m[2][1] * ByMatrix.m[2][0];
    IntoMatrix.m[1][1] = TransposedMatrix.m[0][1] * ByMatrix.m[0][1] + TransposedMatrix.m[1][1] * ByMatrix.m[1][1] + TransposedMatrix.m[2][1] * ByMatrix.m[2][1];
    IntoMatrix.m[1][2] = TransposedMatrix.m[0][1] * ByMatrix.m[0][2] + TransposedMatrix.m[1][1] * ByMatrix.m[1][2] + TransposedMatrix.m[2][1] * ByMatrix.m[2][2];
    
    IntoMatrix.m[2][0] = TransposedMatrix.m[0][2] * ByMatrix.m[0][0] + TransposedMatrix.m[1][2] * ByMatrix.m[1][0] + TransposedMatrix.m[2][2] * ByMatrix.m[2][0];
    IntoMatrix.m[2][1] = TransposedMatrix.m[0][2] * ByMatrix.m[0][1] + TransposedMatrix.m[1][2] * ByMatrix.m[1][1] + TransposedMatrix.m[2][2] * ByMatrix.m[2][1];
    IntoMatrix.m[2][2] = TransposedMatrix.m[0][2] * ByMatrix.m[0][2] + TransposedMatrix.m[1][2] * ByMatrix.m[1][2] + TransposedMatrix.m[2][2] * ByMatrix.m[2][2];
    return IntoMatrix;
}

_AFXINL afxM4d AfxM4dMultiply(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Compatible with m = XMMatrixMultiply(in, mul)
    // Compatible with ColumnMatrixMultiply4x4(m, in, mul)

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]) + (in.m[0][3] * mul.m[3][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]) + (in.m[0][3] * mul.m[3][1]); // colum 0 * row 1
    m.m[0][2] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]) + (in.m[0][3] * mul.m[3][2]); // colum 0 * row 2
    m.m[0][3] = (in.m[0][0] * mul.m[0][3]) + (in.m[0][1] * mul.m[1][3]) + (in.m[0][2] * mul.m[2][3]) + (in.m[0][3] * mul.m[3][3]); // colum 0 * row 3

    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]) + (in.m[1][3] * mul.m[3][0]); // colum 1 * row 0
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]) + (in.m[1][3] * mul.m[3][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]) + (in.m[1][3] * mul.m[3][2]); // colum 1 * row 2
    m.m[1][3] = (in.m[1][0] * mul.m[0][3]) + (in.m[1][1] * mul.m[1][3]) + (in.m[1][2] * mul.m[2][3]) + (in.m[1][3] * mul.m[3][3]); // colum 1 * row 3
    
    m.m[2][0] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]) + (in.m[2][3] * mul.m[3][0]); // colum 2 * row 0
    m.m[2][1] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]) + (in.m[2][3] * mul.m[3][1]); // colum 2 * row 1
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]) + (in.m[2][3] * mul.m[3][2]); // colum 2 * row 2
    m.m[2][3] = (in.m[2][0] * mul.m[0][3]) + (in.m[2][1] * mul.m[1][3]) + (in.m[2][2] * mul.m[2][3]) + (in.m[2][3] * mul.m[3][3]); // colum 2 * row 3

    m.m[3][0] = (in.m[3][0] * mul.m[0][0]) + (in.m[3][1] * mul.m[1][0]) + (in.m[3][2] * mul.m[2][0]) + (in.m[3][3] * mul.m[3][0]); // colum 3 * row 0
    m.m[3][1] = (in.m[3][0] * mul.m[0][1]) + (in.m[3][1] * mul.m[1][1]) + (in.m[3][2] * mul.m[2][1]) + (in.m[3][3] * mul.m[3][1]); // colum 3 * row 1
    m.m[3][2] = (in.m[3][0] * mul.m[0][2]) + (in.m[3][1] * mul.m[1][2]) + (in.m[3][2] * mul.m[2][2]) + (in.m[3][3] * mul.m[3][2]); // colum 3 * row 2
    m.m[3][3] = (in.m[3][0] * mul.m[0][3]) + (in.m[3][1] * mul.m[1][3]) + (in.m[3][2] * mul.m[2][3]) + (in.m[3][3] * mul.m[3][3]); // colum 3 * row 3
    return m;
}

_AFXINL afxM4d AfxM4dMultiplyTransposed(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Compatible with m = XMMatrixMultiplyTranspose(in, mul)

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]) + (in.m[0][3] * mul.m[3][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]) + (in.m[1][3] * mul.m[3][0]); // colum 1 * row 0
    m.m[0][2] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]) + (in.m[2][3] * mul.m[3][0]); // colum 2 * row 0
    m.m[0][3] = (in.m[3][0] * mul.m[0][0]) + (in.m[3][1] * mul.m[1][0]) + (in.m[3][2] * mul.m[2][0]) + (in.m[3][3] * mul.m[3][0]); // colum 3 * row 0

    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]) + (in.m[0][3] * mul.m[3][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]) + (in.m[1][3] * mul.m[3][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]) + (in.m[2][3] * mul.m[3][1]); // colum 2 * row 1
    m.m[1][3] = (in.m[3][0] * mul.m[0][1]) + (in.m[3][1] * mul.m[1][1]) + (in.m[3][2] * mul.m[2][1]) + (in.m[3][3] * mul.m[3][1]); // colum 3 * row 1

    m.m[2][0] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]) + (in.m[0][3] * mul.m[3][2]); // colum 0 * row 2
    m.m[2][1] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]) + (in.m[1][3] * mul.m[3][2]); // colum 1 * row 2
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]) + (in.m[2][3] * mul.m[3][2]); // colum 2 * row 2
    m.m[2][3] = (in.m[3][0] * mul.m[0][2]) + (in.m[3][1] * mul.m[1][2]) + (in.m[3][2] * mul.m[2][2]) + (in.m[3][3] * mul.m[3][2]); // colum 3 * row 2

    m.m[3][0] = (in.m[0][0] * mul.m[0][3]) + (in.m[0][1] * mul.m[1][3]) + (in.m[0][2] * mul.m[2][3]) + (in.m[0][3] * mul.m[3][3]); // colum 0 * row 3
    m.m[3][1] = (in.m[1][0] * mul.m[0][3]) + (in.m[1][1] * mul.m[1][3]) + (in.m[1][2] * mul.m[2][3]) + (in.m[1][3] * mul.m[3][3]); // colum 1 * row 3
    m.m[3][2] = (in.m[2][0] * mul.m[0][3]) + (in.m[2][1] * mul.m[1][3]) + (in.m[2][2] * mul.m[2][3]) + (in.m[2][3] * mul.m[3][3]); // colum 2 * row 3
    m.m[3][3] = (in.m[3][0] * mul.m[0][3]) + (in.m[3][1] * mul.m[1][3]) + (in.m[3][2] * mul.m[2][3]) + (in.m[3][3] * mul.m[3][3]); // colum 3 * row 3
    return m;
}

// MultiplyPlanar/Linear/Affine

_AFXINL afxM3d AfxM3dMultiplyAtm(afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM3d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[0][2] = 0.f;

    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = 0.f;

    m.m[2][0] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[2][1] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[2][2] = 1.f;
    return m;
}

_AFXINL afxM3d AfxM3dMultiplyAtmTransposed(afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM3d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[0][2] = 0.f;

    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = 0.f;

    m.m[2][0] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[2][1] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[2][2] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dMultiplyLtm(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[0][2] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[0][3] = 0.f;

    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[1][3] = 0.f;

    m.m[2][0] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[2][1] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dMultiplyLtmTransposed(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[0][2] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[0][3] = 0.f;

    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[1][3] = 0.f;

    m.m[2][0] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[2][1] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dMultiplyAtm(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Compatible with ColumnMatrixMultiply4x3(m, in, mul)

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in.v[row][] * mul[][i]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[0][2] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[0][3] = 0.f;

    m.m[1][0] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[1][3] = 0.f;

    m.m[2][0] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[2][1] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    m.m[2][3] = 0.f;

    m.m[3][0] = (in.m[3][0] * mul.m[0][0]) + (in.m[3][1] * mul.m[1][0]) + (in.m[3][2] * mul.m[2][0]) + mul.m[3][0]; // colum 3 * row 0
    m.m[3][1] = (in.m[3][0] * mul.m[0][1]) + (in.m[3][1] * mul.m[1][1]) + (in.m[3][2] * mul.m[2][1]) + mul.m[3][1]; // colum 3 * row 1
    m.m[3][2] = (in.m[3][0] * mul.m[0][2]) + (in.m[3][1] * mul.m[1][2]) + (in.m[3][2] * mul.m[2][2]) + mul.m[3][2]; // colum 3 * row 2
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dMultiplyAtmTransposed(afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };

    // Cut off in.v[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in.v[i][] * mul[][col]

    afxM4d m;
    m.m[0][0] = (in.m[0][0] * mul.m[0][0]) + (in.m[0][1] * mul.m[1][0]) + (in.m[0][2] * mul.m[2][0]); // colum 0 * row 0
    m.m[0][1] = (in.m[1][0] * mul.m[0][0]) + (in.m[1][1] * mul.m[1][0]) + (in.m[1][2] * mul.m[2][0]); // colum 1 * row 0
    m.m[0][2] = (in.m[2][0] * mul.m[0][0]) + (in.m[2][1] * mul.m[1][0]) + (in.m[2][2] * mul.m[2][0]); // colum 2 * row 0
    m.m[0][3] = 0.f;

    m.m[1][0] = (in.m[0][0] * mul.m[0][1]) + (in.m[0][1] * mul.m[1][1]) + (in.m[0][2] * mul.m[2][1]); // colum 0 * row 1
    m.m[1][1] = (in.m[1][0] * mul.m[0][1]) + (in.m[1][1] * mul.m[1][1]) + (in.m[1][2] * mul.m[2][1]); // colum 1 * row 1
    m.m[1][2] = (in.m[2][0] * mul.m[0][1]) + (in.m[2][1] * mul.m[1][1]) + (in.m[2][2] * mul.m[2][1]); // colum 2 * row 1
    m.m[1][3] = 0.f;

    m.m[2][0] = (in.m[0][0] * mul.m[0][2]) + (in.m[0][1] * mul.m[1][2]) + (in.m[0][2] * mul.m[2][2]); // colum 0 * row 2
    m.m[2][1] = (in.m[1][0] * mul.m[0][2]) + (in.m[1][1] * mul.m[1][2]) + (in.m[1][2] * mul.m[2][2]); // colum 1 * row 2
    m.m[2][2] = (in.m[2][0] * mul.m[0][2]) + (in.m[2][1] * mul.m[1][2]) + (in.m[2][2] * mul.m[2][2]); // colum 2 * row 2
    m.m[2][3] = 0.f;

    m.m[3][0] = (in.m[0][0] * mul.m[0][3]) + (in.m[0][1] * mul.m[1][3]) + (in.m[0][2] * mul.m[2][3]) + in.m[0][3]; // colum 0 * row 3
    m.m[3][1] = (in.m[1][0] * mul.m[0][3]) + (in.m[1][1] * mul.m[1][3]) + (in.m[1][2] * mul.m[2][3]) + in.m[1][3]; // colum 1 * row 3
    m.m[3][2] = (in.m[2][0] * mul.m[0][3]) + (in.m[2][1] * mul.m[1][3]) + (in.m[2][2] * mul.m[2][3]) + in.m[2][3]; // colum 2 * row 3
    m.m[3][3] = 1.f;
    return m;
}

// Multiply

// MultiplyArrayedV2d

_AFXINL void AfxM2dPostMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV2dPostMultiplyM2d(m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPostMultiplyM3d(m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPostMultiplyM4d(m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPostMultiplyLtm4d(m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPostMultiplyAtm4d(m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPostMultiplyM3d(m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPostMultiplyAtm4d(m, in[i]);
}

// PreMultiply

_AFXINL void AfxM2dPreMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV2dPreMultiplyM2d(in[i], m);
}

_AFXINL void AfxM3dPreMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPreMultiplyM3d(in[i], m);
}

_AFXINL void AfxM4dPreMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPreMultiplyM4d(in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPreMultiplyAtm4d(in[i], m);
}

_AFXINL void AfxM3dPreMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPreMultiplyM3d(in[i], m);
}

_AFXINL void AfxM4dPreMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dPreMultiplyLtm4d(in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV4dPreMultiplyAtm4d(in[i], m);
}

// TransformSerialized

_AFXINL void AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxUnit cnt, afxV2d const in[], afxUnit inStride, afxV2d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV2d*)outPtr = AfxV2dPostMultiplyM2d(m, *(afxV2d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV3d*)outPtr = AfxV3dPostMultiplyM3d(m, *(afxV3d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV4d*)outPtr = AfxV4dPostMultiplyM4d(m, *(afxV4d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV3d*)outPtr = AfxV3dPostMultiplyAtm4d(m, *(afxV3d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV4d*)outPtr = AfxV4dPostMultiplyM3d(m, *(afxV4d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        *(afxV4d*)outPtr = AfxV4dPostMultiplyAtm4d(m, *(afxV4d*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}
