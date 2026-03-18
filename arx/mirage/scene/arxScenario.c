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

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#define _ARX_SCENARIO_C
#include "arxIcd.h"

_ARX afxDrawSystem ArxGetScenarioDrawSystem(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxDrawSystem dsys = scio->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_ARX void* ArxGetScenarioUdd(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->udd;
}

_ARX afxChain const* _ArxScioGetDagRoots(arxScenario scio)
{
    afxError err = { 0 };
    // scio must be a valid arxScenario handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return &scio->dags;
}

_ARX afxStringBase _ArxScioGetAnimUrnStringBase(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->strbAnimMotUrns;
}

_ARX afxClass const* _ArxScioGetNodClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->nodCls;
    AFX_ASSERT_CLASS(cls, afxFcc_NOD);
    return cls;
}

_ARX afxClass const* _ArxScioGetGesClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->gesCls;
    AFX_ASSERT_CLASS(cls, afxFcc_GES);
    return cls;
}

_ARX afxClass const* _ArxScioGetAniClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->aniCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANI);
    return cls;
}

_ARX afxClass const* _ArxScioGetAnikClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->anikCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ANIK);
    return cls;
}

_ARX afxClass const* _ArxScioGetCurvClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->curCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CUR);
    return cls;
}

_ARX afxClass const* _ArxScioGetPupClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->pupCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PUP);
    return cls;
}

_ARX afxClass const* _ArxScioGetPuplClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->puplCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PUPL);
    return cls;
}

_ARX afxClass const* _ArxScioGetCapClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->motoCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PULL);
    return cls;
}

_ARX afxClass const* _ArxScioGetMotvClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->motvCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MOTV);
    return cls;
}

_ARX afxClass const* _ArxScioGetRbufClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->rbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RBUF);
    return cls;
}

_ARX afxClass const* _ArxScioGetMtdClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->mtdCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return cls;
}

_ARX afxClass const* _ArxScioGetMshClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->mshCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    return cls;
}

_ARX afxClass const* _ArxScioGetSklClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->sklCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SKL);
    return cls;
}

_ARX afxClass const* _ArxScioGetMshlClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->mshlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSHL);
    return cls;
}

_ARX afxClass const* _ArxScioGetMdlClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->mdlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return cls;
}

_ARX afxClass const* _ArxScioGetTerClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->terCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TER);
    return cls;
}

_ARX afxStringBase _ArxScioGetModelUrnStringBase(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->strbMdlSklMotUrns;
}

_ARX afxStringBase _ArxScioGetPivotTagStringBase(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->strbJointBiasesTags;
}

_ARX afxStringBase _ArxScioGetMorphTagStringBase(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->strbMorphTags;
}

_ARX afxClass const* _ArxScioGetPoseClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->poseCls;
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);
    return cls;
}

_ARX afxClass const* _ArxScioGetPlceClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->plceCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);
    return cls;
}

_ARX afxClass const* _ArxScioGetRndrClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->renderers;
    AFX_ASSERT_CLASS(cls, afxFcc_RND);
    return cls;
}

_ARX afxClass const* _ArxScioGetSkyClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->skyCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SKY);
    return cls;
}

_ARX afxClass const* _ArxScioGetLitClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const*cls = &scio->litCls;
    AFX_ASSERT_CLASS(cls, afxFcc_LIT);
    return cls;
}

_ARX afxClass const* _ArxScioGetCamClassCb_SW(arxScenario scio)
{
    afxError err = { 0 };
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->camCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);
    return cls;
}

_ARX afxClass const* _ArxScioGetTexClass(arxScenario scio)
{
    afxError err = { 0 };
    // scio must be a valid arxScenario handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->texCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TEX);
    return cls;
}

_ARX afxClass const* _ArxScioGetTechClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->dtecCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);
    return cls;
}

_ARX afxClass const* _ArxScioGetMshtClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->mshtCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSHT);
    return cls;
}

_ARX afxClass const* _ArxScioGetGeomClass(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxClass const* cls = &scio->geomCls;
    AFX_ASSERT_CLASS(cls, afxFcc_GEO);
    return cls;
}

