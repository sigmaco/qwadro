#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "qwadro/inc/afxQwadro.h"
#include "qwadro/inc/render/arxRenderware.h"

#ifdef AFX_OS_WIN
#ifdef AFX_OS_WIN64
#ifdef _AFX_DEBUG
#pragma comment(lib, "../../qwd/w64d/e2coree")
#else
#pragma comment(lib, "../../qwd/w64/e2coree")
#endif
#else
#ifdef _AFX_DEBUG
#pragma comment(lib, "../../qwd/w32d/e2coree")
#else
#pragma comment(lib, "../../qwd/w32/e2coree")
#endif
#endif
#endif

#define ENABLE_DRAW 1

afxBool readyToRender = FALSE;
//afxScript TheApp;
const afxReal CameraSpeed = 30.0f;

afxUri2048 uri;

struct
{
    akxViewConstants    viewConstants;
    akxShaderConstants  shaderConstants;
    akxMaterialConstants materialConstants;
    akxInstanceConstants  objConstants;
    avxBuffer           viewConstantsBuffer; // p, v
    avxBuffer           shdConstantsBuffer;
    avxBuffer           mtlConstantsBuffer;
    avxBuffer           objConstantsBuffer; // m
} gFramesets[3];
#define FRAME_COUNT 3
afxUnit gFrameCnt = FRAME_COUNT;
afxUnit gFrameIdx = FRAME_COUNT - 1;

typedef struct
{
    afxQuat rotQuat;
    afxV3d rotPivot;
    afxReal currRot;
    afxReal rotSpeed;
    afxM4d rotMtx;
    avxSampler smp;
    avxRaster cubemap;
    avxBuffer cube;
    avxVertexInput skyVin;
    arxTechnique skyDtec;
    afxReal cubemapColorIntensity;

    avxColor ambientColor;
    avxColor emissiveColor;
    arxSkyType type;
} Sky;

Sky skybox;

void StepSky(Sky* sky, afxReal dt)
{
    sky->currRot += sky->rotSpeed * dt;
    AfxQuatRotationFromAxis(sky->rotQuat, sky->rotPivot, sky->currRot);
    AfxM4dRotationFromQuat(sky->rotMtx, sky->rotQuat);
}

afxError DrawSky(Sky* sky, avxViewport const* vp, arxCamera cam, afxUnit frameIdx, afxSurface dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;

    AvxCmdUseDrawTechniqueSIGMA(dctx, sky->skyDtec, 0, sky->skyVin, NIL);

    akxViewConstants *viewConstants = &gFramesets[frameIdx].viewConstants;

    if (cam)
    {
        //cam = rnd->activeCamera;

        avxModeSetting sms = { 0 };
        AvxQuerySurfaceSettings(dout, &sms);
        ArxSetCameraAspectRatios(cam, sms.wpOverHp, AFX_V2D(sms.resolution.w, sms.resolution.h), vp->extent);

        viewConstants->viewExtent[0] = vp->extent[0];
        viewConstants->viewExtent[1] = vp->extent[1];

        afxV4d viewPos;
        ArxGetCameraPosition(cam, viewPos);
        AfxV4dCopyAtv3d(viewConstants->viewPos, viewPos);

        afxM4d v, iv, p, ip;
        ArxRecomputeCameraMatrices(cam);
        ArxGetCameraMatrices(cam, p, NIL, v, ip, NIL, iv);
        
        AfxM4dCopy(viewConstants->p, p);
        AfxM4dCopy(viewConstants->ip, ip);
        AfxM4dCopyAtm(viewConstants->v, v);
        AfxM4dCopy(viewConstants->iv, iv);
        AfxM4dMultiply(viewConstants->pv, p, v);
        AfxM4dInvert(viewConstants->ipv, viewConstants->pv);
    }

    AvxCmdUpdateBuffer(dctx, gFramesets[frameIdx].viewConstantsBuffer, 0, sizeof(*viewConstants), viewConstants);

    AvxCmdBindBuffers(dctx, avxBus_DRAW, 0, 0, 1, &AVX_BUFFERED_MAP(gFramesets[frameIdx].viewConstantsBuffer, 0, 0, 0));

    struct
    {
        afxM4d p, v;
    } pushes;
    AfxM4dCopy(pushes.p, viewConstants->p);
    AfxM4dCopy(pushes.v, viewConstants->v);
    AvxCmdPushConstants(dctx, 0, sizeof(pushes), &pushes);

    AvxCmdBindSamplers(dctx, avxBus_DRAW, 0, 1, 1, &sky->smp);
    AvxCmdBindRasters(dctx, avxBus_DRAW, 0, 1, 1, &sky->cubemap);

    AvxCmdBindVertexBuffers(dctx, 0, 1, &AVX_BUFFERED_STREAM(sky->cube, 0, 0, sizeof(afxV3d)));
    //AvxCmdResetVertexStreams(dctx, 1, NIL, (afxUnit32[]) { sizeof(afxV3d) }, NIL);
    //AvxCmdResetVertexAttributes(dctx, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AvxCmdDraw(dctx, 36, 1, 0, 0);
    return err;
}

