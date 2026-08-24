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

#ifndef AFX_EXPONENTIAL_H
#define AFX_EXPONENTIAL_H

#include "qwadro/math/afxVector.h"

////////////////////////////////////////////////////////////////////////////////
// EXTENDED ARITHMETICA                                                       //
////////////////////////////////////////////////////////////////////////////////

// Math functions per vector lane.

// v = sqrt(in)

AFXINL afxV2d     AfxV2dSqrt(afxV2d const in); // v = sqrt(in)
AFXINL afxV3d     AfxV3dSqrt(afxV3d const in); // v = sqrt(in)
AFXINL afxV4d     AfxV4dSqrt(afxV4d const in); // v = sqrt(in)

// v = 1 / sqrt(in)

AFXINL afxV2d     AfxV2dRsqrt(afxV2d const in); // v = 1 / sqrt(in)
AFXINL afxV3d     AfxV3dRsqrt(afxV3d const in); // v = 1 / sqrt(in)
AFXINL afxV4d     AfxV4dRsqrt(afxV4d const in); // v = 1 / sqrt(in)

////////////////////////////////////////////////////////////////////////////////
// Clamping                                                                   //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxV2d     AfxV2dClamp(afxV2d const in, afxV2d const min, afxV2d const max); // v = IN between bounds
AFXINL afxV3d     AfxV3dClamp(afxV3d const in, afxV3d const min, afxV3d const max);
AFXINL afxV4d     AfxV4dClamp(afxV4d const in, afxV4d const min, afxV4d const max);

// This operation clamps a value between 0 and 1. If the value is below 0, it is set to 0; if above 1, it is set to 1. 
// Otherwise, it remains unchanged.
// Often used in graphics for color values, blending factors, or ensuring values stay within a specific range.

AFXINL afxV2d     AfxV2dSaturate(afxV2d const in);
AFXINL afxV3d     AfxV3dSaturate(afxV3d const in);
AFXINL afxV4d     AfxV4dSaturate(afxV4d const in);

AFXINL afxV2d     AfxV2dMin(afxV2d const a, afxV2d const b); // v = minor between A and B
AFXINL afxV3d     AfxV3dMin(afxV3d const a, afxV3d const b); // v = minor between A and B
AFXINL afxV4d     AfxV4dMin(afxV4d const a, afxV4d const b); // v = minor between A and B

AFXINL afxV2d     AfxV2dMax(afxV2d const a, afxV2d const b); // v = major between A and B
AFXINL afxV3d     AfxV3dMax(afxV3d const a, afxV3d const b); // v = major between A and B
AFXINL afxV4d     AfxV4dMax(afxV4d const a, afxV4d const b); // v = major between A and B

AFXINL afxV2d     AfxV2dAbs(afxV2d const in); // v = abs(in)
AFXINL afxV3d     AfxV3dAbs(afxV3d const in); // v = abs(in)
AFXINL afxV4d     AfxV4dAbs(afxV4d const in); // v = abs(in)

AFXINL afxV2d     AfxV2dCeil(afxV2d const in); // v = ceil(in)
AFXINL afxV3d     AfxV3dCeil(afxV3d const in); // v = ceil(in)
AFXINL afxV4d     AfxV4dCeil(afxV4d const in); // v = ceil(in)

AFXINL afxV2d     AfxV2dFloor(afxV2d const in); // v = floor(in)
AFXINL afxV3d     AfxV3dFloor(afxV3d const in); // v = floor(in)
AFXINL afxV4d     AfxV4dFloor(afxV4d const in); // v = floor(in)

// Truncation removes the fractional part of a number, effectively rounding it towards zero.
// Converting floating-point values to integers or simplifying floating-point precision.

AFXINL afxV2d     AfxV2dTruncate(afxV2d const in);
AFXINL afxV3d     AfxV3dTruncate(afxV3d const in);
AFXINL afxV4d     AfxV4dTruncate(afxV4d const in);


// Mix
// The MIX function is conceptually similar to LERP but may involve additional functionality, such as more complex blending modes or different blending parameters. 
// This operation is very similar to lerp, but it often uses clamping or saturating the interpolation result. 
// It linearly blends two values (or vectors) based on a factor t.
// Mixing colors, blending two values in a weighted way.
// v = x * (1 - t) + y * t

AFXINL afxV2d     AfxV2dMix(afxV2d const x, afxV2d const y, afxReal t);
AFXINL afxV3d     AfxV3dMix(afxV3d const x, afxV3d const y, afxReal t);
AFXINL afxV4d     AfxV4dMix(afxV4d const x, afxV4d const y, afxReal t);

