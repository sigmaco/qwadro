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
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA GL/2.
// This software is part of Advanced Video Graphics Extensions.

#define _AFX_CORE_C
//#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AFX_CORE_C
#define _AFX_IOMMU_C
#define _AFX_BUFFER_C
#include "afxIommuDDK.h"

_AFX _afxIomDdi const* _AfxIomGetDdi(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    return iom->ddi;
}

_AFX afxClass const* _AfxIomSW_GetCtxClassCb(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxClass const* cls = &iom->ctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CTX);
    return cls;
}

_AFX afxClass const* _AfxIomSW_GetExuClassCb(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxClass const* cls = &iom->exuCls;
    AFX_ASSERT_CLASS(cls, afxFcc_EXU);
    return cls;
}

_AFX afxClass const* _AfxIomSW_GetFencClassCb(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxClass *cls = &iom->fencCls;
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);
    return cls;
}

_AFX afxClass const* _AfxIomSW_GetBufClassCb(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxClass *cls = &iom->bufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_BUF);
    return cls;
}

_AFX afxError _AfxIomSW_TransferCb(afxIommu iom, afxTransference* ctrl, afxUnit opCnt, void const* ops)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(opCnt);
    AFX_ASSERT(ctrl);
    AFX_ASSERT(ops);

    afxMask iomIoExuMask = iom->ioExuMask;
    afxMask exuMask = ctrl->exuMask;
    afxUnit exuCnt = iom->bridgeCnt;
    afxUnit firstExuIdx = AfxRandom2(0, exuCnt - 1);

    AFX_ASSERT(!exuMask || (iomIoExuMask & exuMask));

    afxBool queued = FALSE;

    while (1)
    {
        for (afxUnit exuIdx = firstExuIdx; exuIdx < exuCnt; exuIdx++)
        {
            firstExuIdx = 0;

            // Skip non-transfer-capable EXUs.
            if (!(iomIoExuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            if (exuMask && !(exuMask & AFX_BITMASK(exuIdx)))
                continue; // for

            // Try to pick one of the dedicated ones when EXUs are not specified.
            if ((!exuMask) && iom->dedIoExuMask && (!(iom->dedIoExuMask & AFX_BITMASK(exuIdx))))
                continue; // for
#if 0
            // if a mask is specified and it is not one of the existing dedicated EXUs in mask.
            if (iom->dedIoExuMask)
            {
                if (exuMask)
                {
                    if ((exuMask & iom->dedIoExuMask))
                    {
                        if (!(iom->dedIoExuMask & AFX_BITMASK(exuIdx)))
                            continue;
                    }
                }
                else
                {

                }

                if (exuMask && (iom->dedIoExuMask & AFX_BITMASK(exuIdx)))
                    continue;
            }
#endif

            afxIoBridge exu;
            if (!AfxGetIoBridges(iom, exuIdx, 1, &exu))
            {
                AfxThrowError();
                continue;
            }

            afxError err2 = _AfxExuTransferVideoMemory(exu, ctrl, opCnt, ops);

            err = err2;

            if (!err2)
            {
                queued = TRUE;
                break; // for
            }

            if (err2 == afxError_TIMEOUT || err2 == afxError_BUSY)
                continue; // for

            AfxThrowError();
        }

        // Try again
        if (err == afxError_TIMEOUT || err == afxError_BUSY)
            continue; // while

        if (err || queued)
            break; // while --- find bridges
    }
    return err;
}

_AFX afxError _AfxIomSW_RemapBuffersCb(afxIommu iom, afxBool unmap, afxUnit cnt, _afxBufRemapping const maps[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AfxIomGetIoExuMask(iom, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxIoBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AfxExuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AfxExuRemapBuffers(dexu, unmap, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_AFX afxError _AfxIomSW_CohereMappedBuffersCb(afxIommu iom, afxBool invalidate, afxUnit cnt, afxBufferedMap const maps[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(cnt);
    AFX_ASSERT(maps);

    afxError queErr;
    afxBool queued = FALSE;

    afxMask dedIoExuMask;
    afxMask ioExuMask = _AfxIomGetIoExuMask(iom, &dedIoExuMask);
    afxUnit exuIdx = 0;
    afxIoBridge dexu;
    afxUnit exuCnt;

    // Firstly, try to put them in a dedicated queue.
    if (dedIoExuMask)
    {
        exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, dedIoExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, dedIoExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AfxExuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }

    // If we can not put them in a dedicated queue, try to put them in a shared one.
    if (!queued)
    {
        exuCnt = AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, ioExuMask, 0, 0, NIL);
        AFX_ASSERT(exuCnt);
        exuIdx = 0;
        while (AfxChooseIoBridges(iom, AFX_INVALID_INDEX, afxAptitude_DMA, ioExuMask, exuIdx++, 1, &dexu))
        {
            queErr = _AfxExuCohereMappedBuffers(dexu, invalidate, cnt, maps);
            err = queErr;

            if (!queErr)
            {
                queued = TRUE;
                break; // while
            }

            if (queErr == afxError_TIMEOUT || queErr == afxError_BUSY)
                continue; // while

            AfxThrowError();
        }
    }
    return err;
}

_AFX _afxIomDdi const _AFX_DDI_IOM =
{
    .fencCls = _AfxIomSW_GetFencClassCb,
    .exuCls = _AfxIomSW_GetExuClassCb,
    .bufCls = _AfxIomSW_GetBufClassCb,

    .transferCb = _AfxIomSW_TransferCb,
    .cohereCb = _AfxIomSW_CohereMappedBuffersCb,
    .remapCb = _AfxIomSW_RemapBuffersCb,

    .allocBufCb = _AfxIomSW_AllocateBuffersCb,
    .deallocBufCb = _AfxIomSW_DeallocateBuffersCb,

    .waitFencCb = _AfxIom_WaitForFencesCbSW,
};

_AFX afxModule AfxGetIommuIcd(afxIommu iom)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxModule icd = AfxGetHost(iom);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    return icd;
}

_AFX afxUnit AfxTestForEnabledIommuExtensions(afxIommu iom, afxUnit cnt, afxString const* exts, afxBool enabled[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(exts);
    afxUnit rslt = 0;

    if (enabled)
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++, enabled[i] = TRUE;
            else
                enabled[i] = FALSE;
        }
    }
    else
    {
        afxString const* reqExts;

        afxUnit matchedIdx;
        for (afxUnit i = 0; i < cnt; i++)
        {
            if (AfxCompareStrings(reqExts, 0, FALSE, 1, &exts[i], &matchedIdx))
                rslt++;
        }
    }

    return rslt;
}

_AFX afxUnit AfxResolveIommuSymbols(afxIommu iom, afxUnit cnt, afxString const names[], void* addresses[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    // @names must be an array of Qwadro strings specifying the wanted symbol's names.
    AFX_ASSERT(names);
    // @addresses must be a recipient array for returned function pointers.
    AFX_ASSERT(addresses);
    afxUnit rslt = 0;

    AFX_ASSERT(iom->ddi->getProcCb);
    rslt = iom->ddi->getProcCb(iom, cnt, names, addresses);

    return rslt;
}

_AFX afxUnit AfxGetIoBridges(afxIommu iom, afxUnit baseIdx, afxUnit cnt, afxIoBridge bridges[])
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    afxUnit bridgeCnt = iom->bridgeCnt;
    cnt = AFX_MIN(cnt, bridgeCnt - baseIdx);

    // bridges can be null or must be a valid pointer to afxIoBridge handles.
    if (!bridges) return cnt;

    //AFX_ASSERT_RANGE(bridgeCnt, baseExuIdx, cnt);
    baseIdx = AFX_MIN(baseIdx, bridgeCnt - 1);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxIoBridge dexu = iom->bridges[baseIdx + i];
        AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &dexu);
        bridges[rslt++] = dexu;
    }
    return rslt;
}

_AFX afxError AfxWaitForIoQueue(afxIommu iom, afxUnit64 timeout, afxUnit exuIdx, afxUnit queId)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    
    afxIoBridge dexu;
    // exuIdx must be a valid index to a bridge.
    AFX_ASSERT_RANGE(iom->bridgeCnt, exuIdx, 1);
    if (!AfxGetIoBridges(iom, exuIdx, 1, &dexu))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &dexu);

    afxIoQueue dque;
    if (!AfxGetIoQueues(dexu, queId, 1, &dque))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_XQUE, 1, &dque);

    if (AfxWaitForEmptyIoQueue(dque, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError AfxWaitForIoBridges(afxIommu iom, afxUnit64 timeout, afxMask exuMask)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    afxUnit exuCnt = iom->bridgeCnt;
    for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
    {
        if (exuMask && !(AFX_TEST_BIT_SET(exuMask, exuIdx)))
            continue;

        afxIoBridge dexu;
        if (!AfxGetIoBridges(iom, exuIdx, 1, &dexu))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &dexu);
        AfxWaitForIdleIoBridge(dexu, timeout);
    }
    return err;
}

