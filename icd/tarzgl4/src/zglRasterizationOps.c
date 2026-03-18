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

#define _ZGL_DBG_IGNORE_RASTERIZER_DISCARD
#define _ZGL_DBG_IGNORE_DEPTH_BIAS
 //#define _ZGL_DBG_IGNORE_DEPTH_TEST
 //#define _ZGL_DBG_IGNORE_DEPTH_WRITE
#define _ZGL_DBG_IGNORE_DEPTH_BOUNDS_TEST
#define _ZGL_DBG_IGNORE_LINE_WIDTH
//#define _ZGL_DBG_IGNORE_STENCIL_TEST
#define _ZGL_DBG_IGNORE_MULTISAMPLING
//#define _ZGL_DBG_IGNORE_BLEND
//#define _ZGL_DBG_IGNORE_LOGICAL_OP
//#define _ZGL_DBG_IGNORE_COLOR_MASK
//#define _ZGL_DBG_IGNORE_SCISSOR_TEST

//#define FORCE_FILL_MODE
//#define FORCE_DEPTH_TEST
//#define FORCE_DEPTH_WRITE
//#define FORCE_DEPTH_COMPARE
//#define FORCE_STENCIL_TEST
//#define FORCE_STENCIL_FUNC
//#define FORCE_STENCIL_MASK
//#define FORCE_SCISSOR_TEST
//#define FORCE_BLEND_EQUATION
//#define FORCE_BLEND_FUNC
//#define FORCE_COLOR_WRITE_MASK
//#define FORCE_LOGIC_OP

#include "zglUtils.h"
#include "zglCommands.h"
#include "zglObjects.h"

