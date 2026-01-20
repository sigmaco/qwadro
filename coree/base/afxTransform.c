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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/io/afxStream.h"

afxTransform const AFX_TRANSFORM_ZERO =
{
    .flags = NIL,
    .pv = { 0.f, 0.f, 0.f },
    .oq = { 0.f, 0.f, 0.f, 0.f },
    .ssm = { { 0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f}, { 0.f, 0.f, 0.f } }
};

afxTransform const AFX_TRANSFORM_IDENTITY =
{
    .flags = NIL,
    .pv = { 0.f, 0.f, 0.f },
    .oq = { 0.f, 0.f, 0.f, 1.f },
    .ssm = { { 1.f, 0.f, 0.f}, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f } }
};

_AFXINL afxReal AfxDetTransform(afxTransform const* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    return AfxM3dDet(t->ssm);
}

_AFXINL afxBool AfxCopyTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    afxTransformFlags dstRigidFlags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    afxTransformFlags srcRigidFlags = (in->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    *t = *in;
    return (srcRigidFlags || dstRigidFlags);
}

_AFXINL afxBool AfxCopyRigidTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    afxTransformFlags dstRigidFlags = (t->flags & afxTransformFlag_RIGID);
    afxTransformFlags srcRigidFlags = (in->flags & afxTransformFlag_RIGID);
    t->flags |= srcRigidFlags;
    AfxV3dCopy(t->pv, in->pv);
    AfxQuatCopy(t->oq, in->oq);
    //AfxM3dReset(t->ssm);
    return (srcRigidFlags || dstRigidFlags);
}

_AFXINL afxBool AfxResetTransform(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    afxBool changed = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    *t = AFX_TRANSFORM_IDENTITY;
    return changed;
}

_AFXINL afxBool AfxZeroTransform(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    afxBool changed = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    *t = AFX_TRANSFORM_ZERO;
    return changed;
}

_AFXINL afxBool AfxMakeTransform(afxTransform* t, afxV3d const pv, afxQuat const oq, afxM3d const ssm, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(oq);
    AFX_ASSERT(ssm);
    AfxV3dCopy(t->pv, pv);
    AfxQuatCopy(t->oq, oq);
    AfxM3dCopy(t->ssm, ssm);

    afxTransformFlags srcFlags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));

    if (check)
    {
        t->flags = NIL;

        if (!AfxV3dIsZero(pv))
            t->flags |= afxTransformFlag_T;

        if (!AfxQuatIsIdentity(oq))
            t->flags |= afxTransformFlag_R;

        if (!AfxM3dIsIdentity(ssm))
            t->flags |= afxTransformFlag_S;
    }
    else t->flags = (afxTransformFlag_RIGID | afxTransformFlag_S);

    return (srcFlags || t->flags);
}

_AFXINL afxBool AfxMakeRigidTransform(afxTransform* t, afxV3d const pos, afxQuat const orient, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    afxTransformFlags srcFlags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    t->flags = NIL;

    if (!pos) AfxV3dZero(t->pv);
    else
    {
        AfxV3dCopy(t->pv, pos);

        if (!check || !AfxV3dIsZero(pos))
            t->flags |= afxTransformFlag_T;
    }

    if (!orient) AfxQuatReset(t->oq);
    else
    {
        AfxQuatCopy(t->oq, orient);

        if (!check || !AfxQuatIsIdentity(orient))
            t->flags |= afxTransformFlag_R;
    }

    AfxM3dCopy(t->ssm, AFX_M3D_IDENTITY);

    return (srcFlags || t->flags);
}

_AFXINL void AfxMultiplyTransform(afxTransform *t, afxTransform const* a, afxTransform const* b)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
#if 0
    afxM3d OrientationB, Temp, tmp;
    AfxM3dRotationQuat(OrientationB, b->oq);
    AfxM3dMultiply(tmp, OrientationB, b->ssm);
    AfxM3dMultiply(Temp, a->ssm, tmp);
    TransposeMatrixMultiply3x3(t->ssm, OrientationB, Temp);
    
    AfxV3dPostMultiplyM3d(t->pv, a->ssm, b->pv);

    afxM3d OrientationA;
    AfxM3dRotationQuat(OrientationA, a->oq);
    AfxV3dPostMultiplyM3d(t->pv, OrientationA, t->pv);
    AfxV3dAdd(t->pv, t->pv, a->pv);
    
    AfxQuatMultiply(t->oq, a->oq, b->oq);
    t->flags = b->flags | a->flags;
