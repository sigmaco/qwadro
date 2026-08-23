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

#include "qwadro/math/afxTrigonometry.h"
#include "qwadro/math/afxArithmetic2.h"
#include "qwadro/math/afxArithmetic.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL afxV2d AfxV2dAdd(afxV2d const a, afxV2d const add)
{
    afxError err = { 0 };
    return AFX_V2D( a.x + add.x,
                    a.y + add.y);
}

_AFXINL afxV3d AfxV3dAdd(afxV3d const a, afxV3d const add)
{
    afxError err = { 0 };
    return AFX_V3D( a.x + add.x,
                    a.y + add.y,
                    a.z + add.z);
}

_AFXINL afxV4d AfxV4dAdd(afxV4d const a, afxV4d const add)
{
    afxError err = { 0 };
    return AFX_V4D( a.x + add.x,
                    a.y + add.y,
                    a.z + add.z,
                    a.w + add.w);
}

// Sub

_AFXINL afxV2d AfxV2dSub(afxV2d const a, afxV2d const sub)
{
    afxError err = { 0 };
    return AFX_V2D( a.x - sub.x,
                    a.y - sub.y);
}

_AFXINL afxV3d AfxV3dSub(afxV3d const a, afxV3d const sub)
{
    afxError err = { 0 };
    return AFX_V3D( a.x - sub.x,
                    a.y - sub.y,
                    a.z - sub.z);
}

_AFXINL afxV4d AfxV4dSub(afxV4d const a, afxV4d const sub)
{
    afxError err = { 0 };
    return AFX_V4D( a.x - sub.x,
                    a.y - sub.y,
                    a.z - sub.z,
                    a.w - sub.w);
}

// Div

_AFXINL afxV2d AfxV2dDiv(afxV2d const a, afxV2d const div)
{
    afxError err = { 0 };
    return AFX_V2D( a.x / div.x,
                    a.y / div.y);
}

_AFXINL afxV3d AfxV3dDiv(afxV3d const a, afxV3d const div)
{
    afxError err = { 0 };
    return AFX_V3D( a.x / div.x,
                    a.y / div.y,
                    a.z / div.z);
}

_AFXINL afxV4d AfxV4dDiv(afxV4d const a, afxV4d const div)
{
    afxError err = { 0 };
    return AFX_V4D( a.x / div.x,
                    a.y / div.y,
                    a.z / div.z,
                    a.w / div.w);
}

// Divs

_AFXINL afxV2d AfxV2dDivs(afxV2d const a, afxReal div)
{
    afxError err = { 0 };
    return AFX_V2D( a.x / div,
                    a.y / div);
}

_AFXINL afxV3d AfxV3dDivs(afxV3d const a, afxReal div)
{
    afxError err = { 0 };
    return AFX_V3D( a.x / div,
                    a.y / div,
                    a.z / div);
}

_AFXINL afxV4d AfxV4dDivs(afxV4d const a, afxReal div)
{
    afxError err = { 0 };
    return AFX_V4D( a.x / div,
                    a.y / div,
                    a.z / div,
                    a.w / div);
}

// Mod

_AFXINL afxV2d AfxV2dMod(afxV2d const a, afxV2d const div)
{
    afxError err = { 0 };
    afxV2d v = AfxV2dDiv(a, div);
    v = AfxV2dTruncate(v);
    return AfxV2dResub(div, v, a);
}

_AFXINL afxV3d AfxV3dMod(afxV3d const a, afxV3d const div)
{
    afxError err = { 0 };
    afxV3d v = AfxV3dDiv(a, div);
    v = AfxV3dTruncate(v);
    return AfxV3dResub(div, v, a);
}

_AFXINL afxV4d AfxV4dMod(afxV4d const a, afxV4d const div)
{
    afxError err = { 0 };
    afxV4d v = AfxV4dDiv(a, div);
    v = AfxV4dTruncate(v);
    return AfxV4dResub(div, v, a);
}

