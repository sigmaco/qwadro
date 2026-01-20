/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_SCENARIO_DDK_H
#define ARX_SCENARIO_DDK_H

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/scene/arxScenario.h"
#include "qwadro/iris/arxRenderContext.h"

#include "../cad/arxModelDDK.h"
#include "../cad/arxPoseDDK.h"
#include "../cad/arxTerrainDDK.h"
//#include "arxAnimationImpl.h"
//#include "arxBodyImpl.h"
//#include "arxCurveImpl.h"
#include "../scene/arxBufferDDK.h"
#include "../scene/arxCameraDDK.h"
#include "../scene/arxNodeDDK.h"
#include "../scene/arxIlluminationDDK.h"
#include "../scene/arxSkyDDK.h"

#include "../anim/arxInstancedAnimationDDK.h"
#include "../anim/arxCurveDDK.h"
#include "../anim/arxPuppetDDK.h"


AFX_DEFINE_STRUCT(_arxScioAcquisition)
{
    arxScenarioConfig     cfg;
    afxDrawSystem           dsys;

    afxClassConfig const*   rbufClsCfg;

    afxClassConfig const*   mtdClsCfg;
    afxClassConfig const*   mshClsCfg;
    afxClassConfig const*   sklClsCfg;
    afxClassConfig const*   mshlClsCfg;
    afxClassConfig const*   mdlClsCfg;
    afxClassConfig const*   poseClsCfg;
    afxClassConfig const*   plceClsCfg;

    afxClassConfig const*   terClsCfg;
    afxClassConfig const*   litClsCfg;
    afxClassConfig const*   rndClsCfg;
    afxClassConfig const*   skyClsCfg;
    afxClassConfig const*   camClsCfg;

    afxClassConfig const*   rctxClsCfg;

    // anim

    afxClassConfig const* curClsCfg;
    afxClassConfig const* gesClsCfg;
    afxClassConfig const* capsClsCfg;
    afxClassConfig const* aniClsCfg;
    afxClassConfig const* anikClsCfg;
    afxClassConfig const* pupClsCfg;
    afxClassConfig const* puplClsCfg;
    afxClassConfig const* motvClsCfg;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxBox              extent;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;
};

