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


#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

//#define _BIND_FBO_FOR_READ_CONCURRENTLY_ON_DRAW_SCOPE TRUE
#define _FLUSH_ON_DRAW_SCOPE_CONCLUSION TRUE
#define _FLUSH_ON_DRAW_SCOPE_SUSPENSION TRUE
#define _CLEAR_CANVAS_WITH_GEN_FBOS TRUE

_ZGL afxError _ZglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLuint fbo, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint z, afxBool multilayered)
{
    afxError err = { 0 };

    if (fbo)
    {
        switch (texTarget)
        {
        case GL_RENDERBUFFER:
        {
            gl->NamedFramebufferRenderbuffer(fbo, glAttachment, GL_RENDERBUFFER, texHandle); _ZglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_2D:
        case GL_TEXTURE_1D:
        case GL_TEXTURE_CUBE_MAP:
            // Multisampled textures are valid FBO attachments and must be handled via glNamedFramebufferTexture (entire texture, not layers)
        case GL_TEXTURE_2D_MULTISAMPLE:
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        {
            GLenum texTarget2;
            if (texTarget != GL_TEXTURE_CUBE_MAP) texTarget2 = texTarget;
            else
            {
                // Where cubeMapFace is one of: GL_TEXTURE_CUBE_MAP_POSITIVE_X, ..., GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
                // Why? Cube maps are not layered textures in the OpenGL sense; you attach one face at a time using FramebufferTexture2D.
                texTarget2 = GL_TEXTURE_CUBE_MAP_POSITIVE_X + z;
            }
            gl->NamedFramebufferTexture(fbo, glAttachment, texHandle, level); _ZglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_2D_ARRAY:
        case GL_TEXTURE_1D_ARRAY:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        case GL_TEXTURE_3D:
        {
            if (multilayered)
            {
                gl->NamedFramebufferTextureLayer(fbo, glAttachment, texHandle, level, z); _ZglThrowErrorOccuried();
            }
            else
            {
                gl->NamedFramebufferTexture(fbo, glAttachment, texHandle, level); _ZglThrowErrorOccuried();
            }
            break;
        }
        default: _ZglThrowErrorOccuried(); break;
        };

        // RETURN IF DSA
        return err;
    }

    switch (texTarget)
    {
    case GL_RENDERBUFFER:
    {
        // This is the right call for attaching a renderbuffer.
        gl->FramebufferRenderbuffer(glTarget, glAttachment, GL_RENDERBUFFER, texHandle); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D:
    case GL_TEXTURE_2D_MULTISAMPLE:
    case GL_TEXTURE_CUBE_MAP:
    {
        GLenum texTarget2;
        if (texTarget != GL_TEXTURE_CUBE_MAP) texTarget2 = texTarget;
        else
        {
            // Where cubeMapFace is one of: GL_TEXTURE_CUBE_MAP_POSITIVE_X, ..., GL_TEXTURE_CUBE_MAP_NEGATIVE_Z.
            // Why? Cube maps are not layered textures in the OpenGL sense; you attach one face at a time using FramebufferTexture2D.
            texTarget2 = GL_TEXTURE_CUBE_MAP_POSITIVE_X + z;
        }
        // These targets require glFramebufferTexture2D, where texTarget is either GL_TEXTURE_2D or GL_TEXTURE_2D_MULTISAMPLE.
        gl->FramebufferTexture2D(glTarget, glAttachment, texTarget2, texHandle, level); _ZglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D_ARRAY:
    case GL_TEXTURE_2D_ARRAY:
    case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
    case GL_TEXTURE_CUBE_MAP_ARRAY:
    {
        // glFramebufferTexture() attaches all layers, not a specific one.
        // If you're intending to attach a specific layer, you should use glFramebufferTextureLayer() instead.
        // Otherwise, this is okay if you're intentionally attaching the entire layered texture (e.g. for layered rendering with gl_Layer in geometry shaders).
        if (multilayered)
        {
            // all layers
            gl->FramebufferTexture(glTarget, glAttachment, texHandle, level); _ZglThrowErrorOccuried();
        }
        else
        {
            // one layer
            gl->FramebufferTextureLayer(glTarget, glAttachment, texHandle, level, z); _ZglThrowErrorOccuried();
        }
        break;
    }
    case GL_TEXTURE_3D:
    {
        // glFramebufferTexture3D is part of EXT_framebuffer_object, not core OpenGL.
        // In core OpenGL, you should use: glFramebufferTextureLayer(glTarget, glAttachment, texHandle, level, z);
#if 0
        gl->FramebufferTexture3D(glTarget, glAttachment, texTarget, texHandle, level, z); _ZglThrowErrorOccuried();
#else
        gl->FramebufferTextureLayer(glTarget, glAttachment, texHandle, level, z); _ZglThrowErrorOccuried();
#endif
        break;
    }
    case GL_TEXTURE_1D:
    {
        // This is valid in OpenGL 3.0+ (or 2.0 with GL_EXT_framebuffer_object).
        // But note: glFramebufferTexture1D is rarely used today, and not supported in Core Profile from OpenGL 4.5 onward.
        gl->FramebufferTexture1D(glTarget, glAttachment, texTarget, texHandle, level); _ZglThrowErrorOccuried();
        break;
    }
    default:
        if (multilayered)
        {
            // all layers
            gl->FramebufferTexture(glTarget, glAttachment, texHandle, level); _ZglThrowErrorOccuried();
        }
        else
        {
            // one layer
            gl->FramebufferTextureLayer(glTarget, glAttachment, texHandle, level, z); _ZglThrowErrorOccuried();
        }
        break;
    };
    return err;
}

_ZGL afxError _DpuBindAndSyncCanv(zglDpu* dpu, GLenum glTarget, avxCanvas canv, afxBool keepBound)
{
    //AfxEntry("canv=%p", canv);
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    afxUnit exuIdx = dpu->m.exuIdx;
    afxBool bound = (canv == dpu->canv);

    if (!canv)
    {
        //if (dpu->canv || dpu->canvGpuHandle)
        if (keepBound)
        {
            gl->BindFramebuffer(glTarget, 0); _ZglThrowErrorOccuried();
            dpu->canvGpuHandle = NIL;
            dpu->canv = NIL;
        }
        return err;
    }

    afxUnit fboHandleIdx = dpu->dpuIterIdx % _ZGL_FBO_SWAPS;

    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
    GLuint glHandle = canv->perDpu[dpu->m.exuIdx][fboHandleIdx].glHandle;
    zglUpdateFlags devUpdReq = (canv->updFlags & ZGL_UPD_FLAG_DEVICE);

    if (glHandle && !(devUpdReq & ZGL_UPD_FLAG_DEVICE_INST))
    {
        //if ((canv != dpu->canv) || (glHandle != dpu->canvGpuHandle))
        {
            AFX_ASSERT(gl->IsFramebuffer(glHandle));
            gl->BindFramebuffer(glTarget, glHandle); _ZglThrowErrorOccuried();
            dpu->canvGpuHandle = glHandle;
            dpu->canv = canv;
            //dpu->canv = NIL;
        }
    }
    else
    {
        if (glHandle)
        {
            AFX_ASSERT(gl->IsFramebuffer(glHandle));
            gl->DeleteFramebuffers(1, &(glHandle)); _ZglThrowErrorOccuried();
            canv->perDpu[dpu->m.exuIdx][fboHandleIdx].glHandle = NIL;
            glHandle = NIL;
        }
#if 0
        if ((!keepBound) && gl->CreateFramebuffers)
        {
            gl->CreateFramebuffers(1, &glHandle); _ZglThrowErrorOccuried();

            if (canv->m.tag.len)
            {
                gl->ObjectLabel(GL_FRAMEBUFFER, glHandle, canv->m.tag.len, (GLchar const*)canv->m.tag.start); _ZglThrowErrorOccuried();
            }
        }
        else
#endif
        {
            gl->GenFramebuffers(1, &(glHandle)); _ZglThrowErrorOccuried();
            gl->BindFramebuffer(glTarget, glHandle); _ZglThrowErrorOccuried();
            AFX_ASSERT(gl->IsFramebuffer(glHandle));
            bound = TRUE;

            if (canv->m.tag.len)
            {
                gl->ObjectLabel(GL_FRAMEBUFFER, glHandle, canv->m.tag.len, (GLchar const*)canv->m.tag.start); _ZglThrowErrorOccuried();
            }
        }

        canv->perDpu[dpu->m.exuIdx][fboHandleIdx].glHandle = glHandle;
        dpu->canv = canv;

        if (canv->m.flags & avxCanvasFlag_VOID)
        {
            /*
                GL_ARB_framebuffer_no_attachments
                (Or its equivalent in OpenGL ES: EXT_framebuffer_no_attachments)
                Introduced in OpenGL 4.3, this extension allows framebuffers to be complete with no attachments; provided certain parameters are explicitly specified.

                How to Use GL_ARB_framebuffer_no_attachments.
                Instead of attaching textures or renderbuffers, we define the framebuffer's default width, 
                height, layers, samples, and format. Then, the framebuffer can be complete with no attachments.
            */

            // Set framebuffer defaults (required when no attachments are used
            gl->FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, canv->m.whd.w);
            gl->FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, canv->m.whd.h);
            gl->FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_LAYERS, canv->m.whd.d);
            gl->FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, (2 ^ canv->m.lodCnt));
            // From zglRaster.c
            // If you don't use the VK_EXT_sample_locations extension, 
            // Vulkan uses implementation-defined fixed sample locations (equivalent to fixedsamplelocations = GL_TRUE in OpenGL).
            GLboolean fixedsamplelocations = GL_TRUE;
            gl->FramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, fixedsamplelocations);

            // No attachments at all
            gl->DrawBuffer(GL_NONE);
            gl->ReadBuffer(GL_NONE);
        }
        else
        {
            afxUnit dsSurIdx[2] = { AFX_INVALID_INDEX, AFX_INVALID_INDEX };
            afxUnit surCnt = AvxQueryCanvasBins(canv, &surCnt, &dsSurIdx[0], &dsSurIdx[1]);
            afxBool combinedDs = (dsSurIdx[1] == dsSurIdx[0]) && (dsSurIdx[0] != AFX_INVALID_INDEX);
            avxRaster surfaces[_ZGL_MAX_SURF_PER_CANV];
            AFX_ASSERT(_ZGL_MAX_SURF_PER_CANV >= surCnt);
            AvxGetDrawBuffers(canv, 0, surCnt, surfaces);

            afxUnit colorIdx = 0;
            GLenum glAttachment;
            GLuint glTexHandle;
            GLenum glTexTarget;

            for (afxUnit i = 0; i < surCnt; i++)
            {
                if (dsSurIdx[0] == i) // depth
                {
                    if (!combinedDs)
                        glAttachment = GL_DEPTH_ATTACHMENT;
                    else
                        glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
                }
                else if (dsSurIdx[1] == i)
                {
                    if (!combinedDs)
                        glAttachment = GL_STENCIL_ATTACHMENT;
                    else
                        glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
                }
                else
                {
                    glAttachment = GL_COLOR_ATTACHMENT0 + colorIdx;
                    ++colorIdx;
                }

                avxRaster ras = surfaces[i];

                if (!ras)
                {
                    AfxThrowError();
                    glTexHandle = 0;
                    glTexTarget = GL_TEXTURE_2D;
                    _ZglBindFboAttachment(dpu->gl, glTarget, NIL, glAttachment, glTexTarget, glTexHandle, 0, 0, (canv->m.whdMin.d > 1));
                }
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                    AFX_ASSERT(AvxGetRasterUsage(ras, avxRasterUsage_DRAW));

                    DpuBindAndSyncRas(dpu, ZGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras, FALSE);
#if RENDERBUFFER_ENABLED
                    if (avxRasterUsage_DRAW == AvxGetRasterUsage(ras, avxRasterUsage_ALL))
                    {
                        glTexHandle = ras->glHandle;
                        glTexTarget = GL_RENDERBUFFER;
                        AFX_ASSERT(gl->IsRenderBuffer(glTexHandle));
                    }
                    else
#endif
                    {
                        glTexHandle = ras->glHandle;
                        glTexTarget = ras->glTarget;
                        AFX_ASSERT(gl->IsTexture(glTexHandle));
                    }
                    _ZglBindFboAttachment(dpu->gl, glTarget, NIL, glAttachment, glTexTarget, glTexHandle, ras->m.baseMip, ras->m.baseLayer, (canv->m.whdMin.d > 1));
                }
            }

            if (canv->m.colorCnt == 0)
            {
                /*
                    A framebuffer object however is not complete without a color buffer so we need to explicitly tell 
                    OpenGL we're not going to render any color data. We do this by setting both the read and draw buffer 
                    to GL_NONE with glDrawBuffer and glReadbuffer.
                */
                // No attachments at all
                gl->DrawBuffer(GL_NONE);
                gl->ReadBuffer(GL_NONE);
            }
        }

        switch (gl->CheckFramebufferStatus(glTarget))
        {
        case GL_FRAMEBUFFER_COMPLETE:
            canv->updFlags &= ~(ZGL_UPD_FLAG_DEVICE);
            //AfxReportMessage("avxCanvas %p hardware-side data instanced.", canv);
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            AfxReportError("Not all framebuffer attachment points are framebuffer attachment complete.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            AfxReportError("No images are attached to the framebuffer.");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            AfxReportError("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            AfxReportError("Incomplete draw frame buffer.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            AfxReportError("Incomplete read frame buffer.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            AfxReportError("Incomplete multisample.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            AfxReportError("Incomplete layer targets.");
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            AfxReportError("Default framebuffer does not exist");
            break;
        default:
            AfxReportError("UNKNOWN");
            _ZglThrowErrorOccuried();
            break;
        }

        if (keepBound)
        {
            AFX_ASSERT(bound);
            dpu->canvGpuHandle = glHandle;
            dpu->canv = canv;
        }
        else
        {
            gl->BindFramebuffer(glTarget, 0); _ZglThrowErrorOccuried();
            dpu->canvGpuHandle = NIL;
            dpu->canv = NIL;
        }
    }
    return err;
}

_ZGL void DpuCommenceDrawScope(zglDpu* dpu, avxDrawScopeFlags flags, avxCanvas canv, afxLayeredRect const* bounds, afxUnit cCnt, avxDrawTarget const* c, avxDrawTarget const* d, avxDrawTarget const* s, afxString const* dbgTag, afxBool defFbo)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.
    dpu->inDrawScope = TRUE;

    avxDrawTarget const* dt;

    if (dbgTag && dbgTag->len)
    {
        gl->PushDebugGroup(GL_DEBUG_SOURCE_THIRD_PARTY, 0, dbgTag->len, (const GLchar*)dbgTag->start); _ZglThrowErrorOccuried();
        dpu->mustCloseDrawScopeDgbGrp = TRUE;
    }

    /*
        The application should always call glClear() for every render target at the start of
        each render target's rendering sequence, provided that the previous contents of the
        attachments are not needed, of course. This explicitly tells the driver we do not
        need the previous state for this frame of rendering, and thus we avoid reading it
        back from memory, as well as putting any undefined buffer contents into a defined "clear color" state.

        One common mistake which is seen here is only clearing part of the framebuffer; i.e.
        calling glClear() while only a portion of the render target is active because of a
        scissor rectangle with only partial screen coverage. We can only completely drop the
        render state when it applies to whole surfaces, so a clear of the whole render target
        should be performed where possible.

        The final requirement placed on the application for efficient use of FBOs in the OpenGL ES
        API is that it should tell the driver which of the color / depth / stencil attachments are
        simply transient working buffers, the value of which can be discarded at the end of rendering
        the current render pass. For example, nearly every 3D render will use color and depth, but for
        most applications the depth buffer is transient and can be safely invalidated. Failure to
        invalidate the unneeded buffers may result in them being written back to memory, wasting memory
        bandwidth and increasing energy consumption of the rendering process.

        The most common mistake at this point is to treat glInvalidateFramebuffer() as equivalent to glClear()
        and place the invalidate call for frame N state at the first use of that FBO in frame N+1. This is too late!
        The purpose of the invalidate call is to tell the driver that that the buffers do not need to be kept,
        so we need to modify the work submission to the GPU for frame which produces those buffers. Telling us
        in the next frame is often after the original frame has been processed. The application needs to ensure
        that the driver knows which buffers are transient before the framebuffer is flushed. Therefore transient
        buffers in frame N should be indicated by calling glInvalidateFramebuffer()before unbinding the FBO in frame N.
        For example:

        #define ALL_BUFFERS COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT
        static const GLEnum invalid_ap[2] = { GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT };

        glClear( ALL_BUFFERS )    // Clear initial state
        glDraw...( ... )          // Draw something to FBO 0 (window surface)

        glBindFramebuffer( 1 )    // Switch away from FBO 0, does not trigger rendering
        glClear( ALL_BUFFERS )    // Clear initial state
        glDraw...( ... )          // Draw something to FBO 1
        ...                      // Draw FBO 1 to completion
        glInvalidateFramebuffer( GL_FRAMEBUFFER, 2, &invalid_ap[0] ); // Only keep color

        glBindFramebuffer(0)      // Switch to FBO 0, unbind and flush FBO 1 for rendering
        glDraw...( ... )          // Draw something else to FBO 0 (window surface)

        glBindFramebuffer( 2 )    // Switch away from FBO 0, does not trigger rendering
        glClear( ALL_BUFFERS )    // Clear initial state
        glDraw...( ... )          // Draw something to FBO 2
        ...                      // Draw FBO 2 to completion
        glInvalidateFramebuffer( GL_FRAMEBUFFER, 2, &invalid_ap[0] ); // Only keep color

        glBindFramebuffer(0)      // Switch to FBO 0, unbind and flush FBO 2 for rendering
        glDraw...( ... )          // Draw something else to FBO 0 (window surface)

        eglSwapBuffers()          // Tell EGL we have finished, flush FBO 0 for rendering
    */



    if (!canv)
    {
        if (defFbo)
        {
            //_DpuBindAndSyncCanv(gl, GL_READ_FRAMEBUFFER, canv);
            _DpuBindAndSyncCanv(dpu, GL_DRAW_FRAMEBUFFER, canv, TRUE);
            //dpu->activeRs.canv = canv;

            dt = &c[0];

            switch (dt->storeOp)
            {
            case avxStoreOp_STORE:
            {
                gl->DrawBuffer(GL_BACK); _ZglThrowErrorOccuried();
                break;
            }
            case avxStoreOp_DISCARD:
            case avxStoreOp_DONT_CARE:
            default:
            {
                gl->DrawBuffer(GL_NONE); _ZglThrowErrorOccuried();
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK }); _ZglThrowErrorOccuried();
                break;
            }
            }

            switch (dt->loadOp)
            {
            case avxLoadOp_LOAD: break;
            case avxLoadOp_CLEAR:
            {
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK }); _ZglThrowErrorOccuried();
                afxReal const *rgba = dt->clearVal.rgba;
                gl->ClearColor(rgba[0], rgba[1], rgba[2], rgba[3]); _ZglThrowErrorOccuried();
                gl->Clear(GL_COLOR_BUFFER_BIT); _ZglThrowErrorOccuried();
                break;
            }
            case avxLoadOp_DONT_CARE:
            default:
            {
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 1, (GLenum[]) { GL_BACK }); _ZglThrowErrorOccuried();
                break;
            }
            }
        }
        else
        {
            AfxThrowError();
        }
        // RETURN
        return;
    }

    afxLayeredRect areaMax;
    AvxGetCanvasExtent(canv, NIL, &areaMax);
    avxRange canvWhd = { areaMax.area.w, areaMax.area.h, areaMax.layerCnt };

    afxUnit maxColSurCnt;
    afxUnit dsSurIdx[2] = { AFX_INVALID_INDEX, AFX_INVALID_INDEX };
    afxUnit maxSurCnt = AvxQueryCanvasBins(canv, &maxColSurCnt, &dsSurIdx[0], &dsSurIdx[1]);
    afxBool hasDs = ((dsSurIdx[1] != AFX_INVALID_INDEX) || (dsSurIdx[0] != AFX_INVALID_INDEX));
    afxBool combinedDs = (hasDs && (dsSurIdx[1] == dsSurIdx[0]));
    cCnt = AFX_MIN(cCnt, maxColSurCnt);

    afxUnit storeCnt = 0;
    GLenum storeBufs[_ZGL_MAX_SURF_PER_CANV];
    afxMask storeBitmask = NIL;

    afxUnit clearCnt = 0;
    GLenum clearBufs[_ZGL_MAX_SURF_PER_CANV];
    afxUnit invalidCnt = 0;
    GLenum invalidBufs[_ZGL_MAX_SURF_PER_CANV];

    GLenum glAttach = GL_NONE;
    dpu->invalidDrawBufCnt = 0;

    for (afxUnit i = 0; i < cCnt; i++)
    {
        dt = &c[i];
        glAttach = GL_COLOR_ATTACHMENT0 + dt->bufIdx;

        if (!(i < maxColSurCnt))
            continue;

        switch (dt->storeOp)
        {
        case avxStoreOp_STORE:
        {
            storeBufs[storeCnt] = glAttach;
            storeCnt++;
            storeBitmask |= AFX_BITMASK(dt->bufIdx);
            break;
        }
        case avxStoreOp_DISCARD:
        case avxStoreOp_DONT_CARE:
        default:
        {
            storeBufs[storeCnt] = GL_NONE;
            storeCnt++;
            storeBitmask &= ~AFX_BITMASK(dt->bufIdx);
            //storeBitmask |= AFX_BITMASK(8 + i);

            invalidBufs[invalidCnt] = glAttach;
            dpu->invalidDrawBufs[invalidCnt] = glAttach;
            dpu->invalidDrawBufCnt = invalidCnt;
            ++invalidCnt;
            break;
        }
        }

        // Prepare cleanups and invalidations.

        if (dt->loadOp != avxLoadOp_LOAD)
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = glAttach;
            ++clearCnt;
        }
    }

    if ((dsSurIdx[0] != AFX_INVALID_INDEX)) // depth (and stencil)
    {
        dt = d;
        glAttach = (s && combinedDs) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;

        // Prepare cleanups and invalidations.

        if (dt && (dt->loadOp != avxLoadOp_LOAD))
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = glAttach;
            ++clearCnt;
        }

        if (dt && (dt->storeOp != avxStoreOp_STORE))
        {
            invalidBufs[invalidCnt] = glAttach;
            ++invalidCnt;
        }
    }

    if ((dsSurIdx[1] != AFX_INVALID_INDEX) && !combinedDs) // stencil
    {
        dt = s;
        glAttach = GL_STENCIL_ATTACHMENT;

        // Prepare cleanups and invalidations.

        if (dt && (dt->loadOp != avxLoadOp_LOAD))
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = glAttach;
            ++clearCnt;
        }

        if (dt && (dt->storeOp != avxStoreOp_STORE))
        {
            invalidBufs[invalidCnt] = glAttach;
            ++invalidCnt;
        }
    }

    // TODO iterate for each canvas surface against arguments

    afxUnit fboHandleIdx = dpu->dpuIterIdx % _ZGL_FBO_SWAPS;

