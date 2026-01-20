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

// This software is part of Advanced Multimedia Extensions.

// It is hard to invent something when there is nothing to be copied.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AMX_MIX_C
//#define _AMX_SOUNDSCAPE_C
//#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_QUEUE_C
//#define _AMX_MIX_INPUT_C
//#define _AMX_SINK_C
#include "amxIcd.h"
#include "../mmux/auxIcd.h"

_AMX afxDrawSystem _AmxMsysGetDsys(afxMixSystem msys)
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys->dsys;
}

_AMX _amxDdiMsys const* _AmxMsysGetDdi(afxMixSystem msys)
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    return msys->ddi;
}

_AMX afxMask _AmxMsysGetIoExuMask(afxMixSystem msys, afxMask* dedIoExuMask)
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    if (dedIoExuMask) *dedIoExuMask = msys->dedIoExuMask;
    return msys->ioExuMask;
}

_AMX afxModule AmxGetMixSystemIcd(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxModule mdle = AfxGetHost(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    return mdle;
}

_AMX void AmxGetEnabledSystemFeatures(afxMixSystem msys, amxFeatures* features)
{
    afxError err = { 0 };
    // @msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    AFX_ASSERT(features);
    *features = msys->requirements;

    return;
}

_AMX afxClass const* _AmxMsysGetSinkClass(afxMixSystem msys)
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->asioCls;
    AFX_ASSERT_CLASS(cls, afxFcc_ASIO);
    return cls;
}

_AMX afxClass const* _AmxMsysGetTraxClass(afxMixSystem msys)
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->traxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TRAX);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMexuClass(afxMixSystem msys)
{
    afxError err = { 0 };
    // msys must be a valid afxMixDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mexuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MEXU);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMixClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mixCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MCTX);
    return cls;
}

_AMX afxClass const* _AmxMsysGetBufClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MBUF);
    return cls;
}

_AMX afxClass const* _AmxMsysGetVidClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->vidCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VID);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMsrcClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->pmpCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSRC);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMencClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mencCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MENC);
    return cls;
}

_AMX afxClass const* _AmxMsysGetMdecClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->mdecCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDEC);
    return cls;
}

_AMX afxClass const* _AmxMsysGetAudClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->wavCls;
    AFX_ASSERT_CLASS(cls, afxFcc_AUD);
    return cls;
}

_AMX afxClass const* _AmxMsysGetSndsClass(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxClass const* cls = &msys->sndsCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SNDS);
    return cls;
}

_AMX _amxDdiMsys const _AMX_MSYS_IMPL =
{
#if 0
    .fencCls = _AmxMsysGetFencClassCb_SW,
    .mexuCls = _AmxMsysGetDexuClassCb_SW,
    .sinkCls = _AmxMsysGetDoutClassCb_SW,
    .bufCls = _AmxMsysGetBufClassCb_SW,
#endif
    .transferCb = _AmxMsysTransferCb_SW,
    .cohereCb = _AmxMsysCohereMappedBuffersCb_SW,
    .remapCb = _AmxMsysRemapBuffersCb_SW,
#if 0
    .allocBufCb = _AmxMsysAllocateBuffersCb_SW,
    .deallocBufCb = _AmxMsysDeallocateBuffersCb_SW,
#endif
};

_AMX afxUnit AmxGetMixBridges(afxMixSystem msys, afxUnit baseIdx, afxUnit cnt, afxMixBridge bridges[])
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxUnit bridgeCnt = msys->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxMixBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxMixBridge mexu = msys->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        bridges[rslt++] = mexu;
    }
    return rslt;
}

_AMX afxUnit AmxChooseMixBridges(afxMixSystem msys, afxUnit mdevId, amxAptitude caps, afxMask exuMask, afxUnit first, afxUnit maxCnt, afxMixBridge bridges[])
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    afxUnit found = 0;
    afxUnit rslt = 0;

    if ((mdevId == AFX_INVALID_INDEX) && (!caps) && (!bridges))
        return maxCnt ? AFX_MIN(maxCnt, msys->bridgeCnt - first) : AFX_MIN(msys->bridgeCnt, msys->bridgeCnt - first);

    afxUnit bridgeCnt = msys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < bridgeCnt; exuIdx++)
    {
        // skip if a mask is specified and this EXU is not bitmapped in it.
        if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
            continue;

        afxMixBridge mexu = msys->bridges[exuIdx];
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

        afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        if ((mdevId != AFX_INVALID_INDEX) && (mdevId != AfxGetObjectId(mdev)))
            continue;

        if (caps)
        {
            amxDeviceInfo capsi;
            AmxQueryMixCapabilities(mdev, &capsi);

            if ((capsi.capabilities & caps) != caps)
                continue;
        }

        if (maxCnt && (found >= first))
        {
            if (bridges)
            {
                bridges[rslt] = mexu;
            }

            if (maxCnt == rslt)
                break;
        }

        if (found >= first)
            rslt++;

        found++;
    }
    return rslt;
}

