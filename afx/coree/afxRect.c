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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "qwadro/afxRect.h"

_AFXINL afxUnit AfxGetRectSize(afxRect const* rc)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    return (rc->w * rc->h);
}

_AFXINL afxBool AfxIsRectVoid(afxRect const* rc)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    //return (0 == AfxGetRectSize(rc));
    return (0 == (rc->w * rc->h));
}

_AFXINL afxRect AfxGetUnitedRect(afxRect const* rc, afxRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);

    // Essentially the inverse of AfxIntersectRects().
    afxInt32 x = AFX_MIN(rc->x, other->x);
    afxInt32 y = AFX_MIN(rc->y, other->y);
    afxUnit32 w, h;
    return AFX_RECT( x, y,
                    (w = (AFX_MAX(rc->x + rc->w, other->x + other->w) - x)),
                    (h = (AFX_MAX(rc->y + rc->h, other->y + other->h) - y)));
}

_AFXINL afxLayeredRect AfxGetUnitedRectLayered(afxLayeredRect const* rc, afxLayeredRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);

    // Essentially the inverse of AfxIntersectRects().
    afxInt32 x = AFX_MIN(rc->area.x, other->area.x);
    afxInt32 y = AFX_MIN(rc->area.y, other->area.y);
    afxInt32 z = AFX_MIN(rc->baseLayer, other->baseLayer);
    afxUnit32 w, h, d;
    return AFX_LAYERED_RECT( x, y, z,
                            (w = (AFX_MAX(rc->area.x + rc->area.w, other->area.x + other->area.w)           - x)),
                            (h = (AFX_MAX(rc->area.y + rc->area.h, other->area.y + other->area.h)           - y)),
                            (d = (AFX_MAX(rc->baseLayer + rc->layerCnt, other->baseLayer + other->layerCnt) - z)));
}

_AFXINL afxRect AfxGetIntersectedRect(afxRect const* rc, afxRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);

    // Essentially the inverse of AfxMergeRects().
    afxInt32 x = AFX_MAX(rc->x, other->x);
    afxInt32 y = AFX_MAX(rc->y, other->y);
    afxUnit32 w, h;
    return AFX_RECT( x, y,
                    (w = (AFX_MIN(rc->x + rc->w, other->x + other->w) - x)),
                    (h = (AFX_MIN(rc->y + rc->h, other->y + other->h) - y)));
}

_AFXINL afxLayeredRect AfxGetIntersectedRectLayered(afxLayeredRect const* rc, afxLayeredRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);

    // Essentially the inverse of AfxMergeRects().
    afxInt32 x = AFX_MAX(rc->area.x, other->area.x);
    afxInt32 y = AFX_MAX(rc->area.y, other->area.y);
    afxInt32 z = AFX_MAX(rc->baseLayer, other->baseLayer);
    afxUnit32 w, h, d;
    return AFX_LAYERED_RECT( x, y, z,
                            (w = (AFX_MIN(rc->area.x + rc->area.w, other->area.x + other->area.w)           - x)),
                            (h = (AFX_MIN(rc->area.y + rc->area.h, other->area.y + other->area.h)           - y)),
                            (d = (AFX_MIN(rc->baseLayer + rc->layerCnt, other->baseLayer + other->layerCnt) - z)));
}

_AFXINL afxBool AfxIsRectEqual(afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return ((a->x == b->x) && (a->y == b->y) && (a->w == b->w) && (a->h == b->h));
}

_AFXINL afxBool AfxIsRectOverlapping(afxRect const* a, afxRect const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    // is B completely right of A?
    if (AFX_MAX(a->x, (afxInt)a->w) < AFX_MIN(b->x, (afxInt)b->w))
        return FALSE;

    // is B completely left of A?
    if (AFX_MAX(b->x, (afxInt)b->w) < AFX_MIN(a->x, (afxInt)a->w))
        return FALSE;

    // is B completely above A?
    if (AFX_MAX(a->y, (afxInt)a->h) < AFX_MIN(b->y, (afxInt)b->h))
        return FALSE;

    // is B completely below A?
    if (AFX_MAX(b->y, (afxInt)b->h) < AFX_MIN(a->y, (afxInt)a->h))
        return FALSE;

    return TRUE; // overlapping
}

_AFXINL afxBool AfxRectContainsBiased(afxRect const* a, afxRect const* b, afxInt tolX, afxInt tolY)
// Compare if rectangle \a a contains rectangle \a b in coordinate, with specified tolerance allowed.
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    return  (b->x >= a->x - tolX) &&
            (b->y >= a->y - tolY) &&
            ((b->x + b->w) <= (a->x + a->w + tolX)) &&
            ((b->y + b->h) <= (a->y + a->h + tolY));
}

