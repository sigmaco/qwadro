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

_ZAL afxError _ZalTraxDtorCb(amxTracker trax)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSES, 1, &trax);

    afxMixSystem msys = AfxGetHost(trax);

    if (_AMX_TRAX_CLASS_CONFIG.dtor(trax))
    {
        AfxThrowError();
    }

    return err;
}

_ZAL afxError _ZalTraxCtorCb(amxTracker trax, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSES, 1, &trax);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxTrackerConfig const* cfg = ((amxTrackerConfig const *)args[1]) + invokeNo;

    if (_AMX_TRAX_CLASS_CONFIG.ctor(trax, (void*[]) { msys, (void*)cfg }, 0))
    {
        AfxThrowError();
        return err;
    }
    
    return err;
}
