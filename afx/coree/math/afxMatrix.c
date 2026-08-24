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

#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/afxStream.h"
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
            if (m.m[i][j] != 1 && m.m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM3dIsIdentity(afxM3d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 3; i++)
        for (afxUnit j = 0; flag && j < 3; j++)
            if (m.m[i][j] != 1 && m.m[j][i] != 0)
                flag = 0;

    return flag;
}

_AFXINL afxBool AfxM4dIsIdentity(afxM4d const m)
{
    afxBool flag = 1;

    for (afxUnit i = 0; flag && i < 4; i++)
        for (afxUnit j = 0; flag && j < 4; j++)
            if (m.m[i][j] != 1 && m.m[j][i] != 0)
                flag = 0;

    return flag;
}

////////////////////////////////////////////////////////////////////////////////
// Initialization methods                                                     //
////////////////////////////////////////////////////////////////////////////////

// Zero

_AFXINL afxM2d AfxM2dZero(void)
{
    afxError err = { 0 };
    return AFX_M2D_ZERO;
}

_AFXINL afxM3d AfxM3dZero(void)
{
    afxError err = { 0 };
    return AFX_M3D_ZERO;
}

_AFXINL afxM4d AfxM4dZero(void)
{
    afxError err = { 0 };
    return AFX_M4D_ZERO;
}

// MakeIdentity

_AFXINL afxM2d AfxM2dIdentity(void)
{
    afxError err = { 0 };
    return AFX_M2D_IDENTITY;
}

_AFXINL afxM3d AfxM3dIdentity(void)
{
    afxError err = { 0 };
    return AFX_M3D_IDENTITY;
}

_AFXINL afxM4d AfxM4dIdentity(void)
{
    afxError err = { 0 };
    return AFX_M4D_IDENTITY;
}

// Set

_AFXINL afxM2d AfxM2dMake(afxV2d const cx, afxV2d const cy)
{
    afxError err = { 0 };

    afxM2d m;
    m.m[0][0] = cx.x;
    m.m[1][0] = cx.y;

    m.m[0][1] = cy.x;
    m.m[1][1] = cy.y;
    return m;
}

_AFXINL afxM3d AfxM3dMake(afxV3d const cx, afxV3d const cy, afxV3d const cz)
{
    afxError err = { 0 };

    // compatible with MatrixColumns3x3(m, x, y, z)

    afxM3d m;
    m.m[0][0] = cx.x;
    m.m[1][0] = cx.y;
    m.m[2][0] = cx.z;

    m.m[0][1] = cy.x;
    m.m[1][1] = cy.y;
    m.m[2][1] = cy.z;

    m.m[0][2] = cz.x;
    m.m[1][2] = cz.y;
    m.m[2][2] = cz.z;
    return m;
}

_AFXINL afxM4d AfxM4dMake(afxV4d const cx, afxV4d const cy, afxV4d const cz, afxV4d const cw)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = cx.x;
    m.m[1][0] = cx.y;
    m.m[2][0] = cx.z;
    m.m[3][0] = cx.w;

    m.m[0][1] = cy.x;
    m.m[1][1] = cy.y;
    m.m[2][1] = cy.z;
    m.m[3][1] = cy.w;

    m.m[0][2] = cz.x;
    m.m[1][2] = cz.y;
    m.m[2][2] = cz.z;
    m.m[3][2] = cz.w;

    // in Qwadro, translation is transposed.
    m.w = cw;
    return m;
}

_AFXINL afxM4d AfxM4dMakeLtm(afxV3d const cx, afxV3d const cy, afxV3d const cz)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = cx.x;
    m.m[1][0] = cx.y;
    m.m[2][0] = cx.z;
    m.m[3][0] = 0.0;

    m.m[0][1] = cy.x;
    m.m[1][1] = cy.y;
    m.m[2][1] = cy.z;
    m.m[3][1] = 0.0;

    m.m[0][2] = cz.x;
    m.m[1][2] = cz.y;
    m.m[2][2] = cz.z;
    m.m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    m.w = AFX_V4D_IDENTITY;
    return m;
}

