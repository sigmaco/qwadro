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

AFXINL afxV2d     AfxV2dAdd(afxV2d const a, afxV2d const add);
AFXINL afxV3d     AfxV3dAdd(afxV3d const a, afxV3d const add);
AFXINL afxV4d     AfxV4dAdd(afxV4d const a, afxV4d const add);

// v = a - b

AFXINL afxV2d     AfxV2dSub(afxV2d const a, afxV2d const sub); // v = a - sub
AFXINL afxV3d     AfxV3dSub(afxV3d const a, afxV3d const sub); // v = a - sub
AFXINL afxV4d     AfxV4dSub(afxV4d const a, afxV4d const sub); // v = a - sub

// v = a / b

AFXINL afxV2d     AfxV2dDiv(afxV2d const a, afxV2d const div); // v = a / div
AFXINL afxV3d     AfxV3dDiv(afxV3d const a, afxV3d const div); // v = a / div
AFXINL afxV4d     AfxV4dDiv(afxV4d const a, afxV4d const div); // v = a / div


// v = a / div

AFXINL afxV2d     AfxV2dDivs(afxV2d const a, afxReal div);
AFXINL afxV3d     AfxV3dDivs(afxV3d const a, afxReal div);
AFXINL afxV4d     AfxV4dDivs(afxV4d const a, afxReal div);

// v = in * 0.5

AFXINL afxV2d     AfxV2dHalf(afxV2d const in);
AFXINL afxV3d     AfxV3dHalf(afxV3d const in);
AFXINL afxV4d     AfxV4dHalf(afxV4d const in);

// v = a % b

AFXINL afxV2d     AfxV2dMod(afxV2d const a, afxV2d const div); // v = a % div
AFXINL afxV3d     AfxV3dMod(afxV3d const a, afxV3d const div); // v = a % div
AFXINL afxV4d     AfxV4dMod(afxV4d const a, afxV4d const div); // v = a % div

// v = a * b

AFXINL afxV2d     AfxV2dMultiply(afxV2d const a, afxV2d const mul); // v = a * mul
AFXINL afxV3d     AfxV3dMultiply(afxV3d const a, afxV3d const mul); // v = a * mul
AFXINL afxV4d     AfxV4dMultiply(afxV4d const a, afxV4d const mul); // v = a * mul

// Scales (or multiplies) a vector or scalar by a constant factor.
// Resizing objects, scaling forces, enlarging or shrinking vectors.
// v = a * lambda

AFXINL afxV2d     AfxV2dScale(afxV2d const a, afxReal lambda); // v = a * lambda
AFXINL afxV3d     AfxV3dScale(afxV3d const a, afxReal lambda); // v = a * lambda
AFXINL afxV4d     AfxV4dScale(afxV4d const a, afxReal lambda); // v = a * lambda

// vector-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.

// v = add + (mul * f)

AFXINL afxV2d     AfxV2dMad(afxV2d const add, afxV2d const mul, afxV2d const f); // v = add + (mul * f)
AFXINL afxV3d     AfxV3dMad(afxV3d const add, afxV3d const mul, afxV3d const f); // v = add + (mul * f)
AFXINL afxV4d     AfxV4dMad(afxV4d const add, afxV4d const mul, afxV4d const f); // v = add + (mul * f)

// scalar-multiply-add (accumulate)
// A common operation that performs (A * B) + C, combining multiplication and addition in a single step. 
// It is often used in shaders and numerical algorithms to reduce computation time.
// Efficient calculation in shaders, physics simulations, and financial models.

// v = add + (mul * lambda)

AFXINL afxV2d     AfxV2dMads(afxV2d const add, afxV2d const mul, afxReal lambda); // v = add + (mul * lambda)
AFXINL afxV3d     AfxV3dMads(afxV3d const add, afxV3d const mul, afxReal lambda); // v = add + (mul * lambda)
AFXINL afxV4d     AfxV4dMads(afxV4d const add, afxV4d const mul, afxReal lambda); // v = add + (mul * lambda)


// Vector-Multiply-Accumulate

AFXINL afxV2d AfxV2dMad2(afxV2d const a, afxV2d const mul, afxV2d const add); // v = add + (a * mul)
AFXINL afxV3d AfxV3dMad2(afxV3d const a, afxV3d const mul, afxV3d const add); // v = add + (a * mul)
AFXINL afxV4d AfxV4dMad2(afxV4d const a, afxV4d const mul, afxV4d const add); // v = add + (a * mul)

// Scalar-Multiply-Accumulate

AFXINL afxV2d AfxV2dMads2(afxV2d const a, afxReal lambda, afxV2d const add); // v = add + (a * lambda)
AFXINL afxV3d AfxV3dMads2(afxV3d const a, afxReal lambda, afxV3d const add); // v = add + (a * lambda)
AFXINL afxV4d AfxV4dMads2(afxV4d const a, afxReal lambda, afxV4d const add); // v = add + (a * lambda)

// reverse subtract
// v = sub - (a * mul)

AFXINL afxV2d     AfxV2dResub(afxV2d const a, afxV2d const mul, afxV2d const sub); // v = sub - (a * mul)
AFXINL afxV3d     AfxV3dResub(afxV3d const a, afxV3d const mul, afxV3d const sub); // v = sub - (a * mul)
AFXINL afxV4d     AfxV4dResub(afxV4d const a, afxV4d const mul, afxV4d const sub); // v = sub - (a * mul)

// Musub

AFXINL afxV2d AfxV2dMusub(afxV2d const a, afxV2d const mul, afxV2d const sub); // v = sub - (a * mul)
AFXINL afxV3d AfxV3dMusub(afxV3d const a, afxV3d const mul, afxV3d const sub); // v = sub - (a * mul)
AFXINL afxV4d AfxV4dMusub(afxV4d const a, afxV4d const mul, afxV4d const sub); // v = sub - (a * mul)

// Musubs

AFXINL afxV2d AfxV2dMusubs(afxV2d const a, afxReal lambda, afxV2d const sub);
AFXINL afxV3d AfxV3dMusubs(afxV3d const a, afxReal lambda, afxV3d const sub);
AFXINL afxV4d AfxV4dMusubs(afxV4d const a, afxReal lambda, afxV4d const sub);

// Recip

AFXINL afxV2d     AfxV2dRecip(afxV2d const in); // v = 1 / in
AFXINL afxV3d     AfxV3dRecip(afxV3d const in); // v = 1 / in
AFXINL afxV4d     AfxV4dRecip(afxV4d const in); // v = 1 / in

// Negates a vector or scalar, flipping its sign. For a vector, it inverts the direction of the vector.
// Used in physics simulations, reversing directions, negating forces, etc.

AFXINL afxV2d     AfxV2dNeg(afxV2d const in);
AFXINL afxV3d     AfxV3dNeg(afxV3d const in);
AFXINL afxV4d     AfxV4dNeg(afxV4d const in);
AFXINL afxV4d     AfxV4dNegAffine(afxV4d const in); // v = inverse of in


#endif//AFX_ARITHMETIC_H
