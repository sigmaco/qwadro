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

#include "qwadro/math/afxVertex.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"


_AFXINL afxReal AfxGetDistanceBetweenV3d(afxV3d const v, afxV3d const other)
{
    // Contribution to us from Crytek

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);
    afxV3d t;
    AfxV3dSub(t, v, other);
    AfxV3dSqrt(t, t);
    return AfxSqrtf(AfxV3dSum(t));
}

_AFXINL afxReal AfxGetAngleBetweenV3d(afxV3d const v, afxV3d const other)
{
    // Contribution to us from Crytek

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(other);

    afxReal cosAngle = AfxV3dDot(v, other) * (AfxV3dMagRecip(v) * AfxV3dMagRecip(other));
    AfxClampd(cosAngle, -1.0, 1.0);
    return AfxAcosf(cosAngle);
}

// ExtractNormalComponent

_AFXINL void AfxExtractNormalV3dComponents(afxV3d const v, afxV3d const normal, afxV3d parallel, afxV3d perpendicular)
{
    // Should be compatible with void XMVector3ComponentsFromNormal(XMVECTOR* pParallel, XMVECTOR* pPerpendicular, FXMVECTOR  V, FXMVECTOR  Normal)

    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(normal);
    AFX_ASSERT(parallel);
    AFX_ASSERT(perpendicular);

    afxReal scale = AfxV3dDot(v, normal);
    AfxV3dScale(parallel, normal, scale);
    AfxV3dSub(perpendicular, v, parallel);
}

// Reflect

_AFXINL void AfxReflectV2d(afxV2d v, afxV2d const incident, afxV2d const normal)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxV2dFill(v, AfxV2dDot(incident, normal));
    AfxV2dAdd(v, v, v);
    AfxV2dResub(v, v, normal, incident);
}

_AFXINL void AfxReflectV3d(afxV3d v, afxV3d const incident, afxV3d const normal)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxV3dFill(v, AfxV3dDot(incident, normal));
    AfxV3dAdd(v, v, v);
    AfxV3dResub(v, v, normal, incident);
}

_AFXINL void AfxReflectV4d(afxV4d v, afxV3d const incident, afxV3d const normal)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = incident - (2 * dot(incident, normal)) * normal

    AfxV4dFill(v, AfxV3dDot(incident, normal));
    AfxV4dAdd(v, v, v);
    AfxV4dResub(v, v, normal, incident);
}

// Refract

_AFXINL void AfxRefractV2d(afxV2d v, afxV2d const incident, afxV2d const normal, afxReal refracIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV2d dot;
    AfxV2dFill(dot, AfxV2dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV2d R;
    AfxV2dResub(R, dot, dot, AFX_V2D_ONE);
    afxV2d const riv = { refracIdx, refracIdx };
    AfxV2dMultiply(R, R, riv);
    AfxV2dResub(R, R, riv, AFX_V2D_ONE);

    if (AfxV2dIsLessOrEqual(R, AFX_V2D_ZERO))
    {
        // Total internal reflection
        AfxV2dZero(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxV2dSqrt(R, R);
        AfxV2dMad(R, R, riv, dot);

        // v = refracIdx * incident - normal * R
        AfxV2dMultiply(v, riv, incident);
        AfxV2dResub(v, normal, R, v);
    }
}

_AFXINL void AfxRefractV3d(afxV3d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV3d dot;
    AfxV3dFill(dot, AfxV3dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV3d R;
    AfxV3dResub(R, dot, dot, AFX_V3D_ONE);
    afxV3d const riv = { refracIdx, refracIdx, refracIdx };
    AfxV3dMultiply(R, R, riv);
    AfxV3dResub(R, R, riv, AFX_V3D_ONE);

    if (AfxV3dIsLessOrEqual(R, AFX_V3D_ZERO))
    {
        // Total internal reflection
        AfxV3dZero(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxV3dSqrt(R, R);
        AfxV3dMad(R, R, riv, dot);

        // v = refracIdx * incident - normal * R
        AfxV3dMultiply(v, riv, incident);
        AfxV3dResub(v, normal, R, v);
    }
}

_AFXINL void AfxRefractV4d(afxV4d v, afxV3d const incident, afxV3d const normal, afxReal refracIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(incident);
    AFX_ASSERT(normal);

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV4d dot;
    AfxV4dFill(dot, AfxV3dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV4d R;
    AfxV4dResub(R, dot, dot, AFX_V4D_ONE);
    afxV4d const riv = { refracIdx, refracIdx, refracIdx, refracIdx };
    AfxV4dMultiply(R, R, riv);
    AfxV4dResub(R, R, riv, AFX_V4D_ONE);

    if (AfxV4dIsLessOrEqual(R, AFX_V4D_ZERO))
    {
        // Total internal reflection
        AfxV4dZero(v);
    }
    else
    {
        // R = refracIdx * dot + sqrt(R)

        AfxV4dSqrt(R, R);
        AfxV4dMad(R, R, riv, dot);

        // v = refracIdx * incident - normal * R
        AfxV4dMultiply(v, riv, incident);
        AfxV4dResub(v, normal, R, v);
    }
}
