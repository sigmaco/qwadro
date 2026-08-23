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

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#define _ARX_CAMERA_C
#include "arxIcd.h"
#include "qwadro/math/afxTrigonometry.h"
#include "qwadro/math/afxArithmetic2.h"

_ARX afxReal64 ArxFindAllowedLodErrorForCamera(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam)
{

    afxReal64 halfFov = fovY * 0.5;
#if 0
    // Mathematically identical but it does use tan().
    afxReal64 pixelsToNdc = errInPixels / (vpHeightInPixels * 0.5);
    afxReal64 tanHalfFov = AfxTan(halfFov);
    return tanHalfFov * pixelsToNdc * distanceFromCam;
#else
    return AfxSin(halfFov) / AfxCos(halfFov) * errInPixels * distanceFromCam / ((afxReal64)vpHeightInPixels * 0.5);
#endif
}

////////////////////////////////////////////////////////////////////////////////

_ARXINL void ArxAdjustCameraAspectRatio(arxCamera cam, afxReal physAspectRatio, afxV2d const screenExtent, afxV2d const windowExtent)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(screenExtent);
    //AFX_ASSERT(windowExtent);
    cam->wpOverHp = physAspectRatio ? physAspectRatio : 1;
    cam->wrOverHr = AFX_MAX(1, screenExtent.v[0]) / AFX_MAX(1, screenExtent.v[1]);
    cam->wwOverHw = AFX_MAX(1, windowExtent.v[0]) / AFX_MAX(1, windowExtent.v[1]);
    cam->shouldSyncP = TRUE;
}

_ARXINL void _ArxComputeCameraMatrices(arxCamera cam, afxM4d* pV, afxM4d* pIv)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(v);
    //AFX_ASSERT(iv);

    afxM3d a, b, c;

    if (!cam->useQuatOrient)
        a = cam->orientM3d;
    else
        a = AfxM3dRotationQuat(cam->orient);

    afxV3d cosv = AfxV3dCos(cam->elevAzimRoll);
    afxV3d sinv = AfxV3dSin(cam->elevAzimRoll);

    b.m[0][0] = cosv.v[1];
    b.m[0][1] = 0.f;
    b.m[0][2] = sinv.v[1];
    b.m[1][0] = 0.f;
    b.m[1][1] = 1.f;
    b.m[1][2] = 0.f;
    b.m[2][0] = -sinv.v[1];
    b.m[2][1] = 0.f;
    b.m[2][2] = cosv.v[1];
    c = AfxM3dMultiply(a, b);
    b.m[0][0] = 1.f;
    b.m[0][1] = 0.f;
    b.m[0][2] = 0.f;
    b.m[1][0] = 0.f;
    b.m[1][1] = cosv.v[0];
    b.m[1][2] = -sinv.v[0];
    b.m[2][0] = 0.f;
    b.m[2][1] = sinv.v[0];
    b.m[2][2] = cosv.v[0];
    a = AfxM3dMultiply(c, b);
    b.m[0][0] = cosv.v[2];
    b.m[0][1] = -sinv.v[2];
    b.m[0][2] = 0.f;
    b.m[1][0] = sinv.v[2];
    b.m[1][1] = cosv.v[2];
    b.m[1][2] = 0.f;
    b.m[2][0] = 0.f;
    b.m[2][1] = 0.f;
    b.m[2][2] = 1.f;
    c = AfxM3dMultiply(a, b);

    afxV4d at;
    at.v3 = AfxV3dPreMultiplyM3d(cam->pos, c);
    at.v[0] = -(at.v[0] + cam->displace.v[0]);
    at.v[1] = -(at.v[1] + cam->displace.v[1]);
    at.v[2] = -(at.v[2] + cam->displace.v[2]);
    at.v[3] = 1.f;
    afxM4d v = AfxM4dFromM3d(c, at);
    afxM4d iv = AfxM4dTransposeAtm(v);
    iv = AfxM4dFromAtm(iv);
    at.v3 = AfxV3dPostMultiplyM3d(c, cam->displace);
    iv.v4[3].v3 = AfxV3dAdd(at.v3, cam->pos);
    iv.m[3][3] = 1.f;
    cam->shouldSyncV = FALSE;
    *pV = v;
    *pIv = iv;
}

