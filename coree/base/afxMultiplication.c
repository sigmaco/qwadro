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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/io/afxStream.h"

////////////////////////////////////////////////////////////////////////////////
// VECTOR                                                                     //
////////////////////////////////////////////////////////////////////////////////

// PostMultiply
// With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

_AFXINL void AfxV2dPostMultiplyM2d(afxV2d v, afxM2d const m, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(v);

    // batch requires a intermediate
    afxV4d t = { in[0], in[1] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]); // column 0 * vector
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]); // column 1 * vector
}

_AFXINL void AfxV3dPostMultiplyM3d(afxV3d v, afxM3d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]); // column 2 * vector
}

_AFXINL void AfxV4dPostMultiplyM4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3] * t[3]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3] * t[3]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3] * t[3]); // column 2 * vector    
    v[3] = (m[3][0] * t[0]) + (m[3][1] * t[1]) + (m[3][2] * t[2]) + (m[3][3] * t[3]); // column 3 * vector
}

_AFXINL void AfxV3dPostMultiplyLtm4d(afxV3d v, afxM4d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]); // column 2 * vector
}

_AFXINL void AfxV4dPostMultiplyM3d(afxV4d v, afxM3d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.

    // Compatible with TransposeVectorTransform4x3(in/v, 0.0, m)

    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]); // column 2 * vector
    v[3] = in[3];
}

_AFXINL void TransposeVectorTransform4x3(afxV3d Dest, float D3, afxM4d const Transform)
{
    // Compatible with TransposeVectorTransform4x3

    Dest[0] = Transform[0][0] * Dest[0] + Transform[0][1] * Dest[1] + Transform[0][2] * Dest[2] + D3 * Transform[3][0];
    Dest[1] = Transform[1][0] * Dest[0] + Transform[1][1] * Dest[1] + Transform[1][2] * Dest[2] + D3 * Transform[3][1];
    Dest[2] = Transform[2][0] * Dest[0] + Transform[2][1] * Dest[1] + Transform[2][2] * Dest[2] + D3 * Transform[3][2];
}

_AFXINL void AfxV2dPostMultiplyAtm4d(afxV2d v, afxM4d const m, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(v);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2]) + (m[0][3]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2]) + (m[1][3]); // column 1 * vector
}

_AFXINL void AfxV3dPostMultiplyAtm4d(afxV3d v, afxM4d const m, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3]); // column 2 * vector
}

_AFXINL void AfxV4dPostMultiplyAtm4d(afxV4d v, afxM4d const m, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(m);
    AFX_ASSERT(in);

    // With post-multiplication, a 4x4 matrix multiplied with a 4x1 column vector took the dot product of each row of the matrix with the vector.
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    v[0] = (m[0][0] * t[0]) + (m[0][1] * t[1]) + (m[0][2] * t[2]) + (m[0][3]); // column 0 * vector    
    v[1] = (m[1][0] * t[0]) + (m[1][1] * t[1]) + (m[1][2] * t[2]) + (m[1][3]); // column 1 * vector    
    v[2] = (m[2][0] * t[0]) + (m[2][1] * t[1]) + (m[2][2] * t[2]) + (m[2][3]); // column 2 * vector
    v[3] = 1.f;
}

// PreMultiply

_AFXINL void AfxV2dPreMultiplyM2d(afxV2d v, afxV2d const in, afxM2d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]); // vector * row 1
}

_AFXINL void AfxV3dPreMultiplyM3d(afxV3d v, afxV3d const in, afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]); // vector * row 1    
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]); // vector * row 2
}

_AFXINL void AfxV4dPreMultiplyM4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    // Compatible with TransposeVectorTransform4x4(v/in, m)

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (t[3] * m[3][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (t[3] * m[3][1]); // vector * row 1    
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (t[3] * m[3][2]); // vector * row 2    
    v[3] = (t[0] * m[0][3]) + (t[1] * m[1][3]) + (t[2] * m[2][3]) + (t[3] * m[3][3]); // vector * row 3
}

// TransformTransposedPoint/Normal/Affine

_AFXINL void AfxV3dPreMultiplyLtm4d(afxV3d v, afxV3d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]); // vector * row 1    
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]); // vector * row 2
}

_AFXINL void AfxV4dPreMultiplyM3d(afxV4d v, afxV4d const in, afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]); // vector * row 1    
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]); // vector * row 2
    v[3] = in[3];
}

_AFXINL void AfxV2dPreMultiplyAtm4d(afxV2d v, afxV2d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (m[2][0]) + (m[3][0]); // vector * row 0
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (m[2][1]) + (m[3][1]); // vector * row 1
}