// Scale

_AFXINL afxV2d AfxV2dScale(afxV2d const a, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V2D( a.x * lambda,
                    a.y * lambda);
}

_AFXINL afxV3d AfxV3dScale(afxV3d const a, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V3D( a.x * lambda,
                    a.y * lambda,
                    a.z * lambda);
}

_AFXINL afxV4d AfxV4dScale(afxV4d const a, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V4D( a.x * lambda,
                    a.y * lambda,
                    a.z * lambda,
                    a.w * lambda);
}

// Half

_AFXINL afxV2d AfxV2dHalf(afxV2d const in)
{
    afxError err = { 0 };
    return AfxV2dMultiply(in, AFX_V2D_S(0.5));
}

_AFXINL afxV3d AfxV3dHalf(afxV3d const in)
{
    afxError err = { 0 };
    return AfxV3dMultiply(in, AFX_V3D_S(0.5));
}

_AFXINL afxV4d AfxV4dHalf(afxV4d const in)
{
    afxError err = { 0 };
    return AfxV4dMultiply(in, AFX_V4D_S(0.5));
}

// Multiply

_AFXINL afxV2d AfxV2dMultiply(afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    return AFX_V2D( a.x * b.x,
                    a.y * b.y);
}

_AFXINL afxV3d AfxV3dMultiply(afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    return AFX_V3D( a.x * b.x,
                    a.y * b.y,
                    a.z * b.z);
}

_AFXINL afxV4d AfxV4dMultiply(afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    return AFX_V4D( a.x * b.x,
                    a.y * b.y,
                    a.z * b.z,
                    a.w * b.w);
}

// Mad

_AFXINL afxV2d AfxV2dMad(afxV2d const add, afxV2d const mul, afxV2d const f)
{
    afxError err = { 0 };
    return AFX_V2D( add.x + (mul.x * f.x),
                    add.y + (mul.y * f.y));
}

_AFXINL afxV3d AfxV3dMad(afxV3d const add, afxV3d const mul, afxV3d const f)
{
    afxError err = { 0 };
    return AFX_V3D( add.x + (mul.x * f.x),
                    add.y + (mul.y * f.y),
                    add.z + (mul.z * f.z));
}

_AFXINL afxV4d AfxV4dMad(afxV4d const add, afxV4d const mul, afxV4d const f)
{
    afxError err = { 0 };
    return AFX_V4D( add.x + (mul.x * f.x),
                    add.y + (mul.y * f.y),
                    add.z + (mul.z * f.z),
                    add.w + (mul.w * f.w));
}

// AddScaled

_AFXINL afxV2d AfxV2dMads(afxV2d const add, afxV2d const mul, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V2D( add.x + (mul.x * lambda),
                    add.y + (mul.y * lambda));
}

_AFXINL afxV3d AfxV3dMads(afxV3d const add, afxV3d const mul, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V3D( add.x + (mul.x * lambda),
                    add.y + (mul.y * lambda),
                    add.z + (mul.z * lambda));
}

_AFXINL afxV4d AfxV4dMads(afxV4d const add, afxV4d const mul, afxReal lambda)
{
    afxError err = { 0 };
    return AFX_V4D( add.x + (mul.x * lambda),
                    add.y + (mul.y * lambda),
                    add.z + (mul.z * lambda),
                    add.w + (mul.w * lambda));
}

// Mad

_AFXINL afxV2d AfxV2dMad2(afxV2d const a, afxV2d const mul, afxV2d const add)
{
    afxError err = { 0 };
    return AFX_V2D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y));
}

_AFXINL afxV3d AfxV3dMad2(afxV3d const a, afxV3d const mul, afxV3d const add)
{
    afxError err = { 0 };
    return AFX_V3D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y),
                    add.z + (a.z * mul.z));
}

