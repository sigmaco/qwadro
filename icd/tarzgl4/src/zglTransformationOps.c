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

//#define _ZGL_DBG_IGNORE_DEPTH_RANGE
#define _ZGL_DBG_IGNORE_PRIM_RESTART
#define _ZGL_DBG_IGNORE_DEPTH_CLAMP

//#define FORCE_CULL_MODE
//#define FORCE_FRONT_FACE
//#define FORCE_VIEWPORT

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

_ZGL void _ZglFlushTsChanges(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;
    
    dpu->primTop = dpu->nextPrimTop;

    avxCullMode nextCullMode = dpu->nextCullMode;
#ifndef FORCE_CULL_MODE
    if (nextCullMode != dpu->cullMode)
#endif
    {
        /*
            GL_CULL_FACE
            If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
        */

        if (nextCullMode == avxCullMode_NONE)
        {
#ifndef FORCE_CULL_MODE
            AFX_ASSERT(dpu->cullMode);
#endif
            gl->Disable(GL_CULL_FACE); _ZglThrowErrorOccuried();
        }
        else
        {
#ifndef FORCE_CULL_MODE
            AFX_ASSERT(!dpu->cullMode);
#endif
            gl->Enable(GL_CULL_FACE); _ZglThrowErrorOccuried();
        }

        if (nextCullMode)
        {
            /*
                glCullFace — specify whether front- or back-facing facets can be culled
                void glCullFace(GLenum mode);

                mode Specifies whether front- or back-facing facets are candidates for culling. Symbolic constants GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK are accepted. The initial value is GL_BACK.

                glCullFace specifies whether front- or back-facing facets are culled (as specified by mode) when facet culling is enabled.
                Facet culling is initially disabled. To enable and disable facet culling, call the glEnable and glDisable commands with the argument GL_CULL_FACE.
                Facets include triangles, quadrilaterals, polygons, and rectangles.

                glFrontFace specifies which of the clockwise and counterclockwise facets are front-facing and back-facing. See glFrontFace.

                If mode is GL_FRONT_AND_BACK, no facets are drawn, but other primitives such as points and lines are drawn.
            */
            gl->CullFace(AfxToGlCullMode(nextCullMode)); _ZglThrowErrorOccuried();
        }
        dpu->cullMode = nextCullMode;
    }

    if (nextCullMode)
    {
        afxBool nextFrontFaceCw = dpu->nextFrontFaceCw;
#ifndef FORCE_FRONT_FACE
        if (nextFrontFaceCw != dpu->frontFaceCw)
#endif
        {
            /*
                glFrontFace — define front- and back-facing polygons
                void glFrontFace(GLenum mode);

                mode Specifies the orientation of front-facing polygons. GL_CW and GL_CCW are accepted. The initial value is GL_CCW.

                In a scene composed entirely of opaque closed surfaces, back-facing polygons are never visible.
                Eliminating these invisible polygons has the obvious benefit of speeding up the rendering of the image.
                To enable and disable elimination of back-facing polygons, call glEnable and glDisable with argument GL_CULL_FACE.

                The projection of a polygon to window coordinates is said to have clockwise winding if an imaginary object following the path from its first vertex, its second vertex, and so on, to its last vertex, and finally back to its first vertex, moves in a clockwise direction about the interior of the polygon.
                The polygon's winding is said to be counterclockwise if the imaginary object following the same path moves in a counterclockwise direction about the interior of the polygon.
                glFrontFace specifies whether polygons with clockwise winding in window coordinates, or counterclockwise winding in window coordinates, are taken to be front-facing.
                Passing GL_CCW to mode selects counterclockwise polygons as front-facing; GL_CW selects clockwise polygons as front-facing.
                By default, counterclockwise polygons are taken to be front-facing.
            */
            gl->FrontFace(nextFrontFaceCw ? GL_CW : GL_CCW); _ZglThrowErrorOccuried();
            dpu->frontFaceCw = nextFrontFaceCw;
        }
    }

    afxMask nextVpUpdMask = dpu->nextVpUpdMask;
#ifndef FORCE_VIEWPORT
    if (nextVpUpdMask)
#endif
    {
        dpu->nextVpUpdMask = NIL;

        afxUnit vpCnt = dpu->nextVpCnt;
        dpu->vpCnt = vpCnt;
        AFX_ASSERT(vpCnt);

#if FORCE_GL_GENERIC_FUNCS
        GLfloat v[ZGL_MAX_VIEWPORTS][4];
        GLdouble v2[ZGL_MAX_VIEWPORTS][2];

        GLuint cnt = 0;
        GLuint first = GL_INVALID_INDEX;

        for (afxUnit i = 0; i < vpCnt; i++)
        {
            if (nextVpUpdMask & AFX_BITMASK(i))
            {
                if (first == GL_INVALID_INDEX)
                    first = i;

                cnt = i + 1 - first;

                dpu->vps[i] = dpu->nextVps[i];
            }
            v[i][0] = dpu->vps[i].origin[0];
            v[i][1] = dpu->vps[i].origin[1];
            v[i][2] = dpu->vps[i].extent[0];
            v[i][3] = dpu->vps[i].extent[1];
            v2[i][0] = dpu->vps[i].minDepth;
            v2[i][1] = dpu->vps[i].maxDepth;
        }

        if (cnt)
        {
            //gl->ViewportArrayv(0, vpCnt, &v[0][0]); _ZglThrowErrorOccuried();
            //gl->DepthRangeArrayv(0, vpCnt, &v2[0][0]); _ZglThrowErrorOccuried();
            gl->ViewportArrayv(first, cnt, &v[first][0]); _ZglThrowErrorOccuried();
            gl->DepthRangeArrayv(first, cnt, &v2[first][0]); _ZglThrowErrorOccuried();
        }
#else
        if (1 < vpCnt)
        {
            AFX_ASSERT(vpCnt);
            GLint x = (GLint)(dpu->nextTs.vps[0].offset[0]);
            GLint y = (GLint)(dpu->nextTs.vps[0].offset[1]);
            GLsizei w = (GLsizei)(dpu->nextTs.vps[0].extent[0]);
            GLsizei h = (GLsizei)(dpu->nextTs.vps[0].extent[1]);
            gl->Viewport(x, y, w, h); _ZglThrowErrorOccuried();

#ifndef _ZGL_DBG_IGNORE_DEPTH_RANGE
            GLdouble n = dpu->nextTs.vps[0].depth[0];
            GLdouble f = dpu->nextTs.vps[0].depth[1];
            gl->DepthRange(n, f); _ZglThrowErrorOccuried();
#endif
        }
        else
        {
            afxUnit cnt = dpu->nextViewportUpdCnt;
            AFX_ASSERT(cnt);
            cnt = AFX_CLAMP(cnt, 0, vpCnt);

            afxMask updMask = dpu->nextViewportUpdMask;

            for (afxUnit i = 0; i < ZGL_MAX_VIEWPORTS; i++)
            {
                if (AFX_TEST_BIT_SET(updMask, i))
                {
                    if (gl->ViewportArrayv)
                    {
                        GLfloat v[ZGL_MAX_VIEWPORTS][4];
                        AFX_ASSERT(ZGL_MAX_VIEWPORTS >= cnt);

                        v[i][0] = dpu->nextTs.vps[i].offset[0],
                        v[i][1] = dpu->nextTs.vps[i].offset[1],
                        v[i][2] = dpu->nextTs.vps[i].extent[0],
                        v[i][3] = dpu->nextTs.vps[i].extent[1];

                        gl->ViewportArrayv(i, 1, &v[0][0]); _ZglThrowErrorOccuried();
                    }
#ifndef _ZGL_DBG_IGNORE_DEPTH_RANGE
                    if (gl->DepthRangeArrayv)
                    {
                        GLdouble v[ZGL_MAX_VIEWPORTS][2];
                        AFX_ASSERT(ZGL_MAX_VIEWPORTS >= cnt);

                        v[i][0] = dpu->nextTs.vps[i].depth[0],
                            v[i][1] = dpu->nextTs.vps[i].depth[1];

                        gl->DepthRangeArrayv(0, 1, &v[0][0]); _ZglThrowErrorOccuried();
                    }
#endif
                    dpu->activeTs.vps[i] = dpu->nextTs.vps[i];
                }

                if (--cnt == 0)
                    break;
            }
        }
#endif
    }

#ifndef _ZGL_DBG_IGNORE_PRIM_RESTART
    afxBool nextPrimRestartEnabled = dpu->nextPrimRestartEnabled;
    if (nextPrimRestartEnabled != dpu->primRestartEnabled)
    {
        /*
            GL_PRIMITIVE_RESTART
            Enables primitive restarting.
            If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index.
            See glPrimitiveRestartIndex.
        */

        if (nextPrimRestartEnabled)
        {
            AFX_ASSERT(!dpu->primRestartEnabled);
            gl->Enable(GL_PRIMITIVE_RESTART); _ZglThrowErrorOccuried();

            /*
                glPrimitiveRestartIndex — specify the primitive restart index
                void glPrimitiveRestartIndex(GLuint index);
                index Specifies the value to be interpreted as the primitive restart index.

                glPrimitiveRestartIndex specifies a vertex array element that is treated specially when primitive restarting is enabled. This is known as the primitive restart index.

                When one of the Draw* commands transfers a set of generic attribute array elements to the GL, if the index within the vertex arrays corresponding to that set is equal to the primitive restart index, then the GL does not process those elements as a vertex.
                Instead, it is as if the drawing command ended with the immediately preceding transfer, and another drawing command is immediately started with the same parameters, but only transferring the immediately following element through the end of the originally specified elements.

                When either glDrawElementsBaseVertex, glDrawElementsInstancedBaseVertex or glMultiDrawElementsBaseVertex is used, the primitive restart comparison occurs before the basevertex offset is added to the array index.
            */

            //gl->PrimitiveRestartIndex(); _ZglThrowErrorOccuried();

            /*
                GL_PRIMITIVE_RESTART_FIXED_INDEX
                Enables primitive restarting with a fixed index.
                If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the fixed primitive index for the specified index type.
                The fixed index is equal to 2^n - 1
                where n is equal to 8 for GL_UNSIGNED_BYTE, 16 for GL_UNSIGNED_SHORT and 32 for GL_UNSIGNED_INT.
            */

            //gl->Enable(GL_PRIMITIVE_RESTART_FIXED_INDEX); _ZglThrowErrorOccuried();
        }
        else
        {
            AFX_ASSERT(dpu->primRestartEnabled);
            gl->Disable(GL_PRIMITIVE_RESTART); _ZglThrowErrorOccuried();
        }
        dpu->primRestartEnabled = nextPrimRestartEnabled;
    }
#endif

#ifndef _ZGL_DBG_IGNORE_DEPTH_CLAMP
    afxBool nextDepthClampEnabled = dpu->nextDepthClampEnabled;
    if (nextDepthClampEnabled != dpu->depthClampEnabled)
    {
        /*
            GL_DEPTH_CLAMP
            If enabled, the -wc =< zc =< wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange.
        */

        if (nextDepthClampEnabled)
        {
            AFX_ASSERT(!dpu->depthClampEnabled);
            gl->Enable(GL_DEPTH_CLAMP); _ZglThrowErrorOccuried();
        }
        else
        {
            AFX_ASSERT(dpu->depthClampEnabled);
            gl->Disable(GL_DEPTH_CLAMP); _ZglThrowErrorOccuried();
        }
        dpu->depthClampEnabled = nextDepthClampEnabled;
    }
#endif

}

