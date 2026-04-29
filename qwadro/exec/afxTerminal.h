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

// TTY --- Our teletypewriter

#ifndef AFX_TERMINAL_H
#define AFX_TERMINAL_H

#include "qwadro/base/afxString.h"
#include "qwadro/base/afxString.h"

AFX afxBool     AfxAcquireTerminal(void);
AFX afxBool     AfxDisposeTerminal(void);

AFX afxResult   AfxPrompt(afxString const* cur, afxString* buf);

AFX afxResult   AfxPrint(afxUnit32 color, afxChar const* msg);
AFX afxResult   AfxPrintf(afxUnit32 color, afxChar const* msg, ...);

#endif//AFX_TERMINAL_H