_AFX afxError AfxWaitForIoIommu(afxIommu iom, afxUnit64 timeout)
{
    afxError err = { 0 };
    // @iom must be a valid afxIommu handle.
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    if (!iom->ddi->waitCb)
    {
        afxUnit exuCnt = iom->bridgeCnt;
        for (afxUnit exuIdx = 0; exuIdx < exuCnt; exuIdx++)
        {
            afxIoBridge dexu;
            if (!AfxGetIoBridges(iom, exuIdx, 1, &dexu))
            {
                AfxThrowError();
                return err;
            }
            AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &dexu);
            AfxWaitForIdleIoBridge(dexu, timeout);
        }
    }
    else if (iom->ddi->waitCb(iom, timeout))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxIomDtorCb(afxIommu iom)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    //AfxWaitForIoIommu(iom, AFX_TIMEOUT_INFINITE);
    //AfxWaitForIoIommu(iom, AFX_TIMEOUT_INFINITE);

    afxUnit bridgeCnt = iom->bridgeCnt;

    AfxDeregisterChainedClasses(&iom->classes);
    AFX_ASSERT(AfxIsChainEmpty(&iom->classes));
#if 0
    AfxAssertObjects(iom->exuCnt, iom->exus, afxFcc_EXU);

    for (afxUnit j = iom->exuCnt; j-- > 0;)
        while (!AfxDisposeObjects(1, &iom->exus[j]));