_AFXINL void AfxV3dPreMultiplyAtm4d(afxV3d v, afxV3d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]); // vector * row 0    
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]); // vector * row 1    
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]); // vector * row 2
}

_AFXINL void AfxV4dPreMultiplyAtm4d(afxV4d v, afxV4d const in, afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(v);

    // With pre-multiplication, the dot product is with the vector and each column of the matrix (since the matrix is now on the right side of the multiplication operator).
    
    // batch requires a intermediate
    afxV4d t = { in[0], in[1], in[2], in[3] };

    v[0] = (t[0] * m[0][0]) + (t[1] * m[1][0]) + (t[2] * m[2][0]) + (m[3][0]); // vector * row 0
    v[1] = (t[0] * m[0][1]) + (t[1] * m[1][1]) + (t[2] * m[2][1]) + (m[3][1]); // vector * row 1
    v[2] = (t[0] * m[0][2]) + (t[1] * m[1][2]) + (t[2] * m[2][2]) + (m[3][2]); // vector * row 2
    //
    v[3] = 1.f;
}

////////////////////////////////////////////////////////////////////////////////
// QUATERNION                                                                 //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxQuatMultiply(afxQuat q, afxQuat const a, afxQuat const b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(q != a);
    AFX_ASSERT(q);

#if 0

    // Should be compatible with XMVECTOR XMQuaternionMultiply(FXMVECTOR Q1, FXMVECTOR Q2)

    // The result represents the rotation A followed by the rotation B to be consistent with 
    // MatrixMulplity concatenation since this function is typically used to concatenate quaternions that 
    // represent rotations (i.e. it returns B*A).

    // Returns the product B*A (which is the concatenation of a rotation A followed by the rotation B)

    afxQuat const t = { b[0], b[1], b[2], b[3] };
    q[0] = (t[3] * a[0]) + (t[0] * a[3]) + (t[1] * a[2]) - (t[2] * a[1]);
    q[1] = (t[3] * a[1]) - (t[0] * a[2]) + (t[1] * a[3]) + (t[2] * a[0]);
    q[2] = (t[3] * a[2]) + (t[0] * a[1]) - (t[1] * a[0]) + (t[2] * a[3]);
    q[3] = (t[3] * a[3]) - (t[0] * a[0]) - (t[1] * a[1]) - (t[2] * a[2]);
#else

    // Compatible with QuaternionMultiply4(q, a, b)

    afxQuat const t = { b[0], b[1], b[2], b[3] };
    q[0] = (t[3] * a[0]) + ((t[0] * a[3]) + (t[2] * a[1]) - (t[1] * a[2]));
    q[1] = (t[1] * a[3]) + (t[0] * a[2]) - (t[2] * a[0]) + (t[3] * a[1]);
    q[2] = (t[3] * a[2]) + ((t[2] * a[3]) + ((t[1] * a[0]) - (t[0] * a[1])));
    q[3] = (t[3] * a[3]) - ((t[2] * a[2]) + (t[1] * a[1]) + (t[0] * a[0]));
#endif
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX                                                                     //
////////////////////////////////////////////////////////////////////////////////

// In post-multiplication, each component of the result, at in given row R and column C, is computed by taking the dot product of the left matrix’s row R by the right matrix’s column C.

// MultiplyTransposed

_AFXINL void AfxM2dMultiply(afxM2d m, afxM2d const in, afxM2d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]); // colum 0 * row 0    
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]); // colum 0 * row 1
    
    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]); // colum 1 * row 0    
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]); // colum 1 * row 1
}

_AFXINL void AfxM2dMultiplyTransposed(afxM2d m, afxM2d const in, afxM2d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]); // colum 0 * row 0    
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]); // colum 1 * row 0
    
    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]); // colum 1 * row 1
}

_AFXINL void AfxM3dMultiply(afxM3d m, afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Compatible with MatrixMultiply3x3(m, in, mul)

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[0][2] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2

    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2

    m[2][0] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[2][1] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
}

_AFXINL void AfxM3dMultiplyTransposed(afxM3d m, afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[0][2] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0

    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1

    m[2][0] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[2][1] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
}