_AFXINL afxBool AfxRectContains(afxRect const* a, afxRect const* b)
// This checks that all corners of b are inside a, i.e., b is entirely within a.
{
    afxError err = { 0 };
    AFX_ASSERT(a);
    AFX_ASSERT(b);    
    return  (b->x >= a->x) &&
            (b->y >= a->y) &&
            ((b->x + b->w) <= (a->x + a->w)) &&
            ((b->y + b->h) <= (a->y + a->h));
}

_AFXINL afxBool AfxIsRectOutside(afxRect const* a, afxRect const* b)
// This test whether @a and @b don't intersect at all.
{
    return ((b->x + (afxInt)b->w) <= a->x ||
        b->x >= (a->x + (afxInt)a->w) ||
        (b->y + (afxInt)b->h) <= a->y ||
        b->y >= (a->y + (afxInt)a->h));
}

_AFXINL afxBool AfxRectIntersects(afxRect const* a, afxRect const* b)
// This test whether @a and @b overlap at all (fully or partially).
{
    return !(   (b->x + (afxInt)b->w) <= a->x ||
                 b->x >= (a->x + (afxInt)a->w) ||
                (b->y + (afxInt)b->h) <= a->y ||
                 b->y >= (a->y + (afxInt)a->h));
}

_AFXINL void AfxExtractRectMargins(afxRect const* outer, afxRect const* inner, afxInt* left, afxInt* top, afxInt* right, afxInt* bottom)
{
    afxError err = { 0 };
    AFX_ASSERT(outer);
    AFX_ASSERT(inner);
    AFX_ASSERT(left);
    AFX_ASSERT(top);
    AFX_ASSERT(right);
    AFX_ASSERT(bottom);
    // calculate the margins between two such rectangles, for example, the difference between an outer rectangle and an inner rectangle;
    // where the inner rectangle is fully contained inside the outer.

    // Assumes inner is inside outer.
    *left = inner->x - outer->x;
    *top = inner->y - outer->y;
    *right = (outer->x + (afxInt)outer->w) - (inner->x + (afxInt)inner->w);
    *bottom = (outer->y + (afxInt)outer->h) - (inner->y + (afxInt)inner->h);
}

_AFXINL afxRect AfxGetFlippedRect(afxRect const* rc, afxUnit height)
{
    afxError err = { 0 };
    AFX_ASSERT(height);
    AFX_ASSERT(rc);
    return AFX_RECT(rc->x, (height - (rc->y + rc->h)), rc->w, rc->h);
}

_AFXINL afxUnit AfxScissorRect(afxRect const* rc, afxUnit maxW, afxUnit maxH, afxRect outsides[4])
{
    afxError err = { 0 };
    AFX_ASSERT(outsides);
    AFX_ASSERT(rc);
    afxUnit invRcCnt = 0;

    // If the given rectangle is completely inside the screen, we calculate the outside areas.

    // These rects may overlap, especially at corners - e.g. if a rect spills out top-left, both the left and top exclusions will cover that corner area.
    // This is different from a region subtraction, where we would remove the intersection and return only non-overlapping leftovers.
    // Useful in GUI layout engines, clipping rejection, or debugging overlays.

    // Left exclusion
    if (rc->x < 0)
        outsides[invRcCnt++] = AFX_RECT( rc->x, rc->y, -rc->x, rc->h );

    // Right exclusion
    if (rc->x + rc->w > maxW)
        outsides[invRcCnt++] = AFX_RECT(maxW, rc->y, (rc->x + rc->w) - maxW, rc->h );

    // Top exclusion
    if (rc->y < 0)
        outsides[invRcCnt++] = AFX_RECT( rc->x, rc->y, rc->w, -rc->y );

    // Bottom exclusion
    if (rc->y + rc->h > maxH)
        outsides[invRcCnt++] = AFX_RECT(rc->x, maxH, rc->w, (rc->y + rc->h) - maxH );

    return invRcCnt;
}

_AFXINL afxRect AfxGetMiniRect(afxRect const* rc, afxRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);
    return AFX_RECT(AFX_MIN(rc->x, other->x),
                    AFX_MIN(rc->y, other->y),
                    AFX_MIN(rc->w, other->w),
                    AFX_MIN(rc->h, other->h));
}

_AFXINL afxLayeredRect AfxGetMiniRectLayered(afxLayeredRect const* rc, afxLayeredRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);
    return AFX_LAYERED_RECT(AFX_MIN(rc->area.x,     other->area.x),
                            AFX_MIN(rc->area.y,     other->area.y),
                            AFX_MIN(rc->baseLayer,  other->baseLayer),
                            AFX_MIN(rc->area.w,     other->area.w),
                            AFX_MIN(rc->area.h,     other->area.h),
                            AFX_MIN(rc->layerCnt,   other->layerCnt));
}

