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

#ifndef ACX_WARP_SYSTEM_H
#define ACX_WARP_SYSTEM_H

#include "qwadro/pro/afxWarpDevice.h"
#include "qwadro/pro/afxWarpContext.h"
#include "qwadro/pro/acxBuffer.h"

#define ACX_MAX_BRIDGES_PER_SYSTEM (32)

AFX_DEFINE_STRUCT(acxSystemConfig)
// The system-wide settings and parameters.
{
    afxUnit             verMajor;
    afxUnit             verMinor;
    // The functions to be capable on bridged devices.
    acxAptitude         caps;
    // The acceleration to be available on bridged devices.
    afxAcceleration     accel;
    // The features to be enabled.
    acxFeatures         features;
    // The number of system extensions to be enabled.
    afxUnit             extCnt;
    // An array of Qwadro strings containing the names of extensions to enable for the desired system.
    afxString const*    exts;
    // The draw system attached to the step system.
    afxDrawSystem       dsys;
    // The number of bridged devices' execution ports.
    afxUnit             exuCnt;
    // An array of configurations for each bridged device.
    acxBridgeConfig     exus[ACX_MAX_BRIDGES_PER_SYSTEM];
    // User-defined data attached to the system.
    void*               udd;
    // Debugging string attached to the system.
    afxString           tag;
};

ACX afxError AcxConfigureWarpSystem
(
    // The installable client driver (ICD) identifier. 
    // This is an integer that uniquely identifies the driver.
    afxUnit icd,

    // A configuration structure that holds the parameters required to establish and configure the warping system.
    acxSystemConfig* cfg
);

/*
    The AcxEstablishWarpSystem() function establishes a new warping system based on a specific driver and configuration.
    It provides the ability to establish a system for rendering and warping operations, allowing for further interaction
    with the synthesis pipeline (such as rendering, resource management, and more). This function is often used during the
    initialization phase of an application to prepare the system for tasks.
*/

ACX afxError AcxEstablishWarpSystem
(
    // The installable client driver (ICD) identifier. 
    // This is an integer that uniquely identifies the driver
    afxUnit icd, 

    // A configuration structure that holds the parameters required to establish and configure the warping system.
    acxSystemConfig const* cfg, 

    // A pointer to an afxMixSystem where the created warping system will be stored. 
    // The function will populate this pointer with the reference to the newly established warping system.
    afxWarpSystem* system
);

/*
    The AcxEnumerateWarpSystems() function enumerates warping systems established by a given installable client driver (ICD).
    By specifying a starting index and a count of systems to retrieve, you can query and retrieve information about the
    established warping systems. This functionality is useful when your application needs to detect and interact with
    multiple systems, providing a way to select the best system for rendering.
*/

ACX afxUnit AcxEnumerateWarpSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration. 
    // If you want to start enumerating from the first available warping system, you would set this value to 0. 
    // If you wish to start from a later point, you can provide an index specifying where to begin.
    afxUnit first,

    // The number of step system to retrieve.
    afxUnit cnt,

    // An array where the enumerated warping systems will be stored. 
    // After the function call, the array will contain the warping systems up to the requested count @cnt.
    afxWarpSystem systems[]
);

/*
    The AcxInvokeWarpSystems() function performs custom actions on a set of warping systems established by a given ICD.
    By specifying a callback function, you can iterate over multiple warping systems and apply specific logic to each system.
    This is useful when you need to perform system-specific operations, such as querying, configuring, or logging properties
    for each warping system. The function provides a robust mechanism for handling multiple systems in a streamlined way.
*/

ACX afxUnit AcxInvokeWarpSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration of warping systems. 
    // It specifies which system to start processing from.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void* udd,

    // A callback function that will be invoked for each warping system being enumerated.
    afxBool(*f)(void*, afxWarpSystem),

    // The number of warping systems to process starting from the first index. 
    // The function will invoke the callback for each of these warping systems.
    afxUnit cnt
);

/*
    The AcxEvokeWarpSystems() function retrieves and process warping systems in one go.
    It not only retrieves the systems but also invokes a callback function on each one to determine when push it to the @systems array,
    enabling you to perform custom logic while working with the retrieved systems.
    This function is useful when you need to both enumerate warping systems and apply specific actions or checks to each system.
*/

