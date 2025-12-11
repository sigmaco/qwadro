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

#ifndef ASX_CONTEXT_DDK_H
#define ASX_CONTEXT_DDK_H

#include "qwadro/pro/afxStepSystem.h"

typedef enum asxContextState
{
    asxContextState_INITIAL,
    asxContextState_RECORDING,
    asxContextState_EXECUTABLE,
    asxContextState_PENDING,
    asxContextState_INVALID,
    asxContextState_INTERNAL_EXECUTING
} asxContextState;

AFX_DECLARE_UNION(_asxCmd);

AFX_DECLARE_STRUCT(_asxIddSctx);

#ifndef _ASX_STEP_C
AFX_DECLARE_STRUCT(_asxDdiSctx);
#else
AFX_DEFINE_STRUCT(_asxDdiSctx)
{
    afxError(*prepare)(afxStepContext, afxBool purge, asxCmdFlags);
    afxError(*compile)(afxStepContext);
    afxError(*recycle)(afxStepContext, afxBool freeMem);
    afxError(*exhaust)(afxStepContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_asxCmdBatch)
{
    afxFcc          fcc;
    afxUnit         uniqId;
    afxLink         recyc;
    afxBool         once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool         deferred;
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.
    afxChain        commands;
    afxArena        cmdArenaCompiled;
};

#ifdef _ASX_STEP_CONTEXT_C
#ifdef _ASX_STEP_CONTEXT_IMPL
AFX_OBJECT(_asxStepContext)
#else
AFX_OBJECT(afxStepContext)
#endif
{
    _asxDdiSctx const*  ddi;
    _asxIddSctx*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
    asxLimits const*    devLimits; // dbg copies
    asxFeatures const*  enabledFeatures; // dbg copies

    asxAptitude         caps;
    afxMask             exuMask;
    afxAtom32           submCnt; // number of submissions
    afxMask64           submQueMask; // one for each queue where this dctx was submitted into.

    asxContextFlags     flags;
    afxStepContext      pool;
    afxChain            classes;
    afxClass            sctxCls;
    afxInterlockedQueue recycQue;
    asxCmdFlags         cmdFlags;
    asxContextState     state;
    afxArena            cmdArena; // owned by dsys data for specific port
    afxChain            commands;
};
#endif

AFX_DEFINE_STRUCT(_asxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_asxCmdLut)
{
    struct
    {
        void* _reservedToBeZero;
    };
    void(*f[])(void*, _asxCmd const*);
};

#define _ASX_CMD_ID(cmdName_) (offsetof(_asxCmdLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_asxCmd)
{
    _asxCmdHdr hdr;

    
};

ASX afxError _AsxSctxPushCmd(afxStepContext sctx, afxUnit id, afxUnit siz, afxCmdId* cmdId, _asxCmd** ppCmd);
ASX _asxCmdBatch* _AsxSctxGetCmdBatch(afxStepContext sctx, afxUnit idx);
ASX afxError _AsxStepResetCb(afxStepContext sctx, afxBool freeMem, afxBool permanent);
ASX afxError _AsxStepEndCb(afxStepContext sctx);

ASX afxClassConfig const _ASX_SCTX_CLASS_CONFIG;

ASX _asxDdiSctx const _ASX_SCTX_DDI;

ASX asxContextState _AsxSctxGetStatus(afxStepContext sctx);

#endif//ASX_CONTEXT_DDK_H
