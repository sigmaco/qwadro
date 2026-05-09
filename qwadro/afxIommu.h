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

// This software is part of Advanced Video Graphics Extensions.

#ifndef AFX_IOMMU_H
#define AFX_IOMMU_H

#include "qwadro/afxDevice.h"
#include "qwadro/afxIoContext.h"
#include "qwadro/afxBuffer.h"
#include "qwadro/afxBuffered.h"
#include "qwadro/afxFence.h"

#define AFX_MAX_BRIDGES (32)

AFX_DEFINE_STRUCT(afxIommuConfig)
// The system-wide settings and parameters prefered/required for acquisition.
{
    afxUnit             verMajor;
    afxUnit             verMinor;
    // The functions to be capable on bridged devices.
    afxService          caps;
    // The acceleration to be available on bridged devices.
    afxAcceleration     accel;
    // The number of bridged devices' execution ports.
    afxUnit             exuCnt;
    // An array of configurations for each bridged device.
    afxIoBridgeConfig   exus[AFX_MAX_BRIDGES];
    // User-defined data attached to the system.
    void*               udd;
    // Debugging string attached to the system.
    afxString           tag;
};

AFX afxError AfxConfigureIommu
(
    // The Id of the installable client driver.
    afxUnit icd,
    // A pointer to a system configuration structure.
    afxIommuConfig* cfg
);

/*
    The AfxEstablishIommu() function establishes a new IOing system based on a specific driver and configuration.
    It provides the ability to establish a system for rendering and IOing operations, allowing for further interaction
    with the graphics pipeline (such as rendering, resource management, and more). This function is often used during the
    initialization phase of an application to prepare the system for graphical tasks.
*/

AFX afxError AfxEstablishIommu
(
    // The installable client driver (ICD) identifier. 
    // This is an integer that uniquely identifies the driver
    afxUnit icd,

    // A configuration structure that holds the parameters required to establish and configure the IOing system.
    afxIommuConfig const* cfg,

    // A pointer to an afxIommu where the created IOing system will be stored. 
    // The function will populate this pointer with the reference to the newly established IOing system.
    afxIommu* iommu
);

/*
    The AfxEnumerateIommus() function enumerates IOing systems established by a given installable client driver (ICD).
    By specifying a starting index and a count of systems to retrieve, you can query and retrieve information about the
    established IOing systems. This functionality is useful when your application needs to detect and interact with
    multiple systems, providing a way to select the best system for rendering.
*/

AFX afxUnit AfxEnumerateIommus
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration. 
    // If you want to start enumerating from the first available IOing system, you would set this value to 0. 
    // If you wish to start from a later point, you can provide an index specifying where to begin.
    afxUnit first,

    // The number of IO system to retrieve.
    afxUnit cnt,

    // An array where the enumerated IOing systems will be stored. 
    // After the function call, the array will contain the IOing systems up to the requested count @cnt.
    afxIommu iommus[]
);

/*
    The AfxInvokeIommus() function performs custom actions on a set of IOing systems established by a given ICD.
    By specifying a callback function, you can iterate over multiple IOing systems and apply specific logic to each system.
    This is useful when you need to perform system-specific operations, such as querying, configuring, or logging properties
    for each IOing system. The function provides a robust mechanism for handling multiple systems in a streamlined way.
*/

AFX afxUnit AfxInvokeIommus
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration of IOing systems. 
    // It specifies which system to start processing from.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void *udd,

    // A callback function that will be invoked for each IOing system being enumerated.
    afxBool(*f)(void*, afxIommu),

    // The number of IOing systems to process starting from the first index. 
    // The function will invoke the callback for each of these IOing systems.
    afxUnit cnt
);

/*
    The AfxEvokeIommus() function retrieves and process IOing systems in one go.
    It not only retrieves the systems but also invokes a callback function on each one to determine when push it to the @systems array,
    enabling you to perform custom logic while working with the retrieved systems.
    This function is useful when you need to both enumerate IOing systems and apply specific actions or checks to each system.
*/

