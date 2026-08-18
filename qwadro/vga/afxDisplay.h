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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.
// This software is part of Advanced User Experience Extensions.

#ifndef AFX_DISPLAY_H
#define AFX_DISPLAY_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/vga/gfx/avxPipeline.h"
#include "qwadro/vga/dma/avxRaster.h"
#include "qwadro/vga/dma/avxBuffer.h"
//#include "qwadro/vga/afxSurface.h"

#define AFX_MAX_GAMMA_CURVE_LENGTH (1025)

typedef enum afxDisplayType
{
    // The expected for a PC display; a fixed display.
    afxDisplay_FIXED,

    // Head-Mounted Display
    // The tracked display is attached to the user’s head. The user cannot touch the display itself. 
    // A VR headset would be an example of this form factor.
    afxDisplayType_HMD,

    // The tracked display is held in the user’s hand, independent from the user’s head. 
    // The user may be able to touch the display, allowing for screen-space UI. 
    // A mobile phone running an AR experience using pass-through video would be an example of this form factor.
    afxDisplayType_HANDHELD
} afxDisplayType;

typedef enum afxDisplayFlag
{
    // The display is attached to (and backing) the desktop.
    afxDisplayFlag_DESKTOP = AFX_BITMASK(0),
} afxDisplayFlags;

typedef enum avxVideoScaling
// mask specifying presentation scaling methods.
{
    // Do not scale.
    // Pixels are mapped to one and only one pixel in the surface.
    avxVideoScaling_ONE_TO_ONE = AFX_BITMASK(0),

    // Minify or magnify such that at least one of the resulting width or height is equal to the corresponding surface dimension, 
    // and the other resulting dimension is less than or equal to the corresponding surface dimension, 
    // with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    avxVideoScaling_ASPECT_RATIO = AFX_BITMASK(1),

    // minify or magnify such that the resulting dimensions are equal to those of the surface.
    avxVideoScaling_STRETCH = AFX_BITMASK(2)
} avxVideoScaling;

typedef enum avxVideoTransform
// This behavior is intended to be lower-level than high-level graphics APIs that directly offer "rotate 90°" or "rotate 180°".
// You will be working closer to the framebuffer/video plane level.
{
    // Token for "no transformation" what helps code clarity; video is left as-is.
    // When not specified, it is treated as IDENTITY, what does nothing about it.
    // If no option is chosen, the Qwadro treats it as identity.
    avxVideoTransform_IDENTITY,

    // Swaps the X and Y axes.
    // Image origin is swapped between horizontal and vertical; transpose.
    // It is equialent to 90 degrees rotated or 270 degrees rotated in shit APIs.
    avxVideoTransform_TRANSPOSE = AFX_BITMASK(0),

    // Vertical flip; top becomes bottom.
    // Invert pixel grid vertically.
    avxVideoTransform_FLIP      = AFX_BITMASK(1),
    
    // Horizontal flip; left becomes right.
    // Invert pixel grid horizontally.
    avxVideoTransform_MIRROR    = AFX_BITMASK(2),

    // Rotate 90o (clockwise), then flip vertically.
    // 90 degress rotated + V mirror
    avxVideoTransform_TFLIP     = (avxVideoTransform_TRANSPOSE | avxVideoTransform_FLIP),

    // Rotate 90o then flip horizontally.
    // 90 degrees rotated + H mirror
    avxVideoTransform_TMIRROR   = (avxVideoTransform_TRANSPOSE | avxVideoTransform_MIRROR),

    // Centers the video, especially if the video resolution is smaller than the output display area. 
    // Not a transform itself, but an alignment flag for CRTC.
    avxVideoTransform_CENTER    = AFX_BITMASK(3),

} avxVideoTransform;

