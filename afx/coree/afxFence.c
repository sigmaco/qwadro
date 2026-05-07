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
#define _AFX_FENCE_C
#include "afxSystemDDK.h"

_AFX afxIommu AfxGetFenceHost(afxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxIommu iom = AfxGetHost(fenc);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    return iom;
}

_AFX afxUnit64 _AfxFencSW_GetValueCb(afxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxUnit64 value = (afxUnit64)AfxLoadAtom64(&fenc->value);

    return value;
}

_AFX afxUnit64 AfxGetFenceValue(afxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    AFX_ASSERT(fenc->ddi->evalCb);
    afxUnit64 value = fenc->ddi->evalCb(fenc);

    // This method mimmicks the D3D12's one.

    return value;
}

_AFX afxError _AfxFencSW_SignalCb(afxFence fenc, afxUnit64 value)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    if (fenc->flags & afxFenceFlag_TIMELINE)
    {
        AFX_ASSERT(value > (afxUnit64)AfxLoadAtom64(&fenc->value));
        AfxStoreAtom64(&fenc->value, (afxInt64)value);
        //AfxIncAtom64(&fenc->value);
    }
    else
    {
        AfxStoreAtom64(&fenc->value, (afxInt64)value);
    }
    return err;
}

_AFX afxError AfxSignalFence(afxFence fenc, afxUnit64 value)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    AFX_ASSERT(fenc->ddi->signalCb);
    err = fenc->ddi->signalCb(fenc, value);
    
    // Also queued on D3D12
    // ID3D12Fence::Signal. Use this method to set a fence value from the CPU side.
    // Use ID3D12CommandQueue::Signal to set a fence from the GPU side.
    
    return err;
}

_AFX afxError _AfxFencSW_WaitCb(afxFence fenc, afxUnit64 value, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxIommu iom = AfxGetFenceHost(fenc);
    afxError err2 = AfxWaitForFences(iom, timeout, TRUE, 1, &fenc, &value);

    if (err2 != afxError_TIMEOUT)
    {
        AfxThrowError();
        err = err2;
    }

    return err;
}

_AFX afxError AfxWaitForFence(afxFence fenc, afxUnit64 value, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    AFX_ASSERT(fenc->ddi->waitCb);
    err = fenc->ddi->waitCb(fenc, value, timeout);

    return err;
}

_AFX _afxDdiFenc const _AFX_FENC_DDI =
{
    .waitCb = _AfxFencSW_WaitCb,
    .signalCb = _AfxFencSW_SignalCb,
    .evalCb = _AfxFencSW_GetValueCb,
};

_AFX afxError _AfxFencDtorCb(afxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    
    return err;
}

_AFX afxError _AfxFencCtorCb(afxFence fenc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxIommu iom = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    afxFenceInfo const* info = ((afxFenceInfo const*)args[1]) + invokeNo;

    fenc->waitQueuedCnt = 0;
    fenc->signalQueuedCnt = 0;
    fenc->signaled = info->initialVal;
    fenc->value = info->initialVal;
    fenc->flags = info->flags;

    fenc->tag = info->tag;
    fenc->udd = info->udd;

    fenc->ddi = &_AFX_FENC_DDI;

    return err;
}

_AFX afxClassConfig const _AFX_CLASS_CONFIG_FENC =
{
    .fcc = afxFcc_FENC,
    .name = "Fence",
    .desc = "Device-Synchronization Fence",
    .fixedSiz = sizeof(AFX_OBJECT(afxFence)),
    .ctor = (void*)_AfxFencCtorCb,
    .dtor = (void*)_AfxFencDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireFences(afxIommu iom, afxUnit cnt, afxFenceInfo const info[], afxFence fences[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);
    AFX_ASSERT(cnt);
    AFX_ASSERT(fences);

    afxClass* cls = (afxClass*)_AfxIomGetDdi(iom)->fencCls(iom);
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { iom, info }))
    {
        AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxIomSW_WaitForFencesCb(afxIommu iom, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, afxFence const fences[], afxUnit64 const values[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    afxClock start, last;
    if (timeout && (timeout != AFX_TIMEOUT_INFINITE))
    {
        AfxGetClock(&start);
        last = start;
    }

    if (!waitAll || (cnt == 1))
    {
        while (1)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxFence dfen = fences[i];
                if (!dfen) continue;
                AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

                if (dfen->flags & afxFenceFlag_TIMELINE)
                {
                    AFX_ASSERT(values);
                    // vkWaitSemaphores waits until the semaphore value is greater than or equal to the specified value.
                    if (values[i] <= (afxUnit64)AfxLoadAtom64(&dfen->value))
                        return err;
                }
                else if (AfxLoadAtom32(&dfen->signaled))
                    return err;
            }

            if (timeout)
            {
                if ((timeout == AFX_TIMEOUT_INFINITE) || (timeout == AFX_TIMEOUT_INFINITE))
                    continue;

                afxClock curr;
                AfxGetClock(&curr);
                afxInt64 dt = AfxGetUltrasecondsElapsed(&last, &curr);
                last = curr;

                if (timeout < dt)
                {
                    err = afxError_TIMEOUT;
                    return err;
                }
            }
        }
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxFence dfen = fences[i];
            if (!dfen) continue;
            AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

            while (1)
            {
                if (dfen->flags & afxFenceFlag_TIMELINE)
                {
                    AFX_ASSERT(values);
                    // vkWaitSemaphores waits until the semaphore value is greater than or equal to the specified value.
                    if (values[i] <= (afxUnit64)AfxLoadAtom64(&dfen->value))
                        break;
                }
                else if (AfxLoadAtom32(&dfen->signaled))
                    break;

                if (timeout)
                {
                    if ((timeout == AFX_TIMEOUT_INFINITE) || (timeout == AFX_TIMEOUT_INFINITE))
                        continue;

                    afxClock curr;
                    AfxGetClock(&curr);
                    afxInt64 dt = AfxGetUltrasecondsElapsed(&last, &curr);
                    last = curr;

                    if (timeout < dt)
                    {
                        err = afxError_TIMEOUT;
                        return err;
                    }
                }
            }
        }
    }

    return err;
}

_AFX afxError AfxWaitForFences(afxIommu iom, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, afxFence const fences[], afxUnit64 const values[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    if (_AfxIomGetDdi(iom)->waitFencCb(iom, timeout, waitAll, cnt, fences, values))
    {
        AfxThrowError();
    }
    return err;
}

_AFX afxError AfxResetFences(afxIommu iom, afxUnit cnt, afxFence const fences[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_IOM, 1, &iom);

    AfxThrowError();
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxFence dfen = fences[i];
        if (!dfen) continue;
        AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

        // Does the new synchronization primitive allow resetting its payload?
        // No, allowing the payload value to “go backwards” is problematic. 
        // Applications looking for reset behavior should create a new instance of the synchronization primitive instead.
        if (!(dfen->flags & afxFenceFlag_TIMELINE))
        {
            AfxStoreAtom64(&dfen->signaled, 0);
        }
    }
#endif
    //if (iom->resetFencCb(iom, cnt, fences))
        //AfxThrowError();

    return err;
}
