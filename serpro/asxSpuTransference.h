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

#ifndef ASX_IMPL___TRANSFERENCE_H
#define ASX_IMPL___TRANSFERENCE_H

#include "asxSpuExecutor.h"

ASX afxError _AsxFillBuffer(asxBuffer buf, afxSize offset, afxSize range, afxUnit stride, afxUnit sampleCnt, afxUnit sampleRate, asxFormat fmt, afxReal amplitude, afxReal freq, afxReal dur);
ASX afxError _AsxDumpBuffer(asxBuffer buf, asxBufferIo const* op, void* dst);
ASX afxError _AsxUpdateBuffer(asxBuffer buf, asxBufferIo const* op, void const* src);
ASX afxError _AsxDownloadBuffer(asxBuffer buf, asxBufferIo const* op, afxStream iob);
ASX afxError _AsxUploadBuffer(asxBuffer buf, asxBufferIo const* op, afxStream iob);
ASX afxError _AsxCopyBuffer(asxBuffer buf, asxBufferCopy const* op, asxBuffer dst);

#endif//ASX_IMPL___TRANSFERENCE_H
