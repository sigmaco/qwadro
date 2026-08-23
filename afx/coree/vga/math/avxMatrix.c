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
 *                               (c) 2017 SIGMA FEDERATION
 *                               ESTADO-MAJOR DA SECURIDAD
 *                                SIGMA TECHNOLOGY GROUP
 *                                       ENGITECH
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>
// This software is part of Advanced Video Graphics Extensions.

//#include "qwadro/vga/avxIcd.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/vga/math/avxMatrix.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/math/afxArithmetic2.h"
#include "qwadro/math/afxArithmetic.h"
#include "qwadro/math/afxTrigonometry.h"

#if 0
avxClipSpace const  AVX_CLIP_SPACE_QWADRO =
{
    .nonRhcs = FALSE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_OPENGL =
{
    .nonRhcs = FALSE,
    .boundary = avxClipSpaceDepth_NEG_ONE_TO_ONE,
    .boundMin = {-1,-1,-1 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_VULKAN =
{
    .nonRhcs = FALSE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};

avxClipSpace const  AVX_CLIP_SPACE_D3D =
{
    .nonRhcs = TRUE,
    .boundary = avxClipSpaceDepth_ZERO_TO_ONE,
    .boundMin = {-1,-1, 0 },
    .boundMax = { 1, 1, 1 }
};
#endif

////////////////////////////////////////////////////////////////////////////////
// Projective transformation matrix operations                                //
////////////////////////////////////////////////////////////////////////////////

_AVXINL afxM4d AfxComputeShadowM4d(afxPlane const p, afxV3d const lightPos)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxV3dIsZero(p.uvwd.v3));
    AFX_ASSERT(!AfxV3dIsInfinite(p.uvwd.v3));

    // plane must be normalized
    afxReal d = AfxFindPlaneDistance(p, lightPos);
    afxM4d m;
#if !0
    m.m[0][0] = d - lightPos.v[0] * p.uvwd.v[0];
    m.m[1][0] = -lightPos.v[0] * p.uvwd.v[1];
    m.m[2][0] = -lightPos.v[0] * p.uvwd.v[2];
    m.m[3][0] = -lightPos.v[0] * p.uvwd.v[AFX_PLANE_DIST];

    m.m[0][1] = -lightPos.v[1] * p.uvwd.v[0];
    m.m[1][1] = d - lightPos.v[1] * p.uvwd.v[1];
    m.m[2][1] = -lightPos.v[1] * p.uvwd.v[2];
    m.m[3][1] = -lightPos.v[1] * p.uvwd.v[AFX_PLANE_DIST];

    m.m[0][2] = -lightPos.v[2] * p.uvwd.v[0];
    m.m[1][2] = -lightPos.v[2] * p.uvwd.v[1];
    m.m[2][2] = d - lightPos.v[2] * p.uvwd.v[2];
    m.m[3][2] = -lightPos.v[2] * p.uvwd.v[AFX_PLANE_DIST];

    m.m[0][3] = -p.uvwd.v[0];
    m.m[1][3] = -p.uvwd.v[1];
    m.m[2][3] = -p.uvwd.v[2];
    m.m[3][3] = d - p.uvwd.v[AFX_PLANE_DIST];
#else
    m.m[0][0] = d - lightPos[0] * p->v[0];
    m.m[0][1] =    -lightPos[1] * p->v[0];
    m.m[0][2] =    -lightPos[2] * p->v[0];
    m.m[0][3] =    -p->v[0];
    m.m[1][0] =    -lightPos[0] * p->v[1];
    m.m[1][1] = d - lightPos[1] * p->v[1];
    m.m[1][2] =    -lightPos[2] * p->v[1];
    m.m[1][3] =    -p->v[1];
    m.m[2][0] =    -lightPos[0] * p->v[2];
    m.m[2][1] =    -lightPos[1] * p->v[2];
    m.m[2][2] = d - lightPos[2] * p->v[2];
    m.m[2][3] =    -p->v[2];
    m.m[3][0] =    -lightPos[0] * p->offset;
    m.m[3][1] =    -lightPos[1] * p->offset;
    m.m[3][2] =    -lightPos[2] * p->offset;
    m.m[3][3] = d - p->offset;
#endif
    return m;
}

_AVXINL afxM4d AfxCubemapMatrix_Direct3D(afxUnit face)
{
    afxError err = { 0 };

    afxM4d m = AfxM4dZero();

    switch (face)
    {
    case 0: // +X
        m.m[0][2] = 1;
        m.m[1][1] = 1;
        m.m[2][0] =-1;
        m.m[3][3] = 1;
        break;
    case 1: // -X
        m.m[0][2] =-1;
        m.m[1][1] = 1;
        m.m[2][0] = 1;
        m.m[3][3] = 1;
        break;
    case 2: // +Y
        m.m[0][0] = 1;
        m.m[2][1] =-1;
        m.m[1][2] = 1;
        m.m[3][3] = 1;
        break;
    case 3: // -Y
        m.m[0][0] = 1;
        m.m[2][1] = 1;
        m.m[1][2] =-1;
        m.m[3][3] = 1;
        break;
    case 4: // +Z
        m.m[0][0] = 1;
        m.m[1][1] = 1;
        m.m[2][2] = 1;
        m.m[3][3] = 1;
        break;
    case 5: // -Z
        m.m[0][0] =-1;
        m.m[1][1] = 1;
        m.m[2][2] =-1;
        m.m[3][3] = 1;
        break;
    };
    return m;
}

_AVXINL afxM4d AfxCubemapMatrix_OpenGL(afxUnit face)
{
    afxError err = { 0 };
    
    afxM4d m = AfxM4dZero();

    switch (face)
    {
    case 0: // +X
        m.m[2][0] = 1;
        m.m[1][1] = 1;
        m.m[0][2] =-1;
        m.m[3][3] = 1;
        break;
    case 1: // -X
        m.m[2][0] =-1;
        m.m[1][1] = 1;
        m.m[0][2] = 1;
        m.m[3][3] = 1;
        break;
    case 2: // +Y
        m.m[0][0] =-1;
        m.m[2][1] =-1;
        m.m[1][2] =-1;
        m.m[3][3] = 1;
        break;
    case 3: // -Y
        m.m[0][0] =-1;
        m.m[2][1] = 1;
        m.m[1][2] = 1;
        m.m[3][3] = 1;
        break;
    case 4: // +Z
        m.m[0][0] =-1;
        m.m[1][1] = 1;
        m.m[2][2] =-1;
        m.m[3][3] = 1;
        break;
    case 5: // -Z
        m.m[0][0] = 1;
        m.m[1][1] = 1;
        m.m[2][2] = 1;
        m.m[3][3] = 1;
        break;
    }
    return m;
}

_AVXINL afxM4d AfxComputeLookToMatrix(afxV3d const eye, afxV3d const dir, afxV3d const up, afxBool nonRhcs)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxV3dIsZero(dir));
    AFX_ASSERT(!AfxV3dIsInfinite(dir));
    AFX_ASSERT(!AfxV3dIsZero(up));
    AFX_ASSERT(!AfxV3dIsInfinite(up));

    if (nonRhcs) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)

        afxV3d z = AfxV3dNormalize(dir, NIL);
        afxV3d x = AfxV3dCross(up, z);
        x = AfxV3dNormalize(x, NIL);
        afxV3d y = AfxV3dCross(z, x);

        afxV3d negEye = AfxV3dNeg(eye);

        afxV4d const w = AFX_V4D(AfxV3dDot(x, negEye), AfxV3dDot(y, negEye), AfxV3dDot(z, negEye), (afxReal)1);
        return AfxM4dMakeAtm(x, y, z, w.v3);
    }
    
    // OpenGL/Vulkan/Qwadro
    // Should be compatible with glLookTo

    afxV3d target = AfxV3dAdd(eye, dir);
    return AfxComputeLookAtMatrix(eye, target, up, FALSE);
}