_ARXINL void _ArxComputeProjectionMatrices(arxCamera cam, afxM4d* pP, afxM4d* pIp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    // uses trigonometric functions (AfxSinf and AfxCosf) to get the tangent of half of the field of view angle.
    afxReal half = AfxSinf(cam->fovY * 0.5f) / AfxCosf(cam->fovY * 0.5f);
    // The initial projection matrix is set up based on the camera’s field of view and aspect ratio. 
    // This is common for perspective projections.
    afxM4d p;
    p.m[0][0] = cam->wrOverHr / (cam->wwOverHw * cam->wpOverHp * half);
    p.m[0][1] = 0.f;
    p.m[0][2] = 0.f;
    p.m[0][3] = 0.f;
    p.m[1][0] = 0.f;
    p.m[1][1] = 1.f / half;
    p.m[1][2] = 0.f;
    p.m[1][3] = 0.f;
    p.m[2][0] = 0.f;
    p.m[2][1] = 0.f;
    p.m[2][3] = -1.f;
    p.m[3][0] = 0.f;
    p.m[3][1] = 0.f;
    p.m[3][3] = 0.f;

    // far and near represent the distance to the far and near clip planes, respectively. 
    // epsilon is used to prevent division by zero or other small-precision issues.
    afxReal far = cam->farClipPlane;
    afxReal near = cam->nearClipPlane;
    afxReal epsilon = cam->depthRangeEpsilon;
    // nearMinFarRecip is the reciprocal of the difference between the near and far planes, used to normalize the depth values. 
    // nearTimesFar helps in scaling the depth projection.
    afxReal nearMinFarRecip = 1.f / (near - far);
    afxReal nearTimesFar = near * far;

    /*
        When the far plane (far) is set to zero, it's commonly associated with a situation where there's no real "far" depth,
        and we want to project to an infinitely distant far plane. In this context, we typically want to deal with projection
        matrices that can handle an infinite far plane or a depth that extends indefinitely.

        When the far plane is zero, we want to ensure that the depth is handled in a way that doesn't result in division by zero errors.
        One way to do this is to use an epsilon value (a very small number close to zero, but not exactly zero) to maintain stability in the projection.
        This ensures the matrix still behaves reasonably when multiplied by vertices.
    */

    switch (cam->depthRange)
    {
    case avxClipSpaceDepth_NEG_ONE_TO_ONE:
    {
        // The depth mapping follows the traditional OpenGL method (near to -1 and far to 1). 
        // If far == 0.f, the projection matrix entries are adjusted using the epsilon value to avoid invalid behavior or division by zero.

        if (far == 0.f)
        {
            // This ensures the depth mapping does not result in NaN or undefined values when there is no far plane.
            p.m[2][2] = epsilon - 1.f;
            p.m[3][2] = (epsilon - 2.f) * near;
        }
        else
        {
            p.m[2][2] = (near + far) * nearMinFarRecip;
            p.m[3][2] = nearMinFarRecip * nearTimesFar + nearMinFarRecip * nearTimesFar;
        }
        break;
    }
    case avxClipSpaceDepth_ZERO_TO_ONE:
    {
        // The depth mapping follows the traditional Direct3D method (and default of Vulkan), the depth range is from 0 to 1.
        // The same handling for far == 0.f applies here with epsilon, ensuring the matrix remains valid when the far plane is not defined.
        // Otherwise, the depth range is calculated using the standard formula for the ZERO_TO_ONE space, 
        // mapping the near and far planes to the appropriate depth buffer range.

        if (far == 0.f)
        {
            p.m[2][2] = epsilon - 1.f;
            p.m[3][2] = (epsilon - 1.f) * near;
        }
        else
        {
            p.m[2][2] = nearMinFarRecip * far;
            p.m[3][2] = nearMinFarRecip * nearTimesFar;
        }
        break;
    }
    case avxClipSpaceDepth_NEG_ONE_TO_ZERO:
    {
        // This experimental depth mapping of Qwadro inverts the typical depth buffer behavior (where closer objects have larger depth values). 
        // Here, near = 0 and far = -1 are set in a reversed manner, making this useful for some custom depth buffer optimizations.
        // The epsilon correction is applied when far == 0.f to ensure the projection matrix remains valid and avoids division by zero.
        // This is especially important for reverse depth mapping, as it helps maintain a valid matrix while ensuring that the depth precision is enhanced when used in custom shaders or for reverse depth buffering.

        // The code comments explain the potential use cases for NEG_ONE_TO_ZERO, such as reverse depth buffering and custom projection matrices. 
        // The expected behavior in rendering techniques like shadow mapping or custom depth-based post-processing is acknowledged.
        // Similar to other cases, when far == 0.f, we use an epsilon value to ensure that the matrix remains valid.

        //

        // Expected behavior for NEG_ONE_TO_ZERO:
        // The near plane should be at 0, and the far plane should be at -1 (as opposed to traditional approaches where the far plane is positive).
        // However, it seems that this depth range might conflict with typical depth-buffering mechanisms, making it hard to find direct support in common hardware or shaders.

        // Although uncommon, there are some potential use cases for this depth range, for example:
        // Reverse Depth Buffering:
        // In some custom rendering techniques, using reverse depth buffers (where closer objects have larger depth values) can improve depth precision.
        // However, the range being from 0 to - 1 complicates things, especially when working with standard depth ranges.
        // Custom Projection Matrices:
        // Some applications or engines might use this range in specialized techniques like shadow mapping or depth-based post-processing effects where you want to invert the "depth" ordering in your projection space.

        // Ensure the range doesn't cause problems in shaders.
        // Since the NEG_ONE_TO_ZERO depth range would invert the expected depth behavior (where closer objects have a smaller depth value), 
        // it can lead to difficulties in standard shaders, depth tests, and other components that expect the depth to increase as objects move further from the camera.
        // You might want to:
        // Test depth-based operations carefully to ensure that they behave as expected.
        // Modify shaders or depth-test configurations to account for this reversed depth space.
        // Consider debugging tools to visualize the depth values in this custom space to ensure they behave as expected.

        if (far != 0.f)
        {
            p.m[2][2] = nearMinFarRecip * near;
            p.m[3][2] = nearMinFarRecip * nearTimesFar;
        }
        else
        {
            // To be tested.
            // Apply some small epsilon correction
            // Using the epsilon correction when far == 0.f to avoid division by zero and keep the projection matrix valid. 
            // This keeps the behavior consistent with the reversed Z approach.
            p.m[2][2] = epsilon - 1.f;
            p.m[3][2] = (epsilon - 1.f) * near;
        }
        break;
    }
    default: AfxThrowError(); break;
    }

    // This section calculates the inverse of the projection matrix p (stored in ip). 
    // This is common in graphics to transform coordinates back from clip space to view space. 
    // The inversion is done by manually calculating the appropriate inverse for the 4x4 matrix, 
    // which is necessary for certain calculations (like unprojection).
    afxM4d ip;
    ip.m[0][0] = 1.f / p.m[0][0];
    ip.m[0][1] = 0.f;
    ip.m[0][2] = 0.f;
    ip.m[0][3] = 0.f;
    ip.m[1][0] = 0.f;
    ip.m[1][1] = 1.f / p.m[1][1];
    ip.m[1][2] = 0.f;
    ip.m[1][3] = 0.f;
    ip.m[2][0] = 0.f;
    ip.m[2][1] = 0.f;
    ip.m[2][2] = 0.f;
    ip.m[2][3] = 1.f / p.m[3][2];
    ip.m[3][0] = 0.f;
    ip.m[3][1] = 0.f;
    ip.m[3][2] = 1.f / p.m[2][3];
    ip.m[3][3] = -(p.m[2][2] / (p.m[3][2] * p.m[2][3]));
    cam->shouldSyncP = FALSE;

    *pP = p;
    *pIp = ip;
}