typedef enum avxVideoAlpha
{
    // Ignore alpha component in the compositing process. 
    // The raster is treated as if it has a constant alpha of 1.0.
    avxVideoAlpha_OPAQUE    = AFX_BITMASK(0),

    // The compositor will respect the alpha component.
    avxVideoAlpha_PREMUL    = AFX_BITMASK(1),

    // The compositor will multiply the color components of the raster by the alpha component during compositing.
    avxVideoAlpha_POSTMUL   = AFX_BITMASK(2)

    // If no option is chosen, the way in which the presentation engine treats the alpha component in the images is unknown to the SIGMA GL/2 API. 
    // Instead, the application is responsible for setting the composite alpha blending mode using native window system commands. 
    // If the application does not set the blending mode using native window system commands, then a platform-specific default will be used.
} avxVideoAlpha;

typedef enum avxScanlining
// Flags indicating the method the raster uses to create an image on a surface.
{
    avxScanlining_IGNORE, // Scanline order is unspecified.
    avxScanlining_PROGRESSIVE, // The image is created from the first scanline to the last without skipping any.
    avxScanlining_INTERLACED_UFF, // upper field first; the even-numbered lines (0, 2, 4, 6, ...)
    avxScanlining_INTERLACED_LFF, // lower field first; the odd-numbered lines (1, 3, 5, 7, ...)
} avxScanlining;

/*
    The gamma curve here is a piecewise linear function; not a spline. 
    It's essentially a lookup table (LUT) or function approximation over the [0,1] domain using a set of explicitly defined control points.
    
    The array afxGammaCapabilites.ctrlPntPos are the X-values (inputs in [0,1] range).
    Often uniformly spaced, but the driver can specify non-uniform spacing.
    Think of this as: x0,x1,x2,...,xn-1 e [0, 1].

    The array afxGammaCurve.curve are the corresponding Y-values (R, G, B output intensities).
    So together, they define: (x0,y0),(x1,y1),...,(xn-1,yn-1).
    Each y_i is a afxV3d, meaning we have three gamma curves: one each for red, green, and blue.

    This is not a B-spline curve because the control points are explicit mappings, i.e., each (x, y) is a sample of the gamma function.
    Also there's no knot vector; the curve is not interpolated using basis functions. Also there's no degree (like cubic or quadratic).
    It’s meant to be rendering-efficient, hence uses simple linear interpolation between points.
*/

AFX_DEFINE_STRUCT(afxGammaCapabilites)
// The capabilities of the hardware for gamma correction, particularly how gamma control points can be used.
{
    // When TRUE, the hardware supports scaling and offsetting the gamma curve.
    // These are applied before gamma correction
    afxBool scaleAndOffsetSupported;
    // The maximum and minimum luminance values that the display can output after gamma conversion.
    // Useful for tone mapping and HDR scenarios.
    afxReal minConvertedValue;
    afxReal maxConvertedValue;
    // Number of gamma control points supported.
    // Can be up to 1025. This determines the resolution of the gamma curve.
    afxUnit ctrlPntCnt;
    // These are the normalized (0.0 to 1.0) input values for the gamma curve.
    // Corresponds to curve[] values in afxGammaCurve.
    afxReal ctrlPntPos[AFX_MAX_GAMMA_CURVE_LENGTH];
};

AFX_DEFINE_STRUCT(afxGammaCurve)
// Structure specifying/describing the gamma curve settings to apply to the display.
{
    // scale and offset are used if afxGammaCapabilites.scaleAndOffsetSupported is TRUE.
    // Each channel (Red, Green, Blue) has its own scaling and offset factor.
    // Formula for each output channel: output = scale * input + offset.
    afxV3d scale;
    afxV3d offset;
    // Defines the gamma curve for red, green, and blue channels.
    // The values correspond to the afxGammaCapabilites.ctrlPntPos.
    // You provide a afxV3d value (with R, G, B floats) for each control point position.
    afxV3d curve[AFX_MAX_GAMMA_CURVE_LENGTH];
};

AFX_DEFINE_STRUCT(afxDisplayPortInfo)
{
    afxDisplay vdu;
    afxChar name[32]; // the name of the display.
    afxChar label[128]; // the friendly name of the display.
    afxUnit dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit resWh[2]; // the physical, native, or preferred resolution of the display.
    avxVideoTransform supportedXforms; // transforms are supported by this display.
    afxBool planeReorder; // can re-arrange the planes on this display in any order relative to each other?
    afxBool persistentContent; // can submit persistent present operations on swapchains created against this display?
    afxRect workArea;
    afxRect fullArea;
};

