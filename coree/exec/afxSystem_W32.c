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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
//#   include <combaseapi.h>
#   include <process.h>
#   include <sys/timeb.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#include "afxSystemDDK.h"

afxUnit const sizeOfSys = sizeof(AFX_OBJ(afxSystem));
afxUnit const sizeOfSysAligned = AFX_ALIGN_SIZE(sizeof(AFX_OBJ(afxSystem)), AFX_SIMD_ALIGNMENT);
AFX_STATIC_ASSERT(AFX_ALIGN_SIZE(sizeof(AFX_OBJ(afxSystem)), AFX_SIMD_ALIGNMENT) >= sizeof(AFX_OBJ(afxSystem)), "");
AFX_STATIC_ASSERT(sizeof(AFX_OBJECT(afxSystem)) > sizeof(afxSystem), "");

_AFX afxBool sysReady = FALSE;
#if !0
_AFX AFX_ALIGNED(16) afxByte theSysData[  AFX_ALIGN_SIZE(sizeof(afxObjectBase), AFX_SIMD_ALIGNMENT) + 
                                        AFX_ALIGN_SIZE(sizeof(AFX_OBJ(afxSystem)), AFX_SIMD_ALIGNMENT)] = { 0 };
_AFX afxSystem TheSystem = (void*)&theSysData;
AFX_STATIC_ASSERT(sizeof(TheSystem[0]) > sizeof(void*), "");
AFX_STATIC_ASSERT(sizeof(theSysData) >= (sizeof(afxObjectBase) + sizeof(TheSystem[0])), "");
#else
_AFX afxSystem TheSystem = NIL;
#endif
extern afxReal64 const renderwareUnitsPerMeter;

AFX afxError _AfxInitMmu(afxThread thr);
AFX afxError _AfxDeinitMmu(afxThread thr);

// READ ONLY METHODS //////////////////////////////////////////////////////////

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

void Win32GetWindowsVersion(afxUnit* verMajor, afxUnit* verMinor, afxUnit* verBuild, afxUnit* platId)
{
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod)
    {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != NULL)
        {
            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (fxPtr(&rovi) == 0)
            {
                printf("Windows Version: %lu.%lu (Build %lu)\n", rovi.dwMajorVersion, rovi.dwMinorVersion, rovi.dwBuildNumber);
                *verMajor = rovi.dwMajorVersion;
                *verMinor = rovi.dwMinorVersion;
                *verBuild = rovi.dwBuildNumber;
                *platId = rovi.dwPlatformId;
            }
            else
            {
                printf("Failed to get version info.\n");
            }
        }
    }
}

afxString Win32GetWindowsName(DWORD major, DWORD minor)
{
    if (major == 10 && minor == 0) return AFX_STRING("Windows 10 or 11");
    if (major == 6 && minor == 3) return AFX_STRING("Windows 8.1");
    if (major == 6 && minor == 2) return AFX_STRING("Windows 8");
    if (major == 6 && minor == 1) return AFX_STRING("Windows 7");
    if (major == 6 && minor == 0) return AFX_STRING("Windows Vista");
    if (major == 5 && minor == 1) return AFX_STRING("Windows XP");
    return AFX_STRING("Unknown Windows version");
}

_AFX afxResult PrimeThreadProc(afxThread thr, afxEvent* ev)
{
    return 0;
}

#if 0
_AFX afxBool SshProc(afxShell ssh, MSG const* msg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SSH, 1, &ssh);

    AfxCallDevice((afxDevice)ssh, 3, msg);

    return TRUE;
}