_AVXINL afxM4d AfxComputeLookAtMatrix(afxV3d const eye, afxV3d const target, afxV3d const up, afxBool nonRhcs)
{
    afxError err = { 0 };

    if (nonRhcs) // Direct3D
    {
        // Should be compatible with XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)

        afxV3d dir = AfxV3dSub(target, eye);
        return AfxComputeLookToMatrix(eye, dir, up, nonRhcs);
    }
    
    // OpenGL/Vulkan/Qwadro
    // Should be compatible with glLookAt

    afxV3d dir = AfxV3dSub(eye, target);
    afxM4d m = AfxComputeLookToMatrix(eye, dir, up, nonRhcs);

    afxV3d f = AfxV3dSub(target, eye);
    f = AfxV3dNormalize(f, NIL);
    afxV3d s = AfxV3dCross(f, up);
    afxV3d u = AfxV3dCross(s, f);
    f = AfxV3dNeg(f);
    afxV4d w = AFX_V4D(-AfxV3dDot(s, eye), -AfxV3dDot(u, eye), AfxV3dDot(f, eye), 1.f );
    return AfxM4dMakeAtm(s, u, f, w.v3);
}

// ORTHOGRAPHIC ////////////////////////////////////////////////////////////////

_AVXINL afxM4d AfxComputeOrthographicMatrix(afxV2d const extent, afxReal near, afxReal far, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxRealIsEqual(extent.v[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent.v[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    
    afxM4d m = AFX_M4D_IDENTITY;

    if (nonRhcs)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal range = 1.f / (far - near);
        m.m[0][0] = 2.f / extent.v[0];
        m.m[1][1] = 2.f / extent.v[1];
        m.m[2][2] = range;
        m.m[3][2] = -range * near;
        return m;
    }

    if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
    {
        // Should be compatible with XMMATRIX XMMatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal range = 1.f / (near - far);
        m.m[0][0] = 2.f / extent.v[0];
        m.m[1][1] = 2.f / extent.v[1];
        m.m[2][2] = range;
        m.m[3][2] = range * near;
        return m;
    }
    
    if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
    {
        return AfxComputeBasicOrthographicMatrix(extent.v[0] / extent.v[1], 1, far, nonRhcs, clip);
    }
    
    AfxThrowError();
    return m;
}

_AVXINL afxM4d AfxComputeOffcenterOrthographicMatrix(afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));

    afxM4d m = AFX_M4D_IDENTITY;

    if (nonRhcs)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = 1.f / (far - near); // [ 0, 1 ]
        m.m[0][0] = 2.f * rw;
        m.m[1][1] = 2.f * rh;
        m.m[2][2] = range;
        m.m[3][0] = -(left + right) * rw;
        m.m[3][1] = -(top + bottom) * rh;
        m.m[3][2] = -range * near;
        return m;
    }

    if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
    {
        // Should be compatible with XMMATRIX XM_CALLCONV XMMatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = 1.f / (near - far); // [ 0, 1 ]
        m.m[0][0] = 2.f * rw;
        m.m[1][1] = 2.f * rh;
        m.m[2][2] = range;
        m.m[3][0] = -(left + right) * rw;
        m.m[3][1] = -(top + bottom) * rh;
        m.m[3][2] = range * near;
        return m;
    }
    
    if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
    {
        // Should be compatible with glOrtho
            
        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = -1.f / (far - near); // [ -1, 1 ]
        m.m[0][0] = 2.f * rw;
        m.m[1][1] = 2.f * rh;
        m.m[2][2] = 2.f * range;
        m.m[3][0] = -(right + left) * rw;
        m.m[3][1] = -(top + bottom) * rh;
        m.m[3][2] = (far + near) * range;
        return m;
    }

    AfxThrowError();
    return m;
}

