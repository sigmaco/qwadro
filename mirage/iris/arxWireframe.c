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

#define _AFX_DRAW_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_MATERIAL_C
#define _ARX_SKELETON_C
#define _ARX_RENDER_CONTEXT_C
#define _ARX_SCENARIO_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"
#include "arxWireframeShaders.h"

// AFX_V4D_CLOUDFLARE --- 1, 1, 1, 1

_ARX afxError ArxLoadWireframePipeline(arxRenderContext rctx, arxSceneMode mode, afxUri const* uri, avxPipeline* pipeline)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    AFX_ASSERT(pipeline);
    //AFX_ASSERT(uri);

    //if (!uri)
    {
        afxDrawSystem dsys = ArxGetRenderDrawSystem(rctx);
        
        if (mode == arxSceneMode_WIRE_FACES)
        {
            avxVertexInput vin;
            avxVertexLayout vtxLay = { 0 };
            vtxLay.binCnt = 1;
            vtxLay.bins[0].attrCnt = 2;
            vtxLay.attrs[0].location = 0;
            vtxLay.attrs[0].fmt = avxFormat_RGB32f;
            vtxLay.attrs[1].offset = sizeof(afxV3d);
            vtxLay.attrs[1].location = 1;
            vtxLay.attrs[1].fmt = avxFormat_RGB32f;
            if (AvxAcquireVertexInputs(dsys, 1, &vtxLay, &vin))
            {
                AfxThrowError();
            }

            avxPipeline pip;
            avxPipelineConfig pipc = { 0 };
            pipc = wirePipc;
            pipc.vin = vin;
            if (AvxAssembleGfxPipelines(dsys, 1, &pipc, &pip))
            {
                AfxThrowError();
                return err;
            }

            AfxDisposeObjects(1, &vin);

            avxCodebase codb;
            AvxGetPipelineCodebase(pip, &codb);
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
            afxUnit stageCnt = 2;
            avxShaderSpecialization specs[2] = { 0 };
            specs[0].stage = avxShaderType_VERTEX;
            specs[0].prog = AFX_STRING("wireBaryVshCode");
            specs[1].stage = avxShaderType_FRAGMENT;
            specs[1].prog = AFX_STRING("wireBaryFshCode");

            afxString s;
            AfxMakeString(&s, 0, wireBaryVshCode, 0);
            if (AvxCompileShader(codb, &AFX_STRING("wireBaryVshCode"), &s))
            {
                AfxThrowError();
            }

            AfxMakeString(&s, 0, wireBaryFshCode, 0);
            if (AvxCompileShader(codb, &AFX_STRING("wireBaryFshCode"), &s))
            {
                AfxThrowError();
            }

            if (AvxReprogramPipeline(pip, stageCnt, specs))
            {
                AfxThrowError();
            }

            AFX_ASSERT(pipeline);
            *pipeline = pip;
        }
        else
        {
            avxVertexInput vin;
            avxVertexLayout vtxLay = { 0 };
            vtxLay.binCnt = 1;
            vtxLay.bins[0].attrCnt = 1;
            vtxLay.attrs[0].location = 0;
            vtxLay.attrs[0].fmt = avxFormat_RGB32f;
            if (AvxAcquireVertexInputs(dsys, 1, &vtxLay, &vin))
            {
                AfxThrowError();
            }

            avxPipeline pip;
            avxPipelineConfig pipc = { 0 };
            pipc = wirePipc;
            pipc.vin = vin;
            if (AvxAssembleGfxPipelines(dsys, 1, &pipc, &pip))
            {
                AfxThrowError();
                return err;
            }

            AfxDisposeObjects(1, &vin);

            avxCodebase codb;
            AvxGetPipelineCodebase(pip, &codb);
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
            afxUnit stageCnt = 2;
            avxShaderSpecialization specs[2] = { 0 };
            specs[0].stage = avxShaderType_VERTEX;
            specs[0].prog = AFX_STRING("wireVshCode");
            specs[1].stage = avxShaderType_FRAGMENT;
            specs[1].prog = AFX_STRING("wireFshCode");

            afxString s;
            AfxMakeString(&s, 0, wireVshCode, 0);
            if (AvxCompileShader(codb, &AFX_STRING("wireVshCode"), &s))
            {
                AfxThrowError();
            }

            AfxMakeString(&s, 0, wireFshCode, 0);
            if (AvxCompileShader(codb, &AFX_STRING("wireFshCode"), &s))
            {
                AfxThrowError();
            }

            if (AvxReprogramPipeline(pip, stageCnt, specs))
            {
                AfxThrowError();
            }

            AFX_ASSERT(pipeline);
            *pipeline = pip;
        }
    }

    return err;
}

