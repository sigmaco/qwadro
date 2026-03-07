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

#ifndef AFX_VECTOR_H
#define AFX_VECTOR_H

#include "qwadro/math/afxScalar.h"
#include "qwadro/math/afxArithmetic.h"
#include "qwadro/math/afxLogarithmic.h"
#include "qwadro/math/afxArithmetic2.h"
#include "qwadro/math/afxTrigonometry.h"

#define AFX_V2D(x_, y_) (afxV2d){ (afxReal)(x_), (afxReal)(y_) } 
#define AFX_V3D(x_, y_, z_) (afxV3d){ (afxReal)(x_), (afxReal)(y_), (afxReal)(z_) } 
#define AFX_V4D(x_, y_, z_, w_) (afxV4d){ (afxReal)(x_), (afxReal)(y_), (afxReal)(z_), (afxReal)(w_) } 
#define AFX_ATV3D(x_, y_) (afxV3d){ (afxReal)(x_), (afxReal)(y_), (afxReal)1 } 
#define AFX_ATV4D(x_, y_, z_) (afxV4d){ (afxReal)(x_), (afxReal)(y_), (afxReal)z_), (afxReal)1 } 
#define AFX_LTV4D(x_, y_, z_) (afxV4d){ (afxReal)(x_), (afxReal)(y_), (afxReal)z_), (afxReal)0 } 

#define AFX_V2D_S(x_) (afxV2d){ (afxReal)(x_), (afxReal)(x_) } 
#define AFX_V3D_S(x_) (afxV3d){ (afxReal)(x_), (afxReal)(x_), (afxReal)(x_) } 
#define AFX_V4D_S(x_) (afxV4d){ (afxReal)(x_), (afxReal)(x_), (afxReal)(x_), (afxReal)(x_) } 
#define AFX_ATV3D_S(x_) (afxV3d){ (afxReal)(x_), (afxReal)(x_), (afxReal)1 } 
#define AFX_ATV4D_S(x_) (afxV4d){ (afxReal)(x_), (afxReal)(x_), (afxReal)(x_), (afxReal)1 } 
#define AFX_LTV4D_S(x_) (afxV4d){ (afxReal)(x_), (afxReal)(x_), (afxReal)(x_), (afxReal)0 } 

#define AFX_V2D_X AFX_V2D(1, 0)
#define AFX_V2D_Y AFX_V2D(0, 1)

#define AFX_V2D_X_NEG AFX_V4D(-1, 0)
#define AFX_V2D_Y_NEG AFX_V2D( 0,-1)

#define AFX_V3D_X AFX_V3D(1, 0, 0)
#define AFX_V3D_Y AFX_V3D(0, 1, 0)
#define AFX_V3D_Z AFX_V3D(0, 0, 1)

#define AFX_V3D_X_NEG AFX_V4D(-1, 0, 0)
#define AFX_V3D_Y_NEG AFX_V3D( 0,-1, 0)
#define AFX_V3D_Z_NEG AFX_V3D( 0, 0,-1)

#define AFX_V4D_X AFX_V4D(1, 0, 0, 0)
#define AFX_V4D_Y AFX_V4D(0, 1, 0, 0)
#define AFX_V4D_Z AFX_V4D(0, 0, 1, 0)
#define AFX_V4D_W AFX_V4D(0, 0, 0, 1)

#define AFX_V4D_X_NEG AFX_V4D(-1, 0, 0, 0)
#define AFX_V4D_Y_NEG AFX_V4D( 0,-1, 0, 0)
#define AFX_V4D_Z_NEG AFX_V4D( 0, 0,-1, 0)
#define AFX_V4D_W_NEG AFX_V4D( 0, 0, 0,-1)

#define AFX_V2D_ONE AFX_V2D(1, 1)
#define AFX_V3D_ONE AFX_V3D(1, 1, 1)
#define AFX_V4D_ONE AFX_V4D(1, 1, 1, 1)
#define AFX_V4D_ONE_NEG AFX_V4D(-1,-1,-1,-1)
#define AFX_V4D_XYZ AFX_V4D(1, 1, 1, 0)

