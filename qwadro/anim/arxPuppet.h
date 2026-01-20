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

#ifndef ARX_PUPPET_H
#define ARX_PUPPET_H

// O objeto arxPuppet aloja o estado de um arxModel no tempo de execução.
// O arxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
// O arxPuppet mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo arxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio arxPuppet.
// Dito isto porque os dados do arxPuppet são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo arxModel.
// Assim, enquanto é comum colocar a hierarquia de estruturas do arxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos arxPuppet no cache e reusá-los.
// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
// O arxPuppet não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para arxPuppet funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

#include "qwadro/cad/arxModel.h"
#include "qwadro/anim/arxPulley.h"
#include "qwadro/scene/arxNode.h"

ARX afxBool ArxGetPuppetModel
(
    arxPuppet pup, 
    arxModel* model
);

ARX afxUnit ArxCountPuppetMotives
(
    arxPuppet pup
);

ARX void ArxRecenterPuppetMotiveClocks
(
    arxPuppet pup, 
    afxReal currClock
);

/// Update all the motors affecting a particular puppet.
ARX void ArxStepPuppetMotives
(
    arxPuppet pup, 
    afxReal /* NOT delta */ time
);

ARX void ArxPurgeTerminatedMotives
(
    arxPuppet pup
);

ARX void ArxAccumulatePuppetAnimations
(
    arxPose rslt, 
    arxPuppet pup, 
    afxUnit basePivotIdx, 
    afxUnit pivotCnt, 
    afxReal allowedErr, 
    afxUnit const sparseJntMap[]
);

ARX void ArxComputePuppetMotionVectors
(
    arxPuppet pup, 
    afxReal secsElapsed, 
    afxBool inverse, 
    afxV3d translation, 
    afxV3d rotation
);

ARX void ArxComputePuppetMotionMatrix
(
    arxPuppet pup, 
    afxReal secsElapsed, 
    afxBool inverse, 
    afxM4d const mm, 
    afxM4d m
);


AFX_DEFINE_STRUCT(arxAnimSampleContext)
{
    afxBool         accelerated;
    afxUnit         firstPivot;
    afxUnit         pivotCnt;
    arxPulley      moto;
    arxPose         pose;
    arxPosture    posb;
    afxReal         allowedErr;
    afxM4d const    displacement;
    afxUnit const*  sparseBoneArray;
};

ARX afxBool     ArxSampleBodyAnimationsUnified(arxPuppet pup, arxAnimSampleContext const* ctx);

ARX afxBool ArxGetPuppetPose
(
    arxPuppet pup, 
    arxPose* pose, 
    arxPosture* placement
);

ARX afxError ArxAttachPuppet
(
    arxPuppet pup, 
    arxNode nod, 
    void (*sync)(arxNodular*), 
    afxFlags dagFlags, 
    afxMask dagMask
);

ARX afxError ArxDetachPuppet
(
    arxPuppet pup
);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(arxPuppetInfo)
{
    arxModel mdl;
    afxUnit partCnt;
    afxUnit baseMshToPart;
    afxUnit const* mshToPartMap;
    arxNode nod;
    void(*dagSync)(arxNodular*);
    afxFlags dagFlags;
    afxMask dagMask;
};

ARX afxError ArxAcquirePuppets
(
    arxScenario scio, 
    arxModel mdl, 
    afxUnit cnt, 
    arxPuppet puppets[]
);

ARX afxUnit ArxPerformManipulatedPose
(
    arxPose pose, 
    afxReal startTime, 
    afxReal duration, 
    afxUnit iterCnt, 
    arxTrackMask* modelMask, 
    afxUnit cnt, 
    arxPuppet puppets[]
);

ASX afxUnit ArxEnumeratePuppets
(
    arxScenario scio, 
    afxUnit first, 
    afxUnit cnt, 
    arxPuppet puppets[]
);

ASX afxUnit ArxInvokePuppets
(
    arxScenario scio, 
    afxUnit first, 
    afxUnit cnt, 
    afxBool(*f)(arxPuppet, void*), 
    void *udd
);

#endif//ARX_PUPPET_H
