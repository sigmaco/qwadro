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

#ifndef ASX_BUFFER_DDK_H
#define ASX_BUFFER_DDK_H

#include "asxStorageDDK.h"

#ifdef _ASX_BUFFER_C
#ifdef _ASX_BUFFER_IMPL
AFX_OBJECT(_asxBuffer)
#else
AFX_OBJECT(asxBuffer)
#endif
{
    afxString       tag;
    void*           udd;
    asxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    asxBufferUsage  usage;
    asxBufferFlags  flags;
    asxFormat       fmt; // for tensor buffer
    afxUnit         fmtBps;
    afxUnit         fmtStride;

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _asxBufStorage  storage[1]; // non-sparse
};
#endif

ASX afxClassConfig const _ASX_SBUF_CLASS_CONFIG;

#endif//ASX_BUFFER_DDK_H
