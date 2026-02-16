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

#include "qwadro/pro/afxWarpSystem.h"
#include "acxBufferDDK.h"
#include "spu/acxSpuExecutor.h"

#ifndef _ACX_WARP_C
AFX_DECLARE_STRUCT(_acxDdiSsys);
#else
AFX_DEFINE_STRUCT(_acxDdiSsys)
{
    afxError(*waitCb)(afxWarpSystem, afxUnit64);
    //afxError(*waitFencCb)(afxWarpSystem, afxBool, afxUnit64, afxUnit, acxFence const[]);
    //afxError(*resetFencCb)(afxWarpSystem, afxUnit, acxFence const[]);
    afxError(*cohereCb)(afxWarpSystem, afxBool, afxUnit, acxBufferedMap const[]);
    afxError(*remapCb)(afxWarpSystem, afxBool, afxUnit, _acxBufferRemapping const[]);
    afxUnit(*getProcCb)(afxWarpSystem, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxWarpSystem, acxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxWarpSystem, afxUnit, acxBufferInfo const[], acxBuffer[]);
    afxError(*deallocBufCb)(afxWarpSystem, afxUnit, acxBuffer[]);

    afxClass const*(*mexuCls)(afxWarpSystem);
    afxClass const*(*fencCls)(afxWarpSystem);
    afxClass const*(*sinkCls)(afxWarpSystem);
    afxClass const*(*bufCls)(afxWarpSystem);
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

#ifdef _ACX_WARP_SYSTEM_C
#ifdef _ACX_WARP_SYSTEM_IMPL
AFX_OBJECT(_acxStepSystem)
#else
AFX_OBJECT(afxWarpSystem)
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
    afxWarpBridge*      bridges;
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
#ifdef _ACX_WARP_SYSTEM_IMPL
//AFX_STATIC_ASSERT(offsetof(struct _afxBaseStepSystem, ctx) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxWarpSystem), ctx) == 0x00, "");
#endif
#endif//_ACX_WARP_SYSTEM_C

ACX afxClassConfig const _ACX_SSYS_CLASS_CONFIG;

ACX afxClass const* _AcxSsysGetSexuClass(afxWarpSystem ssys);
ACX afxClass const* _AcxSsysGetSctxClass(afxWarpSystem ssys);
ACX afxClass const* _AcxSsysGetBufClass(afxWarpSystem ssys);

ACX _acxDdiSsys const* _AcxSsysGetDdi(afxWarpSystem ssys);
ACX afxMask _AcxSsysGetIoExuMask(afxWarpSystem ssys, afxMask* dedIoExuMask);

ACX afxClassConfig const _ACX_SBUF_CLASS_CONFIG;

#endif//ACX_SYSTEM_DDK_H