_ARX void ArxQueryLodErrorTolerance(arxScenario scio, afxReal allowedErr, afxReal *allowedErrEnd, afxReal *allowedErrScaler)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    if (allowedErr > 0.0)
    {
        afxReal s = scio->allowedLodErrFadingFactor * allowedErr;
        *allowedErrEnd = s;
        *allowedErrScaler = 1.0 / (allowedErr - s);
    }
    else
    {
        *allowedErrEnd = 0.0;
        *allowedErrScaler = 0.0;
    }
}

_ARX afxReal _ArxScioGetAllowedLodErrorFadingFactor(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    return scio->allowedLodErrFadingFactor;
}

_ARX void ArxComputeSimilarity(arxScenario scio, afxReal unitsPerMeter, afxV3d const right, afxV3d const up, afxV3d const back, afxV3d const origin, afxM3d ltm, afxM3d iltm, afxV3d atv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(unitsPerMeter);
    AFX_ASSERT(origin);
    AFX_ASSERT(right);
    AFX_ASSERT(back);
    AFX_ASSERT(up);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);

    afxM3d srcAxisSys;
    AfxM3dSetTransposed(srcAxisSys, right, up, back);

    AfxM3dMultiply(ltm, scio->basis, srcAxisSys);

    afxReal lambda = scio->unitsPerMeter / unitsPerMeter;
    AfxV3dScale(ltm[0], ltm[0], lambda);
    AfxV3dScale(ltm[1], ltm[1], lambda);
    AfxV3dScale(ltm[2], ltm[2], lambda);

    AfxM3dInvert(iltm, ltm);

    AfxV3dSub(atv, scio->origin, origin);
}

_ARX afxError ArxUplinkTxds(arxScenario scio, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_RANGE(8, baseSlot, slotCnt);
    
    if (!uris)
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxClearUris(1, &scio->txdUris[i].uri);
        }
    }
    else
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxCopyUri(&scio->txdUris[i].uri, &uris[i]);
        }
    }
    return err;
}

#if 0 // removed
_ARX afxBool _ArxCapturePupCb(arxPuppet bod, void** udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PUP, 1, &bod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxDoesFrustumCullAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxUnit arrel;

        if (!AfxPushArrayUnits(pvs, 1, &arrel, &bod, sizeof(bod))) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_ARX afxError ArxCullPuppets(arxScenario scio, afxFrustum const* f, afxArray* pvs)
{
    afxError err = { 0 };
    // scio must be a valid arxScenario handle.
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(pvs);
    AFX_ASSERT(pvs->cap);
    AFX_ASSERT(f);

    // TODO: Move to implementation

    afxReal lodErr = 0.f;
    ArxInvokePuppets(scio, 0, AFX_U32_MAX, (void*)_ArxCapturePupCb, (void*[]) { (void*)f, &lodErr, pvs });

    return err;
}
#endif

_ARX afxError _ArxScioDtorCb(arxScenario scio)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    AfxExhaustChainedClasses(&scio->classes);

    AFX_ASSERT(!scio->idd);

    AfxDeregisterChainedClasses(&scio->classes);

    return err;
}

