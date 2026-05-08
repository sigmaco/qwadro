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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO DEVICE-SYNCHRONIZATION FENCE                                      //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

/// Fences are a synchronization primitive that can be used to insert a dependency from a queue to the host.
/// Fences have two states - signaled and unsignaled. 
/// A fence can be signaled as part of the execution of a queue submission command. 
/// Fences can be unsignaled on the host with AfxResetFences.
/// Fences can be waited on by the host with the AfxWaitForFences command, and the current state can be queried with AfxGetFenceStatus.

/// The internal data of a fence may include a reference to any resources and pending work associated with signal or unsignal operations performed on that fence object, collectively referred to as the fence’s payload.
/// Mechanisms to import and export that internal data to and from fences are provided below. 
/// These mechanisms indirectly enable applications to share fence state between two or more fences and other synchronization primitives across process and API boundaries.

#ifndef AFX_FENCE_H
#define AFX_FENCE_H

#include "qwadro/afxDevice.h"

typedef enum afxFenceFlag
{
    // The fence interacts with the CPU; that's, the host can wait on or reset it.
    // If not present, it is a device-only fence.
    afxFenceFlag_CPU        = AFX_BITMASK(0),
    // The fence is a timeline semaphore.
    // If not present, it is a binary semaphore.
    afxFenceFlag_PROGRESSIVE   = AFX_BITMASK(1),
    // The fence has been imported from other API.
    afxFenceFlag_EXTERNAL   = AFX_BITMASK(2)
} afxFenceFlags;

AFX_DEFINE_STRUCT(afxFenceInfo)
{
    afxFenceFlags   flags;
    // The initial value for a timeline semaphore.
    // For binary, any value makes the object be created in the signaled state.
    afxUnit64       initialVal;
    afxString       tag;
    void*           udd;

    afxFlags        exFlags;
    afxString       exType;
    void*           exHandle;
    afxString       exName;
};

AFX afxError AfxAcquireFences
// Create a new fence object.
(
    // The IO system that creates the fence.
    afxIommu iom,

    // The number of fences to be acquired.
    afxUnit cnt,

    // The information prescribing the fences to be acquired.
    afxFenceInfo const info[],

    // An array of handles in which the resulting fence objects are returned.
    afxFence fences[]
);

AFX afxError AfxWaitForFences
// Wait for one or more fences to become signaled.
(
    afxIommu iom,

    // The timeout period in units of nanoseconds.
    afxUnit64 timeout,

    // The condition is that all fences must be signaled, else at least one fence is signaled.
    afxBool waitAll,

    // The number of fences to wait on.
    afxUnit cnt,

    // An array of cnt fence handles.    
    afxFence const fences[],

    // An array of @cnt timeline values.
    // Optinal. Only for afxFenceFlag_PROGRESSIVE.
    afxUnit64 const values[]
);

AFX afxError AfxResetFences
// Resets one or more fence objects.
(
    afxIommu iom,
    // The number of fences to reset.
    afxUnit cnt,
    // An array of fence handles to reset.
    afxFence const fences[]
);

////////////////////////////////////////////////////////////////////////////////

// Return the IO system which this fence belongs to.
AFX afxIommu AfxGetFenceHost
(
    afxFence fenc
);

// Return the value/status of this fence.

AFX afxUnit64 AfxGetFenceValue
(
    afxFence fenc
);

AFX afxError AfxSignalFence
(
    afxFence fenc, 
    afxUnit64 value
);

AFX afxError AfxWaitForFence
(
    afxFence fenc, 
    afxUnit64 value, 
    afxUnit64 timeout
);

#endif//AFX_FENCE_H
