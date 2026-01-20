/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_CONSOLE_C
#include "../exec/afxSystemDDK.h"
#include "../../xss/xss.h"
#include "qwadro/exec/afxConsole.h"

#ifdef _AFX_CORE_C
#ifdef _AFX_CONSOLE_C
AFX_OBJECT(afxConsole)
{
    xssVm               vm;
    afxUnit             argc;
    afxChar const**     argv;
    afxString           path;
    afxClock            startClock;
    afxClock            lastClock;

    afxUnit             scriptedEvCnt;
    afxString const*    scriptedEvNames;
    afxStringBase       scriptedEvNameDb;
    afxPool             scriptedEvHandlers;

    xssConsoleCrateInfo const* crates;

#if !0
    afxHandle           xssInitFn;
    afxHandle           xssQuitFn;
    afxHandle           xssStepFn;
    afxHandle           xssDrawFn;
    afxHandle           xssMainVar;
#endif
};
#endif//_AFX_CONSOLE_C
#endif//_AFX_CORE_C

xssVm xssCreateVm(LunaConfiguration const* config);
afxError xssDestroyVm(xssVm vm);

_AFX void GenAcqObj(xssVm vm)
{
    afxObject* ph = xssSetSlotNewForeign(vm, 0, 0, sizeof(afxObject));
    *ph = NIL;
}

_AFX void GenRelObj(void* data)
{
    AfxDisposeObjects(1, data);
    *(void**)data = NIL;
}

_AFX void XssPushError(xssVm vm, afxUnit slot, afxError err)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, err);
}

_AFX void XssPushBool(xssVm vm, afxUnit slot, afxBool val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotBool(vm, slot, val);
}

