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

#ifndef ACX_IMPL___EXECUTOR_H
#define ACX_IMPL___EXECUTOR_H

#include "../acxBridgeDDK.h"
#include "../acxContextDDK.h"
#include "../acxQueueDDK.h"

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
