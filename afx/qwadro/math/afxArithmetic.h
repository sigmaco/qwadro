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
    Basic Arithmetic Functions
    These functions perform basic arithmetic operations.
    Addition: add, sum
    Subtraction: subtract
    Multiplication: multiply, product
    Division: divide, quotient
    Modulus: mod, remainder
    Exponentiation: pow, power, exponent
*/

#ifndef AFX_ARITHMETIC_H
#define AFX_ARITHMETIC_H

#include "qwadro/math/afxVector.h"


////////////////////////////////////////////////////////////////////////////////
// Arithmetica                                                                //
////////////////////////////////////////////////////////////////////////////////

// v = a + b

AFXINL void     AfxV2dAdd(afxV2d v, afxV2d const a, afxV2d const add);
AFXINL void     AfxV3dAdd(afxV3d v, afxV3d const a, afxV3d const add);
AFXINL void     AfxV4dAdd(afxV4d v, afxV4d const a, afxV4d const add);

// v = a - b

AFXINL void     AfxV2dSub(afxV2d v, afxV2d const a, afxV2d const sub); // v = a - sub
AFXINL void     AfxV3dSub(afxV3d v, afxV3d const a, afxV3d const sub); // v = a - sub
AFXINL void     AfxV4dSub(afxV4d v, afxV4d const a, afxV4d const sub); // v = a - sub

// v = a / b

AFXINL void     AfxV2dDiv(afxV2d v, afxV2d const a, afxV2d const div); // v = a / div
AFXINL void     AfxV3dDiv(afxV3d v, afxV3d const a, afxV3d const div); // v = a / div
AFXINL void     AfxV4dDiv(afxV4d v, afxV4d const a, afxV4d const div); // v = a / div


// v = a / div

AFXINL void     AfxV2dDivs(afxV2d v, afxV2d const a, afxReal div);
AFXINL void     AfxV3dDivs(afxV3d v, afxV3d const a, afxReal div);
AFXINL void     AfxV4dDivs(afxV4d v, afxV4d const a, afxReal div);

// v = in * 0.5

AFXINL void     AfxV2dHalf(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dHalf(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dHalf(afxV4d v, afxV4d const in);

// v = a % b

AFXINL void     AfxV2dMod(afxV2d v, afxV2d const a, afxV2d const div); // v = a % div
AFXINL void     AfxV3dMod(afxV3d v, afxV3d const a, afxV3d const div); // v = a % div
AFXINL void     AfxV4dMod(afxV4d v, afxV4d const a, afxV4d const div); // v = a % div

// v = a * b

AFXINL void     AfxV2dMultiply(afxV2d v, afxV2d const a, afxV2d const mul); // v = a * mul
AFXINL void     AfxV3dMultiply(afxV3d v, afxV3d const a, afxV3d const mul); // v = a * mul
AFXINL void     AfxV4dMultiply(afxV4d v, afxV4d const a, afxV4d const mul); // v = a * mul

// Scales (or multiplies) a vector or scalar by a constant factor.
// Resizing objects, scaling forces, enlarging or shrinking vectors.
// v = a * lambda

AFXINL void     AfxV2dScale(afxV2d v, afxV2d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV3dScale(afxV3d v, afxV3d const a, afxReal lambda); // v = a * lambda
AFXINL void     AfxV4dScale(afxV4d v, afxV4d const a, afxReal lambda); // v = a * lambda

// vector-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.

// v = add + (mul * f)

AFXINL void     AfxV2dMad(afxV2d v, afxV2d const add, afxV2d const mul, afxV2d const f); // v = add + (mul * f)
AFXINL void     AfxV3dMad(afxV3d v, afxV3d const add, afxV3d const mul, afxV3d const f); // v = add + (mul * f)
AFXINL void     AfxV4dMad(afxV4d v, afxV4d const add, afxV4d const mul, afxV4d const f); // v = add + (mul * f)

// scalar-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.

// v = add + (mul * lambda)

AFXINL void     AfxV2dMads(afxV2d v, afxV2d const add, afxV2d const mul, afxReal lambda); // v = add + (mul * lambda)
AFXINL void     AfxV3dMads(afxV3d v, afxV3d const add, afxV3d const mul, afxReal lambda); // v = add + (mul * lambda)
AFXINL void     AfxV4dMads(afxV4d v, afxV4d const add, afxV4d const mul, afxReal lambda); // v = add + (mul * lambda)


// Vector-Multiply-Accumulate

AFXINL void AfxV2dMad2(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const add); // v = add + (a * mul)
AFXINL void AfxV3dMad2(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const add); // v = add + (a * mul)
AFXINL void AfxV4dMad2(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const add); // v = add + (a * mul)

// Scalar-Multiply-Accumulate

AFXINL void AfxV2dMads2(afxV2d v, afxV2d const a, afxReal lambda, afxV2d const add); // v = add + (a * lambda)
AFXINL void AfxV3dMads2(afxV3d v, afxV3d const a, afxReal lambda, afxV3d const add); // v = add + (a * lambda)
AFXINL void AfxV4dMads2(afxV4d v, afxV4d const a, afxReal lambda, afxV4d const add); // v = add + (a * lambda)

// reverse subtract
// v = sub - (a * mul)

AFXINL void     AfxV2dResub(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const sub); // v = sub - (a * mul)
AFXINL void     AfxV3dResub(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const sub); // v = sub - (a * mul)
AFXINL void     AfxV4dResub(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const sub); // v = sub - (a * mul)

// Musub

AFXINL void AfxV2dMusub(afxV2d v, afxV2d const a, afxV2d const mul, afxV2d const sub); // v = sub - (a * mul)
AFXINL void AfxV3dMusub(afxV3d v, afxV3d const a, afxV3d const mul, afxV3d const sub); // v = sub - (a * mul)
AFXINL void AfxV4dMusub(afxV4d v, afxV4d const a, afxV4d const mul, afxV4d const sub); // v = sub - (a * mul)

// Musubs

AFXINL void AfxV2dMusubs(afxV2d v, afxV2d const a, afxReal lambda, afxV2d const sub);
AFXINL void AfxV3dMusubs(afxV3d v, afxV3d const a, afxReal lambda, afxV3d const sub);
AFXINL void AfxV4dMusubs(afxV4d v, afxV4d const a, afxReal lambda, afxV4d const sub);

// Recip

AFXINL void     AfxV2dRecip(afxV2d v, afxV2d const in); // v = 1 / in
AFXINL void     AfxV3dRecip(afxV3d v, afxV3d const in); // v = 1 / in
AFXINL void     AfxV4dRecip(afxV4d v, afxV4d const in); // v = 1 / in

// Negates a vector or scalar, flipping its sign. For a vector, it inverts the direction of the vector.
// Used in physics simulations, reversing directions, negating forces, etc.

AFXINL void     AfxV2dNeg(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dNeg(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dNeg(afxV4d v, afxV4d const in);
AFXINL void     AfxV4dNegAffine(afxV4d v, afxV4d const in); // v = inverse of in


#endif//AFX_ARITHMETIC_H