_AFXINL afxM4d AfxM4dMakeAtm(afxV3d const cx, afxV3d const cy, afxV3d const cz, afxV3d const cw)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = cx.x;
    m.m[1][0] = cx.y;
    m.m[2][0] = cx.z;
    m.m[3][0] = 0.0;

    m.m[0][1] = cy.x;
    m.m[1][1] = cy.y;
    m.m[2][1] = cy.z;
    m.m[3][1] = 0.0;

    m.m[0][2] = cz.x;
    m.m[1][2] = cz.y;
    m.m[2][2] = cz.z;
    m.m[3][2] = 0.0;

    // in Qwadro, translation is transposed.
    m.w = AfxV4dFromAtv3d(cw);
    return m;
}

// SetTransposed

_AFXINL afxM2d AfxM2dMakeTransposed(afxV2d const rx, afxV2d const ry)
{
    afxError err = { 0 };

    afxM2d m;
    m.x = rx;
    m.y = ry;
    return m;
}

_AFXINL afxM3d AfxM3dMakeTransposed(afxV3d const rx, afxV3d const ry, afxV3d const rz)
{
    afxError err = { 0 };

    afxM3d m;
    m.x = rx;
    m.y = ry;
    m.z = rz;
    return m;
}

_AFXINL afxM4d AfxM4dMakeTransposed(afxV4d const rx, afxV4d const ry, afxV4d const rz, afxV4d const rw)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = rx;
    m.y = ry;
    m.z = rz;
    m.w = rw;
    return m;
}

_AFXINL afxM4d AfxM4dMakeAtmTransposed(afxV3d const rx, afxV3d const ry, afxV3d const rz, afxV3d const rw)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = AfxV4dFromV3d(rx);
    m.y = AfxV4dFromV3d(ry);
    m.z = AfxV4dFromV3d(rz);
    m.w = AfxV4dFromAtv3d(rw);
    return m;
}

// SetDiagonal

_AFXINL afxM2d AfxM2dDiagonal(afxReal xx, afxReal yy)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.m[0][0] = xx;
    m.m[0][1] = 0.f;

    m.m[1][0] = 0.f;
    m.m[1][1] = yy;
    return m;
}

_AFXINL afxM3d AfxM3dDiagonal(afxReal xx, afxReal yy, afxReal zz)
{
    afxError err = { 0 };

    // compatible with MatrixColumns3x3(m, x, y, z)
    
    afxM3d m;
    m.m[0][0] = xx;
    m.m[0][1] = 0.f;
    m.m[0][2] = 0.f;

    m.m[1][0] = 0.f;
    m.m[1][1] = yy;
    m.m[1][2] = 0.f;

    m.m[2][0] = 0.f;
    m.m[2][1] = 0.f;
    m.m[2][2] = zz;
    return m;
}

_AFXINL afxM4d AfxM4dDiagonal(afxReal xx, afxReal yy, afxReal zz, afxReal ww)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = xx;
    m.m[0][1] = 0.f;
    m.m[0][2] = 0.f;
    m.m[0][3] = 0.f;

    m.m[1][0] = 0.f;
    m.m[1][1] = yy;
    m.m[1][2] = 0.f;
    m.m[1][3] = 0.f;

    m.m[2][0] = 0.f;
    m.m[2][1] = 0.f;
    m.m[2][2] = zz;
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = ww;
    return m;
}

////////////////////////////////////////////////////////////////////////////////
// Transferance methods                                                       //
////////////////////////////////////////////////////////////////////////////////

// Copy

_AFXINL afxM4d AfxM4dFromLtm(afxM4d const in)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];
    m.m[0][3] = 0.f;

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];
    m.m[1][3] = 0.f;

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dFromAtm(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];
    m.m[0][3] = 0.f;

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];
    m.m[1][3] = 0.f;

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = 0.f;

    m.m[3][0] = in.m[3][0];
    m.m[3][1] = in.m[3][1];
    m.m[3][2] = in.m[3][2];
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dTransposeLtm(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];
    m.m[0][2] = in.m[2][0];
    m.m[0][3] = 0.f;

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[2][1];
    m.m[1][3] = 0.f;

    m.m[2][0] = in.m[0][2];
    m.m[2][1] = in.m[1][2];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = 0.f;

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM4d AfxM4dTransposeAtm(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];
    m.m[0][2] = in.m[2][0];
    m.m[0][3] = 0.f;

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[2][1];
    m.m[1][3] = 0.f;

    m.m[2][0] = in.m[0][2];
    m.m[2][1] = in.m[1][2];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = 0.f;

    m.m[3][0] = in.m[0][3];
    m.m[3][1] = in.m[1][3];
    m.m[3][2] = in.m[2][3];
    m.m[3][3] = 1.f;
    return m;
}

