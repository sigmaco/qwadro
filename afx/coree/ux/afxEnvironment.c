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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This software is part of Advanced User Experience Extensions.

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_ENVIRONMENT_C
#define _AUX_WINDOW_C
#include "auxIcd.h"
#include "../xss/xss.h"

// TODO move to afxSystem.
// TODO TLS it
 _AUX afxEnvironment gActiveEnv = NIL;

AUX xssConsoleCrateInfo envXssCrates[];

_AUX afxUnit AfxGetEnvironmentId(afxEnvironment env)
{
    afxError err = { 0 };
    return env ? AfxGetObjectId(env) : 0;
}

_AUX _auxEnvDdi const* _AuxEnvGetDdi(afxEnvironment env)
{
    afxError err = { 0 };
    // @dpy must be a valid afxDisplay handle.
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return env->ddi;
}

_AUX afxClass const* _AuxEnvGetWndClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->wndCls;
    AFX_ASSERT_CLASS(cls, afxFcc_WND);
    return cls;
}

_AUX afxClass const* _AuxEnvGetFntClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->fntCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FNT);
    return cls;
}

_AUX afxClass const* _AuxEnvGetThemClass(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxClass const* cls = &env->themCls;
    AFX_ASSERT_CLASS(cls, afxFcc_THEM);
    return cls;
}

_AUX afxBool AfxGetFocusedWindow(afxUnit seat, afxWindow* window)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    if (seat > 0)
    {
        int a = 1;
    }
    afxWindow wnd = env->focusedWnd;
    if (window) *window = wnd;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    return !!wnd;
}

_AUX afxError _AfxEnvSwFocusWindowCb(afxEnvironment env, afxUnit seat, afxWindow wnd, afxFocusFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    afxWindow curr = env->focusedWnd;

    if (curr != wnd)
    {
        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &curr);

            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS_LOST;
            ev.wnd = curr;
            AfxEmitEvent(curr, &ev.ev);

            if (curr->fullscreen)
            {
                AfxTakeFullscreen(curr, FALSE);
            }
            curr->focused = FALSE;
            env->focusedWnd = NIL;
        }

        if (wnd)
        {
            AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

            if (gActiveEnv != env)
                gActiveEnv = env;

            wnd->focused = TRUE;
            env->focusedWnd = wnd;

            auxEvent ev = { 0 };
            ev.id = auxEventId_FOCUS;
            ev.wnd = wnd;
            AfxEmitEvent(wnd, &ev.ev);
        }
    }

    return err;
}

_AUX afxError AfxFocusWindow(afxUnit seat, afxWindow wnd, afxFocusFlags flags)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->focusCb(env, seat, wnd, flags);
}

_AUX afxError AfxDrawBackgroundEXT(afxDrawContext dctx, afxFlags flags)
{
    afxError err = { 0 };
    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return afxError_NOT_READY;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (!env->ddi->drawBgCb) return afxError_UNSUPPORTED;
    return env->ddi->drawBgCb(env, dctx, flags);
}

_AUX afxBool AfxGetConsole(afxEnvironment env, afxConsole* console)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    afxConsole con = env->con;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_XSS, 1, &con);
    AFX_ASSERT(console);
    *console = con;
    return !!con;
}

_AUX afxBool AfxGetUxVideo(afxEnvironment env, afxDrawSystem* system, afxSurface* surface)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT(system || surface);
    afxBool rslt = 0;

    if (system)
    {
        afxDrawSystem dsys = env->dsys;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
        *system = dsys;
        rslt |= AFX_BITMASK(0);
    }

    if (surface)
    {
        afxSurface dout = NIL;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);
        *surface = dout;
        rslt |= AFX_BITMASK(1);
    }

    return rslt;
}

_AUX afxBool AfxGetUxAudio(afxEnvironment env, afxMixSystem* system, afxSink* sink)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT(system || sink);
    afxBool rslt = 0;

    if (system)
    {
        afxMixSystem msys = env->msys;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
        *system = msys;
        rslt |= AFX_BITMASK(0);
    }

    if (sink)
    {
        afxSink snk = env->aso;
        AFX_TRY_ASSERT_OBJECTS(afxFcc_ASIO, 1, &snk);
        *sink = snk;
        rslt |= AFX_BITMASK(1);
    }
    return rslt;
}

_AUX afxBool _AfxEnvSwHasClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxClipboardFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    return env->seats[seat].clipb.s.len;
}

_AUX afxBool AfxHasClipboardContent(afxUnit seat, afxUnit slot, afxClipboardFlags flags)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->hasClipboardCb(env, seat, slot, flags);
}

_AUX afxUnit _AfxEnvSwGetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxClipboardFlags flags, afxString* buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    return AfxCopyString(buf, 0, &env->seats[seat].clipb.s, 0);
}

