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

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Sound Experience on Qwadro                                      //
//////////////////////////////////////////////////////////////////////////////
// This section is part of SIGMA A4D; Advanced Audio Extensions for Qwadro.

// Qwadro Sound Synthesis Infrastructure

#ifndef ACX_STEP_DEVICE_H
#define ACX_STEP_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/pro/afxStepBridge.h"

AFX_DEFINE_STRUCT(acxFeatures)
{
    afxBool8 a;
};

AFX_DEFINE_STRUCT(acxLimits)
{
    int a;
};

AFX_DEFINE_STRUCT(acxDeviceInfo)
{
    acxAptitude     capabilities;
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

ACX afxBool AcxIsStepDevicePrompt
(
    afxStepDevice sdev
);

/*
    The AcxQueryStepDeviceFeatures() function is a way to query and retrieve detailed information about
    features of a specific stepping device. The amxFeatures structure holds the queried features, and after calling this function,
    you can access the capabilities of the device to make informed decisions about rendering or utilizing the device's features.
*/

ACX void AcxQueryStepDeviceFeatures
(
    // A handle for a stepping device to query for its features.
    afxStepDevice sdev, 

    // A pointer to a amxFeatures structure where the results will be stored.
    acxFeatures* features
);

/*
    The AcxQueryStepDeviceLimits() function queries the hardware limits of a specific stepping device.
    The device's capabilities are often constrained by the physical hardware, and knowing these limits
    is crucial when developing applications that need to operate efficiently within the device's capabilities.
    These limits are often critical for tasks such as optimizing memory usage, setting rendering parameters,
    or determining which rendering techniques can be supported.
*/

ACX void AcxQueryStepDeviceLimits
(
    // The device to query for its limits.
    afxStepDevice sdev, 

    // A amxLimits structure where the device's limits will be stored.
    acxLimits* limits
);

/*
    The AcxIsStepDeviceAcceptable() function determines if a given stepping device is suitable for use based on both the
    required features and hardware limits. If the device meets both the feature requirements and hardware constraints, it
    would be considered acceptable for the application. If it fails to meet either one, the device would not be considered acceptable.
*/

ACX afxBool AcxIsStepDeviceAcceptable
(
    // The device you are considering for use in your application.
    afxStepDevice sdev,

    // A amxFeatures structure containing the required features that the stepping device must support.
    acxFeatures const* features,

    // A amxLimits structure containing the hardware limits that the stepping device should meet or exceed.
    acxLimits const* limits
);

/*
    The AcxQueryStepCapabilities() function retrieves the capabilities of a synthesis device across multiple ports.
    It will return the stepping capabilities in the caps[] array. Each element in caps[] corresponds to the stepping
    capabilities of a specific port, and this information will help the application understand what features and stepping
    methods are supported by each port on the device.

    Returns the number of elements in caps[] array or the number of ports from specified base index.
*/

ACX void AcxQueryStepCapabilities
(
    // The stepping device to query.
    afxStepDevice sdev, 

    acxDeviceInfo* caps
);

  //////////////////////////////////////////////////////////////////////////////
 // IMPLEMENTATION DISCOVERY                                                 //
//////////////////////////////////////////////////////////////////////////////

/**
    The AcxEnumerateStepDevices() function enumerates the available stepping devices associated with an ICD.
    It allows you to discover which devices are available for rendering tasks on a system, which is important in
    scenarios where multiple devices (e.g., integrated and discrete GPUs) exist. The function provides the number
    of devices found and populates an array with the relevant devices for further processing. This is useful for
    applications that need to choose a specific device for rendering or computational tasks.

    Returns the number of step devices inserted in the @devices.
*/

ACX afxUnit AcxEnumerateStepDevices
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
    The AcxInvokeStepDevices() function provides an iterative mechanism to enumerate available stepping devices (e.g., GPUs)
    and invoke a callback function for each device. This approach is useful when you want to perform operations or checks
    on multiple devices without having to manually loop through them.

    Returns the count of found devices.
*/

ACX afxUnit AcxInvokeStepDevices
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
    The AcxEvokeStepDevices() function provides an iterative mechanism to enumerate stepping devices and
    process each device through a callback function. It also stores the enumerated devices in the devices[] array,
    allowing further operations or checks to be performed after enumeration. This function offers flexibility in
    both processing devices and retrieving them for further use, making it useful for applications that need to
    interact with multiple available GPUs or stepping devices in a system.

    Returns the count of found devices.
*/

ACX afxUnit AcxEvokeStepDevices
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
    The AcxChooseStepDevices() function provides a way to select stepping devices that match specified feature and limit requirements.
    It returns the number of selected devices and populates an array with their device IDs. This function is helpful in scenarios
    where an application needs to filter and choose devices based on certain hardware capabilities or constraints, such as selecting
    GPUs that support specific rendering features or fall within particular performance limits.

    Returns the count of found devices.
*/

AVX afxUnit AcxChooseStepDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The operations or features that the device must supports.
    acxDeviceInfo const* caps,

    // A structure that specifies the features that the stepping devices must support.
    acxFeatures const* features,

    // A structure that defines the limits that the stepping devices should meet.
    acxLimits const* limits,

    // The maximum number of devices to be selected and returned in the @ddevIds array.
    afxUnit maxCnt,

    // An array that will be populated with the device IDs of the selected stepping devices.
    afxUnit sdevIds[]
);

#endif//ACX_STEP_DEVICE_H