_AVXINL afxM4d AfxComputeBoundingOrthographicMatrix(afxBox const aabb, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    // Computa uma afxM4d de projeção ortográfica desde uma afxBox.
    afxError err = { 0 };

    //AFX_ASSERT(aabb); // afxBox deve estar no view space.

    // OpenGL
    afxM4d m = AfxComputeOffcenterOrthographicMatrix(aabb.max.v[0], aabb.min.v[0], aabb.max.v[1], aabb.min.v[1], -aabb.min.v[2], -aabb.max.v[2], nonRhcs, clip);
    return m;
}

_AVXINL afxM4d AfxComputeBasicOrthographicMatrix(afxReal aspectRatio, afxReal scale, afxReal range, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    // Computa uma afxM4d de projeção ortográfica genérica.
    afxError err = { 0 };
    AFX_ASSERT(aspectRatio); // w/h
    
    // OpenGL

    if (aspectRatio >= 1.f)
    {
        return AfxComputeOffcenterOrthographicMatrix(-scale * aspectRatio, scale * aspectRatio, -scale, scale, -scale - range, scale + range, nonRhcs, clip);
    }

    aspectRatio = 1.f / aspectRatio;
    return AfxComputeOffcenterOrthographicMatrix(-scale, scale, -scale / aspectRatio, scale / aspectRatio, -scale - range, scale + range, nonRhcs, clip);
}

