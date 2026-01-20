/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

/**
    Qwadro performs all bone animation on decomposed transforms, where "decomposed" means that the position, 
    orientation, and scale/shear components have been pulled out into separately animating quantities. 
    This allows for fair, efficient animation and interpolation, whereas keeping everything as a tangled 4x4 matrix does not. 

    The afxTransform is the primary structure used to store these decomposed transforms. 
    Each structure stores a 4-element position vector, a 4-element quaternion orientation, and a 3x3 scale/shear matrix 
    (note that it may also contain mirroring). 
    Additionally, each transform stores a set of afxTransformFlags that indicates which, if any, of these values current 
    has a non-identity value. These flags are solely used to speed up computation.

    When using transformation matrices we usually use the terms "rotation" and "translation". 
    When using a pose we use the terms "orientation" and "position". 
    But that’s just a subtle difference – it pretty much means the same.
*/

#ifndef AFX_TRANSFORM_H
#define AFX_TRANSFORM_H

#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxQuaternion.h"

typedef enum afxDof
{
    afxDof_NONE,

    afxDof_T_X      = AFX_BITMASK(0),
    afxDof_T_Y      = AFX_BITMASK(1),
    afxDof_T_Z      = AFX_BITMASK(2),
    afxDof_T_XY     = (afxDof_T_X | afxDof_T_Y),
    afxDof_T_XZ     = (afxDof_T_X | afxDof_T_Z),
    afxDof_T_YZ     = (afxDof_T_Y | afxDof_T_Z),
    afxDof_T        = (afxDof_T_X | afxDof_T_YZ),

    afxDof_R_X      = AFX_BITMASK(3),
    afxDof_R_Y      = AFX_BITMASK(4),
    afxDof_R_Z      = AFX_BITMASK(5),
    afxDof_R_XY     = (afxDof_R_X | afxDof_R_Y),
    afxDof_R_XZ     = (afxDof_R_X | afxDof_R_Z),
    afxDof_R_YZ     = (afxDof_R_Y | afxDof_R_Z),
    afxDof_R        = (afxDof_R_X | afxDof_R_YZ),

    // scale/shear
    afxDof_S_X      = AFX_BITMASK(6),
    afxDof_S_Y      = AFX_BITMASK(7),
    afxDof_S_Z      = AFX_BITMASK(8),
    afxDof_S_XY     = (afxDof_S_X | afxDof_S_Y),
    afxDof_S_XZ     = (afxDof_S_X | afxDof_S_Z),
    afxDof_S_YZ     = (afxDof_S_Y | afxDof_S_Z),
    afxDof_S        = (afxDof_S_X | afxDof_S_YZ),

    // DOF of 2D plane-based motion.
    afxDof_PLANAR   = (afxDof_T_XY | afxDof_R_Z)

} afxDof;

typedef enum afxTransformFlag
{
    // The transform has translation.
    afxTransformFlag_T          = AFX_BITMASK(0), // has non-identity position
    // The transform has orientation/rotation.
    afxTransformFlag_R          = AFX_BITMASK(1), // has non-identity orientation
    // The transform has rigid transformation.
    afxTransformFlag_RIGID      = (afxTransformFlag_T | afxTransformFlag_R),
    // The transform has scaling and/or shearing; it is effectively non-rigid transform.
    afxTransformFlag_S          = AFX_BITMASK(2), // has non-identity scale/shear
    // The transform has uniform scaling. [ s, 0, 0, 0, s, 0, 0, 0, s ]
    afxTransformFlag_UNISCALED  = AFX_BITMASK(3),
    // The transform has non-uniform scaling. [ sX, 0, 0, 0, sY, 0, 0, 0, sZ ]
    afxTransformFlag_SCALED     = AFX_BITMASK(4),
    afxTransformFlag_ALL        = (afxTransformFlag_T | afxTransformFlag_R | afxTransformFlag_S)
} afxTransformFlags;

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxTransform)
{
    afxQuat             oq;
    afxV3d              pv; // @ 16
    afxTransformFlags   flags; // @ 28
    afxM3d              ssm; // @ 32
};

AFX afxTransform const AFX_TRANSFORM_ZERO;
AFX afxTransform const AFX_TRANSFORM_IDENTITY;

// You can initialize a afxTransform to the identity transform like this:

AFXINL afxBool  AfxZeroTransform(afxTransform* t); // make zero
AFXINL afxBool  AfxResetTransform(afxTransform* t); // make identity

AFXINL afxBool  AfxCopyTransform(afxTransform* t, afxTransform const *in);

AFXINL afxBool  AfxCopyRigidTransform(afxTransform* t, afxTransform const *in);

AFXINL afxBool  AfxMakeTransform(afxTransform* t, afxV3d const pos, afxQuat const orient, afxM3d const scaleShear, afxBool check);

AFXINL afxBool  AfxMakeRigidTransform(afxTransform* t, afxV3d const pos, afxQuat const orient, afxBool check);

AFXINL void     AfxEnforceTransformDofs(afxTransform* t, afxDof allowedDofs);

AFXINL afxReal  AfxDetTransform(afxTransform const* t);

AFXINL void     AfxMixTransform(afxTransform* t, afxTransform const* a, afxTransform const* b, afxReal time);

AFXINL afxBool  AfxInvertTransform(afxTransform const* in, afxTransform* t);

AFXINL void     AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre);
AFXINL void     AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post);
AFXINL void     AfxMultiplyTransform(afxTransform* t, afxTransform const* a, afxTransform const* b);

AFXINL void     AfxTransformArrayedAtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);
AFXINL void     AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[]);

AFXINL void     AfxTransformAtv3d(afxTransform const* t, afxV3d const in, afxV3d out);
AFXINL void     AfxTransformLtv3d(afxTransform const* t, afxV3d const in, afxV3d out);
AFXINL void     AfxTransformLtv3dTransposed(afxTransform const* t, afxV3d const in, afxV3d out);

AFXINL void     AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxTransform const in[], afxTransform out[]);

AFXINL void     AfxComputeCompositeTransformM4d(afxTransform const* t, afxM4d m); // build composite transform 4x4
AFXINL void     AfxComputeCompositeTransformM4dc(afxTransform const* t, afxM4r m); // build composite transform 4x3 (compact matrix)

AFXINL void BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite);
AFXINL void BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4r ResultComposite3x4);
AFXINL void BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix);
AFXINL void BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix);
AFXINL void BWP_Dispatch(afxTransform const* t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld);
AFXINL void BWPNC_Dispatch(afxTransform const* t, afxM4d const ParentWorld, afxM4d World);

#endif//AFX_TRANSFORM_H
