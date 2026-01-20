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

// This file is part of Advanced RenderWare Extensions.

#define _ARX_SCENE_C
#define _ARX_SKY_C
#include "arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"

afxV3d const skyboxVertices[] =
{
    {-1.0f,  1.0f, -1.0f },
    {-1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },
    { 1.0f,  1.0f, -1.0f },
    {-1.0f,  1.0f, -1.0f },

    {-1.0f, -1.0f,  1.0f },
    {-1.0f, -1.0f, -1.0f },
    {-1.0f,  1.0f, -1.0f },
    {-1.0f,  1.0f, -1.0f },
    {-1.0f,  1.0f,  1.0f },
    {-1.0f, -1.0f,  1.0f },

    { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },

    {-1.0f, -1.0f,  1.0f },
    {-1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f },
    { 1.0f, -1.0f,  1.0f },
    {-1.0f, -1.0f,  1.0f },

    {-1.0f,  1.0f, -1.0f },
    { 1.0f,  1.0f, -1.0f },
    { 1.0f,  1.0f,  1.0f },
    { 1.0f,  1.0f,  1.0f },
    {-1.0f,  1.0f,  1.0f },
    {-1.0f,  1.0f, -1.0f },

    {-1.0f, -1.0f, -1.0f },
    {-1.0f, -1.0f,  1.0f },
    { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },
    {-1.0f, -1.0f,  1.0f },
    { 1.0f, -1.0f,  1.0f }
};

_ARX void ArxStepSky(arxSky sky, afxReal dt)
{
    sky->sky.currRot += sky->sky.rotSpeed * dt;
    AfxQuatRotationAxial(sky->sky.rotQuat, sky->sky.rotPivot, sky->sky.currRot);
    AfxM4dRotationQuat(sky->sky.rotMtx, sky->sky.rotQuat);
}

_ARX afxError ArxDrawSky(afxDrawContext dctx, arxSky sky)
{
    afxError err = { 0 };

    if (sky->skyType == arxSkyType_BOX)
    {
        AvxCmdUseDrawTechniqueSIGMA(dctx, sky->sky.skyDtec, 0, sky->sky.skyVin, NIL);

        AvxCmdBindSamplers(dctx, avxBus_GFX, 0, 1, 1, &sky->sky.smp);
        AvxCmdBindRasters(dctx, avxBus_GFX, 0, 1, 1, &sky->sky.cubemap);

        AvxCmdBindVertexBuffers(dctx, 0, 1, (avxBufferedStream[]) { {.buf = sky->sky.cube, .stride = sizeof(afxV3d) } });
        AvxCmdDraw(dctx, 36, 1, 0, 0);
    }
    else
    {

    }
    return err;
}

_ARX afxError ArxReloadSkyVisual(arxSky sky, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKY, 1, &sky);
    
    arxScenario scio = sky->scio;
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio); // temp workaround to get a context.

    if (sky->skyType == arxSkyType_BOX)
    {
        afxUri faces[6];
        AfxMakeUri(&faces[0], 0, "right.tga", 0); // x+
        AfxMakeUri(&faces[1], 0, "left.tga", 0); // x-
        AfxMakeUri(&faces[3], 0, "top.tga", 0);
        AfxMakeUri(&faces[2], 0, "bottom.tga", 0); // y-
        AfxMakeUri(&faces[4], 0, "front.tga", 0);
        AfxMakeUri(&faces[5], 0, "back.tga", 0);

        avxRasterInfo rasi = { 0 };
        rasi.flags = avxRasterFlag_CUBEMAP;
        rasi.usage = avxRasterUsage_TEXTURE;
        rasi.whd.d = 6;

        afxUnit portId = 0;

        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        avxRaster cubemap = NIL;

        if (uri->fname + uri->fext)
        {
            if (AvxLoadRasters(dsys, 1, &rasi, uri, NIL, &cubemap))
            {
                AfxThrowError();
            }
        }
        else if (AvxLoadSegmentedRaster(dsys, &rasi, uri, faces, &cubemap))
        {
            AfxThrowError();
        }

        if (sky->sky.cubemap)
            AfxDisposeObjects(1, &sky->sky.cubemap);

        sky->sky.cubemap = cubemap;
    }
    return err;
}

