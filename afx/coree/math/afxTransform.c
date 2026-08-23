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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

/*
    A rigid transform (also called a rigid body transformation) preserves distances and angles. 
    That means it can only include:

        Translation
        Rotation
        (Optionally) Reflection

    It cannot include:

        Scaling (changes size; distances no longer preserved)
        Shearing (changes angles; angles no longer preserved)

    Mathematically, in 2D or 3D a rigid transform has the form:

        x' = Rx + t
    
    Where:
        R is an orthogonal matrix ((R^T) R = I )
        t is a translation vector

    If scaling were present, R would no longer be orthogonal.
    If shearing were present, angles would not be preserved.

    If a transformation includes scaling but still preserves angles, 
    it's called a similarity transform, not a rigid transform.
*/

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/afxStream.h"

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
    return (srcRigidFlags || dstRigidFlags); // if has diff
}

_AFXINL afxBool AfxCopyRigidTransform(afxTransform *t, afxTransform const* in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(in);
    afxTransformFlags dstRigidFlags = (t->flags & afxTransformFlag_RIGID);
    afxTransformFlags srcRigidFlags = (in->flags & afxTransformFlag_RIGID);
    t->flags |= srcRigidFlags;
    t->pv = in->pv;
    t->oq = in->oq;
    //AfxM3dReset(t->ssm);
    return (srcRigidFlags || dstRigidFlags); // if has diff
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
    t->pv = pv;
    t->oq = oq;
    t->ssm = ssm;

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

    return (srcFlags || t->flags); // if has diff
}

_AFXINL afxBool AfxMakeRigidTransform(afxTransform* t, afxV3d const pos, afxQuat const orient, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    afxTransformFlags srcFlags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    t->flags = NIL;

    //if (!pos) AfxV3dZero(t->pv);
    //else
    {
        t->pv = pos;

        if (!check || !AfxV3dIsZero(pos))
            t->flags |= afxTransformFlag_T;
    }

    //if (!orient) AfxQuatReset(t->oq);
    //else
    {
        t->oq = orient;

        if (!check || !AfxQuatIsIdentity(orient))
            t->flags |= afxTransformFlag_R;
    }

    t->ssm = AFX_M3D_IDENTITY;

    return (srcFlags || t->flags); // if has diff
}

_AFXINL afxBool AfxMakeRigidTransformScaled(afxTransform* t, afxV3d const pos, afxQuat const orient, afxV3d const scale, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    afxTransformFlags srcFlags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));
    t->flags = NIL;

    AfxSetPosition(t, pos, check);
    AfxSetOrientation(t, orient, check);
    AfxSetScale(t, scale, check);

    return (srcFlags || t->flags); // if has diff
}

_AFXINL void AfxResetOrientation(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    t->oq = AfxQuatIdentity();
    t->flags &= ~afxTransformFlag_R;
}