#define AFX_V2D_ZERO AFX_V2D(0, 0)
#define AFX_V3D_ZERO AFX_V3D(0, 0, 0)
#define AFX_V4D_ZERO AFX_V4D(0, 0, 0, 0)

#define AFX_V2D_HALF AFX_V2D(0.5, 0.5)
#define AFX_V3D_HALF AFX_V3D(0.5, 0.5, 0.5)
#define AFX_V4D_HALF AFX_V4D(0.5, 0.5, 0.5, 0.5)

#define AFX_V2D_MAX AFX_V2D(AFX_R32_MAX, AFX_R32_MAX)
#define AFX_V3D_MAX AFX_V3D(AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX)
#define AFX_V4D_MAX AFX_V4D(AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX, AFX_R32_MAX)

#define AFX_V2D_MIN AFX_V2D(-AFX_R32_MAX, -AFX_R32_MAX)
#define AFX_V3D_MIN AFX_V3D(-AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX)
#define AFX_V4D_MIN AFX_V4D(-AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX, -AFX_R32_MAX)

#define AFX_V2D_EPSILON AFX_V2D(AFX_EPSILON, AFX_EPSILON)
#define AFX_V3D_EPSILON AFX_V3D(AFX_EPSILON, AFX_EPSILON, AFX_EPSILON)
#define AFX_V4D_EPSILON AFX_V4D(AFX_EPSILON, AFX_EPSILON, AFX_EPSILON, AFX_EPSILON)

#define AFX_V2D_PI AFX_V2D(AFX_PI, AFX_PI)
#define AFX_V3D_PI AFX_V3D(AFX_PI, AFX_PI, AFX_PI)
#define AFX_V4D_PI AFX_V4D(AFX_PI, AFX_PI, AFX_PI, AFX_PI)

#define AFX_V2D_PI_OVER2 AFX_V2D(AFX_PI_OVER2, AFX_PI_OVER2)
#define AFX_V3D_PI_OVER2 AFX_V3D(AFX_PI_OVER2, AFX_PI_OVER2, AFX_PI_OVER2)
#define AFX_V4D_PI_OVER2 AFX_V4D(AFX_PI_OVER2, AFX_PI_OVER2, AFX_PI_OVER2, AFX_PI_OVER2)

#define AFX_V2D_PI_OVER_180 AFX_V2D(AFX_PI_OVER_180, AFX_PI_OVER_180)
#define AFX_V3D_PI_OVER_180 AFX_V3D(AFX_PI_OVER_180, AFX_PI_OVER_180, AFX_PI_OVER_180)
#define AFX_V4D_PI_OVER_180 AFX_V4D(AFX_PI_OVER_180, AFX_PI_OVER_180, AFX_PI_OVER_180, AFX_PI_OVER_180)

#define AFX_V2D_180_OVER_PI AFX_V2D(AFX_180_OVER_PI, AFX_180_OVER_PI)
#define AFX_V3D_180_OVER_PI AFX_V3D(AFX_180_OVER_PI, AFX_180_OVER_PI, AFX_180_OVER_PI)
#define AFX_V4D_180_OVER_PI AFX_V4D(AFX_180_OVER_PI, AFX_180_OVER_PI, AFX_180_OVER_PI, AFX_180_OVER_PI)

#define AFX_V4D_IDENTITY AFX_V4D(0, 0, 0, 1)

#define AFX_V(x) _Generic((x), \
    afxReal: (afxV4d){(x), (x), (x), (x)}, \
    afxV2d: (afxV4d){x[0], x[1], 0.0f, 0.0f}, \
    afxV3d: (afxV4d){x[0], x[1], x[2], 1.0f}) 

#define AFX_LTV(v) _Generic((v), \
    float: (afxV4d){(v), 0.f, 0.f, 0.f}, \
    afxReal: (afxV4d){v, 0.f, 0.f, 0.f}, \
    afxV2d:  (afxV4d){v[0], v[1], 0.0f, 0.0f}, \
    afxV3d:  (afxV4d){v[0], v[1], v[2], 0.0f}, \
    default: (afxV4d){(afxReal)v}) 