AFX_DEFINE_STRUCT(afxDisplayInfo)
{
    afxDeviceInfo dev;
    afxDisplay vdu;
    afxChar name[32]; // the name of the display.
    afxChar label[128]; // the friendly name of the display.
    afxUnit dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit resWh[2]; // the physical, native, or preferred resolution of the display.
    afxUnit dpi[2];
    avxVideoTransform supportedXforms; // transforms are supported by this display.
    afxBool planeReorder; // can re-arrange the planes on this display in any order relative to each other?
    afxBool persistentContent; // can submit persistent present operations on swapchains created against this display?
};

AFX_DEFINE_STRUCT(afxDisplayCapabilities)
{
    avxRange currExtent;
    avxRange minBufExtent;
    avxRange maxBufExtent;
    afxUnit minBufCnt;
    afxUnit maxBufCnt;
    avxVideoTransform currTransform;
    avxVideoTransform supportedTransforms;
    avxVideoAlpha supportedCompositeAlpha;
    avxRasterUsage supportedUsage;
};

/*
    Mode-setting refers to configuring the display resolution, refresh rate, and color depth at a low level, 
    typically during system startup or when changing display settings. This is handled by:
        Kernel Mode Setting (KMS)
        User Mode Setting (UMS) (mostly deprecated)

    UMS is the main cause of silly behaviors of applications, mainly the older ones. It happens because the display
    is owned by the shell, not by the application. So the application should never change it for its private interests.
*/

AFX_DEFINE_STRUCT(afxDisplayMode)
// Describes a display mode.
{
    afxUnit width;
    afxUnit height;
    afxUnit freq;
    //afxUnit bpp;

    avxFormat fmt;
    avxVideoScaling scaling;
    avxScanlining scanlineOrdering;
    afxBool stereo;
};

AFX_DEFINE_STRUCT(afxDisplayPortConfig)
{
    afxUnit portId;
    // the name of the display.
    afxString name;
    // the friendly name of the display.
    afxString label;
    // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit dimWh[2];
    // the physical, native, or preferred resolution of the display.
    afxUnit resWh[2];
    afxUnit dpi[2];
    // desktop coordinates
    afxRect workArea;
    afxRect fullArea;
    // transforms are supported by this display.
    avxVideoTransform   supportedXforms;
    // can re-arrange the planes on this display in any order relative to each other?
    afxBool planeReorder;
    // can submit persistent present operations on swapchains created against this display?
    afxBool persistentContent;

    // The debugging tag to identify it.
    afxString tag;
    // An user-defined data to be attached to it.
    void* udd;
};

AFX_DEFINE_STRUCT(afxDisplayConfig)
{
    // The base information about the device.
    afxDeviceInfo dev;
    // the name of the display.
    afxString name;
    // the friendly name of the display.
    afxString label;

    // The debugging tag to identify it.
    afxString tag;
    // An user-defined data to be attached to it.
    void* udd;
};

AUX afxError AfxAcquireDisplays
(
    afxModule icd, 

    afxUnit cnt, 

    afxDisplayConfig const cfg[], 

    afxDisplay displays[]
);