#if _BIND_FBO_FOR_READ_CONCURRENTLY_ON_DRAW_SCOPE
    _DpuBindAndSyncCanv(dpu, GL_READ_FRAMEBUFFER, canv, TRUE);
#endif
    _DpuBindAndSyncCanv(dpu, GL_DRAW_FRAMEBUFFER, canv, TRUE);
    GLuint glHandle = canv->perDpu[dpu->m.exuIdx][fboHandleIdx].glHandle;
    AFX_ASSERT(gl->IsFramebuffer(glHandle));
    //dpu->activeRs.canv = canv;

    afxBool resuming = (flags & avxDrawScopeFlag_RESUME);

    afxBool clipped = ((bounds->area.x && (bounds->area.x > 0)) ||
        (bounds->area.y && (bounds->area.y > 0)) ||
        (bounds->area.w && (bounds->area.w < canvWhd.w)) ||
        (bounds->area.h && (bounds->area.h < canvWhd.h)));
    dpu->drawAreaClipped = clipped;
    dpu->drawArea = *bounds;

    dpu->drawScopeFlags = flags;

    if (!resuming)
    {
        if (invalidCnt)
        {
            if (clipped)
            {
                gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs, bounds->area.x, bounds->area.y, bounds->area.w, bounds->area.h); _ZglThrowErrorOccuried();
            }
            else
            {
                gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs); _ZglThrowErrorOccuried();
            }
        }
    }