#ifdef _ARX_SCENARIO_C
AFX_DEFINE_STRUCT(_arxScioEndpoint)
{
    afxLink          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

#ifdef _ARX_SCENARIO_IMPL
AFX_OBJECT(_arxScenario)
#else
AFX_OBJECT(arxScenario)
#endif
{
    afxChain            classes;
    afxClass            dtecCls;
    afxClass            texCls;
    afxClass            camCls;

    afxClass            mshtCls;
    afxClass            geomCls;


    afxClass        litCls;
    afxClass        renderers;
    afxClass        skyCls;
    afxClass        poseCls;
    afxClass        plceCls;

    afxClass        rbufCls;
    afxClass        mtdCls;
    afxClass        mshCls;
    afxClass        sklCls;
    afxClass        mshlCls;
    afxClass        mdlCls;
    afxClass        terCls;
    afxClass        rctxCls;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;



    afxClass      nodCls;
    afxClass      curCls;
    afxClass      gesCls;
    afxClass      motvCls;
    afxClass      aniCls;
    afxClass      anikCls;
    afxClass      motoCls;
    afxClass      pupCls;
    afxClass      puplCls;

    afxStringBase   strbAnimMotUrns;

    afxChain        dags;
    afxV4d          right;
    afxV4d          up;
    afxV4d          back;
    afxM3d          basis;
    afxV4d          origin;
    afxBox          extent;
    afxReal         unitsPerMeter;
    afxReal         allowedLodErrFadingFactor;


    struct smt* Smt;
    struct smt2* Smt2;


    afxDrawSystem       dsys;
    void*               vbMgr;

    afxBool             nonRhcs;
    avxClipSpaceDepth   clipSpaceDepth;

    afxUri128           txdUris[8];
    afxFile             txdHandles[8];

    afxUnit             minVtxBufSiz;
    afxUnit             maxVtxBufSiz;
    afxUnit             minIdxBufSiz;
    afxUnit             maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_ARX_SCENARIO_C

#ifdef _ARX_GEOMETRY_C

AFX_DEFINE_STRUCT(avxGeometryAttr)
{
    avxFormat           fmt;
    arxVertexFlags      flags;
    afxString8          usage; // 8
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_OBJECT(afxGeometry)
{
    //afxUnit              biasCnt;
    //arxVertexBias*      biases;
    afxUnit             vtxCnt;
    //akxVertex*          vtx; // one for each vertex
    afxUnit*            vtxToVtxMap; // vtxCnt
    afxUnit             attrCnt;
    avxGeometryAttr*    attrs;
    afxByte**           data;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.

    arxVertexCache      cache;
};

#endif

ARX afxClassConfig const _ARX_SCIO_CLASS_CONFIG;

ARX afxClass const* _ArxScioGetTechClass(arxScenario scio);
ARX afxClass*       _ArxScioGetIbufClass(arxScenario scio);
ARX afxClass*       _ArxScioGetVbufClass(arxScenario scio);
ARX afxClass const* _ArxScioGetTexClass(arxScenario scio);

ARX afxClass const* _ArxScioGetGeomClass(arxScenario scio);
ARX afxClass const* _ArxScioGetMshtClass(arxScenario scio);

ARX afxClass const* _ArxScioGetCamClassCb_SW(arxScenario scio);

ARX afxClass const* _ArxScioGetPoseClass(arxScenario scio);
ARX afxClass const* _ArxScioGetPlceClass(arxScenario scio);
ARX afxClass const* _ArxDinGetGeomClass(arxScenario scio);
ARX afxClass const* _ArxScioGetLitClass(arxScenario scio);
ARX afxClass const* _ArxScioGetRndrClass(arxScenario scio);
ARX afxClass const* _ArxScioGetSkyClass(arxScenario scio);

ARX afxClassConfig const _ARX_RND_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TER_CLASS_CONFIG;
ARX afxClassConfig const _ARX_TEX_CLASS_CONFIG;
ARX afxClassConfig const _ARX_DTEC_CLASS_CONFIG;
ARX afxClassConfig const _ARX_VBUF_CLASS_CONFIG;

ARX afxClass const* _ArxScioGetMtdClass(arxScenario scio);
ARX afxClass const* _ArxScioGetSklClass(arxScenario scio);
ARX afxClass const* _ArxScioGetMshlClass(arxScenario scio);
ARX afxClass const* _ArxScioGetMdlClass(arxScenario scio);
ARX afxClass const* _ArxScioGetMshClass(arxScenario scio);
ARX afxClass const* _ArxScioGetTerClass(arxScenario scio);
ARX afxStringBase   _ArxScioGetModelUrnStringBase(arxScenario scio);
ARX afxStringBase   _ArxScioGetPivotTagStringBase(arxScenario scio);
ARX afxStringBase   _ArxScioGetMorphTagStringBase(arxScenario scio);

ARX afxClass const* _ArxScioGetRbufClass(arxScenario scio);

ARX afxClass const* _ArxScioGetCurvClass(arxScenario scio);
ARX afxClass const* _ArxScioGetCapClass(arxScenario scio);
ARX afxClass const* _ArxScioGetPupClass(arxScenario scio);
ARX afxClass const* _ArxScioGetPuplClass(arxScenario scio);
ARX afxClass const* _ArxScioGetGesClass(arxScenario scio);
ARX afxClass const* _ArxScioGetMotvClass(arxScenario scio);
ARX afxClass const* _ArxScioGetAniClass(arxScenario scio);
ARX afxClass const* _ArxScioGetAnikClass(arxScenario scio);

ARX afxStringBase   _ArxScioGetAnimUrnStringBase(arxScenario scio);

ARX afxChain const* _ArxScioGetDagRoots(arxScenario scio);

ARX afxReal _ArxScioGetAllowedLodErrorFadingFactor(arxScenario scio);

ARX afxClass const* _ArxScioGetNodClass(arxScenario scio);

#endif//ARX_SCENARIO_DDK_H