static inline void emit_wire_segment(
    arxWireVertex *vptr,
    afxV3d A,
    afxV3d B)
{
    arxWireVertex *v = vptr;

    // v0 = A
    AfxV3dCopy(v->pos, A);
    AfxV3dCopy(v->bary, AFX_V3D_X);
    ++v;

    // v1 = B
    AfxV3dCopy(v->pos, B);
    AfxV3dCopy(v->bary, AFX_V3D_Y);
    ++v;

    // v2 = C (degenerate = A is fine)
    AfxV3dCopy(v->pos, A);
    AfxV3dCopy(v->bary, AFX_V3D_ZERO);
    ++v;

    //*vptr = v;
}

// Y-up canonical sphere debug segments
static const float SphereSegments_YUp[24][6] =
{
    // XY circle (Z = 0)
    {  1,  0, 0,   0.7071f,  0.7071f, 0 },
    {  0.7071f,  0.7071f, 0,   0,  1, 0 },
    {  0,  1, 0,   -0.7071f,  0.7071f, 0 },
    { -0.7071f,  0.7071f, 0,   -1, 0, 0 },
    { -1,  0, 0,   -0.7071f, -0.7071f, 0 },
    { -0.7071f, -0.7071f, 0,   0, -1, 0 },
    {  0, -1, 0,   0.7071f, -0.7071f, 0 },
    {  0.7071f, -0.7071f, 0,   1, 0, 0 },

    // YZ circle (X = 0)
    { 0,  1, 0,    0,  0.7071f,  0.7071f },
    { 0,  0.7071f,  0.7071f,    0,  0,  1 },
    { 0,  0,  1,    0, -0.7071f,  0.7071f },
    { 0, -0.7071f,  0.7071f,    0, -1, 0 },
    { 0, -1,  0,    0, -0.7071f, -0.7071f },
    { 0, -0.7071f, -0.7071f,    0,  0, -1 },
    { 0,  0, -1,    0,  0.7071f, -0.7071f },
    { 0,  0.7071f, -0.7071f,    0,  1, 0 },

    // ZX circle (Y = 0)
    {  1, 0,  0,    0.7071f, 0,  0.7071f },
    {  0.7071f, 0,  0.7071f,    0, 0,  1 },
    {  0, 0,  1,   -0.7071f, 0,  0.7071f },
    { -0.7071f, 0,  0.7071f,   -1, 0,  0 },
    { -1, 0,  0,   -0.7071f, 0, -0.7071f },
    { -0.7071f, 0, -0.7071f,    0, 0, -1 },
    {  0, 0, -1,    0.7071f, 0, -0.7071f },
    {  0.7071f, 0, -0.7071f,    1, 0,  0 }
};

void SphereDebugDraw_barycentric(arxRenderContext rctx, afxM4d const m, afxReal radius)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);

    ArxPostConstants(rctx, 0, sizeof(push), &push);

    afxUnit vtxCnt = 24 * 3;
    arxWireVertex *out = ArxPostVertices(rctx, vtxCnt, sizeof(out[0]), NIL, 0);

    const float *v = (const float *)SphereSegments_YUp;

    for (int i = 0; i < 24; ++i)
    {
        // Local-space points before transform
        afxV3d A = {
            v[0] * radius,
            v[1] * radius,
            v[2] * radius
        };

        afxV3d B = {
            v[3] * radius,
            v[4] * radius,
            v[5] * radius
        };

        emit_wire_segment(&out[i * 3], A, B);

        v += 6;
    }

    AvxCmdDraw(dctx, vtxCnt, 1, 0, 0);
}

