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

#ifndef AVX_BRIDGE_DDK_H
#define AVX_BRIDGE_DDK_H

#include "afxDrawQueueDDK.h"

AFX_DEFINE_STRUCT(_avxDexuAcq)
{
    afxDrawDevice           ddev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   dqueClsCfg;
    //afxClassConfig const*   dctxClsCfg;
};

AFX_DECLARE_STRUCT(avxDpu);

AFX_DECLARE_STRUCT(_avxDexuIdd);
AFX_DEFINE_STRUCT(_avxDexuDdi)
{
    afxError(*pingCb)(afxDrawBridge, afxUnit);
    afxError(*waitCb)(afxDrawBridge, afxUnit64);

};

#ifdef _AVX_DRAW_BRIDGE_C
#ifdef _AVX_DRAW_BRIDGE_IMPL
AFX_OBJECT(_avxDrawBridge)
#else
AFX_OBJECT(afxDrawBridge)
#endif
{
    _avxDexuDdi const*  ddi;
    _avxDexuIdd*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxDrawDevice       ddev;
    //afxUnit         portId;
    afxUnit             exuIdx;    
    afxChain            classes;
    afxClass            dqueCls;
    // one command pool per bridge to avoid thread interation.
    //afxClass            dctxCls;

    // DPU
    afxCondition        schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex            schedCndMtx;
    afxBool             schedCnt;
    afxThread           worker;
    afxInt              (*workerProc)(afxDrawBridge);
    _avxIoReqLut const* iorpVmt;
    _avxCmdLut const*   cmdVmt;
    afxBool             (*procCb)(avxDpu*);
    avxDpu*             dpu;
};
#endif//_AVX_DRAW_BRIDGE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_DEXU;

//AVX afxUnit _AvxCountDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx);
AVX afxClass const* _AvxDexuGetDqueClass(afxDrawBridge dexu);

AVX afxError _AvxDexuRemapBuffers(afxDrawBridge dexu, afxBool unmap, afxUnit cnt, _avxBufRemapping const maps[]);
AVX afxError _AvxDexuCohereMappedBuffers(afxDrawBridge dexu, afxBool invalidate, afxUnit cnt, avxBufferedMap const maps[]);
AVX afxError _AvxDexuTransferVideoMemory(afxDrawBridge dexu, avxTransference* ctrl, afxUnit opCnt, void const* ops);
AVX afxError _AvxDexuPresentSurfaces(afxDrawBridge dexu, afxUnit cnt, avxPresentation const presentations[], afxUnit queueingMap[]);
AVX afxError _AvxDexuScanSurfaces(afxDrawBridge dexu, afxUnit cnt, avxCaption const captions[], afxUnit queueingMap[]);

AVX afxError _AvxDexuResetQueries(afxDrawBridge dexu, avxQueryPool qryp, afxUnit base, afxUnit cnt);
AVX afxError _AvxDexuQueryResults(afxDrawBridge dexu, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride);

AVX afxError _AvxDexu_PingCb(afxDrawBridge dexu, afxUnit queIdx);

AVX afxError        _AvxAcquireDrawQueues
(
    afxDrawSystem   dsys,
    afxUnit         cnt,
    afxMask const   bridges[],
    afxDrawQueue    queues[]
);

AVX afxError _AvxDsysBridgeDevices(afxDrawSystem dsys, afxUnit cnt, _avxDexuAcq const configs[], afxDrawBridge bridges[]);

#endif//AVX_BRIDGE_DDK_H