_AFXINL afxV4d AfxV4dMad2(afxV4d const a, afxV4d const mul, afxV4d const add)
{
    afxError err = { 0 };
    return AFX_V4D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y),
                    add.z + (a.z * mul.z),
                    add.w + (a.w * mul.w));
}

// AddScaled

_AFXINL afxV2d AfxV2dMads2(afxV2d const a, afxReal lambda, afxV2d const add)
{
    afxError err = { 0 };
    afxV2d mul = { lambda, lambda };
    return AFX_V2D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y));
}

_AFXINL afxV3d AfxV3dMads2(afxV3d const a, afxReal lambda, afxV3d const add)
{
    afxError err = { 0 };
    afxV3d mul = { lambda, lambda, lambda };
    return AFX_V3D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y),
                    add.z + (a.z * mul.z));
}

_AFXINL afxV4d AfxV4dMads2(afxV4d const a, afxReal lambda, afxV4d const add)
{
    afxError err = { 0 };
    afxV4d mul = { lambda, lambda, lambda, lambda };
    return AFX_V4D( add.x + (a.x * mul.x),
                    add.y + (a.y * mul.y),
                    add.z + (a.z * mul.z),
                    add.w + (a.w * mul.w));
}

// Resub

_AFXINL afxV2d AfxV2dResub(afxV2d const a, afxV2d const mul, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V2D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y));
}

_AFXINL afxV3d AfxV3dResub(afxV3d const a, afxV3d const mul, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V3D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z));
}

_AFXINL afxV4d AfxV4dResub(afxV4d const a, afxV4d const mul, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V4D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z),
                    sub.w - (a.w * mul.w));
}

// Musub

_AFXINL afxV2d AfxV2dMusub(afxV2d const a, afxV2d const mul, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V2D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y));
}

_AFXINL afxV3d AfxV3dMusub(afxV3d const a, afxV3d const mul, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V3D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z));
}

_AFXINL afxV4d AfxV4dMusub(afxV4d const a, afxV4d const mul, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    return AFX_V4D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z),
                    sub.w - (a.w * mul.w));
}

// Musubs

_AFXINL afxV2d AfxV2dMusubs(afxV2d const a, afxReal lambda, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    afxV2d mul = { lambda, lambda };
    return AFX_V2D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y));
}

_AFXINL afxV3d AfxV3dMusubs(afxV3d const a, afxReal lambda, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    afxV3d mul = { lambda, lambda, lambda };
    return AFX_V3D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z));
}

_AFXINL afxV4d AfxV4dMusubs(afxV4d const a, afxReal lambda, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    afxV4d mul = { lambda, lambda, lambda, lambda };
    return AFX_V4D( sub.x - (a.x * mul.x),
                    sub.y - (a.y * mul.y),
                    sub.z - (a.z * mul.z),
                    sub.w - (a.w * mul.w));
}

// Recip

_AFXINL afxV2d AfxV2dRecip(afxV2d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    return AfxV2dDiv(AFX_V2D_ONE, in);
}

_AFXINL afxV3d AfxV3dRecip(afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    return AfxV3dDiv(AFX_V3D_ONE, in);
}

_AFXINL afxV4d AfxV4dRecip(afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    return AfxV4dDiv(AFX_V4D_ONE, in);
}

// Negate

_AFXINL afxV2d AfxV2dNeg(afxV2d const in)
{
    afxError err = { 0 };
    return AFX_V2D( -(in.x),
                    -(in.y));
}

_AFXINL afxV3d AfxV3dNeg(afxV3d const in)
{
    afxError err = { 0 };
    return AFX_V3D( -(in.x),
                    -(in.y),
                    -(in.z));
}

_AFXINL afxV4d AfxV4dNeg(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( -(in.x),
                    -(in.y),
                    -(in.z),
                    -(in.w));
}

_AFXINL afxV4d AfxV4dNegAffine(afxV4d const in)
{
    afxError err = { 0 };
    return AFX_V4D( -(in.x),
                    -(in.y),
                    -(in.z),
                     (in.w)); // not negative
}
