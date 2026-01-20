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

#ifndef ACX_STEP_CONTEXT_H
#define ACX_STEP_CONTEXT_H

#include "qwadro/pro/acxBuffer.h"

typedef enum acxCmdFlag
{
    // Commands will be submitted once only and then automatically invalidated.
    acxCmdFlag_ONCE = AFX_BITMASK(0),
    // Commands will be side-loaded (inlined) by a front context.
    acxCmdFlag_DEFERRED = AFX_BITMASK(1),
    // Commands are considered entirely inside a steping scope (to be used by a front context).
    acxCmdFlag_SCOPED = AFX_BITMASK(2),
    // Commands will be shared across more than one SPU concurrently.
    acxCmdFlag_SHARED = AFX_BITMASK(3)
} acxCmdFlags;

typedef enum acxContextFlag
{
    acxContextFlag_TRANSIENT = AFX_BITMASK(0),
    acxContextFlag_MONOLITHIC = AFX_BITMASK(1)
} acxContextFlags;

AFX_DEFINE_STRUCT(acxContextConfig)
{
    acxAptitude     caps;
    afxMask         exuMask;
    acxContextFlags flags;
    acxCmdFlags     cmdFlags;
    afxUnit         auxCnt;
    // The capacity of recycle queue.
    afxUnit         recycCap;
    void*           udd;
    afxString       tag;
};

ACX afxError AcxAcquireStepContexts
(
    afxStepSystem msys,
    afxStepContext pool,
    acxContextConfig const* info,
    afxUnit cnt,
    afxStepContext contexts[]
);

ACX afxError AcxRecycleStepContexts
(
    afxBool freeRes, 
    afxUnit cnt, 
    afxStepContext contexts[]
);

ACX afxError AcxExecuteStepCommands
(
    afxStepSystem msys,
    afxUnit cnt, 
    acxSubmission const subms[]
);

////////////////////////////////////////////////////////////////////////////////

ACX afxMask AcxGetCommandPort
(
    afxStepContext step
);

ACX afxStepContext AcxGetCommandPool
(
    afxStepContext step
);

ACX afxError AcxExhaustStepContext
(
    afxStepContext mctx,
    afxBool freeMem
);

ACX afxError AcxPrepareStepCommands
(
    // The step context which the batch will be allocated from.
    afxStepContext mctx,
    afxBool purge,
    acxCmdFlags flags
);

ACX afxError AcxCompileStepCommands
(
    // The step context recording commands.
    afxStepContext mctx
);

#endif//ACX_STEP_CONTEXT_H