_ARXINL void _ArxRecomputeCameraMatrices(arxCamera cam)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    afxBool syncV = cam->shouldSyncV;
    afxBool syncP = cam->shouldSyncP;

    if (syncV)
        _ArxComputeCameraMatrices(cam, &cam->v, &cam->iv);

    if (syncP)
        _ArxComputeProjectionMatrices(cam, &cam->p, &cam->ip);

    if (syncP || syncV)
    {
        cam->pv = AfxM4dMultiply(cam->p, cam->v);
        cam->ipv = AfxM4dMultiply(cam->ip, cam->iv);
        cam->frustum = AfxMakeFrustum(cam->pv, cam->ipv);
    }
}

_ARXINL void ArxGetCameraMatrices(arxCamera cam, afxM4d* iv, afxM4d* v)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(v || iv);
    _ArxRecomputeCameraMatrices(cam);
    if (v) *v = cam->v;
    if (iv) *iv = cam->iv;
}

_ARXINL void ArxGetProjectionMatrices(arxCamera cam, afxM4d* ip, afxM4d* ipv, afxM4d* pv, afxM4d* p)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(p || ip || pv || ipv);
    _ArxRecomputeCameraMatrices(cam);
    if (p) *p = cam->p;
    if (ip) *ip = cam->ip;
    if (pv) *pv = cam->pv;
    if (ipv) *ipv = cam->ipv;
}