_AFX afxTime AfxDoSystemThreading(afxTime timeout)
{
    afxError err = { 0 };

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxTime first, last, dt;
    AfxGetTime(&first);

    do
    {
        SHORT escSt = 0, lastEscSt = 0;

        MSG msg;
        afxResult msgCnt = 0;
        while (PeekMessageA(&msg, NIL, 0, 0, PM_REMOVE | PM_NOYIELD))
        {
            ++msgCnt;

            if (msg.message == WM_QUIT) // PostQuitMessage()
            {
                if (AfxIsPrimeThread())
                    AfxRequestShutdown(0);
            }
            else if (msg.message == WM_USER)
            {
                void(*event)(void* udd) = (void*)msg.wParam;

                if (event)
                    event((void*)msg.lParam);
            }
            else
            {
                if (!sys->aux.disabled)
                {
                    if (sys->aux.ioctl)
                        sys->aux.ioctl(sys, sys->aux.e2mmuxDll, 0, &timeout);

                    AfxInvokeObjects(&sys->aux.sshCls, 0, 0, (void*)SshProc, &msg);
                }

                if (AfxIsPrimeThread())
                {
                    
                    lastEscSt = escSt;
                    escSt = (GetKeyState(VK_ESCAPE) & 0x8000);

                    if (GetKeyState(VK_F1))
                    {
                        AfxReportHint("User input requested support.");
                        AfxReportHint("Get help at https://sigmaco.org/");
                        system("start https://sigmaco.org");
                    }
                    else if (GetKeyState(VK_F4))
                    {
                        //AfxRequestThreadInterruption(thr);
                        AfxReportWarn("User input requested application break.");
                        AfxRequestShutdown(0);
                    }
                    else if (GetKeyState(VK_F5))
                    {
                        AfxReportWarn("User input requested application reboot.");
                        //_AfxInterruptionAllApplications();
                        //opcode = AFX_OPCODE_REBOOT;
                    }
#if 0
                    else if (lastEscSt && !(escSt))
                    {
                        {
                            afxString cur;
                            AfxMakeString(&cur, 0, "\n$ ", 0);
                            afxString2048 buf;
                            AfxMakeString2048(&buf, NIL);
                            AfxPrompt(&cur, &buf.s);
                            AfxPrint(0, buf.buf);
                            AfxPrint(0, "\n");
                        }
                    }
#endif
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
        break; // <------
    } while (timeout > (dt = (AfxGetTime(&last) - first)));
    return dt;
}
#endif

_AFX void AfxStdAssertHookCallback(char const* exp, char const* file, int line)
{
    assert(exp);
    assert(file);
    assert(line);
    AfxReportf(8, NIL, "\n %s:%i\n\t     %s", file, line, exp);
}

_AFX afxError MountHostVolumes()
{
    afxError err = { 0 };

    afxUri32 urib2;
    AfxMakeUri32(&urib2, NIL);

    char buf[256];
    struct stat st;

    for (int i = 0; i < 26; i++)
    {
        sprintf(buf, "%c:\\", 'A' + i);

        if (0 == stat(buf, &st))
        {
            AfxFormatUri(&urib2.uri, "%c:\\", 'A' + i);
            
            afxFileFlags ioFlags = NIL;

            if ((st.st_mode & _S_IREAD) == _S_IREAD)
                ioFlags |= afxFileFlag_R;

            if ((st.st_mode & _S_IWRITE) == _S_IWRITE)
                ioFlags |= afxFileFlag_W;

            if ((st.st_mode & _S_IEXEC) == _S_IEXEC)
                ioFlags |= afxFileFlag_X;

            AfxMountStorageUnit('a' + i, &urib2.uri, ioFlags);
        }
    }

    int a = 0;
    return err;
}

_AFX afxError _AfxSysMountDefaultFileStorages(afxSystem sys)
{
    afxError err = { 0 };

    afxUri point;
    afxUri2048 location;
    AfxMakeUri2048(&location, NIL);

    if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RX)) AfxThrowError();
    else
    {
        AfxFormatUri(&location.uri, "%.*s", AfxPushString(_AfxGetSystemDirectoryString(NIL)));

        if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RX)) AfxThrowError();
        else
        {
#ifdef AFX_ON_X86_64
#   ifdef AFX_ON_WINDOWS
            AfxMakeUri(&location.uri, 0, "w64", 0);

            if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location.uri, 0, "w64d", 0);

                if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
            AfxMakeUri(&location.uri, 0, "C:\\Windows\\System32", 0);

            if (AfxMountStorageUnit('c', &location.uri, afxFileFlag_RX))
                AfxThrowError();
#   elif defined(AFX_ON_LINUX)
            AfxMakeUri(&location.uri, 0, "x64", 0);

            if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location.uri, 0, "x64d", 0);

                if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#elif defined(AFX_ON_X86_32)
#   ifdef AFX_ON_WINDOWS
            AfxMakeUri(&location.uri, 0, "w32", 0);

            if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location.uri, 0, "w32d", 0);

                if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif

            AfxMakeUri(&location.uri, 0, "C:\\Windows\\SysWow64", 0);

            if (AfxMountStorageUnit('c', &location.uri, afxFileFlag_RX))
                AfxThrowError();
