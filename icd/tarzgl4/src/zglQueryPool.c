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

_ZGL void _DpuBeginQuery(zglDpu* dpu, avxQueryPool qryp, afxUnit qryIdx, afxBool precise)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->m.slotCnt, qryIdx, 1);
    glVmt const* gl = dpu->gl;

    /*
        Occlusion Query

        vkCmdBeginQuery(cmd, pool, index, 0);
        // draw
        vkCmdEndQuery(cmd, pool, index);

        glBeginQuery(GL_SAMPLES_PASSED, query);
        // draw
        glEndQuery(GL_SAMPLES_PASSED);
        GLuint result;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);


        Pipeline Statistics (if supported)

        vkCmdBeginQuery(cmd, pool, index, 0);
        // draw
        vkCmdEndQuery(cmd, pool, index);
        Query pool is created with VK_QUERY_TYPE_PIPELINE_STATISTICS.

        OpenGL: Use ARB_pipeline_statistics_query extension (if supported)

        glBeginQuery(GL_VERTICES_SUBMITTED_ARB, query);
        // draw
        glEndQuery(GL_VERTICES_SUBMITTED_ARB);

        GLuint result;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);

    */

    afxUnit vaoHandleIdx = dpu->dpuIterIdx % _ZGL_VAO_SWAPS;
    gl->BeginQuery(qryp->glTarget, qryp->perDpu[dpu->m.exuIdx].glHandle[qryIdx]);
    dpu->timeElapsedQueryIdActive = qryp->perDpu[dpu->m.exuIdx].glHandle[qryIdx];
}

_ZGL void _DpuEndQuery(zglDpu* dpu, avxQueryPool qryp, afxUnit queryIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->m.slotCnt, queryIdx, 1);
    glVmt const* gl = dpu->gl;

    gl->EndQuery(qryp->glTarget);
    dpu->timeElapsedQueryIdActive = 0;
}