_ZGLINL void _ZglFlushDsChanges(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

    // DEPTH BIAS
#ifndef _ZGL_DBG_IGNORE_DEPTH_BIAS
    afxBool nextDepthBiasEnabled = dpu->nextDepthBiasEnabled;
    if (nextDepthBiasEnabled != dpu->depthBiasEnabled)
    {
        /*
            GL_POLYGON_OFFSET_FILL If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_LINE If enabled, and if the polygon is rendered in GL_LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed.
            See glPolygonOffset.

            GL_POLYGON_OFFSET_POINT If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode.
            See glPolygonOffset.
        */

        static int const QwadroToGlPolygonModeBasedOffset[] =
        {
            GL_POLYGON_OFFSET_FILL,
            GL_POLYGON_OFFSET_LINE,
            GL_POLYGON_OFFSET_POINT
        };
        AFX_ASSERT(QwadroToGlPolygonModeBasedOffset[avxFillMode_FACE] == GL_POLYGON_OFFSET_FILL);

        if (nextDepthBiasEnabled)
        {
            gl->Enable(QwadroToGlPolygonModeBasedOffset[nextFillMode]); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(QwadroToGlPolygonModeBasedOffset[nextFillMode]); _ZglThrowErrorOccuried();
        }
        dpu->depthBiasEnabled = nextDepthBiasEnabled;
    }

    if (nextDepthBiasEnabled)
    {
        afxReal nextDepthBiasSlopeScale = dpu->nextDepthBiasSlopeScale;
        afxReal nextDepthBiasConstFactor = dpu->nextDepthBiasConstFactor;
        afxReal nextDepthBiasClamp = dpu->nextDepthBiasClamp;

        if ((nextDepthBiasSlopeScale != dpu->depthBiasConstFactor) ||
            (nextDepthBiasSlopeScale != dpu->depthBiasSlopeScale) ||
            (nextDepthBiasClamp != dpu->depthBiasClamp))
        {
            /*
                glPolygonOffset — set the scale and units used to calculate depth values
                void glPolygonOffset(GLfloat factor, GLfloat units);

                factor Specifies a scale factor that is used to create a variable depth offset for each polygon. The initial value is 0.
                units Is multiplied by an implementation-specific value to create a constant depth offset. The initial value is 0.

                When GL_POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_LINE, or GL_POLYGON_OFFSET_POINT is enabled, each fragment's depth value will be offset after it is interpolated from the depth values of the appropriate vertices.
                The value of the offset is factor×DZ+r×units, where DZ is a measurement of the change in depth relative to the screen area of the polygon, and r is the smallest value that is guaranteed to produce a resolvable offset for a given implementation.
                The offset is added before the depth test is performed and before the value is written into the depth buffer.

                glPolygonOffset is useful for rendering hidden-line images, for applying decals to surfaces, and for rendering solids with highlighted edges.
            */

            if (gl->PolygonOffsetClamp)
            {
                gl->PolygonOffsetClamp(nextDepthBiasSlopeScale, nextDepthBiasConstFactor, nextDepthBiasClamp); _ZglThrowErrorOccuried();
            }
            else
            {
                gl->PolygonOffset(nextDepthBiasSlopeScale, nextDepthBiasConstFactor); _ZglThrowErrorOccuried();
            }

            dpu->depthBiasConstFactor = nextDepthBiasConstFactor;
            dpu->depthBiasSlopeScale = nextDepthBiasSlopeScale;
            dpu->depthBiasClamp = nextDepthBiasClamp;
        }
    }
#endif

    // DEPTH TEST
#ifndef _ZGL_DBG_IGNORE_DEPTH_TEST

    afxBool nextDepthTestEnabled = dpu->nextDepthTestEnabled;
#ifndef FORCE_DEPTH_TEST
    if (nextDepthTestEnabled != dpu->depthTestEnabled)
#endif
    {
        /*
            GL_DEPTH_TEST
            If enabled, do depth comparisons and update the depth buffer.
            Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
            See glDepthFunc and glDepthRange.
        */

        if (nextDepthTestEnabled)
        {
#ifndef FORCE_DEPTH_TEST
            AFX_ASSERT(!dpu->depthTestEnabled);
#endif
            gl->Enable(GL_DEPTH_TEST); _ZglThrowErrorOccuried();
        }
        else
        {
#ifndef FORCE_DEPTH_TEST
            AFX_ASSERT(dpu->depthTestEnabled);
#endif
            gl->Disable(GL_DEPTH_TEST); _ZglThrowErrorOccuried();
        }
        dpu->depthTestEnabled = nextDepthTestEnabled;
    }

    if (nextDepthTestEnabled)
    {
        avxCompareOp nextDepthCompareOp = dpu->nextDepthCompareOp;
#ifndef FORCE_DEPTH_COMPARE
        if (nextDepthCompareOp != dpu->depthCompareOp)
#endif
        {
            /*
                glDepthFunc — specify the value used for depth buffer comparisons
                void glDepthFunc(GLenum func);

                func = Specifies the depth comparison function. Symbolic constants GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, and GL_ALWAYS are accepted. The initial value is GL_LESS.

                glDepthFunc specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer.
                The comparison is performed only if depth testing is enabled. (See glEnable and glDisable of GL_DEPTH_TEST.)

                func specifies the conditions under which the pixel will be drawn. The comparison functions are as follows:

                GL_NEVER Never passes.
                GL_LESS Passes if the incoming depth value is less than the stored depth value.
                GL_EQUAL Passes if the incoming depth value is equal to the stored depth value.
                GL_LEQUAL Passes if the incoming depth value is less than or equal to the stored depth value.
                GL_GREATER Passes if the incoming depth value is greater than the stored depth value.
                GL_NOTEQUAL Passes if the incoming depth value is not equal to the stored depth value.
                GL_GEQUAL Passes if the incoming depth value is greater than or equal to the stored depth value.
                GL_ALWAYS Always passes.

                The initial value of func is GL_LESS. Initially, depth testing is disabled. If depth testing is disabled or if no depth buffer exists, it is as if the depth test always passes.

                Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
                In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS.
            */

            AFX_ASSERT(nextDepthCompareOp < avxCompareOp_TOTAL);
            gl->DepthFunc(ZglToGlCompareOp(nextDepthCompareOp)); _ZglThrowErrorOccuried();
            dpu->depthCompareOp = nextDepthCompareOp;
        }

#ifndef _ZGL_DBG_IGNORE_DEPTH_WRITE
        afxBool nextDepthWriteDisabled = dpu->nextDepthWriteDisabled;

        /*
            In Vulkan, depth write is explicitly tied to the depth test being enabled.
            If the depth test is disabled, the depth write must also be disabled.
            If the depth test is disabled, there is no meaningful way to write to the depth buffer,
            as no comparison occurs to decide if the depth value should be updated.
        */

        if (!nextDepthTestEnabled)
            nextDepthWriteDisabled = TRUE;

#if 0
        if (dpu->dsDts[0].storeOp != avxStoreOp_STORE)
        {
            // disable depth writing
            nextDepthWriteDisabled = TRUE;
        }
#endif

#ifndef FORCE_DEPTH_WRITE
        if (nextDepthWriteDisabled != dpu->depthWriteDisabled)
#endif
        {
            /*
                glDepthMask — enable or disable writing into the depth buffer.
                void glDepthMask(GLboolean flag);

                flag = Specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

                glDepthMask specifies whether the depth buffer is enabled for writing. If flag is GL_FALSE, depth buffer writing is disabled. Otherwise, it is enabled. Initially, depth buffer writing is enabled.

                Even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
                In order to unconditionally write to the depth buffer, the depth test should be enabled and set to GL_ALWAYS (see glDepthFunc).
            */

            if (nextDepthWriteDisabled)
            {
                //AFX_ASSERT(dpu->activeRs.depthWriteDisabled);
                gl->DepthMask(GL_FALSE); _ZglThrowErrorOccuried();
            }
            else
            {
                //AFX_ASSERT(!dpu->activeRs.depthWriteDisabled);
                gl->DepthMask(GL_TRUE); _ZglThrowErrorOccuried();
            }
            dpu->depthWriteDisabled = nextDepthWriteDisabled;
        }
#endif
    }
#endif

#ifndef _ZGL_DBG_IGNORE_DEPTH_BOUNDS_TEST
    /*
        Can you have the depth test disabled and still have the depth bounds test enabled?
        RESOLUTION:  Yes.  The two tests operate independently.

        How does the depth bounds test operate if there is no depth buffer?
        RESOLUTION:  It is as if the depth bounds test always passes (analogous to the depth test).
    */
    afxBool nextDepthBoundsTestEnabled = dpu->nextDepthBoundsTestEnabled;
    if (nextDepthBoundsTestEnabled != dpu->activeRs.depthBoundsTestEnabled)
    {
        /*
            EXT_depth_bounds_test

            The depth bounds test determines whether the depth value (Zpixel) stored at the location given by the incoming fragment's (xw,yw) location lies within the depth bounds range defined by two values.
            These values are set with

            If there is no depth buffer, it is as if the depth bounds test always passes.
        */

        if (nextDepthBoundsTestEnabled)
        {
            gl->Enable(DEPTH_BOUNDS_TEST_EXT); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(DEPTH_BOUNDS_TEST_EXT); _ZglThrowErrorOccuried();
        }
        dpu->activeRs.depthBoundsTestEnabled = nextDepthBoundsTestEnabled;
    }

    if (nextDepthBoundsTestEnabled)
    {
        /*
            void DepthBoundsEXT(clampd zmin, clampd zmax);

            Each of zmin and zmax are clamped to lie within [0,1] (being of type clampd).  If zmin <= Zpixel <= zmax, then the depth bounds test passes.  Otherwise, the test fails and the fragment is discarded.
            The test is enabled or disabled using Enable or Disable using the constant DEPTH_BOUNDS_TEST_EXT.  When disabled, it is as if the depth bounds test always passes.  If zmin is greater than zmax, then the error INVALID_VALUE is generated.
            The state required consists of two floating-point values and a bit indicating whether the test is enabled or disabled.  In the initial state, zmin and zmax are set to 0.0 and 1.0 respectively; and the depth bounds test is disabled.

            If there is no depth buffer, it is as if the depth bounds test always passes.
        */
        gl->DepthBoundsEXT(dpu->nextDepthBounds[0], dpu->nextDepthBounds[1]); _ZglThrowErrorOccuried();
        dpu->depthBounds[0] = dpu->nextDepthBounds[0];
        dpu->depthBounds[1] = dpu->nextDepthBounds[1];
    }
#endif

    // STENCIL TEST

#ifndef _ZGL_DBG_IGNORE_STENCIL_TEST
    afxBool nextStencilTestEnabled = dpu->nextStencilTestEnabled;
#ifndef FORCE_STENCIL_TEST
    if (nextStencilTestEnabled != dpu->stencilTestEnabled)
#endif
    {
        /*
            GL_STENCIL_TEST
            If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
        */

        if (nextStencilTestEnabled)
        {
#ifndef FORCE_STENCIL_TEST
            AFX_ASSERT(!dpu->stencilTestEnabled);
#endif
            gl->Enable(GL_STENCIL_TEST); _ZglThrowErrorOccuried();
        }
        else
        {
#ifndef FORCE_STENCIL_TEST
            AFX_ASSERT(dpu->stencilTestEnabled);
#endif
            gl->Disable(GL_STENCIL_TEST); _ZglThrowErrorOccuried();
        }
        dpu->stencilTestEnabled = nextStencilTestEnabled;
    }

    // STENCIL FUNC

    if (nextStencilTestEnabled)
    {
        /*
            glStencilFuncSeparate — set front and/or back function and reference value for stencil testing
            void glStencilFuncSeparate(	GLenum face, GLenum func, GLint ref, GLuint mask);

            face = Specifies whether front and/or back stencil state is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            func = Specifies the test function. Eight symbolic constants are valid: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, and GL_ALWAYS. The initial value is GL_ALWAYS.
            ref = Specifies the reference value for the stencil test. ref is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer. The initial value is 0.
            mask = Specifies a mask that is ANDed with both the reference value and the stored stencil value when the test is done. The initial value is all 1's.

            Stenciling, like depth-buffering, enables and disables drawing on a per-pixel basis.
            You draw into the stencil planes using GL drawing primitives, then render geometry and images, using the stencil planes to mask out portions of the screen.
            Stenciling is typically used in multipass rendering algorithms to achieve special effects, such as decals, outlining, and constructive solid geometry rendering.

            The stencil test conditionally eliminates a pixel based on the outcome of a comparison between the reference value and the value in the stencil buffer.
            To enable and disable the test, call glEnable and glDisable with argument GL_STENCIL_TEST.
            To specify actions based on the outcome of the stencil test, call glStencilOp or glStencilOpSeparate.

            There can be two separate sets of func, ref, and mask parameters; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilFunc sets both front and back stencil state to the same values, as if glStencilFuncSeparate were called with face set to GL_FRONT_AND_BACK.

            func is a symbolic constant that determines the stencil comparison function. It accepts one of eight values, shown in the following list.
            ref is an integer reference value that is used in the stencil comparison. It is clamped to the range [0, 2^n - 1], where n is the number of bitplanes in the stencil buffer.
            mask is bitwise ANDed with both the reference value and the stored stencil value, with the ANDed values participating in the comparison.

            If stencil represents the value stored in the corresponding stencil buffer location, the following list shows the effect of each comparison function that can be specified by func.
            Only if the comparison succeeds is the pixel passed through to the next stage in the rasterization process (see glStencilOp).
            All tests treat stencil values as unsigned integers in the range [0,2^n - 1], where n is the number of bitplanes in the stencil buffer.

            The following values are accepted by func:

            GL_NEVER Always fails.
            GL_LESS Passes if ( ref & mask ) < ( stencil & mask ).
            GL_LEQUAL Passes if ( ref & mask ) <= ( stencil & mask ).
            GL_GREATER Passes if ( ref & mask ) > ( stencil & mask ).
            GL_GEQUAL Passes if ( ref & mask ) >= ( stencil & mask ).
            GL_EQUAL Passes if ( ref & mask ) = ( stencil & mask ).
            GL_NOTEQUAL Passes if ( ref & mask ) != ( stencil & mask ).
            GL_ALWAYS Always passes.

            Initially, the stencil test is disabled.
            If there is no stencil buffer, no stencil modification can occur and it is as if the stencil test always passes.
        */

        afxUnit32 nextStencilFrontCompareMask = dpu->nextStencilFront.compareMask;
        avxCompareOp nextStencilFrontCompareOp = dpu->nextStencilFront.compareOp;
        afxUnit32 nextStencilFrontRef = dpu->nextStencilFront.reference;

#ifndef FORCE_STENCIL_FUNC
        if ((nextStencilFrontCompareMask != dpu->stencilFront.compareMask) ||
            (nextStencilFrontCompareOp != dpu->stencilFront.compareOp) ||
            (nextStencilFrontRef != dpu->stencilFront.reference))
#endif
        {
            gl->StencilFuncSeparate(GL_FRONT, ZglToGlCompareOp(nextStencilFrontCompareOp), nextStencilFrontRef, nextStencilFrontCompareMask); _ZglThrowErrorOccuried();

            dpu->stencilFront.compareMask = nextStencilFrontCompareMask;
            dpu->stencilFront.compareOp = nextStencilFrontCompareOp;
            dpu->stencilFront.reference = nextStencilFrontRef;
        }

        afxUnit32 nextStencilBackCompareMask = dpu->nextStencilBack.compareMask;
        avxCompareOp nextStencilBackCompareOp = dpu->nextStencilBack.compareOp;
        afxUnit32 nextStencilBackRef = dpu->nextStencilBack.reference;

#ifndef FORCE_STENCIL_FUNC
        if ((nextStencilBackCompareMask != dpu->stencilBack.compareMask) ||
            (nextStencilBackCompareOp != dpu->stencilBack.compareOp) ||
            (nextStencilBackRef != dpu->stencilBack.reference))
#endif
        {
            gl->StencilFuncSeparate(GL_BACK, ZglToGlCompareOp(nextStencilBackCompareOp), nextStencilBackRef, nextStencilBackCompareMask); _ZglThrowErrorOccuried();

            dpu->stencilBack.compareMask = nextStencilBackCompareMask;
            dpu->stencilBack.compareOp = nextStencilBackCompareOp;
            dpu->stencilBack.reference = nextStencilBackRef;
        }
    }
#endif

    // STENCIL WRITE

    {
        /*
            glStencilMaskSeparate — control the front and/or back writing of individual bits in the stencil planes.
            void glStencilMaskSeparate(	GLenum face, GLuint mask);

            face = Specifies whether the front and/or back stencil writemask is updated. Three symbolic constants are valid: GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
            mask = Specifies a bit mask to enable and disable writing of individual bits in the stencil planes. Initially, the mask is all 1's.

            glStencilMaskSeparate controls the writing of individual bits in the stencil planes.
            The least significant n bits of mask, where n is the number of bits in the stencil buffer, specify a mask.
            Where a 1 appears in the mask, it's possible to write to the corresponding bit in the stencil buffer.
            Where a 0 appears, the corresponding bit is write-protected.
            Initially, all bits are enabled for writing.

            There can be two separate mask writemasks; one affects back-facing polygons, and the other affects front-facing polygons as well as other non-polygon primitives.
            glStencilMask sets both front and back stencil writemasks to the same values, as if glStencilMaskSeparate were called with face set to GL_FRONT_AND_BACK.
        */

        static GLenum const faces[] =
        {
            GL_INVALID_ENUM,
            GL_FRONT,
            GL_BACK,
            GL_FRONT_AND_BACK
        };
        AFX_ASSERT(faces[avxCullMode_BACK] == GL_BACK);

        afxMask facesAffected;
        afxUnit32 nextStencilFrontWriteMask = dpu->nextStencilFront.writeMask;
        afxUnit32 nextStencilBackWriteMask = dpu->nextStencilBack.writeMask;

#if 0
        if (dpu->dsDts[1].storeOp != avxStoreOp_STORE)
        {
            // disable stencil writing
            nextStencilFrontWriteMask = 0;
            nextStencilBackWriteMask = 0;
        }
#endif

        if ((nextStencilFrontWriteMask == nextStencilBackWriteMask)
#ifndef FORCE_STENCIL_MASK
            && (nextStencilFrontWriteMask != dpu->stencilFront.writeMask) ||
            (nextStencilBackWriteMask != dpu->stencilBack.writeMask)
#endif
            )
        {
            gl->StencilMaskSeparate(GL_FRONT_AND_BACK, nextStencilFrontWriteMask); _ZglThrowErrorOccuried();
            dpu->stencilFront.writeMask = nextStencilFrontWriteMask;
            dpu->stencilBack.writeMask = nextStencilBackWriteMask;
        }
        else
        {
#ifndef FORCE_STENCIL_MASK
            if (nextStencilFrontWriteMask != dpu->stencilFront.writeMask)
#endif
            {
                gl->StencilMaskSeparate(GL_FRONT, nextStencilFrontWriteMask); _ZglThrowErrorOccuried();
                dpu->stencilFront.writeMask = nextStencilFrontWriteMask;
            }

#ifndef FORCE_STENCIL_MASK
            if (nextStencilBackWriteMask != dpu->stencilBack.writeMask)
#endif
            {
                gl->StencilMaskSeparate(GL_BACK, nextStencilBackWriteMask); _ZglThrowErrorOccuried();
                dpu->stencilBack.writeMask = nextStencilBackWriteMask;
            }
        }
    }
}

