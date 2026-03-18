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
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#define _ARX_SKELETON_C
#define _ARX_MODEL_C
#define _ARX_MESH_C
#define _ARX_MESH_LINKAGE_C
#define _ARX_MESH_TOPOLOGY_C
//#define _AVX_GEOMETRY_C
#define _ASX_SIMULATION_C
//#include "afx/targa/avxIcd.h"
#include "../scene/arxIcd.h"

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

_ARX afxUnit ArxMeasureNestedStrings(afxUnit cnt, afxString const strings[])
{
    afxError err = { 0 };
    AFX_ASSERT(strings);
    afxUnit totalSiz = 0;
    
    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    } sub;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];
        totalSiz += s->len;
    }
    totalSiz += sizeof(sub) * cnt;
    return totalSiz;
}

_ARX afxError ArxWriteNestedStrings(afxStream out, afxUnit cnt, afxString const strings[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(strings);

    afxUnit siz = ArxMeasureNestedStrings(cnt, strings);

    urdMark mark = { 0 };
    mark.fcc = afxChunkId_STRING;
    mark.siz = siz;

    if (AfxWriteStream(out, sizeof(mark), 0, &mark))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];

        if (AfxWriteStream(out, s->len, 0, s->start))
        {
            AfxThrowError();
            return err;
        }
    }

    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    }
    d =
    {
        .start = 0,
        .len = 0
    };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];
        d.len = s->len;

        if (AfxWriteStream(out, sizeof(d), 0, &d))
        {
            AfxThrowError();
            return err;
        }
        d.start += d.len;
    }
    return err;
}

_ARX afxError ArxReadNestedStrings(afxStream in, afxUnit cnt, afxString strings[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(strings);

    urdMark hdr2;
    if (afxChunkId_STRING != AfxPullStreamChunk(in, &hdr2))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(4096 >= hdr2.siz);
    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);
    AfxReadString(&sdb.s, in, hdr2.siz);

    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    } d;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];

        if (AfxReadStream(in, sizeof(d), 0, &d))
        {
            AfxThrowError();
            break;
        }
        *s = AfxExcerptString(&sdb.s, d.start, d.len);
    }
    return err;
}

_ARX afxError ArxWriteMappedStrings(afxStream out, afxString* sdb, afxUnit cnt, afxString const strings[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(strings);

    struct
    {
        afxUnit32 len;
        afxUnit32 start;
    }
    hdr = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];

        hdr.start = sdb->len;
        hdr.len = s->len;

        if (AfxCatenateStrings(sdb, 1, s))
            AfxThrowError();

        if (AfxWriteStream(out, sizeof(hdr), 0, &hdr))
        {
            AfxThrowError();
        }
    }
    return err;
}

_ARX afxError ArxReadMappedStrings(afxStream in, afxString const* sdb, afxUnit cnt, afxString strings[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(strings);

    struct
    {
        afxUnit32 len;
        afxUnit32 start;
    } hdr;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];

        if (AfxReadStream(in, sizeof(hdr), 0, &hdr))
        {
            AfxResetStrings(1, s);
            AfxThrowError();
        }
        else
        {
            *s = AfxExcerptString(sdb, hdr.start, hdr.len);
        }
    }
    return err;
}

_ARX afxError ArxArchiveMeshes(afxStream out, afxString* sdb, afxUnit cnt, arxMesh meshes[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_MSH, cnt, meshes);
    afxUnit mshIdx = 0;

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());
    MXD_MSH_HDR* mshHdrs = AfxRequestArena(&arena, sizeof(mshHdrs[0]), cnt, NIL, 0);

    afxSize mshHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    if (AfxAdvanceStream(out, cnt * sizeof(MXD_MSH_HDR)))
        AfxThrowError();

    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxMesh msh = meshes[mshIdx];
        
        MXD_MSH_HDR mshHdr = { 0 };
#if 0
        mshHdr.fcc[0] = 'm';
        mshHdr.fcc[1] = 's';
        mshHdr.fcc[2] = 'h';
        mshHdr.fcc[3] = '\0';
        mshHdr.hdrSiz = sizeof(mshHdr) - sizeof(urdMark);
        AfxDumpString(&msh->urn, 0, AFX_MIN(32, msh->urn.len), mshHdr.urn);
