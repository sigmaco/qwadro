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

#include "zalObjects.h"
#include "zalInteropWasapi.h"

_ZAL afxError _ZalMsysDtorCb(afxMixSystem msys)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    _AMX_MSYS_CLASS_CONFIG.dtor(msys);

    return err;
}

_ZAL afxError _ZalMsysCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    __amxMsysAcq const* cfg = AFX_CAST(__amxMsysAcq const*, args[1]) + invokeNo;
    _amxMexuAcq* bridgeCfgs = AFX_CAST(_amxMexuAcq*, args[2]) + invokeNo;

    __amxMsysAcq cfg2 = *cfg;
#if 0
    afxClassConfig asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
    asioClsCfg.fixedSiz = sizeof(AFX_OBJ(afxSink));
    asioClsCfg.ctor = (void*)_ZalAsioCtorCb;
    asioClsCfg.dtor = (void*)_ZalAsioDtorCb;

    cfg2.sinkClsCfg = &asioClsCfg;
#endif
    if (_AMX_MSYS_CLASS_CONFIG.ctor(msys, (void*[]) { icd, &cfg2, bridgeCfgs }, 0)) AfxThrowError();
    else
    {

    }
    return err;
}
