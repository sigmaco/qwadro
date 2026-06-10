/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AVX_SYSTEM_DDK_H
#define AVX_SYSTEM_DDK_H

#include "afxDrawBridgeDDK.h"
//#include "avxIommuDDK.h"
#include "avxFenceDDK.h"
#include "avxPipelineDDK.h"
#include "avxQueryPoolDDK.h"
#include "avxShaderDDK.h"
#include "avxLigatureDDK.h"
#include "avxVertexInputDDK.h"
#include "avxSamplerDDK.h"
#include "avxCanvasDDK.h"
#include "afxSurfaceDDK.h"
#include "avxRasterDDK.h"
#include "avxBufferDDK.h"

AFX_DECLARE_STRUCT(_avxDsysIdd);

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDsysDdi);
#else
AFX_DEFINE_STRUCT(_avxDsysDdi)
{
    afxError(*waitCb)(afxDrawSystem, afxUnit64, afxMask);
    afxError(*waitFencCb)(afxDrawSystem, afxUnit64, afxBool, afxUnit, avxFence const[], afxUnit64 const values[]);
    afxError(*resetFencCb)(afxDrawSystem, afxUnit, avxFence const[]);
    afxError(*cohereCb)(afxDrawSystem, afxBool invalidate, afxUnit, avxBufferedMap const[]);
    afxError(*remapCb)(afxDrawSystem, afxBool, afxUnit, _avxBufRemapping const[]);
    afxUnit(*getProcCb)(afxDrawSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocRasCb)(afxDrawSystem,afxUnit,avxRasterInfo const[],avxRaster[]);
    afxError(*deallocRasCb)(afxDrawSystem, afxUnit, avxRaster[]);
    afxError(*allocBufCb)(afxDrawSystem, afxUnit, avxBufferInfo const[], avxBuffer[]);
    afxError(*deallocBufCb)(afxDrawSystem, afxUnit, avxBuffer[]);

    afxError(*resetQryp)(afxDrawSystem, avxQueryPool, afxUnit base, afxUnit cnt);
    afxResult(*getQrypRslt)(afxDrawSystem, avxQueryPool, avxQueryResultFlags, afxUnit base, afxUnit cnt, afxSize cap, void* dst, afxSize stride);

    afxClass const*(*dexuCls)(afxDrawSystem);
    afxClass const*(*fencCls)(afxDrawSystem);
    afxClass const*(*qrypCls)(afxDrawSystem);
    afxClass const*(*vtxdCls)(afxDrawSystem);
    afxClass const*(*rasCls)(afxDrawSystem);
    afxClass const*(*bufCls)(afxDrawSystem);
    afxClass const*(*sampCls)(afxDrawSystem);
    afxClass const*(*pipCls)(afxDrawSystem);
    afxClass const*(*canvCls)(afxDrawSystem);
    afxClass const*(*shadCls)(afxDrawSystem);
    afxClass const*(*ligaCls)(afxDrawSystem);

    afxClass const*(*txdCls)(afxDrawSystem);

    afxError(*cfgSampCb)(afxDrawSystem, avxSamplerConfig*);
    afxError(*acqSampCb)(afxDrawSystem, afxUnit cnt, avxSamplerConfig const[], avxSampler[]);

    afxError(*cfgCanvCb)(afxDrawSystem, avxCanvasConfig*);
    afxError(*acqCanvCb)(afxDrawSystem, avxCanvasConfig const*, afxUnit cnt, avxCanvas[]);
};
#endif

#ifdef _AVX_DRAW_SYSTEM_C

AFX_DEFINE_STRUCT(_avxDsysAcq)
{
    avxSystemConfig     cfg;
    _avxDexuAcq*        bridgeCfg;
    afxClassConfig const* bufClsCfg;
    afxClassConfig const* rasClsCfg;
    afxClassConfig const* canvClsCfg; // req RAS
    afxClassConfig const* sampClsCfg;
    afxClassConfig const* ligaClsCfg; // req BUF, RAS, SAMP
    afxClassConfig const* shdClsCfg;
    afxClassConfig const* vtxdClsCfg;
    afxClassConfig const* pipClsCfg; // req SHD, LIGA, VTXD
    afxClassConfig const* qrypClsCfg;

    afxClassConfig const* doutClsCfg; // req RAS, CANV

    afxClassConfig const* dinClsCfg; // req BUF

    afxClassConfig const* camClsCfg;
    afxClassConfig const* txdClsCfg; // req RAS

    afxClassConfig const* dexuClsCfg;
    afxClassConfig const* dqueClsCfg;
    afxClassConfig const* dctxClsCfg;
};

#ifndef _AFX_CONTEXT_C
#   error "Require afxDevLink implementation"
#endif
#ifdef _AVX_DRAW_SYSTEM_IMPL
AFX_OBJECT(_avxDrawSystem)
#else
AFX_OBJECT(afxDrawSystem)
#endif
{
    AFX_OBJ(afxDevLink) ctx;

    _avxDsysDdi const*  ddi;
    _avxDsysIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxBool             running;
    afxUnit             bridgeCnt; // one per bridge
    afxDrawBridge*      bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;
    afxMask             cfxExuMask;
    afxMask             dedCfxExuMask;
    afxMask             gfxExuMask;
    afxMask             videoExuMask;

    avxFeatures     requirements;
    avxLimits const*limits;

    //afxChain classes;

    afxClass            bufCls;
    afxClass            rasCls;
    afxClass            canvCls; // req RAS
    afxClass            sampCls;
    afxClass            ligaCls; // req BUF, RAS, SAMP

    afxClass            vinCls;
    afxClass            shadCls;
    afxClass            pipCls; // req SHD, SHD, VTXD

    afxClass            qrypCls;

    afxClass            txdCls; // req RAS

    afxClass            dexuCls;
    afxClass            fencCls;
    afxClass            dctxCls;

    afxBool             nonRhcs;
    avxClipSpaceDepth   clipSpaceDepth;
    afxReal             clipSpaceDepthRangeEpsilon;

    struct
    {
        afxBool         devLocal;
        afxChain        allocs;
    } heaps[2];
};
#ifdef _AVX_DRAW_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxDrawSystem), m) == 0x00, "");
#endif
#endif//_AVX_DRAW_SYSTEM_C

AVX _avxDsysDdi const* _AvxDsysGetDdi(afxDrawSystem dsys);
AVX afxMask _AvxDsysGetIoExuMask(afxDrawSystem dsys, afxMask* dedIoExuMask);

AVX afxClass const* _AvxDsysSW_GetDexuClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetDctxClassCb(afxDrawSystem dsys);

AVX afxClass const* _AvxDsysSW_GetFencClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetBufClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetCanvClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetLigaClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetPipClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetQrypClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetRasClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetSampClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetShdClassCb(afxDrawSystem dsys);
AVX afxClass const* _AvxDsysSW_GetVinClassCb(afxDrawSystem dsys);

AVX afxClass const* _AvxDsysSW_GetTxdClassCb(afxDrawSystem dsys);

AVX avxFeatures const* _AvxDsysGetReqFeatures(afxDrawSystem dsys);
AVX avxLimits const* _AvxDsysGetLimits(afxDrawSystem dsys);

AVX afxClassConfig const _AVX_CLASS_CONFIG_DSYS;
AVX _avxDsysDdi const _AVX_DDI_DSYS;

AVX afxClassConfig const _AVX_CLASS_CONFIG_TXD;

AVX afxError _AvxDsysSW_TransferCb(afxDrawSystem dsys, avxTransference* ctrl, afxUnit opCnt, void const* ops);

#endif//AVX_SYSTEM_DDK_H