_ARXINL afxReal ArxGetCameraFov(arxCamera cam, afxReal* fovX)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    if (fovX)
    {
        afxReal aspRatio = cam->wrOverHr;//screenWidth / screenHeight;
        afxReal vFov = cam->fovY;
        afxReal hFov = 2.0f * AfxAtanf(AfxTanf(vFov / 2.0f) * aspRatio);
        *fovX = hFov;
    }
    return cam->fovY;
}

_ARXINL afxReal ArxAdjustCameraFov(arxCamera cam, afxReal fov)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    afxReal lastVal = cam->fovY;
    // The value 1.0471976f is approximately equal to Pi / 3, or 60 degrees in radians.
    cam->fovY = fov ? fov : 1.0471976f;
    cam->shouldSyncP = TRUE;
    return lastVal;
}

_ARXINL void ArxSetCameraClipPlanes(arxCamera cam, afxReal near, afxReal far)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->nearClipPlane = near ? near : 0.1f;
    cam->farClipPlane = far ? far : 1000.f;
    cam->shouldSyncP = TRUE;
}

_ARXINL void ArxGetCameraFrustum(arxCamera cam, afxReal* nearClipPlane, afxReal* farClipPlane, afxFrustum* frustum)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT(frustum || nearClipPlane || farClipPlane);

    if (nearClipPlane)
        *nearClipPlane = cam->nearClipPlane;

    if (farClipPlane)
        *farClipPlane = cam->farClipPlane;

    if (frustum)
    {
        _ArxRecomputeCameraMatrices(cam);
        *frustum = cam->frustum;
    }
}

_ARXINL avxClipSpaceDepth ArxGetCameraDepthRange(arxCamera cam, afxReal* epsilon)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    if (epsilon) *epsilon = cam->depthRangeEpsilon;
    return cam->depthRange;
}

_ARXINL afxV3d ArxGetCameraDisplacement(arxCamera cam/*, afxV3d displace*/)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(displace);
    // displace[2] is distance from target.
    return cam->displace;
}

_ARXINL void ArxDisplaceCamera(arxCamera cam, afxV3d const displace)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    
    //if (!displace)
        //AfxV3dCopy(cam->displace, AFX_V4D_IDENTITY);
    //else
        cam->displace = displace;

    cam->shouldSyncV = TRUE;
}

_ARXINL afxReal ArxGetCameraDistance(arxCamera cam)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    return cam->displace.v[2];
}

_ARXINL void ArxSetCameraDistance(arxCamera cam, afxReal distance)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    cam->displace.v[2] = distance;
    cam->shouldSyncV = TRUE;
}