//

_AFXINL afxM2d AfxM2dFromM3d(afxM3d const in)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    return m;
}

_AFXINL afxM3d AfxM3dFromM4d(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];
    return m;
}

_AFXINL afxM3d AfxM3dTransposeM4d(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];
    m.m[0][2] = in.m[2][0];

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[2][1];

    m.m[2][0] = in.m[0][2];
    m.m[2][1] = in.m[1][2];
    m.m[2][2] = in.m[2][2];
    return m;
}

_AFXINL afxM3d AfxM3dFromM2d(afxM2d const in)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = AFX_REAL(0);

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = AFX_REAL(0);

    m.m[2][0] = AFX_REAL(0);
    m.m[2][1] = AFX_REAL(0);
    m.m[2][2] = AFX_REAL(1);
    return m;
}

_AFXINL afxM4d AfxM4dFromM2d(afxM2d const in)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = AFX_REAL(0);
    m.m[0][3] = AFX_REAL(0);

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = AFX_REAL(0);
    m.m[1][3] = AFX_REAL(0);

    m.m[2][0] = AFX_REAL(0);
    m.m[2][1] = AFX_REAL(0);
    m.m[2][2] = AFX_REAL(1);
    m.m[2][3] = AFX_REAL(0);

    m.m[3][0] = AFX_REAL(0);
    m.m[3][1] = AFX_REAL(0);
    m.m[3][2] = AFX_REAL(0);
    m.m[3][3] = AFX_REAL(1);
    return m;
}

_AFXINL afxM4d AfxM4dFromM3d(afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = ltm.m[0][0];
    m.m[0][1] = ltm.m[0][1];
    m.m[0][2] = ltm.m[0][2];
    m.m[0][3] = AFX_REAL(0);

    m.m[1][0] = ltm.m[1][0];
    m.m[1][1] = ltm.m[1][1];
    m.m[1][2] = ltm.m[1][2];
    m.m[1][3] = AFX_REAL(0);

    m.m[2][0] = ltm.m[2][0];
    m.m[2][1] = ltm.m[2][1];
    m.m[2][2] = ltm.m[2][2];
    m.m[2][3] = AFX_REAL(0);

    m.m[3][0] = atv.x;
    m.m[3][1] = atv.y;
    m.m[3][2] = atv.z;
    m.m[3][3] = 1.f;
    return m;
}

_AFXINL afxM43d AfxM43dFromM3d(afxM3d const in)
{
    afxError err = { 0 };
    
    afxM43d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];

    m.m[3][0] = 0.f;
    m.m[3][1] = 0.f;
    m.m[3][2] = 0.f;
    return m;
}

_AFXINL afxM43d AfxM43dFromM4d(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM43d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];

    m.m[3][0] = in.m[3][0];
    m.m[3][1] = in.m[3][1];
    m.m[3][2] = in.m[3][2];
    return m;
}

_AFXINL afxM4d AfxM4dFromM43d(afxM43d const in)
{
    afxError err = { 0 };

    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[0][1];
    m.m[0][2] = in.m[0][2];
    m.m[0][3] = 0;

    m.m[1][0] = in.m[1][0];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[1][2];
    m.m[1][3] = 0;

    m.m[2][0] = in.m[2][0];
    m.m[2][1] = in.m[2][1];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = 0;

    m.m[3][0] = in.m[3][0];
    m.m[3][1] = in.m[3][1];
    m.m[3][2] = in.m[3][2];
    m.m[3][3] = 1;
    return m;
}

