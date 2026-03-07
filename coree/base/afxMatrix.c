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
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"


// Memory layout: row-major

// IsIdentity

_AFXINL afxBool AfxM2dIsIdentity(afxM2d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 2; i++)
        for (afxUnit j = 0; flag && j < 2; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM3dIsIdentity(afxM3d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 3; i++)
        for (afxUnit j = 0; flag && j < 3; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM4dIsIdentity(afxM4d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 4; i++)
        for (afxUnit j = 0; flag && j < 4; j++)
            if (m[i][j] != 1 && m[j][i] != 0)
                flag = 0;

    return flag;
}

////////////////////////////////////////////////////////////////////////////////
// Initialization methods                                                     //
////////////////////////////////////////////////////////////////////////////////

// Zero

_AFXINL void AfxM2dZero(afxM2d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM2dCopy(m, AFX_M2D_ZERO);
}

_AFXINL void AfxM3dZero(afxM3d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM3dCopy(m, AFX_M3D_ZERO);
}

_AFXINL void AfxM4dZero(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM4dCopy(m, AFX_M4D_ZERO);
}

// MakeIdentity

_AFXINL void AfxM2dReset(afxM2d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM2dCopy(m, AFX_M2D_IDENTITY);
}

_AFXINL void AfxM3dReset(afxM3d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM3dCopy(m, AFX_M3D_IDENTITY);
}

_AFXINL void AfxM4dReset(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AfxM4dCopy(m, AFX_M4D_IDENTITY);
}

_AFXINL void AfxM4dEnsureLinear(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    // in Qwadro, translation is transposed.
    AfxV4dReset(m[3]);
}

_AFXINL void AfxM4dEnsureAffine(afxM4d m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    m[0][3] = 0.f;
    m[1][3] = 0.f;
    m[2][3] = 0.f;
    m[3][3] = 1.f;
}

// Set

_AFXINL void AfxM2dSet(afxM2d m, afxV2d const cx, afxV2d const cy)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);

    m[0][0] = cx[0];
    m[1][0] = cx[1];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
}

_AFXINL void AfxM3dSet(afxM3d m, afxV3d const cx, afxV3d const cy, afxV3d const cz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);

    // compatible with MatrixColumns3x3(m, x, y, z)

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
}

_AFXINL void AfxM4dSet(afxM4d m, afxV4d const cx, afxV4d const cy, afxV4d const cz, afxV4d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = cx[3];

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = cy[3];

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = cz[3];

    // in Qwadro, translation is transposed.
    AfxV4dCopy(m[3], cw);
}

_AFXINL void AfxM4dSetLinear(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = 0.0;

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = 0.0;

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxV4dReset(m[3]);
}

_AFXINL void AfxM4dSetAffine(afxM4d m, afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(cx);
    AFX_ASSERT(cy);
    AFX_ASSERT(cz);
    AFX_ASSERT(cw);

    m[0][0] = cx[0];
    m[1][0] = cx[1];
    m[2][0] = cx[2];
    m[3][0] = 0.0;

    m[0][1] = cy[0];
    m[1][1] = cy[1];
    m[2][1] = cy[2];
    m[3][1] = 0.0;

    m[0][2] = cz[0];
    m[1][2] = cz[1];
    m[2][2] = cz[2];
    m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    AfxV4dCopyAtv3d(m[3], cw);
}

// SetTransposed

_AFXINL void AfxM2dSetTransposed(afxM2d m, afxV2d const rx, afxV2d const ry)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AfxV2dCopy(m[0], rx);
    AfxV2dCopy(m[1], ry);
}

_AFXINL void AfxM3dSetTransposed(afxM3d m, afxV3d const rx, afxV3d const ry, afxV3d const rz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AfxV3dCopy(m[0], rx);
    AfxV3dCopy(m[1], ry);
    AfxV3dCopy(m[2], rz);
}