#endif

    if (iom->bridges)
    {
#if 0
        AFX_TRY_ASSERT_OBJECTS(afxFcc_EXU, bridgeCnt, iom->bridges);

        for (afxUnit i = bridgeCnt; i-- > 0;)
        {
            AfxDisposeObjects(1, &iom->bridges[i]);
        }
#endif
    }

    afxAllocation const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(iom->bridges[0]),
            .var = (void**)&iom->bridges
        }
    };

    if (AfxDeallocateInstanceData(iom, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxIomCtorCb(afxIommu iom, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    //AFX_ASSERT(spec);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(args[1]);
    _afxIomAcq const* cfg = AFX_CAST(_afxIomAcq const*, args[1]) + invokeNo;

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    _afxExuAcq* bridgeCfgs = cfg->bridgeCfg;

    if (!bridgeCfgs)
    {
        AFX_ASSERT(bridgeCfgs);
        AfxThrowError();
        return err;
    }

    iom->ddi = &_AFX_DDI_IOM;
    iom->idd = NIL;
    iom->udd = cfg->cfg.udd;
    iom->tag = cfg->cfg.tag;

    iom->running = FALSE;

    {
        afxChain* classes = &iom->classes;
        AfxMakeChain(classes, (void*)iom);

        // Must be first to be disponible at disposition of IOM' child objects.
        afxClassConfig exuClsCfg = cfg->exuClsCfg ? *cfg->exuClsCfg : _AFX_CLASS_CONFIG_EXU;
        AFX_ASSERT(exuClsCfg.fcc == afxFcc_EXU);
        AfxMountClass(&iom->exuCls, NIL, classes, &exuClsCfg);

        afxClassConfig ctxClsCfg = cfg->ctxClsCfg ? *cfg->ctxClsCfg : _AFX_CLASS_CONFIG_CTX;
        AFX_ASSERT(ctxClsCfg.fcc == afxFcc_CTX);
        AfxMountClass(&iom->ctxCls, NIL, classes, &ctxClsCfg);

        afxClassConfig bufClsCfg = cfg->bufClsCfg ? *cfg->bufClsCfg : _AFX_CLASS_CONFIG_BUF;
        AFX_ASSERT(bufClsCfg.fcc == afxFcc_BUF);
        AfxMountClass(&iom->bufCls, NIL, classes, &bufClsCfg);

    }

    afxUnit totalDqueCnt = 0;
    afxUnit bridgeCnt = cfg->cfg.exuCnt;

    for (afxUnit i = 0; i < bridgeCnt; i++)
    {
        AFX_ASSERT(bridgeCfgs);
        totalDqueCnt += bridgeCfgs[i].minQueCnt;
    }

    iom->bridgeCnt = bridgeCnt;
    afxUnit baseQueIdx = 0;

    afxAllocation const stashes[] =
    {
        {
            .cnt = bridgeCnt,
            .siz = sizeof(iom->bridges[0]),
            .var = (void**)&iom->bridges
        }
    };

    if (AfxAllocateInstanceData(iom, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&iom->classes);
        AFX_ASSERT(AfxIsChainEmpty(&iom->classes));
        return err;
    }

    if (_AfxIomBridgeDevices(iom, iom->bridgeCnt, bridgeCfgs, iom->bridges))
    {
        AfxThrowError();

        AfxDeallocateInstanceData(iom, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&iom->classes);
        AFX_ASSERT(AfxIsChainEmpty(&iom->classes));
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_EXU, iom->bridgeCnt, iom->bridges);

    iom->ioExuMask = NIL;
    iom->dedIoExuMask = NIL;

    for (afxUnit i = 0; i < iom->bridgeCnt; i++)
    {
        afxIoBridge exu;
        AfxGetIoBridges(iom, i, 1, &exu);
        AFX_ASSERT_OBJECTS(afxFcc_EXU, 1, &exu);

        afxDevice dev = AfxGetBridgedIoDevice(exu, NIL);
        AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);

        afxDeviceInfo capsi;
        AfxQueryIoCapabilities(dev, &capsi);

        if ((capsi.capabilities & afxAptitude_DMA) == afxAptitude_DMA)
            iom->ioExuMask |= AFX_BITMASK(i);
    }

    if (!err)
    {
        afxDevice dev = AfxGetBridgedIoDevice(iom->bridges[0], NIL);

        AfxCallDevice((afxDevice)dev, 3, iom);
        AfxCallDevice((afxDevice)dev, 5, iom);

        if (AfxDoDeviceService((afxDevice)dev))
            AfxThrowError(); // let the device build its DPUs.
            
        if (!err)
        {
            
            {
                iom->running = TRUE;
            }
        }
    }

    if (err)
    {
        AfxDeallocateInstanceData(iom, ARRAY_SIZE(stashes), stashes);

        // Dismout the classes and return the error.
        AfxDeregisterChainedClasses(&iom->classes);
        AFX_ASSERT(AfxIsChainEmpty(&iom->classes));
    }

    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_IOM =
{
    .fcc = afxFcc_IOM,
    .name = "Iommu",
    .desc = "I/O System",
    .fixedSiz = sizeof(AFX_OBJECT(afxIommu)),
    .ctor = (void*)_AfxIomCtorCb,
    .dtor = (void*)_AfxIomDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxUnit AfxInvokeIoIommus(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxIommu), afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule mdle;
    if (!_AfxGetIcd(sys, icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AFX) == (afxModuleFlag_ICD | afxModuleFlag_AFX));

    afxClass const* cls = _AfxIcdGetIomClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_IOM);

    rslt = AfxInvokeObjects(cls, (void*)f, udd, first, cnt);

    return rslt;
}

_AFX afxUnit AfxEvokeIoIommus(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxIommu), afxUnit cnt, afxIommu systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule mdle;
    if (!_AfxGetIcd(sys, icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AFX) == (afxModuleFlag_ICD | afxModuleFlag_AFX));

    afxClass const* cls = _AfxIcdGetIomClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_IOM);

    rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, rslt, systems);

    return rslt;
}