_ZGL void _DpuCopyQueryResults(zglDpu* dpu, avxQueryPool qryp, afxUnit baseQuery, afxUnit queryCnt, avxBuffer buf, afxSize offset, afxSize stride, avxQueryResultFlags flags)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->m.slotCnt, baseQuery, queryCnt);
    AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &buf);
    AFX_ASSERT_RANGE(AvxGetBufferCapacity(buf, 0), offset, stride);
    glVmt const* gl = dpu->gl;

    /*
        Not all pipeline statistics are available in core OpenGL; we'll need ARB extensions.
        OpenGL query results can stall the CPU if the GPU hasn't finished; unless you use GL_QUERY_RESULT_AVAILABLE first.

        Checking Availability (Non-blocking Read)

        GLint available = 0;
        glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);

        if (available) {
            // Now safe to read result
        }

        Equivalent to VK_QUERY_RESULT_WITH_AVAILABILITY_BIT.
    */

    /*
        glGetQueryObject and glGetQueryBufferObject implicitly flush the GL pipeline so that any incomplete rendering delimited by the occlusion query completes in finite time.

        If multiple queries are issued using the same query object id before calling glGetQueryObject or glGetQueryBufferObject, the results of the most recent query will be returned. In this case, when issuing a new query, the results of the previous query are discarded.

        glGetQueryObjecti64v and glGetQueryObjectui64v are available only if the GL version is 3.3 or greater.

        GL_QUERY_RESULT_NO_WAIT is accepted for pname only if the GL version is 4.4 or greater.

        The GL_QUERY_RESULT_BUFFER target is available only if the GL version is 4.4 or higher. On earlier versions of the GL, params is always an address in client
    */

    //DpuBindAndSyncBuf(dpu, GL_QUERY_RESULT_BUFFER, buf, TRUE);
    DpuBindAndSyncBuf(dpu, GL_QUERY_BUFFER, buf, TRUE);

    GLenum pname = (flags & avxQueryResultFlag_WAIT) ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT;

    if (flags & avxQueryResultFlag_64)
    {
        afxSize offsetAccum = offset;

        if (!stride)
            stride = sizeof(afxUnit64);

        for (afxUnit i = 0; i < queryCnt; i++)
        {
            afxUnit idx = baseQuery + i;

            gl->GetQueryObjectui64v(qryp->perDpu[dpu->m.exuIdx].glHandle[i], pname, (GLuint64[]) { offsetAccum });

            if (flags & avxQueryResultFlag_WITH_AVAIL)
            {
                if (gl->GetQueryBufferObjectui64v)
                {
                    gl->GetQueryBufferObjectui64v(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, pname, offsetAccum); _ZglThrowErrorOccuried();
                }
                else
                {
                    gl->GetQueryBufferObjectui64v(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, GL_QUERY_RESULT_AVAILABLE, offsetAccum); _ZglThrowErrorOccuried();
                }
            }
            else
            {
                if (gl->GetQueryBufferObjectui64v)
                {
                    gl->GetQueryBufferObjectui64v(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, pname, offsetAccum); _ZglThrowErrorOccuried();
                }
                else
                {
                    gl->GetQueryObjectui64v(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], pname, (GLuint64[]) { offsetAccum }); _ZglThrowErrorOccuried();
                }
            }

            offsetAccum += stride;
        }
    }
    else
    {
        afxSize offsetAccum = offset;

        if (!stride)
            stride = sizeof(afxUnit);

        for (afxUnit i = 0; i < queryCnt; i++)
        {
            afxUnit idx = baseQuery + i;

            if (flags & avxQueryResultFlag_WITH_AVAIL)
            {
                if (gl->GetQueryBufferObjectuiv)
                {
                    gl->GetQueryBufferObjectuiv(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, pname, offsetAccum); _ZglThrowErrorOccuried();
                }
                else
                {
                    gl->GetQueryBufferObjectuiv(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, GL_QUERY_RESULT_AVAILABLE, offsetAccum); _ZglThrowErrorOccuried();
                }
            }
            else
            {
                if (gl->GetQueryBufferObjectuiv)
                {
                    gl->GetQueryBufferObjectuiv(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], buf->glHandle, pname, offsetAccum); _ZglThrowErrorOccuried();
                }
                else
                {
                    gl->GetQueryObjectuiv(qryp->perDpu[dpu->m.exuIdx].glHandle[idx], pname, (GLuint[]) { offsetAccum }); _ZglThrowErrorOccuried();
                }
            }
        }

        offsetAccum += stride;
    }

    //DpuBindAndSyncBuf(dpu, GL_QUERY_RESULT_BUFFER, NIL, TRUE);
    DpuBindAndSyncBuf(dpu, GL_QUERY_BUFFER, NIL, TRUE);
}

_ZGL void _DpuResetQueries(zglDpu* dpu, avxQueryPool qryp, afxUnit baseQuery, afxUnit queryCnt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->m.slotCnt, baseQuery, queryCnt);
    glVmt const* gl = dpu->gl;

    // Resets queries to be reused.
    // No exact equivalent in GL; queries are automatically reset when re-used with glBeginQuery
}

_ZGL void _DpuQueryTimestamp(zglDpu* dpu, avxQueryPool qryp, afxUnit queryIdx, avxBusStage stage)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    AFX_ASSERT_RANGE(qryp->m.slotCnt, queryIdx, 1);
    glVmt const* gl = dpu->gl;
    AFX_ASSERT(qryp->glTarget == GL_TIMESTAMP);

    /*
        Timestamps are special. Unlike others, timestamps don't use glBeginQuery/glEndQuery.
        They are set via glQueryCounter, not started/stopped.
        Handle them specially; they just need a free slot, not an active range.
        
        Timestamps in OpenGL require a separate query object created with glGenQueries and used with glQueryCounter.

        Timestamp Query

        vkCmdQueryTimestamp(cmd, stage, pool, index);

        glQueryCounter(query, GL_TIMESTAMP);
        // Later:
        GLuint64 timestamp;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timestamp);

    */

    // Inserts a GPU timestamp into a query.
    // glQueryCounter(query, GL_TIMESTAMP).
    AFX_ASSERT(qryp->glTarget == GL_TIMESTAMP);
    gl->QueryCounter(qryp->perDpu[dpu->m.exuIdx].glHandle[queryIdx], GL_TIMESTAMP);
}

