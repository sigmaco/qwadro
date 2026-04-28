/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

/**
    The afxCondition object provides a condition variable for synchronizing threads.

    afxCondition allows a thread to tell other threads that some sort of condition has been met. 
    One or many threads can block waiting for a QWaitCondition to set a condition with wakeOne() or wakeAll(). 
    Use wakeOne() to wake one randomly selected thread or wakeAll() to wake them all.
*/

#ifndef AFX_CONDITION_H
#define AFX_CONDITION_H

#include "qwadro/exec/afxMutex.h"
#include "qwadro/exec/afxAtomic.h"

AFX_DEFINE_STRUCT(afxCondition)
{
#ifdef AFX_OS_WINDOWS
#   ifdef AFX_ISA_X86_64
    // must at least 64 bytes
    afxByte     data[AFX_ALIGN_SIZE(64, AFX_ATOMIC_ALIGNMENT)]; // 64 bytes
#else
    // must at least 36 bytes
    afxByte     data[AFX_ALIGN_SIZE(48, AFX_ATOMIC_ALIGNMENT)]; // 48 bytes
#   endif
#else
    afxByte     data[AFX_ALIGN_SIZE(48, AFX_ATOMIC_ALIGNMENT)]; // 48 bytes
#endif
};

AFX afxError            AfxDeployCondition(afxCondition* cond);
AFX void                AfxDismantleCondition(afxCondition* cond);

AFX afxError            AfxSignalCondition(afxCondition* cond);
AFX afxError            AfxSignalCondition2(afxCondition* cond);
AFX afxError            AfxWaitCondition(afxCondition* cond, afxMutex* mtx);
AFX afxError            AfxWaitTimedCondition(afxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts);

#endif//AFX_CONDITION_H
