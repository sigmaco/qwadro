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

#ifndef AMX_SAMPLE_CONTEXT_H
#define AMX_SAMPLE_CONTEXT_H

/*
    In computer simulation, a sample context refers to the specific scenario, environment, 
    or conditions under which a simulation is run or a particular model is applied. It provides 
    the background that helps define how the simulation operates and what data or parameters are being used. 
    The context can encompass factors such as initial conditions, system variables, assumptions, and constraints 
    that affect the results of the simulation.

    In summary, the sample context in a computer simulation refers to the conditions and assumptions that 
    frame the model or system being simulated.
*/

#include "qwadro/inc/sim/op/afxPoseOps.h"
#include "qwadro/inc/sim/op/afxMiscOps.h"

AMX afxUnit AfxGetCatalystPort(afxCatalyst cyst);

AMX afxUnit AfxGetCatalystPool(afxCatalyst cyst);

AMX afxError AfxCompileSimCommands(afxCatalyst cyst);

AMX afxError AfxRecycleCatalyst(afxCatalyst cyst, afxBool freeRes);

AMX afxError AfxAcquireCatalysts(afxSimulation dsys, afxUnit exuIdx, afxUnit queIdx, afxUnit cnt, afxCatalyst batches[]);

#endif//AMX_SAMPLE_CONTEXT_H