_AMX afxError AmxWaitForMixQueue(afxMixSystem msys, afxTime timeout, afxUnit exuIdx, afxUnit queId)
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(msys->bridgeCnt, exuIdx, 1);

    afxMixBridge mexu;
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(msys->bridgeCnt, exuIdx, 1);
    while (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixQueue mque;
    if (!AmxGetMixQueues(mexu, queId, 1, &mque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MQUE, 1, &mque);

    if (AmxWaitForEmptyMixQueue(mexu, queId, timeout))
        AfxThrowError();

    return err;
}

_AMX afxError AmxWaitForMixBridges(afxMixSystem msys, afxTime timeout, afxMask exuMask)
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxUnit exuCnt = msys->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(AFX_TEST_BIT_SET(exuMask, exuIdx)))
            continue;

        afxMixBridge mexu;
        if (!AmxGetMixBridges(msys, exuIdx, 1, &mexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);
        AmxWaitForIdleMixBridge(mexu, timeout);
    }
    return err;
}

_AMX afxError AmxWaitForMixSystem(afxMixSystem msys, afxTime timeout)
{
    afxError err = { 0 };
    // msys must be a valid afxMixSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (!msys->ddi->waitCb)
    {
        AmxWaitForMixBridges(msys, timeout, AFX_MASK_IGNORED);
    }
    else if (msys->ddi->waitCb(msys, timeout))
        AfxThrowError();

    return err;
}

_AMX afxError _AmxMsysDtorCb(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = AmxGetMixSystemIcd(msys);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);
    AmxWaitForMixSystem(msys, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = msys->bridgeCnt;

    AfxDeregisterChainedClasses(&msys->ctx.classes);
    AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));

    for (afxUnit i = bridgeCnt; i-- > 0;)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &msys->bridges[i]);
        AfxDisposeObjects(1, &msys->bridges[i]);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(msys->bridges[0]),
            .var = (void**)&msys->bridges
        }
    };

    if (AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    if (msys->dsys)
    {
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys->dsys);
        AfxDisposeObjects(1, &msys->dsys);
    }

    return err;
}

