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
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Future Storage.

#ifndef AFX_STORAGE_DDK_H
#define AFX_STORAGE_DDK_H

#include "qwadro/afxSystem.h"

#ifdef _AFX_STORAGE_C

AFX_DEFINE_STRUCT(_afxFsysUnit)
{
    afxLink          fsys;
    afxFileFlags        flags;
    afxUri              rootPath;  // path of exchange point
    afxArchive          arc;
};

AFX_OBJECT(afxStorage)
{
    afxChar             diskId; // single letter identifying this file system.
    afxUri8             baseUrl; // qualified name of exchange point <//./z/>
    afxChain            storages;
    afxChain            classes;
    afxClass            fileCls;
    afxClass            archCls;
    afxClass            urdCls;
    afxChain            fileChain;
};

#endif//_AFX_STORAGE_C

AFX afxClass const*     AfxGetFileClass(afxStorage fsys);
AFX afxClass const*     AfxGetArchiveClass(afxStorage fsys);

#endif//AFX_STORAGE_DDK_H
