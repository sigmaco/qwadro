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

#define _ARX_DRAW_C
#define _ARX_MATERIAL_C
#include "../scene/arxIcd.h"

_ARX afxString const* ArxGetMaterialUrn(arxMaterial mtl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    return &mtl->urn;
}

_ARX afxUnit ArxGetMaterialTextures(arxMaterial mtl, afxUnit baseIdx, afxUnit cnt, avxRaster rasters[], avxSampler samplers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(rasters);
    afxUnit rslt = 0;

    return rslt;
}

_ARX afxError ArxBindMaterialTextures(arxMaterial mtl, afxUnit baseIdx, afxUnit cnt, avxRaster rasters[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(rasters);

    return err;
}

_ARX afxError ArxBindMaterialTextureSamplers(arxMaterial mtl, afxUnit baseIdx, afxUnit cnt, avxSampler samplers[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(samplers);

    return err;
}

#if 0
_ARX void ArxReloadMaterialTexture(arxMaterial mtl, afxUri const *tex)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    avxRaster tex2 = mtl->tex;

    if (tex2)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex2);
        AfxDisposeObjects(1, (void*[]) { mtl->tex });
    }

    mtl->tex = NIL;

    if (tex)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex);

        arxScenario scio = AfxGetHost(mtl);
        AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);

        afxDrawSystem dsys = ArxGetScenarioDrawSystem(scio);

        avxRasterInfo rasi = { 0 };
        rasi.usage = avxRasterUsage_TEXTURE;

        if (AvxLoadRasters(dsys, 1, &rasi, &tex[0], NIL, &mtl->tex)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &mtl->tex);
        }
    }
}
#endif

// How to register a material template?

_ARX void ArxUpdateMaterial(arxMaterial mtl, arxMaterialInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    mtl->staticFriction = info->staticFriction;
    mtl->restitution = info->restitution;
    mtl->density = info->density;
    
    AfxV3dCopy(mtl->albedo, info->albedo);
    mtl->metallic = info->metallic;
    mtl->roughness = info->roughness;
}

_ARX afxError _ArxMtlDtorCb(arxMaterial mtl)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    AfxDeallocateString(&mtl->urn);

    return err;
}

_ARX afxError _ArxMtlCtorCb(arxMaterial mtl, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    arxMtd mtd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
    afxString const* name = !args[1] ? &(afxString const) { 0 } : AFX_CAST(afxString const*, args[1]) + invokeNo;
    
    AfxCloneString(&mtl->urn, name);

    mtl->roughness = 0;
    mtl->metallic = 0;
    AfxV3dSet(mtl->albedo, 1, 1, 1);
    mtl->albedoMapId = -1;
    
    return err;
}

_ARX afxClassConfig const _ARX_MTL_CLASS_CONFIG =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .desc = "Material",
    .fixedSiz = sizeof(AFX_OBJECT(arxMaterial)),
    .ctor = (void*)_ArxMtlCtorCb,
    .dtor = (void*)_ArxMtlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxUnit ArxEnumerateMaterials(arxMtd mtd, afxUnit first, afxUnit cnt, arxMaterial materials[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
    afxClass const* cls = &mtd->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)materials);
}

_ARX afxBool _ArxFindNamedMtlHandleCb(arxMaterial mtl, void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* ids;
        arxMaterial* handles;
        afxUnit rslt;
    } *udd2 = udd;

    afxUnit idx;
    if (AfxCompareStrings(&mtl->urn, 0, TRUE, udd2->cnt, &udd2->materials[udd2->rslt], &idx))
    {
        udd2->handles[udd2->rslt] = mtl;
        ++udd2->rslt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 0; // break iterator
    }

    return 1;
}

_ARX afxBool _ArxFindNamedMtlHandleAndIdCb(arxMaterial mtl, void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* ids;
        arxMaterial* handles;
        afxUnit rslt;
    } *udd2 = udd;

    afxUnit idx;
    if (AfxCompareStrings(&mtl->urn, 0, TRUE, udd2->cnt, &udd2->materials[udd2->rslt], &idx))
    {
        udd2->ids[udd2->rslt] = AfxGetObjectId(mtl);
        udd2->handles[udd2->rslt] = mtl;
        ++udd2->rslt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 0; // break iterator
    }

    return 1;
}