#if !0
    if (canv->perDpu[dpu->m.exuIdx][fboHandleIdx].storeBitmask != storeBitmask)
#endif
    {
        canv->perDpu[dpu->m.exuIdx][fboHandleIdx].storeBitmask = storeBitmask; // cache it
        gl->DrawBuffers(storeCnt, storeBufs); _ZglThrowErrorOccuried();

        // What to do with depth/stencil storage set to discard?
        // Should we force a glDepthMask to zero right there?
        // Should we force a glStencilMask to zero right there?
    }

    // Effectively invalidate the buffers.

    // TODO: Sanitize area to canvas' bounds.
    GLint scissor[4];
    scissor[0] = bounds->area.x;
    scissor[1] = bounds->area.y;
    scissor[2] = bounds->area.w;
    scissor[3] = bounds->area.h;
    AFX_ASSERT(bounds->area.w);
    AFX_ASSERT(bounds->area.h);
    AFX_ASSERT(gl->ScissorArrayv);
    gl->ScissorArrayv(0, 1, scissor);

    // We must store the scissor rects to avoid DPU to further apply invalid rects.
    for (afxUnit iter = 0; iter < ZGL_MAX_VIEWPORTS; iter++)
    {
        dpu->nextScisRects[iter] = bounds->area;
        dpu->nextScisUpdMask |= AFX_BITMASK(iter);
    }

    // Effectively clear the buffers.
    if (!resuming)
    {
        for (afxUnit i = 0; i < clearCnt; i++)
        {
            switch (clearBufs[i])
            {
            case GL_DEPTH_ATTACHMENT:
            {
                dt = d;
                gl->ClearBufferfv(GL_DEPTH, 0, &dt->clearVal.depth); _ZglThrowErrorOccuried();
                break;
            }
            case GL_DEPTH_STENCIL_ATTACHMENT:
            {
                dt = d;
                gl->ClearBufferfi(GL_DEPTH_STENCIL, 0, dt->clearVal.depth, dt->clearVal.stencil); _ZglThrowErrorOccuried();
                break;
            }
            case GL_STENCIL_ATTACHMENT:
            {
                dt = s;
                GLint sCv = dt->clearVal.stencil;
                gl->ClearBufferiv(GL_STENCIL, 0, &sCv); _ZglThrowErrorOccuried();
                break;
            }
            default:
            {
                dt = &c[i];
                afxReal const* rgba = dt->clearVal.rgba;
                GLint dbi = clearBufs[i] - GL_COLOR_ATTACHMENT0;
                gl->ClearBufferfv(GL_COLOR, /*GL_DRAW_BUFFER0 +*/ dbi, rgba); _ZglThrowErrorOccuried();
                break;
            }
            }
        }
    }
}

