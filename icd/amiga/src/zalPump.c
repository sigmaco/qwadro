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

_ZAL afxError _ZalMpmpDtorCb(amxPump pmp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);

    afxMixSystem msys = AfxGetHost(pmp);

    if (_AMX_MSRC_CLASS_CONFIG.dtor(pmp))
    {
        AfxThrowError();
    }

    return err;
}

_ZAL afxError _ZalMpmpCtorCb(amxPump pmp, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSRC, 1, &pmp);

    afxMixSystem msys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &pmp);
    amxPumpConfig const* cfg = ((amxPumpConfig const *)args[1]) + invokeNo;

    if (_AMX_MSRC_CLASS_CONFIG.ctor(pmp, (void*[]) { msys, (void*)cfg }, 0))
    {
        AfxThrowError();
        return err;
    }
    
    return err;
}