_ARXINL afxV3d ArxGetCameraOrbit(arxCamera cam/*, afxV3d elevAzimRoll*/)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(elevAzimRoll);
    return cam->elevAzimRoll;
}

_ARXINL void ArxOrbitCamera(arxCamera cam, afxV3d const elevAzimRoll)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    //if (!elevAzimRoll)
        //AfxV3dCopy(cam->elevAzimRoll, AFX_V4D_IDENTITY);
    //else
        cam->elevAzimRoll = elevAzimRoll;

    cam->shouldSyncV = TRUE;
}

_ARXINL void ArxGetCameraDirectionX(arxCamera cam, afxV3d* left, afxV3d* right)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(left || right, left != right);
    _ArxRecomputeCameraMatrices(cam);
    if (left) *left = AfxV3dNeg(cam->iv.v4[0].v3);
    if (right) *right = cam->iv.v4[0].v3;
}

_ARXINL void ArxGetCameraDirectionY(arxCamera cam, afxV3d* down, afxV3d* up)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(down || up, down != up);
    _ArxRecomputeCameraMatrices(cam);
    if (down) *down = AfxV3dNeg(cam->iv.v4[1].v3);
    if (up) *up = cam->iv.v4[1].v3;
}

_ARXINL void ArxGetCameraDirectionZ(arxCamera cam, afxV3d* near, afxV3d* far)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT2(far || near, far != near);
    _ArxRecomputeCameraMatrices(cam);
    if (near) *near = cam->iv.v4[2].v3;
    if (far) *far = AfxV3dNeg(cam->iv.v4[2].v3);
}

_ARXINL afxV3d ArxGetCameraTranslation(arxCamera cam/*, afxV3d point*/)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(point);
    _ArxRecomputeCameraMatrices(cam);
    return cam->iv.v4[3].v3;
}

_ARXINL void ArxSetCameraOrigin(arxCamera cam, afxV3d const point)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    
    //if (!point)
        //AfxV3dZero(cam->pos);
    //else
        cam->pos = point;

    cam->shouldSyncV = TRUE;
}

_ARXINL void ArxTranslateCamera(arxCamera cam, afxV3d const motion)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(motion);

    _ArxRecomputeCameraMatrices(cam);

#if 0 // The RW way; just for test actually
    afxV3d offset =
    {
        motion[0] * cam->iv[0][0] + motion[1] * cam->iv[1][0] + motion[2] * cam->iv[2][0],
        motion[0] * cam->iv[0][1] + motion[1] * cam->iv[1][1] + motion[2] * cam->iv[2][1],
        motion[0] * cam->iv[0][2] + motion[1] * cam->iv[1][2] + motion[2] * cam->iv[2][2]
    };
    AfxV3dAdd(cam->pos, cam->pos, offset);
#else
    cam->pos.v[0] = motion.v[0] * cam->iv.m[0][0] + cam->pos.v[0];
    cam->pos.v[1] = motion.v[0] * cam->iv.m[0][1] + cam->pos.v[1];
    cam->pos.v[2] = motion.v[0] * cam->iv.m[0][2] + cam->pos.v[2];

    cam->pos.v[0] = motion.v[1] * cam->iv.m[1][0] + cam->pos.v[0];
    cam->pos.v[1] = motion.v[1] * cam->iv.m[1][1] + cam->pos.v[1];
    cam->pos.v[2] = motion.v[1] * cam->iv.m[1][2] + cam->pos.v[2];

    cam->pos.v[0] = motion.v[2] * cam->iv.m[2][0] + cam->pos.v[0];
    cam->pos.v[1] = motion.v[2] * cam->iv.m[2][1] + cam->pos.v[1];
    cam->pos.v[2] = motion.v[2] * cam->iv.m[2][2] + cam->pos.v[2];
#endif
    cam->shouldSyncV = TRUE;
}

