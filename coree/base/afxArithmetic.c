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

#include "qwadro/math/afxTrigonometry.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// Add

_AFXINL void AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    v[0] = a[0] + add[0];
    v[1] = a[1] + add[1];
}

_AFXINL void AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    v[0] = a[0] + add[0];
    v[1] = a[1] + add[1];
    v[2] = a[2] + add[2];
}

_AFXINL void AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    v[0] = a[0] + add[0];
    v[1] = a[1] + add[1];
    v[2] = a[2] + add[2];
    v[3] = a[3] + add[3];
}

// Sub

_AFXINL void AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const sub)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);
    v[0] = a[0] - sub[0];
    v[1] = a[1] - sub[1];
}

_AFXINL void AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const sub)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);
    v[0] = a[0] - sub[0];
    v[1] = a[1] - sub[1];
    v[2] = a[2] - sub[2];
}

_AFXINL void AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const sub)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);
    v[0] = a[0] - sub[0];
    v[1] = a[1] - sub[1];
    v[2] = a[2] - sub[2];
    v[3] = a[3] - sub[3];
}

// Div

_AFXINL void AfxV2dDiv(afxV2d v, afxV2d const a, afxV2d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div[0];
    v[1] = a[1] / div[1];
}

_AFXINL void AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div[0];
    v[1] = a[1] / div[1];
    v[2] = a[2] / div[2];
}

_AFXINL void AfxV4dDiv(afxV4d v, afxV4d const a, afxV4d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div[0];
    v[1] = a[1] / div[1];
    v[2] = a[2] / div[2];
    v[3] = a[3] / div[3];
}

// Divs

_AFXINL void AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
}

_AFXINL void AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
}

_AFXINL void AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);
    v[0] = a[0] / div;
    v[1] = a[1] / div;
    v[2] = a[2] / div;
    v[3] = a[3] / div;
}

// Mod

_AFXINL void AfxV2dMod(afxV2d v, afxV2d const a, afxV2d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);

    AfxV2dDiv(v, a, div);
    AfxTruncateV2d(v, v);
    AfxV2dResub(v, div, v, a);
}

_AFXINL void AfxV3dMod(afxV3d v, afxV3d const a, afxV3d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);

    AfxV3dDiv(v, a, div);
    AfxTruncateV3d(v, v);
    AfxV3dResub(v, div, v, a);
}

_AFXINL void AfxV4dMod(afxV4d v, afxV4d const a, afxV4d const div)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(div);

    AfxV4dDiv(v, a, div);
    AfxTruncateV4d(v, v);
    AfxV4dResub(v, div, v, a);
}

// Scale

_AFXINL void AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
}

_AFXINL void AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
}

_AFXINL void AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    v[0] = a[0] * lambda;
    v[1] = a[1] * lambda;
    v[2] = a[2] * lambda;
    v[3] = a[3] * lambda;
}

// Half

_AFXINL void AfxV2dHalf(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV2dMultiply(v, in, AFX_V2D_S(0.5));
}

_AFXINL void AfxV3dHalf(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV3dMultiply(v, in, AFX_V3D_S(0.5));
}

_AFXINL void AfxV4dHalf(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV4dMultiply(v, in, AFX_V4D_S(0.5));
}

// Multiply

_AFXINL void AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
}

_AFXINL void AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
}

_AFXINL void AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    v[0] = a[0] * b[0];
    v[1] = a[1] * b[1];
    v[2] = a[2] * b[2];
    v[3] = a[3] * b[3];
}

// Mad

_AFXINL void AfxV2dMad(afxV2d v, afxV2d const add, afxV2d const mul, afxV2d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
}

_AFXINL void AfxV3dMad(afxV3d v, afxV3d const add, afxV3d const mul, afxV3d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
    v[2] = add[2] + (mul[2] * f[2]);
}

_AFXINL void AfxV4dMad(afxV4d v, afxV4d const add, afxV4d const mul, afxV4d const f)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(mul);
    AFX_ASSERT(f);
    v[0] = add[0] + (mul[0] * f[0]);
    v[1] = add[1] + (mul[1] * f[1]);
    v[2] = add[2] + (mul[2] * f[2]);
    v[3] = add[3] + (mul[3] * f[3]);
}

// AddScaled

_AFXINL void AfxV2dMads(afxV2d v, afxV2d const add, afxV2d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
}