_ZGL void DpuSetViewports(zglDpu* dpu, afxUnit first, afxUnit cnt, avxViewport const vp[])
{
    afxError err = { 0 };
    AFX_ASSERT_RANGE(ZGL_MAX_VIEWPORTS, first, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit vpIdx = first + i;
        dpu->nextVps[vpIdx] = vp[i];
        dpu->nextVpUpdMask |= AFX_BITMASK(vpIdx);
    }
}

_ZGL void DpuBeginQueryIndexedEXT(zglDpu* dpu, avxQueryPool queryPool, afxUnit query, afxBool precise, afxUnit index)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Begin an indexed query.

        queryPool is the query pool that will manage the results of the query.
        query is the query index within the query pool that will contain the results.
        flags is a bitmask of VkQueryControlFlagBits specifying constraints on the types of queries that can be performed.
        index is the query type specific index. When the query type is VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT or VK_QUERY_TYPE_PRIMITIVES_GENERATED_EXT, the index represents the vertex stream.
    */

    GLenum glTarget = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN /*queryPool->glTarget*/;
    gl->BeginQueryIndexed(glTarget, index, queryPool->perDpu[dpu->m.exuIdx].glHandle[query]); _ZglThrowErrorOccuried();
}

_ZGL void DpuBeginTransformFeedbackEXT(zglDpu* dpu, afxUnit firstCounterBuffer, afxUnit counterBufferCount, avxBuffer pCounterBuffers[], const afxSize pCounterBufferOffsets[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Make transform feedback active in the command buffer.

        firstCounterBuffer is the index of the first transform feedback buffer corresponding to pCounterBuffers[0] and pCounterBufferOffsets[0].
        counterBufferCount is the size of the pCounterBuffers and pCounterBufferOffsets arrays.
        pCounterBuffers is NULL or a pointer to an array of VkBuffer handles to counter buffers. Each buffer contains a 4 byte integer value representing the byte offset from the start of the corresponding transform feedback buffer from where to start capturing vertex data. If the byte offset stored to the counter buffer location was done using vkCmdEndTransformFeedbackEXT it can be used to resume transform feedback from the previous location. In that case, a pipeline barrier is required between the calls to vkCmdEndTransformFeedbackEXT and vkCmdBeginTransformFeedbackEXT, with VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT as the source and destination stages, VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT as the source access and VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT as the destination access. If pCounterBuffers is NULL, then transform feedback will start capturing vertex data to byte offset zero in all bound transform feedback buffers. For each element of pCounterBuffers that is VK_NULL_HANDLE, transform feedback will start capturing vertex data to byte zero in the corresponding bound transform feedback buffer.
        pCounterBufferOffsets is NULL or a pointer to an array of VkDeviceSize values specifying offsets within each of the pCounterBuffers where the counter values were previously written. The location in each counter buffer at these offsets must be large enough to contain 4 bytes of data. This data is the number of bytes captured by the previous transform feedback to this buffer. If pCounterBufferOffsets is NULL, then it is assumed the offsets are zero.
    */

    GLenum fillMode[] =
    {
        [avxFillMode_POINT] = GL_POINTS, // GL_POINTS
        [avxFillMode_LINE] = GL_LINES, // GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP, GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY
        [avxFillMode_FACE] = GL_TRIANGLES, // GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
    };

    gl->BeginTransformFeedback(fillMode[dpu->nextFillMode]); _ZglThrowErrorOccuried();
}

_ZGL void DpuBindTransformFeedbackBuffersEXT(zglDpu* dpu, afxUnit firstBinding, afxUnit bindingCount, avxBuffer pBuffers[], const afxSize pOffsets[], const afxSize pSizes[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    GLuint xfbo = 0;
    for (afxUnit i = 0; i < bindingCount; i++)
    {
        if (pOffsets || pSizes)
        {
            afxUnit offset = pOffsets ? pOffsets[i] : 0;
            afxUnit range = pSizes ? pSizes[i] : pBuffers[i]->m.reqSiz - offset;
            gl->TransformFeedbackBufferRange(xfbo, firstBinding, pBuffers[i]->glHandle, offset, range); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->TransformFeedbackBufferBase(xfbo, firstBinding, pBuffers[i]->glHandle); _ZglThrowErrorOccuried();
        }
    }
}

_ZGL void DpuDrawIndirectByteCountEXT(zglDpu* dpu, afxUnit instanceCount, afxUnit firstInstance, avxBuffer counterBuffer, afxSize counterBufferOffset, afxUnit counterOffset, afxUnit vertexStride)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Draw primitives with indirect parameters where the vertex count is derived from the counter byte value in the counter buffer.

        instanceCount is the number of instances to draw.
        firstInstance is the instance ID of the first instance to draw.
        counterBuffer is the buffer handle from where the byte count is read.
        counterBufferOffset is the offset into the buffer used to read the byte count, which is used to calculate the vertex count for this draw call.
        counterOffset is subtracted from the byte count read from the counterBuffer at the counterBufferOffset
        vertexStride is the stride in bytes between each element of the vertex data that is used to calculate the vertex count from the counter value. This value is typically the same value that was used in the graphics pipeline state when the transform feedback was captured as the XfbStride.
    */

    // To record a non-indexed draw call, where the vertex count is based on a byte count read from a buffer and the passed in vertex stride parameter.

    GLenum top = AfxToGlTopology(dpu->nextPrimTop);

    GLuint xfbo = 0;

    //for (afxUnit i = 0; i < )
    if (instanceCount > 1)
    {
        // NOTE: Stopped because counter is internally managed by GL driver.
        // render multiple instances of primitives using a count derived from a transform feedback object.
        gl->DrawTransformFeedbackInstanced(top, xfbo, instanceCount); _ZglThrowErrorOccuried();


        // render multiple instances of primitives using a count derived from a specifed stream of a transform feedback object.
        //gl->DrawTransformFeedbackStreamInstanced(top, xfbo, stream, instanceCount); _ZglThrowErrorOccuried();
    }
    else
    {
        // NOTE: Stopped because counter is internally managed by GL driver.
        // render primitives using a count derived from a specifed stream of a transform feedback object.
        //gl->DrawTransformFeedbackStream(top, xfbo, stream); _ZglThrowErrorOccuried();

        // render primitives using a count derived from a transform feedback object.
        gl->DrawTransformFeedback(top, xfbo); _ZglThrowErrorOccuried();
    }
}

_ZGL void DpuEndQueryIndexedEXT(zglDpu* dpu, avxQueryPool queryPool, afxUnit query, afxUnit index)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Ends a query.

        queryPool is the query pool that is managing the results of the query.
        query is the query index within the query pool where the result is stored.
        index is the query type specific index.
    */

    GLenum glTarget = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN /*queryPool->glTarget*/;
    gl->EndQueryIndexed(glTarget, index); _ZglThrowErrorOccuried();
}

