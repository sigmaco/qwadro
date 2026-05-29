/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This software is part of Advanced MMUX Extensions.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

/*
    A hypervisor, also known as a virtual machine monitor or virtualizer, is a type of computer software, firmware or hardware that 
    creates and runs virtual machines. A computer on which a hypervisor runs one or more virtual machines is called a host machine 
    or virtualization server, and each virtual machine is called a guest machine. The hypervisor presents the guest operating systems 
    with a virtual operating platform and manages the execution of the guest operating systems.

    A virtual desktop refers to a desktop OS, such as Windows 10, that runs on top of an enterprise hypervisor. Users employ a virtual 
    desktop infrastructure (VDI) to access virtual desktops through thin clients. A remote desktop protocol transmits screen images and 
    keyboard and.

    When people talk about "desktop environments", they often mix them up with several related but distinct layers of the graphical stack.
    A desktop environment is the full, integrated graphical user experience.
    A desktop environment usually includes:
     - A window manager
     - A panel/taskbar
     - A launcher/menu
     - A settings center
     - File manager
     - Theme system
     - Notification service
     - Background services (like portals, power manager…)

    It's the high-level layer.

    A window manager controls window placement, focus, tiling, decorations, and resize behavior.
    A DE includes a WM, but you can also run a WM alone without a DE.

    The display server (or display protocol layer) is the backend that speaks to your hardware and coordinates screens and input.
    On Wayland, the compositor is also the display server.
    A compositor draws windows into an off-screen buffer and blends them onto the screen.
    On Wayland, the compositor = the display server = usually the WM too.

    Graphical toolkits provide GUI widgets (buttons, menus) and applications use them.
    A desktop environment normally standardizes on one (GNOME uses GTK, KDE uses Qt).

    Some environments are "shells" but not full desktop environments. It is also refered to as application environment (or shell layers).
    These sit between the WM and the full desktop environment concept.

    Login/session environments are responsible for user login and starting the graphical session.
    These choose which desktop environment or WM to start.

    You can run environments in different session modes (or session types).
    Wayland session, X11 session, Mir session (historical Ubuntu), fbdev/direct-to-framebuffer (no WM) to name a few of them.
*/

#ifndef AUX_ENVIRONMENT_H
#define AUX_ENVIRONMENT_H

#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/hid/afxKeyboard.h"
#include "qwadro/ux/afxShell.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/mix/afxMixSystem.h"

#define AFX_MAX_USERS_PER_ENVIRONMENT (2)

typedef enum afxClipboardFlag
{
    afxClipboardFlag_PROTECED = AFX_BITMASK(0),
} afxClipboardFlags;

typedef enum afxFocusFlag
{
    afxFocusFlag_FLASH = AFX_BITMASK(0),
} afxFocusFlags;

typedef enum afxEnvironmentType
{
    // The environment is a GUI that represents a workspace on a computer screen, 
    // allowing users to interact with files, applications, and system functions through visual elements like icons and windows. 
    // It simplifies navigation and task management, making it easier for users to access and organize their digital resources.
    afxEnvironmentType_DESKTOP,
    // The environment is a computer assuming to have exclusive control over the display.
    afxEnvironmentType_TERMINAL,
    // The environment is a head-mounted device with visor offering or assisting virtual or augmented reality.
    afxEnvironmentType_VR,
    afxEnvironmentType_AR,
} afxEnvironmentType;

typedef enum afxSessionState
{
    afxSessionState_NONE,
    afxSessionState_CONNECTING,
    afxSessionState_SOCKET_CONNECTED,
    afxSessionState_BANNER_RECEIVED,
    afxSessionState_INITIAL_KEX,
    afxSessionState_KEXINIT_RECEIVED,
    afxSessionState_DH,
    afxSessionState_AUTHENTICATING,
    afxSessionState_AUTHENTICATED,
    afxSessionState_ERROR,
    afxSessionState_DISCONNECTED
} afxSessionState;

typedef enum afxAuthMethod
{
    afxAuthMethod_NONE, // Try to authenticate through the "none" method.
    afxAuthMethod_PASSWORD, // Try to authenticate by password.
    afxAuthMethod_PRIVKEY, // Authenticate with public/private key or certificate.
    afxAuthMethod_PASSPHRASE, // Tries to automatically authenticate with public key and "none".
    afxAuthMethod_PUBKEY, // Try to authenticate with the given public key.

    afxAuthMethod_TOTAL
} afxAuthMethod;

typedef enum afxSeatFlag
{
    afxSeatFlag_RESERVED = AFX_BITMASK(31)
} afxSeatFlags;

typedef enum afxSeatCap
{
    afxSeatCap_KEYBOARD,
    afxSeatCap_MOUSE,
    afxSeatCap_GAMEPAD,
} afxSeatCaps;