afxError BuildSkybox(Sky* sky, afxDrawSystem dsys, arxRenderware rwe)
{
    afxError err = NIL;

    AfxV3dSet(sky->rotPivot, 0, 0, 1);
    sky->cubemapColorIntensity = 1.f;
    sky->rotSpeed = 0.f;
    sky->currRot = 0.f;

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

    afxUri facesUri[6];
    AfxMakeUri(&facesUri[0], 0, "right.tga", 0); // x+
    AfxMakeUri(&facesUri[1], 0, "left.tga", 0); // x-
    AfxMakeUri(&facesUri[3], 0, "top.tga", 0);
    AfxMakeUri(&facesUri[2], 0, "bottom.tga", 0); // y-
    AfxMakeUri(&facesUri[4], 0, "front.tga", 0);
    AfxMakeUri(&facesUri[5], 0, "back.tga", 0);

    afxUri cubeDir;
    AfxMakeUri(&cubeDir, 0, "//./d/art/skybox/purple", 0);

    avxRasterInfo rasi = { 0 };
    rasi.whd.d = 6;
    rasi.usage |= avxRasterUsage_TEXTURE;
    rasi.flags |= avxRasterFlag_CUBEMAP;
    
    sky->cubemap = AvxLoadSegmentedRaster(dsys, &rasi, &cubeDir, facesUri);
    //AfxFlipRaster(sky->cubemap, FALSE, TRUE);

    AvxMakeColor(sky->ambientColor, 0.1, 0.1, 0.1, 1);
    AvxMakeColor(sky->emissiveColor, 0.1, 0.1, 0.1, 1);

    const float SIZE = 500.f;
    afxReal skyboxVertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //for (afxUnit i = 0; i < sizeof(skyboxVertices) / sizeof(skyboxVertices[0]); i++)
    //    skyboxVertices[i] *= 100;

    avxBufferInfo vboSpec = { 0 };
    vboSpec.size = sizeof(skyboxVertices);
    vboSpec.usage = avxBufferUsage_VERTEX;
    vboSpec.flags = avxBufferFlag_W;
    AvxAcquireBuffers(dsys, 1, &vboSpec, &sky->cube);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &sky->cube);

    avxBufferIo vboIop = { 0 };
    vboIop.srcStride = 1;
    vboIop.dstStride = 1;
    vboIop.rowCnt = sizeof(skyboxVertices);
    AvxUpdateBuffer(sky->cube, 1, &vboIop, skyboxVertices, 0);

    afxUri uri;
    AfxMakeUri(&uri, 0, "//./d/gfx/skybox/skybox.xsh.xml", 0);
    ArxLoadDrawTechnique(rwe, &uri, &sky->skyDtec);
    sky->type = arxSkyType_BOX;

    avxVertexLayout vtxl = { 0 };
    vtxl.binCnt = 1;
    vtxl.bins[0].instRate = 0;
    vtxl.bins[0].pin = 0;
    vtxl.bins[0].attrCnt = 1;
    vtxl.attrs[0].location = 0;
    vtxl.attrs[0].offset = 0;
    vtxl.attrs[0].fmt = avxFormat_RGB32f;

    AvxDeclareVertexInputs(dsys, 1, &vtxl, &sky->skyVin);
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &sky->skyVin);

    avxSamplerConfig smpSpec = { 0 };
    smpSpec.magnify = avxTexelFilter_LINEAR;
    smpSpec.minify = avxTexelFilter_LINEAR;
    smpSpec.mipFlt = avxTexelFilter_LINEAR;
    smpSpec.uvw[0] = avxTexelWrap_EDGE;
    smpSpec.uvw[1] = avxTexelWrap_EDGE;
    smpSpec.uvw[2] = avxTexelWrap_EDGE;
    AvxConfigureSampler(dsys, &smpSpec);
    AvxDeclareSamplers(dsys, 1, &smpSpec, &sky->smp);
    AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &sky->smp);

    return err;
}