afxError ArxCmdSetWireframeThickness(arxRenderContext rctx, afxReal width)
{
    // There is a dedicated function to it to reduce draw commands issued when using hardware lines.

    afxError err = { 0 };
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;
    rctx->wireThickness = width;
    AvxCmdSetLineWidth(dctx, width);
    return err;
}

void SphereDebugDraw(arxRenderContext rctx, afxM4d const m, afxReal radius)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;
    
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);

    ArxPostConstants(rctx, 0, sizeof(push), &push);

    afxUnit vtxCnt = 24 * 2;
    afxV3d *out = ArxPostVertices(rctx, vtxCnt, sizeof(out[0]), NIL, 0);

    // Pointer to the table of unit circle/sphere line segments
    const float *v = (float *)SphereSegments_YUp;

    for (int i = 0; i < 24; ++i)
    {
        // Local-space points before transform
        afxV3d A = {
            v[0] * radius,
            v[1] * radius,
            v[2] * radius
        };

        afxV3d B = {
            v[3] * radius,
            v[4] * radius,
            v[5] * radius
        };

        AfxV3dCopy(out[i * 2 + 0], A);
        AfxV3dCopy(out[i * 2 + 1], B);

        // Move to next segment (6 floats per segment)
        v += 6;
    }

    AvxCmdDraw(dctx, vtxCnt, 1, 0, 0);

}

void drawWireSphereLineStrip(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs)
{
    /*
        A meaningful sphere wireframe requires both lats and longs to be positive.
        Trying to draw lines with either dimension missing makes no geometric sense.
    */

    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);

    ArxPostConstants(rctx, 0, sizeof(push), &push);

    if (!lats || !longs)
        lats = (longs = 16);

    // Vertex counts for each component
    afxUnit vtxCntLong = (longs > 0 && lats > 0) ? (lats * longs * 2) : 0;
    afxUnit vtxCntLat = (lats > 0 && longs > 0) ? (lats * longs * 2) : 0;

    afxUnit totalVerts = vtxCntLong + vtxCntLat;
    if (totalVerts == 0)
        return;

    // One single allocation
    afxV3d* verts = ArxPostVertices(rctx, totalVerts, sizeof(afxV3d), NIL, 0);
    if (!verts) return;

    afxV3d* out = verts;
    afxUnit written = 0;

    //
    // ------------------------------------------------------------
    //   1. Longitude lines (vertical meridians)
    // ------------------------------------------------------------
    //
    if (lats > 0 && longs > 0)
    {
        for (int i = 0; i < lats; i++)
        {
            float lat0 = AFX_PI * (-0.5f + (float)i / lats);
            float lat1 = AFX_PI * (-0.5f + (float)(i + 1) / lats);

            float z0 = AfxSin(lat0), zr0 = AfxCos(lat0);
            float z1 = AfxSin(lat1), zr1 = AfxCos(lat1);

            for (int j = 0; j < longs; j++)
            {
                float lng = 2.f * AFX_PI * (float)j / longs;
                float x = AfxCos(lng), y = AfxSin(lng);

                afxV3d* v = &out[written];

                // top
                v[0][0] = radius * x * zr0;
                v[0][1] = radius * y * zr0;
                v[0][2] = radius * z0;

                // bottom
                v[1][0] = radius * x * zr1;
                v[1][1] = radius * y * zr1;
                v[1][2] = radius * z1;

                written += 2;
            }
        }
        AvxCmdDraw(dctx, vtxCntLong, 1, 0, 0);
    }

    //
    // ------------------------------------------------------------
    //   2. Latitude lines (horizontal rings)
    // ------------------------------------------------------------
    //
    if (lats > 0 && longs > 0)
    {
        for (int i = 0; i < lats; i++)
        {
            float lat = AFX_PI * (-0.5f + (float)i / lats);
            float z = AfxSin(lat), zr = AfxCos(lat);

            for (int j = 0; j < longs; j++)
            {
                float lng0 = 2.f * AFX_PI * (float)j / longs;
                float lng1 = 2.f * AFX_PI * (float)(j + 1) / longs;

                afxV3d* v = &out[written];

                // start
                v[0][0] = radius * AfxCos(lng0) * zr;
                v[0][1] = radius * AfxSin(lng0) * zr;
                v[0][2] = radius * z;

                // end
                v[1][0] = radius * AfxCos(lng1) * zr;
                v[1][1] = radius * AfxSin(lng1) * zr;
                v[1][2] = radius * z;

                written += 2;
            }
        }

        AvxCmdDraw(dctx, vtxCntLat, 1, 0, vtxCntLong);
    }
}