_ZGL afxError _DpuBindAndSyncQryp(zglDpu* dpu, afxBool syncOnly, avxQueryPool qryp)
{
    //AfxEntry("pip=%p", pip);
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;


    if (!qryp)
    {
        
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
        GLuint* glHandle = &qryp->perDpu[dpu->m.exuIdx].glHandle[/*dpu->m.exuIdx * qryp->m.slotCnt*/0];

        if ((!glHandle[0]) || (qryp->updFlags & ZGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle[0])
            {
                gl->DeleteQueries(qryp->m.slotCnt, glHandle); _ZglThrowErrorOccuried();
                glHandle = NIL;
            }
            gl->GenQueries(qryp->m.slotCnt, glHandle); _ZglThrowErrorOccuried();
            //qryp->glHandle = glHandle;

            if (qryp->m.tag.len)
            {
                for (afxUnit i = 0; i < qryp->m.slotCnt; i++)
                {
                    gl->ObjectLabel(GL_QUERY, glHandle[i], qryp->m.tag.len, (GLchar const*)qryp->m.tag.start); _ZglThrowErrorOccuried();
                }
            }
            ///AfxReportMessage("Query qryp inited. %u", qryp->m.slotCnt);
        }

        qryp->updFlags |= ~ZGL_UPD_FLAG_DEVICE;
    }

    return err;
}

_ZGL afxError _ZglQrypDtor(avxQueryPool qryp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);
    afxDrawSystem dsys = AvxGetQueryPoolHost(qryp);

    for (afxUnit j = 0; j < ZGL_MAX_DPUS; j++)
    {
        if (qryp->perDpu[j].glHandle)
        {
            for (afxUnit i = 0; i < qryp->m.slotCnt; i++)
            {
                if (qryp->perDpu[j].glHandle[/*j * qryp->m.slotCnt + */i])
                {
                    _ZglDsysEnqueueDeletion(dsys, j, GL_QUERY_BUFFER, (afxSize)qryp->perDpu[j].glHandle[/*j * qryp->m.slotCnt + */i]);
                }
            }
            AfxDeallocate((void**)&qryp->perDpu[j].glHandle, AfxHere());
            qryp->perDpu[j].glHandle = 0;
        }
    }
    return err;
}

_ZGL afxError _ZglQrypCtor(avxQueryPool qryp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_QRYP, 1, &qryp);

    afxDrawSystem dsys = args[0];
    avxQueryPoolConfig const* cfg = (avxQueryPoolConfig const*)args[1];

    for (afxUnit i = 0; i < ZGL_MAX_DPUS; i++)
    {
        AfxCoallocate(qryp->m.slotCnt, sizeof(qryp->perDpu[i].glHandle), 0, AfxHere(), (void**)&qryp->perDpu[i].glHandle);
    }
    qryp->updFlags = ZGL_UPD_FLAG_DEVICE_INST;
    qryp->m.type = cfg->type;
    qryp->m.slotCnt = cfg->slotCnt;

    /*
        Query Target	            Description
        GL_SAMPLES_PASSED	        Occlusion query
        GL_ANY_SAMPLES_PASSED	    Binary occlusion test (faster)
        GL_TIME_ELAPSED	            Measures duration between glBeginQuery and glEndQuery
        GL_TIMESTAMP	            Query a single timestamp via glQueryCounter
        GL_PRIMITIVES_GENERATED	    Pipeline stat (with extension)
        GL_VERTICES_SUBMITTED_ARB	Pipeline stat (with ARB extension)
    */

    if (qryp->m.type == avxQueryType_OCCLUSION)
    {
        qryp->glTarget = GL_SAMPLES_PASSED;
    }
    else if (qryp->m.type == avxQueryType_PIPELINE)
    {
        qryp->glTarget = 0;
    }
    else if (qryp->m.type == avxQueryType_TIMESTAMP)
    {
        qryp->glTarget = GL_TIME_ELAPSED;
    }
    else AfxThrowError();

    qryp->qrypUniqueId = ++dsys->qrypUniqueId;

    return err;
}