afxBool CamEventFilter(afxObject *obj, afxObject *watched, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    arxCamera cam = (void*)obj;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case auxEventId_AXIS:
    {
        //afxMouse mse = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_MSE, 1, &mse);

        // TODO Leva isso para o usuário

        if (AfxIsMousePressed(0, AFX_LMB))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta, NIL, NIL);
            deltaEar[1] = -AfxRadf(delta[0]);
            deltaEar[0] = -AfxRadf(delta[1]);
            deltaEar[2] = 0.f;
            afxV3d v;
            ArxConsultCameraOrientation(cam, deltaEar, avxBlendOp_ADD, v);
            ArxResetCameraOrientation(cam, v);
        }

        if (AfxIsMousePressed(0, AFX_RMB))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta, NIL, NIL);
            off[0] = -AfxRadf(delta[0]);
            off[1] = -AfxRadf(delta[1]);
            off[2] = 0.f;
            afxV3d v;
            ArxConsultCameraDisplacement(cam, off, avxBlendOp_ADD, v);
            ArxResetCameraDisplacement(cam, v);
        }
        break;
    }
    case auxEventId_WHEEL:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        ArxApplyCameraDistance(cam, w);
        break;
    }
    case auxEventId_KEY:
    {
        //afxKeyboard kbd = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_KBD, 1, &kbd);
        break;
    }
    default:break;
    }
    return FALSE;
}

void UpdateFrameMovement(arxCamera cam, afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;
    
    afxSession ses;
    AfxGetSession(&ses);

    afxReal64 MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.
    afxReal64 ForwardSpeed = (AfxGetKeyPressure(0, afxKey_W) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_S) ? 1 : 0.0f);
    afxReal64 RightSpeed = (AfxGetKeyPressure(0, afxKey_A) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_D) ? 1 : 0.0f);
    afxReal64 UpSpeed = (AfxGetKeyPressure(0, afxKey_Q) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_E) ? 1 : 0.0f);
    afxV3d v =
    {
        MovementThisFrame * RightSpeed,
        MovementThisFrame * UpSpeed,
        MovementThisFrame * ForwardSpeed
    };
    ArxTranslateCamera(cam, v);


}

