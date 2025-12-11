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

#ifndef ASX_BRIDGE_DDK_H
#define ASX_BRIDGE_DDK_H

#include "asxQueueDDK.h"

AFX_DEFINE_STRUCT(_asxSexuAcq)
{
    afxStepDevice           sdev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   sctxClsCfg;
};

AFX_DECLARE_STRUCT(asxSpu);

#ifdef _ASX_STEP_BRIDGE_C
#ifdef _ASX_STEP_BRIDGE_IMPL
AFX_OBJECT(_asxStepBridge)
#else
AFX_OBJECT(afxStepBridge)
#endif
{
    afxStepDevice   sdev;
    //afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        squeCls;

    afxError        (*pingCb)(afxStepBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxStepBridge);
    _asxIoReqLut const*iorpVmt;
    afxBool         (*procCb)(asxSpu*);
    asxSpu*         spu;
};
#endif//_ASX_STEP_BRIDGE_C

ASX afxClassConfig const _ASX_SEXU_CLASS_CONFIG;

ASX afxClass const* _AsxSexuGetSqueClass(afxStepBridge sexu);

ASX afxError _AsxSsysBridgeDevices(afxStepSystem ssys, afxUnit cnt, _asxSexuAcq const configs[], afxStepBridge bridges[]);

ASX afxError _AsxSexuRemapBuffers(afxStepBridge sexu, afxBool unmap, afxUnit cnt, _asxBufferRemapping const maps[]);
ASX afxError _AsxSexuCohereMappedBuffers(afxStepBridge sexu, afxBool discard, afxUnit cnt, asxBufferedMap const maps[]);
ASX afxError _AsxSexuTransferStepMemory(afxStepBridge sexu, asxTransference* ctrl, afxUnit opCnt, void const* ops);

ASX afxError _AsxSexu_PingCb(afxStepBridge sexu, afxUnit queIdx);

//ASX afxError _AsxSexuSubmitCallback(afxStepBridge sexu, afxError(*f)(void*, void*), void* udd);
ASX afxError _AsxSexuExecuteStepCommands(afxStepBridge sexu, afxUnit frameCnt, afxUnit cnt, afxStepContext contexts[]);

#endif//ASX_BRIDGE_DDK_H