_AFX afxUnit AfxEnumerateIoIommus(afxUnit icd, afxUnit first, afxUnit cnt, afxIommu systems[])
{
    afxError err = { 0 };
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxModule mdle;
    if (!_AfxGetIcd(sys, icd, &mdle))
    {
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
    AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AFX) == (afxModuleFlag_ICD | afxModuleFlag_AFX));

    afxClass const* cls = _AfxIcdGetIomClass(mdle);
    AFX_ASSERT_CLASS(cls, afxFcc_IOM);

    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, rslt, systems);

    return rslt;
}

_AFX afxError AfxConfigureIoIommu(afxUnit icd, afxIommuConfig* cfg)
{
    afxError err = { 0 };

    if (!cfg)
    {
        AFX_ASSERT(cfg);
        AfxThrowError();
        return err;
    }

    afxIommu sys;
    AfxGetIommu(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    if (icd != AFX_INVALID_INDEX)
    {
        afxModule drv;
        AFX_ASSERT(icd != AFX_INVALID_INDEX);
        if (!_AfxGetIcd(sys, icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD | afxModuleFlag_AFX));
    }

    afxAptitude caps = cfg->caps;
    afxAcceleration accel = cfg->accel;

    if (0 == cfg->exuCnt)
    {
        cfg->exuCnt = 0;

        for (afxUnit i = 0; i < AFX_MAX_BRIDGES; i++)
        {
            afxUnit devId = i;
            
            afxDevice dev;
            if (AfxEnumerateIoDevices(icd, devId, 1, &dev))
            {
                AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);

                afxDeviceInfo capsi;
                AfxQueryIoCapabilities(dev, &capsi);

                if (caps && !(caps & capsi.capabilities))
                    continue;

                if (accel && !(accel & capsi.acceleration))
                    continue;

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].devId = devId;
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
            afxDeviceInfo capsi = { 0 };
            capsi.acceleration = cfg->exus[i].acceleration ? cfg->exus[i].acceleration : accel;
            capsi.capabilities = cfg->exus[i].capabilities ? cfg->exus[i].capabilities : caps;
            capsi.minQueCnt = cfg->exus[i].minQueCnt;

            afxUnit devId;
            if (AfxChooseIoDevices(icd, &capsi, NIL, NIL, 1, &devId))
            {
                afxDevice dev;
                AfxEnumerateIoDevices(icd, devId, 1, &dev);
                AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &dev);

                AfxQueryIoCapabilities(dev, &capsi);

                cfg->exus[cfg->exuCnt].capabilities = capsi.capabilities;
                cfg->exus[cfg->exuCnt].acceleration = capsi.acceleration;
                cfg->exus[cfg->exuCnt].devId = devId;
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

_AFX afxError AfxEstablishIoIommu(afxUnit icd, afxIommuConfig const* cfg, afxIommu* system)
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

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);

    afxClass* iomCls = NIL;
    afxModule drv = NIL;

    if (icd != AFX_INVALID_INDEX)
    {
        if (!_AfxGetIcd(sys, icd, &drv))
        {
            AfxThrowError();
            return err;
        }
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
        AFX_ASSERT(AfxTestModule(drv, afxModuleFlag_ICD /*| afxModuleFlag_AFX*/));

        iomCls = (afxClass*)_AfxIcdGetIomClass(drv);
        AFX_ASSERT_CLASS(iomCls, afxFcc_IOM);
    }
    else
    {
        static afxBool inited = FALSE;
        static afxClass staticIomCls = { 0 };

        if (!inited)
        {
            AfxMountClass(&staticIomCls, NIL, NIL, &_AFX_CLASS_CONFIG_IOM);
            inited = TRUE;
        }

        iomCls = &staticIomCls;
        AFX_ASSERT_CLASS(iomCls, afxFcc_IOM);
    }

    // Acquire bridges and queues
    afxUnit totalDqueCnt = 0;
    afxUnit baseQueIdx[AFX_MAX_BRIDGES] = { 0 };
    _afxExuAcq bridgeCfg[AFX_MAX_BRIDGES] = { 0 };
    afxUnit bridgeCnt = 0;

    AFX_ASSERT_RANGE(AFX_MAX_BRIDGES, 0, cfg->exuCnt);

    for (afxUnit i = 0; i < cfg->exuCnt; i++)
    {
        afxIoBridgeConfig const* exuCfg = &cfg->exus[i];

        afxUnit bridgeIdx = AFX_INVALID_INDEX;

#if 0 // disable it to allow multibridge to same device port
        for (afxUnit j = 0; j < bridgeCnt; j++)
        {
            if (cfg->auxs[j].portId == bridgeCfg[j].portId)
            {
                bridgeCfg[bridgeIdx].minQueCnt += cfg->auxs[i].minQueCnt;
                bridgeCfg[bridgeIdx].queuePriority = NIL;
                //bridgeCfg[bridgeIdx].flags |= cfg->auxBridges[i].flags;

                baseQueIdx[bridgeIdx] = totalDqueCnt;
                totalDqueCnt += bridgeCfg[bridgeIdx].minQueCnt;
                break;
            }
        }
#endif

        // If not found, enlist this bridge it.
        if (bridgeIdx != AFX_INVALID_INDEX)
            continue;

        afxDevice dev;
        if (!AfxEnumerateIoDevices(icd, exuCfg->devId, 1, &dev))
        {
            AfxThrowError();
            break;
        }
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_DEV, 1, &dev);

            if (AfxCallDevice((afxDevice)dev, 1, NIL))
            {
                AfxThrowError(); // let the device build its DPUs.
                break;
            }
        }

        bridgeCfg[bridgeCnt].dev = dev;

        afxUnit minQueCnt = AFX_CLAMP(exuCfg->minQueCnt, 1, AFX_MAX_QUEUES_PER_BRIDGE);
        afxDeviceInfo capsi2 = { 0 };
        capsi2.capabilities = exuCfg->capabilities;
        capsi2.acceleration = exuCfg->acceleration;
        capsi2.minQueCnt = minQueCnt;

        bridgeCfg[bridgeCnt].exuIdx = bridgeCnt;
        bridgeCfg[bridgeCnt].minQueCnt = minQueCnt;
        bridgeCfg[bridgeCnt].xqueClsCfg = &_AFX_CLASS_CONFIG_XQUE;
        //bridgeCfg[bridgeCnt].dctxClsCfg = &_AFX_CLASS_CONFIG_CTX;
        ++bridgeCnt;
    }

    if (err)
    {
        return err;
    }

    _afxIomAcq cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.cfg.exuCnt = bridgeCnt;
    //cfg2.cfg.extCnt = cfg->extCnt;
    //cfg2.cfg.exts = cfg->exts;
    cfg2.cfg.udd = cfg->udd;
    cfg2.cfg.tag = cfg->tag;

    cfg2.bridgeCfg = &bridgeCfg[0];

    AFX_ASSERT_CLASS(iomCls, afxFcc_IOM);

    afxIommu iom;
    if (AfxAcquireObjects(iomCls, 1, (afxObject*)&iom, (void const*[]) { drv, &cfg2 }))
    {
        AfxThrowError();
        return err;
    }
    
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    *system = iom;

    return err;
}