_AFXINL afxM4d AfxM4dTransposeM3d(afxM3d const ltm, afxV4d const atv)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = ltm.m[0][0];
    m.m[0][1] = ltm.m[1][0];
    m.m[0][2] = ltm.m[2][0];
    m.m[0][3] = 0.f;

    m.m[1][0] = ltm.m[0][1];
    m.m[1][1] = ltm.m[1][1];
    m.m[1][2] = ltm.m[2][1];
    m.m[1][3] = 0.f;

    m.m[2][0] = ltm.m[0][2];
    m.m[2][1] = ltm.m[1][2];
    m.m[2][2] = ltm.m[2][2];
    m.m[2][3] = 0.f;

    m.m[3][0] = atv.x;
    m.m[3][1] = atv.y;
    m.m[3][2] = atv.z;
    m.m[3][3] = 1.f;
    return m;
}

// Transpose

_AFXINL afxM2d AfxM2dTranspose(afxM2d const in)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    return m;
}

_AFXINL afxM3d AfxM3dTranspose(afxM3d const in)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];
    m.m[0][2] = in.m[2][0];

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[2][1];

    m.m[2][0] = in.m[0][2];
    m.m[2][1] = in.m[1][2];
    m.m[2][2] = in.m[2][2];
    return m;
}

_AFXINL afxM4d AfxM4dTranspose(afxM4d const in)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.m[0][0] = in.m[0][0];
    m.m[0][1] = in.m[1][0];
    m.m[0][2] = in.m[2][0];
    m.m[0][3] = in.m[3][0];

    m.m[1][0] = in.m[0][1];
    m.m[1][1] = in.m[1][1];
    m.m[1][2] = in.m[2][1];
    m.m[1][3] = in.m[3][1];

    m.m[2][0] = in.m[0][2];
    m.m[2][1] = in.m[1][2];
    m.m[2][2] = in.m[2][2];
    m.m[2][3] = in.m[3][2];

    m.m[3][0] = in.m[0][3];
    m.m[3][1] = in.m[1][3];
    m.m[3][2] = in.m[2][3];
    m.m[3][3] = in.m[3][3];
    return m;
}

////////////////////////////////////////////////////////////////////////////////
// Arithmetic methods                                                         //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL afxM2d AfxM2dAdd(afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.x = AfxV2dAdd(a.x, b.x);
    m.y = AfxV2dAdd(a.y, b.y);
    return m;
}

_AFXINL afxM3d AfxM3dAdd(afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.x = AfxV3dAdd(a.x, b.x);
    m.y = AfxV3dAdd(a.y, b.y);
    m.z = AfxV3dAdd(a.z, b.z);
    return m;
}

_AFXINL afxM4d AfxM4dAdd(afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };

    afxM4d m;
    m.x = AfxV4dAdd(a.x, b.x);
    m.y = AfxV4dAdd(a.y, b.y);
    m.z = AfxV4dAdd(a.z, b.z);
    m.w = AfxV4dAdd(a.w, b.w);
    return m;
}

// Sub

_AFXINL afxM2d AfxM2dSub(afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.x = AfxV2dSub(a.x, b.x);
    m.y = AfxV2dSub(a.y, b.y);
    return m;
}

_AFXINL afxM3d AfxM3dSub(afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.x = AfxV3dSub(a.x, b.x);
    m.y = AfxV3dSub(a.y, b.y);
    m.z = AfxV3dSub(a.z, b.z);
    return m;
}

_AFXINL afxM4d AfxM4dSub(afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = AfxV4dSub(a.x, b.x);
    m.y = AfxV4dSub(a.y, b.y);
    m.z = AfxV4dSub(a.z, b.z);
    m.w = AfxV4dSub(a.w, b.w);
    return m;
}

// Diff

_AFXINL afxM2d AfxM2dDiff(afxM2d const a, afxM2d const b)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.x = AfxV2dSub(a.x, b.x);
    m.y = AfxV2dSub(a.y, b.y);
    return m;
}

_AFXINL afxM3d AfxM3dDiff(afxM3d const a, afxM3d const b)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.x = AfxV3dSub(a.x, b.x);
    m.y = AfxV3dSub(a.y, b.y);
    m.z = AfxV3dSub(a.z, b.z);
    return m;
}