_AMX afxError _AmxMsysCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    __amxMsysAcq const* cfg = AFX_CAST(__amxMsysAcq const*, args[1]) + invokeNo;
    _amxMexuAcq* bridgeCfgs = AFX_CAST(_amxMexuAcq*, args[2]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    msys->ddi = &_AMX_MSYS_IMPL;
    msys->idd = NIL;
    msys->udd = cfg->udd;
    msys->tag = cfg->tag;

    msys->running = FALSE;

    msys->requirements = cfg->reqFeatures;

    {
        afxChain* classes = &msys->ctx.classes;
        AfxMakeChain(classes, (void*)msys);

        afxClassConfig clsCfg;

        clsCfg = cfg->sndsClsCfg ? *cfg->sndsClsCfg : _AMX_SNDS_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_SNDS);
        AfxMountClass(&msys->sndsCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mbufClsCfg ? *cfg->mbufClsCfg : _AMX_MBUF_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MBUF);
        AfxMountClass(&msys->mbufCls, NIL, classes, &clsCfg);

        clsCfg = cfg->wavClsCfg ? *cfg->wavClsCfg : _AMX_AUD_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_AUD);
        AfxMountClass(&msys->wavCls, NIL, classes, &clsCfg);

        clsCfg = cfg->vidClsCfg ? *cfg->vidClsCfg : _AMX_VID_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_VID);
        AfxMountClass(&msys->vidCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mdecClsCfg ? *cfg->mdecClsCfg : _AMX_MDEC_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MDEC);
        AfxMountClass(&msys->mdecCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mencClsCfg ? *cfg->mencClsCfg : _AMX_MENC_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MENC);
        AfxMountClass(&msys->mencCls, NIL, classes, &clsCfg);

        clsCfg = cfg->pmpClsCfg ? *cfg->pmpClsCfg : _AMX_MSRC_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MSRC);
        AfxMountClass(&msys->pmpCls, NIL, classes, &clsCfg);

        clsCfg = cfg->mixClsCfg ? *cfg->mixClsCfg : _AMX_MCTX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MCTX);
        AfxMountClass(&msys->mixCls, NIL, classes, &clsCfg);

        clsCfg = cfg->traxClsCfg ? *cfg->traxClsCfg : _AMX_TRAX_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_TRAX);
        AfxMountClass(&msys->traxCls, NIL, classes, &clsCfg);

        afxClassConfig asioClsCfg;
        if (cfg->sinkClsCfg) asioClsCfg = *cfg->sinkClsCfg;
        else
        {
            asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
            if (_AuxIcdGetInteropSinkClass(msys, &AFX_STRING(""), &asioClsCfg))
            {
                asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
            }
        }
        AFX_ASSERT(asioClsCfg.fcc == afxFcc_ASIO);
        AfxMountClass(&msys->asioCls, NIL, classes, &asioClsCfg); // require mdev, sout

        clsCfg = cfg->mexuClsCfg ? *cfg->mexuClsCfg : _AMX_MEXU_CLASS_CONFIG;
        AFX_ASSERT(clsCfg.fcc == afxFcc_MEXU);
        AfxMountClass(&msys->mexuCls, NIL, classes, &clsCfg);
    }

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->bridgeCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    msys->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(msys->bridges[0]),
            .var = (void**)&msys->bridges
        }
    };

    if (AfxAllocateInstanceData(msys, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));
        return err;
    }

    if ((msys->dsys = cfg->dsys))
    {
        msys->dsys = cfg->dsys;
        AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &msys->dsys);
        AfxReacquireObjects(1, &msys->dsys);
    }

    if (_AmxMsysBridgeDevices(msys, msys->bridgeCnt, bridgeCfgs, msys->bridges))
    {
        AfxThrowError();

        AfxDisposeObjects(1, &msys->dsys);

        AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, msys->bridgeCnt, msys->bridges);

    msys->ioExuMask = NIL;
    msys->dedIoExuMask = NIL;
    msys->gfxExuMask = NIL;
    msys->cfxExuMask = NIL;
    msys->dedCfxExuMask = NIL;
    msys->videoExuMask = NIL;

    for (afxUnit i = 0; i < msys->bridgeCnt; i++)
    {
        afxMixBridge mexu;
        AmxGetMixBridges(msys, i, 1, &mexu);
        AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

        afxMixDevice mdev = AmxGetBridgedMixDevice(mexu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

        amxDeviceInfo capsi;
        AmxQueryMixCapabilities(mdev, &capsi);

        if ((capsi.capabilities & amxAptitude_DMA) == amxAptitude_DMA)
            msys->ioExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (amxAptitude_DMA | amxAptitude_PCX | amxAptitude_SFX)) == amxAptitude_DMA)
            msys->dedIoExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & amxAptitude_PCX) == amxAptitude_PCX)
            msys->cfxExuMask |= AFX_BITMASK(i);
        if ((capsi.capabilities & (amxAptitude_PCX | amxAptitude_SFX)) == amxAptitude_PCX)
            msys->dedCfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & amxAptitude_SFX) == amxAptitude_SFX)
            msys->gfxExuMask |= AFX_BITMASK(i);

        if ((capsi.capabilities & avxAptitude_PRESENT) == avxAptitude_PRESENT)
            msys->videoExuMask |= AFX_BITMASK(i);
    }

    AfxMakeChain(&msys->activeTrackers, msys);

    afxMixDevice mdev = AmxGetBridgedMixDevice(msys->bridges[0], NIL);

    AfxCallDevice((afxDevice)mdev, 3, msys);
    AfxCallDevice((afxDevice)mdev, 5, msys);

    if (AfxDoDeviceService((afxDevice)mdev)) AfxThrowError(); // let the device build its SPUs.
    else
    {
        msys->running = TRUE;
    }

    if (err)
    {
        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&msys->ctx.classes);
        AFX_ASSERT(AfxIsChainEmpty(&msys->ctx.classes));

        AfxDeallocateInstanceData(msys, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_AMX afxClassConfig const _AMX_MSYS_CLASS_CONFIG =
{
    .fcc = afxFcc_MSYS,
    .name = "MixSystem",
    .desc = "Mix I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxMixSystem)),
    .ctor = (void*)_AmxMsysCtorCb,
    .dtor = (void*)_AmxMsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxUnit AmxEnumerateMixSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxMixSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));

    afxClass const* cls = _AmxIcdGetMsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);

    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, rslt, systems);

    return rslt;
}

_AMX afxUnit AmxEvokeMixSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt, afxMixSystem systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
    
    afxClass const* cls = _AmxIcdGetMsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);
    
    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, rslt, systems);

    return rslt;
}