AFX afxUnit AfxEvokeIommus
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The index of the first IOing system to retrieve. 
    // This allows you to start processing from a specific IOing system rather than always starting from the first.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void* udd,

    // The callback function that will be invoked for each IOing system.
    afxBool(*f)(void*, afxIommu),

    // The number of IOing systems to process starting from the first index. 
    // This specifies how many systems the function should attempt to retrieve.
    afxUnit cnt,

    // An array where the retrieved IOing systems will be stored. 
    // The function will fill this array with the actual IOing systems that were retrieved, up to the number @cnt.
    afxIommu iommus[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AfxGetSystemIcd() function retrieves the ICD (Installable Client Driver) running a specific IOing system. 
    This allows the application to determine which implementation is being used for the IOing system and to perform 
    further actions with the corresponding driver or module.
*/

AFX afxModule       AfxGetSystemIcd
(
    // The IOing system for which the ICD is being queried.
    afxIommu   iom
);

/*
    The AfxResolveSystemSymbols() function retrieves the address of a device-specific function that allows you to 
    dynamically load Vulkan function pointers for a device object at runtime. It is part of Qwadro's extensible and 
    dynamic approach, enabling developers to load Qwadro functions based on their needs, instead of hardcoding everything.

    Returns the number of procedures found and retrieved.
*/

AFX afxUnit         AfxResolveSystemSymbols
(
    // The established IOing system.
    afxIommu   iom, 

    // The number of requested procedures.
    afxUnit         cnt, 

    // An array of strings representing procedure names or identifiers. 
    // The function looks up procedures based on these names.
    afxString const names[], 

    // An array of function pointers to store the procedure addresses.
    void*           addresses[]
);

/*
    The AfxGetIoBridges() function retrieves IOing bridges for a established IOing system. 
    Ioing bridges are components that link and provide communication between an established system and its working devices. 
    This function allows applications to query and retrieve information about the established bridges, 
    which can be useful when dealing with systems that involve multiple hardware/software interfaces.

    Returns the number of arranged bridges. If @bridges is NIL, it returns the total number of bridges from the base index.
*/

AFX afxUnit         AfxGetIoBridges
(
    // The established IOing system.
    afxIommu   iom, 

    // The base index to begin the enumeration of the bridges.
    afxUnit         baseIdx, 

    // The number of bridges to be retrieved.
    afxUnit         cnt, 

    // An array where the function will store the retrieved IOing bridges.
    afxIoBridge   bridges[]
);

/*
    The AfxChooseIoBridges() function provides a way to select specific IOing bridges in an established IOing system, 
    filtered by device ID. The function returns the selected bridges in an array and allows the application 
    to filter the available bridges based on the provided indices. This is useful for applications that need to work 
    with multiple bridges or interfaces between components in a IOing system, such as managing communication between 
    the CPU and GPU or between different parts of the graphics pipeline.

    Returns the number of arranged IO bridges.
*/

AFX afxUnit         AfxChooseIoBridges
(
    // The established IOing system.
    afxIommu   iom, 

    // An optional device ID for which the bridges must be linked against.
    afxUnit         ddevId, 

    // An optional bitmask describing the IOing device's port capabilities for which bridges must be linked against. 
    afxAptitude     caps,

    afxMask         exuMask,

    // The first index of the bridges to begin selection from.
    afxUnit         first, 
    
    // The maximum number of bridges to be selected and returned.
    afxUnit         maxCnt, 

    // An array that will hold the selected IOing bridges.
    afxIoBridge   bridges[]
);

/*
    The AfxWaitForIommu() function waits for a IOing system to become ready, ensuring synchronization between the 
    application and the graphics context. It is particularly useful for managing asynchronous tasks or ensuring the system 
    is in a stable state before proceeding with further operations. It provides a way to wait for completion or readiness 
    while managing timeouts for better control over the execution flow.

    If the system is not ready within the given @timeout, the function may return an error or a timeout code.
*/

AFX afxError        AfxWaitForIommu
(
    // The IOing system that you want to wait for.
    afxIommu   iom, 
    
    // The timeout value that defines how long the function should wait before returning. 
    // It is expressed in microseconds and defines how long to wait for the IOing system 
    // to be ready or for the IOing operation to complete.
    afxUnit64       timeout
);

/*
    The AfxWaitForIoBridges() function waits for a specific bridge in a IOing system to become ready or finish its operation. 
    It is useful for synchronizing tasks in graphics pipelines or handling communication between multiple devices. 
    By providing a timeout, it ensures that the function does not block indefinitely and allows you to proceed with other 
    operations if the bridge does not become ready in time. This function is useful in contexts where bridges or execution 
    units handle asynchronous tasks, requiring careful synchronization to ensure that operations proceed in the correct order.

    If the bridge does not reach the ready state within the timeout, the function might return an error.
*/

AFX afxError        AfxWaitForIoBridges
(
    // The IOing system to which the specific bridge belongs.
    afxIommu   iom, 

    // The timeout period that the function should wait for the bridge to become ready or to complete its operation. 
    // The time is expressed in microseconds, and the function will stop waiting once this period has elapsed.
    afxUnit64       timeout,

    // The index of the execution unit (bridge) that needs to be waited on. 
    // If the IOing system has multiple bridges or execution units (e.g., for communication between different components or devices), 
    // this index identifies the particular bridge you're concerned with.
    afxMask         exuMask
);

/*
    The AfxWaitForIoQueue() function waits for a specific queue in a IOing system to become ready or finish its tasks. 
    It is useful for managing synchronization in systems that utilize multiple command queues, ensuring that one queue's 
    operations are complete before continuing with the next phase of processing. The timeout parameter provides control 
    over how long to wait, preventing the system from hanging indefinitely.

    If the queue does not become ready within this time frame, the function will return an error.
*/

AFX afxError        AfxWaitForIoQueue
(
    // The IOing system that contains the queue.
    afxIommu   iom,

    // The timeout period defines how long the function will wait for the queue to become ready. 
    // The time is specified in microseconds.
    afxUnit64       timeout,

    // The execution unit index, which likely refers to a specific queue or processing unit within the IOing system.
    // This helps to identify which execution unit's queue you are waiting for, especially if there are multiple queues in the system.
    afxUnit         exuIdx,

    // The queue you want to wait for. 
    // Ioing systems often have multiple command queues for various tasks like graphics, compute, or transfer operations. 
    // The queId specifies which queue's state should be checked.
    afxUnit         queId
);

AFX afxUnit AfxTestForEnabledSystemExtensions
(
    afxIommu iom, 
    afxUnit cnt, 
    afxString const* exts, 
    afxBool enabled[]
);

#endif//AFX_IOMMU_H