_ARX afxError _ArxSkyDtorCb(arxSky sky)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKY, 1, &sky);

    if (sky->skyType == arxSkyType_BOX)
    {
        AfxDisposeObjects(1, &sky->sky.cubemap);
        AfxDisposeObjects(1, &sky->sky.cube);
        AfxDisposeObjects(1, &sky->sky.skyDtec);
        AfxDisposeObjects(1, &sky->sky.skyVin);
        AfxDisposeObjects(1, &sky->sky.smp);
    }

    return err;
}

_ARX afxError _ArxSkyCtorCb(arxSky sky, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SKY, 1, &sky);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    arxSkyInfo const* info = AFX_CAST(arxSkyInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio); // temp workaround to get a context.

    sky->scio = scio;

    AfxV3dSet(sky->sky.rotPivot, 0, 0, 1);
    sky->sky.cubemapColorIntensity = 1.f;
    sky->sky.rotSpeed = 0.f;
    sky->sky.currRot = 0.f;
    AvxMakeColor(sky->sky.ambientColor, 0.1, 0.1, 0.1, 1);
    AvxMakeColor(sky->sky.emissiveColor, 0.1, 0.1, 0.1, 1);
    
    sky->skyType = info->skyType;

    if (info->skyType == arxSkyType_BOX)
    {
        arxMeshBlueprint mshb = { 0 };
        mshb.attrCnt = 1;
        mshb.vtxCnt = ARRAY_SIZE(skyboxVertices);
        mshb.triCnt = mshb.vtxCnt / 3;
        ArxBuildMeshes(scio, 1, NIL, &mshb, &sky->skyMsh);
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &sky->skyMsh);

        ArxFormatVertexAttribute(sky->skyMsh, 0, avxFormat_RGB32f, arxVertexFlag_POSITIONAL | arxVertexFlag_SPATIAL | arxVertexFlag_ATV);

        ArxUpdateVertexData(sky->skyMsh, 0, 0, 0, mshb.vtxCnt, skyboxVertices, sizeof(skyboxVertices[0]));

        // sky
        afxUri cubeUri[6];
#if !0
#if 0    
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/day/right.tga", 0);
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/day/left.tga", 0);
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/day/top.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/day/bottom.tga", 0);
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/day/front.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/day/back.tga", 0);
#else
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/purple/right.tga", 0); // x+
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/purple/left.tga", 0); // x-
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/purple/top.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/purple/bottom.tga", 0); // y-
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/purple/front.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/purple/back.tga", 0);
#endif
#else
        AfxMakeUri(&cubeUri[0], 0, "../art/skybox/envmap_interstellar/interstellar_rt.tga", 0); // x+
        AfxMakeUri(&cubeUri[1], 0, "../art/skybox/envmap_interstellar/interstellar_lf.tga", 0); // x-
        AfxMakeUri(&cubeUri[3], 0, "../art/skybox/envmap_interstellar/interstellar_up.tga", 0);
        AfxMakeUri(&cubeUri[2], 0, "../art/skybox/envmap_interstellar/interstellar_dn.tga", 0); // y-
        AfxMakeUri(&cubeUri[4], 0, "../art/skybox/envmap_interstellar/interstellar_ft.tga", 0);
        AfxMakeUri(&cubeUri[5], 0, "../art/skybox/envmap_interstellar/interstellar_bk.tga", 0);
