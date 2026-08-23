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

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"

////////////////////////////////////////////////////////////////////////////////

_AFXINL afxV2d AfxV2dNdc(afxV2d const from, afxV2d const total)
{
    afxError err = { 0 };
    return AFX_V2D( AfxNdcf(from.v[0], total.v[0]),
                    AfxNdcf(from.v[1], total.v[1]));
}

_AFXINL afxV2d AfxV2dUnndc(afxV2d const from, afxV2d const total)
{
    afxError err = { 0 };
    return AFX_V2D( AfxUnndcf(from.v[0], total.v[0]),
                    AfxUnndcf(from.v[1], total.v[1]));
}