_AFXINL void TransposeMatrixMultiply3x3(afxM3d IntoMatrix, afxM3d const TransposedMatrix, afxM3d const ByMatrix)
{
    // Compatible with TransposeMatrixMultiply3x3(out, in, mul)

// 0 1 2, 3 4 5, 6 7 8

    // This function takes a already transposed matrix while AfxM3dMultiplyTransposed computes a new one. 

    IntoMatrix[0][0] = TransposedMatrix[0][0] * ByMatrix[0][0] + TransposedMatrix[1][0] * ByMatrix[1][0] + TransposedMatrix[2][0] * ByMatrix[2][0];
    IntoMatrix[0][1] = TransposedMatrix[0][0] * ByMatrix[0][1] + TransposedMatrix[1][0] * ByMatrix[1][1] + TransposedMatrix[2][0] * ByMatrix[2][1];
    IntoMatrix[0][2] = TransposedMatrix[0][0] * ByMatrix[0][2] + TransposedMatrix[1][0] * ByMatrix[1][2] + TransposedMatrix[2][0] * ByMatrix[2][2];

    IntoMatrix[1][0] = TransposedMatrix[0][1] * ByMatrix[0][0] + TransposedMatrix[1][1] * ByMatrix[1][0] + TransposedMatrix[2][1] * ByMatrix[2][0];
    IntoMatrix[1][1] = TransposedMatrix[0][1] * ByMatrix[0][1] + TransposedMatrix[1][1] * ByMatrix[1][1] + TransposedMatrix[2][1] * ByMatrix[2][1];
    IntoMatrix[1][2] = TransposedMatrix[0][1] * ByMatrix[0][2] + TransposedMatrix[1][1] * ByMatrix[1][2] + TransposedMatrix[2][1] * ByMatrix[2][2];
    
    IntoMatrix[2][0] = TransposedMatrix[0][2] * ByMatrix[0][0] + TransposedMatrix[1][2] * ByMatrix[1][0] + TransposedMatrix[2][2] * ByMatrix[2][0];
    IntoMatrix[2][1] = TransposedMatrix[0][2] * ByMatrix[0][1] + TransposedMatrix[1][2] * ByMatrix[1][1] + TransposedMatrix[2][2] * ByMatrix[2][1];
    IntoMatrix[2][2] = TransposedMatrix[0][2] * ByMatrix[0][2] + TransposedMatrix[1][2] * ByMatrix[1][2] + TransposedMatrix[2][2] * ByMatrix[2][2];
}

_AFXINL void AfxM4dMultiply(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Compatible with m = XMMatrixMultiply(in, mul)
    // Compatible with ColumnMatrixMultiply4x4(m, in, mul)

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]) + (in[0][3] * mul[3][0]); // colum 0 * row 0
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]) + (in[0][3] * mul[3][1]); // colum 0 * row 1
    m[0][2] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]) + (in[0][3] * mul[3][2]); // colum 0 * row 2
    m[0][3] = (in[0][0] * mul[0][3]) + (in[0][1] * mul[1][3]) + (in[0][2] * mul[2][3]) + (in[0][3] * mul[3][3]); // colum 0 * row 3

    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]) + (in[1][3] * mul[3][0]); // colum 1 * row 0
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]) + (in[1][3] * mul[3][1]); // colum 1 * row 1
    m[1][2] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]) + (in[1][3] * mul[3][2]); // colum 1 * row 2
    m[1][3] = (in[1][0] * mul[0][3]) + (in[1][1] * mul[1][3]) + (in[1][2] * mul[2][3]) + (in[1][3] * mul[3][3]); // colum 1 * row 3
    
    m[2][0] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]) + (in[2][3] * mul[3][0]); // colum 2 * row 0
    m[2][1] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]) + (in[2][3] * mul[3][1]); // colum 2 * row 1
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]) + (in[2][3] * mul[3][2]); // colum 2 * row 2
    m[2][3] = (in[2][0] * mul[0][3]) + (in[2][1] * mul[1][3]) + (in[2][2] * mul[2][3]) + (in[2][3] * mul[3][3]); // colum 2 * row 3

    m[3][0] = (in[3][0] * mul[0][0]) + (in[3][1] * mul[1][0]) + (in[3][2] * mul[2][0]) + (in[3][3] * mul[3][0]); // colum 3 * row 0
    m[3][1] = (in[3][0] * mul[0][1]) + (in[3][1] * mul[1][1]) + (in[3][2] * mul[2][1]) + (in[3][3] * mul[3][1]); // colum 3 * row 1
    m[3][2] = (in[3][0] * mul[0][2]) + (in[3][1] * mul[1][2]) + (in[3][2] * mul[2][2]) + (in[3][3] * mul[3][2]); // colum 3 * row 2
    m[3][3] = (in[3][0] * mul[0][3]) + (in[3][1] * mul[1][3]) + (in[3][2] * mul[2][3]) + (in[3][3] * mul[3][3]); // colum 3 * row 3
}

