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

#ifndef ACX_IMPL___TRANSFERENCE_H
#define ACX_IMPL___TRANSFERENCE_H

#include "acxSpuExecutor.h"

ACX afxError _AcxFillBuffer(acxBuffer buf, afxSize offset, afxSize range, afxUnit stride, afxUnit sampleCnt, afxUnit sampleRate, acxFormat fmt, afxReal amplitude, afxReal freq, afxReal dur);
ACX afxError _AcxDumpBuffer(acxBuffer buf, acxBufferIo const* op, void* dst);
ACX afxError _AcxUpdateBuffer(acxBuffer buf, acxBufferIo const* op, void const* src);
ACX afxError _AcxDownloadBuffer(acxBuffer buf, acxBufferIo const* op, afxStream iob);
ACX afxError _AcxUploadBuffer(acxBuffer buf, acxBufferIo const* op, afxStream iob);
ACX afxError _AcxCopyBuffer(acxBuffer buf, acxBufferCopy const* op, acxBuffer dst);

#endif//ACX_IMPL___TRANSFERENCE_H
