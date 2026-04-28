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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAIOR DA SEGURIDADE
 *                                 SIGMA TECHNOLOGY GROUP
 *                                        ENGITECH
 */

// This code is part of SIGMA Future Storage.

/**
    The afxSource structure allows the game to provide files to fill the external sources.
    You can specify a streaming file or a file in-memory, regardless of the "Stream" option.
    Make sure that only one of siz, buf or file is non-null. if both file and path are set, idFile is passed to low-level IO and path is used as stream name (for profiling purposes).
    When using the in-memory file (buf and siz), it is the responsibility of the application to ensure the memory stays valid for the entire duration of the consumption.
*/

#ifndef AFX_SOURCE_H
#define AFX_SOURCE_H

#include "qwadro/base/afxUri.h"

AFX_DEFINE_STRUCT(afxSource)
{
    // Cookie identifying the source, given by hashing the name of the source given in the project.
    afxUnit32   extlSrcCookie;
    // Codec ID for the file.
    afxUnit32   cdcId;
    // File path for the source. If not NULL, the source will be streaming from disk. Set buf to NULL. If file is set, this field is used as stream name (for profiling purposes).
    afxUri      uri; 
    // Pointer to the in-memory file.  If not NULL, the source will be read from memory. Set szFile and idFile to NULL.
    void*       buf;
    // Size of the data pointed by buf.
    afxUnit32   siz;
    // File ID.  If not zero, the source will be streaming from disk.  This ID can be anything.  Note that you must override the low-level IO to resolve this ID to a real file.  See \ref streamingmanager_lowlevel for more information on overriding the Low Level IO.
    afxFile     file;
    afxStream   iob;
    afxUnit32   offset;
};

AFXINL void AfxMakeSource(afxSource* src);

AFXINL void AfxMakeSourceFromMemory(afxSource* src, void* mem, afxUnit32 siz, afxUnit32 extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromUri(afxSource* src, afxUri const* uri, afxUnit extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromFile(afxSource* src, afxFile file, afxUnit32 extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromStream(afxSource* src, afxStream iob, afxUnit32 offset, afxUnit32 range);

#endif//AFX_SOURCE_H
