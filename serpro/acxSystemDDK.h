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

// This software is part of Advanced Multimedia Extensions.

#ifndef ACX_SYSTEM_DDK_H
#define ACX_SYSTEM_DDK_H

#include "qwadro/pro/afxStepSystem.h"
#include "acxBufferDDK.h"
#include "spu/acxSpuExecutor.h"

#ifndef _ACX_STEP_C
AFX_DECLARE_STRUCT(_acxDdiSsys);
#else
AFX_DEFINE_STRUCT(_acxDdiSsys)
{
    afxError(*waitCb)(afxStepSystem, afxUnit64);
    //afxError(*waitFencCb)(afxStepSystem, afxBool, afxUnit64, afxUnit, acxFence const[]);
    //afxError(*resetFencCb)(afxStepSystem, afxUnit, acxFence const[]);
    afxError(*cohereCb)(afxStepSystem, afxBool, afxUnit, acxBufferedMap const[]);
    afxError(*remapCb)(afxStepSystem, afxBool, afxUnit, _acxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxStepSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxStepSystem, acxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxStepSystem, afxUnit, acxBufferInfo const[], acxBuffer[]);
    afxError(*deallocBufCb)(afxStepSystem, afxUnit, acxBuffer[]);

    afxClass const*(*mexuCls)(afxStepSystem);
    afxClass const*(*fencCls)(afxStepSystem);
    afxClass const*(*sinkCls)(afxStepSystem);
    afxClass const*(*bufCls)(afxStepSystem);
};
#endif


AFX_DEFINE_STRUCT(__acxSsysAcq)
{
    afxUnit             bridgeCnt;
    acxFeatures         reqFeatures;
    afxUnit             reqExtCnt;
    afxString const*    reqExts;
    void*               udd;
    afxString           tag;
    afxDrawSystem       dsys;

    afxClassConfig const* sbufClsCfg;

    afxClassConfig const* sexuClsCfg;
    afxClassConfig const* sctxClsCfg;
};

AFX_DECLARE_STRUCT(_acxIddSsys);

#ifdef _ACX_STEP_SYSTEM_C
#ifdef _ACX_STEP_SYSTEM_IMPL
AFX_OBJECT(_acxStepSystem)
#else
AFX_OBJECT(afxStepSystem)
#endif
{
    AFX_OBJECT(afxDevLink) ctx;
    _acxDdiSsys const*      ddi;
    _acxIddSsys*            idd;
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
    acxFeatures         requirements;
    acxLimits const*    limits;

    //afxChain            classes;
    afxClass            sexuCls;
    afxClass            sbufCls;
    afxClass            sctxCls;    
};
#ifdef _ACX_STEP_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseStepSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxStepSystem), ctx) == 0x00, "");
#endif
#endif//_ACX_STEP_SYSTEM_C

ACX afxClassConfig const _ACX_SSYS_CLASS_CONFIG;

ACX afxClass const* _AcxSsysGetSexuClass(afxStepSystem ssys);
ACX afxClass const* _AcxSsysGetSctxClass(afxStepSystem ssys);
ACX afxClass const* _AcxSsysGetBufClass(afxStepSystem ssys);

ACX _acxDdiSsys const* _AcxSsysGetDdi(afxStepSystem ssys);
ACX afxMask _AcxSsysGetIoExuMask(afxStepSystem ssys, afxMask* dedIoExuMask);

ACX afxClassConfig const _ACX_SBUF_CLASS_CONFIG;

#endif//ACX_SYSTEM_DDK_H