#endif
        mshHdr.flags = msh->flags;
        mshHdr.topology = msh->topology;
        mshHdr.triCnt = msh->triCnt;
        mshHdr.vtxCnt = msh->vtxCnt;
        mshHdr.morphCnt = msh->morphCnt;
        mshHdr.mtlCnt = msh->mtlCnt;
        mshHdr.secCnt = msh->secCnt;
        mshHdr.biasCnt = msh->biasCnt;
        mshHdr.idxCnt = msh->idxCnt;
        mshHdr.attrCnt = msh->attrCnt;

        // WRITE TOPOLOGY DATA

        if (msh->sideToAdjacentMap)
        {
            mshHdr.sideToAdjacentMapBaseOff = AfxAskStreamPosn(out);

            if (AfxWriteStream(out, msh->triCnt * sizeof(msh->sideToAdjacentMap[0]), 0, msh->sideToAdjacentMap))
                AfxThrowError();
        }

        // write materials' identifier strings.

        mshHdr.mtlIdsBaseOff = AfxAskStreamPosn(out);
        if (ArxWriteMappedStrings(out, sdb, msh->mtlCnt, msh->materials))
            AfxThrowError();

        // write sections' descrptor structures.

        mshHdr.sectionsBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.secCnt; i++)
        {
            arxMeshSection* sec = &msh->sections[i];
            MXD_MSHS_HDR mshs;
            mshs.baseTriIdx = sec->baseTriIdx;
            mshs.triCnt = sec->triCnt;
            mshs.flags = sec->flags;
            mshs.mtlIdx = sec->mtlIdx;

            if (AfxWriteStream(out, sizeof(mshs), 0, &mshs))
                AfxThrowError();
        }

        // write biases' descriptor structures

        mshHdr.biasesBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.biasCnt; i++)
        {
            arxMeshBias* v = &msh->biases[i];
            MXD_MSHV_HDR mshv;
            mshv.triCnt = v->triCnt;
            mshv.flags = v->flags;

            if (AfxWriteStream(out, sizeof(mshv), 0, &mshv))
                AfxThrowError();
        }

        // write biases' identifier strings.

        mshHdr.biasIdsBaseOff = AfxAskStreamPosn(out);
        if (ArxWriteMappedStrings(out, sdb, msh->biasCnt, msh->biasId))
            AfxThrowError();

        if (msh->jointsForTriMap)
        {
            mshHdr.jointsForTriMapBaseOff = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, msh->jointsForTriCnt * sizeof(msh->jointsForTriMap[0]), 0, msh->jointsForTriMap))
                AfxThrowError();
        }

        if (msh->triToJointMap)
        {
            mshHdr.triToJointMapBaseOff = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, msh->triToJointCnt * sizeof(msh->triToJointMap[0]), 0, msh->triToJointMap))
                AfxThrowError();
        }

        // WRITE VERTEX DATA

        if (msh->vtxToVtxMap)
        {
            mshHdr.vtxToVtxMapBaseOff = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, msh->vtxCnt * sizeof(msh->vtxToVtxMap), 0, msh->vtxToVtxMap))
                AfxThrowError();
        }

        if (msh->vtxToTriMap)
        {
            mshHdr.vtxToTriMapBaseOff = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, msh->vtxCnt * sizeof(msh->vtxToTriMap[0]), 0, msh->vtxToTriMap))
                AfxThrowError();
        }

        mshHdr.indicesBaseOff = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, msh->idxCnt * sizeof(msh->indices[0]), 0, msh->indices))
            AfxThrowError();

        // write attributes' identifier strings.

        mshHdr.vtxAttrIdsBaseOff = AfxAskStreamPosn(out);
        if (ArxWriteMappedStrings(out, sdb, msh->attrCnt, msh->attrIds))
            AfxThrowError();

        // write attributes' info and data

        MXD_VTX_ATTR_HDR msvas[16] = { 0 };
        mshHdr.vtxAttrInfoBaseOff = AfxAskStreamPosn(out);
        if (AfxAdvanceStream(out, sizeof(msvas[0]) * msh->attrCnt))
            AfxThrowError();

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            msvas[i].encodedFmt = msh->attrInfo[i].fmt;
            msvas[i].decodedFmt = msh->attrInfo[i].fmt;
            msvas[i].flags = msh->attrInfo[i].flags;

            avxFormatDescription pfd;
            AvxDescribeFormats(1, &msvas[i].encodedFmt, &pfd);

            afxSize fmtSiz = pfd.stride;// AfxVertexFormatGetSize(msvas[i].encodedFmt);
            msvas[i].baseDataOffset = AfxAskStreamPosn(out);
            if (AfxWriteStream(out, fmtSiz * msh->vtxCnt * msh->morphCnt, 0, msh->vtxAttrData[i]))
                AfxThrowError();
        }

        afxSize bkpPos = AfxAskStreamPosn(out);
        if (AfxSeekStream(out, mshHdr.vtxAttrInfoBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (AfxWriteStream(out, sizeof(msvas[0]) * msh->attrCnt, 0, msvas))
            AfxThrowError();

        if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
            AfxThrowError();

        // SHAPE DATA

        mshHdr.morphsBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.morphCnt; i++)
        {
            arxMeshMorph* m = &msh->morphs[i];
            MXD_MSHM_HDR mshm;
            mshm.flags = m->flags;
            mshm.baseVtx = m->baseVtx;
            mshm.morphedAttrs = m->affectedAttrs;
            mshm.morphedAttrCnt = m->affectedAttrCnt;

            if (AfxWriteStream(out, sizeof(mshm), 0, &mshm))
                AfxThrowError();
        }

        mshHdr.secAabbBaseOff = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, msh->morphCnt * msh->secCnt * sizeof(msh->secAabb[0]), 0, msh->secAabb))
            AfxThrowError();

        mshHdr.biasObbBaseOff = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, msh->morphCnt * msh->biasCnt * sizeof(msh->biasObb[0]), 0, msh->biasObb))
            AfxThrowError();

        mshHdr.morphTagsBaseOff = AfxAskStreamPosn(out);
        if (ArxWriteMappedStrings(out, sdb, msh->morphCnt, msh->morphTags))
            AfxThrowError();

        // write mesh extensions
        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        if (AfxWriteStream(out, sizeof(extMark), 0, &extMark))
            AfxThrowError();

        // write mesh info
        //mshHdr.hdrSiz = AfxAskStreamPosn(out) - mshHdrOffBkp - sizeof(urdMark);

        mshHdrs[mshIdx] = mshHdr;

    } while (!err && (++mshIdx < cnt));

    // write out all headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    if (AfxSeekStream(out, mshHdrOffBkp, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (AfxWriteStream(out, cnt * sizeof(mshHdrs[0]), 0, mshHdrs))
        AfxThrowError();

    AFX_ASSERT(endOfHdrs == AfxAskStreamPosn(out));

    if (AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN))
        AfxThrowError();

    AfxDismantleArena(&arena);

    return err;
}

