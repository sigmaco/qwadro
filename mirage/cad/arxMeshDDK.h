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

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_MESH_DDK_H
#define ARX_MESH_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DEFINE_STRUCT(_arxDdiMsh);
AFX_DEFINE_STRUCT(_arxIddMsh);

#ifdef _ARX_MESH_C

AFX_DEFINE_STRUCT(arxMeshAttr)
{
    afxString8          usage; // 8
    arxVertexFlags      flags;
    avxFormat           fmt;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_DEFINE_STRUCT(arxVertexBuffer)
{
    afxChain    meshes;
    afxLink  buf;

};

// Meshes just reference a material ID. This avoids material DAGs attached directly to geometry.

AFX_OBJECT(arxMesh)
{
    _arxIddMsh*         idd;
    _arxDdiMsh const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // TOPOLOGY DATA
    arxMeshFlags        flags;
    avxTopology         topology; // actually only TRILIST
    afxUnit             triCnt; // count of primitives.
    afxUnit*            sideToAdjacentMap; // [edgeCnt]
    union
    {
        // for collision
        afxV3d*         edgeNrm; // [edgeCnt]
        afxV4d*         edgeNrmw; // [edgeCnt]
    };
    afxUnit             mtlCnt; // used by sections
    afxString*          materials; // [mtlCnt]
    afxUnit             secCnt;
    arxMeshSection*     sections; // [secCnt]
    afxUnit             biasCnt;
    arxMeshBias*        biases;
    // nested bias identifier strings for fast lookup.
    afxString*          biasId;
    afxUnit             jointsForTriCnt;
    afxUnit*            jointsForTriMap;
    afxUnit             triToJointCnt;
    afxUnit*            triToJointMap;

    // VERTEX DATA
    afxUnit             vtxCnt;
    afxUnit*            vtxToVtxMap; // [vtxCnt]
    afxUnit*            vtxToTriMap; // [vtxCnt]
    afxUnit             minIdxSiz;
    afxUnit             idxCnt; // count of primitive indices.
    afxUnit*            indices; // [idxCnt] --- indices into primitive vertices.
    afxUnit             attrCnt; // used by morphes.
    afxString*          attrIds;
    arxMeshAttr*        attrInfo;
    afxByte**           vtxAttrData; // [attrCnt]

    // SHAPE DATA
    afxUnit             morphCnt;
    arxMeshMorph*       morphs;
    // nested morph tags for fast lookup.
    afxString*          morphTags; // [morphCnt]
    // nested section AABB for fast lookup
    afxBox*             secAabb; // [morphCnt][secCnt]
    // nested bias OBB for fast lookup.
    afxBox*             biasObb; // [morphCnt][biasCnt]

    // TODO: move to mesh rig
    arxVertexCache      vtxCache;
    avxBuffer           ibo;
    afxUnit32           iboBase;
    afxUnit32           iboRange;
    afxUnit32           iboStride;

    afxString           urn; // 32
};

#endif

ARX afxClassConfig const _ARX_MSH_CLASS_CONFIG;

#endif//ARX_MESH_DDK_H