_ARXINL afxV4d ArxFindWorldCoordinates(arxCamera cam, afxV2d const wh, afxV3d const screenPoint/*, afxV4d worldPoint*/)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(wh);
    //AFX_ASSERT(screenPoint);
    //AFX_ASSERT(worldPoint);

    afxV4d v = AFX_V4D(
        ((screenPoint.v[0] + screenPoint.v[0]) - wh.v[0]) / wh.v[0],
        ( screenPoint.v[1] + screenPoint.v[1]  - wh.v[1]) / wh.v[1],
        ((screenPoint.v[2] + screenPoint.v[2] - 1.f - -1.f) * 0.5f),
        1.f
    );

    _ArxRecomputeCameraMatrices(cam);

    afxV4d v2 = AfxV4dPreMultiplyM4d(v, cam->ip);
    v = AfxV4dPreMultiplyM4d(v2, cam->iv);
    afxV4d worldPoint;
    worldPoint.v3 = AfxV3dScale(v.v3, 1.f / v.v[3]);
    worldPoint.v[3] = 1.f;
    return worldPoint;
}

_ARXINL afxV3d ArxFindScreenCoordinates(arxCamera cam, afxV2d const wh, afxV4d const worldPoint/*, afxV3d screenPoint*/)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(wh);
    //AFX_ASSERT(worldPoint);
    AFX_ASSERT(worldPoint.v[3] == 1.f);
    //AFX_ASSERT(screenPoint);

    _ArxRecomputeCameraMatrices(cam);

    afxV4d v = AfxV4dPreMultiplyM4d(worldPoint, cam->v);
    afxV4d v2 = AfxV4dPreMultiplyM4d(v, cam->p);
    v.v3 = AfxV3dScale(v2.v3, 1.f / v2.v[3]);

    if (cam->depthRange == avxClipSpaceDepth_NEG_ONE_TO_ONE)
        v.v[2] = v.v[2] + v.v[2] - 1.f;

    return AfxV3dMake((v.v[0] + 1.f) * wh.v[0] * 0.5f, (v.v[1] + 1.f) * wh.v[1] * 0.5f, v.v[2]);
}

_ARXINL void ArxGetCameraPickingRay(arxCamera cam, afxV2d const wh, afxV2d const cursor, afxV4d* origin, afxV3d* normal)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(wh);
    //AFX_ASSERT(cursor);
    AFX_ASSERT(origin);
    AFX_ASSERT(normal);

    _ArxRecomputeCameraMatrices(cam);

    origin->v3 = ArxGetCameraTranslation(cam);
    origin->w = 1;

    afxV4d v = AFX_V4D(
        (cursor.v[0] + cursor.v[0] - wh.v[0]) / wh.v[0],
        (cursor.v[1] + cursor.v[1] - wh.v[1]) / wh.v[1],
        -1.f, 
        1.f
    );

    afxV4d v2 = AfxV4dPreMultiplyM4d(v, cam->ip);
    v2.v[3] = 0.0;
    v = AfxV4dPreMultiplyM4d(v2, cam->iv);
    
    // should normalize or zero
    afxReal len = AfxV3dMag(v.v3);

    if (len <= 0.0000099999997f)
        *normal = AfxV3dZero();
    else
        *normal = AfxV3dScale(v.v3, 1.f / len);
}

