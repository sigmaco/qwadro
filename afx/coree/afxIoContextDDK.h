/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#ifndef AFX_IO_CONTEXT_DDK_H
#define AFX_IO_CONTEXT_DDK_H

#include "afxFenceDDK.h"

typedef enum afxContextState
/// Each IO context is always in one of the following states
{
    // When a IO context is allocated, it is in the initial state.
    // Some commands are able to reset a IO context (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    // Command buffers in the initial state canv only be moved to the recording state, or freed.
    afxContextState_INITIAL,

    // BeginCmdBuffer() changes the state of a IO context from the initial state to the recording state.
    // Once a IO context is in the recording state, AfxCmd* commands canv be used to record to the IO context.
    afxContextState_RECORDING,

    // AfxCompileCmdBuffer() ends the recording of a IO context, and moves it from the recording state to the executable state.
    // Executable command buffers canv be submitted, reset, or recorded to another IO context.
    afxContextState_EXECUTABLE,

    // Queue submission of a IO context changes the state of a IO context from the executable state to the pending state.
    // Whilst in the pending state, applications must not attempt to modify the IO context in any way - as the device may be processing the commands recorded to it.
    // Once execution of a IO context completes, the IO context either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    // A synchronization command should be used to detect when this occurs.
    afxContextState_PENDING,

    // Some operations, such as modifying or deleting a resource that was used in a command recorded to a IO context, will transition the state of that IO context into the invalid state.
    // Command buffers in the invalid state canv only be reset or freed.
    afxContextState_INVALID,

    afxContextState_INTERNAL_EXECUTING,
} afxContextState;

AFX_DEFINE_STRUCT(_afxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_afxCmd)
{
    _afxCmdHdr hdr;

    struct
    {
        _afxCmdHdr hdr;

        afxUnit cnt;
        struct
        {
            afxIoContext dctx;
            afxUnit batchId;
        } AFX_SIMD contexts[];
    } ExecuteCommands;

    // DEBUG

    struct
    {
        _afxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } CommenceDebugScope;
    struct
    {
        _afxCmdHdr hdr;

        afxInt nothing;
    } ConcludeDebugScope;
    struct
    {
        _afxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } MarkDebugMilestone;

    // BUFFER OPERATIONS

    struct
    {
        _afxCmdHdr hdr;

        afxBuffer src;
        afxBuffer dst;
        afxUnit opCnt;
        afxBufferCopy AFX_SIMD ops[];
    } CopyBuffer;
    struct
    {
        _afxCmdHdr hdr;

        afxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxUnit value;
    } FillBuffer;
    struct
    {
        _afxCmdHdr hdr;

        afxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxByte AFX_SIMD data[];
    } UpdateBuffer;

};

AFX_DEFINE_UNION(_afxCmdLut)
{
    struct
    {
        void* _reservedToBeZero;

        void* ExecuteCommands;

        void* CommenceDebugScope;
        void* ConcludeDebugScope;
        void* MarkDebugMilestone;

        void* CopyBuffer;
        void* FillBuffer;
        void* UpdateBuffer;

    };
    void(*f[])(void*, _afxCmd const*);
};

#define _AFX_CMD_ID(cmdName_) (offsetof(_afxCmdLut, cmdName_) / sizeof(void*))

AFX_DECLARE_STRUCT(_afxCtxIdd);

#ifndef _AFX_DRAW_C
AFX_DECLARE_STRUCT(_afxCtxDdi);
#else
AFX_DEFINE_STRUCT(_afxCtxDdi)
{
    afxError(*exhaust)(afxIoContext, afxBool freeMem);
    afxError(*prepare)(afxIoContext, afxBool purge, afxCmdFlags);
    afxError(*compile)(afxIoContext);
    afxError(*recycle)(afxIoContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_afxCmdBatch)
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

AFX_DEFINE_STRUCT(_afxCtxRoll)
{


};

AFX_DEFINE_STRUCT(_afxCtxCmdBin)
{
    afxChain commands;

};

#ifdef _AFX_IO_CONTEXT_C
#ifdef _AFX_IO_CONTEXT_IMPL
AFX_OBJECT(_afxIoContext)
#else
AFX_OBJECT(afxIoContext)
#endif
{
    _afxCtxDdi const*   ddi;
    _afxCtxIdd*         idd;
    // Debugging tag.
    afxString           tag;
    // User-defined data.
    void*               udd;

    afxAptitude         caps;
    afxMask             exuMask;
    afxAtom32           submCnt; // number of submissions
    afxMask64           submQueMask; // one for each queue where this dctx was submitted into.

    // Is short-lived? That is, does not recycle batches, etc.
    //afxBool             transient;
    afxContextFlags     flags;
    afxIoContext        pool;
    afxChain            classes;
    afxClass            ctxCls;
    afxInterlockedQueue recycQue;
    afxCmdFlags         cmdFlags;
    afxContextState     state;
    afxArena            cmdArena; // owned by iom data for specific port
    afxChain            commands;

    afxChain            ioCmds;
    afxChain            IOCmds;
    afxChain            dispatchCmds;

    afxUnit             dbgUtilOpenLabelCnt;

};
#endif//_AFX_IO_CONTEXT_C

AFX _afxCmdBatch* _AfxCtxGetCmdBatch(afxIoContext ctx, afxUnit idx);
AFX _afxCmd* _AfxCtxPushCmd(afxIoContext ctx, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AFX afxError _AfxCtxImplResetCb(afxIoContext ctx, afxBool freeMem, afxBool permanent);
AFX afxError _AfxCtxImplEndCb(afxIoContext ctx);

AFX afxError _AfxCtxRecycleCb(afxIoContext ctx, afxBool freeRes);
AFX afxError _AfxCtxCompileCb(afxIoContext ctx);
AFX afxError _AfxCtxDiscardCb(afxIoContext ctx, afxBool freeRes);
AFX afxError _AfxCtxPrepareCb(afxIoContext ctx, afxBool purge, afxCmdFlags flags);
AFX afxError _AfxCtxExhaustCb(afxIoContext ctx, afxBool freeMem);

AFX afxClass const* _AfxCtxGetCtxClass(afxIoContext dctx);
AFX afxContextState _AfxCtxGetStatus(afxIoContext dctx);

AFX afxClassConfig const _AFX_CLASS_CONFIG_CTX;
AFX _afxCtxDdi const _AFX_DDI_CTX;


#endif//AFX_IO_CONTEXT_DDK_H