_AUX afxUnit AfxGetClipboardContent(afxUnit seat, afxUnit slot, afxClipboardFlags flags, afxString* buf)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return 0;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    return env->ddi->getClipboardCb(env, seat, slot, flags, buf);
}

_AUX afxError _AfxEnvSwSetClipboardContentCb(afxEnvironment env, afxUnit seat, afxUnit slot, afxClipboardFlags flags, afxString const* buf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);
    AfxCopyString(&env->seats[seat].clipb.s, 0, buf, 0);
    return err;
}

_AUX afxError AfxSetClipboardContent(afxUnit seat, afxUnit slot, afxClipboardFlags flags, afxString const* text)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return err;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    if (env->ddi->setClipboardCb(env, seat, slot, flags, text))
        AfxThrowError();

    return err;
}

_AUX afxBool AfxGetCursorPlacement(afxUnit seat, afxWindow wnd, afxBool onFrame, afxRect* rc)
{
    afxError err = { 0 };
    AFX_ASSERT(!onFrame || (wnd));
    AFX_ASSERT(rc);
    afxBool rslt = TRUE;

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT_RANGE(env->seatCnt, seat, 1);

    if (env->ddi->getCurs)
    {
        return env->ddi->getCurs(env, seat, wnd, onFrame, rc);
    }
    else
    {
        afxRect cursRect = env->cursRect;

        if (rc)
            *rc = cursRect;

        if (wnd)
        {
            afxRect frameRc = wnd->frameRc, surfaceRc;
            AfxGetWindowArea(wnd, afxAnchor_TOP | afxAnchor_LEFT, &surfaceRc);

            if (onFrame)
            {
                *rc = AfxGetIntersectedRect(&cursRect, &frameRc);
            }
            else
            {
                *rc = AfxGetIntersectedRect(&cursRect, &surfaceRc);
            }
        }
        rslt = !AfxIsRectVoid(rc);
    }
    return rslt;
}

_AUX afxUnit64 _AfxEnvSwPollInputCb(afxEnvironment env, afxFlags flags, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    return 0;
}

_AUX afxTime AfxDoUx(afxFlags flags, afxUnit64 timeout)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env)) return 0;
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxTime first, last, dt;
    AfxGetTime(&first);

    env->ddi->pumpCb(env, 0, timeout);

    dt = (AfxGetTime(&last) - first);

    return dt;
}

_AUX afxError AfxTakeFullscreen(afxWindow wnd, afxBool fullscreen)
{
    afxError err = { 0 };

    afxEnvironment env;
    if (!AfxGetActiveEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (wnd)
    {
        AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);

        if (!fullscreen && wnd->fullscreen)
        {
            env->ddi->fseCb(env, wnd, FALSE);
            wnd->fullscreen = FALSE;
        }
        else
        {
            env->ddi->fseCb(env, wnd, fullscreen);
            wnd->fullscreen = TRUE;
        }
    }
    else
    {
        env->ddi->fseCb(env, NIL, FALSE);
    }
    return err;
}

_AUX afxBool AFX_ENV_EVENT_HANDLER(afxEnvironment env, auxEvent *ev)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (ev->ev.id != afxEvent_UX)
    {
        return FALSE;
    }

    afxWindow focusedWnd = NIL;
    AfxGetFocusedWindow(ev->seat, &focusedWnd);

    switch (ev->id)
    {
    default:
    {
        auxEvent ev2 = *ev;
        ev2.ev.id = afxEvent_UX;
        ev2.ev.siz = sizeof(ev2);
        
        if (!ev2.wnd)
            ev2.wnd = focusedWnd;

        if (focusedWnd)
            AfxEmitEvent(focusedWnd, (void*)&ev2);
    }
    }
    return TRUE;
}

_AUX _auxEnvDdi const _AUX_ENV_DDI_SW =
{
    .pumpCb = _AfxEnvSwPollInputCb,
    .hasClipboardCb = _AfxEnvSwHasClipboardContentCb,
    .getClipboardCb = _AfxEnvSwGetClipboardContentCb,
    .setClipboardCb = _AfxEnvSwSetClipboardContentCb,
    .focusCb = _AfxEnvSwFocusWindowCb,

    .cfgWndCb = _AvxEnvSwConfigureWndCb,
    .acqWndCb = _AuxEnvSwAcquireWndCb,
};

_AUX afxError _AuxEnvSwDtorCb(afxEnvironment env)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    AFX_ASSERT(!env->idd);
    AfxDeregisterChainedClasses(&env->classes);

    afxUri location;
    AfxMakeUri(&location, 0, "system", 0);
    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);

    return err;
}