_ARX afxError ArxSetWireframeConstants(arxRenderContext rctx, afxReal wireThickness, avxColor wireCol, avxColor fillCol)
{
    rctx->wireThickness = wireThickness ? wireThickness : 0.02;
    AvxCopyColor(rctx->wireCol, wireCol ? wireCol : AFX_V3D_ONE);
    AvxCopyColor(rctx->fillCol, fillCol ? fillCol : AFX_V3D(0.1, 0.1, 0.1));
}

_ARX afxError ArxPushTransform(arxRenderContext rctx, afxM4d const m)
{
    afxError err = { 0 };
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);

    ArxPostConstants(rctx, 0, sizeof(push), &push);
    return err;
}

void drawWireSphere1(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs)
{
    /*
        New formulae.
        Only one vertex per grid point. No duplicates except seam column (required for wrapping).
        Index buffer is compact. Only 2 × (segments) indices.
        No degenerate seam segments. We emit exactly the segments needed.
        Easy LOD. Just decrease lats/longs.
        Fast. Much fewer vertices than your original push-every-segment method.
    */

    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    // --------------------------
    // Constants Push
    // --------------------------
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);

    ArxPostConstants(rctx, 0, sizeof(push), &push);

    if (!lats && !longs)
        lats = 16, longs = 16;

    // --------------------------
    // Vertex Layout
    // --------------------------
    //
    // We create a clean grid:
    //
    //   lat = 0..lats      (so lats+1 rows, includes poles)
    //   lon = 0..longs     (so longs+1 vertices per row, seam duplicated once)
    //
    // total vertices = (lats+1) * (longs+1)
    //
    afxUnit rowStride = longs + 1;
    afxUnit vtxCnt = (lats + 1) * rowStride;

    afxV3d* verts = ArxPostVertices(rctx, vtxCnt, sizeof(afxV3d), NIL, 0);

    // --------------------------
    // Build Vertex Positions
    // --------------------------
    afxUnit idx = 0;
    for (afxUnit lat = 0; lat <= lats; lat++)
    {
        afxReal theta = (afxReal)lat / (afxReal)lats * AFX_PI - AFX_PI / 2.0f;
        afxReal y = radius * AfxSin(theta);
        afxReal r = radius * AfxCos(theta);

        for (afxUnit lon = 0; lon <= longs; lon++)
        {
            afxReal phi = (afxReal)lon / (afxReal)longs * 2.0f * AFX_PI;
            AfxV3dCopy(verts[idx++], AFX_V3D(r * AfxCos(phi), y, r * sin(phi)));
        }
    }

    // --------------------------
    // Index Buffer Layout
    // --------------------------
    //
    // 1. Latitude rings (horizontal):
    //
    //    For each lat row (except poles), connect:
    //       (lat, lon) -> (lat, lon+1)
    //
    // 2. Longitude rings (vertical):
    //
    //    For each lon column, connect:
    //       (lat, lon) -> (lat+1, lon)
    //
    // Total segments:
    //
    //   lat-segments: (lats - 1) * longs
    //   lon-segments: (longs) * lats
    //
    // Each segment = 2 indices.
    //
    afxUnit latSegCnt = (lats - 1) * longs;
    afxUnit lonSegCnt = longs * lats;

    afxUnit idxCnt = (latSegCnt + lonSegCnt) * 2;

    afxIndex* indices = ArxPostVertexIndices(rctx, idxCnt, sizeof(afxIndex), NIL, 0);

    // --------------------------
    // Fill Index Buffer
    // --------------------------
    afxUnit ii = 0;

    // ---- LATITUDE RINGS ----
    for (afxUnit lat = 1; lat < lats; lat++)
    {            // skip poles
        afxUnit base = lat * rowStride;
        for (afxUnit lon = 0; lon < longs; lon++)
        {       // wrap handled by rowStride
            afxIndex i0 = base + lon;
            afxIndex i1 = base + lon + 1;
            indices[ii++] = i0;
            indices[ii++] = i1;
        }
    }

    // ---- LONGITUDE RINGS ----
    for (afxUnit lon = 0; lon < longs; lon++)
    {
        for (afxUnit lat = 0; lat < lats; lat++)
        {
            afxIndex i0 = lat * rowStride + lon;
            afxIndex i1 = (lat + 1) * rowStride + lon;
            indices[ii++] = i0;
            indices[ii++] = i1;
        }
    }

    // --------------------------
    // Issue Draw Call
    // --------------------------
    AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
}