afxBool AskNextFrame(afxSurface dout, afxUnit* outBufIdx, afxUnit* frameIdxHolder)
{
    afxBool rslt = FALSE;

    afxUnit outBufIdx2 = AFX_INVALID_INDEX;
    if (0 == AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_NONE, NIL, &outBufIdx2, NIL))
    {
        gFrameIdx = (gFrameIdx + 1) % gFrameCnt;
        *frameIdxHolder = gFrameIdx;
        *outBufIdx = outBufIdx2;
        rslt = TRUE;
    }
    return rslt;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;

    // Boot up the Qwadro

    afxSystemConfig sysc = { 0 };
    AfxConfigureSystem(&sysc);
    AfxBootstrapSystem(&sysc);

    // Mount the demo's asset storage.

    AfxMountStorageUnit('d', AfxUri("../demo"), afxFileFlag_RX);

    // Acquire hardware device contexts

    afxUnit drawIcd = 0;
    afxDrawSystem dsys;
    afxDrawSystemConfig dsyc = { 0 };
    dsyc.caps = afxDrawFn_DRAW;
    dsyc.accel = afxAcceleration_DPU;
    dsyc.exuCnt = 1;
    AvxConfigureDrawSystem(drawIcd, &dsyc);
    AvxEstablishDrawSystem(drawIcd, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Starting a session

    afxSession ses;
    afxSessionConfig sCfg = { 0 };
    sCfg.dsys = dsys;
    AfxAcquireSession(0, &sCfg, &ses);
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AfxOpenSession(ses, NIL, NIL, NIL);

    // Acquire a drawable surface

    afxWindow wnd;
    afxSurface dout;
    afxWindowConfig wcfg = { 0 };
    wcfg.dsys = dsys;
    AfxConfigureWindow(&wcfg, NIL, AFX_V2D(0.5, 0.5));
    AfxAcquireWindow(&wcfg, &wnd);
    AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
    AfxGetWindowSurface(wnd, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Open a draw input mechanism

    arxRenderware rwe;
    arxRenderwareConfig rwec = { 0 };
    rwec.dsys = dsys;
    rwec.proc = NIL;
    rwec.udd = NIL;
    rwec.cmdPoolMemStock = 4096;
    rwec.estimatedSubmissionCnt = 3;
    ArxOpenRenderware(0, &rwec, &rwe);
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);

    // Build the skybox
    
    AfxZero(&skybox, sizeof(skybox));
    BuildSkybox(&skybox, dsys, rwe);

    //afxTxd txd;
    //afxUri uri;
    //AfxMakeUri(&uri, 0, "../art/skybox/purple/purple.txd", 0);
    //AfxOpenTxd(rwe, &uri, &txd);

    // Acquire a view point
    // Handle mouse-camera interaction

    arxCamera cam;
    ArxAcquireCameras(rwe, 1, &cam);
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AfxConnectObjects(cam, 1, (afxObject[]) { wnd }, (void*)CamEventFilter);

    // Build our renderer

    avxBufferInfo bufSpec[] =
    {
        {.size = sizeof(akxViewConstants), .flags = avxBufferFlag_W,.usage = avxBufferUsage_UNIFORM },
        {.size = sizeof(akxShaderConstants), .flags = avxBufferFlag_W,.usage = avxBufferUsage_UNIFORM },
        {.size = sizeof(akxMaterialConstants), .flags = avxBufferFlag_W,.usage = avxBufferUsage_UNIFORM },
        {.size = sizeof(akxInstanceConstants), .flags = avxBufferFlag_W,.usage = avxBufferUsage_UNIFORM }
    };

    for (afxUnit i = 0; i < gFrameCnt; i++)
    {
        //AfxBuildRasters(dsys, 1, &rnd->framesets[i].depthSurf, &depthSurfB);

        AvxAcquireBuffers(dsys, 1, &bufSpec[0], &gFramesets[i].viewConstantsBuffer);
        AvxAcquireBuffers(dsys, 1, &bufSpec[1], &gFramesets[i].shdConstantsBuffer);
        AvxAcquireBuffers(dsys, 1, &bufSpec[2], &gFramesets[i].mtlConstantsBuffer);
        AvxAcquireBuffers(dsys, 1, &bufSpec[3], &gFramesets[i].objConstantsBuffer);
    }

    // Operation contexts

    afxUnit frameCap = AFX_CLAMP(wcfg.dout.latency, 1, 3);

    afxDrawContext drawContexts[3];
    avxContextInfo ctxi = { 0 };
    ctxi.caps = afxDrawFn_DRAW;
    AvxAcquireDrawContexts(dsys, &ctxi, frameCap, drawContexts);
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, frameCap, drawContexts);

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    readyToRender = TRUE;

    while (1)
    {
        AfxPollInput(NIL, 0);

        if (!AfxSystemIsExecuting())
            break;

        afxClock currClock;
        AfxGetClock(&currClock);
        afxReal64 ct = AfxGetSecondsElapsed(&startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&lastClock, &currClock);
        lastClock = currClock;

        UpdateFrameMovement(cam, dt);

        if (!readyToRender)
            continue;

        afxUnit frameIdx = 0;
        afxUnit outBufIdx = 0;
        if (!AskNextFrame(dout, &outBufIdx, &frameIdx))
            continue;

        afxDrawContext dctx = drawContexts[outBufIdx];

        afxUnit batchId;
        if (AvxRecordDrawCommands(dctx, TRUE, FALSE, &batchId))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        afxRect area;
        avxCanvas canv;
        AvxGetSurfaceCanvas(dout, outBufIdx, &canv, &area);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

        avxDrawScope dps = { 0 };
        dps.canv = canv;
        dps.area.area = area;
        dps.targetCnt = 1;
        dps.targets[0].clearVal = AVX_COLOR_VALUE(0.3f, 0.1f, 0.3f, 1);
        dps.targets[0].loadOp = avxLoadOp_CLEAR;
        dps.targets[0].storeOp = avxStoreOp_STORE;
        dps.ds[0].clearVal.depth = 1.0;
        dps.ds[0].clearVal.stencil = 0;
        dps.ds[0].loadOp = avxLoadOp_CLEAR;
        dps.ds[0].storeOp = avxStoreOp_STORE;
        AvxCmdCommenceDrawScope(dctx, &dps);

        avxViewport vp = AVX_VIEWPORT(0, 0, area.w, area.h, 0, 1);
        AvxCmdAdjustViewports(dctx, 0, 1, &vp);

        DrawSky(&skybox, &vp, cam, frameIdx, dout, dctx);

        AvxCmdConcludeDrawScope(dctx);

        if (AvxCompileDrawCommands(dctx, batchId))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        avxFence drawCompletedFence = NIL;
        avxSubmission subm = { 0 };
        subm.fence = drawCompletedFence;
        subm.dctx = dctx;
        subm.batchId = batchId;

        if (AvxExecuteDrawCommands(dsys, 1, &subm))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, subm.exuMask);

        avxPresentation pres = { 0 };
        pres.dout = dout;
        pres.bufIdx = outBufIdx;
        //pres.waitOnDpu = drawCompletedFence;

        if (AvxPresentSurfaces(dsys, 1, &pres))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
        }
    }

    AfxDisposeObjects(1, &wnd);
    AfxDisposeObjects(1, &ses);
    AfxDisposeObjects(1, &dsys);
    AfxDoSystemShutdown(0);
    return 0;
}
