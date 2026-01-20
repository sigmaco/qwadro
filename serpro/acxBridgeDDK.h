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

#ifndef ACX_BRIDGE_DDK_H
#define ACX_BRIDGE_DDK_H

#include "acxQueueDDK.h"

AFX_DEFINE_STRUCT(_acxSexuAcq)
{
    afxStepDevice           sdev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   sctxClsCfg;
};

AFX_DECLARE_STRUCT(acxSpu);

#ifdef _ACX_STEP_BRIDGE_C
#ifdef _ACX_STEP_BRIDGE_IMPL
AFX_OBJECT(_acxStepBridge)
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
    _acxIoReqLut const*iorpVmt;
    afxBool         (*procCb)(acxSpu*);
    acxSpu*         spu;
};
#endif//_ACX_STEP_BRIDGE_C

ACX afxClassConfig const _ACX_SEXU_CLASS_CONFIG;

ACX afxClass const* _AcxSexuGetSqueClass(afxStepBridge sexu);

ACX afxError _AcxSsysBridgeDevices(afxStepSystem ssys, afxUnit cnt, _acxSexuAcq const configs[], afxStepBridge bridges[]);

ACX afxError _AcxSexuRemapBuffers(afxStepBridge sexu, afxBool unmap, afxUnit cnt, _acxBufferRemapping const maps[]);
ACX afxError _AcxSexuCohereMappedBuffers(afxStepBridge sexu, afxBool discard, afxUnit cnt, acxBufferedMap const maps[]);
ACX afxError _AcxSexuTransferStepMemory(afxStepBridge sexu, acxTransference* ctrl, afxUnit opCnt, void const* ops);

ACX afxError _AcxSexu_PingCb(afxStepBridge sexu, afxUnit queIdx);

//ACX afxError _AcxSexuSubmitCallback(afxStepBridge sexu, afxError(*f)(void*, void*), void* udd);
ACX afxError _AcxSexuExecuteStepCommands(afxStepBridge sexu, afxUnit frameCnt, afxUnit cnt, afxStepContext contexts[]);

#endif//ACX_BRIDGE_DDK_H
