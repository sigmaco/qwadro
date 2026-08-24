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

#include "qwadro/math/afxVertex.h"
#include "qwadro/math/afxTransformation.h"
#include "qwadro/math/afxInterpolation.h"
#include "qwadro/math/afxMultiplication.h"
#include "qwadro/math/afxTrigonometry.h"


_AFXINL afxReal AfxGetDistanceBetweenV3d(afxV3d const v, afxV3d const other)
{
    // Contribution to us from Crytek

    afxError err = { 0 };
    afxV3d t;
    t = AfxV3dSub(v, other);
    t = AfxV3dSqrt(t);
    return AfxSqrtf(AfxV3dSum(t));
}

_AFXINL afxReal AfxGetAngleBetweenV3d(afxV3d const v, afxV3d const other)
{
    // Contribution to us from Crytek

    afxError err = { 0 };
    afxReal cosAngle = AfxV3dDot(v, other) * (AfxV3dMagRecip(v) * AfxV3dMagRecip(other));
    cosAngle = AfxClampd(cosAngle, -1.0, 1.0);
    return AfxAcosf(cosAngle);
}

// ExtractNormalComponent

_AFXINL void AfxExtractNormalV3dComponents(afxV3d const v, afxV3d const normal, afxV3d parallel, afxV3d perpendicular)
{
    // Should be compatible with void XMVector3ComponentsFromNormal(XMVECTOR* pParallel, XMVECTOR* pPerpendicular, FXMVECTOR  V, FXMVECTOR  Normal)

    afxError err = { 0 };
    afxReal scale = AfxV3dDot(v, normal);
    parallel = AfxV3dScale(normal, scale);
    perpendicular = AfxV3dSub(v, parallel);
}

// Reflect

_AFXINL afxV2d AfxV2dReflect(afxV2d const incident, afxV2d const normal)
{
    afxError err = { 0 };

    // v = incident - (2 * dot(incident, normal)) * normal

    afxV2d v = AfxV2dFill(AfxV2dDot(incident, normal));
    v = AfxV2dAdd(v, v);
    return AfxV2dResub(v, normal, incident);
}

_AFXINL afxV3d AfxV3dReflect(afxV3d const incident, afxV3d const normal)
{
    afxError err = { 0 };

    // v = incident - (2 * dot(incident, normal)) * normal

    afxV3d v = AfxV3dFill(AfxV3dDot(incident, normal));
    v = AfxV3dAdd(v, v);
    return AfxV3dResub(v, normal, incident);
}

_AFXINL afxV4d AfxV4dReflect(afxV3d const incident, afxV3d const normal)
{
    afxError err = { 0 };

    // v = incident - (2 * dot(incident, normal)) * normal

    afxV4d v = AfxV4dFill(AfxV3dDot(incident, normal));
    v = AfxV4dAdd(v, v);
    return AfxV4dResub(v, AfxV4dFromV3d(normal), AfxV4dFromV3d(incident));
}

// Refract

_AFXINL afxV2d AfxV2dRefract(afxV2d const incident, afxV2d const normal, afxReal refracIdx)
{
    afxError err = { 0 };

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV2d dot = AfxV2dFill(AfxV2dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV2d R = AfxV2dResub(dot, dot, AFX_V2D_ONE);
    afxV2d const riv = { refracIdx, refracIdx };
    R = AfxV2dMultiply(R, riv);
    R = AfxV2dResub(R, riv, AFX_V2D_ONE);

    if (AfxV2dIsLessOrEqual(R, AFX_V2D_ZERO))
    {
        // Total internal reflection
        return AFX_V2D_ZERO;
    }

    // R = refracIdx * dot + sqrt(R)

    R = AfxV2dSqrt(R);
    R = AfxV2dMad(R, riv, dot);

    // v = refracIdx * incident - normal * R
    afxV2d v = AfxV2dMultiply(riv, incident);
    return AfxV2dResub(normal, R, v);
}

_AFXINL afxV3d AfxV3dRefract(afxV3d const incident, afxV3d const normal, afxReal refracIdx)
{
    afxError err = { 0 };
    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV3d dot = AfxV3dFill(AfxV3dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV3d R = AfxV3dResub(dot, dot, AFX_V3D_ONE);
    afxV3d const riv = { refracIdx, refracIdx, refracIdx };
    R = AfxV3dMultiply(R, riv);
    R = AfxV3dResub(R, riv, AFX_V3D_ONE);

    if (AfxV3dIsLessOrEqual(R, AFX_V3D_ZERO))
    {
        // Total internal reflection
        return AFX_V3D_ZERO;
    }

    // R = refracIdx * dot + sqrt(R)

    R = AfxV3dSqrt(R);
    R = AfxV3dMad(R, riv, dot);

    // v = refracIdx * incident - normal * R
    afxV3d v = AfxV3dMultiply(riv, incident);
    return AfxV3dResub(normal, R, v);
}

_AFXINL afxV4d AfxV4dRefract(afxV3d const incident, afxV3d const normal, afxReal refracIdx)
{
    afxError err = { 0 };

    // v = refracIdx * incident - normal * (refracIdx * dot(incident, normal) + sqrt(1 - refracIdx * refracIdx * (1 - dot(incident, normal) * dot(incident, normal))))

    afxV4d dot = AfxV4dFill(AfxV3dDot(incident, normal));

    // R = 1.0f - refracIdx * refracIdx * (1.0f - IDotN * IDotN)

    afxV4d R = AfxV4dResub(dot, dot, AFX_V4D_ONE);
    afxV4d const riv = { refracIdx, refracIdx, refracIdx, refracIdx };
    R = AfxV4dMultiply(R, riv);
    R = AfxV4dResub(R, riv, AFX_V4D_ONE);

    if (AfxV4dIsLessOrEqual(R, AFX_V4D_ZERO))
    {
        // Total internal reflection
        return AFX_V4D_ZERO;
    }

    // R = refracIdx * dot + sqrt(R)

    R = AfxV4dSqrt(R);
    R = AfxV4dMad(R, riv, dot);

    // v = refracIdx * incident - normal * R
    afxV4d v = AfxV4dMultiply(riv, AfxV4dFromV3d(incident));
    return AfxV4dResub(AfxV4dFromV3d(normal), R, v);
}
