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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced Parallel Computing Extensions.

#ifndef ACX_IMPL___EXECUTOR_H
#define ACX_IMPL___EXECUTOR_H

#include "../afxWarpBridgeDDK.h"
#include "../afxWarpContextDDK.h"
#include "../afxWarpQueueDDK.h"

AFX_DEFINE_STRUCT(acxSpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxWarpBridge   sexu;
    afxThread       thr;
    afxWarpSystem   ssys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedSamples;

    afxWarpContext  sctx;
};

// standard sound port methods

ACX _acxIoReqLut const _ACX_SPU_IORP_VMT;
ACX afxInt   _ACX_SPU_THREAD_PROC(afxWarpBridge sexu);
ACX afxBool  _AcxSpu_ProcCb(acxSpu*);

ACX afxError _AcxSpuRollWarpContexts(acxSpu* spu, afxWarpContext mctx);

#endif//ACX_IMPL___EXECUTOR_H