AFX_DEFINE_STRUCT(afxSeatConfig)
{
    afxString u;
    afxFlags flags;
    afxUnit portIdx;
    afxUnit queueCnt;
    afxReal const* queuePriority;

    // SSH
    // Note from libssh
    // Most server implementations do not permit changing the username during authentication.
    // The username should only be set with ssh_options_set() only before you connect to the server.
    afxUri host;
    afxAuthMethod method;
    afxString username;
    afxString credential;
};

AFX_DEFINE_STRUCT(afxEnvironmentConfig)
// The environment-wide settings and parameters.
{
    afxUnit verMajor;
    afxUnit verMinor;

    // The friendly name for this environment.
    afxString name;

    // The features to be enabled for this environment.
    afxShellFeatures features;
    // The number of environment extensions to be enabled.
    afxUnit extCnt;
    // An array of Qwadro strings containing the names of extensions to enable for the environment.
    afxString const* exts;
    // The draw system attached to the environment.

    afxDrawSystem dsys;
    // The mix system attached to the environment.

    afxMixSystem msys;

    // The number of seats.
    afxUnit seatCnt;
    // An array of configurations for each seat.
    afxSeatConfig seats[AFX_MAX_USERS_PER_ENVIRONMENT];

    // User-defined data attached to the environment.
    void* udd;
    // Debugging string attached to the environment.
    afxString tag;

    afxUnit ddevId;
    afxUnit vduIdx;
    afxUnit sdevId;
    afxUnit soutIdx;
};

AUX afxError AfxConfigureEnvironment
(
    afxModule auxIcd, 

    afxEnvironmentConfig* cfg
);

AUX afxError AfxEstablishEnvironment
(
    afxModule auxIcd,

    afxEnvironmentConfig const* cfg, 

    afxEnvironment* environment
);

AUX afxBool AfxGetActiveEnvironment
(
    afxEnvironment* environment
);

////////////////////////////////////////////////////////////////////////////////

AUX afxUnit AfxGetEnvironmentId
(
    // can be NIL
    afxEnvironment env
);

AUX afxBool AfxGetConsole
(
    afxEnvironment env, 

    afxConsole* console
);

AUX afxBool AfxGetEnvironmentAvx
(
    afxEnvironment env,

    afxDrawSystem* system,

    afxSurface* surface
);

AUX afxBool AfxGetEnvironmentAmx
(
    afxEnvironment env,

    afxMixSystem* system, 

    afxSink* sink
);

AUX afxError AfxBeginFrame(afxEnvironment env);
AUX afxError AfxWaitFrame(afxEnvironment env);
AUX afxError AfxEndFrame(afxEnvironment env);

AUX afxError AfxReconnectKeyboard(afxEnvironment env, afxUnit portIdx, afxUnit hidNo);

AUX afxTime AfxDoUx(afxFlags flags, afxUnit64 timeout);

AUX afxError AfxTakeFullscreen(afxWindow wnd, afxBool fullscreen);

AUX afxBool AfxGetCursorPlacement(afxUnit seat, afxWindow wnd, afxBool onFrame, afxRect* rc);

/*
    The AfxHasClipboardContent() function checks whether there is any content in the clipboard associated with a 
    specific context (seat) and specific clipboard "slot" (slot). The flags parameter might be used to control 
    how the content is checked (e.g., specific content types or whether the check should be case-sensitive or type-specific).

    This function returns a boolean type, meaning it will return either TRUE or FALSE.
    TRUE if the clipboard content is present in the specified slot for the given seat.
    FALSE if the clipboard is empty, or no content exists for the given seat and slot.

    If you're developing software where multiple users can interact with a shared clipboard (e.g., collaborative applications), 
    this function could allow you to check the clipboard content for each user or session.

    If your system supports multiple clipboard slots (e.g., one for text, one for images, etc.), 
    you could check whether content exists in a specific slot for a given user or session.

    In some advanced applications, you may want to verify if the clipboard contains specific types of data before 
    performing operations like pasting, processing, or transferring the content.
*/

AUX afxBool AfxHasClipboardContent
(
    // This represents the "seat" or user context from which the request is made.
    afxUnit seat, 
    
    // This could represent a specific clipboard "slot" or "buffer" that the function is checking. 
    // Some systems might allow multiple slots or buffers for clipboard content 
    // (e.g., different types of data, like text, images, etc.), so this could specify which slot to check for content.
    afxUnit slot, 

    // Flags are often used to modify the behavior of the function. 
    // These could provide additional options or constraints for checking clipboard content, 
    // such as whether to only check for specific content types (e.g., text, image), 
    // or other behaviors like forcing an immediate check, filtering, or checking with extra constraints.
    afxClipboardFlags flags
);