_AFX void XssPushReal(xssVm vm, afxUnit slot, afxReal val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AFX void XssPushReal64(xssVm vm, afxUnit slot, afxReal64 val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AFX void XssPushInt(xssVm vm, afxUnit slot, afxInt val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AFX void XssPushNat(xssVm vm, afxUnit slot, afxUnit val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AFX void XssPushSize(xssVm vm, afxUnit slot, afxSize val)
{
    xssEnsureSlots(vm, 1);
    xssSetSlotDouble(vm, slot, val);
}

_AFX afxBool XssPullBool(xssVm vm, afxUnit slot)
{
    return (afxBool)xssGetSlotDouble(vm, slot);
}

_AFX afxReal XssPullReal(xssVm vm, afxUnit slot)
{
    return (afxReal)xssGetSlotDouble(vm, slot);
}

_AFX afxReal64 XssPullReal64(xssVm vm, afxUnit slot)
{
    return (afxReal64)xssGetSlotDouble(vm, slot);
}

_AFX void XssPullReal3(xssVm vm, afxUnit slot, afxV3d v)
{
    v[0] = (afxReal)xssGetSlotDouble(vm, slot);
    v[1] = (afxReal)xssGetSlotDouble(vm, slot + 1);
    v[2] = (afxReal)xssGetSlotDouble(vm, slot + 2);
}

_AFX afxInt XssPullInt(xssVm vm, afxUnit slot)
{
    return (afxInt)xssGetSlotDouble(vm, slot);
}

_AFX afxUnit XssPullNat(xssVm vm, afxUnit slot)
{
    return (afxUnit)xssGetSlotDouble(vm, slot);
}

_AFX afxSize XssPullSize(xssVm vm, afxUnit slot)
{
    return (afxSize)xssGetSlotDouble(vm, slot);
}

_AFX afxHandle XssPullHandle(xssVm vm, afxUnit slot)
{
    return (afxHandle)xssGetSlotHandle(vm, slot);
}

_AFX void XssPushHandle(xssVm vm, afxUnit slot, afxHandle han)
{
    afxError err;
    xssSetSlotHandle(vm, slot, han);
}

_AFX void* XssPullInstance(xssVm vm, afxUnit slot)
{
    return *(void**)xssGetSlotForeign(vm, slot);
}

_AFX void XssPushInstance(xssVm vm, afxUnit slot, void* obj)
{
    afxError err;
    AFX_ASSERT(slot == 0);
    *(void**)xssSetSlotNewForeign(vm, 0, 0, sizeof(afxObject)) = obj;
}

_AFX void XssPullUri(xssVm vm, afxUnit slot, afxUri* uri)
{
    const char* s = xssGetSlotString(vm, slot);
    AfxMakeUri(uri, 0, s, 0);
}

_AFX void XssPullString(xssVm vm, afxUnit slot, afxString* s)
{
    const char* src = xssGetSlotString(vm, slot);
    AfxMakeString(s, 0, src, 0);
}

_AFX void XssUnhandle(xssVm vm, afxUnit cnt, afxHandle han[])
{
    for (afxUnit i = cnt; i-- > 0;)
    {
        xssReleaseHandle(vm, han[i]);
        han[i] = NIL;
    }
}

_AFX void XssHandleEachCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[])
{
    afxString2048 sig;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if (!(handles[i] = (afxHandle)xssMakeCallHandle(vm, sig.buf)))
        {
            XssUnhandle(vm, i, handles);
        }
    }
}

_AFX afxUnit XssHandleAnyCall(xssVm vm, afxUnit cnt, afxString const signatures[], afxHandle handles[])
{
    afxUnit rslt = 0;

    afxString2048 sig;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString2048(&sig, &signatures[i]);

        if ((handles[i] = (afxHandle)xssMakeCallHandle(vm, sig.buf)))
            ++rslt;
    }
    return rslt;
}

_AFX afxUnit XssHandleAnyVar(xssVm vm, afxString const* domain, afxUnit baseSlot, afxUnit cnt, afxString const name[], afxHandle handles[])
{
    afxUnit rslt = 0;

    afxString32 dom, nam;
    AfxMakeString32(&dom, domain);

    xssEnsureSlots(vm, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxMakeString32(&nam, &name[i]);

        if (xssHasVariable(vm, dom.buf, nam.buf))
        {
            xssGetVariable(vm, dom.buf, nam.buf, baseSlot + i);

            if (xssGetSlotType(vm, baseSlot + i) == LUNA_TYPE_UNKNOWN)
            {
                handles[i] = xssGetSlotHandle(vm, baseSlot + i);
                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxError XssCall(xssVm vm, afxHandle method)
{
    return xssCall(vm, method);
}

_AFX void AfxGetScriptTime(afxConsole xss, afxReal64* ct, afxReal64* dt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);
    afxClock currClock;
    AfxGetClock(&currClock);
    AFX_ASSERT(ct);
    *ct = AfxGetSecondsElapsed(&xss->startClock, &currClock);
    AFX_ASSERT(dt);
    *dt = AfxGetSecondsElapsed(&xss->lastClock, &currClock);
    xss->lastClock = currClock;
}

#if 0
_AFX afxBool _AfxXssEvent(afxConsole xss, afxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    switch (ev->id)
    {
    case afxEventId_RUN:
    {
        if (xss->xssInitFn)
        {
            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssCall(xss->vm, xss->xssInitFn);
        }
        break;
    }
    case afxEventId_QUIT:
    {
        if (xss->xssQuitFn)
        {
            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssCall(xss->vm, xss->xssQuitFn);
        }
        break;
    }
    default:
    {
        if (xss->xssStepFn)
        {
            afxReal64 ct, dt;
            AfxGetScriptTime(xss, &ct, &dt);

            XssPushHandle(xss->vm, 0, xss->xssMainVar);
            XssPushReal(xss->vm, 1, dt);
            XssCall(xss->vm, xss->xssStepFn);
        }
        break;
    }
    }

    return err;
}
#endif


// The maximum number of foreign methods a single class defines. Ideally, we
// would use variable-length arrays for each class in the table below, but
// C++98 doesn't have any easy syntax for nested global static data, so we
// just use worst-case fixed-size arrays instead.
//
// If you add a new method to the longest class below, make sure to bump this.
// Note that it also includes an extra slot for the sentinel value indicating
// the end of the list.
#define MAX_METHODS_PER_CLASS 14

// The maximum number of foreign classes a single built-in module defines.
//
// If you add a new class to the largest module below, make sure to bump this.
// Note that it also includes an extra slot for the sentinel value indicating
// the end of the list.
#define MAX_CLASSES_PER_MODULE 6

// Describes one foreign method in a class.
typedef struct
{
    bool isStatic;
    const char* signature;
    LunaForeignMethodFn method;
} MethodRegistry;

// Describes one class in a built-in module.
typedef struct
{
    const char* name;

    MethodRegistry methods[MAX_METHODS_PER_CLASS];
} ClassRegistry;

// Describes one built-in module.
typedef struct
{
    // The name of the module.
    const char* name;

    // Pointer to the string containing the source code of the module. We use a
    // pointer here because the string variable itself is not a constant
    // expression so can't be used in the initializer below.
    const char **source;

    ClassRegistry classes[MAX_CLASSES_PER_MODULE];
} ModuleRegistry;

// To locate foreign classes and modules, we build a big directory for them in
// static data. The nested collection initializer syntax gets pretty noisy, so
// define a couple of macros to make it easier.
#define SENTINEL_METHOD { false, NULL, NULL }
#define SENTINEL_CLASS { NULL, { SENTINEL_METHOD } }
#define SENTINEL_MODULE {NULL, NULL, { SENTINEL_CLASS } }

#define MODULE(name) { #name, &name##ModuleSource, {
#define END_MODULE SENTINEL_CLASS } },

#define CLASS(name) { #name, {
#define END_CLASS SENTINEL_METHOD } },

#define METHOD(signature, fn) { false, signature, fn },
#define STATIC_METHOD(signature, fn) { true, signature, fn },
#define ALLOCATE(fn) { true, "<allocate>", (LunaForeignMethodFn)fn },
#define FINALIZE(fn) { true, "<finalize>", (LunaForeignMethodFn)fn },

#if 0
// The array of built-in modules.
static ModuleRegistry modules[] =
{
#if 0
  MODULE(io)
#if 0
    CLASS(Directory)
      STATIC_METHOD("create_(_,_)", directoryCreate)
      STATIC_METHOD("delete_(_,_)", directoryDelete)
      STATIC_METHOD("list_(_,_)", directoryList)
    END_CLASS
#endif
#if 0
    CLASS(File)
      ALLOCATE(fileAllocate)
      FINALIZE(fileFinalize)
      STATIC_METHOD("delete_(_,_)", fileDelete)
      STATIC_METHOD("open_(_,_,_)", fileOpen)
      STATIC_METHOD("realPath_(_,_)", fileRealPath)
      STATIC_METHOD("sizePath_(_,_)", fileSizePath)
      METHOD("close_(_)", fileClose)
      METHOD("descriptor", fileDescriptor)
      METHOD("readBytes_(_,_,_)", fileReadBytes)
      METHOD("size_(_)", fileSize)
      METHOD("stat_(_)", fileStat)
      METHOD("writeBytes_(_,_,_)", fileWriteBytes)
    END_CLASS
#endif
#if 0
    CLASS(Stat)
      STATIC_METHOD("path_(_,_)", statPath)
      METHOD("blockCount", statBlockCount)
      METHOD("blockSize", statBlockSize)
      METHOD("device", statDevice)
      METHOD("group", statGroup)
      METHOD("inode", statInode)
      METHOD("linkCount", statLinkCount)
      METHOD("mode", statMode)
      METHOD("size", statSize)
      METHOD("specialDevice", statSpecialDevice)
      METHOD("user", statUser)
      METHOD("isDirectory", statIsDirectory)
      METHOD("isFile", statIsFile)
    END_CLASS
#endif
#if 0
    CLASS(Stdin)
      STATIC_METHOD("isRaw", stdinIsRaw)
      STATIC_METHOD("isRaw=(_)", stdinIsRawSet)
      STATIC_METHOD("isTerminal", stdinIsTerminal)
      STATIC_METHOD("readStart_()", stdinReadStart)
      STATIC_METHOD("readStop_()", stdinReadStop)
    END_CLASS
    CLASS(Stdout)
      STATIC_METHOD("flush()", stdoutFlush)
    END_CLASS
#endif
  END_MODULE
#endif
#if 0
  MODULE(os)
    CLASS(Platform)
      STATIC_METHOD("homePath", platformHomePath)
      STATIC_METHOD("isPosix", platformIsPosix)
      STATIC_METHOD("name", platformName)
    END_CLASS
    CLASS(Process)
      STATIC_METHOD("allArguments", processAllArguments)
      STATIC_METHOD("cwd", processCwd)
      STATIC_METHOD("pid", processPid)
      STATIC_METHOD("ppid", processPpid)
      STATIC_METHOD("version", processVersion)
    END_CLASS
  END_MODULE
#endif
#if 0
  MODULE(repl)
  END_MODULE
#endif
#if 0
  MODULE(scheduler)
    CLASS(Scheduler)
      STATIC_METHOD("captureMethods_()", schedulerCaptureMethods)
    END_CLASS
  END_MODULE
  MODULE(timer)
    CLASS(Timer)
      STATIC_METHOD("startTimer_(_,_)", timerStartTimer)
    END_CLASS
  END_MODULE
#endif
  SENTINEL_MODULE
};
#endif

#undef SENTINEL_METHOD
#undef SENTINEL_CLASS
#undef SENTINEL_MODULE
#undef MODULE
#undef END_MODULE
#undef CLASS
#undef END_CLASS
#undef METHOD
#undef STATIC_METHOD
#undef FINALIZER

// Looks for a built-in module with [name].
//
// Returns the BuildInModule for it or NULL if not found.
static xssConsoleCrateInfo const* findModule(xssVm vm, const char* name)
{
    afxConsole con = xssGetUserData(vm);
    
    for (int i = 0; con->crates[i].name.start != NULL; i++)
    {
        if (AfxStrcmp(name, con->crates[i].name.start) == 0) return &con->crates[i];
    }

    return NULL;
}

// Looks for a class with [name] in [module].
static xssConsoleClassInfo const* findClass(LunaVM* vm, xssConsoleCrateInfo const* module, const char* name)
{
    for (int i = 0; module->classes[i].name.start != NULL; i++)
    {
        if (AfxStrcmp(name, module->classes[i].name.start) == 0) return &module->classes[i];
    }

    return NULL;
}

// Looks for a method with [signature] in [clas].
static LunaForeignMethodFn findMethod(LunaVM* vm, xssConsoleClassInfo const* clas,
    bool isStatic, const char* signature)
{
    for (int i = 0; clas->methods[i].signature.start != NULL; i++)
    {
        xssConsoleMethodInfo* method = &clas->methods[i];
        if (isStatic == method->isStatic &&
            AfxStrcmp(signature, method->signature.start) == 0)
        {
            return method->method;
        }
    }

    return NULL;
}

void loadModuleComplete(LunaVM* vm, const char* name, struct LunaLoadModuleResult result)
{
    if (result.source == NULL) return;

    free((void*)result.source);
}

LunaLoadModuleResult loadBuiltInModule(LunaVM* vm, const char* name)
{
    LunaLoadModuleResult result = { 0 };
    xssConsoleCrateInfo const* module = findModule(vm, name);
    if (module == NULL) return result;

    size_t length = AfxStrlen(module->source->start);
    char* copy = (char*)malloc(length + 1);
    AfxCopy(copy, module->source->start, length + 1);

    result.onComplete = loadModuleComplete;
    result.source = copy;
    return result;
}

void onLunaLoadModuleCompleteFn(xssVm vm, const char* name, struct LunaLoadModuleResult result)
{
    AfxDisposeObjects(1, &result.userData);
}

// Attempts to read the source for [module] relative to the current root
// directory.
//
// Returns it if found, or NULL if the module could not be found. Exits if the
// module was found but could not be read.
static LunaLoadModuleResult loadModule(LunaVM* vm, const char* module)
{
    LunaLoadModuleResult result = { 0 };
    
    afxUri2048 uri;
    AfxMakeUri2048(&uri, NIL);
    afxError err = { 0 };
    //AfxMakeUri(&uri, nameString->value, 0);
    AfxFormatUri(&uri.uri, "../%.*s.xs", AfxStrlen(module), module);

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);
    AfxReloadFile(file, &uri.uri);

    if (!file)
    {
        //AfxThrowError();
        //return NIL;
    }
    else
    {
        afxString code;
        AfxMakeString(&code, 0, AfxGetStreamBuffer(file, 0), AfxGetStreamCapacity(file));
        result.source = code.start;
        result.userData = file;
    }

    result.onComplete = onLunaLoadModuleCompleteFn;

    // If we didn't find it, it may be a module built into the CLI or VM, so keep
    // going.
    if (result.source != NULL)
        return result;

    // Otherwise, see if it's a built-in module.
    return loadBuiltInModule(vm, module);
}

LunaForeignMethodFn bindBuiltInForeignMethod(
    LunaVM* vm, const char* moduleName, const char* className, bool isStatic,
    const char* signature)
{
    // TODO: Assert instead of return NULL?
    xssConsoleCrateInfo const* module = findModule(vm, moduleName);
    if (module == NULL) return NULL;

    xssConsoleClassInfo const* clas = findClass(vm, module, className);
    if (clas == NULL) return NULL;

    return findMethod(vm, clas, isStatic, signature);
}

LunaForeignClassMethods bindBuiltInForeignClass(
    LunaVM* vm, const char* moduleName, const char* className)
{
    LunaForeignClassMethods methods = { NULL, NULL };

    xssConsoleCrateInfo const* module = findModule(vm, moduleName);
    if (module == NULL) return methods;

    xssConsoleClassInfo const* clas = findClass(vm, module, className);
    if (clas == NULL) return methods;

    methods.allocate = findMethod(vm, clas, true, "<allocate>");
    methods.finalize = (LunaFinalizerFn)findMethod(vm, clas, true, "<finalize>");

    return methods;
}

// Binds foreign methods declared in either built in modules, or the injected
// API test modules.
static LunaForeignMethodFn bindForeignMethod(LunaVM* vm, const char* module,
    const char* className, bool isStatic, const char* signature)
{
    LunaForeignMethodFn method = bindBuiltInForeignMethod(vm, module, className,
        isStatic, signature);
    if (method != NULL) return method;
#if 0
    if (bindMethodFn != NULL)
    {
        return bindMethodFn(vm, module, className, isStatic, signature);
    }
#endif
    return NULL;
}

// Binds foreign classes declared in either built in modules, or the injected
// API test modules.
static LunaForeignClassMethods bindForeignClass(
    LunaVM* vm, const char* module, const char* className)
{
    LunaForeignClassMethods methods = bindBuiltInForeignClass(vm, module,
        className);
    if (methods.allocate != NULL) return methods;
#if 0
    if (bindClassFn != NULL)
    {
        return bindClassFn(vm, module, className);
    }
#endif
    return methods;
}

void VM_error(xssVm env, LunaErrorType type, const char* module,
    int line, const char* message) {

    if (type == LUNA_ERROR_COMPILE)
    {
        AfxReportError("ERR/CP: %s,%u --- %s", module, line, message);
    }
    else if (type == LUNA_ERROR_RUNTIME)
    {
        AfxReportError("ERR/RT: %s,%u --- %s", module, line, message);
    }
    else if (type == LUNA_ERROR_STACK_TRACE)
    {
        AfxReportError("ERR/ST: %s,%u --- %s", module, line, message);
    }
}

_AFX afxError _AuxXssDtorCb(afxConsole xss)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    if (xss->vm)
        xssDestroyVm(xss->vm);

    return err;
}

_AFX afxError _AuxXssCtorCb(afxConsole xss, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    afxSystem sys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxConsoleConfig const* cfg = ((afxConsoleConfig const*)args[1]) + invokeNo;
    
    AfxResetStrings(1, &xss->path);

    xss->argc = 0;
    xss->argv = NIL;

    LunaConfiguration vmCfg = { 0 };
    xssInitConfiguration(&vmCfg);
    vmCfg.parent = NIL;// sysVm;
    vmCfg.errorFn = VM_error;
    vmCfg.bindForeignMethodFn = bindForeignMethod;
    vmCfg.bindForeignClassFn = bindForeignClass;
    //vmCfg.resolveModuleFn = resolveModule;
    vmCfg.loadModuleFn = loadModule;
    vmCfg.initialHeapSize = 512;
    vmCfg.minHeapSize = 512;
    vmCfg.userData = xss;
    xss->vm = xssCreateVm(&vmCfg);

    AfxGetClock(&xss->startClock);
    xss->lastClock = xss->startClock;

    xss->crates = cfg->crates;

    return err;
}

_AFX afxClassConfig const _AFX_XSS_CLASS_CONFIG =
{
    .fcc = afxFcc_XSS,
    .name = "Console",
    .desc = "Cross-System Scripting Console",
    .fixedSiz = sizeof(AFX_OBJECT(afxConsole)),
    .ctor = (void*)_AuxXssCtorCb,
    .dtor = (void*)_AuxXssDtorCb,
    //.event = (void*)_AfxXssEvent
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireConsole(afxConsoleConfig const* cfg, afxConsole* console)
{
    afxError err = { 0 };

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxClass* cls = (afxClass*)_AfxSysGetXssClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_XSS);

    afxConsole xss;
    if (AfxAcquireObjects(cls, 1, (void*)&xss, ((void const*[]) { sys, cfg })))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    AFX_ASSERT(console);
    *console = xss;

    return err;
}

#if 0
_AFX afxError AfxAcquireScript(afxString const* domain, afxString const* code, afxConsole* script)
{
    afxError err = { 0 };

    afxShell sh;
    AfxGetShell(&sh);
    AfxAssertObjects(1, sh, afxFcc_USYS);
    afxClass* cls = _AuxEnvGetXssClass();
    AFX_ASSERT_CLASS(cls, afxFcc_XSS);

    if (AfxAcquireObjects(cls, 1, (void*)script, ((void const*[]) { sh, domain, code })))
        AfxThrowError();

    AfxAssertObjects(1, script, afxFcc_XSS);
    afxResult r;

    afxResult xRslt;

    xRslt = AfxInjectScript(domain, code);
    AFX_ASSERT(!xRslt);
#if 0
    afxHandle fn[4];
    afxString fns[4];
    AfxMakeString(&fns[0], "init()", 0);
    AfxMakeString(&fns[1], "quit()", 0);
    AfxMakeString(&fns[2], "step(_)", 0);
    AfxMakeString(&fns[3], "draw(_,_)", 0);
    XssHandleAnyCall(sh->vm, 4, fns, fn);

    LunaHandle* initMethod = NULL;
    xssEnsureSlots(sh->vm, 3);
    initMethod = xssMakeCallHandle(sh->vm, "init()");

    if (!xssHasVariable(sh->vm, domain->start, "Game"))
    {
        AfxThrowError();
    }

    xssGetVariable(sh->vm, domain->start, "Game", 0);

    if (xssGetSlotType(sh->vm, 0) != LUNA_TYPE_UNKNOWN)
    {
        AfxThrowError();
    }

    LunaHandle* gameClass = xssGetSlotHandle(sh->vm, 0);
    LunaHandle* updateMethod = xssMakeCallHandle(sh->vm, "update()");
    LunaHandle* drawMethod = xssMakeCallHandle(sh->vm, "draw(_)");

    xssSetSlotHandle(sh->vm, 0, gameClass);
    xRslt = xssCall(sh->vm, initMethod);

    xssReleaseHandle(sh->vm, initMethod);
    initMethod = NULL;
#endif
    return err;
}
#endif

_AFX afxError AfxLoadScript(afxConsole xss, afxString const* scope, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT(uri);

    afxString scope2;

    if (AfxIsStringEmpty(scope))
        AfxGetUriTargetString(uri, &scope2);
    else
        scope2 = *scope;

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);
    AfxReloadFile(file, uri);

    if (!file) AfxThrowError();
    else
    {
        afxString code;
        AfxMakeString(&code, 0, AfxGetStreamBuffer(file, 0), AfxGetStreamCapacity(file));

        afxResult xRslt;
        xRslt = AfxInjectScript(xss, &scope2, &code);
        AFX_ASSERT(!xRslt);

        if (!xRslt)
        {
#if 0
            afxShell sh;
            AfxGetShell(&sh);

            afxHandle fn[4];
            afxString fns[4];
            AfxMakeString(&fns[0], "init()", 0);
            AfxMakeString(&fns[1], "quit()", 0);
            AfxMakeString(&fns[2], "step(_)", 0);
            AfxMakeString(&fns[3], "draw(_,_)", 0);
            XssHandleAnyCall(sh->vm, 4, fns, fn);

            afxHandle var;
            afxString vars;
            AfxMakeString(&vars, "Game", 0);
            XssHandleAnyVar(sh->vm, &domain, 0, 1, &vars, &var);

            sh->xssInitFn = fn[0];
            sh->xssQuitFn = fn[1];
            sh->xssStepFn = fn[2];
            sh->xssDrawFn = fn[3];
            sh->xssMainVar = var;

            XssPushHandle(sh->vm, 0, var);
            XssCall(sh->vm, fn[0]); // init
            XssUnhandle(sh->vm, 1, &fn[0]);
#endif
        }

        AfxDisposeObjects(1, &file);
    }
    return err;
}

#if 0
_AFX afxResult AfxRunScript(afxConsole xss, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_XSS, 1, &xss);

    if (uri)
    {
        afxString domain;
        AfxGetUriTargetString(uri, &domain);

        if (AfxLoadScript(&domain, uri)) AfxThrowError();
        else
        {
            afxHandle fn[4];
            afxString fns[4];
            AfxMakeString(&fns[0], "init()", 0);
            AfxMakeString(&fns[1], "quit()", 0);
            AfxMakeString(&fns[2], "step(_)", 0);
            AfxMakeString(&fns[3], "draw(_,_)", 0);
            XssHandleAnyCall(xss->vm, 4, fns, fn);

            afxHandle var;
            afxString vars;
            AfxMakeString(&vars, "Game", 0);
            XssHandleAnyVar(xss->vm, &domain, 0, 1, &vars, &var);

            xss->xssInitFn = fn[0];
            xss->xssQuitFn = fn[1];
            xss->xssStepFn = fn[2];
            xss->xssDrawFn = fn[3];
            xss->xssMainVar = var;

            XssPushHandle(xss->vm, 0, var);
            XssCall(xss->vm, fn[0]); // init
            XssUnhandle(xss->vm, 1, &fn[0]);
        }
    }

    AfxGetClock(&xss->startClock);
    xss->lastClock = xss->startClock;
    return 0;
}
#endif

_AFX afxResult AfxInjectScript(afxConsole xss, afxString const* scope, afxString const* code)
{
    afxError err = { 0 };

    afxString32 scope2;
    AfxMakeString32(&scope2, scope);

    void* vm = xss->vm;

    AfxReportf(0, NIL, code->start);
    afxResult xRslt = xssInterpret(vm, scope2.buf, code->start);

    return xRslt;
}