// PERSPECTIVE /////////////////////////////////////////////////////////////////

_AVXINL afxM4d AfxComputePerspectiveMatrix(afxV2d const extent, afxReal near, afxReal far, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxRealIsEqual(extent.v[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent.v[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    
    afxM4d m = AFX_M4D_IDENTITY;

    if (nonRhcs)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
        
        afxReal near2 = 2.f * near;
        afxReal range = far / (far - near);
        m.m[0][0] = near2 / extent.v[0];
        m.m[1][1] = near2 / extent.v[1];
        m.m[2][2] = range;
        m.m[2][3] = 1.f;
        m.m[3][2] = -range * near;
        return m;
    }

    if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
    {
        // Should be compatible with XMMATRIX XMMatrixPerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)

        afxReal near2 = 2.f * near;
        afxReal range = far / (near - far);
        m.m[0][0] = near2 / extent.v[0];
        m.m[1][1] = near2 / extent.v[1];
        m.m[2][2] = range;
        m.m[2][3] = -1.f;
        m.m[3][2] = range * near;
        return m;
    }
    
    if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
    {
        AfxThrowError();
    }

    AfxThrowError();
    return m;
}

_AVXINL afxM4d AfxComputeFovPerspectiveMatrix(afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    
    afxM4d m = AFX_M4D_IDENTITY;

    if (nonRhcs)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

        afxReal h = AfxCosf(0.5f * fovY) / AfxSinf(0.5f * fovY);
        afxReal w = h / aspectRatio;
        afxReal range = far / (far - near);
        m.m[0][0] = w;
        m.m[1][1] = h;
        m.m[2][2] = range;
        m.m[2][3] = 1.f;
        m.m[3][2] = -range * near;
        return m;
    }

    if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
    {
        // Should be compatible with XMMATRIX XMMatrixPerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)

        afxReal h = AfxCosf(0.5f * fovY) / AfxSinf(0.5f * fovY);
        afxReal w = h / aspectRatio;
        afxReal range = far / (near - far);
        m.m[0][0] = w;
        m.m[1][1] = h;
        m.m[2][2] = range;
        m.m[2][3] = -1.f;
        m.m[3][2] = range * near;
        return m;
    }
    
    if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
    {
        // Should be compatible with glPerspective

        afxReal f = 1.f / AfxTanf(fovY * 0.5f);
        afxReal range = 1.f / (near - far);
        m.m[0][0] = f / aspectRatio;
        m.m[1][1] = f;
        m.m[2][2] = (near + far) * range;
        m.m[2][3] = -1.f;
        m.m[3][2] = 2.f * near * far * range;
        return m;
    }

    AfxThrowError();
    return m;
}

_AVXINL afxM4d AfxComputeOffcenterPerspectiveMatrix(afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    
    afxM4d m = AFX_M4D_ZERO;

    if (nonRhcs)
    {
        AFX_ASSERT(clip == avxClipSpaceDepth_ZERO_TO_ONE); // Direct3D

        // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal near2 = 2.f * near;
        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = far / (far - near);
        m.m[0][0] = near2 * rw;
        m.m[1][1] = near2 * rh;
        m.m[2][0] = -(left + right) * rw;
        m.m[2][1] = -(top + bottom) * rh;
        m.m[2][2] = range;
        m.m[2][3] = 1.f;
        m.m[3][2] = -range * near;
        return m;
    }

    if (clip == avxClipSpaceDepth_ZERO_TO_ONE) // Vulkan
    {
        // Should be compatible with XMMATRIX XMMatrixPerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)

        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = far / (near - far);
        afxReal near2 = 2.f * near;
        m.m[0][0] = near2 * rw;
        m.m[1][1] = near2 * rh;
        m.m[2][0] = (left + right) * rw;
        m.m[2][1] = (top + bottom) * rh;
        m.m[2][2] = range;
        m.m[2][3] = -1.f;
        m.m[3][2] = range * near;
        return m;
    }
    
    if (clip == avxClipSpaceDepth_NEG_ONE_TO_ONE) // OpenGL
    {
        // Preprojective matrix
        // Should be compatible with glFrustum

        afxReal rw = 1.f / (right - left);
        afxReal rh = 1.f / (top - bottom);
        afxReal range = -1.f / (far - near);
        afxReal near2 = 2.f * near;
        m.m[0][0] = near2 * rw;
        m.m[1][1] = near2 * rh;
        m.m[2][0] = (right + left) * rw;
        m.m[2][1] = (top + bottom) * rh;
        m.m[2][2] = (far + near) * range;
        m.m[2][3] = -1.f;
        m.m[3][2] = far * near2 * range;
        return m;
    }
    
    AfxThrowError();
    return m;
}

_AVXINL afxM4d AfxComputeBasicPerspectiveMatrix(afxReal aspectRatio, afxReal range, afxBool nonRhcs, avxClipSpaceDepth clip)
{
    afxError err = { 0 };
    return AfxComputeFovPerspectiveMatrix(AFX_PI / 4.f, aspectRatio, 0.01f, range, nonRhcs, clip);
}

// MakeView
// Memory layout:

_AVXINL afxM4d AfxComputeRenderWareViewM4d(afxM4d const cam)
{
    afxError err = { 0 };

    afxM4d m;
    afxM4d inv = AfxM4dInvert(cam, NIL);
    m.m[0][0] = -inv.m[0][0];
    m.m[0][1] = inv.m[0][1];
    m.m[0][2] = inv.m[0][2];
    m.m[0][3] = 0.0f;
    m.m[1][0] = -inv.m[1][0];
    m.m[1][1] = inv.m[1][1];
    m.m[1][2] = inv.m[1][2];
    m.m[1][3] = 0.0f;
    m.m[2][0] = -inv.m[2][0];
    m.m[2][1] = inv.m[2][1];
    m.m[2][2] = inv.m[2][2];
    m.m[2][3] = 0.0f;
    m.m[3][0] = -inv.m[3][0];
    m.m[3][1] = inv.m[3][1];
    m.m[3][2] = inv.m[3][2];
    m.m[3][3] = 1.0f;
    return m;
}

// MakeProjection
// Memory layout:

_AVXINL afxM4d AfxComputeRenderWareProjectionM4d(avxViewport const vp, afxBool perspective)
{
    afxError err = { 0 };
    AFX_ASSERT(vp.maxDepth);
    AFX_ASSERT(vp.minDepth < vp.maxDepth);
    
    afxReal rw = 1.f / vp.extent.v[0];
    afxReal rh = 1.f / vp.extent.v[1];
    afxReal rd = 1.f / (vp.maxDepth - vp.minDepth);

    afxM4d m;
    m.m[0][0] = rw;
    m.m[0][1] = 0.f;
    m.m[0][2] = 0.f;
    m.m[0][3] = 0.f;

    m.m[1][0] = 0.f;
    m.m[1][1] = rh;
    m.m[1][2] = 0.f;
    m.m[1][3] = 0.f;

    m.m[2][0] = vp.origin.v[0] * rw;
    m.m[2][1] = vp.origin.v[1] * rh;
    m.m[3][0] = -m.m[2][0];
    m.m[3][1] = -m.m[2][1];

    if (perspective)
    {
        m.m[2][2] = (vp.maxDepth + vp.minDepth) * rd;
        m.m[2][3] = 1.f;

        m.m[3][2] = -2.f * vp.minDepth * vp.maxDepth * rd;
        m.m[3][3] = 0.f;
    }
    else
    {
        m.m[2][2] = 2.f * rd;
        m.m[2][3] = 0.f;

        m.m[3][2] = -(vp.maxDepth + vp.minDepth) * rd;
        m.m[3][3] = 1.f;
    }
    return m;
}

// DecomposePerspective

_AVXINL void AfxDecomposePerspectiveM4d(afxM4d const m, afxV2d const extent, afxReal* near, afxReal* far)
{
    afxError err = { 0 };

    // OpenGL
    AfxThrowError();
    *near = m.m[3][2] / (m.m[2][2] - 1.f);
    *far = m.m[3][2] / (m.m[2][2] + 1.f);
}

_AVXINL void AfxDecomposeFovPerspectiveM4d(afxM4d m, afxReal* fovY, afxReal* aspectRatio, afxReal* near, afxReal* far)
{
    afxError err = { 0 };

    // OpenGL
    *fovY = 2.f * AfxAtanf(1.f / m.m[1][1]);
    *aspectRatio = m.m[1][1] / m.m[0][0];
    *near = m.m[3][2] / (m.m[2][2] - 1.f);
    *far = m.m[3][2] / (m.m[2][2] + 1.f);
}

_AVXINL void AfxDecomposeOffcenterPerspectiveM4d(afxM4d m, afxReal* left, afxReal* right, afxReal* top, afxReal* bottom, afxReal* near, afxReal* far)
{
    afxError err = { 0 };

    // OpenGL
    afxReal n = m.m[3][2] / (m.m[2][2] - 1.f);
    afxReal f = m.m[3][2] / (m.m[2][2] + 1.f);
    *left = (n / m.m[0][0]) * (m.m[2][0] - 1.f);
    *right = (n / m.m[0][0]) * (m.m[2][0] + 1.f);
    *top = (n / m.m[1][1]) * (m.m[2][1] + 1.f);
    *bottom = (n / m.m[1][1]) * (m.m[2][1] - 1.f);
    *near = n;
    *far = f;
}

_AVXINL void AvxDoNdcCoordinates(afxUnit cnt, afxV4d const clipPos[], afxV4d ndcPos[], afxUnit srcStride, afxUnit dstStride)
{
    afxError err = { 0 };
    AFX_ASSERT_ALIGNMENT(srcStride, sizeof(afxReal));
    AFX_ASSERT(!srcStride || (srcStride >= sizeof(afxV3d)));
    AFX_ASSERT_ALIGNMENT(dstStride, sizeof(afxReal));
    AFX_ASSERT(!dstStride || (dstStride >= sizeof(afxV3d)));
    srcStride = AFX_OR(srcStride, sizeof(afxV3d));
    dstStride = AFX_OR(dstStride, sizeof(afxV3d));
    afxUnit srcStep = srcStride / sizeof(afxReal);
    afxUnit dstStep = dstStride / sizeof(afxReal);
    afxReal const* clipPos2 = &clipPos[0].v[0];
    afxReal* ndcPos2 = &ndcPos[0].v[0];

    if ((clipPos == ndcPos) && (dstStep == srcStep))
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit srcIdx = i * srcStep;
            afxReal w = clipPos2[srcIdx + 3];
            afxReal wr = 1.f / w;
            ndcPos2[srcIdx + 0] *= wr;
            ndcPos2[srcIdx + 1] *= wr;
            ndcPos2[srcIdx + 2] *= wr;
            // w_ndc is always 1.0. The purpose of this step is to flatten from 4D homogeneous space to 3D normalized space.
            ndcPos2[srcIdx + 3] = 1.0;
        }
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            afxUnit dstIdx = i * dstStep;
            afxUnit srcIdx = i * srcStep;
            afxReal w = clipPos2[srcIdx + 3];
            afxReal wr = 1.f / w;
            ndcPos2[dstIdx + 0] = clipPos2[srcIdx + 0] * wr;
            ndcPos2[dstIdx + 1] = clipPos2[srcIdx + 1] * wr;
            ndcPos2[dstIdx + 2] = clipPos2[srcIdx + 2] * wr;
            // w_ndc is always 1.0. The purpose of this step is to flatten from 4D homogeneous space to 3D normalized space.
            ndcPos2[dstIdx + 3] = 1.0;
        }
    }
}
