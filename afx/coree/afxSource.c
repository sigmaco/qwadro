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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#include "afxIoDDK.h"


_AFXINL void AfxMakeSource(afxSource *src)
{
    //AfxAssignFcc(src, afxFcc_SRC);
    src->extlSrcCookie = 0;
    src->cdcId = 0;
    AfxResetUris(1, &src->uri);
    src->buf = NIL;
    src->siz = 0;
    src->file = NIL;
}

_AFXINL void AfxMakeSourceFromMemory(afxSource *src, void* mem, afxUnit32 siz, afxUnit32 extlSrcCookie, afxUnit32 idCodec)
{
    //AfxAssignFcc(src, afxFcc_SRC);

    // specify source by memory.
    src->extlSrcCookie = extlSrcCookie;
    src->cdcId = idCodec;
    AfxResetUris(1, &src->uri);
    src->buf = mem;
    src->siz = siz;
    src->file = NIL;
}

_AFXINL void AfxMakeSourceFromUri(afxSource *src, afxUri const* uri, afxUnit extlSrcCookie, afxUnit32 idCodec)
{
    //AfxAssignFcc(src, afxFcc_SRC);

    // specify source by streaming file name.
    src->extlSrcCookie = extlSrcCookie;
    src->cdcId = idCodec;
    AfxReflectUri(&src->uri, uri);
    src->buf = NIL;
    src->siz = 0;
    src->file = NIL;
}

_AFXINL void AfxMakeSourceFromFile(afxSource *src, afxFile file, afxUnit32 extlSrcCookie, afxUnit32 idCodec)
{
    //AfxAssignFcc(src, afxFcc_SRC);

    // specify source by streaming file ID.
    src->extlSrcCookie = extlSrcCookie;
    src->cdcId = idCodec;
    AfxResetUris(1, &src->uri);
    src->buf = NIL;
    src->siz = 0;
    src->file = file;
}

_AFXINL void AfxMakeSourceFromStream(afxSource *src, afxStream iob, afxUnit32 offset, afxUnit32 range)
{
    //AfxAssignFcc(src, afxFcc_SRC);

    // specify source by streaming file ID.
    src->extlSrcCookie = 0;
    src->cdcId = 0;
    AfxResetUris(1, &src->uri);
    src->buf = NIL;
    src->siz = range;
    src->file = NIL;
    src->iob = iob;
    src->offset = offset;
}
