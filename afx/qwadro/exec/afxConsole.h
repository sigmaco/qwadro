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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_CONSOLE_H
#define AFX_CONSOLE_H

#include "qwadro/base/afxUri.h"

// Add concept of environmental variables, such as $(name) -> value, to be used to form strings, paths, etc.

AFX_DEFINE_HANDLE(afxHandle);


AFX_DEFINE_STRUCT(xssConsoleMethodInfo)
// Describes one foreign method in a class.
{
    afxBool isStatic;
    afxString signature;
    /*LunaForeignMethodFn*/void* method;
};

AFX_DEFINE_STRUCT(xssConsoleClassInfo)
// Describes one class in a built-in module.
{
    afxString name;
    afxUnit methodCnt;
    xssConsoleMethodInfo const* methods;
};

// Describes one built-in module.
AFX_DEFINE_STRUCT(xssConsoleCrateInfo)
{
    // The name of the module.
    afxString name;

    // Pointer to the string containing the source code of the module. We use a
    // pointer here because the string variable itself is not a constant
    // expression so can't be used in the initializer below.
    afxString*source;

    afxUnit classCnt;
    xssConsoleClassInfo const* classes;
};

AFX_DEFINE_STRUCT(afxConsoleConfig)
{
    afxConsole parent;
    afxUnit crateCnt;
    xssConsoleCrateInfo const* crates;
};

AFX afxError AfxAcquireConsole
(
    afxConsoleConfig const* cfg, 
    afxConsole* console
);

////////////////////////////////////////////////////////////////////////////////

AFX void AfxGetScriptTime(afxConsole xss, afxReal64* ct, afxReal64* dt);

////////////////////////////////////////////////////////////////////////////////

AFX afxResult AfxRunScript(afxConsole xss, afxUri const* uri); // Enters the main event loop and waits until exit() is called. Returns the value that was passed to exit() (which is 0 if exit() is called via quit()).

AFX afxError AfxLoadScript(afxConsole xss, afxString const* scope, afxUri const* uri);

AFX afxError AfxInjectScript(afxConsole xss, afxString const* scope, afxString const* code);

#endif//AFX_CONSOLE_H