#define AFX_ATV(v) _Generic((v), \
    afxReal: (afxV4d){v, 0.f, 0.f, 1.f}, \
    afxV2d:  (afxV4d){v[0], v[1], 0.0f, 1.0f}, \
    afxV3d:  (afxV4d){v[0], v[1], v[2], 1.0f}, \
    default: (afxV4d){(afxReal)v}) 

////////////////////////////////////////////////////////////////////////////////
// Initialization                                                             //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxV2dZero(afxV2d v);
AFXINL void     AfxV3dZero(afxV3d v);
AFXINL void     AfxV4dZero(afxV4d v);
AFXINL void     AfxV4dReset(afxV4d v);

AFXINL void     AfxV2dFill(afxV2d v, afxReal value);
AFXINL void     AfxV3dFill(afxV3d v, afxReal value);
AFXINL void     AfxV4dFill(afxV4d v, afxReal value);

AFXINL void     AfxV2dSet(afxV2d v, afxReal x, afxReal y);
AFXINL void     AfxV3dSet(afxV3d v, afxReal x, afxReal y, afxReal z);
AFXINL void     AfxV4dSet(afxV4d v, afxReal x, afxReal y, afxReal z, afxReal w);

////////////////////////////////////////////////////////////////////////////////
// Logic                                                                      //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxBool  AfxV4dIsIdentity(afxV4d const v);