#endif

    afxM3d am, bm, tmp;
    AfxM3dRotationQuat(am, a->oq);
    AfxM3dRotationQuat(bm, b->oq);
    AfxM3dMultiply(t->ssm, bm, b->ssm);
    AfxM3dMultiply(tmp, a->ssm, t->ssm);    
    TransposeMatrixMultiply3x3(t->ssm, bm, tmp);
    afxV3d pos;
    AfxV3dPostMultiplyM3d(pos, a->ssm, b->pv);
    AfxV3dPostMultiplyM3d(t->pv, am, pos);
    AfxV3dAdd(t->pv, a->pv, t->pv);
    AfxQuatMultiply(t->oq, a->oq, b->oq);
    t->flags = b->flags | a->flags;
}

_AFXINL void AfxPreMultiplyTransform(afxTransform* t, afxTransform const* pre)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, pre, t);
    AfxCopyTransform(t, &tmp);
}

_AFXINL void AfxPostMultiplyTransform(afxTransform* t, afxTransform const* post)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    afxTransform tmp;
    AfxMultiplyTransform(&tmp, t, post);
    AfxCopyTransform(t, &tmp);
}

_AFXINL void AfxMixTransform(afxTransform *t, afxTransform const* a, afxTransform const* b, afxReal time)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(a);
    AFX_ASSERT(b);

    afxTransformFlags flags = b->flags | a->flags;
    t->flags = flags;

    if (!(flags & afxTransformFlag_T)) AfxV3dZero(t->pv);
    else
    {
        AfxV3dMix(t->pv, a->pv, b->pv, time);
    }

    if (!(flags & afxTransformFlag_R)) AfxQuatReset(t->oq);
    else
    {
        AfxV4dMix(t->oq, a->oq, b->oq, time);
        AfxQuatNormalize(t->oq, t->oq);
    }

    if (!(flags & afxTransformFlag_S)) AfxM3dReset(t->ssm);
    else
    {
        AfxM3dMix(t->ssm, a->ssm, b->ssm, time);
    }
}

_AFXINL afxBool AfxInvertTransform(afxTransform const* in, afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);

    afxTransformFlags flags = in->flags & (afxTransformFlag_RIGID | afxTransformFlag_S);

    if (!flags)
    {
        AfxResetTransform(t);
        return FALSE;
    }

    afxQuat q;
    if (flags & afxTransformFlag_R)
        AfxQuatConj(q, in->oq);
    else
        AfxQuatReset(q);
    
    if (!(flags & (afxTransformFlag_T | afxTransformFlag_S)))
        return AfxMakeRigidTransform(t, NIL, q, TRUE);

    afxM3d iqm, ss, tmp;
    AfxM3dRotationQuat(iqm, q);
    AfxM3dInvert(ss, in->ssm);
    AfxM3dMultiply(tmp, ss, iqm);
    TransposeMatrixMultiply3x3(ss, iqm, tmp);

    afxV4d ip, ip2, ip3;
    AfxV3dNeg(ip, in->pv);
    AfxV3dPostMultiplyM3d(ip2, ss, ip);
    AfxV3dPostMultiplyM3d(ip3, iqm, ip2);
    return AfxMakeTransform(t, ip3, q, ss, TRUE);
}

_AFXINL afxDof AfxV3dEnforceDofs(afxV3d pos, afxDof allowedDofs)
{
    afxError err = { 0 };
    AFX_ASSERT(pos);
    allowedDofs = (allowedDofs & afxDof_T);

    if (!allowedDofs) AfxResetV4d(pos);
    else
    {
        switch (allowedDofs)
        {
        case afxDof_T_XY:
            //pos[0] = 0;
            //pos[1] = 0;
            pos[2] = 0;
            break;
        case afxDof_T_XZ:
            //pos[0] = 0;
            pos[1] = 0;
            //pos[2] = 0;
            break;
        case afxDof_T_YZ:
            pos[0] = 0;
            //pos[1] = 0;
            //pos[2] = 0;
            break;
        case afxDof_T_X:
            //pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0;
            break;
        case afxDof_T_Y:
            pos[0] = 0;
            //pos[1] = 0;
            pos[2] = 0;
            break;
        case afxDof_T_Z:
            pos[0] = 0;
            pos[1] = 0;
            //pos[2] = 0;
            break;
        default: break;
        }
    }
    return allowedDofs;
}