/*
    The AfxEnumerateDisplays() function enumerates the available displays connected to a specific graphics driver (specified by icd). 
    It returns a list of display handles in the displays[] array, and you can specify which displays to start from (first) and how many to return (cnt). 
    This is useful for applications that need to discover all available monitors or screens in a multi-display environment.

    This function returns the number of displays successfully enumerated and returned in the displays[] array.

    In applications that support multiple monitors (e.g., games, media players, or productivity tools), 
    this function could be used to detect all available displays and dynamically adjust the output or window placement based on the number of screens connected.

    In applications that support multiple monitors (e.g., games, media players, or productivity tools), 
    this function could be used to detect all available displays and dynamically adjust the output or window placement based on the number of screens connected.

    In a graphics configuration or display management tool, AfxEnumerateDisplays could be used to detect all connected displays, 
    allowing the user to select a display for configuration or to apply settings (e.g., resolution, refresh rate, color adjustments).

    In systems that involve virtual desktops or remote desktop solutions, this function can be used to enumerate and display available screens or virtual displays, 
    helping the user to configure virtual displays or stream the content to multiple screens.

    If you are developing a system performance monitoring tool, 
    AfxEnumerateDisplays can be used to track which displays are connected and monitor the performance of each display or GPU.

    AfxEnumerateDisplays is a function designed to enumerate the available displays connected to a system, given a specific graphics context or device driver. 
    It is particularly useful for applications that need to manage or query multiple monitors in a system. 
    You can specify the starting point (first) and the number of displays to enumerate (cnt), 
    and the function returns an array of display handles, which you can then use to get additional information about each display.
*/