_ARX afxError ArxDownloadModel(arxModel mdl, afxStream out)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit mdlIdx = 0;

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxUnit uniqueMeshCnt = 0;
    arxMesh meshes[256];

    MXD_MDL_HDR mdlHdr = { 0 };
    mdlHdr.fcc[0] = 'm';
    mdlHdr.fcc[1] = '4';
    mdlHdr.fcc[2] = 'd';
    mdlHdr.fcc[3] = '\0';
    mdlHdr.hdrSiz = sizeof(mdlHdr) - sizeof(urdMark);
    AfxM3dReset(mdlHdr.basis);
    AfxV3dZero(mdlHdr.origin);
    mdlHdr.unitsPerMeter = 1.f;
    mdlHdr.lodType = mdl->lodType;
    mdlHdr.boneCnt = mdl->skl->boneCnt;
    mdlHdr.flags = mdl->flags;
    mdlHdr.displace = mdl->displace;
    mdlHdr.rigCnt = mdl->rigCnt;

    afxSize hdrPosBkp = AfxAskStreamPosn(out);
    if (AfxAdvanceStream(out, sizeof(mdlHdr)))
        AfxThrowError();

    // write the skeleton
    {
        // write joints' parent indices

        mdlHdr.jntPiBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.boneCnt * sizeof(afxUnit32), 0, mdl->skl->bonePi))
            AfxThrowError();

        for (afxUnit i = 0; i < mdlHdr.boneCnt; i++)
        {
            AfxReportComment("download bone %i -> %i", i, mdl->skl->bonePi[i]);
        }

        // write joints' local transforms

        mdlHdr.jntLtBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.boneCnt * sizeof(afxTransform), 0, mdl->skl->boneLt))
            AfxThrowError();

        // write joints' inverse world matrices

        mdlHdr.jntIwBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.boneCnt * sizeof(afxM4d), 0, mdl->skl->boneIw))
            AfxThrowError();

        // write joints' LOD error values

        mdlHdr.jntLeBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.boneCnt * sizeof(afxReal32), 0, mdl->skl->boneLe))
            AfxThrowError();

        // write joints' identifier strings (to string database)
        mdlHdr.jntIdBase = AfxAskStreamPosn(out);

        if (ArxWriteMappedStrings(out, &sdb.s, mdlHdr.boneCnt, mdl->skl->boneId))
            AfxThrowError();
    }

    mdlHdr.mshIdBase = AfxAskStreamPosn(out);

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        arxMeshLinkage rig = mdl->rigs[i];
        if (!rig) continue;
        arxMesh msh = rig->msh;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        if (msh)
        {
            afxBool skip = FALSE;
            for (afxUnit j = 0; j < i; j++)
            {
                arxMeshLinkage prevRig = mdl->rigs[j];

                // must skip this mesh if this has already been included before.
                if (!prevRig || (prevRig->msh == msh))
                {
                    skip = TRUE;
                    break;
                }
            }

            if (skip) continue;  // skip mesh if it has already been included before.
            else
            {
                meshes[uniqueMeshCnt++] = msh;
                if (ArxWriteMappedStrings(out, &sdb.s, 1, &msh->urn))
                    AfxThrowError();
            }
        }
    }

    mdlHdr.mshCnt = uniqueMeshCnt;
    mdlHdr.rigBase = AfxAskStreamPosn(out);

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        arxMeshLinkage rig = mdl->rigs[i];
        if (!rig) continue;
        arxMesh msh = rig->msh;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        if (msh)
        {
            afxUnit mshIdx = i;

            for (afxUnit j = 0; j < i; j++)
            {
                arxMeshLinkage prevRig = mdl->rigs[j];

                // reuse mesh already included before.
                if (prevRig && (prevRig->msh == msh))
                {
                    mshIdx = j;
                    break;
                }
            }

            MXD_MSHR_HDR rigi = { 0 };
            rigi.flags = rig->flags;
            rigi.mshIdx = mshIdx;

            if (AfxWriteStream(out, sizeof(rigi), 0, &rigi))
                AfxThrowError();
        }
    }

    mdlHdr.mshInfoBase = AfxAskStreamPosn(out);

    if (ArxArchiveMeshes(out, &sdb.s, uniqueMeshCnt, meshes))
        AfxThrowError();

    {
        // write model extensions
        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        if (AfxWriteStream(out, sizeof(extMark), 0, &extMark))
            AfxThrowError();
    }

    {
        // write string database
        mdlHdr.sdbStart = AfxAskStreamPosn(out);
        mdlHdr.sdbSiz = sdb.s.len;

        if (AfxWriteStream(out, mdlHdr.sdbSiz, 0, sdb.s.start))
        {
            AfxThrowError();
            return err;
        }
    }

    // write model header
    mdlHdr.hdrSiz = AfxAskStreamPosn(out) - hdrPosBkp - sizeof(urdMark);
    
    if (AfxWriteStreamAt(out, hdrPosBkp, sizeof(mdlHdr), 0, &mdlHdr))
        AfxThrowError();

    if (AfxAdvanceStream(out, mdlHdr.hdrSiz))
        AfxThrowError();

    return err;
}