_AFXINL afxBool AfxSetOrientation(afxTransform* t, afxQuat const orient, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
#if 0
    if (!orient)
    {
        AfxQuatReset(t->oq);
        t->flags &= ~afxTransformFlag_R;
    }
    else
#endif
    {
        t->oq = orient;

        if (!check || !AfxQuatIsIdentity(orient))
        {
            t->flags |= afxTransformFlag_R;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL void AfxResetPosition(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    t->pv = AfxV3dZero();
    t->flags &= ~afxTransformFlag_T;
}

_AFXINL afxBool AfxSetPosition(afxTransform* t, afxV3d const pos, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
#if 0
    if (!pos)
    {
        AfxV3dZero(t->pv);
        t->flags &= ~afxTransformFlag_T;
    }
    else
#endif
    {
        t->pv = pos;

        if (!check || !AfxV3dIsZero(pos))
        {
            t->flags |= afxTransformFlag_T;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL void AfxResetScale(afxTransform* t)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    t->ssm = AfxM3dIdentity();
    t->flags &= ~afxTransformFlag_S;
}

_AFXINL afxBool AfxSetScale(afxTransform* t, afxV3d const scale, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
#if 0
    if (!scale)
    {
        AfxM3dReset(t->ssm);
        t->flags &= ~afxTransformFlag_S;
    }
    else
#endif
    {
        t->ssm = AfxM3dScaling(scale);

        if (!check || !AfxM3dIsIdentity(t->ssm))
        {
            t->flags |= afxTransformFlag_S;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL afxBool AfxSetScaleShearing(afxTransform* t, afxM3d const ssm, afxBool check)
{
    afxError err = { 0 };
    AFX_ASSERT(t);
#if 0
    if (!ssm)
    {
        AfxM3dReset(t->ssm);
        t->flags &= ~afxTransformFlag_S;
    }
    else
#endif
    {
        t->ssm = ssm;

        if (!check || !(AfxM3dIsIdentity(ssm)))
        {
            t->flags |= afxTransformFlag_S;
            return TRUE;
        }
    }
    return FALSE;
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

    afxM3d am = AfxM3dRotationQuat(a->oq);
    afxM3d bm = AfxM3dRotationQuat(b->oq);
    t->ssm = AfxM3dMultiply(bm, b->ssm);
    afxM3d tmp = AfxM3dMultiply(a->ssm, t->ssm);
    t->ssm = TransposeMatrixMultiply3x3(t->ssm, bm, tmp);
    afxV3d pos = AfxV3dPostMultiplyM3d(a->ssm, b->pv);
    t->pv = AfxV3dPostMultiplyM3d(am, pos);
    t->pv = AfxV3dAdd(a->pv, t->pv);
    t->oq = AfxQuatMultiply(a->oq, b->oq);
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

    if (!(flags & afxTransformFlag_T)) t->pv = AfxV3dZero();
    else
    {
        t->pv = AfxV3dMix(a->pv, b->pv, time);
    }

    if (!(flags & afxTransformFlag_R)) t->oq = AfxQuatIdentity();
    else
    {
        t->oq = AfxV4dMix(a->oq, b->oq, time);
        t->oq = AfxQuatNormalize(t->oq, NIL);
    }

    if (!(flags & afxTransformFlag_S)) t->ssm = AfxM3dIdentity();
    else
    {
        t->ssm = AfxM3dMix(a->ssm, b->ssm, time);
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
        q = AfxQuatConj(in->oq);
    else
        q = AfxQuatIdentity();
    
    if (!(flags & (afxTransformFlag_T | afxTransformFlag_S)))
        return AfxMakeRigidTransform(t, AFX_V3D_ZERO, q, TRUE);

    afxM3d iqm = AfxM3dRotationQuat(q);
    afxM3d ss = AfxM3dInvert(in->ssm, NIL);
    afxM3d tmp = AfxM3dMultiply(ss, iqm);
    ss = TransposeMatrixMultiply3x3(ss, iqm, tmp);

    afxV3d ip = AfxV3dNeg(in->pv);
    afxV3d ip2 = AfxV3dPostMultiplyM3d(ss, ip);
    afxV3d ip3 = AfxV3dPostMultiplyM3d(iqm, ip2);
    return AfxMakeTransform(t, ip3, q, ss, TRUE);
}

_AFXINL afxV3d AfxV3dEnforceDofs(afxV3d const pv, afxDof allowedDofs)
{
    afxError err = { 0 };

    allowedDofs = (allowedDofs & afxDof_T);

    if (!allowedDofs)
        return AFX_V3D_ZERO;

    afxV3d pos = pv;

    switch (allowedDofs)
    {
    case afxDof_T_XY:
        //pos.v[0] = 0;
        //pos.v[1] = 0;
        pos.v[2] = 0;
        break;
    case afxDof_T_XZ:
        //pos.v[0] = 0;
        pos.v[1] = 0;
        //pos.v[2] = 0;
        break;
    case afxDof_T_YZ:
        pos.v[0] = 0;
        //pos.v[1] = 0;
        //pos.v[2] = 0;
        break;
    case afxDof_T_X:
        //pos.v[0] = 0;
        pos.v[1] = 0;
        pos.v[2] = 0;
        break;
    case afxDof_T_Y:
        pos.v[0] = 0;
        //pos.v[1] = 0;
        pos.v[2] = 0;
        break;
    case afxDof_T_Z:
        pos.v[0] = 0;
        pos.v[1] = 0;
        //pos.v[2] = 0;
        break;
    default: pos = pv; break;
    }
    return pos;
}

_AFXINL afxQuat AfxQuatEnforceDofs(afxQuat const oq, afxDof allowedDofs)
{
    afxError err = { 0 };

    allowedDofs = (allowedDofs & afxDof_R);

    if (!allowedDofs)
        return AFX_QUAT_IDENTITY;
    
    afxQuat rot;

    switch (allowedDofs)
    {
    case afxDof_R_XY: rot = AfxQuatDofXY(oq); break;
    case afxDof_R_XZ: rot = AfxQuatDofXZ(oq); break;
    case afxDof_R_YZ: rot = AfxQuatDofYZ(oq); break;
    case afxDof_R_X: rot = AfxQuatDofX(oq); break;
    case afxDof_R_Y: rot = AfxQuatDofY(oq); break;
    case afxDof_R_Z: rot = AfxQuatDofZ(oq); break;
    default: rot = oq; break;
    }
    return AfxQuatNormalize(rot, NIL);
}

_AFXINL afxM3d AfxM3dEnforceDofs(afxM3d const ssm, afxDof allowedDofs)
{
    afxError err = { 0 };

    allowedDofs = (allowedDofs & afxDof_S);

    if (!allowedDofs)
        return AFX_M3D_IDENTITY;

    switch (allowedDofs)
    {
    case afxDof_S_XY: return AfxM3dDofXY(ssm);
    case afxDof_S_XZ: return AfxM3dDofXZ(ssm);
    case afxDof_S_YZ: return AfxM3dDofYZ(ssm);
    case afxDof_S_X: return AfxM3dDofX(ssm);
    case afxDof_S_Y: return AfxM3dDofY(ssm);
    case afxDof_S_Z: return AfxM3dDofZ(ssm);
    default: break;
    }
    return ssm;
}

_AFXINL void AfxEnforceTransformDofs(afxTransform* t, afxDof allowedDofs)
{
    // Should be compatible with void ClipTransformDOFs(transform *Result, unsigned int AllowedDOFs)

    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(allowedDofs);

    t->oq = AfxQuatEnforceDofs(t->oq, allowedDofs);
    if (!(allowedDofs & afxDof_R))
        t->flags &= ~afxTransformFlag_R;

    t->pv = AfxV3dEnforceDofs(t->pv, allowedDofs);
    if (!(allowedDofs & afxDof_T))
        t->flags &= ~afxTransformFlag_T;

    t->ssm = AfxM3dEnforceDofs(t->ssm, allowedDofs);
    if (!(allowedDofs & afxDof_S))
        t->flags &= ~afxTransformFlag_S;
}

_AFX afxM3d AfxComputeCompositeTransformM3d(afxTransform const* t)
{
    // Based on AfxComputeCompositeTransformM4d

    afxError err = { 0 };
    AFX_ASSERT(t);

    if (t->flags & afxTransformFlag_S)
    {
        afxM3d tmp2 = AfxM3dRotationQuat(t->oq);
        afxM3d tmp = AfxM3dMultiply(tmp2, t->ssm);
        return AfxM3dTranspose(tmp);
    }
    else
    {
        afxM3d tmp = AfxM3dRotationQuat(t->oq);
        return AfxM3dTranspose(tmp);
    }
}

_AFXINL afxM4d AfxComputeCompositeTransformM4d(afxTransform const* t)
{
    // Should be compatible with void BuildCompositeTransform4x4

    afxError err = { 0 };
    AFX_ASSERT(t);

    if (t->flags & afxTransformFlag_S)
    {
        afxM3d tmp2 = AfxM3dRotationQuat(t->oq);
        afxM3d tmp = AfxM3dMultiply(tmp2, t->ssm);
        return AfxM4dTransposeM3d(tmp, AfxV4dFromV3d(t->pv));
    }
    else
    {
        afxM3d tmp = AfxM3dRotationQuat(t->oq);
        return AfxM4dTransposeM3d(tmp, AfxV4dFromV3d(t->pv));
    }
}

_AFXINL afxM43d AfxComputeCompositeTransformM4dc(afxTransform const* t)
{
    // Should be compatible with void BuildCompositeTransform4x3

    afxError err = { 0 };
    AFX_ASSERT(t);

    if (t->flags & afxTransformFlag_S)
    {
        afxM3d tmp = AfxM3dRotationQuat(t->oq);
        afxM43d m = AfxM43dFromM3d(AfxM3dMultiply(tmp, t->ssm));
        m.w = t->pv;
        return m;
    }
    else
    {
        afxM43d m = AfxM43dFromM3d(AfxM3dRotationQuat(t->oq));
        m.w = t->pv;
        return m;
    }
}

_AFXINL afxV3d AfxTransformAtv3d(afxTransform const* t, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    afxV3d out = AfxV3dPostMultiplyM3d(t->ssm, in);
    out = AfxV3dRotate(out, t->oq);
    out = AfxV3dAdd(t->pv, out);
    return out;
}

_AFXINL void AfxTransformArrayedAtv3d(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    AfxM3dPostMultiplyV3d(t->ssm, cnt, in, out);
    AfxQuatRotateV3d(t->oq, cnt, out, out);

    for (afxUnit i = 0; i < cnt; i++)
        out[i] = AfxV3dAdd(t->pv, out[i]);
}

_AFXINL afxV3d AfxTransformLtv3d(afxTransform const* t, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    // Compatible with TransformVectorInPlace()

    afxV3d out = AfxV3dPostMultiplyM3d(t->ssm, in);
    out = AfxV3dRotate(out, t->oq);
    return out;
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
    AfxQuatRotateV3d(t->oq, cnt, out, out);
}

_AFXINL afxV3d AfxTransformLtv3dTransposed(afxTransform const* t, afxV3d const in)
{
    afxError err = { 0 };
    AFX_ASSERT(t);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq = AfxQuatConj(t->oq);
    afxV3d out = AfxV3dRotate(in, iq);
    out = AfxV3dPreMultiplyM3d(out, t->ssm);
    return out;
}

_AFXINL void AfxTransformArrayedLtv3dTransposed(afxTransform const* t, afxUnit cnt, afxV3d const in[], afxV3d out[])
{
    afxError err = { 0 };
    AFX_ASSERT(t);
    AFX_ASSERT(cnt);
    AFX_ASSERT(in);
    AFX_ASSERT(out);

    // Compatible with TransformVectorInPlaceTransposed(in/out, t)

    afxQuat iq = AfxQuatConj(t->oq);
    AfxQuatRotateV3d(iq, cnt, in, out);
    AfxM3dPreMultiplyV3d(t->ssm, cnt, out, out);
}

static inline afxM4d BuildIdentityWorldPoseOnly_Generic(afxM4d const ParentMatrix/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = ParentMatrix;
    return ResultWorldMatrix;
}

static inline afxM4d BuildPositionWorldPoseOnly_Generic(afxV3d const Position, afxM4d const ParentMatrix/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = ParentMatrix;
    ResultWorldMatrix.w = AfxV4dFromAtv3d(AfxV3dPreMultiplyAtm4d(Position, ParentMatrix));
    return ResultWorldMatrix;
}

static inline afxM4d BuildFullWorldPoseOnly_Generic(afxTransform const* t, afxM4d const ParentMatrix/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d m = AfxComputeCompositeTransformM4d(t);
    afxM4d ResultWorldMatrix = AfxM4dMultiplyAtm(m, ParentMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BuildSingleCompositeFromWorldPose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix/*, afxM4d ResultComposite*/)
{
    afxM4d ResultComposite = AfxM4dMultiplyAtm(InverseWorld4x4, WorldMatrix);
    return ResultComposite;
}

static inline afxM43d BuildSingleCompositeFromWorldPoseTranspose_Generic(afxM4d const InverseWorld4x4, afxM4d const WorldMatrix/*, afxM43d ResultComposite3x4*/)
{
    //ColumnMatrixMultiply4x3Transpose(ResultComposite3x4, InverseWorld4x4, WorldMatrix);
    // 0 4 8 12
    afxM43d ResultComposite3x4;
    ResultComposite3x4.m[0][0] = InverseWorld4x4.m[0][0] * WorldMatrix.m[0][0] + InverseWorld4x4.m[0][1] * WorldMatrix.m[1][0] + InverseWorld4x4.m[0][2] * WorldMatrix.m[2][0];
    ResultComposite3x4.m[0][1] = InverseWorld4x4.m[1][0] * WorldMatrix.m[0][0] + InverseWorld4x4.m[1][1] * WorldMatrix.m[1][0] + InverseWorld4x4.m[1][2] * WorldMatrix.m[2][0];
    ResultComposite3x4.m[0][2] = InverseWorld4x4.m[2][0] * WorldMatrix.m[0][0] + InverseWorld4x4.m[2][1] * WorldMatrix.m[1][0] + InverseWorld4x4.m[2][2] * WorldMatrix.m[2][0];

    ResultComposite3x4.m[1][0] = InverseWorld4x4.m[3][2] * WorldMatrix.m[2][0] + InverseWorld4x4.m[3][1] * WorldMatrix.m[1][0] + InverseWorld4x4.m[3][0] * WorldMatrix.m[0][0] + WorldMatrix.m[3][0];
    ResultComposite3x4.m[1][1] = InverseWorld4x4.m[0][0] * WorldMatrix.m[0][1] + InverseWorld4x4.m[0][1] * WorldMatrix.m[1][1] + InverseWorld4x4.m[0][2] * WorldMatrix.m[2][1];
    ResultComposite3x4.m[1][2] = InverseWorld4x4.m[1][2] * WorldMatrix.m[2][1] + InverseWorld4x4.m[1][0] * WorldMatrix.m[0][1] + InverseWorld4x4.m[1][1] * WorldMatrix.m[1][1];
    
    ResultComposite3x4.m[2][0] = InverseWorld4x4.m[2][1] * WorldMatrix.m[1][1] + InverseWorld4x4.m[2][2] * WorldMatrix.m[2][1] + InverseWorld4x4.m[2][0] * WorldMatrix.m[0][1];
    ResultComposite3x4.m[2][1] = InverseWorld4x4.m[3][1] * WorldMatrix.m[1][1] + InverseWorld4x4.m[3][2] * WorldMatrix.m[2][1] + InverseWorld4x4.m[3][0] * WorldMatrix.m[0][1] + WorldMatrix.m[3][1];
    ResultComposite3x4.m[2][2] = InverseWorld4x4.m[0][0] * WorldMatrix.m[0][2] + InverseWorld4x4.m[0][1] * WorldMatrix.m[1][2] + InverseWorld4x4.m[0][2] * WorldMatrix.m[2][2];
    
    ResultComposite3x4.m[3][0] = InverseWorld4x4.m[1][2] * WorldMatrix.m[2][2] + InverseWorld4x4.m[1][0] * WorldMatrix.m[0][2] + InverseWorld4x4.m[1][1] * WorldMatrix.m[1][2];
    ResultComposite3x4.m[3][1] = InverseWorld4x4.m[2][1] * WorldMatrix.m[1][2] + InverseWorld4x4.m[2][2] * WorldMatrix.m[2][2] + InverseWorld4x4.m[2][0] * WorldMatrix.m[0][2];
    ResultComposite3x4.m[3][2] = InverseWorld4x4.m[3][1] * WorldMatrix.m[1][2] + InverseWorld4x4.m[3][2] * WorldMatrix.m[2][2] + InverseWorld4x4.m[3][0] * WorldMatrix.m[0][2] + WorldMatrix.m[3][2];
    
    return ResultComposite3x4;
}

static inline afxM4d BuildIdentityWorldPoseComposite_Generic(afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d* ResultComposite/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = BuildIdentityWorldPoseOnly_Generic(ParentMatrix);
    *ResultComposite = BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BuildPositionWorldPoseComposite_Generic(afxV3d const Position, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d* ResultComposite/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = BuildPositionWorldPoseOnly_Generic(Position, ParentMatrix);
    *ResultComposite = BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BuildFullWorldPoseComposite_Generic(afxTransform const* t, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d* ResultComposite/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = BuildFullWorldPoseOnly_Generic(t, ParentMatrix);
    *ResultComposite = BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BuildPositionOrientationWorldPoseOnly_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix/*, afxM4d ResultWorldMatrix*/)
{
    afxTransform t = { 0 };
    t.flags = afxTransformFlag_T | afxTransformFlag_R;
    t.oq =  Orientation;
    t.pv = Position;
    afxM4d ResultWorldMatrix = BuildFullWorldPoseOnly_Generic(&t, ParentMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BuildPositionOrientationWorldPoseComposite_Generic(afxV3d const Position, afxQuat const Orientation, afxM4d const ParentMatrix, afxM4d const InverseWorld4x4, afxM4d* ResultComposite/*, afxM4d ResultWorldMatrix*/)
{
    afxM4d ResultWorldMatrix = BuildPositionOrientationWorldPoseOnly_Generic(Position, Orientation, ParentMatrix);
    *ResultComposite = BuildSingleCompositeFromWorldPose_Generic(InverseWorld4x4, ResultWorldMatrix);
    return ResultWorldMatrix;
}

static inline afxM4d BWP_Dispatch(afxTransform const* t, afxM4d const InverseWorld4x4, afxM4d* Composite/*, afxM4d World*/, afxM4d const ParentWorld)
{
    afxTransformFlags flags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));

    if (!flags)
    {
        return BuildIdentityWorldPoseComposite_Generic(ParentWorld, InverseWorld4x4, Composite);
    }

    if (flags == afxTransformFlag_T)
    {
        return BuildPositionWorldPoseComposite_Generic(t->pv, ParentWorld, InverseWorld4x4, Composite);
    }
    else if (flags == afxTransformFlag_RIGID)
    {
        return BuildPositionOrientationWorldPoseComposite_Generic(t->pv, t->oq, ParentWorld, InverseWorld4x4, Composite);
    }
    else if (flags & afxTransformFlag_S)
    {
        return BuildFullWorldPoseComposite_Generic(t, ParentWorld, InverseWorld4x4, Composite);
    }

    // added due to the need for return.
    return ParentWorld;
}

static inline afxM4d BWPNC_Dispatch(afxTransform const* t, afxM4d const ParentWorld)
{
    afxTransformFlags flags = (t->flags & (afxTransformFlag_RIGID | afxTransformFlag_S));

    if (!flags)
    {
        return BuildIdentityWorldPoseOnly_Generic(ParentWorld);
    }

    if (flags == afxTransformFlag_T)
    {
        return BuildPositionWorldPoseOnly_Generic(t->pv, ParentWorld);
    }
    else if (flags == afxTransformFlag_RIGID)
    {
        return BuildPositionOrientationWorldPoseOnly_Generic(t->pv, t->oq, ParentWorld);
    }
    else if (flags & afxTransformFlag_S)
    {
        return BuildFullWorldPoseOnly_Generic(t, ParentWorld);
    }

    // added due to the need for return.
    return ParentWorld;
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxAssimilateTransforms(afxM3d const ltm, afxM3d const iltm, afxV3d const atv, afxUnit cnt, afxTransform const in[], afxTransform out[])
{
    afxError err = { 0 };
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