_ZGL void DpuConcludeDrawScope(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    AFX_ASSERT(dpu->inDrawScope != FALSE); // This is a transfer operation.
    dpu->inDrawScope = FALSE;

    afxUnit invalidCnt = dpu->invalidDrawBufCnt;
    afxBool clipped = dpu->drawAreaClipped;

#if 0
    if (invalidCnt)
    {
        if (clipped)
        {
            gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs, dpu->drawArea.x, dpu->drawArea.y, dpu->drawArea.w, dpu->drawArea.h); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs); _ZglThrowErrorOccuried();
        }
    }
#endif

    // https://graphics1600.rssing.com/chan-25333833/all_p4.html
    // Switch to zero to immediately trigger any pending buffer operation.

    afxBool toSuspend = (dpu->drawScopeFlags & avxDrawScopeFlag_SUSPEND);

    if (!toSuspend)
    {
#if _BIND_FBO_FOR_READY_CONCURRENTLY_ON_DRAW_SCOPE
        _DpuBindAndSyncCanv(dpu, GL_READ_FRAMEBUFFER, 0, TRUE);
#endif
        _DpuBindAndSyncCanv(dpu, GL_DRAW_FRAMEBUFFER, 0, TRUE);

        if (dpu->mustCloseDrawScopeDgbGrp)
        {
            gl->PopDebugGroup(); _ZglThrowErrorOccuried();
        }

#if _FLUSH_ON_DRAW_SCOPE_CONCLUSION
        gl->Flush(); _ZglThrowErrorOccuried(); // flush was presenting/swapping without wglSwapBuffers.
#endif
    }
    else
    {
        /*
            When suspending a scope, 
            we don't unbind the FBO to allow it to prepare discards and resolves at once right into the next FBO.
        */
#if _FLUSH_ON_DRAW_SCOPE_SUSPENSION
        gl->Flush(); _ZglThrowErrorOccuried(); // flush was presenting/swapping without wglSwapBuffers.
#endif
    }

    avxCanvas canv = dpu->canv;

    if (canv)
    {
        afxUnit surCnt;
        surCnt = AvxQueryCanvasBins(canv, NIL, NIL, NIL);
#if 0
        if (surCnt)
        {
            AFX_ASSERT(_ZGL_MAX_SURF_PER_CANV >= surCnt);
            avxRaster surfaces[_ZGL_MAX_SURF_PER_CANV];
            AvxGetDrawBuffers(canv, 0, surCnt, surfaces);

            for (afxUnit i = 0; i < surCnt; i++)
            {
                avxRaster ras = surfaces[i];

                if (ras)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                    ras->updFlags |= ZGL_UPD_FLAG_HOST_FLUSH;
                }
            }
        }