_AFXINL afxM4d AfxM4dDiff(afxM4d const a, afxM4d const b)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = AfxV4dSub(a.x, b.x);
    m.y = AfxV4dSub(a.y, b.y);
    m.z = AfxV4dSub(a.z, b.z);
    m.w = AfxV4dSub(a.w, b.w);
    return m;
}

// Scale

_AFXINL afxM2d AfxM2dScale(afxM2d const in, afxReal scale)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.x = AfxV2dScale(in.x, scale);
    m.y = AfxV2dScale(in.y, scale);
    return m;
}

_AFXINL afxM3d AfxM3dScale(afxM3d const in, afxReal scale)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.x = AfxV3dScale(in.x, scale);
    m.y = AfxV3dScale(in.y, scale);
    m.z = AfxV3dScale(in.z, scale);
    return m;
}

_AFXINL afxM4d AfxM4dScale(afxM4d const in, afxReal scale)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = AfxV4dScale(in.x, scale);
    m.y = AfxV4dScale(in.y, scale);
    m.z = AfxV4dScale(in.z, scale);
    m.w = AfxV4dScale(in.w, scale);
    return m;
}

// AddScaled

_AFXINL afxM2d AfxM2dMads(afxM2d const add, afxM2d const mul, afxReal scale)
{
    afxError err = { 0 };
    
    afxM2d m;
    m.x = AfxV2dMads(add.x, mul.x, scale);
    m.y = AfxV2dMads(add.y, mul.y, scale);
    return m;
}

_AFXINL afxM3d AfxM3dMads(afxM3d const add, afxM3d const mul, afxReal scale)
{
    afxError err = { 0 };
    
    afxM3d m;
    m.x = AfxV3dMads(add.x, mul.x, scale);
    m.y = AfxV3dMads(add.y, mul.y, scale);
    m.z = AfxV3dMads(add.z, mul.z, scale);
    return m;
}

_AFXINL afxM4d AfxM4dMads(afxM4d const add, afxM4d const mul, afxReal scale)
{
    afxError err = { 0 };
    
    afxM4d m;
    m.x = AfxV4dMads(add.x, mul.x, scale);
    m.y = AfxV4dMads(add.y, mul.y, scale);
    m.z = AfxV4dMads(add.z, mul.z, scale);
    m.w = AfxV4dMads(add.w, mul.w, scale);
    return m;
}

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Negate

_AFXINL afxM3d AfxM3dInvert(afxM3d const in, afxReal* determinant)
{
    // Inspired on void MatrixInvert3x3(float *DestInit, const float *SourceInit)
    afxError err = { 0 };
    
    afxReal det =   (in.m[1][1] * in.m[2][2] - in.m[2][1] * in.m[1][2]) * in.m[0][0] - 
                    (in.m[2][2] * in.m[1][0] - in.m[2][0] * in.m[1][2]) * in.m[0][1] + 
                    (in.m[2][1] * in.m[1][0] - in.m[2][0] * in.m[1][1]) * in.m[0][2];
    if (determinant) *determinant = det;

    if (det == 0.0)
        return in;

    det = 1.0 / det;

    afxM3d m;

    m.m[0][0] =  ( in.m[1][1] * in.m[2][2] - in.m[2][1] * in.m[1][2]) * det;
    m.m[1][0] = -((in.m[2][2] * in.m[1][0] - in.m[2][0] * in.m[1][2]) * det);
    m.m[2][0] =  ( in.m[2][1] * in.m[1][0] - in.m[2][0] * in.m[1][1]) * det;

    m.m[0][1] = -((in.m[0][1] * in.m[2][2] - in.m[2][1] * in.m[0][2]) * det);
    m.m[1][1] =  ( in.m[2][2] * in.m[0][0] - in.m[2][0] * in.m[0][2]) * det;
    m.m[2][1] = -((in.m[2][1] * in.m[0][0] - in.m[2][0] * in.m[0][1]) * det);

    m.m[0][2] =  ( in.m[0][1] * in.m[1][2] - in.m[1][1] * in.m[0][2]) * det;
    m.m[1][2] = -((in.m[1][2] * in.m[0][0] - in.m[0][2] * in.m[1][0]) * det);
    m.m[2][2] =  ( in.m[1][1] * in.m[0][0] - in.m[0][1] * in.m[1][0]) * det;

    return m;
}

