/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

  //////////////////////////////////////////////////////////////////////////////
 // DRAW DEVICE COMMAND, OPERATION AND COMMUNICATION QUUEUE                  //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

/*
    In Qwadro, a IO queue is an entity used in graphics programming and rendering to manage and organize the tasks related to IOing or rendering operations. 
    It efficiently handles the sequence and execution of IO commands, particularly in environments where multiple rendering tasks are required.

    A IO queue is a queue of tasks that need to be executed by the graphics system. 
    It organizes these tasks in the order they should be processed.

    The primary purpose of a IO queue is to manage the sequence and execution of IOing operations efficiently. 
    It ensures that rendering commands are executed in a controlled manner, optimizing performance and maintaining consistency in graphical output.

    The commands are processed in the order they are added to the queue. 
    This order is crucial for achieving the desired visual results, especially in complex scenes where the order of rendering affects the final output.

    The IO queue helps in managing synchronization between different rendering tasks, ensuring that operations are performed in the correct sequence and avoiding issues such as race conditions or visual artifacts.

    By batching and organizing IO commands, a IO queue can reduce the overhead associated with frequent state changes or direct hardware interactions.
    It ensures that rendering commands are executed in the correct order, maintaining visual consistency and accuracy.
    And it also helps manage complex scenes and multiple rendering tasks efficiently, which is crucial for high-performance graphics applications.
*/

#ifndef AFX_IO_QUEUE_H
#define AFX_IO_QUEUE_H

#include "qwadro/afxBuffer.h"
#include "qwadro/math/afxVector.h"

AFX_DEFINE_STRUCT(afxSubmission)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxFlags            flags;

    // An array of IO contexts that represent the individual IOing commands or operations to be executed. 
    // Each afxIoContext could contain specific information about a set of IOing commands, such as commands 
    // for rendering objects, setting up shaders, and managing resources.
    afxIoContext        ctx;

    afxFence            wait;
    afxUnit64           waitValue;
    afxMask             waitStageMask;
    afxUnit32           waitReserved;
    // A fence which will be signaled when the operation have completed execution.
    afxFence            signal;
    afxUnit64           signalValue;
    afxMask             signalStageMask;
    afxUnit32           signalReserved;

};

AFX_DEFINE_STRUCT(afxTransference)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask         exuMask;
    afxUnit         baseQueIdx;
    afxUnit         queCnt;

    // A semaphore upon which to wait on before the operation begin execution.
    afxFence    wait;
    afxUnit64   waitValue;
    afxMask     waitStageMask;
    afxUnit32   waitReserved;
    // A semaphore which will be signaled when the operation have completed execution.
    // A fence which will be signaled when the operation have completed execution.
    afxFence    signal;
    afxUnit64   signalValue;
    afxMask     signalStageMask;
    afxUnit32   signalReserved;

    union
    {
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

AFX afxIommu AfxGetIoQueueDock(afxIoQueue xque);

AFX afxError AfxWaitForEmptyIoQueue
(
    afxIoQueue xque, 
    afxUnit64 timeout
);

/// Open a queue debug label region.
/// Close a queue debug label region.
/// Insert a label into a queue.

AFX void AfxBeginIoQueueDebugScope
(
    afxIommu iom, 
    afxUnit exuIdx, 
    afxUnit queIdx, 
    afxString const* name, 
    afxV4d const color
);

AFX void AfxPushIoQueueDebugLabel
(
    afxIommu iom, 
    afxUnit exuIdx, 
    afxUnit queIdx, 
    afxString const* name, 
    afxV4d const color
);

AFX void AfxEndIoQueueDebugScope
(
    afxIommu iom, 
    afxUnit exuIdx, 
    afxUnit queIdx
);

#endif//AFX_IO_QUEUE_H