#   elif defined(AFX_ON_LINUX)
            AfxMakeUri(&location.uri, 0, "x32", 0);

            if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX)) AfxThrowError();
#       ifdef _DEBUG
            else
            {
                AfxMakeUri(&location.uri, 0, "x32d", 0);

                if (AfxMountStorageUnit('z', &location.uri, afxFileFlag_RWX))
                    AfxThrowError();
            }
#       endif
#   endif
#endif
        }
    }
#if 0
    if (!err)
    {
        AfxMakeUri(&location.uri, "code", 0);

        if (AfxMountStorageUnit('c', &location.uri, afxFileFlag_RX)) AfxThrowError();
        else
        {
            AfxMakeUri(&location.uri, "sound", 0);

            if (AfxMountStorageUnit('s', &location.uri, afxFileFlag_RX)) AfxThrowError();
            else
            {
                AfxMakeUri(&location.uri, "data", 0);

                if (AfxMountStorageUnit('d', &location.uri, afxFileFlag_RX)) AfxThrowError();
                else
                {
                    AfxMakeUri(&location.uri, "art", 0);

                    if (AfxMountStorageUnit('a', &location.uri, afxFileFlag_RX)) AfxThrowError();
                    else
                    {
                        AfxMakeUri(&location.uri, "tmp", 0);

                        if (AfxMountStorageUnit('t', &location.uri, afxFileFlag_RWX))
                            AfxThrowError();
                    }
                }
            }
        }
    }
#endif
    //MountHostVolumes();
    return err;
}

_AFX afxError _AfxSysDtorCb(afxSystem sys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    AfxDisposeObjects(1, &sys->e2coree);
    AfxDisposeObjects(1, &sys->primeThr);

    // objects will be released at class drop.
    AfxDeregisterChainedClasses(&sys->classes);

    //AfxDeallocateUri(&sys->pwd.uri);

    //AfxDismantleArena(&sys->ioArena);
    //AfxReleaseObject(&(sys->memD->obj));

    //_AfxFreeExternalAllocations();

    _AfxDeinitMmu(NIL);

    AfxZero(sys, sizeof(sys[0]));

    return err;
}