ACX afxUnit AcxEvokeWarpSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The index of the first warping system to retrieve. 
    // This allows you to start processing from a specific warping system rather than always starting from the first.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void* udd,

    // The callback function that will be invoked for each warping system.
    afxBool(*f)(void*, afxWarpSystem),

    // The number of warping systems to process starting from the first index. 
    // This specifies how many systems the function should attempt to retrieve.
    afxUnit cnt,

    // An array where the retrieved warping systems will be stored. 
    // The function will fill this array with the actual warping systems that were retrieved, up to the number @cnt.
    afxWarpSystem systems[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AcxGetWarpSystemIcd() function retrieves the ICD (Installable Client Driver) running a specific warping system.
    This allows the application to determine which implementation is being used for the warping system and to perform
    further actions with the corresponding driver or module.
*/

ACX afxModule AcxGetWarpSystemIcd
(
    // The warping system for which the ICD is being queried.
    afxWarpSystem ssys
);

/*
    The AcxGetWarpBridges() function retrieves warping bridges for a established warping system.
    Mixing bridges are components that link and provide communication between an established system and its working devices.
    This function allows applications to query and retrieve information about the established bridges,
    which can be useful when dealing with systems that involve multiple hardware/software interfaces.

    Returns the number of arranged bridges. If @bridges is NIL, it returns the total number of bridges from the base index.
*/

ACX afxUnit AcxGetWarpBridges
(
    // The established warping system.
    afxWarpSystem ssys, 

    // The base index to begin the enumeration of the bridges.
    afxUnit baseIdx, 

    // The number of bridges to be retrieved.
    afxUnit cnt, 

    // An array where the function will store the retrieved warping bridges.
    afxWarpBridge bridges[]
);

/*
    The AcxChooseWarpBridges() function provides a way to select specific warping bridges in an established warping system,
    filtered by device ID. The function returns the selected bridges in an array and allows the application
    to filter the available bridges based on the provided indices. This is useful for applications that need to work
    with multiple bridges or interfaces between components in a warping system, such as managing communication between
    the CPU and GPU or between different parts of the synthesis pipeline.
*/

ACX afxUnit AcxChooseWarpBridges
(
    // The established warping system.
    afxWarpSystem ssys, 

    // An optional device ID for which the bridges must be linked against.
    afxUnit mdevId,

    // An optional bitmask describing the warping device's port capabilities for which bridges must be linked against. 
    acxAptitude caps,

    afxMask exuMask,

    // The first index of the bridges to begin selection from.
    afxUnit first,

    // The maximum number of bridges to be selected and returned.
    afxUnit maxCnt,

    // An array that will hold the selected warping bridges.
    afxWarpBridge bridges[]
);

/*
    The AcxWaitForWarpSystem() function waits for a warping system to become ready, ensuring synchronization between the
    application and the synthesis context. It is particularly useful for managing asynchronous tasks or ensuring the system
    is in a stable state before proceeding with further operations. It provides a way to wait for completion or readiness
    while managing timeouts for better control over the execution flow.

    If the system is not ready within the given @timeout, the function may return an error or a timeout code.
*/

ACX afxError AcxWaitForWarpSystem
(
    // The warping system that you want to wait for.
    afxWarpSystem ssys, 

    // The timeout value that defines how long the function should wait before returning. 
    // It is expressed in microseconds and defines how long to wait for the warping system 
    // to be ready or for the warping operation to complete.
    afxTime timeout
);

/*
    The AcxWaitForWarpBridges() function waits for a specific bridge in a warping system to become ready or finish its operation.
    It is useful for synchronizing tasks in synthesis pipelines or handling communication between multiple devices.
    By providing a timeout, it ensures that the function does not block indefinitely and allows you to proceed with other
    operations if the bridge does not become ready in time. This function is useful in contexts where bridges or execution
    units handle asynchronous tasks, requiring careful synchronization to ensure that operations proceed in the correct order.

    If the bridge does not reach the ready state within the timeout, the function might return an error.
*/

ACX afxError AcxWaitForWarpBridges
(
    // The warping system to which the specific bridge belongs.
    afxWarpSystem ssys, 

    // The timeout period that the function should wait for the bridge to become ready or to complete its operation. 
    // The time is expressed in microseconds, and the function will stop waiting once this period has elapsed.
    afxTime timeout,

    // The bitmask of the execution units (bridge) that needs to be waited on. 
    // If the warping system has multiple bridges or execution units (e.g., for communication between different components or devices), 
    // this bitmask identifies the particular bridges you're concerned with.
    afxMask exuMask
);

/*
    The AcxWaitForWarpQueue() function waits for a specific queue in a warping system to become ready or finish its tasks.
    It is useful for managing synchronization in systems that utilize multiple command queues, ensuring that one queue's
    operations are complete before continuing with the next phase of processing. The timeout parameter provides control
    over how long to wait, preventing the system from hanging indefinitely.

    If the queue does not become ready within this time frame, the function will return an error.
*/

ACX afxError AcxWaitForWarpQueue
(
    // The warping system that contains the queue.
    afxWarpSystem ssys, 

    // The timeout period defines how long the function will wait for the queue to become ready. 
    // The time is specified in microseconds.
    afxTime timeout,

    // The execution unit index, which likely refers to a specific queue or processing unit within the warping system.
    // This helps to identify which execution unit's queue you are waiting for, especially if there are multiple queues in the system.
    afxUnit exuIdx, 

    // The queue you want to wait for. 
    // Mixing systems often have multiple command queues for various tasks like synthesis, compute, or transfer operations. 
    // The queId specifies which queue's state should be checked.
    afxUnit queId
);

ACX void AcxGetEnabledSystemFeatures
(
    afxWarpSystem ssys, 
    acxFeatures* features
);

#endif//ACX_WARP_SYSTEM_H