_ARXINL void ArxComputeCameraRelativePlanarBases(arxCamera cam, afxBool screenOrthogonal, afxV3d const planeNormal, afxV4d const pointOnPlane, afxV3d* xBasis, afxV3d* yBasis)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    //AFX_ASSERT(planeNormal);
    //AFX_ASSERT(pointOnPlane);
    AFX_ASSERT(xBasis);
    AFX_ASSERT(yBasis);

    ArxGetCameraDirectionX(cam, NIL, xBasis);
    afxV3d v;

    if (!screenOrthogonal) v = *xBasis;
    else
    {
        afxV3d up;
        afxV4d origin, camToPoint;
        origin.v3 = ArxGetCameraTranslation(cam);
        origin.w = 1;
        ArxGetCameraDirectionY(cam, NIL, &up);
        camToPoint = AfxV4dSub(pointOnPlane, origin);
        v.v[0] = up.v[2] * camToPoint.v[1] - up.v[1] * camToPoint.v[2];
        v.v[1] = up.v[0] * camToPoint.v[2] - up.v[2] * camToPoint.v[0];
        v.v[2] = up.v[1] * camToPoint.v[0] - up.v[0] * camToPoint.v[1];
    }

    yBasis->v[0] = (v.v[2] * planeNormal.v[1]) - (v.v[1] * planeNormal.v[2]);
    yBasis->v[1] = (v.v[0] * planeNormal.v[2]) - (v.v[2] * planeNormal.v[0]);
    yBasis->v[2] = (v.v[1] * planeNormal.v[0]) - (v.v[0] * planeNormal.v[1]);

    // should normalize or zero
    afxReal len = AfxV3dMag(*yBasis);
    
    if (len <= 0.0000099999997f)
        *yBasis = AfxV3dZero();
    else
        *yBasis = AfxV3dScale(*yBasis, AfxRecip(len));
}

#if 0
_ARX afxBool ArxProcessCameraInteraction(arxCamera cam, afxUnit port, afxReal64 speed, afxReal64 dt)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    afxBool updated = FALSE;

    if (AfxIsLmbPressed(port))
    {
        afxV2d delta;
        afxV3d deltaEar;
        AfxGetMouseMotion(0, delta);
        deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
        deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
        deltaEar[2] = 0.f;

        AfxApplyCameraElevAzimRoll(cam, deltaEar);
        updated = TRUE;
    }

    if (AfxIsRmbPressed(port))
    {
        afxV2d delta;
        afxV3d off;
        AfxGetMouseMotion(0, delta);
        off[0] = -((afxReal)(delta[0] * AFX_PI / 180.f));
        off[1] = -((afxReal)(delta[1] * AFX_PI / 180.f));
        off[2] = 0.f;

        AfxApplyCameraDisplacement(cam, off);
        updated = TRUE;
    }

    afxReal w = AfxGetMouseWheelDelta(port);
    w = w / 120.f; // WHEEL_DELTA
    ArxApplyCameraDistance(cam, w);

    if (!speed)
        speed = 30.f;

    afxReal64 frameStep = dt * speed;
    // Note: because the negZ axis is forward, we have to invert the way you'd normally think about the 'W' or 'S' key's action.
    afxReal64 fwdSpeed = (AfxGetKeyPressure(0, afxKey_W) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_S) ? 1 : 0.0f);
    afxReal64 sideSpeed = (AfxGetKeyPressure(0, afxKey_A) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_D) ? 1 : 0.0f);
    afxReal64 upSpeed = (AfxGetKeyPressure(0, afxKey_Q) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_E) ? 1 : 0.0f);
    afxV3d v =
    {
        frameStep * sideSpeed,
        frameStep * upSpeed,
        frameStep * fwdSpeed
    };

    if (AfxV3dSum(v))
    {
        ArxTranslateCamera(cam, v);
        updated = TRUE;
    }
    return updated;
}
#endif

#if 0
_ARX afxBool _AvxCamEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = { 0 };
    arxCamera cam = (void*)obj;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case AFX_EVENT_MSE_AXIS_UPDATED:
    {
        //afxMouse mse = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_MSE, 1, &mse);

        // TODO Leva isso para o usuário

        if (AfxIsLmbPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -AfxRadf(delta[0]);
            deltaEar[0] = -AfxRadf(delta[1]);
            deltaEar[2] = 0.f;
            afxV3d v;
            ArxGetCameraOrbit(cam, deltaEar, avxBlendOp_ADD, v);
            ArxOrbitCamera(cam, v);
        }

        if (AfxIsRmbPressed(0))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta);
            off[0] = -AfxRadf(delta[0]);
            off[1] = -AfxRadf(delta[1]);
            off[2] = 0.f;
            afxV3d v;
            ArxGetCameraDisplacement(cam, off, avxBlendOp_ADD, v);
            ArxDisplaceCamera(cam, v);
        }
        break;
    }
    case AFX_EVENT_MSE_WHEEL_UPDATED:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        ArxApplyCameraDistance(cam, w);
        break;
    }
    case AFX_EVENT_KBD_PRESSED:
    {
        //afxKeyboard kbd = (void*)watched;
        //AFX_ASSERT_OBJECTS(afxFcc_KBD, 1, &kbd);
        break;
    }
    default:break;
    }
    return FALSE;
}
#endif

