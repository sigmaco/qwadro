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

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_PLACEMENT_H
#define ARX_PLACEMENT_H

// O objeto arxPosture é um buffer usado para manter o estado de um arxSkeleton de arxModel como expressado no "world space". 

#include "qwadro/iris/arxIrisDefs.h"

ARX afxUnit ArxGetPostureCapacity(arxPosture plce);

ARX afxM4d* ArxGetPostureMatrices(arxPosture plce, afxUnit baseArtIdx);

ARX afxM4d* ArxGetPostureDeltas(arxPosture plce, afxUnit baseArtIdx);

/// A afxM4d composta está no mesmo layout como aquela retornada de GetPosture4x4. 
/// É igual ao "world space" --- afxM4d espacial para a articulação (como dada por GetPosture4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articulação (como alojada no arxSkeleton). 
/// Portanto, esta representa a diferença entre a atual posição no world-space da articulação e sua posição de repouso no world-space.

/// As afxM4d de transformação no world-space e compostas são garantida a estarem contíguas, assim sendo, você pode também obter o ponteiro para o início da array e usá-lo para manualmente acessar articulações individuais. 

// Pass NIL as @pose to ArxProducePosture() to compute the rest placement.
// Pass TRUE as composite to ArxProducePosture() to compute the placement's composite buffer.
ARX void ArxProducePosture(arxPosture plce, arxPose pose, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxM4d const displace, afxBool skipDeltas);

// extract the pose
ARX void ArxProducePosturePose(arxPosture plce, arxSkeleton skl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const displace, afxBool rigid, arxPose pose);

ARX void ArxProduceCompositeMatrices(arxPosture plce, arxSkeleton skl, afxUnit baseJnt, afxUnit cnt, afxBool /*3x4*/transposed, afxM4d matrices[]);
ARX void ArxProduceIndexedCompositeMatrices(arxPosture plce, arxSkeleton skl, afxUnit cnt, afxUnit const jntMap[], afxBool /*3x4*/transposed, afxM4d matrices[]);

////////////////////////////////////////////////////////////////////////////////

ARX afxError ArxAcquirePostures(arxScenario scio, afxUnit cnt, afxUnit const artCnt[], afxBool const excludeDeltas[], arxPosture placements[]);

#endif//ARX_PLACEMENT_H