_AFXINL void AfxM4dMultiplyTransposed(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Compatible with m = XMMatrixMultiplyTranspose(in, mul)

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]) + (in[0][3] * mul[3][0]); // colum 0 * row 0
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]) + (in[1][3] * mul[3][0]); // colum 1 * row 0
    m[0][2] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]) + (in[2][3] * mul[3][0]); // colum 2 * row 0
    m[0][3] = (in[3][0] * mul[0][0]) + (in[3][1] * mul[1][0]) + (in[3][2] * mul[2][0]) + (in[3][3] * mul[3][0]); // colum 3 * row 0

    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]) + (in[0][3] * mul[3][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]) + (in[1][3] * mul[3][1]); // colum 1 * row 1
    m[1][2] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]) + (in[2][3] * mul[3][1]); // colum 2 * row 1
    m[1][3] = (in[3][0] * mul[0][1]) + (in[3][1] * mul[1][1]) + (in[3][2] * mul[2][1]) + (in[3][3] * mul[3][1]); // colum 3 * row 1

    m[2][0] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]) + (in[0][3] * mul[3][2]); // colum 0 * row 2
    m[2][1] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]) + (in[1][3] * mul[3][2]); // colum 1 * row 2
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]) + (in[2][3] * mul[3][2]); // colum 2 * row 2
    m[2][3] = (in[3][0] * mul[0][2]) + (in[3][1] * mul[1][2]) + (in[3][2] * mul[2][2]) + (in[3][3] * mul[3][2]); // colum 3 * row 2

    m[3][0] = (in[0][0] * mul[0][3]) + (in[0][1] * mul[1][3]) + (in[0][2] * mul[2][3]) + (in[0][3] * mul[3][3]); // colum 0 * row 3
    m[3][1] = (in[1][0] * mul[0][3]) + (in[1][1] * mul[1][3]) + (in[1][2] * mul[2][3]) + (in[1][3] * mul[3][3]); // colum 1 * row 3
    m[3][2] = (in[2][0] * mul[0][3]) + (in[2][1] * mul[1][3]) + (in[2][2] * mul[2][3]) + (in[2][3] * mul[3][3]); // colum 2 * row 3
    m[3][3] = (in[3][0] * mul[0][3]) + (in[3][1] * mul[1][3]) + (in[3][2] * mul[2][3]) + (in[3][3] * mul[3][3]); // colum 3 * row 3
}

// MultiplyPlanar/Linear/Affine

_AFXINL void AfxM3dMultiplyAtm(afxM3d m, afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[0][2] = 0.f;

    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = 0.f;

    m[2][0] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[2][1] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[2][2] = 1.f;
}

_AFXINL void AfxM3dMultiplyAtmTransposed(afxM3d m, afxM3d const in, afxM3d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[0][2] = 0.f;

    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = 0.f;

    m[2][0] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[2][1] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[2][2] = 1.f;
}

