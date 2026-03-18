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


#include "qowBase.h"

_QOW afxError _QowThemDtorCb(afxTheme them)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THEM, 1, &them);

    if (_AUX_THEM_CLASS_CONFIG.dtor(them))
        AfxThrowError();

    return err;
}

_QOW afxError _QowThemCtorCb(afxTheme them, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THEM, 1, &them);

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AFX_ASSERT(args[1]);
    //afxFontConfig const* cfg = AFX_CAST(afxFontConfig const*, args[1]) + invokeNo;

    if (_AUX_THEM_CLASS_CONFIG.ctor(them, args, invokeNo))
    {
        AfxThrowError();
        return err;
    }
    
    return err;
}