_AUX afxError _AuxEnvSwCtorCb(afxEnvironment env, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _auxEnvAcq const* cfg = AFX_CAST(_auxEnvAcq const*, args[1]) + invokeNo;
    AFX_ASSERT(cfg);

    AfxInstallEventHandler(env, (void*)AFX_ENV_EVENT_HANDLER);

    env->ddi = &_AUX_ENV_DDI_SW;
    env->idd = NIL;
    env->udd = cfg->cfg.udd;
    env->tag = cfg->cfg.tag;

    AfxMakeString512(&env->name, &cfg->cfg.name);

    {
        AfxMakeChain(&env->classes, env);

        afxClassConfig wndClsCfg = cfg->wndClsCfg ? *cfg->wndClsCfg : _AUX_WND_CLS_CFG;
        AfxMountClass(&env->wndCls, NIL, &env->classes, &wndClsCfg);

        afxClassConfig fntClsCfg = cfg->fntClsCfg ? *cfg->fntClsCfg : _AUX_FNT_CLASS_CONFIG;
        AfxMountClass(&env->fntCls, NIL, &env->classes, &fntClsCfg);

        afxClassConfig themClsCfg = cfg->themClsCfg ? *cfg->themClsCfg : _AUX_THEM_CLASS_CONFIG;
        AfxMountClass(&env->themCls, NIL, &env->classes, &themClsCfg);

    }

    afxUnit dwmCnt = 1;

    for (afxUnit i = 0; i < dwmCnt; i++)
    {
        afxDesktop* dwm = &env->dwm;

        dwm->res.w = 1;
        dwm->res.h = 1;
        dwm->res.d = 1;
        dwm->dout = NIL;
        dwm->refreshRate = 1;
    }

    env->focusedWnd = NIL;
    env->cursCapturedOn = NIL;

    env->seatCnt = AFX_MAX(AFX_MIN(AFX_MAX_USERS_PER_ENVIRONMENT, cfg->cfg.seatCnt), 1);

    for (afxUnit i = 0; i < env->seatCnt; i++)
    {
        AfxMakeString4096(&env->seats[i].clipb, NIL);

        env->seats[i].hidNo;
        AfxZero(&env->seats[i], sizeof(env->seats[0]));
        env->seats[i].buttonCnt = AFX_MB_TOTAL;
        env->seats[i].keyCnt = afxKey_TOTAL;
    }

    if (cfg->cfg.dsys)
    {
        afxDrawSystem dsys = cfg->cfg.dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
        AfxReacquireObjects(1, &dsys);
        env->dsys = dsys;
        env->vduIdx = cfg->cfg.vduIdx;
    }
    else
    {
        env->dsys = NIL;
        env->vduIdx = AFX_INVALID_INDEX;
    }

    if (!err)
    {
        if (cfg->cfg.msys)
        {
            afxMixSystem msys = cfg->cfg.msys;
            AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
            AfxReacquireObjects(1, &msys);
            env->msys = msys;
            env->soutIdx = cfg->cfg.soutIdx;

            afxSinkConfig asoCfg = { 0 };
            AfxConfigureAudioSink(msys, &asoCfg);
            AfxOpenAudioSink(msys, &asoCfg, &env->aso);
        }
        else
        {
            env->msys = NIL;
            env->soutIdx = AFX_INVALID_INDEX;
#if 0
            afxMixSystem ssys = NIL;
            afxUnit sdevId = cfg->sdevId;

            if (sdevId != AFX_INVALID_INDEX)
            {
                amxSystemConfig sccfg;
                AmxConfigureMixSystem(sdevId, &sccfg);

                if (AmxEstablishMixSystem(sdevId, &sccfg, &ssys))
                    AfxThrowError();
            }
            env->ssys = ssys;
            env->sdevId = sdevId;
            env->soutIdx = cfg->soutIdx;
            afxSinkConfig asoCfg;
            AfxConfigureAudioSink(ssys, &asoCfg);
            AfxOpenAudioSink(ssys, &asoCfg, &env->aso);
#endif

#if 0
            afxStream file;
            AfxOpenFile(AfxUri("../system/boot"), afxFileFlag_R, &file);
            afxUnit siz = AfxMeasureStream(file);
            afxUnit16* buf;
            AfxAllocate(AfxHere(), siz + 441000, 0, (void**)&buf);
            AfxReadStreamAt(file, 44, siz, 0, buf);

            while (1)
            {
                afxReal data[48000 * 2];
                
                for (afxUnit i = 0; i < 48000; i++)
                    data[i] = AfxRandom2(8000, 490956520);

                //AmxUploadAudioStream(env->aso, file, 0);
                afxUnit fc = (siz / 2) / 44100 / 2;
                afxUnit fc2 = fc / 2;

                afxUnit j = 0;
                for (afxUnit i = 0; i < fc; i++)
                {
                    AfxPushVoice(env->aso, 44100, &buf[j]);
                    j += 44100 * 2;
                //AfxSleep(1);
                }
            }
            AfxDisposeObjects(1, &file);
#endif
        }

        if (!err)
        {
            afxUri location;
            AfxMakeUri(&location, 0, "tmp", 0);

            if (AfxMountStorageUnit('d', &location, afxFileFlag_RWX)) AfxThrowError();
            else
            {
                afxUri uri;
                AfxMakeUri(&uri, 0, "system/qwadro.xss", 0);

                afxString s;
                AfxMakeString(&s, 0, "qwadro", 0);
                //AfxLoadScript(&s, &uri);

                if (err)
                    AfxDismountStorageUnit('d', &location, afxFileFlag_RWX);
            }

            afxConsole con;
            afxConsoleConfig conCfg = { 0 };
            conCfg.crates = envXssCrates;

            if (AfxAcquireConsole(&conCfg, &con))
            {
                AfxThrowError();
            }

            env->con = con;

            if (err)
            {
                AfxDisposeObjects(1, &env->msys);
            }
        }

        if (err)
        {
            AfxDisposeObjects(1, &env->dsys);
        }
    }

    if (err)
        AfxDeregisterChainedClasses(&env->classes);

    return err;
}

