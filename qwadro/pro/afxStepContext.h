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

#ifndef ASX_STEP_CONTEXT_H
#define ASX_STEP_CONTEXT_H

#include "qwadro/pro/asxBuffer.h"

typedef enum asxCmdFlag
{
    // Commands will be submitted once only and then automatically invalidated.
    asxCmdFlag_ONCE = AFX_BITMASK(0),
    // Commands will be side-loaded (inlined) by a front context.
    asxCmdFlag_DEFERRED = AFX_BITMASK(1),
    // Commands are considered entirely inside a steping scope (to be used by a front context).
    asxCmdFlag_SCOPED = AFX_BITMASK(2),
    // Commands will be shared across more than one SPU concurrently.
    asxCmdFlag_SHARED = AFX_BITMASK(3)
} asxCmdFlags;

typedef enum asxContextFlag
{
    asxContextFlag_TRANSIENT = AFX_BITMASK(0),
    asxContextFlag_MONOLITHIC = AFX_BITMASK(1)
} asxContextFlags;

AFX_DEFINE_STRUCT(asxContextConfig)
{
    asxAptitude     caps;
    afxMask         exuMask;
    asxContextFlags flags;
    asxCmdFlags     cmdFlags;
    afxUnit         auxCnt;
    // The capacity of recycle queue.
    afxUnit         recycCap;
    void*           udd;
    afxString       tag;
};

ASX afxError AsxAcquireStepContexts
(
    afxStepSystem msys,
    afxStepContext pool,
    asxContextConfig const* info,
    afxUnit cnt,
    afxStepContext contexts[]
);

ASX afxError AsxRecycleStepContexts
(
    afxBool freeRes, 
    afxUnit cnt, 
    afxStepContext contexts[]
);

ASX afxError AsxExecuteStepCommands
(
    afxStepSystem msys,
    afxUnit cnt, 
    asxSubmission const subms[]
);

////////////////////////////////////////////////////////////////////////////////

ASX afxMask AsxGetCommandPort
(
    afxStepContext step
);

ASX afxStepContext AsxGetCommandPool
(
    afxStepContext step
);

ASX afxError AsxExhaustStepContext
(
    afxStepContext mctx,
    afxBool freeMem
);

ASX afxError AsxPrepareStepCommands
(
    // The step context which the batch will be allocated from.
    afxStepContext mctx,
    afxBool purge,
    asxCmdFlags flags
);

ASX afxError AsxCompileStepCommands
(
    // The step context recording commands.
    afxStepContext mctx
);

#endif//ASX_STEP_CONTEXT_H