_AFX afxError _AfxSysCtorCb(afxSystem sys, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxManifest* ini = args[0];
    afxSystemConfig const *cfg = args[1];

    //AfxZero(sys, sizeof(sys[0]));

    _AfxInitMmu(NIL);

    afxChain* classes = &sys->classes;
    AfxMakeChain(classes, sys);
    AfxMountClass(&sys->strbCls, NIL, classes, &_AFX_STRB_CLASS_CONFIG);

    // setting the process working directory and the Qwadro/system working directory...
    afxUri uri;
    afxChar buf[2048];
    GetModuleFileNameA(NIL, buf, sizeof(buf));
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxMakeUri(&uri, 0, buf, 0);
    AfxMakeUri2048(&sys->pwd, &uri);
    AfxCanonicalizePath(&sys->pwd.uri, TRUE);
    GetModuleFileNameA(GetModuleHandleA("e2coree"), buf, sizeof(buf));    
    AfxStrrchr(buf, '\\')[1] = '\0';
    AfxStrcat(buf, "..\\");
    AfxMakeUri(&uri, 0, buf, 0);
    AfxMakeUri2048(&sys->qwd, &uri);
    AfxCanonicalizePath(&sys->qwd.uri, TRUE);
    SetCurrentDirectoryA(AfxGetUriData(&sys->pwd.uri, 0));

    sys->primeTid = AfxGetTid();
    AFX_ASSERT(sys->primeTid);

    sys->maxMemUsage = cfg->maxMemUsage;
    sys->memPageSiz = cfg->memPageSiz;
    sys->allocGranularity = cfg->allocGranularity;
    sys->ioBufSiz = cfg->ioBufSiz;
    sys->unitsPerMeter = cfg->unitsToMeter;

    sys->assertHook = cfg->assertHook;
    sys->profilerPopTimer = cfg->profilerPopTimer;
    sys->profilerPushTimer = cfg->profilerPushTimer;
    sys->profilerPostMarker = cfg->profilerPostMarker;

    sys->ptrSiz = sizeof(void*);
    sys->bigEndian = FALSE;

    //sys->genrlArenaSpace = cfg->genrlArenaSpace;
    //sys->ioArenaSpace = cfg->ioArenaSpace;
    sys->hwThreadingCap = cfg->hwThreadingCap;
    sys->reallocatorFn = cfg->reallocatorFn;

    sys->acxDisabled = cfg->acxDisabled;
    sys->auxDisabled = cfg->auxDisabled;
    sys->avxDisabled = cfg->avxDisabled;

    afxClassConfig thrClsCfg = _AFX_THR_CLASS_CONFIG;
    thrClsCfg.unitsPerPage = sys->hwThreadingCap;
    AfxMountClass(&sys->thrCls, NIL, classes, &thrClsCfg);

    afxClassConfig thrpClsCfg = _AFX_THRP_CLASS_CONFIG;
    AfxMountClass(&sys->thrpCls, NIL, classes, &thrpClsCfg); // require thr

    afxClassConfig mmuClsCfg = _AFX_MMU_CLASS_CONFIG;
    AfxMountClass(&sys->mmuCls, NIL, classes, &mmuClsCfg);

    afxClassConfig devClsCfg = _AFX_DEV_CLASS_CONFIG;
    devClsCfg.ctor = NIL; // must be called by specialized device constructor. It is needed to perform nested specialized device creation.
    AfxMountClass(&sys->devCls, NIL, classes, &devClsCfg);

    afxClassConfig svcClsCfg = _AFX_SVC_CLASS_CONFIG;
    AfxMountClass(&sys->svcCls, NIL, classes, &svcClsCfg);


    afxClassConfig cdcClsCfg = _AFX_CDC_CLASS_CONFIG;
    AfxMountClass(&sys->cdcCls, NIL, classes, &cdcClsCfg);

    afxClassConfig iobClsCfg = _AFX_IOB_CLASS_CONFIG;
    AfxMountClass(&sys->iosCls, NIL, classes, &iobClsCfg);

    afxClassConfig fsysClsCfg = _AFX_FSYS_CLASS_CONFIG;
    //AfxMountClass(&sys->fileMgr, NIL, classes, &_AfxFileMgrConfig);
    AfxMountClass(&sys->fsysCls, NIL, classes, &fsysClsCfg); // require iob, arch

    afxClassConfig mdleClsCfg = _AFX_MDLE_CLASS_CONFIG;
    AfxMountClass(&sys->mdleCls, NIL, classes, &mdleClsCfg);
    //AfxMountClass(&sys->icdCls, &sys->exeMgr, classes, &_AFX_ICD_CLASS_CONFIG);

    afxClassConfig xssClsCfg = _AFX_XSS_CLASS_CONFIG;
    AfxMountClass(&sys->xssCls, NIL, classes, &xssClsCfg);

    afxClassConfig exuClsCfg = _AFX_EXU_CLASS_CONFIG;
    AfxMountClass(&sys->exuCls, NIL, classes, &exuClsCfg);

    afxThreadConfig thrCfg = { 0 };
    thrCfg.tid = (sys->primeTid = AfxGetTid());
    
    if (AfxAcquireThreads(AfxHere(), &thrCfg, 1, &sys->primeThr))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_THR, 1, &sys->primeThr);
        //sys->primeThr = primeThr;

        if (_AfxSysMountDefaultFileStorages(sys))
        {
            AfxThrowError();
        }
        else
        {
            afxUri128 urib;
            AfxMakeUri128(&urib, NIL);
            AfxFormatUri(&urib.uri, "e2coree");
            afxModule e2coree;

            if (AfxAcquireModule(&urib.uri, AFX_BITMASK(8), &e2coree))
            {
                AfxThrowError();
            }
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &e2coree);
                sys->e2coree = e2coree;

                if (AfxAcquireIoBridge(&cfg->exus[0], &sys->primeExu))
                    AfxThrowError();

                if (err)
                    AfxDisposeObjects(1, &sys->e2coree);
            }
        }
    }

    if (err)
        AfxDeregisterChainedClasses(&sys->classes);

    return err;
}