// Lerp
// LERP is a method to interpolate linearly between two values. 
// In the context of 4D vectors, it calculates the intermediate vector between a start and end vector based on a factor t ranging from 0.0 to 1.0.
// Linearly interpolates between two values (or vectors) based on a parameter t (usually between 0 and 1). 
// It blends two values in a straight line.
// Animation, transition effects, smooth movement between two points.
// x + t * (y - x)

AFXINL afxV2d     AfxV2dLerp(afxV2d const x, afxV2d const y, afxReal t);
AFXINL afxV3d     AfxV3dLerp(afxV3d const x, afxV3d const y, afxReal t);
AFXINL afxV4d     AfxV4dLerp(afxV4d const x, afxV4d const y, afxReal t);

////////////////////////////////////////////////////////////////////////////////
// EXPONENTIAL ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////

/*
    Exponential Functions
    Functions that compute the exponential of a number.

    Exponential: exp, exp2, exp10 (depending on the base)

    Logarithmic Exponential: log1p (for log(1+x))
*/

// Returns the natural exponentiation of x; e^x.

AFXINL afxReal64    AfxExp(afxReal64 s);
AFXINL afxReal      AfxExpf(afxReal s);

// Returns 2 raised to the x power; 2^x.

AFXINL afxReal64    AfxExp2(afxReal64 s);
AFXINL afxReal      AfxExp2f(afxReal s);

AFXINL afxReal64    AfxExp10(afxReal64 s);
AFXINL afxReal      AfxExp10f(afxReal s);

AFXINL afxV2d         AfxV2dExp(afxV2d const in);
AFXINL afxV3d         AfxV3dExp(afxV3d const in);
AFXINL afxV4d         AfxV4dExp(afxV4d const in);

AFXINL afxV2d         AfxV2dExp2(afxV2d const in);
AFXINL afxV3d         AfxV3dExp2(afxV3d const in);
AFXINL afxV4d         AfxV4dExp2(afxV4d const in);

AFXINL afxV2d         AfxV2dExp10(afxV2d const in);
AFXINL afxV3d         AfxV3dExp10(afxV3d const in);
AFXINL afxV4d         AfxV4dExp10(afxV4d const in);

 // Returns x raised to the y power; x^y.
 // Results are undefined if x < 0 or if x = 0 and y <= 0.

AFXINL afxReal64    AfxPow(afxReal64 base, afxReal64 exp);
AFXINL afxReal      AfxPowf(afxReal base, afxReal exp);

AFXINL afxV2d         AfxV2dPow(afxV2d const in);
AFXINL afxV3d         AfxV3dPow(afxV3d const in);
AFXINL afxV4d         AfxV4dPow(afxV4d const in);


////////////////////////////////////////////////////////////////////////////////
// LOGARITHMIC ARITHMETICA                                                    //
////////////////////////////////////////////////////////////////////////////////

/*
    Logarithmic Functions
    Functions that deal with logarithms.

    Natural Logarithm: ln
    Logarithm Base 10: log10
    Logarithm Base 2: log2
    General Logarithm: log (with specified base)
*/

// Returns the natural logarithm of x; returns the value y which satisfies the equation x = e^y.
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog(afxReal64 s);
AFXINL afxReal      AfxLogf(afxReal s);

// Returns the base 2 logarithm of x; returns the value y which satisfies the equation x = 2^y.
// Results are undefined if x <= 0.

AFXINL afxReal64    AfxLog2(afxReal64 s);
AFXINL afxReal      AfxLog2f(afxReal s);

AFXINL afxReal64    AfxLog10(afxReal64 s);
AFXINL afxReal      AfxLog10f(afxReal s);


AFXINL afxV2d         AfxV2dLog(afxV2d const in);
AFXINL afxV3d         AfxV3dLog(afxV3d const in);
AFXINL afxV4d         AfxV4dLog(afxV4d const in);

AFXINL afxV2d         AfxV2dLog2(afxV2d const in);
AFXINL afxV3d         AfxV3dLog2(afxV3d const in);
AFXINL afxV4d         AfxV4dLog2(afxV4d const in);

AFXINL afxV2d         AfxV2dLog10(afxV2d const in);
AFXINL afxV3d         AfxV3dLog10(afxV3d const in);
AFXINL afxV4d         AfxV4dLog10(afxV4d const in);


#endif//AFX_EXPONENTIAL_H
