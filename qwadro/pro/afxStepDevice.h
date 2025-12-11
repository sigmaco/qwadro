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

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Sound Experience on Qwadro                                      //
//////////////////////////////////////////////////////////////////////////////
// This section is part of SIGMA A4D; Advanced Audio Extensions for Qwadro.

// Qwadro Sound Synthesis Infrastructure

#ifndef ASX_STEP_DEVICE_H
#define ASX_STEP_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/pro/afxStepBridge.h"

AFX_DEFINE_STRUCT(asxFeatures)
{
    afxBool8 a;
};

AFX_DEFINE_STRUCT(asxLimits)
{
    int a;
};

AFX_DEFINE_STRUCT(asxDeviceInfo)
{
    asxAptitude     capabilities;
    afxAcceleration acceleration;
    afxUnit         minQueCnt; // usually 3
    afxUnit         maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    // User-defined data.
    void*           udd;
    // Debugging tag.
    afxString       tag;
};

  //////////////////////////////////////////////////////////////////////////////
 // DEVICE HANDLING                                                          //
//////////////////////////////////////////////////////////////////////////////

ASX afxBool AsxIsStepDevicePrompt
(
    afxStepDevice sdev
);

/*
    The AsxQueryStepDeviceFeatures() function is a way to query and retrieve detailed information about
    features of a specific stepping device. The amxFeatures structure holds the queried features, and after calling this function,
    you can access the capabilities of the device to make informed decisions about rendering or utilizing the device's features.
*/

ASX void AsxQueryStepDeviceFeatures
(
    // A handle for a stepping device to query for its features.
    afxStepDevice sdev, 

    // A pointer to a amxFeatures structure where the results will be stored.
    asxFeatures* features
);

/*
    The AsxQueryStepDeviceLimits() function queries the hardware limits of a specific stepping device.
    The device's capabilities are often constrained by the physical hardware, and knowing these limits
    is crucial when developing applications that need to operate efficiently within the device's capabilities.
    These limits are often critical for tasks such as optimizing memory usage, setting rendering parameters,
    or determining which rendering techniques can be supported.
*/

ASX void AsxQueryStepDeviceLimits
(
    // The device to query for its limits.
    afxStepDevice sdev, 

    // A amxLimits structure where the device's limits will be stored.
    asxLimits* limits
);

/*
    The AsxIsStepDeviceAcceptable() function determines if a given stepping device is suitable for use based on both the
    required features and hardware limits. If the device meets both the feature requirements and hardware constraints, it
    would be considered acceptable for the application. If it fails to meet either one, the device would not be considered acceptable.
*/

ASX afxBool AsxIsStepDeviceAcceptable
(
    // The device you are considering for use in your application.
    afxStepDevice sdev,

    // A amxFeatures structure containing the required features that the stepping device must support.
    asxFeatures const* features,

    // A amxLimits structure containing the hardware limits that the stepping device should meet or exceed.
    asxLimits const* limits
);

/*
    The AsxQueryStepCapabilities() function retrieves the capabilities of a synthesis device across multiple ports.
    It will return the stepping capabilities in the caps[] array. Each element in caps[] corresponds to the stepping
    capabilities of a specific port, and this information will help the application understand what features and stepping
    methods are supported by each port on the device.

    Returns the number of elements in caps[] array or the number of ports from specified base index.
*/

ASX void AsxQueryStepCapabilities
(
    // The stepping device to query.
    afxStepDevice sdev, 

    asxDeviceInfo* caps
);

  //////////////////////////////////////////////////////////////////////////////
 // IMPLEMENTATION DISCOVERY                                                 //
//////////////////////////////////////////////////////////////////////////////

/**
    The AsxEnumerateStepDevices() function enumerates the available stepping devices associated with an ICD.
    It allows you to discover which devices are available for rendering tasks on a system, which is important in
    scenarios where multiple devices (e.g., integrated and discrete GPUs) exist. The function provides the number
    of devices found and populates an array with the relevant devices for further processing. This is useful for
    applications that need to choose a specific device for rendering or computational tasks.

    Returns the number of step devices inserted in the @devices.
*/

ASX afxUnit AsxEnumerateStepDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The number ordinal for the first step device to start the iteration from.
    afxUnit first, 

    // The maximum number of step devices to be retrieved.
    afxUnit cnt, 

    // An array that will be populated with the stepping devices found during the enumeration.
    afxStepDevice devices[]
);

/*
    The AsxInvokeStepDevices() function provides an iterative mechanism to enumerate available stepping devices (e.g., GPUs)
    and invoke a callback function for each device. This approach is useful when you want to perform operations or checks
    on multiple devices without having to manually loop through them.

    Returns the count of found devices.
*/

ASX afxUnit AsxInvokeStepDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The ordinal number for the first stepping device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd, 

    // The callback function that will be invoked for each stepping device during enumeration.
    afxBool(*f)(void*,afxStepDevice), 

    // The maximum number of stepping devices to be considered during the invocation.
    afxUnit cnt
);

/*
    The AsxEvokeStepDevices() function provides an iterative mechanism to enumerate stepping devices and
    process each device through a callback function. It also stores the enumerated devices in the devices[] array,
    allowing further operations or checks to be performed after enumeration. This function offers flexibility in
    both processing devices and retrieving them for further use, making it useful for applications that need to
    interact with multiple available GPUs or stepping devices in a system.

    Returns the count of found devices.
*/

ASX afxUnit AsxEvokeStepDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The ordinal number for the first stepping device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd, 

    // The callback function that will be invoked for each stepping device during enumeration.
    afxBool(*f)(void*,afxStepDevice), 

    // The maximum number of stepping devices to be considered during the invocation.
    afxUnit cnt, 

    // An array that will be populated with the stepping devices found during the enumeration process.
    afxStepDevice devices[]
);

/*
    The AsxChooseStepDevices() function provides a way to select stepping devices that match specified feature and limit requirements.
    It returns the number of selected devices and populates an array with their device IDs. This function is helpful in scenarios
    where an application needs to filter and choose devices based on certain hardware capabilities or constraints, such as selecting
    GPUs that support specific rendering features or fall within particular performance limits.

    Returns the count of found devices.
*/

AVX afxUnit AsxChooseStepDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The operations or features that the device must supports.
    asxDeviceInfo const* caps,

    // A structure that specifies the features that the stepping devices must support.
    asxFeatures const* features,

    // A structure that defines the limits that the stepping devices should meet.
    asxLimits const* limits,

    // The maximum number of devices to be selected and returned in the @ddevIds array.
    afxUnit maxCnt,

    // An array that will be populated with the device IDs of the selected stepping devices.
    afxUnit sdevIds[]
);

#endif//ASX_STEP_DEVICE_H