_ARX afxError _ArxScioCtorCb(arxScenario scio, void** args, afxUnit invokeNo)
{
    //AfxEntry("scio=%p,uri=%.*s", scio, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STRING_EMPTY);
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    AfxZero(scio, sizeof(scio[0]));

    afxModule drv = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(args[1]);
    _arxScioAcquisition const* cfg = ((_arxScioAcquisition const *)args[1]) + invokeNo;

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);
    scio->dsys = cfg->dsys;

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxChain *classes = &scio->classes;
    AfxMakeChain(classes, (void*)scio);

    afxUnit defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    scio->minIdxBufSiz = defStreamSiz / 4;
    scio->maxIdxBufSiz = 0;
    scio->minVtxBufSiz = defStreamSiz - scio->minIdxBufSiz;
    scio->maxVtxBufSiz = 0;

    scio->udd = cfg && cfg->cfg.udd ? cfg->cfg.udd : NIL;

    scio->procCb = cfg && cfg->cfg.proc ? cfg->cfg.proc : NIL;

    afxClassConfig camClsCfg = cfg->camClsCfg ? *cfg->camClsCfg : _ARX_CAM_CLASS_CONFIG;
    AFX_ASSERT(camClsCfg.fcc == afxFcc_CAM);
    AfxMountClass(&scio->camCls, NIL, classes, &camClsCfg);

    //tmpClsConf = _AvxGeomClsCfg;
    //AfxMountClass(&scio->geomCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxMshtClsCfg;
    //AfxMountClass(&scio->mshtCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AVX_TEX_CLASS_CONFIG;
    //AfxMountClass(&scio->texCls, NIL, classes, &tmpClsConf);

    afxClassConfig rbufClsCfg = cfg->rbufClsCfg ? *cfg->rbufClsCfg : _ARX_RBUF_CLASS_CONFIG;
    AFX_ASSERT(rbufClsCfg.fcc == afxFcc_RBUF);
    AfxMountClass(&scio->rbufCls, NIL, classes, &rbufClsCfg);

    afxClassConfig mtdClsCfg = cfg->mtdClsCfg ? *cfg->mtdClsCfg : _ARX_MTD_CLASS_CONFIG;
    AFX_ASSERT(mtdClsCfg.fcc == afxFcc_MTL);
    AfxMountClass(&scio->mtdCls, NIL, classes, &mtdClsCfg); // require tex

    afxClassConfig mshClsCfg = cfg->mshClsCfg ? *cfg->mshClsCfg : _ARX_MSH_CLASS_CONFIG;
    AFX_ASSERT(mshClsCfg.fcc == afxFcc_MSH);
    AfxMountClass(&scio->mshCls, NIL, classes, &mshClsCfg); // require msht, vtd

    afxClassConfig sklClsCfg = cfg->sklClsCfg ? *cfg->sklClsCfg : _ARX_SKL_CLASS_CONFIG;
    AFX_ASSERT(sklClsCfg.fcc == afxFcc_SKL);
    AfxMountClass(&scio->sklCls, NIL, classes, &sklClsCfg);

    afxClassConfig mshlClsCfg = cfg->mshlClsCfg ? *cfg->mshlClsCfg : _ARX_MSHL_CLASS_CONFIG;
    AFX_ASSERT(mshlClsCfg.fcc == afxFcc_MSHL);
    AfxMountClass(&scio->mshlCls, NIL, classes, &mshlClsCfg); // require msh, skl

    afxClassConfig mdlClsCfg = cfg->mdlClsCfg ? *cfg->mdlClsCfg : _ARX_MDL_CLASS_CONFIG;
    AFX_ASSERT(mdlClsCfg.fcc == afxFcc_MDL);
    AfxMountClass(&scio->mdlCls, NIL, classes, &mdlClsCfg); // require skl, msh

    afxClassConfig terClsCfg = cfg->terClsCfg ? *cfg->terClsCfg : _ARX_TER_CLASS_CONFIG;
    AFX_ASSERT(terClsCfg.fcc == afxFcc_TER);
    AfxMountClass(&scio->terCls, NIL, classes, &terClsCfg); // require all

    afxClassConfig rctxClsCfg = cfg->rctxClsCfg ? *cfg->rctxClsCfg : _ARX_RCTX_CLASS_CONFIG;
    AFX_ASSERT(rctxClsCfg.fcc == afxFcc_RCTX);
    AfxMountClass(&scio->rctxCls, NIL, classes, &rctxClsCfg); // require all

    scio->strbMdlSklMotUrns = NIL;
    scio->strbJointBiasesTags = NIL;
    scio->strbMorphTags = NIL;

    AfxAcquireStringCatalogs(1, &scio->strbMdlSklMotUrns);
    AfxAcquireStringCatalogs(1, &scio->strbJointBiasesTags);
    AfxAcquireStringCatalogs(1, &scio->strbMorphTags);

    afxClassConfig dtecClsCfg = _ARX_DTEC_CLASS_CONFIG;
    AfxMountClass(&scio->dtecCls, NIL, classes, &dtecClsCfg);

    afxClassConfig poseClsCfg = cfg->poseClsCfg ? *cfg->poseClsCfg : _ARX_POSE_CLASS_CONFIG;
    AFX_ASSERT(poseClsCfg.fcc == afxFcc_POSE);
    AfxMountClass(&scio->poseCls, NIL, classes, &poseClsCfg);

    afxClassConfig plceClsCfg = cfg->plceClsCfg ? *cfg->plceClsCfg : _ARX_PLCE_CLASS_CONFIG;
    AFX_ASSERT(plceClsCfg.fcc == afxFcc_PLCE);
    AfxMountClass(&scio->plceCls, NIL, classes, &plceClsCfg);

    afxClassConfig litClsCfg = cfg->litClsCfg ? *cfg->litClsCfg : _ARX_LIT_CLASS_CONFIG;
    AFX_ASSERT(litClsCfg.fcc == afxFcc_LIT);
    AfxMountClass(&scio->litCls, NIL, classes, &litClsCfg);

    afxClassConfig rndClsCfg = cfg->rndClsCfg ? *cfg->rndClsCfg : _ARX_RND_CLASS_CONFIG;
    AFX_ASSERT(rndClsCfg.fcc == afxFcc_RND);
    AfxMountClass(&scio->renderers, NIL, classes, &rndClsCfg); // require all

    afxClassConfig skyClsCfg = cfg->skyClsCfg ? *cfg->skyClsCfg : _ARX_SKY_CLASS_CONFIG;
    AFX_ASSERT(skyClsCfg.fcc == afxFcc_SKY);
    AfxMountClass(&scio->skyCls, NIL, classes, &skyClsCfg);

    // anim
    afxClassConfig clsCfg;

    clsCfg = _ARX_NOD_CLASS_CONFIG;
    AfxMountClass(&scio->nodCls, NIL, classes, &clsCfg);

    clsCfg = cfg->curClsCfg ? *cfg->curClsCfg : _ARX_CUR_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_CUR);
    AfxMountClass(&scio->curCls, NIL, classes, &clsCfg);

    clsCfg = cfg->gesClsCfg ? *cfg->gesClsCfg : _ARX_GES_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_GES);
    AfxMountClass(&scio->gesCls, NIL, classes, &clsCfg);

    clsCfg = cfg->aniClsCfg ? *cfg->aniClsCfg : _ARX_ANI_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_ANI);
    AfxMountClass(&scio->aniCls, NIL, classes, &clsCfg); // require ges

    clsCfg = cfg->anikClsCfg ? *cfg->anikClsCfg : _ARX_ANIK_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_ANIK);
    AfxMountClass(&scio->anikCls, NIL, classes, &clsCfg); // require ges

    clsCfg = cfg->capsClsCfg ? *cfg->capsClsCfg : _ARX_MOTO_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_PULL);
    AfxMountClass(&scio->motoCls, NIL, classes, &clsCfg);

    clsCfg = cfg->motvClsCfg ? *cfg->motvClsCfg : _ARX_MOTV_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_MOTV);
    AfxMountClass(&scio->motvCls, NIL, classes, &clsCfg);

    clsCfg = cfg->puplClsCfg ? *cfg->puplClsCfg : _ARX_PUPL_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_PUPL);
    AfxMountClass(&scio->puplCls, NIL, classes, &clsCfg); // require moto

    clsCfg = cfg->pupClsCfg ? *cfg->pupClsCfg : _ARX_PUP_CLASS_CONFIG;
    AFX_ASSERT(clsCfg.fcc == afxFcc_PUP);
    AfxMountClass(&scio->pupCls, NIL, classes, &clsCfg); // require moto

    AfxAcquireStringCatalogs(1, &scio->strbAnimMotUrns);