_ARX afxBool _ArxFindNamedMtlIdCb(arxMaterial mtl, void* udd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* ids;
        arxMaterial* handles;
        afxUnit rslt;
    } *udd2 = udd;

    afxUnit idx;
    if (AfxCompareStrings(&mtl->urn, 0, TRUE, udd2->cnt, &udd2->materials[udd2->rslt], &idx))
    {
        udd2->ids[idx] = AfxGetObjectId(mtl);
        ++udd2->rslt;
        --udd2->cnt;

        if (udd2->cnt == 0)
            return 0; // break iterator
    }

    return 1;
}

_ARX afxUnit ArxFindMaterials(arxMtd mtd, afxUnit cnt, afxString const materials[], afxUnit ids[], arxMaterial handles[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);

    afxClass* cls = (afxClass*)&mtd->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    afxUnit rslt = 0;

    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* ids;
        arxMaterial* handles;
        afxUnit rslt;
    }
    udd2 =
    {
        .cnt = cnt,
        .materials = materials,
        .ids = ids,
        .handles = handles,
        .rslt = 0
    };

    if (ids)
    {
        if (handles)
        {
            rslt = AfxInvokeObjects(cls, (void*)_ArxFindNamedMtlHandleAndIdCb, &udd2, 0, AFX_U32_MAX);
        }
        else
        {
            rslt = AfxInvokeObjects(cls, (void*)_ArxFindNamedMtlIdCb, &udd2, 0, AFX_U32_MAX);
        }
    }
    else
    {
        if (handles)
        {
            rslt = AfxInvokeObjects(cls, (void*)_ArxFindNamedMtlHandleCb, &udd2, 0, AFX_U32_MAX);
        }
        else
        {
            AFX_ASSERT(ids || handles);
            AfxThrowError();
        }
    }

    return udd2.rslt;
}

_ARX afxError ArxDeclareMaterials(arxMtd mtd, afxUnit cnt, afxString const names[], arxMaterial materials[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);
    AFX_ASSERT(materials);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)&mtd->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)materials, (void const*[]) { mtd, names }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_ARX afxError _ArxMtdDtorCb(arxMtd mtd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);

    AfxDismountClass(&mtd->mtlCls);

    AfxDeallocateString(&mtd->urn);

    return err;
}

_ARX afxError _ArxMtdCtorCb(arxMtd mtd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtd);

    arxScenario scio = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    afxString const* name = !args[1] ? &(afxString const) { 0 } : AFX_CAST(afxString const*, args[1]) + invokeNo;
    //AFX_ASSERT(args[2]);
    arxMaterialLayout const* layout = AFX_CAST(arxMaterialLayout const*, args[2]) + invokeNo;

    AfxCloneString(&mtd->urn, name);

    AfxMakeChain(&mtd->classes, mtd);
    afxClassConfig mtlClsCfg = /*cfg->mtlClsCfg ? *cfg->mtlClsCfg :*/ _ARX_MTL_CLASS_CONFIG;
    AFX_ASSERT(mtlClsCfg.fcc == afxFcc_MTL);
    AfxMountClass(&mtd->mtlCls, NIL, &mtd->classes, &mtlClsCfg); // require tex

    return err;
}

_ARX afxClassConfig const _ARX_MTD_CLASS_CONFIG =
{
    .fcc = afxFcc_MTL,
    .name = "Mtd",
    .desc = "Material Template Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(arxMtd)),
    .ctor = (void*)_ArxMtdCtorCb,
    .dtor = (void*)_ArxMtdDtorCb
};

_ARX afxError ArxAcquireMtds(arxScenario scio, afxUnit cnt, afxString const names[], arxMaterialLayout const layouts[], arxMtd mtds[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(mtds);
    AFX_ASSERT(names);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxScioGetMtdClass(scio);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)mtds, (void const*[]) { scio, names, layouts }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}