#endif
        afxUri cubeDir;
        afxUri facesUri[6];
        AfxMakeUri(&facesUri[0], 0, "right.tga", 0); // x+
        AfxMakeUri(&facesUri[1], 0, "left.tga", 0); // x-
        AfxMakeUri(&facesUri[3], 0, "top.tga", 0);
        AfxMakeUri(&facesUri[2], 0, "bottom.tga", 0); // y-
        AfxMakeUri(&facesUri[4], 0, "front.tga", 0);
        AfxMakeUri(&facesUri[5], 0, "back.tga", 0);
        AfxMakeUri(&cubeDir, 0, "../art/skybox/purple", 0);
        avxRasterInfo rasi = { 0 };
        rasi.flags = avxRasterFlag_CUBEMAP;
        rasi.usage = avxRasterUsage_TEXTURE;
        AvxLoadSegmentedRaster(dsys, &rasi, &cubeDir, facesUri, &sky->sky.cubemap);
        
        //for (afxUnit i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0][0]); i++)
        //    skyboxVertices[i] *= 100;

        afxUri uri;
        AfxMakeUri(&uri, 0, "../data/pipeline/skybox/skybox.xsh.xml", 0);
        ArxLoadRenderTechnique(scio, &uri, &sky->sky.skyDtec);

        avxVertexLayout skyVtxl =
        {
            .binCnt = 1,
            .bins = { AVX_VERTEX_STREAM(0, 0, 0, 0, 1) },
            .attrs = { AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f) }
        };

        AvxAcquireVertexInputs(dsys, 1, &skyVtxl, &sky->sky.skyVin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &sky->sky.skyVin);

        avxSamplerConfig smpSpec = { 0 };
        smpSpec.magnify = avxTexelFilter_LINEAR;
        smpSpec.minify = avxTexelFilter_LINEAR;
        smpSpec.mipFlt = avxTexelFilter_LINEAR;
        smpSpec.uvw[0] = avxTexelWrap_EDGE;
        smpSpec.uvw[1] = avxTexelWrap_EDGE;
        smpSpec.uvw[2] = avxTexelWrap_EDGE;

        AvxAcquireSamplers(dsys, 1, &smpSpec, &sky->sky.smp);
        AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &sky->sky.smp);
    }
    else
    {
        ArxBuildSphereMesh(scio, 2.0, 20, 20, TRUE, NIL, &sky->skyMsh);

        AvxMakeColor(sky->apexCol, 0.f, 0.15f, 0.66f, 1.f);
        AvxMakeColor(sky->centreCol, 0.81f, 0.38f, 0.66f, 1.f);

        avxPipeline pip;
        avxPipelineConfig pipb = { 0 };
        AvxAssembleGfxPipelines(dsys, 1, &pipb, &pip);
        AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

        avxCodebase codb;
        AvxGetPipelineCodebase(pip, &codb);
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
        avxShaderSpecialization specs[2] = { 0 };
        specs[0].stage = avxShaderType_VERTEX;
        specs[0].prog = AFX_STRING("skydomeVs");
        specs[1].stage = avxShaderType_FRAGMENT;
        specs[1].prog = AFX_STRING("skydomeFs");
        AvxCompileShaderFromDisk(codb, &specs[0].prog, AfxUri("../src/skydome/skydomeVs.glsl"));
        AvxCompileShaderFromDisk(codb, &specs[1].prog, AfxUri("../src/skydome/skydomeFs.glsl"));
        AvxReprogramPipeline(pip, 2, specs);

        struct
        {
            afxM4d p, v, m;
            afxV4d apexCol, centreCol;
        } data;
        avxBuffer buf;
        avxBufferInfo bufi = { 0 };
        bufi.size = sizeof(data);
        bufi.usage = avxBufferUsage_UNIFORM;
        bufi.flags = avxBufferFlag_W;
        AvxAcquireBuffers(dsys, 1, &bufi, &buf);
        
    }
    return err;
}

_ARX afxClassConfig const _ARX_SKY_CLASS_CONFIG =
{
    .fcc = afxFcc_SKY,
    .name = "Sky",
    .desc = "Sky",
    .fixedSiz = sizeof(AFX_OBJECT(arxSky)),
    .ctor = (void*)_ArxSkyCtorCb,
    .dtor = (void*)_ArxSkyDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxBuildSky(arxScenario scio, arxSkyInfo const* info, arxSky* sky)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(info);
    AFX_ASSERT(sky);

    afxClass* cls = (afxClass*)_ArxScioGetSkyClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_SKY);

    if (AfxAcquireObjects(cls, 1, (afxObject*)sky, (void const*[]) { scio, info }))
        AfxThrowError();

    return err;
}
