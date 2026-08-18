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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AFX_IOMMU_DDK_H
#define AFX_IOMMU_DDK_H

#include "afxIoBridgeDDK.h"
#include "afxFenceDDK.h"
#include "afxBufferDDK.h"

AFX_DECLARE_STRUCT(_afxIomIdd);

#ifndef _AFX_CORE_C
AFX_DECLARE_STRUCT(_afxIomDdi);
#else
AFX_DEFINE_STRUCT(_afxIomDdi)
{
    afxError(*waitCb)(afxIommu, afxUnit64);
    afxError(*waitFencCb)(afxIommu, afxUnit64, afxBool, afxUnit, afxFence const[], afxUnit64 const values[]);
    afxError(*resetFencCb)(afxIommu, afxUnit, afxFence const[]);
    afxError(*cohereCb)(afxIommu, afxBool invalidate, afxUnit, afxBufferedMap const[]);
    afxError(*remapCb)(afxIommu, afxBool, afxUnit, _afxBufRemapping const[]);
    afxUnit(*getProcCb)(afxIommu, afxUnit, afxString const[], void*[]);
    afxError(*transferCb)(afxIommu dsys, afxTransference* ctrl, afxUnit opCnt, void const* ops);
    afxError(*allocBufCb)(afxIommu, afxUnit, afxBufferInfo const[], afxBuffer[]);
    afxError(*deallocBufCb)(afxIommu, afxUnit, afxBuffer[]);

    afxClass const*(*exuCls)(afxIommu);
    afxClass const*(*fencCls)(afxIommu);
    afxClass const*(*bufCls)(afxIommu);
};
#endif

#ifdef _AFX_IOMMU_C

AFX_DEFINE_STRUCT(_afxIomAcq)
{
    afxIommuConfig          cfg;
    _afxExuAcq*             bridgeCfg;
    afxClassConfig const*   bufClsCfg; // buffer
    afxClassConfig const*   exuClsCfg; // bridge
    afxClassConfig const*   portClsCfg; // queue
    afxClassConfig const*   ctxClsCfg; // context
};

#ifdef _AFX_IOMMU_IMPL
AFX_OBJECT(_afxIommu)
#else
AFX_OBJECT(afxIommu)
#endif
{
    _afxIomDdi const*  ddi;
    _afxIomIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxChain            classes;
    afxClass            semaphores;
    afxClass            fencCls;
    afxClass            exuCls;
    afxClass            ctxCls;
    afxClass            bufCls;

    afxBool             running;
    afxUnit             bridgeCnt; // one per bridge
    afxIoBridge*        bridges;
    afxMask             ioExuMask;
    afxMask             dedIoExuMask;


    struct
    {
        afxBool         devLocal;
        afxChain        allocs;
    } heaps[2];
};
#ifdef _AFX_IOMMU_IMPL
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxIommu), m) == 0x00, "");
#else
//AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxIommu), m) == 0x00, "");
#endif
#endif//_AFX_IOMMU_C

AFX afxClassConfig const _AFX_CLASS_CONFIG_IOM;
AFX _afxIomDdi const _AFX_DDI_IOM;

AFX _afxIomDdi const* _AfxIomGetDdi(afxIommu iom);
AFX afxMask _AfxIomGetIoExuMask(afxIommu iom, afxMask* dedIoExuMask);

AFX afxClass const* _AfxIomSW_GetExuClassCb(afxIommu iom);
AFX afxClass const* _AfxIomSW_GetCtxClassCb(afxIommu iom);

AFX afxClass const* _AfxIomSW_GetFencClassCb(afxIommu iom);
AFX afxClass const* _AfxIomSW_GetBufClassCb(afxIommu iom);

#endif//AFX_IOMMU_DDK_H
