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

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#include "../icd/avxIcd.h"

_AVX afxReal AfxToSrgba(afxReal linear)
{
    return linear <= 0.0031308f ? linear * 12.92f : AfxPowf(linear, 1.0f / 2.4f) * 1.055f - 0.055f;
}

_AVX afxReal AfxFromSrgba(afxReal sRgb)
{
    return sRgb <= 0.04045f ? sRgb / 12.92f : AfxPowf((sRgb + 0.055f) / 1.055f, 2.4f);
}

_AVXINL avxColor AvxMakeColor(afxReal r, afxReal g, afxReal b, afxReal a)
{
    afxError err = { 0 };
    return AfxV4dMake(r, g, b, a);
}

_AVXINL avxColor AvxMakeColor8(afxByte r, afxByte g, afxByte b, afxByte a)
{
    afxError err = { 0 };
    return AVX_COLOR(   r / 255.f,
                        g / 255.f,
                        b / 255.f,
                        a / 255.f);
}

_AVXINL avxColor AvxMakeColorA(avxColor const c, afxReal a)
{
    afxError err = { 0 };
    return AvxMakeColor(c.v[0], c.v[1], c.v[2], a);
}

_AVXINL avxColor AvxMakeColorA8(avxColor const c, afxByte a)
{
    afxError err = { 0 };
    return AvxMakeColor(c.v[0], c.v[1], c.v[2], a / 255.f);
}

_AVXINL avxColor AvxMakeColorRgba8(afxUnit rgba)
{
    afxError err = { 0 };
    return AVX_COLOR(   (afxReal)((rgba >> 24) & 0xFF) / 255.0f,
                        (afxReal)((rgba >> 16) & 0xFF) / 255.0f,
                        (afxReal)((rgba >> 8) & 0xFF) / 255.0f,
                        (afxReal)((rgba & 0xFF) / 255.0f));
}

_AVXINL avxColor AvxMakeColorArgb8(afxUnit argb)
{
    afxError err = { 0 };
    return AVX_COLOR(   (afxReal)((argb >> 16) & 0xFF) / 255.0f,
                        (afxReal)((argb >> 8) & 0xFF) / 255.0f,
                        (afxReal)((argb & 0xFF) / 255.0f),
                        (afxReal)((argb >> 24) & 0xFF) / 255.0f);
}

_AVXINL avxColor AvxAddColor(avxColor const a, avxColor const b)
{
    afxError err = { 0 };
    return AfxV4dAdd(a, b);
}

_AVXINL avxColor AvxSubColor(avxColor const a, avxColor const b)
{
    afxError err = { 0 };
    return AfxV4dSub(a, b);
}

_AVXINL avxColor AvxScaleColor(avxColor const from, afxReal lambda)
{
    afxError err = { 0 };
    return AfxV4dScale(from, lambda);
}

_AVXINL afxUnit32 AvxGetColorRgba8(avxColor const c)
{
    afxError err = { 0 };
    return ((((afxUnit32)(c.v[0] * 255.0f)) << 24) | 
            (((afxUnit32)(c.v[1] * 255.0f)) << 16) | 
            (((afxUnit32)(c.v[2] * 255.0f)) << 8) | 
             ((afxUnit32)(c.v[3] * 255.0f)));
}

_AVXINL afxUnit32 AvxGetColorArgb8(avxColor const c)
{
    afxError err = { 0 };
    return ((((afxUnit32)(c.v[3] * 255.0f)) << 24) | 
            (((afxUnit32)(c.v[0] * 255.0f)) << 16) | 
            (((afxUnit32)(c.v[1] * 255.0f)) << 8) | 
             ((afxUnit32)(c.v[2] * 255.0f)));
}

_AVXINL avxColor AvxPremulColorAlpha(avxColor const in)
{
    afxError err = { 0 };
    return AVX_COLOR(   in.v[0] * in.v[3],
                        in.v[1] * in.v[3],
                        in.v[2] * in.v[3],
                        in.v[3]);
}

_AVXINL avxColor AvxMixColor(avxColor const c0, avxColor const c1, afxReal u)
{
    afxError err = { 0 };
    u = AfxClampf(u, 0.f, 1.f);
    return AfxV4dMix(c0, c1, u);
}

_AVXINL afxReal AvxSetHue(afxReal h, afxReal m1, afxReal m2)
{
    if (h < 0.f)
        h += 1.f;

    if (h > 1.f)
        h -= 1.f;

    if (h < 1.f / 6.f)
        return m1 + (m2 - m1) * h * 6.f;
    else if (h < 3.f / 6.f)
        return m2;
    else if (h < 4.f / 6.f)
        return m1 + (m2 - m1) * (2.f / 3.f - h) * 6.f;

    return m1;
}

_AVXINL avxColor AvxMakeColorHsla(afxReal h, afxReal s, afxReal l, afxByte a)
{
    afxError err = { 0 };
    h = AfxModf(h, 1.f);

    if (h < 0.0f)
        h += 1.f;

    s = AfxClampf(s, 0.f, 1.f);
    l = AfxClampf(l, 0.f, 1.f);
    afxReal m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
    afxReal m1 = 2.f * l - m2;
    return AVX_COLOR(   AfxClampf(AvxSetHue(h + 1.f / 3.f, m1, m2), 0.f, 1.f),
                        AfxClampf(AvxSetHue(h, m1, m2), 0.f, 1.f),
                        AfxClampf(AvxSetHue(h - 1.f / 3.f, m1, m2), 0.f, 1.f),
                        a / 255.f);
}

_AVXINL avxColor AvxMakeColorHsl(afxReal h, afxReal s, afxReal l)
{
    afxError err = { 0 };
    return AvxMakeColorHsla(h, s, l, 0xFF);
}
