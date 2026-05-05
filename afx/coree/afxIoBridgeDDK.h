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

#ifndef AFX_IO_BRIDGE_DDK_H
#define AFX_IO_BRIDGE_DDK_H

#include "afxIoQueueDDK.h"

AFX_DEFINE_STRUCT(_afxExuAcq)
{
    afxDevice               dev;
    //afxUnit                 portId;
    afxUnit                 exuIdx;
    afxUnit                 minQueCnt;
    afxClassConfig const*   xqueClsCfg;
    //afxClassConfig const*   ctxClsCfg;
};

AFX_DECLARE_STRUCT(afxXpu);

AFX_DECLARE_STRUCT(_afxIddExu);
AFX_DEFINE_STRUCT(_afxDdiExu)
{
    afxError(*pingCb)(afxIoBridge, afxUnit);
    afxError(*waitCb)(afxIoBridge, afxUnit64);

};

#ifdef _AFX_IO_BRIDGE_C 
#ifdef _AFX_IO_BRIDGE_IMPL
AFX_OBJECT(_afxIoBridge)
#else
AFX_OBJECT(afxIoBridge)
#endif
{
    _afxDdiExu const*  ddi;
    _afxIddExu*        idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxDevice           dev;
    //afxUnit           portId;
    afxUnit             exuIdx;    
    afxChain            classes;
    afxClass            xqueCls;
    // one command pool per bridge to avoid thread interation.
    //afxClass          ctxCls;

    // XPU
    afxCondition        schedCnd; // signaled when some task is scheduled to this bridge.
    afxMutex            schedCndMtx;
    afxBool             schedCnt;
    afxThread           worker;
    afxInt              (*workerProc)(afxIoBridge);
    _afxIoReqLut const* iorpVmt;
    _afxCmdLut const*   cmdVmt;
    afxBool             (*procCb)(afxXpu*);
    afxXpu*             xpu;
};
#endif//_AFX_IO_BRIDGE_C

AFX afxClassConfig const _AFX_CLASS_CONFIG_EXU;

//AFX afxUnit _AfxCountIoQueues(afxIoBridge exu, afxUnit baseQueIdx);
AFX afxClass const* _AfxExuGetXqueClass(afxIoBridge exu);

AFX afxError _AfxExuRemapBuffers(afxIoBridge exu, afxBool unmap, afxUnit cnt, _afxBufferRemapping const maps[]);
AFX afxError _AfxExuCohereMappedBuffers(afxIoBridge exu, afxBool invalidate, afxUnit cnt, afxBufferedMap const maps[]);
AFX afxError _AfxExuTransferMemory(afxIoBridge exu, afxTransference* ctrl, afxUnit opCnt, void const* ops);

AFX afxError _AfxExu_PingCb(afxIoBridge exu, afxUnit queIdx);

AFX afxError _AfxAcquireIoQueues
(
    afxIommu iom,
    afxUnit cnt,
    afxMask const bridges[],
    afxIoQueue queues[]
);

AFX afxError _AfxIomBridgeDevices
(
    afxIommu iom, 
    afxUnit cnt, 
    _afxExuAcq const configs[],
    afxIoBridge bridges[]
);

#endif//AFX_BRIDGE_DDK_H