_ZGL void _ZglFlushRsChanges(zglDpu* dpu)
{
    afxError err = { 0 };
    glVmt const* gl = dpu->gl;

#ifndef _ZGL_DBG_IGNORE_RASTERIZER_DISCARD

    afxBool nextRasterizationDisabled = dpu->nextRasterizationDisabled;
    if (nextRasterizationDisabled != dpu->rasterizationDisabled)
    {
        /*
            GL_RASTERIZER_DISCARD
            If enabled, primitives are discarded after the optional transform feedback stage, but before rasterization.
            Furthermore, when enabled, glClear, glClearBufferData, glClearBufferSubData, glClearTexImage, and glClearTexSubImage are ignored.
        */

        if (nextRasterizationDisabled)
        {
            gl->Enable(GL_RASTERIZER_DISCARD); _ZglThrowErrorOccuried();
        }
        else
        {
            gl->Disable(GL_RASTERIZER_DISCARD); _ZglThrowErrorOccuried();
        }
        dpu->rasterizationDisabled = nextRasterizationDisabled;
    }
#endif

    avxFillMode nextFillMode = dpu->nextFillMode;
#ifndef FORCE_FILL_MODE
    if (nextFillMode != dpu->fillMode)
#endif
    {
        /*
            glPolygonMode — select a polygon rasterization mode.
            void glPolygonMode(	GLenum face, GLenum mode);

            face = Specifies the polygons that mode applies to. Must be GL_FRONT_AND_BACK for front- and back-facing polygons.
            mode = Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL. The initial value is GL_FILL for both front- and back-facing polygons.

            glPolygonMode controls the interpretation of polygons for rasterization.
            face describes which polygons mode applies to: both front and back-facing polygons (GL_FRONT_AND_BACK).
            The polygon mode affects only the final rasterization of polygons.
            In particular, a polygon's vertices are lit and the polygon is clipped and possibly culled before these modes are applied.

            Three modes are defined and can be specified in mode:

            GL_POINT Polygon vertices that are marked as the start of a boundary edge are drawn as points. Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_LINE Boundary edges of the polygon are drawn as line segments. Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
            GL_FILL The interior of the polygon is filled. Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.

            Vertices are marked as boundary or nonboundary with an edge flag. Edge flags are generated internally by the GL when it decomposes triangle stips and fans.
        */
        gl->PolygonMode(GL_FRONT_AND_BACK, AfxToGlFillMode(nextFillMode)); _ZglThrowErrorOccuried();
        dpu->fillMode = nextFillMode;
    }

    // FLUSH DEPTH/STENCIL CHANGES
    _ZglFlushDsChanges(dpu);

#ifndef _ZGL_DBG_IGNORE_LINE_WIDTH
    if (nextFillMode == avxFillMode_LINE)
    {
        /*
            glLineWidth — specify the width of rasterized lines.
            void glLineWidth(GLfloat width);

            width = Specifies the width of rasterized lines. The initial value is 1.

            glLineWidth specifies the rasterized width of both aliased and antialiased lines.
            Using a line width other than 1 has different effects, depending on whether line antialiasing is enabled.
            To enable and disable line antialiasing, call glEnable and glDisable with argument GL_LINE_SMOOTH.
            Line antialiasing is initially disabled.

            If line antialiasing is disabled, the actual width is determined by rounding the supplied width to the nearest integer.
            (If the rounding results in the value 0, it is as if the line width were 1.) If |delta x|>=|delta y|, i pixels are filled in each column that is rasterized, where i is the rounded value of width.
            Otherwise, i pixels are filled in each row that is rasterized.

            If antialiasing is enabled, line rasterization produces a fragment for each pixel square that intersects the region lying within the rectangle having width equal to the current line width, length equal to the actual length of the line, and centered on the mathematical line segment.
            The coverage value for each fragment is the window coordinate area of the intersection of the rectangular region with the corresponding pixel square.
            This value is saved and used in the final rasterization step.

            Not all widths can be supported when line antialiasing is enabled.
            If an unsupported width is requested, the nearest supported width is used.
            Only width 1 is guaranteed to be supported; others depend on the implementation.
            Likewise, there is a range for aliased line widths as well.
            To query the range of supported widths and the size difference between supported widths within the range, call glGet with arguments GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.

            The line width specified by glLineWidth is always returned when GL_LINE_WIDTH is queried.
            Clamping and rounding for aliased and antialiased lines have no effect on the specified value.

            Nonantialiased line width may be clamped to an implementation-dependent maximum.
            Call glGet with GL_ALIASED_LINE_WIDTH_RANGE to determine the maximum width.

            In OpenGL 1.2, the tokens GL_LINE_WIDTH_RANGE and GL_LINE_WIDTH_GRANULARITY were replaced by GL_ALIASED_LINE_WIDTH_RANGE, GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY.
            The old names are retained for backward compatibility, but should not be used in new code.
        */

        afxReal nextLineWidth = dpu->nextLineWidth;

        if (nextLineWidth != dpu->lineWidth)
        {
            gl->LineWidth(nextLineWidth); _ZglThrowErrorOccuried();
            dpu->lineWidth = nextLineWidth;
        }
    }
#endif

#ifndef _ZGL_DBG_IGNORE_MULTISAMPLING
    afxBool nextMsEnabled = dpu->nextMsEnabled;
    if (nextMsEnabled != dpu->activeRs.msEnabled)
    {
        /*
            GL_MULTISAMPLE
            If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
        */

        if (nextMsEnabled)
        {
            AFX_ASSERT(!dpu->activeRs.msEnabled);
            gl->Enable(GL_MULTISAMPLE); _ZglThrowErrorOccuried();
        }
        else
        {
            AFX_ASSERT(dpu->activeRs.msEnabled);
            gl->Disable(GL_MULTISAMPLE); _ZglThrowErrorOccuried();

        }
        dpu->activeRs.msEnabled = nextMsEnabled;
    }

    if (nextMsEnabled)
    {
        afxUnit nextSampleCnt = dpu->nextSampleCnt;

        if (nextSampleCnt != dpu->activeRs.sampleCnt)
        {
            /*
                GL_SAMPLE_MASK
                If enabled, the sample coverage mask generated for a fragment during rasterization will be ANDed with the value of GL_SAMPLE_MASK_VALUE before shading occurs. See glSampleMaski.
            */

            if (nextSampleCnt)
            {
                AFX_ASSERT(!dpu->activeRs.sampleCnt);
                gl->Enable(GL_SAMPLE_MASK); _ZglThrowErrorOccuried();

                for (afxUnit i = 0; i < nextSampleCnt; i++)
                {
                    dpu->activeRs.sampleMasks[i] = dpu->nextSampleMasks[i];
                    gl->SampleMaski(i, dpu->nextSampleMasks[i]); _ZglThrowErrorOccuried();
                }
            }
            else
            {
                AFX_ASSERT(dpu->activeRs.sampleCnt);
                gl->Disable(GL_SAMPLE_MASK); _ZglThrowErrorOccuried();
            }
            dpu->activeRs.sampleCnt = nextSampleCnt;
        }

        afxReal minSampleShadingValue = dpu->nextMinSampleShadingValue;
        afxBool nextSampleShadingEnabled = dpu->nextSampleShadingEnabled;

        if (nextSampleShadingEnabled != dpu->activeRs.sampleShadingEnabled)
        {
            /*
                GL_SAMPLE_SHADING
                If enabled, the active fragment shader is run once for each covered sample, or at fraction of this rate as determined by the current value of GL_MIN_SAMPLE_SHADING_VALUE. See glMinSampleShading.
            */

            if (nextSampleShadingEnabled)
            {
                AFX_ASSERT(dpu->activeRs.sampleShadingEnabled);
                gl->Enable(GL_SAMPLE_SHADING); _ZglThrowErrorOccuried();
            }
            else
            {
                AFX_ASSERT(!dpu->activeRs.sampleShadingEnabled);
                gl->Disable(GL_SAMPLE_SHADING); _ZglThrowErrorOccuried();
            }

            if (gl->MinSampleShading) // gl v4.0
            {
                gl->MinSampleShading(minSampleShadingValue); _ZglThrowErrorOccuried();
            }

            dpu->activeRs.sampleShadingEnabled = nextSampleShadingEnabled;
            dpu->activeRs.minSampleShadingValue = minSampleShadingValue;
        }

        afxBool nextAlphaToOneEnabled = dpu->nextAlphaToOneEnabled;
        if (nextAlphaToOneEnabled != dpu->activeRs.alphaToOneEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_ONE
                If enabled, each sample alpha value is replaced by the maximum representable alpha value.
            */

            if (nextAlphaToOneEnabled)
            {
                AFX_ASSERT(!dpu->activeRs.alphaToOneEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_ONE); _ZglThrowErrorOccuried();
            }
            else
            {
                AFX_ASSERT(dpu->activeRs.alphaToOneEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_ONE); _ZglThrowErrorOccuried();
            }
            dpu->activeRs.alphaToOneEnabled = nextAlphaToOneEnabled;
        }

        afxBool nextAlphaToCoverageEnabled = dpu->nextAlphaToCoverageEnabled;
        if (nextAlphaToCoverageEnabled != dpu->activeRs.alphaToCoverageEnabled)
        {
            /*
                GL_SAMPLE_ALPHA_TO_COVERAGE
                If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location.
                The temporary coverage value is then ANDed with the fragment coverage value.
            */

            if (nextAlphaToCoverageEnabled)
            {
                AFX_ASSERT(!dpu->activeRs.alphaToCoverageEnabled);
                gl->Enable(GL_SAMPLE_ALPHA_TO_COVERAGE); _ZglThrowErrorOccuried();
            }
            else
            {
                AFX_ASSERT(dpu->activeRs.alphaToCoverageEnabled);
                gl->Disable(GL_SAMPLE_ALPHA_TO_COVERAGE); _ZglThrowErrorOccuried();
            }
            dpu->activeRs.alphaToCoverageEnabled = nextAlphaToCoverageEnabled;
        }
    }
#endif

    // SCISSOR
#ifndef _ZGL_DBG_IGNORE_SCISSOR_TEST
    
    afxUnit32 nextScisUpdMask = dpu->nextScisUpdMask;

#ifndef FORCE_SCISSOR_TEST
    if (nextScisUpdMask)
#endif
    {
        dpu->nextScisUpdMask = NIL;

        afxUnit vpCnt = dpu->vpCnt;
        GLint v[ZGL_MAX_VIEWPORTS][4];

        GLuint first = GL_INVALID_INDEX;
        GLuint cnt = 0;

#if FORCE_GL_GENERIC_FUNCS
        for (afxUnit i = 0; i < vpCnt; i++)
        {
#ifndef FORCE_SCISSOR_TEST
            if (nextScisUpdMask & AFX_BITMASK(i))
#endif
            {
                if (first == GL_INVALID_INDEX)
                    first = i;

                cnt = i + 1 - first;

                dpu->scisRects[i] = dpu->nextScisRects[i];
            }

            v[i][0] = dpu->scisRects[i].x;
            v[i][1] = dpu->scisRects[i].y;
            v[i][2] = dpu->scisRects[i].w;
            v[i][3] = dpu->scisRects[i].h;
        }

        if (cnt)
        {
            //gl->ScissorArrayv(0, cnt, &v[0][0]); _ZglThrowErrorOccuried();
            gl->ScissorArrayv(first, cnt, &v[first][0]); _ZglThrowErrorOccuried();
        }
#else
        for (afxUnit i = 0; i < ZGL_MAX_VIEWPORTS; i++)
        {
            AFX_ASSERT_RANGE(ZGL_MAX_VIEWPORTS, i, 1);

            if (AFX_TEST_BIT_SET(updMask, i))
            {
                v[i][0] = dpu->nextRs.scisRects[i].origin[0],
                v[i][1] = dpu->nextRs.scisRects[i].origin[1],
                v[i][2] = dpu->nextRs.scisRects[i].extent[0],
                v[i][3] = dpu->nextRs.scisRects[i].extent[1];

                if (gl->ScissorArrayv)
                {
                    gl->ScissorArrayv(i, 1, &v[0][0]); _ZglThrowErrorOccuried();
                }
                else
                {
                    GLint x = (GLint)(dpu->nextRs.scisRects[0].origin[0]);
                    GLint y = (GLint)(dpu->nextRs.scisRects[0].origin[1]);
                    GLsizei w = (GLsizei)(dpu->nextRs.scisRects[0].extent[0]);
                    GLsizei h = (GLsizei)(dpu->nextRs.scisRects[0].extent[1]);

                    AFX_ASSERT(i == 0);
                    gl->Scissor(x, y, w, h); _ZglThrowErrorOccuried();
                }
                AfxCopyRect(&(dpu->activeRs.scisRects[i]), &dpu->nextRs.scisRects[i]);
            }
        }
#endif
    }
#endif

    afxBool blendUsed = FALSE;
    afxBool blendConstantsUsed = FALSE;
    afxUnit outCnt = dpu->nextOutCnt;
    dpu->outCnt = outCnt;

    for (afxUnit i = 0; i < outCnt; i++)
    {
        avxColorOutput const* nco = &dpu->nextOuts[i];
        avxColorOutput* co = &dpu->outs[i];
        //AvxGetColorOutputs(ras, 0, outCnt, ch);

#ifndef _ZGL_DBG_IGNORE_BLEND

        if (co->blendEnabled != nco->blendEnabled)
            co->blendEnabled = nco->blendEnabled;

        if (nco->blendEnabled)
        {
            if (!blendUsed)
            {
                blendUsed = TRUE;
                if (!dpu->anyBlendEnabled)
                {
                    /*
                        GL_BLEND
                        If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
                    */
                    gl->Enable(GL_BLEND); _ZglThrowErrorOccuried();
                    dpu->anyBlendEnabled = TRUE;
                }
            }

#ifndef FORCE_BLEND_EQUATION
            if ((nco->blendConfig.rgbBlendOp != co->blendConfig.rgbBlendOp) ||
                (nco->blendConfig.aBlendOp != co->blendConfig.aBlendOp))
#endif
            {
                /*
                    glBlendEquationSeparate — set the RGB blend equation and the alpha blend equation separately.

                    The blend equations determines how a new pixel (the source color) is combined with a pixel already in the framebuffer (the destination color).
                    These functions specify one blend equation for the RGB-color components and one blend equation for the alpha component.
                    glBlendEquationSeparatei specifies the blend equations for a single draw buffer whereas glBlendEquationSeparate sets the blend equations for all draw buffers.

                    The blend equations use the source and destination blend factors specified by either glBlendFunc or glBlendFuncSeparate.
                    See glBlendFunc or glBlendFuncSeparate for a description of the various blend factors.
                */
                gl->BlendEquationSeparatei(i,   AfxToGlBlendOp(nco->blendConfig.rgbBlendOp),
                                                AfxToGlBlendOp(nco->blendConfig.aBlendOp)); _ZglThrowErrorOccuried();

                co->blendConfig.rgbBlendOp = nco->blendConfig.rgbBlendOp;
                co->blendConfig.aBlendOp = nco->blendConfig.aBlendOp;
            }
#ifndef FORCE_BLEND_FUNC
            if ((nco->blendConfig.rgbSrcFactor != co->blendConfig.rgbSrcFactor) ||
                (nco->blendConfig.rgbDstFactor != co->blendConfig.rgbDstFactor) ||
                (nco->blendConfig.aSrcFactor != co->blendConfig.aSrcFactor) ||
                (nco->blendConfig.aDstFactor != co->blendConfig.aDstFactor))
#endif
            {
                /*
                    glBlendFuncSeparate — specify pixel arithmetic for RGB and alpha components separately.

                    Pixels can be drawn using a function that blends the incoming (source) RGBA values with the RGBA values that are already in the frame buffer (the destination values).
                    Blending is initially disabled. Use glEnable and glDisable with argument GL_BLEND to enable and disable blending.

                    glBlendFuncSeparate defines the operation of blending for all draw buffers when it is enabled.
                    glBlendFuncSeparatei defines the operation of blending for a single draw buffer specified by buf when enabled for that draw buffer.
                    srcRGB specifies which method is used to scale the source RGB-color components. dstRGB specifies which method is used to scale the destination RGB-color components.
                    Likewise, srcAlpha specifies which method is used to scale the source alpha color component, and dstAlpha specifies which method is used to scale the destination alpha component.
                    The possible methods are described in the following table. Each method defines four scale factors, one each for red, green, blue, and alpha.
                */
                gl->BlendFuncSeparatei(i,   AfxToGlBlendFactor(nco->blendConfig.rgbSrcFactor),
                                            AfxToGlBlendFactor(nco->blendConfig.rgbDstFactor),
                                            AfxToGlBlendFactor(nco->blendConfig.aSrcFactor),
                                            AfxToGlBlendFactor(nco->blendConfig.aDstFactor)); _ZglThrowErrorOccuried();

                co->blendConfig.rgbSrcFactor = nco->blendConfig.rgbSrcFactor;
                co->blendConfig.rgbDstFactor = nco->blendConfig.rgbDstFactor;
                co->blendConfig.aSrcFactor = nco->blendConfig.aSrcFactor;
                co->blendConfig.aDstFactor = nco->blendConfig.aDstFactor;
            }

            // do it at pipeline binding?
            if (!blendConstantsUsed)
            {
                if ((nco->blendConfig.aDstFactor == avxBlendFactor_CONST_A) ||
                    (nco->blendConfig.aSrcFactor == avxBlendFactor_CONST_A) ||
                    (nco->blendConfig.rgbDstFactor == avxBlendFactor_CONST_RGB) ||
                    (nco->blendConfig.rgbSrcFactor == avxBlendFactor_CONST_RGB) ||
                    (nco->blendConfig.aDstFactor == avxBlendFactor_ONE_MINUS_CONST_A) ||
                    (nco->blendConfig.aSrcFactor == avxBlendFactor_ONE_MINUS_CONST_A) ||
                    (nco->blendConfig.rgbDstFactor == avxBlendFactor_ONE_MINUS_CONST_RGB) ||
                    (nco->blendConfig.rgbSrcFactor == avxBlendFactor_ONE_MINUS_CONST_RGB))
                {
                    blendConstantsUsed = TRUE;
                }
            }
        }
#endif

#ifndef _ZGL_DBG_IGNORE_COLOR_MASK

        // The color write mask operation is applied regardless of whether blending is enabled.
        
        // The color write mask operation is applied only if Color Write Enable is enabled for the respective attachment. 
        // Otherwise the color write mask is ignored and writes to all components of the attachment are disabled.
#ifndef FORCE_COLOR_WRITE_MASK
        if (nco->discardMask != co->discardMask)
#endif
        {
#if 0
            if (dpu->activeRs.colDts[i].storeOp == avxStoreOp_STORE)
#endif
            {
                /*
                    glColorMask, glColorMaski — enable and disable writing of frame buffer color components.

                    glColorMask and glColorMaski specify whether the individual color components in the frame buffer can or cannot be written.
                    glColorMaski sets the mask for a specific draw buffer, whereas glColorMask sets the mask for all draw buffers.
                    If red is GL_FALSE, for example, no change is made to the red component of any pixel in any of the color buffers, regardless of the drawing operation attempted.

                    Changes to individual bits of components cannot be controlled. Rather, changes are either enabled or disabled for entire color components.
                */
                gl->ColorMaski(i,   (nco->discardMask & avxColorMask_R) != avxColorMask_R,
                                    (nco->discardMask & avxColorMask_G) != avxColorMask_G,
                                    (nco->discardMask & avxColorMask_B) != avxColorMask_B,
                                    (nco->discardMask & avxColorMask_A) != avxColorMask_A); _ZglThrowErrorOccuried();
                co->discardMask = nco->discardMask;
            }
        }
#endif
    }

#ifndef _ZGL_DBG_IGNORE_BLEND
    if ((!blendUsed) && dpu->anyBlendEnabled)
    {
        gl->Disable(GL_BLEND); _ZglThrowErrorOccuried();
        dpu->anyBlendEnabled = FALSE;
    }

    if (blendUsed && blendConstantsUsed)
    {
        /*
            glBlendColor — set the blend color.

            The GL_BLEND_COLOR may be used to calculate the source and destination blending factors. The color components are clamped to the range [0,1] before being stored.
            See glBlendFunc for a complete description of the blending operations. Initially the GL_BLEND_COLOR is set to (0, 0, 0, 0).
        */

        AvxCopyColor(dpu->blendConstants, dpu->nextBlendConstants);
        gl->BlendColor(dpu->nextBlendConstants[0],
            dpu->nextBlendConstants[1],
            dpu->nextBlendConstants[2],
            dpu->nextBlendConstants[3]); _ZglThrowErrorOccuried();
    }
#endif

#ifndef _ZGL_DBG_IGNORE_LOGICAL_OP

    avxLogicOp nextLogicOp = dpu->nextLogicOp;
    afxBool nextLogicOpEnabled = dpu->nextLogicOpEnabled;

#ifndef FORCE_LOGIC_OP
    if (nextLogicOpEnabled != dpu->logicOpEnabled)
#endif
    {
        if (nextLogicOpEnabled)
        {
#ifndef FORCE_LOGIC_OP
            AFX_ASSERT(!dpu->logicOpEnabled);
#endif
            gl->Enable(GL_COLOR_LOGIC_OP); _ZglThrowErrorOccuried();
        }
        else
        {
#ifndef FORCE_LOGIC_OP
            AFX_ASSERT(dpu->logicOpEnabled);
#endif
            gl->Disable(GL_COLOR_LOGIC_OP); _ZglThrowErrorOccuried();
        }
        dpu->logicOpEnabled = nextLogicOpEnabled;
    }

    if (nextLogicOpEnabled)
    {
#ifndef FORCE_LOGIC_OP
        if (nextLogicOp != dpu->logicOp)
#endif
        {
            /*
                glLogicOp — specify a logical pixel operation for rendering.

                glLogicOp specifies a logical operation that, when enabled, is applied between the incoming RGBA color and the RGBA color at the corresponding location in the frame buffer.
                To enable or disable the logical operation, call glEnable and glDisable using the symbolic constant GL_COLOR_LOGIC_OP. The initial value is disabled.

                Th opcode is a symbolic constant. The logical operation is applied independently to each bit pair of the source and destination colors.

                When more than one RGBA color buffer is enabled for drawing, logical operations are performed separately for each enabled buffer, using for the destination value the contents of that buffer (see glDrawBuffer).

                Logic operations have no effect on floating point draw buffers.
                However, if GL_COLOR_LOGIC_OP is enabled, blending is still disabled in this case.
            */
            gl->LogicOp(AfxToGlLogicOp(nextLogicOp)); _ZglThrowErrorOccuried();
            dpu->logicOp = nextLogicOp;
        }
    }
#endif
}