// Invert
// Memory layout: hybrid

_AFXINL afxM4d AfxM4dInvert(afxM4d const in, afxReal* determinant)
{
    afxError err = { 0 };

    // From StackOverflow, by wangzhe, at https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    afxM4d m;

    m.m[0][0] =  in.m[1][1] * in.m[2][2] * in.m[3][3] - in.m[1][1] * in.m[2][3] * in.m[3][2] - in.m[2][1] * in.m[1][2] * in.m[3][3] + in.m[2][1] * in.m[1][3] * in.m[3][2] + in.m[3][1] * in.m[1][2] * in.m[2][3] - in.m[3][1] * in.m[1][3] * in.m[2][2];
    m.m[1][0] = -in.m[1][0] * in.m[2][2] * in.m[3][3] + in.m[1][0] * in.m[2][3] * in.m[3][2] + in.m[2][0] * in.m[1][2] * in.m[3][3] - in.m[2][0] * in.m[1][3] * in.m[3][2] - in.m[3][0] * in.m[1][2] * in.m[2][3] + in.m[3][0] * in.m[1][3] * in.m[2][2];
    m.m[2][0] =  in.m[1][0] * in.m[2][1] * in.m[3][3] - in.m[1][0] * in.m[2][3] * in.m[3][1] - in.m[2][0] * in.m[1][1] * in.m[3][3] + in.m[2][0] * in.m[1][3] * in.m[3][1] + in.m[3][0] * in.m[1][1] * in.m[2][3] - in.m[3][0] * in.m[1][3] * in.m[2][1];
    m.m[3][0] = -in.m[1][0] * in.m[2][1] * in.m[3][2] + in.m[1][0] * in.m[2][2] * in.m[3][1] + in.m[2][0] * in.m[1][1] * in.m[3][2] - in.m[2][0] * in.m[1][2] * in.m[3][1] - in.m[3][0] * in.m[1][1] * in.m[2][2] + in.m[3][0] * in.m[1][2] * in.m[2][1];
    
    m.m[0][1] = -in.m[0][1] * in.m[2][2] * in.m[3][3] + in.m[0][1] * in.m[2][3] * in.m[3][2] + in.m[2][1] * in.m[0][2] * in.m[3][3] - in.m[2][1] * in.m[0][3] * in.m[3][2] - in.m[3][1] * in.m[0][2] * in.m[2][3] + in.m[3][1] * in.m[0][3] * in.m[2][2];
    m.m[1][1] =  in.m[0][0] * in.m[2][2] * in.m[3][3] - in.m[0][0] * in.m[2][3] * in.m[3][2] - in.m[2][0] * in.m[0][2] * in.m[3][3] + in.m[2][0] * in.m[0][3] * in.m[3][2] + in.m[3][0] * in.m[0][2] * in.m[2][3] - in.m[3][0] * in.m[0][3] * in.m[2][2];
    m.m[2][1] = -in.m[0][0] * in.m[2][1] * in.m[3][3] + in.m[0][0] * in.m[2][3] * in.m[3][1] + in.m[2][0] * in.m[0][1] * in.m[3][3] - in.m[2][0] * in.m[0][3] * in.m[3][1] - in.m[3][0] * in.m[0][1] * in.m[2][3] + in.m[3][0] * in.m[0][3] * in.m[2][1];
    m.m[3][1] =  in.m[0][0] * in.m[2][1] * in.m[3][2] - in.m[0][0] * in.m[2][2] * in.m[3][1] - in.m[2][0] * in.m[0][1] * in.m[3][2] + in.m[2][0] * in.m[0][2] * in.m[3][1] + in.m[3][0] * in.m[0][1] * in.m[2][2] - in.m[3][0] * in.m[0][2] * in.m[2][1];
    
    m.m[0][2] =  in.m[0][1] * in.m[1][2] * in.m[3][3] - in.m[0][1] * in.m[1][3] * in.m[3][2] - in.m[1][1] * in.m[0][2] * in.m[3][3] + in.m[1][1] * in.m[0][3] * in.m[3][2] + in.m[3][1] * in.m[0][2] * in.m[1][3] - in.m[3][1] * in.m[0][3] * in.m[1][2];
    m.m[1][2] = -in.m[0][0] * in.m[1][2] * in.m[3][3] + in.m[0][0] * in.m[1][3] * in.m[3][2] + in.m[1][0] * in.m[0][2] * in.m[3][3] - in.m[1][0] * in.m[0][3] * in.m[3][2] - in.m[3][0] * in.m[0][2] * in.m[1][3] + in.m[3][0] * in.m[0][3] * in.m[1][2];
    m.m[2][2] =  in.m[0][0] * in.m[1][1] * in.m[3][3] - in.m[0][0] * in.m[1][3] * in.m[3][1] - in.m[1][0] * in.m[0][1] * in.m[3][3] + in.m[1][0] * in.m[0][3] * in.m[3][1] + in.m[3][0] * in.m[0][1] * in.m[1][3] - in.m[3][0] * in.m[0][3] * in.m[1][1];
    m.m[3][2] = -in.m[0][0] * in.m[1][1] * in.m[3][2] + in.m[0][0] * in.m[1][2] * in.m[3][1] + in.m[1][0] * in.m[0][1] * in.m[3][2] - in.m[1][0] * in.m[0][2] * in.m[3][1] - in.m[3][0] * in.m[0][1] * in.m[1][2] + in.m[3][0] * in.m[0][2] * in.m[1][1];
    
    m.m[0][3] = -in.m[0][1] * in.m[1][2] * in.m[2][3] + in.m[0][1] * in.m[1][3] * in.m[2][2] + in.m[1][1] * in.m[0][2] * in.m[2][3] - in.m[1][1] * in.m[0][3] * in.m[2][2] - in.m[2][1] * in.m[0][2] * in.m[1][3] + in.m[2][1] * in.m[0][3] * in.m[1][2];
    m.m[1][3] =  in.m[0][0] * in.m[1][2] * in.m[2][3] - in.m[0][0] * in.m[1][3] * in.m[2][2] - in.m[1][0] * in.m[0][2] * in.m[2][3] + in.m[1][0] * in.m[0][3] * in.m[2][2] + in.m[2][0] * in.m[0][2] * in.m[1][3] - in.m[2][0] * in.m[0][3] * in.m[1][2];
    m.m[2][3] = -in.m[0][0] * in.m[1][1] * in.m[2][3] + in.m[0][0] * in.m[1][3] * in.m[2][1] + in.m[1][0] * in.m[0][1] * in.m[2][3] - in.m[1][0] * in.m[0][3] * in.m[2][1] - in.m[2][0] * in.m[0][1] * in.m[1][3] + in.m[2][0] * in.m[0][3] * in.m[1][1];
    m.m[3][3] =  in.m[0][0] * in.m[1][1] * in.m[2][2] - in.m[0][0] * in.m[1][2] * in.m[2][1] - in.m[1][0] * in.m[0][1] * in.m[2][2] + in.m[1][0] * in.m[0][2] * in.m[2][1] + in.m[2][0] * in.m[0][1] * in.m[1][2] - in.m[2][0] * in.m[0][2] * in.m[1][1];

    afxReal det = in.m[0][0] * m.m[0][0] + in.m[0][1] * m.m[1][0] + in.m[0][2] * m.m[2][0] + in.m[0][3] * m.m[3][0];

    if (det)
    {
        det = 1.0 / det;

        for (afxUnit i = 0; i < 4; i++)
            for (afxUnit j = 0; j < 4; j++)
                m.m[i][j] = m.m[i][j] * det;
    }

    if (determinant) *determinant = det;

    return m;
}

