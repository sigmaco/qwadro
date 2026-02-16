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

// This file is part of Advanced RenderWare Extensions.

#define _ARX_C
#define _ARX_CURVE_C
#include "../scene/arxIcd.h"

_ARXINL afxUnit ArxCountCurveKnots(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit knotCnt = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        knotCnt = c->ctrlCnt / c->dimens;
        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        knotCnt = c->knotCnt;
        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        knotCnt = 0;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        knotCnt = 1;
        break;
    }
    default: AfxThrowError(); break;
    }
    AFX_ASSERT(knotCnt == c->knotCnt);
#endif
    return c->knotCnt;
}

_ARXINL afxUnit ArxGetCurveDimensionalityUnchecked(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit dims = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaKeyframes32f:
    {
        dims = c->dimens;
        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        dims = c->ctrlCnt / c->knotCnt;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        dims = c->ctrlCnt;
        break;
    }
    case arxCurveFormat_D3Constant32f:
    {
        dims = 3;
        break;
    }
    case arxCurveFormat_D4Constant32f:
    {
        dims = 4;
        break;
    }
    default: AfxThrowError(); break;
    };

    AFX_ASSERT(dims == c->dimens);
#endif
    return c->dimens;
}

_ARXINL afxUnit ArxGetCurveDimensionality(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
#if _AFX_DEBUG
    AFX_ASSERT(ArxCountCurveKnots(c) ? ArxGetCurveDimensionalityUnchecked(c) : 0);
#endif
    return c->dimens;
}

_ARX void ArxDescribeCurve(arxCurve cur, arxCurveInfo2* desc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &cur);
    AFX_ASSERT(desc);

    desc->fmt = cur->fmt;
    desc->degree = cur->degree;
    desc->flags = cur->flags;
    desc->dimens = ArxGetCurveDimensionality(cur);
    desc->knotCnt = ArxCountCurveKnots(cur);
    desc->ctrlCnt = cur->ctrlCnt;

}

_ARXINL afxInt ArxGetCurveDegree(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return c->degree;
}

_ARXINL arxCurveFlags ArxGetCurveFlags(arxCurve c, arxCurveFlags mask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return (!mask) ? c->flags : (c->flags & mask);
}

_ARXINL afxBool ArxTestCurveKeyframed(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return !!ArxGetCurveFlags(c, arxCurveFlag_KEYFRAMED);
}

_ARXINL afxBool ArxTestCurveIdentity(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    return !!ArxGetCurveFlags(c, arxCurveFlag_IDENTITY);
}

_ARXINL afxBool ArxTestCurveConstantOrIdentity(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    return !!ArxGetCurveFlags(c, arxCurveFlag_IDENTITY | arxCurveFlag_CONSTANT);
}

_ARXINL afxBool ArxTestCurveConstantNotIdentity(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    return ArxTestCurveConstantOrIdentity(c) && !ArxTestCurveIdentity(c);
}

_ARXINL afxReal* ArxGetCurveKnots(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
#if _AFX_DEBUG
    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        AFX_ASSERT(!c->knots);
        break;
    }
    default:break;
    }
#endif
    return c->knots;
}

_ARXINL afxReal* ArxGetCurveControls(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    arxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case arxCurveFormat_D3Constant32f: return c->ctrls3;
    case arxCurveFormat_D4Constant32f: return c->ctrls4;
    default: break;
    }
    return c->ctrls;
}

_ARX afxUnit ArxFindCurveKnot(arxCurve c, afxReal t)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;
    
    switch (c->fmt)
    {
    case arxCurveFormat_DaK32fC32f:
    {
        knotIdx = c->ddi->findKnotCb(c, t, 0);
        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_ARX afxUnit AfxFindCurveCloseKnot(arxCurve c, afxReal t, afxUnit baseIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaK32fC32f:
    {
        knotIdx = c->ddi->findKnotCb(c, t, baseIdx);
        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_ARX void ArxCopyCurve(arxCurve c, arxCurve src)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    AFX_ASSERT(src);
    AFX_ASSERT(c->fmt == src->fmt);
    AfxCatchError(err);
    arxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case arxCurveFormat_DaIdentity:
    {
        c->dimens = src->dimens;
        break;
    }
    default:
    {
        c->ddi->copyCb(c, src);
        break;
    }
    }
}

_ARX void ArxUpdateCurveKnots(arxCurve c, afxUnit knotCnt, afxUnit dimens, afxReal const* knotSrc, afxReal const* ctrlSrc)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    AFX_ASSERT_RANGE(c->knotCnt, 0, knotCnt);
    AFX_ASSERT(dimens == c->dimens);
    c->ddi->updateCb(c, knotCnt, dimens, knotSrc, ctrlSrc);
}

_ARX void ArxExtractCurveKnotValues(arxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlRslts, afxReal const* identity)
{
    afxError err = { 0 };
    AFX_ASSERT(c);

    if (ArxTestCurveIdentity(c))
    {
        for (afxUnit i = 0; i < c->dimens; i++)
            ctrlRslts[i] = identity[i];

        // Early exit.
        return;
    }

    arxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case arxCurveFormat_DaIdentity:
    {
        if (knotCnt == 1 && knotResults)
            knotResults[0] = 0.0;

        if (ctrlRslts)
        {
            for (afxUnit i = c->dimens; i-- > 0;)
                ctrlRslts[i] = identity[i];
        }
        break;
    }
    default:
    {
        AFX_ASSERT(c->ddi->extractCb);
        c->ddi->extractCb(c, knotIdx, knotCnt, knotResults, ctrlRslts, identity);
        break;
    }
    }
}