/*
    The AfxGetClipboardContent() function is a useful function for retrieving clipboard content in complex systems where you need to handle multiple users, 
    multiple clipboard slots, or different types of clipboard data. Its flexibility with the seat, slot, 
    and flags parameters provides a lot of control over how and what clipboard data is accessed.

    AfxGetClipboardContent attempts to fetch content from the clipboard for a specific context (seat) and a specific clipboard slot (slot). 
    It retrieves the content (likely in text format) and places it into the provided string buffer (buf).

    Imagine an application where multiple users can interact with their own clipboards, 
    or where different types of clipboard data are stored in separate slots.

    In an application that allows multiple users to interact with clipboards (e.g., a shared virtual workspace), 
    this function could be used to get the clipboard content for a specific user (seat).

    If the clipboard supports multiple slots (such as one for text, one for images, etc.), 
    you could use this function to retrieve the content from a specific slot. 
    This makes it possible to manage different types of content separately.

    The flags parameter could allow you to request the clipboard content in different formats (plain text, formatted text, or other data types) or with special conditions. 
    For example, you might use flags to ensure that the clipboard content is returned in a specific encoding or format.

    If the application deals with complex data transfers (such as transferring data between different components of the system, 
    or between systems), this function could be used to pull content from the clipboard into the application for further processing.
*/

AUX afxUnit AfxGetClipboardContent
(
    // Represents the "seat" or context in which the request is being made.
    afxUnit seat, 

    // Refers to a specific clipboard "slot" or "buffer" that you want to get the content from. 
    // Systems with multiple clipboard slots may store different types of data (text, images, etc.) in each slot, 
    // so this parameter identifies which slot's content you're interested in.
    afxUnit slot, 

    // Flags are used to modify the behavior of a function. 
    // In this case, the flags might allow you to specify particular conditions under which the clipboard content should be fetched.
    afxClipboardFlags flags,

    // This is a pointer to a string buffer that will be populated with the clipboard content. 
    // After the function is called, buf will hold the clipboard content (likely in string format, assuming this is text content). 
    // This would be where the content is copied for further processing or use.
    afxString* buf
);

AUX afxError AfxSetClipboardContent
(
    // This represents the "seat" or context from which the request is being made.
    afxUnit seat, 

    // This refers to a specific clipboard "slot" or "buffer" that will be updated.Just like with AfxGetClipboardContent, 
    // systems with multiple clipboard slots might store different types of data(e.g., text in one slot, images in another).
    // This parameter indicates which slot is being targeted for the content change.
    afxUnit slot, 

    // Flags are used to provide additional options or behaviors when setting the clipboard content.
    afxClipboardFlags flags,

    // This is a pointer to the string data (likely text) that will be placed into the clipboard. 
    // After the function call, the clipboard slot specified by slot will contain this text data. 
    // This might be the string that will be copied to the clipboard for further use, such as pasting.
    afxString const* text
);

/*
    The AfxFocusWindow() function sets or changes the focus to a specific afxWindow within the active environment. 
    By calling AfxFocusWindow, the application is requesting that the specified window should become the active, 
    focused window; meaning it gets keyboard input, is highlighted in some way, and is at the forefront of the screen.
*/

AUX afxError AfxFocusWindow
(
    // This represents the "seat" or context of the user requesting the focus, 
    // which might be the user context that is interacting with the window.
    afxUnit seat, 

    // Refers to the afxWindow object that application want to focus on. 
    // This is the window that the function will bring to the foreground or give input focus to.
    afxWindow wnd, 
    
    // Flags are used to provide additional behavior or options for the function. 
    // These flags modify how the focus is applied, like whether it should be forced, 
    // whether there are visual effects, or whether certain windows are excluded.
    afxFocusFlags flags
);

/*
    The AfxGetFocusedWindow() function checks which window currently has the focus and return that window object. 
    The function updates the <window> placeholder to point to the window that is currently in focus 
    (i.e., the window receiving keyboard input and being visually active).

    This function will return a boolean value (true or false), indicating whether the focus retrieval was successful.
    TRUE if the focus was successfully retrieved and the window parameter has been set to the focused window.
    FALSE if there was no focused window or an error occurred.
*/

AUX afxBool AfxGetFocusedWindow
(
    // This refers to the "seat" or user context in which the request is made. 
    afxUnit seat, 

    // This is a pointer to a placeholder for an afxWindow object. 
    // After the function is called, the <window> parameter will hold the afxWindow object that is currently focused. 
    // The function will update this pointer to point to the focused window.
    afxWindow* window
);

AUX afxError AfxDrawBackgroundEXT(afxDrawContext dctx, afxFlags flags);

#endif//AUX_ENVIRONMENT_H
