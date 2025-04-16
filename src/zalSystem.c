/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "zalObjects.h"
#include "zalInteropWasapi.h"

_ZAL afxError _ZalMsysDtorCb(afxMixSystem msys)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (_ZalWasapiDestroy(&msys->wasapi))
        AfxThrowError();

    _AMX_MSYS_CLASS_CONFIG.dtor(msys);

    return err;
}

_ZAL afxError _ZalMsysCtorCb(afxMixSystem msys, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _amxMsysAcquisition const* cfg = AFX_CAST(_amxMsysAcquisition const*, args[1]) + invokeNo;
    _amxMexuAcquisition* bridgeCfgs = AFX_CAST(_amxMexuAcquisition*, args[2]) + invokeNo;

    _amxMsysAcquisition cfg2 = *cfg;

    afxClassConfig asioClsCfg = _AMX_ASIO_CLASS_CONFIG;
    asioClsCfg.fixedSiz = sizeof(AFX_OBJ(afxSink));
    asioClsCfg.ctor = (void*)_ZalAsioCtorCb;
    asioClsCfg.dtor = (void*)_ZalAsioDtorCb;

    cfg2.sinkClsCfg = &asioClsCfg;
    
    if (_AMX_MSYS_CLASS_CONFIG.ctor(msys, (void*[]) { icd, &cfg2, bridgeCfgs }, 0)) AfxThrowError();
    else
    {
        if (_ZalWasapiCreate(&msys->wasapi))
            AfxThrowError();
    }
    return err;
}