_AFXINL afxDof AfxQuatEnforceDofs(afxQuat rot, afxDof allowedDofs)
{
    afxError err = { 0 };
    AFX_ASSERT(rot);
    allowedDofs = (allowedDofs & afxDof_R);

    if (!allowedDofs) AfxQuatReset(rot);
    else
    {
        switch (allowedDofs)
        {
        case afxDof_R_XY:
            //rot[0] = 0;
            //rot[1] = 0;
            rot[2] = 0;
            break;
        case afxDof_R_XZ:
            //rot[0] = 0;
            rot[1] = 0;
            //rot[2] = 0;
            break;
        case afxDof_R_YZ:
            rot[0] = 0;
            //rot[1] = 0;
            //rot[2] = 0;
            break;
        case afxDof_R_X:
            //rot[0] = 0;
            rot[1] = 0;
            rot[2] = 0;
            break;
        case afxDof_R_Y:
            rot[0] = 0;
            //rot[1] = 0;
            rot[2] = 0;
            break;
        case afxDof_R_Z:
            rot[0] = 0;
            rot[1] = 0;
            //rot[2] = 0;
            break;        
        default: break;
        }
        AfxQuatNormalize(rot, rot);
    }
    return allowedDofs;
}

_AFXINL afxDof AfxM3dEnforceDofs(afxM3d ssm, afxDof allowedDofs)
{
    afxError err = { 0 };
    AFX_ASSERT(ssm);
    allowedDofs = (allowedDofs & afxDof_S);

    if (!allowedDofs) AfxM3dReset(ssm);
    else
    {
        switch (allowedDofs)
        {
        case afxDof_S_XY:
            //AfxV3dSet(ssm[0], 1, 0, 0);
            //AfxV3dSet(ssm[1], 0, 1, 0);
            AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        case afxDof_S_XZ:
            //AfxV3dSet(ssm[0], 1, 0, 0);
            AfxV3dSet(ssm[1], 0, 1, 0);
            //AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        case afxDof_S_YZ:
            AfxV3dSet(ssm[0], 1, 0, 0);
            //AfxV3dSet(ssm[1], 0, 1, 0);
            //AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        case afxDof_S_X:
            //AfxV3dSet(ssm[0], 1, 0, 0);
            AfxV3dSet(ssm[1], 0, 1, 0);
            AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        case afxDof_S_Y:
            AfxV3dSet(ssm[0], 1, 0, 0);
            //AfxV3dSet(ssm[1], 0, 1, 0);
            AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        case afxDof_S_Z:
            AfxV3dSet(ssm[0], 1, 0, 0);
            AfxV3dSet(ssm[1], 0, 1, 0);
            //AfxV3dSet(ssm[2], 0, 0, 1);
            break;
        default: break;
        }
    }
    return allowedDofs;
}

_AFXINL void AfxEnforceTransformDofs(afxTransform* t, afxDof allowedDofs)
{
    // Should be compatible with void ClipTransformDOFs(transform *Result, unsigned int AllowedDOFs)

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(allowedDofs);

    if (!(AfxQuatEnforceDofs(t->oq, allowedDofs) & afxDof_R))
        t->flags &= ~afxTransformFlag_R;

    if (!(AfxV3dEnforceDofs(t->pv, allowedDofs) & afxDof_T))
        t->flags &= ~afxTransformFlag_T;

    if (!(AfxM3dEnforceDofs(t->ssm, allowedDofs) & afxDof_S))
        t->flags &= ~afxTransformFlag_S;
}

_AFX void AfxComputeCompositeTransformM4d(afxTransform const* t, afxM4d m)
{
    // Should be compatible with void BuildCompositeTransform4x4

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(m);

    if (t->flags & afxTransformFlag_S)
    {
        afxM3d tmp, tmp2;
        AfxM3dRotationQuat(tmp2, t->oq);
        AfxM3dMultiply(tmp, tmp2, t->ssm);
        AfxM4dTransposeM3d(m, tmp, t->pv);
    }
    else
    {
        afxM3d tmp;
        AfxM3dRotationQuat(tmp, t->oq);
        AfxM4dTransposeM3d(m, tmp, t->pv);
    }
}

_AFX void AfxComputeCompositeTransformM4dc(afxTransform const* t, afxM4r m)
{
    // Should be compatible with void BuildCompositeTransform4x3

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(m);

    if (t->flags & afxTransformFlag_S)
    {
        afxM3d tmp;
        AfxM3dRotationQuat(tmp, t->oq);
        AfxM3dMultiply(m, tmp, t->ssm);
        AfxV3dCopy(m[3], t->pv);
    }
    else
    {
        AfxM3dRotationQuat(m, t->oq);
        AfxV3dCopy(m[3], t->pv);
    }
}

