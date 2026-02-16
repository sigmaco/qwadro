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

#ifndef ACX_CONTEXT_DDK_H
#define ACX_CONTEXT_DDK_H

#include "qwadro/pro/afxWarpSystem.h"

typedef enum acxContextState
{
    acxContextState_INITIAL,
    acxContextState_RECORDING,
    acxContextState_EXECUTABLE,
    acxContextState_PENDING,
    acxContextState_INVALID,
    acxContextState_INTERNAL_EXECUTING
} acxContextState;

AFX_DECLARE_UNION(_acxCmd);

AFX_DECLARE_STRUCT(_acxIddSctx);

#ifndef _ACX_WARP_C
AFX_DECLARE_STRUCT(_acxDdiSctx);
#else
AFX_DEFINE_STRUCT(_acxDdiSctx)
{
    afxError(*prepare)(afxWarpContext, afxBool purge, acxCmdFlags);
    afxError(*compile)(afxWarpContext);
    afxError(*recycle)(afxWarpContext, afxBool freeMem);
    afxError(*exhaust)(afxWarpContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_acxCmdBatch)
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

#ifdef _ACX_WARP_CONTEXT_C
#ifdef _ACX_WARP_CONTEXT_IMPL
AFX_OBJECT(_acxStepContext)
#else
AFX_OBJECT(afxWarpContext)
#endif
{
    _acxDdiSctx const*  ddi;
    _acxIddSctx*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
    acxLimits const*    devLimits; // dbg copies
    acxFeatures const*  enabledFeatures; // dbg copies

    acxAptitude         caps;
    afxMask             exuMask;
    afxAtom32           submCnt; // number of submissions
    afxMask64           submQueMask; // one for each queue where this dctx was submitted into.

    acxContextFlags     flags;
    afxWarpContext      pool;
    afxChain            classes;
    afxClass            sctxCls;
    afxInterlockedQueue recycQue;
    acxCmdFlags         cmdFlags;
    acxContextState     state;
    afxArena            cmdArena; // owned by dsys data for specific port
    afxChain            commands;
};
#endif

AFX_DEFINE_STRUCT(_acxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_acxCmdLut)
{
    struct
    {
        void* _reservedToBeZero;
    };
    void(*f[])(void*, _acxCmd const*);
};

#define _ACX_CMD_ID(cmdName_) (offsetof(_acxCmdLut, cmdName_) / sizeof(void*))

AFX_DEFINE_UNION(_acxCmd)
{
    _acxCmdHdr hdr;

    
};

ACX afxError _AcxSctxPushCmd(afxWarpContext sctx, afxUnit id, afxUnit siz, afxCmdId* cmdId, _acxCmd** ppCmd);
ACX _acxCmdBatch* _AcxSctxGetCmdBatch(afxWarpContext sctx, afxUnit idx);
ACX afxError _AcxStepResetCb(afxWarpContext sctx, afxBool freeMem, afxBool permanent);
ACX afxError _AcxStepEndCb(afxWarpContext sctx);

ACX afxClassConfig const _ACX_SCTX_CLASS_CONFIG;

ACX _acxDdiSctx const _ACX_SCTX_DDI;

ACX acxContextState _AcxSctxGetStatus(afxWarpContext sctx);

#endif//ACX_CONTEXT_DDK_H