_ARX afxReal ArxExtractCurveKnotValue(arxCurve c, afxUnit knotIdx, afxReal* ctrlRslt, afxReal const* identity)
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    afxReal f = 0.0;
    ArxExtractCurveKnotValues(c, knotIdx, 1, &f, ctrlRslt, identity);
    return f;
}

_ARX void ArxEvaluateCurveAtKnot(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identity)
{
    afxError err;
    afxReal *tiBuffer;
    afxReal *piBuffer;
    afxReal tiBufferSpace[8];
    afxReal piBufferSpace[64];

    if (ArxTestCurveConstantOrIdentity(c))
    {
        ArxExtractCurveKnotValue(c, 0, rslt, identity);
    }
    else
    {
        c->ddi->evalAtKnotCb(c, dimens, normalize, bwdsLoop, fwdsLoop, curveDur, knotIdx, t, rslt, identity);
    }
}

_ARX void ArxEvaluateCurveAt(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identity)
{
    ArxEvaluateCurveAtKnot(c, dimens, normalize, bwdsLoop, fwdsLoop, curveDur, ArxFindCurveKnot(c, t), t, rslt, identity);
}

_ARX void ArxMakeCurveDaKC32f(arxCurve c, afxUnit degree, afxUnit dim, afxUnit knotCnt, afxReal const knots[], afxReal const ctrls[])
{
    afxError err = { 0 };
    AFX_ASSERT(c);
    c->fmt = arxCurveFormat_DaK32fC32f;
    c->degree = degree;
    c->dimens = dim; // unused
    c->knotCnt = knotCnt;
    c->knots = (afxReal*)knots;
    c->ctrlCnt = dim * knotCnt;
    c->ctrls = (afxReal*)ctrls;
}

// BLUEPRINT

_ARXINL void ArxResetCurveInfo(arxCurveInfo* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
{
    //cb->needsFreeing = FALSE;
    cb->fmt = fmt;
    cb->degree = degree;
    cb->dimens = dimens;
    cb->knotCnt = knotCnt;
    cb->knots = NIL;
    cb->ctrls = NIL;
    cb->src = NIL;
    cb->sampleCnt = -1;
    cb->sampleDim = -1;
    cb->xformedSamples = NIL;
    cb->origSamples = NIL;
}

_ARX void ArxBeginCurve(arxCurveBlueprint* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cb);
    cb->curveBuilderNeedsFreeing = FALSE;
    cb->fmt = fmt;
    cb->degree = degree;
    cb->dimens = dimens;
    cb->knotCnt = knotCnt;
    cb->knotArray = 0;
    cb->ctrlArray = 0;
    cb->srcCurve = 0;
    cb->sampleCnt = -1;
    cb->sampleDimension = -1;
    cb->transformedSamples = 0;
    cb->originalSamples = 0;
}

_ARX void ArxBeginCurveCopy(arxCurveBlueprint* cb, arxCurve src)
{
    afxError err = { 0 };
    AFX_ASSERT(cb);
    ArxBeginCurve(cb, src->fmt, ArxGetCurveDegree(src), ArxGetCurveDimensionality(src), ArxCountCurveKnots(src));
    cb->srcCurve = src;
}

_ARX _arxDdiCur const _ARX_CURVE_DDI =
{
    NIL
};