_AFX afxError AfxConfigureSystem(afxSystemConfig* config, afxUri const* ini)
{
    afxError err = { 0 };
    afxSystemConfig cfg = { 0 };

    afxManifest ini2;
    AfxDeployManifest(&ini2);

    afxUri2048 urib;
    AfxMakeUri2048(&urib, NIL);

    if (ini)
    {
        AfxCopyUri(&urib.uri, ini);
    }
    else
    {
        AfxFormatUri(&urib.uri, "../system.ini");
    }

    if (AfxLoadInitializationFile(&ini2, &urib.uri))
    {
        afxString2048 tbs;
        AfxMakeString2048(&tbs, TheSystem ? AfxGetUriString(&TheSystem->pwd.uri) : NIL);
        AfxCatenateString(&tbs.s, "system.ini", 0);
        
        afxUri uri2;
        AfxWrapUriString(&uri2, &tbs.s);
        AfxLoadInitializationFile(&ini2, &uri2);
    }

#ifdef AFX_ON_WINDOWS
    SYSTEM_INFO si;
    GetSystemInfo(&si);
#endif

    afxString sSystem = AFX_STRING("System");

    if (!AfxGetInitializationNat(&ini2, &sSystem, &AFX_STRING("nIoBufferSize"), &cfg.ioBufSiz) || !cfg.ioBufSiz)
    {
        cfg.ioBufSiz = BUFSIZ;
    }

    if (!AfxGetInitializationReal(&ini2, &sSystem, &AFX_STRING("fUnitsPerMeter"), &cfg.unitsToMeter) || !cfg.unitsToMeter)
    {
        cfg.unitsToMeter = renderwareUnitsPerMeter;
    }

    if (!AfxGetInitializationNat(&ini2, &sSystem, &AFX_STRING("nHwThreadingCapacity"), &cfg.hwThreadingCap) || !cfg.hwThreadingCap)
    {
        cfg.hwThreadingCap =
#ifdef AFX_ON_WINDOWS
            si.dwNumberOfProcessors;
#else
            1;
#endif
    }

    if (!AfxGetInitializationNat(&ini2, &sSystem, &AFX_STRING("nMemoryPageSize"), &cfg.memPageSiz) || !cfg.memPageSiz)
    {
        cfg.memPageSiz =
#ifdef AFX_ON_WINDOWS
            si.dwPageSize;
#else
            4096;
#endif
    }

    if (!AfxGetInitializationNat(&ini2, &sSystem, &AFX_STRING("nAllocationGranularity"), &cfg.allocGranularity) || !cfg.allocGranularity)
    {
        cfg.allocGranularity =
#ifdef AFX_ON_WINDOWS
            si.dwAllocationGranularity;
#else
            4096;
#endif
    }

    cfg.assertHook = AfxStdAssertHookCallback;
    
    //cfg.root = NIL;
    cfg.profilerPopTimer = NIL;
    cfg.profilerPushTimer = NIL;
    cfg.profilerPostMarker = NIL;

    //if (platform)
    {
#ifdef AFX_ON_WINDOWS
        static afxHostSystemConfigExt hostSysExt = { 0 };

        AfxZero(&hostSysExt.w32, sizeof(hostSysExt.w32));
        hostSysExt.w32.hInst = GetModuleHandle(NULL);
        hostSysExt.w32.hWnd = GetActiveWindow();

        cfg.exts = &hostSysExt.ext;
#endif
    }

    if (!(AfxGetInitializationBool(&ini2, &AFX_STRING("DrawSystem"), &AFX_STRING("bDisabled"), &cfg.avxDisabled)))
    {
        cfg.avxDisabled = FALSE;
    }

    if (!(AfxGetInitializationBool(&ini2, &AFX_STRING("MixSystem"), &AFX_STRING("bDisabled"), &cfg.acxDisabled)))
    {
        cfg.acxDisabled = FALSE;
    }

    AfxMakeUri32(&cfg.shell, AfxUri("//./z/wshell4d.inf"));

    if (!(AfxGetInitializationBool(&ini2, &AFX_STRING("Shell"), &AFX_STRING("bDisabled"), &cfg.auxDisabled)))
    {
        cfg.auxDisabled = FALSE;

        AfxGetInitializationString(&ini2, &AFX_STRING("Shell"), &AFX_STRING("sModule"), &cfg.shell.uri.s);
        AfxReparseUri(&cfg.shell.uri);
    }

    AfxDismantleManifest(&ini2);

    *config = cfg;
}