AUX afxUnit AfxEnumerateDisplays
(
    // This parameter likely refers to an identifier for a specific graphics context or device driver (ICD - Installable Client Driver). 
    // In multi-GPU systems or setups where multiple rendering contexts are available (e.g., NVIDIA Optimus, AMD Switchable Graphics, or external GPUs), 
    //this parameter might be used to specify which GPU or context the enumeration should be associated with.
    afxUnit icd,

    // This parameter likely specifies the index of the first display to enumerate. For example, 
    // setting this value to 0 could indicate that you want to start enumerating from the first display, 
    // while a higher value (e.g., 1 or 2) would allow you to skip certain displays or start from a specific point in the list.
    afxUnit first,

    // This parameter specifies the maximum number of displays to enumerate. 
    // It tells the function how many display identifiers to return in the displays[] array. 
    // If there are more available displays than the specified count, only the first cnt displays will be returned.
    afxUnit cnt,

    // This is an array where the function will store the enumerated displays. 
    // Each element in this array will likely represent a handle or identifier for a specific display connected to the system. 
    // The exact details stored in displays[] will depend on the underlying system, 
    // but typically it would contain information like the display’s resolution, color depth, or device name.
    afxDisplay displays[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AfxGetDisplayPort() function retrieves a specific display port for a particular display (dpy). 
    The function allows you to query which port or connection is being used for a particular display.

    This function returns an afxDisplay object, an identifier that represents the "port" or connection for a given display. 
    It could be an abstraction representing a display connection (e.g., HDMI, DisplayPort) or a specific graphical output configuration for a given display. 
    It may include information about the connection type, properties, or resolution.

    For example, in a system with multiple displays and multiple ports, 
    you might call this function to determine which output port (e.g., HDMI, DisplayPort, VGA) is connected to a given display. 
    This function shall return an afxDisplayPort object containing details about the port, such as its connection type, resolution, or other settings.

    In systems with multiple monitors, you might use AfxGetDisplayPort to retrieve information about which ports are active, 
    which ports are connected to which monitors, and the configuration of each port (resolution, refresh rate, etc.).

    In applications that need to dynamically configure or adjust display settings (e.g., gaming, multimedia, virtual desktop environments), 
    this function could help you determine what kind of ports are being used and how to adjust the display accordingly.

    In graphical systems that support different connection types or virtual ports (such as multi-GPU setups, VR systems, or advanced display configurations), 
    this function could provide essential information about which display port is currently active and its settings.

    AfxGetDisplayPort is used to retrieve information about the port or connection used by a particular display in a system. 
    It is useful in applications dealing with multiple monitors or advanced display setups where you need to determine the details about the connected ports, 
    such as the type of connection (HDMI, DisplayPort) or the configuration of the display.
*/

AUX afxDisplayPort AfxGetDisplayPort
(
    // This parameter likely represents the afxDisplay you are working with. 
    // It could be an identifier for a screen, windowing system, or graphical output that the function is querying. 
    // For example, in multi-display systems (e.g., dual monitors or more), dpy would refer to the specific display you want to access.
    afxDisplay dpy, 

    // This parameter refers to a "port" associated with the display. 
    // A "port" in this case might refer to an output or interface connected to the display, 
    // such as HDMI, VGA, DisplayPort, or other display connections. 
    // Alternatively, it could represent a logical or virtual port associated with a specific graphical display setup in a multi-monitor environment.
    afxUnit port
);

/*
    The AfxQueryDisplayModes() method queries the available display modes for a specific display (dpy) and port (port) and for a specific pixel format (fmt). 
    The function will return a list of supported display modes that match the provided criteria (such as resolution, refresh rate, and color format) in the modes[] array.

    The return value is an indicator of the actual number of display modes returned (indicating how many modes were successfully retrieved).

    Imagine you're developing an application that needs to offer users the ability to change display settings based on available resolutions and formats.

    In a multi-monitor setup, you may have multiple displays with different ports (e.g., one connected via HDMI and another via DisplayPort). 
    This function could help your application query the available display modes for each display and port combination.

    In applications where users need to configure their display settings dynamically (e.g., a video player, graphics application, or game), 
    this function can provide a list of compatible display modes based on the display's capabilities and user preferences.

    By specifying a pixel format (avxFormat fmt), this function can help narrow down the display modes to those that support specific color depths or encoding formats. 
    For example, you might want to list only modes that support 10-bit color depth or a specific video format (e.g., YUV 4:2:0).

    In applications that involve video playback, 3D rendering, or graphics-intensive tasks, 
    you may need to query and select display modes that best match the desired quality or performance (e.g., high resolution, high refresh rate).

    AfxQueryDisplayModes is a function that queries the available display modes for a particular display and port, supporting a specified pixel format. 
    It provides useful information for applications that need to dynamically configure or query display settings, such as games, media players, or multi-display systems.
*/

AUX afxUnit AfxQueryDisplayModes
(
    // This represents the display that you're querying. 
    // It could be an identifier for the display (screen or monitor) you are interested in, 
    // especially in multi-display setups where you may have more than one display or monitor connected.
    afxDisplay dpy,

    // This specifies the port of the display (e.g., HDMI, DisplayPort, VGA, etc.) that you are querying for supported display modes. 
    // Different ports might support different display resolutions, refresh rates, or other graphical settings.
    afxUnit port,

    // This refers to the pixel format or the color format (e.g., RGBA, YUV, etc.) you are interested in. 
    // The function will return the display modes that are compatible with this specific format. 
    // For example, you might request display modes that support a particular color depth or encoding format (like RGB888, YUV420, etc.).
    avxFormat fmt,

    // This is a count parameter, representing the number of modes that can be returned. 
    // The function will use this to ensure it doesn't return more modes than can be stored in the modes[] array. 
    // It may also be used as an input to specify the maximum number of display modes you want to retrieve.
    afxUnit cnt,

    // This is an array that will hold the display modes returned by the function. 
    // Each element in this array will contain information about a particular display mode, 
    // such as resolution (width, height), refresh rate, color depth, and other properties.
    afxDisplayMode modes[]
);

/*
    The AfxScanBackDisplayBuffer() function retrieves the contents of the back buffer for a specified display (dpy) and port (port). 
    It copies the image data from the back buffer to a surface (dout). 
    The "back buffer" typically refers to the off-screen rendering area where graphics are drawn before being displayed on the screen. 
    This is a standard practice in graphics rendering, especially in double-buffered systems, where the front buffer is what the user sees, 
    and the back buffer holds the next frame of graphics that will be shown.

    The function essentially "scans" the current state of the back buffer and provides a copy of it in the dout surface. 
    This can be useful for tasks such as:
     - Capturing the current screen state for processing (e.g., screenshot or video capture).
     - Analyzing the back buffer's content, such as for post-processing effects, diagnostics, or debugging graphical output.
     - Manipulating or exporting the rendered content before it is presented to the user.
    
    This function returns an error code that indicates the success or failure of the function.
    If no error is returned, the function successfully scanned the back buffer and copied the contents to the dout surface.
    If something went wrong (e.g., invalid parameters, failed access to the back buffer), the function will return an appropriate error code.

    If you need to take a screenshot or capture the current state of the display (for example, in a screen recording or for diagnostic purposes), 
    this function would be used to grab the back buffer and save or process the image.

    If you need to take a screenshot or capture the current state of the display (for example, in a screen recording or for diagnostic purposes), 
    this function would be used to grab the back buffer and save or process the image.

    In graphics applications (such as video editing software or games), you might want to process the back buffer contents before presenting it on-screen, 
    applying effects or filters, or analyzing the image data.

    During development or testing, you might want to inspect the current frame buffer contents to debug rendering issues, 
    verify graphical output, or track down performance problems.

    AfxScanBackDisplayBuffer() is a function that allows you to capture the current contents of the back buffer for a specific display and port. 
    It is useful for tasks such as screen capture, image processing, debugging, and graphics manipulation. 
    By copying the back buffer into a surface object (dout), you can work with the image data off-screen before it's shown on the user's display.
*/

AUX afxError AfxScanBackDisplayBuffer
(
    // This represents the display or graphical context from which you want to get the back buffer. 
    // It could refer to a specific screen, monitor, or display device. 
    // In systems with multiple displays, this would identify which display you're interacting with.
    afxDisplay dpy,

    // This refers to the specific output port (e.g., HDMI, DisplayPort) connected to the display. 
    // It can be used to target a specific graphical connection in a multi-port display system. 
    // This parameter allows you to specify the port for which you want to scan the back buffer.
    afxUnit port,

    // This parameter represents the surface object where the back buffer will be copied.
    // The surface is an object that holds image data (pixel data) from the back buffer, 
    // essentially capturing the content of the screen before it is displayed.
    // After the function is called, dout will contain a copy of the back buffer, allowing you to manipulate or process the image data further.
    afxSurface dout
);

/*
    The AfxQueryGammaControlCapabilites() method queries the gamma control capabilities of a specified display (dpy) and port (port). 
    The function fills the caps structure with the information about the gamma control features available on that display/port. 
    Gamma control involves adjustments to the display's color curves, which can affect the brightness and contrast of the image, 
    making the visual output more suitable for different environments or preferences.
    
    This function returns a code that indicates the success or failure of the function. 
    It may return no error, indicating that the function successfully retrieved the gamma control capabilities.
    Otherwise, other error codes may be returned if there was an issue (e.g., the display doesn't support gamma control, 
    or there was an invalid parameter), it will return an error code to indicate the problem.

    In applications that require advanced display calibration or custom visual settings (such as image editing software, video players, or games), 
    you might use this function to check if gamma adjustments are possible and then allow the user to fine-tune the display’s brightness and contrast.

    In applications that require advanced display calibration or custom visual settings (such as image editing software, video players, or games), 
    you might use this function to check if gamma adjustments are possible and then allow the user to fine-tune the display’s brightness and contrast.

    Some systems or applications (such as media players or virtual reality applications) might need to adjust the gamma settings dynamically based on the content being displayed or user preferences. 
    This function would allow you to query if those adjustments are possible on a specific display or port.

    In gaming or multimedia applications, where the visual quality is critical, this function could be used to query whether gamma control is supported, 
    allowing the game to adjust the display settings for better visibility in dark scenes, for example.

    AfxQueryGammaControlCapabilites allows you to query a display’s gamma control capabilities, 
    which is useful in applications that need to adjust or fine-tune the brightness and contrast of a display. 
    It's particularly relevant in environments where precise control over display color is required, such as gaming, 
    media playback, or professional graphics applications.

    WARNING: [WSHELL7] Win32: only supported while in full-screen mode.
    This function may only be available or functional when the display is in full-screen mode. 
    This could be due to the fact that gamma control adjustments are often more reliable and accurate when they are applied to the entire screen in a full-screen context, 
    rather than when a window is being rendered in windowed mode.
*/

AUX afxError AfxQueryGammaControlCapabilites
(
    // This parameter represents the display or graphical context you're working with. 
    // It identifies the specific display (screen or monitor) that you are querying for gamma control capabilities.
    afxDisplay dpy,

    // This refers to the specific port of the display (e.g., HDMI, DisplayPort) that you are querying. 
    // In systems with multiple ports or outputs, the port indicates which specific connection to check for gamma control support.
    afxUnit port,

    // This is a pointer to a structure (afxGammaCapabilites) that will be filled with the gamma control capabilities of the display/port. 
    // After the function call, this structure will contain information about the gamma control features supported by the display, 
    // such as the ability to adjust the gamma curve, the range of values that can be set, and any other gamma-related settings.
    afxGammaCapabilites* caps
);

/*
    The AfxDescribeGammaControl() function queries the gamma curve for the specified display (dpy) and port (port). 
    It fills the desc structure with information about the current gamma settings applied to the display. 
    A gamma curve is used to adjust the overall brightness and contrast of a display, mapping input color values to output color values. 
    The function would describe the gamma curve settings, such as the curve shape, the adjustments applied, and the specific gamma values used.

    This function returns an error code that indicates the success or failure of the function call. 
    It may return no error, meaning that the function successfully described the gamma curve.
    Or may reutrn other error codes, if there was an issue (e.g., gamma control is not supported, invalid parameters), 
    an error code is returned to describe the failure.

    In professional applications (e.g., photo or video editing, medical imaging, or graphics design), 
    it’s important to know the gamma curve applied to a display for accurate color reproduction. 
    This function would allow you to inspect the current gamma curve to ensure it meets the desired standard.

    Some applications allow users to adjust the display settings, including gamma correction, to fit specific lighting conditions or personal preferences. 
    By describing the current gamma curve, you can present this information to users and allow them to tweak the settings.

    In video games or media applications, gamma adjustments can be used to enhance visibility in darker scenes. 
    This function can be used to query the current gamma curve to determine whether adjustments are needed based on the content being displayed.

    For developers and system administrators, this function can be used to check if the system’s display settings are correctly configured. 
    It can also help identify if any unexpected gamma settings are applied, which might be causing issues with display output.

    Since the function is specifically noted to be supported in full-screen mode, 
    it’s likely useful for video players or other full-screen applications where you might want to adjust gamma settings dynamically based on the content being displayed.

    The fact that this function is only supported in full-screen mode suggests that gamma adjustments are more reliable and meaningful when applied to the entire screen,
    rather than to a specific windowed context. This could be due to how certain gamma-related effects may not function correctly in windowed mode,
    especially in systems that use double-buffering or other optimizations that differ between full-screen and windowed states.

    AfxDescribeGammaControl is a function that allows you to query and describe the gamma curve applied to a specific display and port. 
    This function is useful in situations where you need to inspect or adjust the gamma correction for color accuracy, 
    display calibration, or user preference, especially in full-screen environments where gamma adjustments are more consistent and accurate.
*/

AUX afxError AfxDescribeGammaControl
(
    // This represents the display or graphical context you are querying. 
    // It could refer to a specific screen, monitor, or graphical output device. 
    // If the system supports multiple displays, this parameter allows you to specify which display you're interested in.
    afxDisplay dpy,

    // This parameter likely refers to the specific port of the display (e.g., HDMI, DisplayPort) you're querying for gamma control. 
    // Different ports may support different gamma correction capabilities, so specifying the port allows you to query the settings for a particular display connection.
    afxUnit port,

    // This is a pointer to a structure (afxGammaCurve) that will hold the gamma curve description after the function call. 
    // The structure will likely contain details such as the current gamma curve applied to the display, 
    // its values, and any other relevant properties (e.g., gamma strength, adjustment ranges, etc.).
    afxGammaCurve* desc
);

/*
    The AfxControlGamma() function modifiesthe gamma curve on a specified display (dpy) and port (port) using the settings provided in the ctrl structure. 
    The gamma curve typically controls how input colors are mapped to output colors, affecting the display’s brightness, contrast, and overall visual appearance. 
    By adjusting the gamma curve, you can make subtle corrections to the image, such as making dark scenes brighter or enhancing the visibility of midtones.

    Gamma correction is often used to compensate for the non-linear behavior of display devices and can be essential for applications where accurate color reproduction is necessary.
    
    This function returns an error code indicating the success or failure of the operation. 
    It may return no error, meaninng that the gamma control was successfully applied.
    Or may return other error codes, if there was an issue (e.g., invalid parameters, unsupported operation), 
    it will return an error code to indicate the failure.

    This function is designed to work only in full-screen mode on Windows systems. 
    This could be because gamma control is typically more effective when applied to the entire screen, 
    especially when using hardware acceleration or certain display technologies. In windowed mode, 
    gamma adjustments may not work consistently due to how the windowing system and the display pipeline handle rendering.

    In applications where color accuracy is important (e.g., photo editing software, video production, or medical imaging), 
    AfxControlGamma allows you to adjust the gamma to ensure that colors are displayed correctly.

    In applications where color accuracy is important (e.g., photo editing software, video production, or medical imaging), 
    AfxControlGamma allows you to adjust the gamma to ensure that colors are displayed correctly.

    In gaming or video playback applications, this function could be used to enhance the visual experience by adjusting the gamma curve, 
    making dark scenes brighter, or fine-tuning the brightness and contrast for different content.

    If you are building a system that needs to adjust display settings based on environmental factors (such as ambient light) or user preferences, 
    AfxControlGamma could be used to dynamically adjust the gamma curve in real-time.

    As the function is supported only in full-screen mode, it is ideal for applications like games, media players, 
    or presentations that run in full-screen mode and require dynamic visual adjustments.

    This function is useful in diagnostic tools where you might need to adjust and test various gamma settings to ensure 
    the display is functioning correctly and to fine-tune the visual output.
*/

AUX afxError AfxControlGamma
(
    // This parameter represents the display (monitor or graphical output device) that you want to control. 
    // In a system with multiple displays, this specifies which display you want to modify the gamma settings for.
    afxDisplay dpy,

    // This parameter represents the display (monitor or graphical output device) that you want to control. 
    // In a system with multiple displays, this specifies which display you want to modify the gamma settings for.
    afxUnit port,

    // This parameter is a pointer to a structure (afxGammaCurve) containing the new gamma curve values that you want to apply to the display. 
    // The structure contains the gamma adjustments (such as the gamma strength and curve points) that will be used to modify the display’s color output. 
    // It defines how input color values should be transformed or adjusted by the gamma curve.
    afxGammaCurve const* ctrl
);

/*
    The AfxFindPhysicalAspectRatio() function determines the physical aspect ratio of a screen based on its width and height. 
    The function computes the ratio based on a predefined set of conditions.
    @screenWidth and @screenHeight are expected to be afxUnit types, likely representing screen dimensions.
    The function returns a afxReal64 value representing the calculated aspect ratio.

    The ratio is calculated by dividing the width by the height.
    The ratio is then compared to predefined thresholds (1.4, 1.6, etc) to assign it a specific aspect ratio (1.33, 1.56, or 1.78).

    Common Aspect Ratios
     - 4:3 (Standard definition CRT monitors and some old TVs)
     - 16:9 (Widescreen HDTV, most modern displays)
     - 16:10 (Popular for some computer monitors and laptops)
     - 5:4 (Older computer monitors, especially in office environments)
     - 21:9 (Ultrawide monitors, often used for gaming or multitasking)
     - 3:2 (Used by some laptops, like Microsoft Surface)
     - 32:9 (Super ultrawide displays)

    Explanation.
    The width-to-height ratio for 5:4 is 1.25. In the extended function, there is a check for div <= 1.5 which assigns the ratio 1.25 to it.
    The ultrawide monitors, such as 21:9, are typically in the range of 2.33 (21 divided by 9). There is a condition to check for values between 1.7 and 2.0 to accommodate this.
    For even wider screens (like 32:9), there is check for ratios greater than 2.5 too.

    This function performs an approximation and is intended to be used when the user does not know the aspect ratio of the desired device.
*/

AUX afxReal64 AfxFindPhysicalAspectRatio
(
    afxUnit screenWidth, 
    afxUnit screenHeight
);

#endif//AFX_DISPLAY_H