AFXINL afxBool  AfxV2dIsEqual(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsEqual(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsEqual(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsNearEqual(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsNearEqual(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsNearEqual(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsDiff(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsDiff(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsDiff(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsInBounds(afxV2d const v, afxV2d const bounds);
AFXINL afxBool  AfxV3dIsInBounds(afxV3d const v, afxV3d const bounds);
AFXINL afxBool  AfxV4dIsInBounds(afxV4d const v, afxV4d const bounds);

AFXINL afxBool  AfxV2dIsLessOrEqual(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsLessOrEqual(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsLessOrEqual(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsLessThan(afxV2d const v, afxV2d const other);
AFXINL afxBool  AfxV3dIsLessThan(afxV3d const v, afxV3d const other);
AFXINL afxBool  AfxV4dIsLessThan(afxV4d const v, afxV4d const other);

AFXINL afxBool  AfxV2dIsZero(afxV2d const v);
AFXINL afxBool  AfxV3dIsZero(afxV3d const v);
AFXINL afxBool  AfxV4dIsZero(afxV4d const v);

AFXINL afxBool  AfxV2dIsNan(afxV2d const v);
AFXINL afxBool  AfxV3dIsNan(afxV3d const v);
AFXINL afxBool  AfxV4dIsNan(afxV4d const v);

AFXINL afxBool  AfxV2dIsInfinite(afxV2d const v);
AFXINL afxBool  AfxV3dIsInfinite(afxV3d const v);
AFXINL afxBool  AfxV4dIsInfinite(afxV4d const v);

AFXINL afxBool AfxV3dIsNormalized(afxV3d const v);

////////////////////////////////////////////////////////////////////////////////
// Transferance                                                               //
////////////////////////////////////////////////////////////////////////////////

AFXINL void     AfxV2dCopy(afxV2d v, afxV2d const in);
AFXINL void     AfxV3dCopy(afxV3d v, afxV3d const in);
AFXINL void     AfxV4dCopy(afxV4d v, afxV4d const in);

AFXINL void     AfxV3dCopyV2d(afxV3d v, afxV2d const in); // 2D linear transformation vector. Z is 0.
AFXINL void     AfxV4dCopyV2d(afxV4d v, afxV2d const in);
AFXINL void     AfxV4dCopyV3d(afxV4d v, afxV3d const in);
AFXINL void     AfxV3dCopyAtv2d(afxV3d v, afxV2d const in); // 2D affine transfomartion vector. Z is 1.
AFXINL void     AfxV4dCopyAtv3d(afxV4d v, afxV3d const in); // 3D affine transformation vector. W is 1.

AFXINL void     AfxV2dSwap(afxV2d v, afxV2d other);
AFXINL void     AfxV3dSwap(afxV3d v, afxV3d other);
AFXINL void     AfxV4dSwap(afxV4d v, afxV4d other);

// Normalizing a vector scales it so that its length becomes 1 (unit vector), preserving its direction.
// Ensuring a vector has a unit length for operations like lighting calculations, physics simulations, etc.

AFXINL afxReal  AfxV2dNormalize(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalize(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalize(afxV4d v, afxV4d const in);
AFXINL afxReal  AfxV4dNormalizeV3d(afxV4d v, afxV3d const in);
AFXINL afxReal  AfxV3dNormalizeV4d(afxV3d v, afxV4d const in);

AFXINL afxReal  AfxV2dNormalizeEstimated(afxV2d v, afxV2d const in);
AFXINL afxReal  AfxV3dNormalizeEstimated(afxV3d v, afxV3d const in);
AFXINL afxReal  AfxV4dNormalizeEstimated(afxV4d v, afxV4d const in);

////////////////////////////////////////////////////////////////////////////////
// Algebra                                                                    //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxReal  AfxV2dSum(afxV2d const v);
AFXINL afxReal  AfxV3dSum(afxV3d const v);
AFXINL afxReal  AfxV4dSum(afxV4d const v);

// Computes the dot product (a dot b)
// The dot product only defines the angle between both vectors.
// You may remember that the cosine or cos function becomes 0 when the angle is 90 degrees or 1 when the angle is 0.
// This allows us dst easily test if the two vectors are orthogonal or parallel dst each other using the dot product (orthogonal means the vectors are at a right-angle dst each other).

AFXINL afxReal  AfxV2dDot(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDot(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDot(afxV4d const v, afxV4d const other);

// The AfxV*dDist function calculates the distance between two vectors.
// Computes the Euclidean distance between two points or vectors in space. It is used to measure the straight-line distance between two points.
// Measuring distances between two points (e.g., in collision detection, pathfinding).

AFXINL afxReal  AfxV2dDist(afxV2d const v, afxV2d const other);
AFXINL afxReal  AfxV3dDist(afxV3d const v, afxV3d const other);
AFXINL afxReal  AfxV4dDist(afxV4d const v, afxV4d const other);

// Computes the squared magnitude
// This is a simple operation that squares a scalar value (multiplies it by itself). It's used to compute the square of a number or a component.
// Used in distance calculations, computing the squared length of a vector, etc.

AFXINL afxReal  AfxV2dSq(afxV2d const v);
AFXINL afxReal  AfxV3dSq(afxV3d const v);
AFXINL afxReal  AfxV4dSq(afxV4d const v);

// The magnitude/length of a vector in Cartesian coordinates is the square root of the sum of the squares of its coordinates.

AFXINL afxReal  AfxV2dMag(afxV2d const v);
AFXINL afxReal  AfxV3dMag(afxV3d const v);
AFXINL afxReal  AfxV4dMag(afxV4d const v);

AFXINL afxReal  AfxV2dMagRecip(afxV2d const v);
AFXINL afxReal  AfxV3dMagRecip(afxV3d const v);
AFXINL afxReal  AfxV4dMagRecip(afxV4d const v);

// Computes the cross product (a cross b) and stores the result in v
// The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal dst both the input vectors.
// If both the input vectors are orthogonal dst each other as well, a cross product would result in 3 orthogonal vectors; this will prove useful in the upcoming chapters.
// The following image shows what this looks like in 3D space:

AFXINL void     AfxV2dCross(afxV2d v, afxV2d const a, afxV2d const b);
AFXINL void     AfxV3dCross(afxV3d v, afxV3d const a, afxV3d const b);
AFXINL void     AfxV4dCross(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c);

AFXINL void     AfxCopyArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxCopyArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxCopyArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

AFXINL void     AfxZeroOrNormalizeArrayedV2d(afxUnit cnt, afxV2d const in[], afxV2d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV3d(afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxZeroOrNormalizeArrayedV4d(afxUnit cnt, afxV4d const in[], afxV4d out[]);

#endif//AFX_VECTOR_H