_ZGL void DpuEndTransformFeedbackEXT(zglDpu* dpu, afxUnit firstCounterBuffer, afxUnit counterBufferCount, avxBuffer pCounterBuffers[], const afxSize pCounterBufferOffsets[])
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        Make transform feedback inactive in the command buffer.

        firstCounterBuffer is the index of the first transform feedback buffer corresponding to pCounterBuffers[0] and pCounterBufferOffsets[0].
        counterBufferCount is the size of the pCounterBuffers and pCounterBufferOffsets arrays.
        pCounterBuffers is NULL or a pointer to an array of VkBuffer handles to counter buffers. The counter buffers are used to record the current byte positions of each transform feedback buffer where the next vertex output data would be captured. This can be used by a subsequent vkCmdBeginTransformFeedbackEXT call to resume transform feedback capture from this position. It can also be used by vkCmdDrawIndirectByteCountEXT to determine the vertex count of the draw call.
        pCounterBufferOffsets is NULL or a pointer to an array of VkDeviceSize values specifying offsets within each of the pCounterBuffers where the counter values can be written. The location in each counter buffer at these offsets must be large enough to contain 4 bytes of data. The data stored at this location is the byte offset from the start of the transform feedback buffer binding where the next vertex data would be written. If pCounterBufferOffsets is NULL, then it is assumed the offsets are zero.
    */

    gl->EndTransformFeedback(); _ZglThrowErrorOccuried();
}