_AFXINL afxM4d AfxM4dInvertAtm(afxM4d const in, afxReal* determinant)
{
    // Should be compatible with void MatrixInvert4x3(float *DestInit, const float *SourceInit)

    afxReal det =   (in.m[1][1] * in.m[2][2] - in.m[2][1] * in.m[1][2]) * in.m[0][0] - 
                    (in.m[2][2] * in.m[0][1] - in.m[2][1] * in.m[0][2]) * in.m[1][0] + 
                    (in.m[1][2] * in.m[0][1] - in.m[1][1] * in.m[0][2]) * in.m[2][0];

    if (determinant) *determinant = det;

    // gr ignora LT se não for inversível mas nulifica o W.
    if (det == (afxReal)0)
        return AFX_M4D_IDENTITY;

    afxReal recip = 1.0 / det;
    afxM4d m;
    m.m[0][0] =  ( in.m[1][1] * in.m[2][2] - in.m[2][1] * in.m[1][2]) * recip;
    m.m[0][1] = -((in.m[2][2] * in.m[0][1] - in.m[2][1] * in.m[0][2]) * recip);
    m.m[0][2] =  ( in.m[1][2] * in.m[0][1] - in.m[1][1] * in.m[0][2]) * recip;
    m.m[0][3] = 0.f;

    m.m[1][0] = -((in.m[1][0] * in.m[2][2] - in.m[2][0] * in.m[1][2]) * recip);
    m.m[1][1] =  ( in.m[2][2] * in.m[0][0] - in.m[2][0] * in.m[0][2]) * recip;
    m.m[1][2] = -((in.m[1][2] * in.m[0][0] - in.m[0][2] * in.m[1][0]) * recip);
    m.m[1][3] = 0.f;

    m.m[2][0] =  ( in.m[2][1] * in.m[1][0] - in.m[2][0] * in.m[1][1]) * recip;
    m.m[2][1] = -((in.m[2][1] * in.m[0][0] - in.m[2][0] * in.m[0][1]) * recip);
    m.m[2][2] =  ( in.m[1][1] * in.m[0][0] - in.m[1][0] * in.m[0][1]) * recip;
    m.m[2][3] = 0.f;

    m.m[3][0] = -(m.m[2][0] * in.m[3][2] + m.m[1][0] * in.m[3][1] + m.m[0][0] * in.m[3][0]);
    m.m[3][1] = -(m.m[2][1] * in.m[3][2] + m.m[1][1] * in.m[3][1] + m.m[0][1] * in.m[3][0]);
    m.m[3][2] = -(m.m[2][2] * in.m[3][2] + m.m[1][2] * in.m[3][1] + m.m[0][2] * in.m[3][0]);
    m.m[3][3] = 1.f;

    return m;
}