_AUX afxClassConfig const _AUX_ENV_CLS_CFG =
{
    .fcc = afxFcc_ENV,
    .name = "Environment",
    .desc = "MMUX Environment",
    .fixedSiz = sizeof(AFX_OBJECT(afxEnvironment)),
    .ctor = (void*)_AuxEnvSwCtorCb,
    .dtor = (void*)_AuxEnvSwDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError _AuxIcdSwConfigureEnvCb(afxModule auxIcd, afxEnvironmentConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &auxIcd);
    AFX_ASSERT(AfxTestModule(auxIcd, afxModuleFlag_ICD | afxModuleFlag_AUX));
    AFX_ASSERT(cfg);

    if (cfg->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);
    }

    if (cfg->msys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &cfg->msys);
    }

    return err;
}

_AUX afxError AfxConfigureEnvironment(afxModule auxIcd, afxEnvironmentConfig* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &auxIcd);

    if (!AfxTestModule(auxIcd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return err;
    }

    AFX_ASSERT(cfg);
    if (AfxFailed(_AuxGetDdi(auxIcd)->cfgEnvCb(auxIcd, cfg)))
    {
        AfxThrowError();
        return err;
    }

    if (cfg->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);
    }

    if (cfg->msys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &cfg->msys);
    }

    return err;
}

_AUX afxError _AuxIcdSwEstablishEnvCb(afxModule auxIcd, afxEnvironmentConfig const* cfg, afxEnvironment* environment)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &auxIcd);
    AFX_ASSERT(AfxTestModule(auxIcd, afxModuleFlag_ICD | afxModuleFlag_AUX));
    AFX_ASSERT(environment);
    AFX_ASSERT(cfg);

    _auxEnvAcq cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.cfg.dsys = cfg->dsys;
    cfg2.cfg.seatCnt = cfg->seatCnt;
    //cfg2.cfg.seats = cfg->seats;
    cfg2.cfg.ddevId = cfg->ddevId;
    cfg2.cfg.sdevId = cfg->sdevId;
    cfg2.cfg.soutIdx = cfg->soutIdx;
    cfg2.cfg.msys = cfg->msys;

    cfg2.cfg.tag = cfg->tag;
    cfg2.cfg.udd = cfg->udd;
    cfg2.cfg.name = cfg->name;

    afxClass* envCls = (afxClass*)_AuxIcdGetEnvClass(auxIcd);
    AFX_ASSERT_CLASS(envCls, afxFcc_ENV);

    afxEnvironment env;
    if (AfxFailed(AfxAcquireObjects(envCls, 1, (afxObject*)&env, (void const*[]) { auxIcd, &cfg2, NIL })))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    *environment = env;

    return err;
}

_AUX afxError AfxEstablishEnvironment(afxModule auxIcd, afxEnvironmentConfig const* cfg, afxEnvironment* environment)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &auxIcd);
    AFX_ASSERT(environment);
    AFX_ASSERT(cfg);

    if (!AfxTestModule(auxIcd, afxModuleFlag_ICD | afxModuleFlag_AUX))
    {
        AfxThrowError();
        err = afxError_INCOMPATIBLE_DRIVER;
        return err;
    }

    afxEnvironment env = NIL;
    if (AfxFailed(_AuxGetDdi(auxIcd)->acqEnvCb(auxIcd, cfg, &env)))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    *environment = env;

    return err;
}

_AUX afxBool AfxGetActiveEnvironment(afxEnvironment* environment)
{
    afxError err = { 0 };
    afxEnvironment env = gActiveEnv;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (environment)
        *environment = env;

    return !!env;
}
