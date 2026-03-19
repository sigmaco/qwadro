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
//#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include "zalInteropWasapi.h"
#include <float.h>

_ZAL afxError _ZalMexuDtorCb(afxMixBridge mexu)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = AmxGetBridgedMixSystem(mexu, NIL);

    if (_AMX_MEXU_CLASS_CONFIG.dtor(mexu))
        AfxThrowError();



    return err;
}

_ZAL afxError _ZalMexuCtorCb(afxMixBridge mexu, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MEXU, 1, &mexu);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    _amxMexuAcq const* cfg = AFX_CAST(_amxMexuAcq const*, args[1]) + invokeNo;

    if (_AMX_MEXU_CLASS_CONFIG.ctor(mexu, (void*[]) { msys, (void*)cfg }, 0))
    {
        AfxThrowError();
        return err;
    }

    if (err)
    {
        _AMX_MEXU_CLASS_CONFIG.dtor(mexu);
    }

    return err;
}