void ArxDrawLine(arxRenderContext rctx, afxV3d origin, afxV3d target)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    afxV3d* verts = ArxPostVertices(rctx, 2, sizeof(afxV3d), NIL, 0);
    AfxV3dCopy(verts[0], origin);
    AfxV3dCopy(verts[1], target);

    AvxCmdDraw(dctx, 2, 1, 0, 0);
}

void drawWireSphere(arxRenderContext rctx, afxM4d const m, afxReal radius, afxUnit lats, afxUnit longs)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    // ---- Constants ----
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);
    ArxPostConstants(rctx, 0, sizeof(push), &push);

    // ---- Default behavior ----
    if (lats == 0 && longs == 0)
        lats = longs = 16;

    //
    // ================================================================
    // MODE 1: FULL GRID (lats>0 && longs>0)
    // ================================================================
    //
    if (lats > 0 && longs > 0)
    {
        afxUnit rowStride = longs + 1;
        afxUnit vtxCnt = (lats + 1) * rowStride;
        afxV3d* verts = ArxPostVertices(rctx, vtxCnt, sizeof(afxV3d), NIL, 0);

        // ----------------- Build vertices -----------------
        afxUnit k = 0;
        for (afxUnit i = 0; i <= lats; i++)
        {
            afxReal t = (afxReal)i / (afxReal)lats;
            afxReal theta = t * AFX_PI - AFX_PI * 0.5f;
            afxReal y = radius * AfxSin(theta);
            afxReal r = radius * AfxCos(theta);

            for (afxUnit j = 0; j <= longs; j++)
            {
                afxReal u = (afxReal)j / (afxReal)longs;
                afxReal phi = u * 2.0f * AFX_PI;

                verts[k][0] = r * AfxCos(phi);
                verts[k][1] = y;
                verts[k][2] = r * AfxSin(phi);
                k++;
            }
        }

        // ----------------- Build indices -----------------
        afxUnit latSegCnt = (lats - 1) * longs;
        afxUnit lonSegCnt = longs * lats;
        afxUnit idxCnt = (latSegCnt + lonSegCnt) * 2;

        afxIndex* indices = ArxPostVertexIndices(rctx, idxCnt, sizeof(afxIndex), NIL, 0);

        afxUnit ii = 0;

        // latitude rings, skipping poles
        for (afxUnit i = 1; i < lats; i++)
        {
            afxUnit base = i * rowStride;
            for (afxUnit j = 0; j < longs; j++)
            {
                indices[ii++] = base + j;
                indices[ii++] = base + j + 1;
            }
        }

        // longitude lines
        for (afxUnit j = 0; j < longs; j++)
        {
            for (afxUnit i = 0; i < lats; i++)
            {
                indices[ii++] = i * rowStride + j;
                indices[ii++] = (i + 1) * rowStride + j;
            }
        }

        AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
        return;
    }

    //
    // ================================================================
    // MODE 2: LATITUDE-ONLY (Z MODE)  (lats > 0 && longs == 0)
    // ================================================================
    //
    if (lats > 0 && longs == 0)
    {
        // sample count for each circle (compact, symmetric)
        afxUnit samples = lats + 1;
        afxUnit vtxCnt = lats * samples;

        afxV3d* verts = ArxPostVertices(rctx, vtxCnt, sizeof(afxV3d), NIL, 0);

        // ---------- build circles ----------
        afxUnit k = 0;
        for (afxUnit i = 1; i <= lats; i++)
        {
            afxReal t = (afxReal)i / (afxReal)lats;
            afxReal theta = t * AFX_PI - AFX_PI * 0.5f;
            afxReal y = radius * AfxSin(theta);
            afxReal r = radius * AfxCos(theta);

            for (afxUnit s = 0; s < samples; s++)
            {
                afxReal u = (afxReal)s / (afxReal)samples;
                afxReal phi = u * 2.f * AFX_PI;

                verts[k][0] = r * AfxCos(phi);
                verts[k][1] = y;
                verts[k][2] = r * AfxSin(phi);
                k++;
            }
        }

        // ---------- indices (wrap via index) ----------
        afxUnit idxCnt = lats * samples * 2;
        afxIndex* indices = ArxPostVertexIndices(rctx, idxCnt, sizeof(afxIndex), NIL, 0);

        afxUnit ii = 0;
        for (afxUnit i = 0; i < lats; i++)
        {
            afxUnit base = i * samples;
            for (afxUnit s = 0; s < samples; s++)
            {
                afxIndex i0 = base + s;
                afxIndex i1 = base + ((s + 1) % samples); // wrap
                indices[ii++] = i0;
                indices[ii++] = i1;
            }
        }

        AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
        return;
    }

    // ================================================================
    // MODE 3: LONGITUDE-ONLY (C MODE)  (lats == 0 && longs > 0)
    // ================================================================
    if (lats == 0 && longs > 0)
    {
        afxUnit samples = longs + 1; // samples along meridian
        afxUnit vtxCnt = longs * samples;
        afxV3d* verts = ArxPostVertices(rctx, vtxCnt, sizeof(afxV3d), NIL, 0);

        // --------- build vertices ----------
        afxUnit k = 0;
        for (afxUnit j = 0; j < longs; j++)
        {
            afxReal phi = 2.f * AFX_PI * (afxReal)j / (afxReal)longs;
            afxReal c = AfxCos(phi);
            afxReal s = AfxSin(phi);

            for (afxUnit p = 0; p < samples; p++)
            {
                afxReal t = (afxReal)p / (afxReal)(samples - 1); // 0..1
                afxReal theta = t * AFX_PI - AFX_PI * 0.5f;

                afxReal y = radius * AfxSin(theta);
                afxReal z = radius * AfxCos(theta);
                afxReal x = 0.0f;

                // rotate around Y
                verts[k][0] = x * c + z * s;
                verts[k][1] = y;
                verts[k][2] = z * c - x * s;
                k++;
            }
        }

        // --------- build indices ----------
        afxUnit idxCnt = longs * (samples - 1) * 2      // vertical meridian segments
            + longs * 2                     // top pole horizontal connections
            + longs * 2;                    // bottom pole horizontal connections

        afxIndex* indices = ArxPostVertexIndices(rctx, idxCnt, sizeof(afxIndex), NIL, 0);
        afxUnit ii = 0;

        // vertical segments along each meridian
        for (afxUnit j = 0; j < longs; j++)
        {
            afxUnit base = j * samples;
            for (afxUnit p = 0; p < samples - 1; p++)
            {
                indices[ii++] = base + p;
                indices[ii++] = base + p + 1;
            }
        }

        // horizontal connections at top pole
        for (afxUnit j = 0; j < longs; j++)
        {
            afxUnit top0 = j * samples + 0;
            afxUnit top1 = ((j + 1) % longs) * samples + 0;
            indices[ii++] = top0;
            indices[ii++] = top1;
        }

        // horizontal connections at bottom pole
        for (afxUnit j = 0; j < longs; j++)
        {
            afxUnit bot0 = j * samples + (samples - 1);
            afxUnit bot1 = ((j + 1) % longs) * samples + (samples - 1);
            indices[ii++] = bot0;
            indices[ii++] = bot1;
        }

        AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
        return;
    }
}