_AMX afxUnit AmxInvokeMixSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxMixSystem), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    if (!_AmxGetIcd(icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AMX) == (afxModuleFlag_ICD | afxModuleFlag_AMX));
    
    afxClass const* cls = _AmxIcdGetMsysClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_MSYS);

    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_AMX afxError AmxConfigureMixSystem(afxUnit icd, amxSystemConfig* cfg)
{
    afxError err = { 0 };

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    amxAptitude caps = cfg->caps;
    afxAcceleration accel = cfg->accel;

    afxModule drv;
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    if (!_AmxGetIcd(icd, &drv))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AMX));

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AVX_MAX_BRIDGES_PER_SYSTEM; i++)
        {
            afxUnit mdevId = i;

            afxMixDevice mdev;
            if (AmxEnumerateMixDevices(icd, mdevId, 1, &mdev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

                amxDeviceInfo capsi;
                AmxQueryMixCapabilities(mdev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].mdevId = mdevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
        }
    }
    else
    {
        afxUnit exuCnt = cfg->exuCnt;
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < exuCnt; i++)
        {
            amxDeviceInfo capsi = { 0 };
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;

            afxUnit mdevId;
            if (AmxChooseMixDevices(icd, &capsi, NIL, NIL, 1, &mdevId))
            {
                afxMixDevice mdev;
                AmxEnumerateMixDevices(icd, mdevId, 1, &mdev);
                AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

                AmxQueryMixCapabilities(mdev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].mdevId = mdevId;
                cfg->exus[cfg->exuCnt].minQueCnt = capsi.minQueCnt;
                cfg->exus[cfg->exuCnt].queuePriority = NIL;
                ++cfg->exuCnt;
            }
#if 0
            else
            {
                cfg->exus[i].capabilities = caps.capabilities;
                cfg->exus[i].acceleration = caps.acceleration;
                cfg->exus[i].minQueCnt = caps.minQueCnt;
                cfg->exus[i].ddevId = NIL;
            }
#endif
        }
    }
    return err;
}

_AMX afxError AmxEstablishMixSystem(afxUnit icd, amxSystemConfig const* cfg, afxMixSystem* system)
{
    afxError err = { 0 };
    AFX_ASSERT(icd != AFX_INVALID_INDEX);
    AFX_ASSERT(system);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT(cfg->exuCnt);

        if (!cfg->exuCnt)
        {
            AfxThrowError();
            return err;
        }
    }

    afxClass* msysCls = NIL;
    afxModule drv = NIL;

    if (icd != AFX_INVALID_INDEX)
    {
        if (!_AmxGetIcd(icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AMX));

        msysCls = (afxClass*)_AmxIcdGetMsysClass(drv);
        AFX_ASSERT_CLASS(msysCls, afxFcc_MSYS);

    }
    else
    {
        static afxBool clsInited = FALSE;
        static afxClass staticMsysCls = { 0 };

        if (!clsInited)
        {
            AfxMountClass(&staticMsysCls, NIL, NIL, &_AMX_MSYS_CLASS_CONFIG);
            clsInited = TRUE;
        }

        msysCls = &staticMsysCls;
        AFX_ASSERT_CLASS(msysCls, afxFcc_MSYS);
    }

    // Acquire bridges and queues
    afxUnit totalSqueCnt = 0;
    afxUnit baseQueIdx[AMX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    _amxMexuAcq bridgeCfg[AMX_MAX_BRIDGES_PER_SYSTEM] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(AMX_MAX_BRIDGES_PER_SYSTEM, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        amxBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable to allowed multibridge to same port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalSqueCnt;
                totalSqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxMixDevice mdev;
        if (!AmxEnumerateMixDevices(icd, exuCfg->mdevId, 1, &mdev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDEV, 1, &mdev);

            if (AfxCallDevice((afxDevice)mdev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].mdev = mdev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, AMX_MAX_QUEUES_PER_BRIDGE);;
        amxDeviceInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].mqueClsCfg = &_AMX_MQUE_CLASS_CONFIG;
        bridgeCfg[bridgeCnt].mixClsCfg = &_AMX_MCTX_CLASS_CONFIG;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    __amxMsysAcq cfg2 = { 0 };
    cfg2.bridgeCnt = bridgeCnt;
    cfg2.reqExtCnt = cfg->extCnt;
    cfg2.reqExts = cfg->exts;
    cfg2.reqFeatures = cfg->features;
    cfg2.udd = cfg->udd;
    cfg2.tag = cfg->tag;
    cfg2.dsys = cfg->dsys;

    AFX_ASSERT_CLASS(msysCls, afxFcc_MSYS);

    afxMixSystem msys;
    if (AfxAcquireObjects(msysCls, 1, (afxObject*)&msys, (void const*[]) { drv, &cfg2, &bridgeCfg[0], }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    *system = msys;
    return err;
}