_AFXINL void AfxV3dMads(afxV3d v, afxV3d const add, afxV3d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
    v[2] = add[2] + (mul[2] * lambda);
}

_AFXINL void AfxV4dMads(afxV4d v, afxV4d const add, afxV4d const mul, afxReal lambda)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(mul);
    AFX_ASSERT(add);
    v[0] = add[0] + (mul[0] * lambda);
    v[1] = add[1] + (mul[1] * lambda);
    v[2] = add[2] + (mul[2] * lambda);
    v[3] = add[3] + (mul[3] * lambda);
}

// Mad

_AFXINL void AfxV2dMad2(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
}

_AFXINL void AfxV3dMad2(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
    v[2] = add[2] + (a[2] * mul[2]);
}

_AFXINL void AfxV4dMad2(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(add);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
    v[2] = add[2] + (a[2] * mul[2]);
    v[3] = add[3] + (a[3] * mul[3]);
}

// AddScaled

_AFXINL void AfxV2dMads2(afxV2d v, afxV2d const a, afxReal lambda, afxV2d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    afxV2d mul = { lambda, lambda };
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
}

_AFXINL void AfxV3dMads2(afxV3d v, afxV3d const a, afxReal lambda, afxV3d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    afxV3d mul = { lambda, lambda, lambda };
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
    v[2] = add[2] + (a[2] * mul[2]);
}

_AFXINL void AfxV4dMads2(afxV4d v, afxV4d const a, afxReal lambda, afxV4d const add)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(add);
    afxV4d mul = { lambda, lambda, lambda, lambda };
    v[0] = add[0] + (a[0] * mul[0]);
    v[1] = add[1] + (a[1] * mul[1]);
    v[2] = add[2] + (a[2] * mul[2]);
    v[3] = add[3] + (a[3] * mul[3]);
}

// Resub

_AFXINL void AfxV2dResub(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
}

_AFXINL void AfxV3dResub(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
}

_AFXINL void AfxV4dResub(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
    v[3] = sub[3] - (a[3] * mul[3]);
}

// Musub

_AFXINL void AfxV2dMusub(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
}

_AFXINL void AfxV3dMusub(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
}

_AFXINL void AfxV4dMusub(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(mul);
    AFX_ASSERT(sub);

    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
    v[3] = sub[3] - (a[3] * mul[3]);
}

// Musubs

_AFXINL void AfxV2dMusubs(afxV2d v, afxV2d const a, afxReal lambda, afxV2d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);

    afxV2d mul = { lambda, lambda };
    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
}

_AFXINL void AfxV3dMusubs(afxV3d v, afxV3d const a, afxReal lambda, afxV3d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);

    afxV3d mul = { lambda, lambda, lambda };
    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
}

_AFXINL void AfxV4dMusubs(afxV4d v, afxV4d const a, afxReal lambda, afxV4d const sub)
{
    // Should be compatible with XMVectorNegativeMultiplySubtract

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(a);
    AFX_ASSERT(sub);

    afxV4d mul = { lambda, lambda, lambda, lambda };
    v[0] = sub[0] - (a[0] * mul[0]);
    v[1] = sub[1] - (a[1] * mul[1]);
    v[2] = sub[2] - (a[2] * mul[2]);
    v[3] = sub[3] - (a[3] * mul[3]);
}

// Recip

_AFXINL void AfxV2dRecip(afxV2d v, afxV2d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV2dDiv(v, AFX_V2D_ONE, in);
}

_AFXINL void AfxV3dRecip(afxV3d v, afxV3d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV3dDiv(v, AFX_V3D_ONE, in);
}

_AFXINL void AfxV4dRecip(afxV4d v, afxV4d const in)
{
    // Should be compatible with XMVECTOR XMVectorReciprocal(FXMVECTOR V)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    AfxV4dDiv(v, AFX_V4D_ONE, in);
}

// Negate

_AFXINL void AfxV2dNeg(afxV2d v, afxV2d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
}

_AFXINL void AfxV3dNeg(afxV3d v, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
}

_AFXINL void AfxV4dNeg(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = -(in[3]);
}

_AFXINL void AfxV4dNegAffine(afxV4d v, afxV4d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(in);
    v[0] = -(in[0]);
    v[1] = -(in[1]);
    v[2] = -(in[2]);
    v[3] = (in[3]);
}