_ARX afxError _ArxCurDtorCb(arxCurve c)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);

    switch (c->fmt)
    {
    case arxCurveFormat_DaK32fC32f:
    {
        if (c->knots)
            AfxDeallocate((void**)&c->knots, AfxHere());

        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    {
        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    default: break;
    }
    return err;
}

_ARX afxError _ArxCurCtorCb(arxCurve c, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxCurveInfo const* info = ((arxCurveInfo const*)args[1]) + invokeNo;

    c->ddi = &_ARX_CURVE_DDI;
    c->idd = NIL;
    c->udd = info->udd;
    c->tag = info->tag;

    arxCurveFormat fmt = info->fmt;
    c->fmt = fmt;
    c->degree = info->degree;
    c->flags = NIL;

    switch (fmt)
    {
    
    case arxCurveFormat_DaK32fC32f:
    {
        c->dimens = info->dimens; // unused        
        c->knotCnt = info->knotCnt;
        c->knots = NIL;
        c->ctrlCnt = info->knotCnt * info->dimens;
        c->ctrls = NIL;

        if (c->knotCnt && AfxAllocate(c->knotCnt * sizeof(c->knots[0]), 0, AfxHere(), (void**)&c->knots))
            AfxThrowError();

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        if (!c->knotCnt)
        {
            if (c->ctrlCnt)
                c->flags |= arxCurveFlag_KEYFRAMED;
            else
                c->flags |= arxCurveFlag_IDENTITY;
        }
        else if (c->knotCnt == 1)
        {
            c->flags |= arxCurveFlag_CONSTANT;
        }
        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        c->dimens = info->dimens;
        c->knotCnt = 0; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = 0; // unused
        c->ctrls = NIL; // unused
        c->flags |= arxCurveFlag_IDENTITY | arxCurveFlag_CONSTANT;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        c->dimens = info->dimens; // unused
        c->knotCnt = info->knotCnt; // unused
        AFX_ASSERT(c->knotCnt == 1);
        c->knots = NIL; // unused
        c->ctrlCnt = info->dimens;
        c->ctrls = NIL;

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        c->flags |= arxCurveFlag_CONSTANT;
        break;
    }
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        c->dimens = info->dimens; // unused
        c->knotCnt = 1; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = 0; // unused
        c->ctrls = NIL; // unused
        c->flags |= arxCurveFlag_CONSTANT;
        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    {
        c->dimens = info->dimens;
        c->flags |= arxCurveFlag_KEYFRAMED;
        c->knotCnt = info->knotCnt; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = info->dimens * info->knotCnt;
        c->ctrls = NIL;

        AFX_ASSERT(c->knotCnt == c->ctrlCnt / c->dimens);

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        break;
    }
    default: AfxThrowError(); break;
    }

    return err;
}

_ARX afxClassConfig const _ARX_CUR_CLASS_CONFIG =
{
    .fcc = afxFcc_CUR,
    .name = "Curve",
    .desc = "Geometric Curve",
    .fixedSiz = sizeof(AFX_OBJECT(arxCurve)),
    .ctor = (void*)_ArxCurCtorCb,
    .dtor = (void*)_ArxCurDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireCurves(arxScenario scio, afxUnit cnt, arxCurveInfo const infos[], arxCurve curves[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetCurvClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_CUR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)curves, (void const*[]) { scio, infos }))
    {
        AfxThrowError();
    }
    else
    {
        if (!err)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                arxCurve c = curves[i];
                arxCurveFormat fmt = c->fmt;
                arxCurveInfo const* info = &infos[i];
                afxReal const* ctrlArray = info->ctrls;

                if (!info->src)
                {
                    if (!ctrlArray)
                    {
                        if (info->knotCnt)
                            AfxThrowError();
                    }
                    else
                    {
                        if (!info->knots)
                        {
                            if (info->knotCnt != 1 && (info->degree || fmt))
                                AfxThrowError();
                        }
                    }
                }

                if (!err)
                {
                    if ((info->sampleCnt != (afxUnit)-1) && info->origSamples)
                    {
                        AfxThrowError();
                        //((void(__cdecl *)(arxCurve*, int, int, const float *))*(f))(c, sampleCnt, info->sampleDimension, info->originalSamples);
                    }

                    if (info->src)
                    {
                        ArxCopyCurve(c, info->src);
                    }
                    else
                    {
                        if (fmt != arxCurveFormat_DaKeyframes32f)
                        {
                            ArxUpdateCurveKnots(c, info->knotCnt, info->dimens, info->knots, info->ctrls);
                        }
                        else
                        {
                            ArxUpdateCurveKnots(c, info->knotCnt, info->dimens, info->knots ? info->knots : (afxReal const[]) { 0.f }, info->ctrls);
                        }
                    }
                }
            }
        }
    }
    return err;
}