void drawWireBox(arxRenderContext rctx, afxM4d const m, afxV3d halfExtents)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    // ---- Constants ----
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);
    ArxPostConstants(rctx, 0, sizeof(push), &push);

    const float hx = halfExtents[0];
    const float hy = halfExtents[1];
    const float hz = halfExtents[2];

    // 8 corners
    afxV3d const verts[] =
    {   {-hx, -hy, -hz },
        { hx, -hy, -hz },
        { hx,  hy, -hz },
        {-hx,  hy, -hz },

        {-hx, -hy,  hz },
        { hx, -hy,  hz },
        { hx,  hy,  hz },
        {-hx,  hy,  hz } };

    // 12 edges = 24 indices
    afxUnit32 const indices[] =
    {
        0,1, 1,2, 2,3, 3,0,   // bottom rectangle
        4,5, 5,6, 6,7, 7,4,   // top rectangle
        0,4, 1,5, 2,6, 3,7    // verticals
    };

    afxUnit vtxCnt = 8;
    ArxPostVertices(rctx, vtxCnt, sizeof(verts[0]), verts, sizeof(verts[0]));

    afxUnit idxCnt = 24;
    ArxPostVertexIndices(rctx, idxCnt, sizeof(indices[0]), indices, sizeof(indices[0]));

    AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
}