#if 0
    AFX_ASSERT(cfg->unitsPerMeter);
    AFX_ASSERT(!AfxV3dIsZero(cfg->right));
    AFX_ASSERT(!AfxV3dIsZero(cfg->up));
    AFX_ASSERT(!AfxV3dIsZero(cfg->back));

    AfxV3dCopy(scio->right, config->right);
    AfxV3dCopy(scio->up, config->up);
    AfxV3dCopy(scio->back, config->back);
#else
    AfxM3dReset(scio->basis);
    AfxV3dSet(scio->right, 1, 0, 0);
    AfxV3dSet(scio->up, 0, 1, 0);
    AfxV3dSet(scio->back, 0, 0, 1);
#endif
    AfxM3dSet(scio->basis, scio->right, scio->up, scio->back);

    AfxCopyBoxes(1, &cfg->extent, 0, &scio->extent, 0);
    AfxV3dCopy(scio->origin, cfg->origin);

    if (!cfg->unitsPerMeter)
        scio->unitsPerMeter = 1.f;
    else
        scio->unitsPerMeter = cfg->unitsPerMeter;

    if (!cfg->allowedLodErrFadingFactor)
        scio->allowedLodErrFadingFactor = 0.80000001;
    else
        scio->allowedLodErrFadingFactor = cfg->allowedLodErrFadingFactor;

    AfxMakeChain(&scio->dags, scio);


    scio->vbMgr = NIL;

    scio->nonRhcs = FALSE;
    scio->clipSpaceDepth = AvxGetSystemClipSpaceDepth(scio->dsys, NIL);

    return err;
}