_AFXINL void AfxM4dMultiplyLtm(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[0][2] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[0][3] = 0.f;

    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[1][3] = 0.f;

    m[2][0] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[2][1] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyLtmTransposed(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[0][2] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[0][3] = 0.f;

    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[1][3] = 0.f;

    m[2][0] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[2][1] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyAtm(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Compatible with ColumnMatrixMultiply4x3(m, in, mul)

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[row<in>][col<mul>] = in[row][] * mul[][i]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[0][2] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[0][3] = 0.f;

    m[1][0] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[1][3] = 0.f;

    m[2][0] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[2][1] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
    m[2][3] = 0.f;

    m[3][0] = (in[3][0] * mul[0][0]) + (in[3][1] * mul[1][0]) + (in[3][2] * mul[2][0]) + mul[3][0]; // colum 3 * row 0
    m[3][1] = (in[3][0] * mul[0][1]) + (in[3][1] * mul[1][1]) + (in[3][2] * mul[2][1]) + mul[3][1]; // colum 3 * row 1
    m[3][2] = (in[3][0] * mul[0][2]) + (in[3][1] * mul[1][2]) + (in[3][2] * mul[2][2]) + mul[3][2]; // colum 3 * row 2
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dMultiplyAtmTransposed(afxM4d m, afxM4d const in, afxM4d const mul)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT(mul);
    AFX_ASSERT(in != mul);
    AFX_ASSERT(mul != m);

    // Cut off in[][col] * mul[row][] to visualize it in your mind
    // m[col<mul>][row<in>] = in[i][] * mul[][col]

    m[0][0] = (in[0][0] * mul[0][0]) + (in[0][1] * mul[1][0]) + (in[0][2] * mul[2][0]); // colum 0 * row 0
    m[0][1] = (in[1][0] * mul[0][0]) + (in[1][1] * mul[1][0]) + (in[1][2] * mul[2][0]); // colum 1 * row 0
    m[0][2] = (in[2][0] * mul[0][0]) + (in[2][1] * mul[1][0]) + (in[2][2] * mul[2][0]); // colum 2 * row 0
    m[0][3] = 0.f;

    m[1][0] = (in[0][0] * mul[0][1]) + (in[0][1] * mul[1][1]) + (in[0][2] * mul[2][1]); // colum 0 * row 1
    m[1][1] = (in[1][0] * mul[0][1]) + (in[1][1] * mul[1][1]) + (in[1][2] * mul[2][1]); // colum 1 * row 1
    m[1][2] = (in[2][0] * mul[0][1]) + (in[2][1] * mul[1][1]) + (in[2][2] * mul[2][1]); // colum 2 * row 1
    m[1][3] = 0.f;

    m[2][0] = (in[0][0] * mul[0][2]) + (in[0][1] * mul[1][2]) + (in[0][2] * mul[2][2]); // colum 0 * row 2
    m[2][1] = (in[1][0] * mul[0][2]) + (in[1][1] * mul[1][2]) + (in[1][2] * mul[2][2]); // colum 1 * row 2
    m[2][2] = (in[2][0] * mul[0][2]) + (in[2][1] * mul[1][2]) + (in[2][2] * mul[2][2]); // colum 2 * row 2
    m[2][3] = 0.f;

    m[3][0] = (in[0][0] * mul[0][3]) + (in[0][1] * mul[1][3]) + (in[0][2] * mul[2][3]) + in[0][3]; // colum 0 * row 3
    m[3][1] = (in[1][0] * mul[0][3]) + (in[1][1] * mul[1][3]) + (in[1][2] * mul[2][3]) + in[1][3]; // colum 1 * row 3
    m[3][2] = (in[2][0] * mul[0][3]) + (in[2][1] * mul[1][3]) + (in[2][2] * mul[2][3]) + in[2][3]; // colum 2 * row 3
    m[3][3] = 1.f;
}

// Multiply

// MultiplyArrayedV2d

_AFXINL void AfxM2dPostMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dPostMultiplyM2d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM4d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPostMultiplyLtm4d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPostMultiplyAtm4d(out[i], m, in[i]);
}

_AFXINL void AfxM3dPostMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyM3d(out[i], m, in[i]);
}

_AFXINL void AfxM4dPostMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPostMultiplyAtm4d(out[i], m, in[i]);
}

// PreMultiply

_AFXINL void AfxM2dPreMultiplyV2d(afxM2d const m, afxUnit cnt, afxV2d const in[], afxV2d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV2dPreMultiplyM2d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyV3d(afxM3d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyV4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM4d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPreMultiplyAtm4d(out[i], in[i], m);
}

_AFXINL void AfxM3dPreMultiplyLtv4d(afxM3d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyM3d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyLtv3d(afxM4d const m, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dPreMultiplyLtm4d(out[i], in[i], m);
}

_AFXINL void AfxM4dPreMultiplyAtv4d(afxM4d const m, afxUnit cnt, afxV4d const in[], afxV4d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV4dPreMultiplyAtm4d(out[i], in[i], m);
}

// TransformSerialized

_AFXINL void AfxM2dPostMultiplyV2dSerialized(afxM2d const m, afxUnit cnt, afxV2d const in[], afxUnit inStride, afxV2d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV2dPostMultiplyM2d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyV3dSerialized(afxM3d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyV4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);

    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM4d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv3dSerialized(afxM4d const m, afxUnit cnt, afxV3d const in[], afxUnit inStride, afxV3d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM3dPostMultiplyLtv4dSerialized(afxM3d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyM3d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}

_AFXINL void AfxM4dPostMultiplyAtv4dSerialized(afxM4d const m, afxUnit cnt, afxV4d const in[], afxUnit inStride, afxV4d out[], afxUnit outStride)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(inStride);
    AFX_ASSERT(out);
    AFX_ASSERT(outStride);
    afxByte* inPtr = (afxByte*)in;
    afxByte* outPtr = (afxByte*)out;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV4dPostMultiplyAtm4d((void*)outPtr, m, (void*)inPtr);
        inPtr += inStride;
        outPtr += outStride;
    }
}
