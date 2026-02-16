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

#define _AMX_MIX_C
#define _AMX_MIX_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AMX_MIX_DEVICE_C
#define _AMX_MIX_SYSTEM_C
#define _AMX_SINK_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_TRACK_C
#define _AMX_AUDIO_C
#define _AMX_PUMP_C
#include "amxIcd.h"

_AMX afxError AmxDescribePump(amxPump pmp, amxPumpInfo* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(info);



    return err;
}

_AMX afxError AmxDescribePumpStreams(amxPump pmp, afxUnit first, afxUnit cnt, amxPumpStreamInfo infos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(infos);



    return err;
}

_AMX afxError AmxFormatPumpStreams(amxPump pmp, afxUnit first, afxUnit cnt, amxPumpStreamInfo infos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(infos);



    return err;
}

/*
    Pump In:
    In software, this could refer to feeding data into a system or process. For example, data might be "pumped in" from an external source, 
    like an API call, or from a database into an application for processing. It might also refer to injecting dependencies into an 
    application or system—such as adding configurations or resources dynamically.

    Pump Out:
    This would refer to outputting or sending data out of a system. For instance, it could mean sending the result of a computation, 
    a response to a request, or even logging data for external monitoring. In a web server context, you might "pump out" responses to 
    client requests, such as HTML, JSON, or other data.

    Pump Up:
    This could refer to increasing the intensity or capacity of a certain resource. For instance, it might refer to boosting performance 
    by adding more computational resources or increasing the power of a feature (e.g., "pumping up" a machine learning model by adding 
    more training data). It could also refer to increasing the priority or speed of certain tasks—like "pumping up" the frequency of 
    updates in a real-time system.

    Pump Down:
    This could mean scaling down or reducing certain resources or capacities, like reducing the load on a server or scaling back a 
    service's execution to free up resources. In terms of system processes, "pumping down" could be used for lowering priority on 
    background tasks or reducing resource consumption to conserve power or memory. Another possible meaning might be reducing pressure 
    in terms of system load or reducing the rate of data intake/output.
*/

_AMX afxError AmxPumpIn(amxPump pmp, amxPacket* pkt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(pkt);



    return err;
}

_AMX afxError AmxPumpOut(amxPump pmp, amxPacket* pkt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(pkt);



    return err;
}

_AMX afxError AmxPumpUp(amxPump pmp, amxPacket* pkt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(pkt);



    return err;
}

_AMX afxError AmxPumpDown(amxPump pmp, amxPacket* pkt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);
    AFX_ASSERT(pkt);



    return err;
}

_AMX afxError _AmxPmpDtorCb(amxPump pmp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);



    return err;
}

_AMX afxError _AmxPmpCtorCb(amxPump pmp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxPumpConfig const* cfg = ((amxPumpConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);



    return err;
}

_AMX afxClassConfig const _AMX_MSRC_CLASS_CONFIG =
{
    .fcc = afxFcc_MSRC,
    .name = "Pump",
    .desc = "Media I/O Pump",
    .fixedSiz = sizeof(AFX_OBJECT(amxPump)),
    .ctor = (void*)_AmxPmpCtorCb,
    .dtor = (void*)_AmxPmpDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireInputPumps(afxMixSystem msys, afxUnit cnt, amxPumpConfig const cfgs[], amxPump pumps[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(pumps);

    afxClass* cls = (afxClass*)_AmxMsysGetMsrcClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MSRC);
    
    if (AfxAcquireObjects(cls, cnt, (afxObject*)pumps, (void const*[]) { msys, cfgs }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSRC, cnt, pumps);

    return err;
}

_AMX afxError AmxAcquireOutputPumps(afxMixSystem msys, afxUnit cnt, amxPumpConfig const cfgs[], amxPump pumps[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(pumps);

    afxClass* cls = (afxClass*)_AmxMsysGetMsrcClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_MSRC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pumps, (void const*[]) { msys, cfgs }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MSRC, cnt, pumps);

    return err;
}

_AMX afxError AmxAcquirePumpFromFile(afxMixSystem msys, afxUri const* uri, amxPump* pump)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(pump);
    AFX_ASSERT(uri);

    return err;
}

_AMX afxError AmxAssumeMediaStream(amxPump pmp, afxUnit capstan, afxStream iob, afxSize iobBase)
{

}