_ARXINL void ArxRestoreCamera(arxCamera cam)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    cam->flags = NIL;

    cam->wpOverHp = 1.33f;
    cam->wrOverHr = 1.33f;
    cam->wwOverHw = 1.f;
    cam->fovY = 1.0471976f;

    cam->nearClipPlane = 0.0001f;// 0.4;
    cam->farClipPlane = 1000000.0;// 1000.f;

    arxScenario scio = AfxGetHost(cam);
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxReal rangeEps = AFX_EPSILON;
    cam->depthRange = AvxGetSystemClipSpaceDepth(dsys, &rangeEps);
    cam->depthRangeEpsilon = rangeEps;
    //cam->depthRange = avxClipSpaceDepth_NEG_ONE_TO_ONE;
    //cam->depthRangeEpsilon = AFX_EPSILON;

    cam->useQuatOrient = TRUE;
    cam->pos = AfxV3dZero();
    cam->orient = AfxQuatIdentity();
    cam->orientM3d = AfxM3dIdentity();
    cam->elevAzimRoll = AfxV3dZero();
    cam->displace = AfxV3dZero();
    cam->shouldSyncP = TRUE;
    cam->shouldSyncV = TRUE;
    _ArxRecomputeCameraMatrices(cam);
}

_ARX afxError ArxSetCameraNode(arxCamera cam, arxNode nod, void(*sync)(arxNodular*), afxFlags dagFlags, afxMask dagMask)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    ArxMakeNodulation(&cam->nodu, nod, sync, dagFlags, dagMask);

    return err;
}

_ARX afxBool ArxGetCameraNode(arxCamera cam, arxNode* node)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    arxNode nod;
    ArxGetNode(&cam->nodu, &nod);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);
    AFX_ASSERT(node);
    *node = nod;
    return !!nod;
}

_ARX afxError _ArxCamDtorCb(arxCamera cam)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    ArxSetCameraNode(cam, NIL, NIL, NIL, NIL);

    return err;
}

_ARX afxError _ArxCamCtorCb(arxCamera cam, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(args[1]);
    arxCameraConfig const* cfg = &(AFX_CAST(arxCameraConfig const*, args[1])[invokeNo]);

    afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // TODO: Get it from DSYS/DDEV
    afxReal rangeEps = AFX_EPSILON;
    cam->depthRange = AvxGetSystemClipSpaceDepth(dsys, &rangeEps);
    cam->depthRangeEpsilon = rangeEps;

    cam->perspective = TRUE;

    afxM4d m = AfxM4dIdentity();
    afxM4d m2 = AfxM4dIdentity();
    cam->frustum = AfxMakeFrustum(m, m2);
    
    ArxRestoreCamera(cam);

    cam->nodu = (arxNodular) { 0 };
    ArxSetCameraNode(cam, NIL, NIL, NIL, NIL);

    return err;
}

_ARX afxClassConfig const _ARX_CAM_CLASS_CONFIG =
{
    .fcc = afxFcc_CAM,
    .name = "Camera",
    .desc = "Device-Aware Camera",
    .fixedSiz = sizeof(AFX_OBJECT(arxCamera)),
    .ctor = (void*)_ArxCamCtorCb,
    .dtor = (void*)_ArxCamDtorCb,
#if 0
    .eventFilter = (void*)_ArxCamEventFilter
#endif
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireCameras(arxScenario scio, afxUnit cnt, arxCameraConfig const cfg[], arxCamera cameras[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

    afxClass* cls = (afxClass*)_ArxScioGetCamClassCb_SW(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)cameras, (void const*[]) { scio, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_CAM, cnt, cameras);

    return err;
}