_AFXINL afxRect AfxGetMaxiRect(afxRect const* rc, afxRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);
    return AFX_RECT(AFX_MAX(rc->x, other->x),
                    AFX_MAX(rc->y, other->y),
                    AFX_MAX(rc->w, other->w),
                    AFX_MAX(rc->h, other->h));
}

_AFXINL afxLayeredRect AfxGetMaxiRectLayered(afxLayeredRect const* rc, afxLayeredRect const* other)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(other);
    return AFX_LAYERED_RECT(AFX_MAX(rc->area.x,     other->area.x),
                            AFX_MAX(rc->area.y,     other->area.y),
                            AFX_MAX(rc->baseLayer,  other->baseLayer),
                            AFX_MAX(rc->area.w,     other->area.w),
                            AFX_MAX(rc->area.h,     other->area.h),
                            AFX_MAX(rc->layerCnt,   other->layerCnt));
}

_AFXINL afxRect AfxGetClampedRect(afxRect const* rc, afxRect const* min, afxRect const* max)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    return AFX_RECT(    AFX_CLAMP(rc->x, min->x, max->x),
                        AFX_CLAMP(rc->y, min->y, max->y),
                        AFX_CLAMP(rc->w, min->w, max->w),
                        AFX_CLAMP(rc->h, min->h, max->h));
}

_AFXINL afxLayeredRect AfxGetClampedRectLayered(afxLayeredRect const* rc, afxLayeredRect const* min, afxLayeredRect const* max)
{
    afxError err = { 0 };
    AFX_ASSERT(rc);
    AFX_ASSERT(min);
    AFX_ASSERT(max);
    return AFX_LAYERED_RECT(    AFX_CLAMP(rc->area.x,   min->area.x,    max->area.x),
                                AFX_CLAMP(rc->area.y,   min->area.y,    max->area.y),
                                AFX_CLAMP(rc->baseLayer,min->baseLayer, max->baseLayer),
                                AFX_CLAMP(rc->area.w,   min->area.w,    max->area.w),
                                AFX_CLAMP(rc->area.h,   min->area.h,    max->area.h),
                                AFX_CLAMP(rc->layerCnt, min->layerCnt,  max->layerCnt));
}

_AFXINL afxRect AfxGetAccumulatedRect(afxUnit cnt, afxRect const rects[])
{
    afxError err = { 0 };
    AFX_ASSERT(rects);

    // Initialize to extreme values
    afxInt32 minX = AFX_I32_MAX;
    afxInt32 minY = AFX_I32_MAX;
    afxUnit32 maxX = AFX_I32_MIN;
    afxUnit32 maxY = AFX_I32_MIN;

    // Iterate over the list of rectangles
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRect r = rects[i];

        // Update min and max values for bounding rectangle
        if (r.x < minX) minX = r.x;
        if (r.y < minY) minY = r.y;
        if (r.x + r.w > maxX) maxX = r.x + r.w;
        if (r.y + r.h > maxY) maxY = r.y + r.h;
    }

    return AFX_RECT(minX, minY, maxX - minX, maxY - minY);
}

afxRect ResolveSurfaceRect(
    afxRect const* screen,
    afxRect const* desired,
    afxAnchor anchor,
    afxRect const* current)
{
    afxRect out = *desired;

    // If no anchor is specified, just preserve position
    if (anchor == 0)
    {
        out.x = current->x;
        out.y = current->y;
        return out;
    }

    // Compute the anchor points in both rects
    afxInt screenAx = screen->x;
    afxInt screenAy = screen->y;
    afxInt winAx = 0;
    afxInt winAy = 0;

    // Screen anchor point
    if (anchor & afxAnchor_LEFT)
        screenAx = screen->x;
    else if (anchor & afxAnchor_CENTER)
        screenAx = screen->x + screen->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        screenAx = screen->x + screen->w;

    if (anchor & afxAnchor_TOP)
        screenAy = screen->y;
    else if (anchor & afxAnchor_MIDDLE)
        screenAy = screen->y + screen->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        screenAy = screen->y + screen->h;

    // Window anchor point (relative to window's own rect)
    if (anchor & afxAnchor_LEFT)
        winAx = 0;
    else if (anchor & afxAnchor_CENTER)
        winAx = desired->w / 2;
    else if (anchor & afxAnchor_RIGHT)
        winAx = desired->w;

    if (anchor & afxAnchor_TOP)
        winAy = 0;
    else if (anchor & afxAnchor_MIDDLE)
        winAy = desired->h / 2;
    else if (anchor & afxAnchor_BOTTOM)
        winAy = desired->h;

    // Position the window so its anchor point coincides with the screen's anchor point
    out.x = screenAx - winAx + desired->x;
    out.y = screenAy - winAy + desired->y;

    return out;
}