_ZGL void DpuDrawMeshTasksEXT(zglDpu* dpu, afxUnit w, afxUnit h, afxUnit d)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
         One or more work groups is launched by calling

          void DrawMeshTasksEXT(uint num_groups_x,
                                uint num_groups_y,
                                uint num_groups_z);

        If there is an active program object for the task shader stage,
        work groups are processed by the active program for the task
        shader stage. If there is no active program object for the task shader
        stage, work groups are instead processed by the active
        program for the mesh shader stage.  The active program for both shader
        stages will be determined in the same manner as the active program for other
        pipeline stages, as described in section 7.3. While the individual shader
        invocations within a work group are executed as a unit, work groups are
        executed completely independently and in unspecified order.
    */

    void (*DrawMeshTasksEXT)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) = NIL;
    DrawMeshTasksEXT(w, h, d);
}

_ZGL void DpuDrawMeshTasksIndirectEXT(zglDpu* dpu, avxBuffer buf, afxSize offset, afxUnit drawCnt, afxUnit stride)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        The command

          void DrawMeshTasksIndirectEXT(intptr indirect);

          typedef struct {
            uint x;
            uint y;
	    uint z;
          } DrawMeshTasksIndirectCommandEXT;

        is equivalent to calling DrawMeshTasksEXT with the parameters sourced from
        a DrawMeshTasksIndirectCommandEXT struct stored in the buffer currently
        bound to the DRAW_INDIRECT_BUFFER binding at an offset, in basic machine
        units, specified by <indirect>.  If the <x>, <y> and <z> read from the
        indirect draw buffer exceed the limits specified in DrawMeshTasksEXT, then
        the results of this command are undefined.

        ..............

        The command

          void MultiDrawMeshTasksIndirectEXT(intptr indirect,
                                             sizei drawcount,
                                             sizei stride);

        behaves identically to DrawMeshTasksIndirectEXT, except that <indirect> is
        treated as an array of <drawcount> DrawMeshTasksIndirectCommandEXT
        structures.    <indirect> contains the offset of the first element of the
        array within the buffer currently bound to the DRAW_INDIRECT buffer
        binding. <stride> specifies the distance, in basic machine units, between
        the elements of the array. If <stride> is zero, the array elements are
        treated as tightly packed. <stride> must be a multiple of four, otherwise
        an INVALID_VALUE error is generated.
    */

    void (*DrawMeshTasksIndirectEXT)(GLintptr indirect) = NIL;
    void (*MultiDrawMeshTasksIndirectEXT)(GLintptr indirect, GLsizei drawcount, GLsizei stride) = NIL;

    DpuBindAndSyncBuf(dpu, /*GL_DRAW_INDIRECT*/GL_DRAW_INDIRECT_BUFFER, buf, TRUE);

    if (1 >= drawCnt)
    {
        DrawMeshTasksIndirectEXT(offset);
    }
    else
    {
        MultiDrawMeshTasksIndirectEXT(offset, drawCnt, stride);
    }
}

