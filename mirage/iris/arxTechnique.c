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

#define _AFX_DRAW_TECHNIQUE_C
#include "../scene/arxIcd.h"

ARX afxError _AvxParseXmlPipelineBlueprint(afxXmlNode const* xmle, afxUnit specId, avxPipelineConfig* pipb, avxShaderType shaderStages[], afxUri shaderUris[], afxString shaderFns[]);

////////////////////////////////////////////////////////////////////////////////

_ARX afxCmdId AvxCmdUseDrawTechniqueSIGMA(afxDrawContext dctx, arxTechnique dtec, afxUnit passIdx, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    AFX_ASSERT_RANGE(dtec->passCnt, passIdx, 1);
    arxDrawPass* pass = &dtec->passes[passIdx];
    return  AvxCmdBindPipeline(dctx, pass->pip, vin, dynamics);
}

_ARX afxError ArxUpdateRasterizationPass(arxTechnique dtec, afxUnit passIdx, avxPipelineConfig const* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    arxScenario scio = AfxGetHost(dtec);
    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxPipeline pip;
    AvxAssembleGfxPipelines(dsys, 1, cfg, &pip);

    if (dtec->passes[passIdx].pip)
    {
        AfxDisposeObjects(1, &dtec->passes[passIdx].pip);
    }
    dtec->passes[passIdx].pip = pip;
    return err;
}

_ARX afxError _ArxDtecDtorCb(arxTechnique dtec)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    
    AfxDeallocate((void**)&dtec->passes, AfxHere());

    return err;
}

_ARX afxError _ArxDtecCtorCb(arxTechnique dtec, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxUnit const passCnt = *(((afxUnit const*)args[1]) + invokeNo);

     AfxAllocate(passCnt * sizeof(dtec->passes[0]), 0, AfxHere(), (void**)&dtec->passes);

    for (afxUnit i = 0; i < passCnt; i++)
    {
        dtec->passes[i].pip = NIL;
    }

    dtec->passCnt = passCnt;

    return err;
}

_ARX afxClassConfig const _ARX_DTEC_CLASS_CONFIG =
{
    .fcc = afxFcc_DTEC,
    .name = "DrawTechnique",
    .desc = "Draw Technique",
    .fixedSiz = sizeof(AFX_OBJECT(arxTechnique)),
    .ctor = (void*)_ArxDtecCtorCb,
    .dtor = (void*)_ArxDtecDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxLoadRenderTechnique(arxScenario scio, afxUri const* uri, arxTechnique* technique)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(technique);
    AFX_ASSERT(uri);

    afxXml xml;
    afxUri fpath;
    afxString query;
    AfxClipUriPath(&fpath, uri);
    AfxGetUriQueryString(uri, TRUE, &query);

    if (AfxLoadXml(&xml, &fpath))
    {
        AfxThrowError();
        return err;
    }
    //else if (!AfxFindXmlAttributedChildElements(&xml, 0, 0, &AfxStaticString("DrawTechnique"), &AfxStaticString("id"), 1, &query, &xmlElemIdx))
    else if (!AfxTestXmlRoot(&xml, &AFX_STRING("DrawTechnique")))
    {
        AfxThrowError();
        AfxCleanUpXml(&xml);
        return err;
    }

    afxUnit passCnt = AfxCountXmlAttributedChildElements(xml.root, NIL, &AFX_STRING("Pass"), NIL, NIL);

    afxClass* cls = (afxClass*)_ArxScioGetTechClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);

    if (AfxAcquireObjects(cls, 1, (afxObject*)technique, (void const*[]) { scio, (void*)&passCnt })) AfxThrowError();
    else
    {
        afxString name, ncontent, acontent;
        afxInt passIdx = 0;

        avxShaderType shaderStages[6] = { 0 };
        afxUri shaderUris[6] = { 0 };
        afxString shaderFns[6] = { 0 };

        afxXmlNode const* passNode = NIL;
        for (afxUnit nodeIter0 = 0; nodeIter0 < passCnt; nodeIter0++)
        {
            passNode = AfxFindXmlAttributedChildElement(xml.root, passNode, &AFX_STRING("Pass"), NIL, NIL);

            if (!passNode) break;

            afxUnit arrelIdx;
            avxPipelineConfig config = { 0 };
            _AvxParseXmlPipelineBlueprint(passNode, 0, &config, shaderStages, shaderUris, shaderFns);

            ArxUpdateRasterizationPass(*technique, passIdx, &config);

            avxPipeline pip = (*technique)->passes[passIdx].pip;

            avxCodebase codb;
            AvxGetPipelineCodebase(pip, &codb);
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
            avxShaderSpecialization specs[8] = { 0 };

            for (afxUnit i = 0; i < config.stageCnt; i++)
            {
                afxUri fileExt;
                AfxExcerptPathSegments(&shaderUris[i], NIL, NIL, &fileExt, &fileExt);

                if (AvxCompileShaderFromDisk(codb, &fileExt.s, &shaderUris[i]))
                    AfxThrowError();

                specs[i].stage = shaderStages[i];
                specs[i].prog = fileExt.s;
                specs[i].fn = shaderFns[i];
            }

            if (AvxReprogramPipeline(pip, config.stageCnt, specs))
                AfxThrowError();

            ++passIdx;
        }
    }

    AfxCleanUpXml(&xml);

    return err;
}