void drawWireBox2(arxRenderContext rctx, afxV3d const center, afxQuat const rotation, afxV3d const halfExtents)
{
    afxDrawContext dctx = rctx->frames[rctx->frameIdx].drawDctx;

    // ---- Constants ----
    arxWireframePush push;
    push.uWireThickness = rctx->wireThickness;
    AfxV3dCopy(push.uWireColor, rctx->wireCol);
    AfxV3dCopy(push.uFillColor, rctx->fillCol);
    
    //AfxM4dCopyAtm(push.uM, m ? m : AFX_M4D_IDENTITY);
    // Build model matrix
    AfxM4dComposeAffineTransformation(push.uM, halfExtents, AFX_V3D_ONE, rotation, center);
    // We scale by halfExtents and not halfExtents * 2, because the unit cube in this version 
    // spans from -1 to + 1, that is already size = 2.
    // Scaling by halfExtents yields: unitCube(-1..1) * halfExt => (-halfExt ... +halfExt)
    // Perfect for physics engines.

    ArxPostConstants(rctx, 0, sizeof(push), &push);

    // 8 corners
    // Unit cube vertices (centered at origin)
    const afxV3d verts[] =
    {   {-1, -1, -1 },
        { 1, -1, -1 },
        { 1,  1, -1 },
        {-1,  1, -1 },
        {-1, -1,  1 },
        { 1, -1,  1 },
        { 1,  1,  1 },
        {-1,  1,  1 } };

    // 12 edges = 24 indices
    const afxUnit32 indices[] =
    {
        0,1, 1,2, 2,3, 3,0,   // bottom
        4,5, 5,6, 6,7, 7,4,   // top
        0,4, 1,5, 2,6, 3,7    // verticals
    };

    afxUnit vtxCnt = 8;
    ArxPostVertices(rctx, vtxCnt, sizeof(verts[0]), verts, sizeof(verts[0]));

    afxUnit idxCnt = 24;
    ArxPostVertexIndices(rctx, idxCnt, sizeof(indices[0]), indices, sizeof(indices[0]));

    AvxCmdDrawIndexed(dctx, idxCnt, 1, 0, 0, 0);
}