_ZGL void DpuDrawMeshTasksIndirectCountEXT(zglDpu* dpu, avxBuffer buf, afxSize offset, avxBuffer cntBuf, afxSize cntBufOffset, afxUnit maxDrawCnt, afxUnit stride)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    /*
        The command

          void MultiDrawMeshTasksIndirectCountEXT(intptr indirect,
                                                  intptr drawcount,
                                                  sizei maxdrawcount,
                                                  sizei stride);

        behaves similarly to MultiDrawMeshTasksIndirectEXT, except that <drawcount>
        defines an offset (in bytes) into the buffer object bound to the
        PARAMETER_BUFFER binding point at which a single <sizei> typed value
        is stored, which contains the draw count. <maxdrawcount> specifies the
        maximum number of draws that are expected to be stored in the buffer.
        If the value stored at <drawcount> into the buffer is greater than
        <maxdrawcount>, an implementation stop processing draws after
        <maxdrawcount> parameter sets.
    */

    void (*MultiDrawMeshTasksIndirectCountEXT)(GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) = NIL;

    DpuBindAndSyncBuf(dpu, /*GL_DRAW_INDIRECT*/GL_DRAW_INDIRECT_BUFFER, buf, TRUE);
    DpuBindAndSyncBuf(dpu, GL_PARAMETER_BUFFER, cntBuf, TRUE);

    MultiDrawMeshTasksIndirectCountEXT(offset, cntBufOffset, maxDrawCnt, stride);

}