_AFXINL void AfxTransformAtv3d(afxTransform const* t, afxV3d const in, afxV3d out)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(t->ssm, 1, in, out);
    AfxQuatRotateV3dArray(t->oq, 1, out, out);
    AfxV3dAdd(out, t->pv, out);
}

_AFXINL void AfxTransformArrayedAtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(t->ssm, cnt, in, out);
    AfxQuatRotateV3dArray(t->oq, cnt, out, out);

    for (afxUnit i = 0; i < cnt; i++)
        AfxV3dAdd(out[i], t->pv, out[i]);
}

_AFXINL void AfxTransformLtv3d(afxTransform const* t, afxV3d const in, afxV3d out)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlace()

    AfxM3dPostMultiplyV3d(t->ssm, 1, in, out);
    AfxQuatRotateV3dArray(t->oq, 1, out, out);
}

_AFXINL void AfxTransformArrayedLtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlace()

    AfxM3dPostMultiplyV3d(t->ssm, cnt, in, out);
    AfxQuatRotateV3dArray(t->oq, cnt, out, out);
}

_AFXINL void AfxTransformLtv3dTransposed(afxTransform const* t, afxV3d const in, afxV3d out)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq;
    AfxQuatConj(iq, t->oq);
    AfxQuatRotateV3dArray(iq, 1, in, out);
    AfxM3dPreMultiplyV3d(t->ssm, 1, out, out);
}

_AFXINL void AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq;
    AfxQuatConj(iq, t->oq);
    AfxQuatRotateV3dArray(iq, cnt, in, out);
    AfxM3dPreMultiplyV3d(t->ssm, cnt, out, out);
}

static inline void BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
}

static inline void BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    AfxM4dCopy(ResultWorldMatrix, ParentMatrix);
    AfxV3dPreMultiplyAtm4d(ResultWorldMatrix[3], Position, ParentMatrix);
}

static inline void BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxM4d m;
    AfxComputeCompositeTransformM4d(t, m);
    AfxM4dMultiplyAtm(ResultWorldMatrix, m, ParentMatrix);
}

static inline void BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4d ResultComposite)
{
    AfxM4dMultiplyAtm(ResultComposite, InverseWorld4x4, WorldMatrix);
}

static inline void BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix, afxM4r ResultComposite3x4)
{
    //ColumnMatrixMultiply4x3Transpose(ResultComposite3x4, InverseWorld4x4, WorldMatrix);
    // 0 4 8 12
    ResultComposite3x4[0][0] = InverseWorld4x4[0][0] * WorldMatrix[0][0] + InverseWorld4x4[0][1] * WorldMatrix[1][0] + InverseWorld4x4[0][2] * WorldMatrix[2][0];
    ResultComposite3x4[0][1] = InverseWorld4x4[1][0] * WorldMatrix[0][0] + InverseWorld4x4[1][1] * WorldMatrix[1][0] + InverseWorld4x4[1][2] * WorldMatrix[2][0];
    ResultComposite3x4[0][2] = InverseWorld4x4[2][0] * WorldMatrix[0][0] + InverseWorld4x4[2][1] * WorldMatrix[1][0] + InverseWorld4x4[2][2] * WorldMatrix[2][0];

    ResultComposite3x4[1][0] = InverseWorld4x4[3][2] * WorldMatrix[2][0] + InverseWorld4x4[3][1] * WorldMatrix[1][0] + InverseWorld4x4[3][0] * WorldMatrix[0][0] + WorldMatrix[3][0];
    ResultComposite3x4[1][1] = InverseWorld4x4[0][0] * WorldMatrix[0][1] + InverseWorld4x4[0][1] * WorldMatrix[1][1] + InverseWorld4x4[0][2] * WorldMatrix[2][1];
    ResultComposite3x4[1][2] = InverseWorld4x4[1][2] * WorldMatrix[2][1] + InverseWorld4x4[1][0] * WorldMatrix[0][1] + InverseWorld4x4[1][1] * WorldMatrix[1][1];
    
    ResultComposite3x4[2][0] = InverseWorld4x4[2][1] * WorldMatrix[1][1] + InverseWorld4x4[2][2] * WorldMatrix[2][1] + InverseWorld4x4[2][0] * WorldMatrix[0][1];
    ResultComposite3x4[2][1] = InverseWorld4x4[3][1] * WorldMatrix[1][1] + InverseWorld4x4[3][2] * WorldMatrix[2][1] + InverseWorld4x4[3][0] * WorldMatrix[0][1] + WorldMatrix[3][1];
    ResultComposite3x4[2][2] = InverseWorld4x4[0][0] * WorldMatrix[0][2] + InverseWorld4x4[0][1] * WorldMatrix[1][2] + InverseWorld4x4[0][2] * WorldMatrix[2][2];
    
    ResultComposite3x4[3][0] = InverseWorld4x4[1][2] * WorldMatrix[2][2] + InverseWorld4x4[1][0] * WorldMatrix[0][2] + InverseWorld4x4[1][1] * WorldMatrix[1][2];
    ResultComposite3x4[3][1] = InverseWorld4x4[2][1] * WorldMatrix[1][2] + InverseWorld4x4[2][2] * WorldMatrix[2][2] + InverseWorld4x4[2][0] * WorldMatrix[0][2];
    ResultComposite3x4[3][2] = InverseWorld4x4[3][1] * WorldMatrix[1][2] + InverseWorld4x4[3][2] * WorldMatrix[2][2] + InverseWorld4x4[3][0] * WorldMatrix[0][2] + WorldMatrix[3][2];
}

