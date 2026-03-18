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
#define _AMX_VIDEO_C
#include "amxIcd.h"

/*
    2026.01.10. Tive um sonho onde estava para acontecer algo que fez com que meus colegas
    de escola estivessem se arrumando para congregar naquele lugar, porém, como sempre, 
    eu estava postergando me prontificar, e aparentemente esse é um dos pontos importantes desse ato.
    Pois quando me toquei disso, e que era um hábito, o sonho foi encerrado.
*/

_AMX afxError AmxBindVideoPicture(amxVideo vid, afxUnit bufIdx, avxRaster pic)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    if (vid->bufs[bufIdx].pic)
        AfxDisposeObjects(1, &vid->bufs[bufIdx].pic);

    vid->bufs[bufIdx].pic = pic;

    if (pic)
        AfxReacquireObjects(1, &pic);

    return err;
}

_AMX afxError _AmxVidDtorCb(amxVideo vid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = AfxGetHost(vid);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);

    if (vid->canv)
        AfxDisposeObjects(1, &vid->canv);

    return err;
}

_AMX afxError _AmxVidCtorCb(amxVideo vid, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VID, 1, &vid);

    afxMixSystem msys = AfxGetHost(vid);
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    amxVideoConfig const *cfg = ((amxVideoConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    vid->chromaBitDepth = cfg->chromaBitDepth;
    vid->codec = cfg->codec;
    vid->dpbCap = cfg->dpbCap;
    vid->exuMask = cfg->exuMask;
    vid->flags = cfg->flags;
    vid->lumaBitDepth = cfg->lumaBitDepth;
    vid->maxActiveRefPics = cfg->maxActiveRefPics;
    vid->maxCodedWh[0] = cfg->maxCodedWh[0];
    vid->maxCodedWh[1] = cfg->maxCodedWh[1];
    vid->picFmt = cfg->picFmt;
    vid->refPicFmt = cfg->refPicFmt;
    vid->subsampling[0] = cfg->subsampling[0];
    vid->subsampling[1] = cfg->subsampling[1];
    vid->subsampling[2] = cfg->subsampling[2];
    vid->usage = cfg->usage;

    afxDrawSystem dsys = _AmxMsysGetDsys(msys);

    avxCanvas canv;
    avxCanvasConfig ccfg = { 0 };
    ccfg.whd.w = vid->maxCodedWh[0];
    ccfg.whd.h = vid->maxCodedWh[1];
    ccfg.udd = vid;
    ccfg.tag = AFX_STRING("video");
    AvxConfigureCanvas(dsys, &ccfg);
    if (AvxAcquireCanvas(dsys, &ccfg, 1, &canv))
    {
        AfxThrowError();
    }

    vid->canv = canv;

    return err;
}

_AMX afxClassConfig const _AMX_VID_CLASS_CONFIG =
{
    .fcc = afxFcc_VID,
    .name = "Video",
    .desc = "Video",
    .fixedSiz = sizeof(AFX_OBJECT(amxVideo)),
    .ctor = (void*)_AmxVidCtorCb,
    .dtor = (void*)_AmxVidDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AmxAcquireVideos(afxMixSystem msys, afxUnit cnt, amxVideoConfig const cfg[], amxVideo videos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(videos);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetVidClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_VID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)videos, (void const*[]) { msys, (void*)cfg }))
        AfxThrowError();

    return err;
}

_AMX afxError AmxAcquireVideoEncoders(afxMixSystem msys, afxUnit cnt, amxVideoEncodeConfig const cfg[], amxVideo videos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(videos);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetVidClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_VID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)videos, (void const*[]) { msys, (void*)cfg }))
        AfxThrowError();

    return err;
}

_AMX afxError AmxAcquireVideoDecoders(afxMixSystem msys, afxUnit cnt, amxVideoDecodeConfig const cfg[], amxVideo videos[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MSYS, 1, &msys);
    AFX_ASSERT(videos);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxMsysGetVidClass(msys);
    AFX_ASSERT_CLASS(cls, afxFcc_VID);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)videos, (void const*[]) { msys, (void*)cfg }))
        AfxThrowError();

    return err;
}
