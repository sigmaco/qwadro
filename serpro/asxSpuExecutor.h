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

#ifndef ASX_IMPL___EXECUTOR_H
#define ASX_IMPL___EXECUTOR_H

#include "asxBridgeDDK.h"
#include "asxContextDDK.h"
#include "asxQueueDDK.h"

AFX_DEFINE_STRUCT(asxSpu)
{
    afxUnit         portId; // exuIdx
    afxUnit         exuIdx;
    afxStepBridge   sexu;
    afxThread       thr;
    afxStepSystem   ssys;

    afxBool         running;
    afxBool         instanced;
    afxSize         numOfFedSamples;

    afxStepContext  sctx;
};

// standard sound port methods

ASX _asxIoReqLut const _ASX_SPU_IORP_VMT;
ASX afxInt   _ASX_SPU_THREAD_PROC(afxStepBridge sexu);
ASX afxBool  _AsxSpu_ProcCb(asxSpu*);

ASX afxError _AsxSpuRollStepContexts(asxSpu* spu, afxStepContext mctx);

#endif//ASX_IMPL___EXECUTOR_H