static inline void BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildIdentityWorldPoseOnly_Generic(ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

static inline void BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionWorldPoseOnly_Generic(Position, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

static inline void BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildFullWorldPoseOnly_Generic(t, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

static inline void BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d ResultWorldMatrix)
{
    afxTransform t = { 0 };
    t.flags = afxTransformFlag_T | afxTransformFlag_R;
    AfxQuatCopy(t.oq, Orientation);
    AfxV3dCopy(t.pv, Position);
    BuildFullWorldPoseOnly_Generic(&t, ParentMatrix, ResultWorldMatrix);
}

static inline void BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d ResultComposite, afxM4d ResultWorldMatrix)
{
    BuildPositionOrientationWorldPoseOnly_Generic(Position, Orientation, ParentMatrix, ResultWorldMatrix);
    BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix, ResultComposite);
}

static inline void BWP_Dispatch(afxTransform const* t, afxM4d const InverseWorld4x4, afxM4d Composite, afxM4d World, afxM4d const ParentWorld)
{
    afxTransformFlags flags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));

    if (!flags)
    {
        BuildIdentityWorldPoseComposite_Generic(ParentWorld, InverseWorld4x4, Composite, World);
        return;
    }

    if (flags == afxTransformFlag_T)
    {
        BuildPositionWorldPoseComposite_Generic(t->pv, ParentWorld, InverseWorld4x4, Composite, World);
    }
    else if (flags == afxTransformFlag_RIGID)
    {
        BuildPositionOrientationWorldPoseComposite_Generic(t->pv, t->oq, ParentWorld, InverseWorld4x4, Composite, World);
    }
    else if (flags & afxTransformFlag_S)
    {
        BuildFullWorldPoseComposite_Generic(t, ParentWorld, InverseWorld4x4, Composite, World);
    }
}

static inline void BWPNC_Dispatch(afxTransform const* t, afxM4d const ParentWorld, afxM4d World)
{
    afxTransformFlags flags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));

    if (!flags)
    {
        BuildIdentityWorldPoseOnly_Generic(ParentWorld, World);
        return;
    }

    if (flags == afxTransformFlag_T)
    {
        BuildPositionWorldPoseOnly_Generic(t->pv, ParentWorld, World);
    }
    else if (flags == afxTransformFlag_RIGID)
    {
        BuildPositionOrientationWorldPoseOnly_Generic(t->pv, t->oq, ParentWorld, World);
    }
    else if (flags & afxTransformFlag_S)
    {
        BuildFullWorldPoseOnly_Generic(t, ParentWorld, World);
    }
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxUnit cnt, afxTransform const in[], afxTransform out[])
{
    afxError err = { 0 };
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(atv);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxAssimilateAtv3d(ltm, atv, 1, &in[i].pv, &out[i].pv);
        AfxAssimilateQuat(ltm, iltm, 1, &in[i].oq, &out[i].oq);
        AfxAssimilateLtm3d(ltm, iltm, 1, &in[i].ssm, &out[i].ssm);
    }
}