_ARX afxClassConfig const _ARX_SCIO_CLASS_CONFIG =
{
    .fcc = afxFcc_SCIO,
    .name = "Scenario",
    .desc = "Draw Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(arxScenario)),
    .ctor = (void*)_ArxScioCtorCb,
    .dtor = (void*)_ArxScioDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxConfigureScenario(afxUnit icd, arxScenarioConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT(cfg);

    if (icd != AFX_INVALID_INDEX)
    {
        afxModule drv;
        if (!_ArxGetIcd(icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_ARX));

        afxClass* scioCls = (afxClass*)_ArxIcdGetScioClass(drv);
        AFX_ASSERT_CLASS(scioCls, afxFcc_SCIO);
    }

    afxDrawSystem dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
    AfxV3dSet(cfg->right, 1, 0, 0);
    AfxV3dSet(cfg->up, 0, 1, 0);
    AfxV3dSet(cfg->back, 0, 0, 1);
    AfxV3dZero(cfg->origin);
    AfxMakeAabb(&cfg->extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    cfg->unitsPerMeter = 1.f;
    cfg->allowedLodErrFadingFactor = 0.80000001;

    return err;
}

_ARX afxError ArxAcquireScenario(afxUnit icd, arxScenarioConfig const* cfg, arxScenario* input)
{
    afxError err = { 0 };
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* scioCls = NIL;
    afxModule drv = NIL;

    if (icd != AFX_INVALID_INDEX)
    {
        if (!_ArxGetIcd(icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_ARX));

        scioCls = (afxClass*)_ArxIcdGetScioClass(drv);
        AFX_ASSERT_CLASS(scioCls, afxFcc_SCIO);
    }
    else
    {
        static afxBool clsInited = FALSE;
        static afxClass staticScioCls = { 0 };

        if (!clsInited)
        {
            AfxMountClass(&staticScioCls, NIL, NIL, &_ARX_SCIO_CLASS_CONFIG);
            clsInited = TRUE;
        }

        scioCls = &staticScioCls;
        AFX_ASSERT_CLASS(scioCls, afxFcc_SCIO);
    }

    afxDrawSystem dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    _arxScioAcquisition cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.dsys = dsys;

    cfg2.allowedLodErrFadingFactor = cfg->allowedLodErrFadingFactor;
    cfg2.unitsPerMeter = cfg->unitsPerMeter;

    AfxV3dCopy(cfg2.right, cfg->right);
    AfxV3dCopy(cfg2.up, cfg->up);
    AfxV3dCopy(cfg2.back, cfg->back);
    AfxV3dCopy(cfg2.origin, cfg->origin);
    AfxCopyBoxes(1, &cfg->extent, 0, &cfg2.extent, 0);

    AFX_ASSERT_CLASS(scioCls, afxFcc_SCIO);

    arxScenario scio;
    if (AfxAcquireObjects(scioCls, 1, (afxObject*)&scio, (void const*[]) { drv, &cfg2 }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    AFX_ASSERT(input);
    *input = scio;

    return err;
}
