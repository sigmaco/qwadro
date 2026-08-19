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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_MESH_IO_H
#define ARX_MESH_IO_H

#include "qwadro/cad/arxMesh.h"

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(MXD_MSHS_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED MESH SECTION HEADER
{
    arxMeshSectionFlags flags;
    afxUnit32 mtlIdx;
    afxUnit32 baseTriIdx;
    afxUnit32 triCnt;
};

AFX_DEFINE_STRUCT(MXD_MSHV_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED MESH BIAS HEADER
{
    arxMeshBiasFlags flags;
    afxUnit trisBaseOff;
    afxUnit32 triCnt;
};

AFX_DEFINE_STRUCT(MXD_MSHM_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED MESH MORPH HEADER
{
    arxMeshMorphFlags flags;
    afxUnit morphedAttrCnt;
    afxMask32 morphedAttrs;
    afxUnit baseVtx;
};

AFX_DEFINE_STRUCT(MXD_VTX_ATTR_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED VERTEX ATTRIBUTE HEADER
{
    arxVertexFlags  flags;
    afxUnit32       baseDataOffset;
    avxFormat       encodedFmt;
    avxFormat       decodedFmt;
};

AFX_DEFINE_STRUCT(MXD_MSH_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED MESH HEADER
{
    arxMeshFlags    flags;
    avxTopology     topology; // actually only TRILIST
    afxUnit         triCnt; // count of primitives.
    afxUnit32       sideToAdjacentMapBaseOff; // [edgeCnt]
    afxUnit         mtlCnt; // used by sections
    afxUnit32       mtlIdsBaseOff; // [mtlCnt]
    afxUnit         secCnt;
    afxUnit32       sectionsBaseOff; // [secCnt]
    afxUnit         biasCnt;
    afxUnit32       biasesBaseOff;
    // nested bias identifier strings for fast lookup.
    afxUnit32       biasIdsBaseOff;
    afxUnit         jointsForTriCnt;
    afxUnit32       jointsForTriMapBaseOff;
    afxUnit         triToJointCnt;
    afxUnit32       triToJointMapBaseOff;

    // VERTEX DATA
    afxUnit         vtxCnt;
    afxUnit32       vtxToVtxMapBaseOff; // [vtxCnt]
    afxUnit32       vtxToTriMapBaseOff; // [vtxCnt]
    afxUnit         minIdxSiz;
    afxUnit         idxCnt; // count of primitive indices.
    afxUnit32       indicesBaseOff; // [idxCnt] --- indices into primitive vertices.
    afxUnit         attrCnt; // used by morphes.
    afxUnit32       vtxAttrIdsBaseOff;
    afxUnit32       vtxAttrInfoBaseOff;

    // SHAPE DATA
    afxUnit         morphCnt;
    afxUnit32       morphsBaseOff;
    // nested section AABB for fast lookup
    afxUnit32       secAabbBaseOff; // [morphCnt][secCnt]
    // nested bias OBB for fast lookup.
    afxUnit32       biasObbBaseOff; // [morphCnt][biasCnt]
    // nested morph tags for fast lookup.
    afxUnit32       morphTagsBaseOff; // [morphCnt]

};

AFX_DEFINE_STRUCT(MXD_MDL_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, SERIALIZED MODEL HEADER
{
    afxUnit8        fcc[4];
    afxUnit32       hdrSiz;
    afxUnit32       hdrVer;
    afxUnit32       segCnt;
    afxUnit32       segStart;
    afxUnit32       sdbSiz;
    afxUnit32       sdbStart;

    afxM3d          basis;
    afxV3d          origin;
    afxReal         unitsPerMeter;

    afxUnit32       flags;
    afxUnit32       lodType;
    afxUnit32       boneCnt;
    afxUnit32       jntPiBase;
    afxUnit32       jntLtBase;
    afxUnit32       jntIwBase;
    afxUnit32       jntLeBase;
    afxUnit32       jntIdBase;
    afxUnit32       jntUddBase;
    afxTransform    displace;
    afxUnit32       mshCnt;
    afxUnit32       mshIdBase;
    afxUnit32       mshInfoBase;
    afxUnit32       rigCnt;
    afxUnit32       rigBase;

    // look for joint extensions
    // look for rig extensions
};

AFX_DEFINE_STRUCT(MXD_MSHR_HDR)
// SIGMA/ENGITECH, MODEL EXCHANGE DICTIONARY, MODEL-MESH RIG HEADER
{
    afxUnit32       mshIdx;
    afxUnit32       flags;
    // transplanted skeleton is a runtime thing
};

#pragma pack(pop)

ARX afxError ArxUpdateMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    void const* src, 
    afxUnit srcIdxSiz
);

ARX afxError ArxDumpMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    void* dst, 
    afxUnit dstIdxSiz
);

ARX afxError ArxUploadMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    afxStream in, 
    afxUnit srcIdxSiz
);

ARX afxError ArxDownloadMeshIndices
(
    arxMesh msh, 
    afxUnit baseTriIdx, 
    afxUnit triCnt, 
    afxStream out, 
    afxUnit dstIdxSiz
);

ARX afxError ArxUpdateVertexData
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseVtx, 
    afxUnit vtxCnt, 
    void const* src, 
    afxUnit32 srcStride
);

ARX afxError ArxUploadVertexData
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseVtx, 
    afxUnit vtxCnt, 
    afxStream in, 
    afxUnit stride
);

ARX afxError ArxDownloadVertexData
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseVtx, 
    afxUnit vtxCnt, 
    afxStream out, 
    afxUnit stride
);

/*
    The ArxExportMeshTopology() function exports and potentially processes a mesh's topology in different formats 
    (like triangle lists or line lists).
*/

ARX afxError ArxExportMeshTopology
(
    // The mesh object from which the topology is being extracted.
    arxMesh msh,
    // The desired topology format for the export (could be a triangle list or line list).
    avxTopology top,
    // The index of the base triangle to start exporting from.
    afxUnit baseTriIdx,
    // The number of triangles to export.
    afxUnit triCnt,
    // A pointer to the destination buffer where the processed topology will be stored.
    void* dst,
    // The size of the indices to be stored in the destination buffer.
    afxUnit dstIdxSiz
);

ARX afxError ArxExportMeshData
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseVtx, 
    afxUnit vtxCnt, 
    void* dst, 
    afxUnit stride
);

ARX afxError ArxExportMeshDataDeindexed
(
    arxMesh msh, 
    afxUnit attrIdx, 
    afxUnit morphIdx, 
    afxUnit baseIdx, 
    afxUnit idxCnt, 
    void* dst, 
    afxUnit stride
);

////////////////////////////////////////////////////////////////////////////////

#endif//ARX_MESH_IO_H
