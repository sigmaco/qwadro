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
    afxWarpDevice           sdev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   squeClsCfg;
    afxClassConfig const*   sctxClsCfg;
};

AFX_DECLARE_STRUCT(acxSpu);

#ifdef _ACX_WARP_BRIDGE_C
#ifdef _ACX_WARP_BRIDGE_IMPL
AFX_OBJECT(_acxStepBridge)
#else
AFX_OBJECT(afxWarpBridge)
#endif
{
    afxWarpDevice   sdev;
    //afxUnit         portId;
    afxUnit         exuIdx;
    
    afxChain        classes;
    afxClass        squeCls;

    afxError        (*pingCb)(afxWarpBridge, afxUnit);

    // SPU
    afxCondition    schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex        schedCndMtx;
    afxBool         schedCnt;
    afxThread       worker;
    afxInt          (*workerProc)(afxWarpBridge);
    _acxIoReqLut const*iorpVmt;
    afxBool         (*procCb)(acxSpu*);
    acxSpu*         spu;
};
#endif//_ACX_WARP_BRIDGE_C

ACX afxClassConfig const _ACX_SEXU_CLASS_CONFIG;

ACX afxClass const* _AcxSexuGetSqueClass(afxWarpBridge sexu);

ACX afxError _AcxSsysBridgeDevices(afxWarpSystem ssys, afxUnit cnt, _acxSexuAcq const configs[], afxWarpBridge bridges[]);

ACX afxError _AcxSexuRemapBuffers(afxWarpBridge sexu, afxBool unmap, afxUnit cnt, _acxBufferRemapping const maps[]);
ACX afxError _AcxSexuCohereMappedBuffers(afxWarpBridge sexu, afxBool discard, afxUnit cnt, acxBufferedMap const maps[]);
ACX afxError _AcxSexuTransferWarpMemory(afxWarpBridge sexu, acxTransference* ctrl, afxUnit opCnt, void const* ops);

ACX afxError _AcxSexu_PingCb(afxWarpBridge sexu, afxUnit queIdx);

//ACX afxError _AcxSexuSubmitCallback(afxWarpBridge sexu, afxError(*f)(void*, void*), void* udd);
ACX afxError _AcxSexuExecuteWarpCommands(afxWarpBridge sexu, afxUnit frameCnt, afxUnit cnt, afxWarpContext contexts[]);

#endif//ACX_BRIDGE_DDK_H