_ARX afxError ArxArchiveModel(arxModel mdl, afxUri const* uri)
{
    afxError err = { 0 };
    afxStream iob;
    afxStreamInfo info = {0};
    info.flags = afxIoFlag_WRITEABLE;
    info.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &info, &iob);

    AfxReopenFile(iob, uri, afxFileFlag_W);
    ArxDownloadModel(mdl, iob);
    AfxDisposeObjects(1, &iob);

    return err;
}

_ARX afxError ArxUploadModel(arxScenario scio, arxMtd mtd, arxSkeleton skl, afxString const* urn, afxStream in, arxModel* model)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(model);

    // Qwadro doesn't use afxChunkID_STRUCT

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

#pragma push(pack, 1)
    MXD_MDL_HDR mdlHdr;
#pragma pop(pack)

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxString strings[256];
    arxMesh meshes[256];
    arxMeshRigging rigis[256];

    if (AfxReadStream(in, sizeof(mdlHdr), 0, &mdlHdr))
        AfxThrowError();

    afxSize posBkp = AfxAskStreamPosn(in);
    if (AfxSeekStream(in, mdlHdr.sdbStart, afxSeekOrigin_BEGIN))
        AfxThrowError();

    AfxReadString(&sdb.s, in, mdlHdr.sdbSiz);

    if (AfxSeekStream(in, mdlHdr.jntIdBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    if (ArxReadMappedStrings(in, &sdb.s, mdlHdr.boneCnt, strings))
        AfxThrowError();

    arxModel mdl;
    arxModelBlueprint mdlb = { 0 };
    mdlb.lodType = mdlHdr.lodType;
    mdlb.mtd = mtd;
    mdlb.rigCnt = mdlHdr.rigCnt;
    mdlb.displace = mdlHdr.displace;
    
    if (skl) mdlb.skl = skl;
    else
    {
        mdlb.boneCnt = mdlHdr.boneCnt;
        mdlb.bones = strings;
    }

    if (ArxAssembleModels(scio, 1, urn, &mdlb, &mdl))
        AfxThrowError();

    arxModelInfo mdli;
    ArxDescribeModel(mdl, &mdli);

    arxSkeleton origSkl = NIL;
    arxSkeletonInfo skli = { 0 };

    if (!skl)
        origSkl = mdli.skl;
    else
    {
        skli.boneCnt = mdlHdr.boneCnt;
        skli.bones = strings;
        if (ArxAcquireSkeletons(scio, 1, urn, &skli, &origSkl))
            AfxThrowError();
    }

    if (!err)
    {
        afxUnit32* parentIdx = AfxRequestArena(&arena, sizeof(parentIdx[0]), mdlHdr.boneCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntPiBase, mdlHdr.boneCnt * sizeof(parentIdx[0]), 0, parentIdx))
            AfxThrowError();

        for (afxUnit i = 0; i < mdlHdr.boneCnt; i++)
        {
            AfxReportComment("upload bone %i -> %i", i, parentIdx[i]);
        }

        ArxReplaceBoneParents(origSkl, 0, mdlb.boneCnt, parentIdx, sizeof(parentIdx[0]));

        afxTransform* local = AfxRequestArena(&arena, sizeof(local[0]), mdlHdr.boneCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntLtBase, mdlHdr.boneCnt * sizeof(local[0]), 0, local))
            AfxThrowError();

        ArxReplaceBoneTransforms(origSkl, 0, mdlb.boneCnt, local);

        afxM4d* iw = AfxRequestArena(&arena, sizeof(iw[0]), mdlHdr.boneCnt, NIL, 0);
        if (AfxSeekStream(in, mdlHdr.jntIwBase, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (AfxReadStreamAt(in, mdlHdr.jntIwBase, mdlHdr.boneCnt * sizeof(iw[0]), 0, iw))
            AfxThrowError();

        ArxReplaceBoneInversors(origSkl, 0, mdlb.boneCnt, iw, sizeof(iw[0]));

        afxReal* lodErr = AfxRequestArena(&arena, sizeof(lodErr[0]), mdlHdr.boneCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntLeBase, mdlHdr.boneCnt * sizeof(lodErr[0]), 0, lodErr))
            AfxThrowError();

        ArxReplaceBoneNegligence(origSkl, 0, mdlb.boneCnt, lodErr);
    }

    if (AfxSeekStream(in, mdlHdr.mshIdBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    // Read mesh identifier strings.
    if (ArxReadMappedStrings(in, &sdb.s, mdlHdr.mshCnt, strings))
        AfxThrowError();

    if (AfxSeekStream(in, mdlHdr.mshInfoBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    arxMeshBlueprint mshb = { 0 };
    
    MXD_MSH_HDR* mshHdrs = AfxRequestArena(&arena, sizeof(mshHdrs[0]), mdlHdr.mshCnt, NIL, 0);

    if (AfxReadStream(in, mdlHdr.mshCnt * sizeof(MXD_MSH_HDR), 0, mshHdrs))
        AfxThrowError();

    arxMeshBlueprint* mshbs = AfxRequestArena(&arena, sizeof(mshbs[0]), mdlHdr.mshCnt, NIL, 0);

    for (afxUnit i = 0; i < mdlHdr.mshCnt; i++)
    {
        MXD_MSH_HDR* mshHdr = &mshHdrs[i];
        arxMeshBlueprint* mshb = &mshbs[i];
        AfxZero(mshb, sizeof(mshb[0]));

        afxString* attrs = AfxRequestArena(&arena, sizeof(attrs[0]), mshHdr->attrCnt, NIL, 0);
        if (AfxSeekStream(in, mshHdr->vtxAttrIdsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (ArxReadMappedStrings(in, &sdb.s, mshHdr->attrCnt, attrs))
            AfxThrowError();
        mshb->attrCnt = mshHdr->attrCnt;
        mshb->attrs = attrs;

        afxString* biases = AfxRequestArena(&arena, sizeof(biases[0]), mshHdr->biasCnt, NIL, 0);
        if (AfxSeekStream(in, mshHdr->biasIdsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (ArxReadMappedStrings(in, &sdb.s, mshHdr->biasCnt, biases))
            AfxThrowError();
        mshb->biasCnt = mshHdr->biasCnt;
        mshb->biases = biases;

        afxString* morphTags = AfxRequestArena(&arena, sizeof(morphTags[0]), mshHdr->morphCnt, NIL, 0);
        if (AfxSeekStream(in, mshHdr->morphTagsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (ArxReadMappedStrings(in, &sdb.s, mshHdr->morphCnt, morphTags))
            AfxThrowError();
        mshb->morphCnt = mshHdr->morphCnt;
        mshb->morphTags = morphTags;

        afxString* materials = AfxRequestArena(&arena, sizeof(materials[0]), mshHdr->mtlCnt, NIL, 0);
        if (AfxSeekStream(in, mshHdr->mtlIdsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (ArxReadMappedStrings(in, &sdb.s, mshHdr->mtlCnt, materials))
            AfxThrowError();
        mshb->materials = materials;

        mshb->mtlCnt = mshHdr->mtlCnt;
        mshb->secCnt = mshHdr->secCnt;
        mshb->triCnt = mshHdr->triCnt;
        mshb->vtxCnt = mshHdr->vtxCnt;


    }

    if (ArxBuildMeshes(scio, mdlHdr.mshCnt, strings, mshbs, meshes))
        AfxThrowError();

    for (afxUnit i = 0; i < mdlHdr.mshCnt; i++)
    {
        MXD_MSH_HDR* mshHdr = &mshHdrs[i];
        arxMesh msh = meshes[i];

        if (AfxSeekStream(in, mshHdr->indicesBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        if (ArxUploadMeshIndices(msh, 0, mshHdr->triCnt, in, sizeof(afxUnit32)))
            AfxThrowError();

        for (afxUnit j = 0; j < mshHdr->attrCnt; j++)
        {
            if (AfxSeekStream(in, mshHdr->vtxAttrInfoBaseOff + j * sizeof(MXD_VTX_ATTR_HDR), afxSeekOrigin_BEGIN))
                AfxThrowError();

            MXD_VTX_ATTR_HDR vaHdr;
            if (AfxReadStream(in, sizeof(vaHdr), 0, &vaHdr))
                AfxThrowError();

            ArxFormatVertexAttribute(msh, j, vaHdr.decodedFmt, vaHdr.flags);

            if (AfxSeekStream(in, vaHdr.baseDataOffset, afxSeekOrigin_BEGIN))
                AfxThrowError();

            for (afxUnit k = 0; k < mshHdr->morphCnt; k++)
            {
                avxFormatDescription pfd;
                AvxDescribeFormats(1, &vaHdr.encodedFmt, &pfd);
                afxUnit stride = pfd.stride;// AfxVertexFormatGetSize(vaHdr.encodedFmt);

                if (ArxUploadVertexData(msh, j, k, 0, mshHdr->vtxCnt, in, stride))
                    AfxThrowError();
            }
        }

        if (AfxSeekStream(in, mshHdr->sectionsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        for (afxUnit k = 0; k < mshHdr->secCnt; k++)
        {
            MXD_MSHS_HDR mshsHdr;
            if (AfxReadStream(in, sizeof(mshsHdr), 0, &mshsHdr))
                AfxThrowError();

            arxMeshSection mshs = { 0 };
            mshs.baseTriIdx = mshsHdr.baseTriIdx;
            mshs.triCnt = mshsHdr.triCnt;
            mshs.mtlIdx = mshsHdr.mtlIdx;
            mshs.flags = mshsHdr.flags;

            if (ArxSectionizeMesh(msh, k, 1, &mshs))
                AfxThrowError();
        }

        if (AfxSeekStream(in, mshHdr->morphsBaseOff, afxSeekOrigin_BEGIN))
            AfxThrowError();

        for (afxUnit k = 0; k < mshHdr->morphCnt; k++)
        {
            MXD_MSHM_HDR mshmHdr;
            if (AfxReadStream(in, sizeof(mshmHdr), 0, &mshmHdr))
                AfxThrowError();

            arxMeshMorph mshm = { 0 };
            mshm.affectedAttrs = mshmHdr.morphedAttrs;
            mshm.flags = mshmHdr.flags;

            if (ArxReformMesh(msh, k, 1, &mshm))
                AfxThrowError();
        }
    }

    if (AfxSeekStream(in, mdlHdr.rigBase, afxSeekOrigin_BEGIN))
        AfxThrowError();

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        MXD_MSHR_HDR mshrHdr;
        if (AfxReadStream(in, sizeof(mshrHdr), 0, &mshrHdr))
            AfxThrowError();

        arxMesh msh = meshes[mshrHdr.mshIdx];

        arxMeshInfo mshi;
        ArxDescribeMesh(msh, &mshi);

        rigis[i].flags = mshrHdr.flags;
        rigis[i].msh = msh;
        rigis[i].origSkl = mdl->skl;

        arxMaterial mtlArray[256];
        ArxDeclareMaterials(mdli.mtd, mshi.mtlCnt, rigis[i].msh->materials, mtlArray);

        arxMeshLinkage rig = mdl->rigs[i];
        if (!rig) continue;
        rig->flags |= mshrHdr.flags;
    }

    if (ArxRigMeshes(mdl, 0, mdlHdr.rigCnt, rigis))
        AfxThrowError();

    AfxDismantleArena(&arena);

    *model = mdl;

    return err;
}

_ARX afxError ArxLoadModel(arxScenario scio, arxMtd mtd, arxSkeleton skl, afxString const* urn, afxUri const* uri, arxModel* model)
{
    afxError err = { 0 };

    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_READABLE;
    info.usage = afxIoUsage_FILE;
    if (AfxAcquireStream(1, &info, &iob))
    {
        AfxThrowError();
        return err;
    }

    if (AfxReopenFile(iob, uri, afxFileFlag_R))
    {
        AfxThrowError();
    }
    else
    {
        if (ArxUploadModel(scio, mtd, skl, urn, iob, model))
            AfxThrowError();
    }
    AfxDisposeObjects(1, &iob);

    return err;
}