_AFXINL void AfxM4dSetTransposed(afxM4d m, afxV4d const rx, afxV4d const ry, afxV4d const rz, afxV4d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);
    AfxV4dCopy(m[0], rx);
    AfxV4dCopy(m[1], ry);
    AfxV4dCopy(m[2], rz);
    AfxV4dCopy(m[3], rw);
}

_AFXINL void AfxM4dSetLinearTransposed(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);

    AfxV4dCopyV3d(m[0], rx);
    AfxV4dCopyV3d(m[0], ry);
    AfxV4dCopyV3d(m[0], rz);
    AfxV4dCopyV3d(m[0], rw);
}

_AFXINL void AfxM4dSetAffineTransposed(afxM4d m, afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(rx);
    AFX_ASSERT(ry);
    AFX_ASSERT(rz);
    AFX_ASSERT(rw);

    AfxV4dCopyAtv3d(m[0], rx);
    AfxV4dCopyAtv3d(m[0], ry);
    AfxV4dCopyAtv3d(m[0], rz);
    AfxV4dCopyAtv3d(m[0], rw);
}

// SetDiagonal

_AFXINL void AfxM2dDiagonal(afxM2d m, afxReal xx, afxReal yy)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    m[0][0] = xx;
    m[0][1] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = yy;
}

_AFXINL void AfxM3dDiagonal(afxM3d m, afxReal xx, afxReal yy, afxReal zz)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    // compatible with MatrixColumns3x3(m, x, y, z)

    m[0][0] = xx;
    m[0][1] = 0.f;
    m[0][2] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = yy;
    m[1][2] = 0.f;

    m[2][0] = 0.f;
    m[2][1] = 0.f;
    m[2][2] = zz;
}

