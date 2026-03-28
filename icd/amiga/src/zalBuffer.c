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

#include "zalInteropWasapi.h"
#include "zalObjects.h"

_ZAL afxError _ZalMbufDtorCb(amxBuffer mbuf)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    afxMixSystem msys = AfxGetHost(mbuf);

    if (mbuf->m.storage[0].mapRange)
    {
        AmxUnmapBuffer(mbuf, TRUE);
        AFX_ASSERT(!mbuf->m.storage[0].mapRange);
    }

    if (_AMX_MBUF_CLASS_CONFIG.dtor(mbuf))
        AfxThrowError();

    return err;
}

_ZAL afxError _ZalMbufCtorCb(amxBuffer mbuf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MBUF, 1, &mbuf);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxBufferInfo const *spec = ((amxBufferInfo const *)args[1]) + invokeNo;

    if (_AMX_MBUF_CLASS_CONFIG.ctor(mbuf, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }



    return err;
}
