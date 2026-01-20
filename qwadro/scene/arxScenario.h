/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_SCENARIO_H
#define ARX_SCENARIO_H

// arxScenario devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, há um conceito de uma fila de submissão de trabalho para a GPU. Porém, diferentemente do Vulkan, no QWADRO esta "fila" é separada em dois.
// A primeira parte contém a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

/*
    Draw input refers to the data or instructions provided to a system or application to create or render graphical content. 
    It involves the input that specifies what should be drawn, how it should be drawn, and where it should appear.

    In graphics programming, draw input consists of the commands or data used to generate visual content on the screen.
    In 3D graphics, vertex data includes coordinates, colors, and texture coordinates that define the geometry of objects.
    In game development, draw input refers to the data and instructions used to render game graphics and scenes.
    In vector graphics, draw input consists of commands and parameters that define shapes and lines in a vector-based drawing application.

*/

/*
    Motion Engines in Animation and Graphics.
    In computer graphics or animation, "motion engines" might refer to the systems or software that control the movement of characters, 
    objects, or camera perspectives in a scene. Examples include physics engines or animation systems used in video games, movies, or simulations.
    Game Engines with Motion Physics: In game development, motion engines are often built into game engines (like Unity, Unreal Engine, etc.) 
    to simulate realistic movements, from the character's walk cycle to the behavior of vehicles or fluids.
*/

#include "qwadro/draw/afxDrawSystem.h"

#include "qwadro/iris/arxIrisDefs.h"
#include "qwadro/iris/arxRenderModel.h"
#include "qwadro/iris/arxRenderPuppet.h"
#include "qwadro/iris/arxRenderTerrain.h"
#include "qwadro/cad/arxTerrain.h"
#include "qwadro/cad/arxModel.h"
#include "qwadro/cad/arxMesh.h"
#include "qwadro/scene/arxPosture.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/scene/arxCamera.h"
#include "qwadro/scene/arxSky.h"
#include "qwadro/scene/arxBufferizer.h"
#include "qwadro/iris/arxTechnique.h"
#include "qwadro/iris/arxRenderContext.h"
#include "qwadro/scene/arxIllumination.h"
#include "qwadro/iris/arxRenderer.h"
#include "qwadro/anim/arxAnimation.h"


typedef enum arxEventId
{
    arxEventId_
} arxEventId;

AFX_DEFINE_STRUCT(arxEvent)
{
    arxEventId  id;
    afxBool     posted, accepted;
    void*       udd[1];
};

typedef afxBool(*afxDrawInputProc)(arxScenario scio, arxEvent const* ev);

AFX_DEFINE_STRUCT(arxScenarioConfig)
{
    afxDrawSystem       dsys;
    afxDrawInputProc    proc;
    afxUnit             dragTxuBase;
    afxUnit             dragTxuCnt;

    afxV3d              right;
    afxV3d              up;
    afxV3d              back;
    afxV3d              origin;
    afxBox              extent;
    afxReal             unitsPerMeter;
    afxReal             allowedLodErrFadingFactor;

    afxUnit             cmdPoolMemStock;
    afxUnit             estimatedSubmissionCnt;
    afxUnit             minVtxBufCap;
    afxUnit             maxVtxBufCap;
    afxUnit             minIdxBufCap;
    afxUnit             maxIdxBufCap;
    void*               udd;
};

ARX afxDrawSystem ArxGetScenarioDrawSystem
(
    arxScenario scio
);

ARX void* ArxGetScenarioUdd
(
    arxScenario scio
);

ARX afxError ArxUplinkTxds(arxScenario scio, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[]);

ARX void ArxQueryLodErrorTolerance
(
    arxScenario scio, 
    afxReal allowedErr, 
    afxReal *allowedErrEnd, 
    afxReal *allowedErrScaler
);

ARX void ArxComputeSimilarity
(
    arxScenario scio, 
    afxReal unitsPerMeter, 
    afxV3d const right, 
    afxV3d const up, 
    afxV3d const back, 
    afxV3d const origin, 
    afxM3d ltm, 
    afxM3d iltm, 
    afxV3d atv
);

ARX afxUnit ArxRecenterAllPulleyClocks
(
    arxScenario scio, 
    afxReal dCurrentClock
);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxConfigureScenario
(
    afxUnit icd, 
    arxScenarioConfig* cfg
);

ARX afxError ArxAcquireScenario
(
    afxUnit icd,
    arxScenarioConfig const* cfg, 
    arxScenario* input
);

#endif//ARX_SCENARIO_H
