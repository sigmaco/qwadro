/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions.

#ifndef AMX_VIDEO_H
#define AMX_VIDEO_H

#include "qwadro/mix/afxMixDefs.h"

typedef enum amxVideoFlag
{
    amxVideoFlag_DECODER    = AFX_BITMASK(0),
    amxVideoFlag_ENCODER    = AFX_BITMASK(1),
} amxVideoFlags;

typedef enum amxVideoUsage
{
    amxVideoUsage_DECODE    = AFX_BITMASK(0), // used as a texture by DSYS
    amxVideoUsage_ENCODE    = AFX_BITMASK(1),
    amxVideoUsage_COMPOSE   = AFX_BITMASK(2),
} amxVideoUsage;

AFX_DEFINE_STRUCT(amxVideoConfig)
{
    afxMask         exuMask;
    afxFlags        flags;
    amxFormat       picFmt;
    afxUnit         maxCodedWh[2];
    amxFormat       refPicFmt;
    // The maximum number of DPB slots.
    afxUnit         dpbCap;
    afxUnit         maxActiveRefPics;

    // video profile
    afxString       codec;
    amxVideoUsage   usage;
    afxUnit         subsampling[3]; // 4:2:0, 4:2:2, 4:4:4 --- nothing means monochrome.
    afxUnit         lumaBitDepth; // 8, 10, 12
    afxUnit         chromaBitDepth; // 8, 10, 12
};

AFX_DEFINE_STRUCT(amxVideoDecodeConfig)
{
    afxString codec;
};

AFX_DEFINE_STRUCT(amxVideoEncodeConfig)
{
    afxString codec;
};

AMX afxError AmxAcquireVideos
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxVideoConfig const cfg[], 
    amxVideo videos[]
);

AMX afxError AmxAcquireVideoDecoders
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxVideoDecodeConfig const cfg[], 
    amxVideo videos[]
);

AMX afxError AmxAcquireVideoEncoders
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxVideoEncodeConfig const cfg[], 
    amxVideo videos[]
);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxBindVideoSamples(amxVideo vid, afxUnit cnt, avxBufferedMap const maps[]);
AMX afxError AmxBindVideoBuffer(amxVideo vid, avxBuffer buf, afxSize offset, afxSize range);
AMX afxError AmxBindVideoPicture(amxVideo vid, afxUnit bufIdx, avxRaster ras);

// Decompresses the next frame of video on a background thread.
AMX afxError AmxDoVideoFrame(amxVideo vid, afxMask exuMask, afxFlags flags);
// Checks on the status of a background video frame decompression.
AMX afxError AmxWaitVideoFrame(amxVideo vid, afxUnit64 timeout, afxFlags flags);
// Moves to the next video frame in the video file.
AMX afxError AmxAdvanceVideo(amxVideo vid, afxFlags flags);

#ifdef _AMX_VIDEO_C
#ifdef _AMX_VIDEO_IMPL
AFX_OBJECT(_amxVideo)
#else
AFX_OBJECT(amxVideo)
#endif
{
    afxMask         exuMask;
    afxFlags        flags;
    amxFormat       picFmt;
    afxUnit         maxCodedWh[2];
    amxFormat       refPicFmt;
    // The maximum number of DPB slots.
    afxUnit         dpbCap;
    afxUnit         maxActiveRefPics;

    // video profile
    afxString       codec;
    amxVideoUsage   usage;
    afxUnit         subsampling[3]; // 4:2:0, 4:2:2, 4:4:4 --- nothing means monochrome.
    afxUnit         lumaBitDepth; // 8, 10, 12
    afxUnit         chromaBitDepth; // 8, 10, 12

    avxCanvas canv;

    struct
    {
        avxBuffer   buf;
        afxSize     bufOffset;
        afxSize     bufSize;
        amxBuffer   mbuf;
        afxSize     mbufOffset;
        afxSize     mbufSize;
        avxRaster   pic;
    } bufs[2];
    afxUnit bufCnt;
};
#endif//_AMX_VIDEO_C

#endif//AMX_VIDEO_H