_AFXINL void AfxM4dDiagonal(afxM4d m, afxReal xx, afxReal yy, afxReal zz, afxReal ww)
{
    afxError err = { 0 };
    AFX_ASSERT(m);

    m[0][0] = xx;
    m[0][1] = 0.f;
    m[0][2] = 0.f;
    m[0][3] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = yy;
    m[1][2] = 0.f;
    m[1][3] = 0.f;

    m[2][0] = 0.f;
    m[2][1] = 0.f;
    m[2][2] = zz;
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = ww;
}

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL void AfxM2dCopy(afxM2d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopy(afxM3d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM4dCopy(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = in[0][3];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = in[1][3];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = in[2][3];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
    m[3][3] = in[3][3];
}

_AFXINL void AfxM4dCopyLtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT2(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = 0.f;

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = 0.f;

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dCopyAtm(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT2(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];
    m[0][3] = 0.f;

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];
    m[1][3] = 0.f;

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dCopyLtmTransposed(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(in, m, m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = 0.f;

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = 0.f;

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
    m[3][3] = 1.f;
}

_AFXINL void AfxM4dCopyAtmTransposed(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(in, m, m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = 0.f;

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = 0.f;

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = 0.f;

    m[3][0] = in[0][3];
    m[3][1] = in[1][3];
    m[3][2] = in[2][3];
    m[3][3] = 1.f;
}

//

_AFXINL void AfxM2dCopyM3d(afxM2d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(in, m);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopyM4d(afxM3d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, in, (void*)m != (void*)in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM3dCopyM4dTransposed(afxM3d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT3(m, in, (void*)m != (void*)in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM3dCopyM2d(afxM3d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AFX_REAL(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AFX_REAL(0);

    m[2][0] = AFX_REAL(0);
    m[2][1] = AFX_REAL(0);
    m[2][2] = AFX_REAL(1);
}

_AFXINL void AfxM4dCopyM2d(afxM4d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = AFX_REAL(0);
    m[0][3] = AFX_REAL(0);

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = AFX_REAL(0);
    m[1][3] = AFX_REAL(0);

    m[2][0] = AFX_REAL(0);
    m[2][1] = AFX_REAL(0);
    m[2][2] = AFX_REAL(1);
    m[2][3] = AFX_REAL(0);

    m[3][0] = AFX_REAL(0);
    m[3][1] = AFX_REAL(0);
    m[3][2] = AFX_REAL(0);
    m[3][3] = AFX_REAL(1);
}

_AFXINL void AfxM4dCopyM3d(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(ltm);
    AFX_ASSERT(atv);
    AFX_ASSERT_DIFF(m, ltm);

    m[0][0] = ltm[0][0];
    m[0][1] = ltm[0][1];
    m[0][2] = ltm[0][2];
    m[0][3] = AFX_REAL(0);

    m[1][0] = ltm[1][0];
    m[1][1] = ltm[1][1];
    m[1][2] = ltm[1][2];
    m[1][3] = AFX_REAL(0);

    m[2][0] = ltm[2][0];
    m[2][1] = ltm[2][1];
    m[2][2] = ltm[2][2];
    m[2][3] = AFX_REAL(0);

    m[3][0] = atv[0];
    m[3][1] = atv[1];
    m[3][2] = atv[2];
    m[3][3] = 1.f;
}

_AFXINL void AfxV3d4Copy(afxV3d4 m, afxV3d4 const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
}

_AFXINL void AfxV3d4CopyM3d(afxV3d4 m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = 0.f;
}

_AFXINL void AfxV3d4CopyM4d(afxV3d4 m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AFX_ASSERT_DIFF(m, in);

    m[0][0] = in[0][0];
    m[0][1] = in[0][1];
    m[0][2] = in[0][2];

    m[1][0] = in[1][0];
    m[1][1] = in[1][1];
    m[1][2] = in[1][2];

    m[2][0] = in[2][0];
    m[2][1] = in[2][1];
    m[2][2] = in[2][2];

    m[3][0] = in[3][0];
    m[3][1] = in[3][1];
    m[3][2] = in[3][2];
}

_AFXINL void AfxM4dCopyM3dTransposed(afxM4d m, afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(m);
    AFX_ASSERT(atv);

    m[0][0] = ltm[0][0];
    m[0][1] = ltm[1][0];
    m[0][2] = ltm[2][0];
    m[0][3] = 0.f;

    m[1][0] = ltm[0][1];
    m[1][1] = ltm[1][1];
    m[1][2] = ltm[2][1];
    m[1][3] = 0.f;

    m[2][0] = ltm[0][2];
    m[2][1] = ltm[1][2];
    m[2][2] = ltm[2][2];
    m[2][3] = 0.f;

    m[3][0] = atv[0];
    m[3][1] = atv[1];
    m[3][2] = atv[2];
    m[3][3] = 1.f;
}

// Swap

_AFXINL void AfxM2dSwap(afxM2d m, afxM2d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM2d tmp;
    AfxM2dCopy(tmp, m);
    AfxM2dCopy(other, tmp);
    AfxM2dCopy(m, other);
}

_AFXINL void AfxM3dSwap(afxM3d m, afxM3d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM3d tmp;
    AfxM3dCopy(tmp, m);
    AfxM3dCopy(other, tmp);
    AfxM3dCopy(m, other);
}

_AFXINL void AfxM4dSwap(afxM4d m, afxM4d other)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(other);
    AFX_ASSERT(m != other);
    afxM4d tmp;
    AfxM4dCopy(tmp, m);
    AfxM4dCopy(other, tmp);
    AfxM4dCopy(m, other);
}

// Transpose

_AFXINL void AfxM2dCopyTransposed(afxM2d m, afxM2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
}

_AFXINL void AfxM3dCopyTransposed(afxM3d m, afxM3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
}

_AFXINL void AfxM4dCopyTransposed(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(m != in);

    m[0][0] = in[0][0];
    m[0][1] = in[1][0];
    m[0][2] = in[2][0];
    m[0][3] = in[3][0];

    m[1][0] = in[0][1];
    m[1][1] = in[1][1];
    m[1][2] = in[2][1];
    m[1][3] = in[3][1];

    m[2][0] = in[0][2];
    m[2][1] = in[1][2];
    m[2][2] = in[2][2];
    m[2][3] = in[3][2];

    m[3][0] = in[0][3];
    m[3][1] = in[1][3];
    m[3][2] = in[2][3];
    m[3][3] = in[3][3];
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetic methods                                                         //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxM2dAdd(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dAdd(m[0], a[0], b[0]);
    AfxV2dAdd(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dAdd(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dAdd(m[0], a[0], b[0]);
    AfxV3dAdd(m[1], a[1], b[1]);
    AfxV3dAdd(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dAdd(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dAdd(m[0], a[0], b[0]);
    AfxV4dAdd(m[1], a[1], b[1]);
    AfxV4dAdd(m[2], a[2], b[2]);
    AfxV4dAdd(m[3], a[3], b[3]);
}

// Sub

_AFXINL void AfxM2dSub(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dSub(m[0], a[0], b[0]);
    AfxV2dSub(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dSub(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dSub(m[0], a[0], b[0]);
    AfxV3dSub(m[1], a[1], b[1]);
    AfxV3dSub(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dSub(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dSub(m[0], a[0], b[0]);
    AfxV4dSub(m[1], a[1], b[1]);
    AfxV4dSub(m[2], a[2], b[2]);
    AfxV4dSub(m[3], a[3], b[3]);
}

// Diff

_AFXINL void AfxM2dDiff(afxM2d m, afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV2dSub(m[0], a[0], b[0]);
    AfxV2dSub(m[1], a[1], b[1]);
}

_AFXINL void AfxM3dDiff(afxM3d m, afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV3dSub(m[0], a[0], b[0]);
    AfxV3dSub(m[1], a[1], b[1]);
    AfxV3dSub(m[2], a[2], b[2]);
}

_AFXINL void AfxM4dDiff(afxM4d m, afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AfxV4dSub(m[0], a[0], b[0]);
    AfxV4dSub(m[1], a[1], b[1]);
    AfxV4dSub(m[2], a[2], b[2]);
    AfxV4dSub(m[3], a[3], b[3]);
}

// Scale

_AFXINL void AfxM2dScale(afxM2d m, afxM2d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV2dScale(m[0], in[0], scale);
    AfxV2dScale(m[1], in[1], scale);
}

_AFXINL void AfxM3dScale(afxM3d m, afxM3d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV3dScale(m[0], in[0], scale);
    AfxV3dScale(m[1], in[1], scale);
    AfxV3dScale(m[2], in[2], scale);
}

_AFXINL void AfxM4dScale(afxM4d m, afxM4d const in, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(in);
    AfxV4dScale(m[0], in[0], scale);
    AfxV4dScale(m[1], in[1], scale);
    AfxV4dScale(m[2], in[2], scale);
    AfxV4dScale(m[3], in[3], scale);
}

// AddScaled

_AFXINL void AfxM2dMads(afxM2d m, afxM2d const add, afxM2d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV2dMads(m[0], add[0], mul[0], scale);
    AfxV2dMads(m[1], add[1], mul[1], scale);
}

_AFXINL void AfxM3dMads(afxM3d m, afxM3d const add, afxM3d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV3dMads(m[0], add[0], mul[0], scale);
    AfxV3dMads(m[1], add[1], mul[1], scale);
    AfxV3dMads(m[2], add[2], mul[2], scale);
}

_AFXINL void AfxM4dMads(afxM4d m, afxM4d const add, afxM4d const mul, afxReal scale)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AfxV4dMads(m[0], add[0], mul[0], scale);
    AfxV4dMads(m[1], add[1], mul[1], scale);
    AfxV4dMads(m[2], add[2], mul[2], scale);
    AfxV4dMads(m[3], add[3], mul[3], scale);
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Negate

_AFXINL afxReal AfxM3dInvert(afxM3d m, afxM3d const in)
{
    // Inspired on void MatrixInvert3x3(float *DestInit, const float *SourceInit)
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(in != m);

    afxReal det = (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * in[0][0] - (in[2][2] * in[1][0] - in[2][0] * in[1][2]) * in[0][1] + (in[2][1] * in[1][0] - in[2][0] * in[1][1]) * in[0][2];

    if (det != 0.0)
    {
        det = 1.0 / det;
        m[0][0] =  ( in[1][1] * in[2][2] - in[2][1] * in[1][2]) * det;
        m[1][0] = -((in[2][2] * in[1][0] - in[2][0] * in[1][2]) * det);
        m[2][0] =  ( in[2][1] * in[1][0] - in[2][0] * in[1][1]) * det;

        m[0][1] = -((in[0][1] * in[2][2] - in[2][1] * in[0][2]) * det);
        m[1][1] =  ( in[2][2] * in[0][0] - in[2][0] * in[0][2]) * det;
        m[2][1] = -((in[2][1] * in[0][0] - in[2][0] * in[0][1]) * det);

        m[0][2] =  ( in[0][1] * in[1][2] - in[1][1] * in[0][2]) * det;
        m[1][2] = -((in[1][2] * in[0][0] - in[0][2] * in[1][0]) * det);
        m[2][2] =  ( in[1][1] * in[0][0] - in[0][1] * in[1][0]) * det;
    }
    else
    {
        AfxM3dCopy(m, in);
        AFX_ASSERT(det != 0.0);
    }
    return det;
}

// Invert
// Memory layout: hybrid

_AFXINL afxReal AfxM4dInvert(afxM4d m, afxM4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(in);
    AFX_ASSERT(m);
    AFX_ASSERT(in != m);

    // From StackOverflow, by wangzhe, at https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

    m[0][0] =  in[1][1] * in[2][2] * in[3][3] - in[1][1] * in[2][3] * in[3][2] - in[2][1] * in[1][2] * in[3][3] + in[2][1] * in[1][3] * in[3][2] + in[3][1] * in[1][2] * in[2][3] - in[3][1] * in[1][3] * in[2][2];
    m[1][0] = -in[1][0] * in[2][2] * in[3][3] + in[1][0] * in[2][3] * in[3][2] + in[2][0] * in[1][2] * in[3][3] - in[2][0] * in[1][3] * in[3][2] - in[3][0] * in[1][2] * in[2][3] + in[3][0] * in[1][3] * in[2][2];
    m[2][0] =  in[1][0] * in[2][1] * in[3][3] - in[1][0] * in[2][3] * in[3][1] - in[2][0] * in[1][1] * in[3][3] + in[2][0] * in[1][3] * in[3][1] + in[3][0] * in[1][1] * in[2][3] - in[3][0] * in[1][3] * in[2][1];
    m[3][0] = -in[1][0] * in[2][1] * in[3][2] + in[1][0] * in[2][2] * in[3][1] + in[2][0] * in[1][1] * in[3][2] - in[2][0] * in[1][2] * in[3][1] - in[3][0] * in[1][1] * in[2][2] + in[3][0] * in[1][2] * in[2][1];
    
    m[0][1] = -in[0][1] * in[2][2] * in[3][3] + in[0][1] * in[2][3] * in[3][2] + in[2][1] * in[0][2] * in[3][3] - in[2][1] * in[0][3] * in[3][2] - in[3][1] * in[0][2] * in[2][3] + in[3][1] * in[0][3] * in[2][2];
    m[1][1] =  in[0][0] * in[2][2] * in[3][3] - in[0][0] * in[2][3] * in[3][2] - in[2][0] * in[0][2] * in[3][3] + in[2][0] * in[0][3] * in[3][2] + in[3][0] * in[0][2] * in[2][3] - in[3][0] * in[0][3] * in[2][2];
    m[2][1] = -in[0][0] * in[2][1] * in[3][3] + in[0][0] * in[2][3] * in[3][1] + in[2][0] * in[0][1] * in[3][3] - in[2][0] * in[0][3] * in[3][1] - in[3][0] * in[0][1] * in[2][3] + in[3][0] * in[0][3] * in[2][1];
    m[3][1] =  in[0][0] * in[2][1] * in[3][2] - in[0][0] * in[2][2] * in[3][1] - in[2][0] * in[0][1] * in[3][2] + in[2][0] * in[0][2] * in[3][1] + in[3][0] * in[0][1] * in[2][2] - in[3][0] * in[0][2] * in[2][1];
    
    m[0][2] =  in[0][1] * in[1][2] * in[3][3] - in[0][1] * in[1][3] * in[3][2] - in[1][1] * in[0][2] * in[3][3] + in[1][1] * in[0][3] * in[3][2] + in[3][1] * in[0][2] * in[1][3] - in[3][1] * in[0][3] * in[1][2];
    m[1][2] = -in[0][0] * in[1][2] * in[3][3] + in[0][0] * in[1][3] * in[3][2] + in[1][0] * in[0][2] * in[3][3] - in[1][0] * in[0][3] * in[3][2] - in[3][0] * in[0][2] * in[1][3] + in[3][0] * in[0][3] * in[1][2];
    m[2][2] =  in[0][0] * in[1][1] * in[3][3] - in[0][0] * in[1][3] * in[3][1] - in[1][0] * in[0][1] * in[3][3] + in[1][0] * in[0][3] * in[3][1] + in[3][0] * in[0][1] * in[1][3] - in[3][0] * in[0][3] * in[1][1];
    m[3][2] = -in[0][0] * in[1][1] * in[3][2] + in[0][0] * in[1][2] * in[3][1] + in[1][0] * in[0][1] * in[3][2] - in[1][0] * in[0][2] * in[3][1] - in[3][0] * in[0][1] * in[1][2] + in[3][0] * in[0][2] * in[1][1];
    
    m[0][3] = -in[0][1] * in[1][2] * in[2][3] + in[0][1] * in[1][3] * in[2][2] + in[1][1] * in[0][2] * in[2][3] - in[1][1] * in[0][3] * in[2][2] - in[2][1] * in[0][2] * in[1][3] + in[2][1] * in[0][3] * in[1][2];
    m[1][3] =  in[0][0] * in[1][2] * in[2][3] - in[0][0] * in[1][3] * in[2][2] - in[1][0] * in[0][2] * in[2][3] + in[1][0] * in[0][3] * in[2][2] + in[2][0] * in[0][2] * in[1][3] - in[2][0] * in[0][3] * in[1][2];
    m[2][3] = -in[0][0] * in[1][1] * in[2][3] + in[0][0] * in[1][3] * in[2][1] + in[1][0] * in[0][1] * in[2][3] - in[1][0] * in[0][3] * in[2][1] - in[2][0] * in[0][1] * in[1][3] + in[2][0] * in[0][3] * in[1][1];
    m[3][3] =  in[0][0] * in[1][1] * in[2][2] - in[0][0] * in[1][2] * in[2][1] - in[1][0] * in[0][1] * in[2][2] + in[1][0] * in[0][2] * in[2][1] + in[2][0] * in[0][1] * in[1][2] - in[2][0] * in[0][2] * in[1][1];

    afxReal det = in[0][0] * m[0][0] + in[0][1] * m[1][0] + in[0][2] * m[2][0] + in[0][3] * m[3][0];

    if (det)
    {
        det = 1.0 / det;

        for (afxUnit i = 0; i < 4; i++)
            for (afxUnit j = 0; j < 4; j++)
                m[i][j] = m[i][j] * det;
    }
    return det;
}

_AFXINL afxReal AfxM4dInvertAtm(afxM4d m, afxM4d const in)
{
    // Should be compatible with void MatrixInvert4x3(float *DestInit, const float *SourceInit)

    afxReal det = (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * in[0][0] - (in[2][2] * in[0][1] - in[2][1] * in[0][2]) * in[1][0] + (in[1][2] * in[0][1] - in[1][1] * in[0][2]) * in[2][0];

    if (det == (afxReal)0) AfxM4dReset(m); // gr ignora LT se não for inversível mas nulifica o W.
    else
    {
        afxReal recip = 1.0 / det;
        m[0][0] =  ( in[1][1] * in[2][2] - in[2][1] * in[1][2]) * recip;
        m[0][1] = -((in[2][2] * in[0][1] - in[2][1] * in[0][2]) * recip);
        m[0][2] =  ( in[1][2] * in[0][1] - in[1][1] * in[0][2]) * recip;
        m[0][3] = 0.f;

        m[1][0] = -((in[1][0] * in[2][2] - in[2][0] * in[1][2]) * recip);
        m[1][1] =  ( in[2][2] * in[0][0] - in[2][0] * in[0][2]) * recip;
        m[1][2] = -((in[1][2] * in[0][0] - in[0][2] * in[1][0]) * recip);
        m[1][3] = 0.f;

        m[2][0] =  ( in[2][1] * in[1][0] - in[2][0] * in[1][1]) * recip;
        m[2][1] = -((in[2][1] * in[0][0] - in[2][0] * in[0][1]) * recip);
        m[2][2] =  ( in[1][1] * in[0][0] - in[1][0] * in[0][1]) * recip;
        m[2][3] = 0.f;
    }
    m[3][0] = -(m[2][0] * in[3][2] + m[1][0] * in[3][1] + m[0][0] * in[3][0]);
    m[3][1] = -(m[2][1] * in[3][2] + m[1][1] * in[3][1] + m[0][1] * in[3][0]);
    m[3][2] = -(m[2][2] * in[3][2] + m[1][2] * in[3][1] + m[0][2] * in[3][0]);
    m[3][3] = 1.f;
    return det;
}

// Det

_AFXINL afxReal AfxM2dDet(afxM2d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * m[1][1]) - 
            (m[0][1] * m[1][0]);
}

_AFXINL afxReal AfxM3dDet(afxM3d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])) - 
            (m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])) + 
            (m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

_AFXINL afxReal AfxM4dDet(afxM4d const m)
{
    afxError err = { 0 };
    AFX_ASSERT(m);
    return  (m[0][0] * m[1][1] * m[2][2] * m[3][3]) +
            (m[0][0] * m[1][2] * m[2][3] * m[3][1]) +
            (m[0][0] * m[1][3] * m[2][1] * m[3][2]) +
            (m[0][1] * m[1][0] * m[2][3] * m[3][2]) +
            (m[0][1] * m[1][2] * m[2][0] * m[3][3]) +
            (m[0][1] * m[1][3] * m[2][2] * m[3][0]) +
            (m[0][2] * m[1][0] * m[2][1] * m[3][3]) +
            (m[0][2] * m[1][1] * m[2][3] * m[3][0]) +
            (m[0][2] * m[1][3] * m[2][0] * m[3][1]) +
            (m[0][3] * m[1][0] * m[2][2] * m[3][1]) +
            (m[0][3] * m[1][1] * m[2][0] * m[3][2]) +
            (m[0][3] * m[1][2] * m[2][1] * m[3][0]) -
            (m[0][0] * m[1][1] * m[2][3] * m[3][2]) -
            (m[0][0] * m[1][2] * m[2][1] * m[3][3]) -
            (m[0][0] * m[1][3] * m[2][2] * m[3][1]) -
            (m[0][1] * m[1][0] * m[2][2] * m[3][3]) -
            (m[0][1] * m[1][2] * m[2][3] * m[3][0]) -
            (m[0][1] * m[1][3] * m[2][0] * m[3][2]) -
            (m[0][2] * m[1][0] * m[2][3] * m[3][1]) -
            (m[0][2] * m[1][1] * m[2][0] * m[3][3]) -
            (m[0][2] * m[1][3] * m[2][1] * m[3][0]) -
            (m[0][3] * m[1][0] * m[2][1] * m[3][2]) -
            (m[0][3] * m[1][1] * m[2][2] * m[3][0]) -
            (m[0][3] * m[1][2] * m[2][0] * m[3][1]);
}