// Det

_AFXINL afxReal AfxM2dDet(afxM2d const m)
{
    afxError err = { 0 };
    return  (m.m[0][0] * m.m[1][1]) - 
            (m.m[0][1] * m.m[1][0]);
}

_AFXINL afxReal AfxM3dDet(afxM3d const m)
{
    afxError err = { 0 };
    return  (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])) - 
            (m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])) + 
            (m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));
}

_AFXINL afxReal AfxM4dDet(afxM4d const m)
{
    afxError err = { 0 };
    return  (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
            (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
            (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) +
            (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +
            (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
            (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]) +
            (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
            (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
            (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
            (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
            (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) +
            (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) -
            (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -
            (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
            (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
            (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
            (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
            (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -
            (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
            (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
            (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
            (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) -
            (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) -
            (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]);
}

// DOFs


_AFXINL afxM3d AfxM3dDofX(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = m.x;
    m2.y = AFX_V3D_Y;
    m2.z = AFX_V3D_Z;
    return m2;
}

_AFXINL afxM3d AfxM3dDofY(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = AFX_V3D_X;
    m2.y = m.y;
    m2.z = AFX_V3D_Z;
    return m2;
}

_AFXINL afxM3d AfxM3dDofZ(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = AFX_V3D_X;
    m2.y = AFX_V3D_Y;
    m2.z = m.z;
    return m2;
}

_AFXINL afxM3d AfxM3dDofXY(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = m.x;
    m2.y = m.y;
    m2.z = AFX_V3D_Z;
    return m2;
}

_AFXINL afxM3d AfxM3dDofXZ(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = m.x;
    m2.y = AFX_V3D_Y;
    m2.z = m.z;
    return m2;
}

_AFXINL afxM3d AfxM3dDofYZ(afxM3d const m)
{
    afxError err = { 0 };

    afxM3d m2;
    m2.x = AFX_V3D_X;
    m2.y = m.y;
    m2.z = m.z;
    return m2;
}
