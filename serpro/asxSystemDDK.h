/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   P R O C E S S E M E N T   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef ASX_SYSTEM_DDK_H
#define ASX_SYSTEM_DDK_H

#include "qwadro/pro/afxStepSystem.h"
#include "asxBufferDDK.h"
#include "asxSpuExecutor.h"

#ifndef _ASX_STEP_C
AFX_DECLARE_STRUCT(_asxDdiSsys);
#else
AFX_DEFINE_STRUCT(_asxDdiSsys)
{
    afxError(*waitCb)(afxStepSystem, afxUnit64);
    //afxError(*waitFencCb)(afxStepSystem, afxBool, afxUnit64, afxUnit, asxFence const[]);
    //afxError(*resetFencCb)(afxStepSystem, afxUnit, asxFence const[]);
    afxError(*cohereCb)(afxStepSystem, afxBool, afxUnit, asxBufferedMap const[]);
    afxError(*remapCb)(afxStepSystem, afxBool, afxUnit, _asxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxStepSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxStepSystem, asxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxStepSystem, afxUnit, asxBufferInfo const[], asxBuffer[]);
    afxError(*deallocBufCb)(afxStepSystem, afxUnit, asxBuffer[]);

    afxClass const*(*mexuCls)(afxStepSystem);
    afxClass const*(*fencCls)(afxStepSystem);
    afxClass const*(*sinkCls)(afxStepSystem);
    afxClass const*(*bufCls)(afxStepSystem);
};
#endif


AFX_DEFINE_STRUCT(__asxSsysAcq)
{
    afxUnit             bridgeCnt;
    asxFeatures         reqFeatures;
    afxUnit             reqExtCnt;
    afxString const*    reqExts;
    void*               udd;
    afxString           tag;
    afxDrawSystem       dsys;

    afxClassConfig const* sbufClsCfg;

    afxClassConfig const* sexuClsCfg;
    afxClassConfig const* sctxClsCfg;
};

AFX_DECLARE_STRUCT(_asxIddSsys);

#ifdef _ASX_STEP_SYSTEM_C
#ifdef _ASX_STEP_SYSTEM_IMPL
AFX_OBJECT(_asxStepSystem)
#else
AFX_OBJECT(afxStepSystem)
#endif
{
    AFX_OBJECT(afxDevLink) ctx;
    _asxDdiSsys const*      ddi;
    _asxIddSsys*            idd;
    // User-defined data.
    void*                   udd;
    // Debugging tag.
    afxString               tag;

    afxDrawSystem       dsys;

    afxBool             running;
    afxUnit             bridgeCnt;
    afxStepBridge*      bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;
    afxMask             cfxExuMask;
    afxMask             dedCfxExuMask;
    asxFeatures         requirements;
    asxLimits const*    limits;

    //afxChain            classes;
    afxClass            sexuCls;
    afxClass            sbufCls;
    afxClass            sctxCls;    
};
#ifdef _ASX_STEP_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseStepSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxStepSystem), ctx) == 0x00, "");
#endif
#endif//_ASX_STEP_SYSTEM_C

ASX afxClassConfig const _ASX_SSYS_CLASS_CONFIG;

ASX afxClass const* _AsxSsysGetSexuClass(afxStepSystem ssys);
ASX afxClass const* _AsxSsysGetSctxClass(afxStepSystem ssys);
ASX afxClass const* _AsxSsysGetBufClass(afxStepSystem ssys);

ASX _asxDdiSsys const* _AsxSsysGetDdi(afxStepSystem ssys);
ASX afxMask _AsxSsysGetIoExuMask(afxStepSystem ssys, afxMask* dedIoExuMask);

ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG;

#endif//ASX_SYSTEM_DDK_H