#endif
    }
}

_ZGL void DpuNextPass(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    AFX_ASSERT(dpu->inDrawScope != FALSE); // This is a transfer operation.
    gl->Flush(); _ZglThrowErrorOccuried();
}

_ZGL afxError _ZglDpuClearCanvas(zglDpu* dpu, afxUnit bufCnt, afxUnit const bins[], avxClearValue const values[], afxUnit areaCnt, afxLayeredRect const areas[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    AFX_ASSERT(dpu->inDrawScope);
    // This is not a transfer command, as it required a ongoing draw scope to having a target canvas.

    // void vkCmdClearAttachments( VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);

    GLuint fboOpDst;
    GLuint tempFbo;
    afxBool useTempFbo = _CLEAR_CANVAS_WITH_GEN_FBOS;
    if (useTempFbo)
    {
        gl->GenFramebuffers(1, &tempFbo); _ZglThrowErrorOccuried();
        fboOpDst = tempFbo;
    }
    else
    {
        fboOpDst = dpu->fboOpDst;
    }

    AfxThrowError();
    // What about the bound FBO via draw scope?
    gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, fboOpDst);

#if !0
    // TODO: It will be need to generate a new FBO, bind attachments to it, to be able to choose LOD and layer, then clear.
    for (afxUnit i = 0; i < bufCnt; i++)
    {
        avxRaster ras;
        if(!AvxGetDrawBuffers(dpu->canv, bins[i], 1, &ras))
            continue;

        for (afxUnit j = 0; j < areaCnt; j++)
        {
            afxLayeredRect const* area = &areas[j];
            GLint scissor[4];
            scissor[0] = area->area.x;
            scissor[1] = area->area.y;
            scissor[2] = area->area.w;
            scissor[3] = area->area.h;
            AFX_ASSERT(area->area.w);
            AFX_ASSERT(area->area.h);
            AFX_ASSERT(gl->ScissorArrayv);
            gl->ScissorArrayv(0, 1, scissor);

            for (afxUnit k = 0; k < area->layerCnt; k++)
            {
                afxUnit layerIdx = area->baseLayer + k;

                if (gl->ClearNamedFramebufferfv)
                {
                    _ZglBindFboAttachment(gl, GL_DRAW_FRAMEBUFFER, fboOpDst, ras->glAttachment, ras->glTarget, ras->glHandle, 0, layerIdx, FALSE);

                    switch (ras->glAttachment)
                    {
                    case GL_DEPTH_ATTACHMENT:
                    {
                        gl->ClearNamedFramebufferfv(fboOpDst, GL_DEPTH, 0, &values[i].depth); _ZglThrowErrorOccuried();
                        break;
                    }
                    case GL_DEPTH_STENCIL_ATTACHMENT:
                    {
                        gl->ClearNamedFramebufferfi(fboOpDst, GL_DEPTH_STENCIL, 0, values[i].depth, values[i].stencil); _ZglThrowErrorOccuried();
                        break;
                    }
                    case GL_STENCIL_ATTACHMENT:
                    {
                        GLint sCv = values[i].stencil;
                        gl->ClearNamedFramebufferiv(fboOpDst, GL_STENCIL, 0, &sCv); _ZglThrowErrorOccuried();
                        break;
                    }
                    default:
                    {
                        afxReal const* rgba = values[i].rgba;
                        GLint dbi = GL_COLOR_ATTACHMENT0;
                        gl->ClearNamedFramebufferfv(fboOpDst, GL_COLOR, /*GL_DRAW_BUFFER0 +*/ dbi, rgba); _ZglThrowErrorOccuried();
                        break;
                    }
                    }
                    _ZglBindFboAttachment(gl, GL_DRAW_FRAMEBUFFER, fboOpDst, ras->glAttachment, ras->glTarget, NIL, 0, layerIdx, FALSE);
                }
                else
                {
                    _ZglBindFboAttachment(gl, GL_DRAW_FRAMEBUFFER, NIL, ras->glAttachment, ras->glTarget, ras->glHandle, 0, layerIdx, FALSE);

                    switch (ras->glAttachment)
                    {
                    case GL_DEPTH_ATTACHMENT:
                    {
                        gl->ClearBufferfv(GL_DEPTH, 0, &values[i].depth); _ZglThrowErrorOccuried();
                        break;
                    }
                    case GL_DEPTH_STENCIL_ATTACHMENT:
                    {
                        gl->ClearBufferfi(GL_DEPTH_STENCIL, 0, values[i].depth, values[i].stencil); _ZglThrowErrorOccuried();
                        break;
                    }
                    case GL_STENCIL_ATTACHMENT:
                    {
                        GLint sCv = values[i].stencil;
                        gl->ClearBufferiv(GL_STENCIL, 0, &sCv); _ZglThrowErrorOccuried();
                        break;
                    }
                    default:
                    {
                        afxReal const* rgba = values[i].rgba;
                        GLint dbi = GL_COLOR_ATTACHMENT0;
                        gl->ClearBufferfv(GL_COLOR, /*GL_DRAW_BUFFER0 +*/ dbi, rgba); _ZglThrowErrorOccuried();
                        break;
                    }
                    }
                    _ZglBindFboAttachment(gl, GL_DRAW_FRAMEBUFFER, NIL, ras->glAttachment, ras->glTarget, NIL, 0, layerIdx, FALSE);
                }
            }
        }
    }
#endif

    if (useTempFbo)
    {
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        gl->DeleteFramebuffers(1, &tempFbo); _ZglThrowErrorOccuried();
    }
    else
    {
        gl->BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}

_ZGL afxError _ZglDpuResolveCanvas(zglDpu* dpu, avxCanvas src, avxCanvas dst, afxUnit opCnt, avxRasterCopy const ops[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        vkCmdResolveImage
        Purpose: Resolves a multisampled image to a non-multisampled image.
        Filter: Always uses nearest filtering (VK_FILTER_NEAREST).
        Restrictions:
            Source must be multisampled.
            Destination must be single-sampled.
            Format must match.
            Only supports resolving a 1:1 region (no scaling or filtering choice).

        To mimic vkCmdResolveImage, we should:
            Use nearest filtering only (to match Vulkan's behavior).
            Ensure that we resolve from a multisample framebuffer into a single-sample texture.
            Avoid any format conversion or scaling.
    */

    for (afxUnit sbufIdx = 0, dbufIdx = 0; (sbufIdx < src->m.binCnt) && (dbufIdx < dst->m.binCnt); sbufIdx++, dbufIdx++)
    {
        if (src->m.bins[sbufIdx].buf && dst->m.bins[dbufIdx].buf)
            _ZglDpuResolveRaster(dpu, src->m.bins[sbufIdx].buf, dst->m.bins[dbufIdx].buf, opCnt, ops);
    }
    return err;
}

_ZGL afxError _ZglDpuBlitCanvas(zglDpu* dpu, avxCanvas src, avxCanvas dst, afxUnit opCnt, avxRasterBlit const ops[], avxTexelFilter flt)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        vkCmdBlitImage
        Purpose: General-purpose image copy, potentially with format conversion and filtering.
        Filter: User-specified --- VK_FILTER_NEAREST or VK_FILTER_LINEAR.
        More flexible, can scale images and convert formats.

        To match Vulkan's vkCmdBlitImage, OpenGL's glBlitFramebuffer can be used similarly,
        but with either GL_LINEAR or GL_NEAREST as needed. Or, for more general flexibility
        (e.g., for mipmap generation or format conversion), a shader-based blit might be required.
    */

    for (afxUnit sbufIdx = 0, dbufIdx = 0; (sbufIdx < src->m.binCnt) && (dbufIdx < dst->m.binCnt); sbufIdx++, dbufIdx++)
    {
        if (src->m.bins[sbufIdx].buf && dst->m.bins[dbufIdx].buf)
            _ZglDpuBlitRaster(dpu, src->m.bins[sbufIdx].buf, dst->m.bins[dbufIdx].buf, opCnt, ops, flt);
    }
    return err;
}

#if 0
_ZGL afxError _ZglReadjustCanvasCb(avxCanvas canv, afxWarp const whd)
{
	afxError err = { 0 };
	AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
	AFX_ASSERT(whd);
	AFX_ASSERT(whd[0]);
	AFX_ASSERT(whd[1]);
    afxWarp minWhd = { ZGL_MAX_CANVAS_WIDTH, ZGL_MAX_CANVAS_HEIGHT, ZGL_MAX_CANVAS_LAYERS }, surWhd;
    
    for (afxUnit i = 0; i < canv->m.binCnt; i++)
    {
        avxRaster ras = canv->m.bins[i].ras;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AvxGetRasterExtent(ras, 0, surWhd);
            AfxWhdMin(minWhd, minWhd, surWhd);
        }
    }

    if ((whd[0] > minWhd[0]) || (whd[1] > minWhd[1]))
        AfxThrowError();

    if ((whd[0] != minWhd[0]) || (whd[1] != minWhd[1]))
    {
        canv->m.whd[0] = whd[0] < minWhd[0] ? whd[0] : minWhd[0];
        canv->m.whd[1] = whd[1] < minWhd[1] ? whd[1] : minWhd[1];

        for (afxUnit i = 0; i < )
        canv->updFlags[exuIdx] |= ZGL_UPD_FLAG_DEVICE_INST;
    }
	return err;
}
#endif

_ZGL afxError _AfxCanvDropAllSurfaces(avxCanvas canv)
{
	afxError err = { 0 };
	AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

	//canv->m.colorCnt = 0;
    //canv->m.surfCnt = 0;
	return err;
}

_ZGL afxError _ZglCanvDtor(avxCanvas canv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

    afxDrawSystem dsys = AvxGetCanvasHost(canv);

    for (afxUnit i = 0; i < canv->m.binCnt; i++)
    {
        avxRaster ras = canv->m.bins[i].buf;

        if (ras)
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
            AfxDisposeObjects(1, &ras);
            canv->m.bins[i].buf = NIL;
        }
    }

    for (afxUnit i = 0; i < ZGL_MAX_DPUS; i++)
    {
        for (afxUnit j = 0; j < _ZGL_FBO_SWAPS; j++)
        {
            if (canv->perDpu[i][j].glHandle)
            {
                _ZglDsysEnqueueDeletion(dsys, i, GL_FRAMEBUFFER, (afxSize)canv->perDpu[i][j].glHandle);
                canv->perDpu[i][j].glHandle = 0;
            }
        }
    }

    if (_AVX_CLASS_CONFIG_CANV.dtor(canv))
        AfxThrowError();

    return err;
}

_ZGL afxError _ZglCanvCtor(avxCanvas canv, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };

    if (_AVX_CLASS_CONFIG_CANV.ctor(canv, args, invokeNo)) AfxThrowError();
    else
    {
        //canv->m.readjust = _ZglReadjustCanvasCb;

        afxDrawSystem dsys = AvxGetCanvasHost(canv);
        canv->canvUniqueId = ++dsys->canvUniqueId;

        canv->updFlags = ZGL_UPD_FLAG_DEVICE_INST;
        AfxZero(canv->perDpu, sizeof(canv->perDpu));
    }
    return err;
}
