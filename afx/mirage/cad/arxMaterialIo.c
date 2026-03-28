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

_ARX afxError _ArxParseXmlMtdMaterialProp(afxXmlNode const* xml, arxMtd mtd, arxMaterial mtl, afxUnit propIdx)
{
    afxError err = { 0 };
    AFX_ASSERT(mtd);

    afxString const itemDefAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("uri"),
        AFX_STRING("map"),
        AFX_STRING("vector"),
        AFX_STRING("scalar"),
        // impact
        AFX_STRING("density"),
        AFX_STRING("restitution"),
        AFX_STRING("hardness"),
        // friction
        AFX_STRING("static"),
        AFX_STRING("dynamic"),
        // damage
        AFX_STRING("crackGrowthRate"),
        AFX_STRING("fractureToughness"),
        AFX_STRING("wearFactor"),
    };

    afxString32 id = { 0 };
    afxUri128 uri = { 0 };
    afxUri128 map = { 0 };

    arxMaterialInfo mtli = { 0 };

    afxXmlAttr const* a = NIL;
    AFX_ITERATE_CHAIN_B2F(a, elem, &xml->attributes)
    {
        afxUnit sIdx;
        if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(itemDefAttrNames), itemDefAttrNames, &sIdx))
        {
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            continue;
        }

        switch (sIdx)
        {
        case 0: // id
        {
            id.s = a->content;
            break;
        }
        case 1: // uri
        {
            uri.uri.s = a->content;
            AfxReparseUri(&uri.uri);
            break;
        }
        case 2: // map
        {
            map.uri.s = a->content;
            AfxReparseUri(&map.uri);

            if (propIdx == 0)
            {

            }
            else if (propIdx == 1)
            {

            }
            else if (propIdx == 2)
            {

            }
            else if (propIdx == 3)
            {

            }
            break;
        }
        case 3: // vector
        {
            if (propIdx == 0)
            {
                afxInt cnt = AfxScanString(&a->content, "%f %f %f", &mtli.albedo[0], &mtli.albedo[1], &mtli.albedo[2]);

                if (3 != cnt)
                    AfxV3dSet(mtli.albedo, 1, 1, 1);
            }
            break;
        }
        case 4: // scalar
        {
            if (propIdx == 2)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.metallic);
                if (!cnt) mtli.metallic = 0;
            }
            else if (propIdx == 3)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.roughness);
                if (!cnt) mtli.roughness = 0;
            }
            else if (propIdx == 5)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.reflectivity);
            }
            break;
        }
        case 5: // density
        {
            if (propIdx == 6)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.density);
            }
            break;
        }
        case 6: // restitution
        {
            if (propIdx == 6)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.restitution);
            }
            break;
        }
        case 7: // hardness
        {
            if (propIdx == 6)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.hardness);
            }
            break;
        }
        case 8: // static
        {
            if (propIdx == 7)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.staticFriction);
            }
            break;
        }
        case 9: // dynamic
        {
            if (propIdx == 7)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.dynFriction);
            }
            break;
        }
        case 10: // crackGrowthRate
        {
            if (propIdx == 8)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.crackGrowthRate);
            }
            break;
        }
        case 11: // fractureToughness
        {
            if (propIdx == 8)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.fractureToughness);
            }
            break;
        }
        case 12: // wearFactor
        {
            if (propIdx == 8)
            {
                afxInt cnt = AfxScanString(&a->content, "%f", &mtli.wearFactor);
            }
            break;
        }
        default:
        {
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
        }
    }

    ArxUpdateMaterial(mtl, &mtli);

    afxString const itemDefElemNames[] =
    {
        AFX_STRING(""),
    };

    afxXmlNode const* child = NIL;
    AFX_ITERATE_CHAIN_B2F(child, parent, &xml->children)
    {
        afxUnit sIdx = AFX_INVALID_INDEX;
        if (!AfxCompareStrings(&child->name, 0, TRUE, ARRAY_SIZE(itemDefElemNames), itemDefElemNames, &sIdx))
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            continue;
        }

        switch (sIdx)
        {
        default:
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            break;
        }
        }
    }

    return err;
}

_ARX afxError _ArxParseXmlMtdMaterial(afxXmlNode const* xml, arxMtd mtd)
{
    afxError err = { 0 };
    AFX_ASSERT(mtd);

    afxString const definitionAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("base"),
    };

    afxXmlAttr const* a = NIL;
    AFX_ITERATE_CHAIN_B2F(a, elem, &xml->attributes)
    {
        afxUnit sIdx;
        if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(definitionAttrNames), definitionAttrNames, &sIdx))
        {
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            continue;
        }

        switch (sIdx)
        {
        case 0: // id
        {
            break;
        }
        case 1: // base
        {
            break;
        }
        default:
        {
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
        }
    }

    arxMaterial mtl;
    ArxDeclareMaterials(mtd, 1, &xml->content, &mtl);

    afxString const matElemNames[] =
    {
        AFX_STRING("Albedo"),
        AFX_STRING("Normal"),
        AFX_STRING("Metallic"),
        AFX_STRING("Roughness"),
        AFX_STRING("Ao"),
        AFX_STRING("Reflectivity"),
        AFX_STRING("Impact"),
        AFX_STRING("Friction"),
        AFX_STRING("Damage")
    };

    afxXmlNode const* child = NIL;
    AFX_ITERATE_CHAIN_B2F(child, parent, &xml->children)
    {
        afxUnit sIdx = AFX_INVALID_INDEX;
        if (!AfxCompareStrings(&child->name, 0, TRUE, ARRAY_SIZE(matElemNames), matElemNames, &sIdx))
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            continue;
        }

        switch (sIdx)
        {
        case 0: // Albedo
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 1: // Normal
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 2: // Metallic
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 3: // Roughness
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 4: // Ao
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 5: // Reflectivity
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 6: // Impact
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 7: // Friction
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        case 8: // Damage
        {
            _ArxParseXmlMtdMaterialProp(child, mtd, mtl, sIdx);
            break;
        }
        default:
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            break;
        }
        }
    }

    return err;
}

_ARX afxError _ArxParseXmlMtd(afxXmlNode const* xml, arxMtd mtd)
{
    afxError err = { 0 };
    AFX_ASSERT(mtd);

    afxUnit sIdx;
#if 0
    afxString const mtdAttrNames[] =
    {
        AFX_STRING(""),
    };

    afxXmlAttr const* a = NIL;
    AFX_ITERATE_CHAIN_B2F(afxXmlAttr const, a, elem, &xml->attributes)
    {
        if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(mtdAttrNames), mtdAttrNames, &sIdx))
            continue;

        switch (sIdx)
        {
        case 0: // topology
        {
            break;
        }
        default:
        {
            AfxReportY("Attribute '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
        }
    }
#endif

    afxString const mtdElemNames[] =
    {
        AFX_STRING("Material")
    };

    afxXmlNode const* child = NIL;
    AFX_ITERATE_CHAIN_B2F(child, parent, &xml->children)
    {
        if (!AfxCompareStrings(&child->name, 0, TRUE, ARRAY_SIZE(mtdElemNames), mtdElemNames, &sIdx))
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            continue;
        }

        switch (sIdx)
        {
        case 0: // definitions
        {
            if (_ArxParseXmlMtdMaterial(child, mtd))
            {
                AfxThrowError();
            }
            break;
        }
        default:
        {
            AfxReportY("Element '%.*s' not handled.", AfxPushString(&child->name));
            break;
        }
        }
    }

    return err;
}

_ARX afxError ArxLoadMtd(arxScenario scio, afxUri const* uri, arxMtd* pMtd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SCIO, 1, &scio);
    AFX_ASSERT(pMtd);
    AFX_ASSERT(uri);
    AFX_ASSERT(uri && !AfxIsUriBlank(uri));

    AfxReportMessage("Uploading MTD '%.*s'", AfxPushString(&uri->s));

    afxXml xml;
    afxString query;
    afxUri fpath, fext;
    AfxGetUriQueryString(uri, TRUE, &query);
    AfxExcerptUriExtension(&fext, uri, FALSE);
    AfxExcerptUriPath(&fpath, uri);

    if (AfxIsUriBlank(&fext)) AfxThrowError();
    else if (0 != AfxCompareString(AfxGetUriString(&fext), 0, ".xml", 4, TRUE))
    {
        AfxReportError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
        AfxThrowError();
    }
    else if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
    else
    {
        if (!AfxTestXmlRoot(&xml, &AFX_STRING("Mtd")))
        {
            AfxThrowError();
        }
        else
        {
            arxMtd mtd;
            afxUri fname;
            AfxExcerptUriFile(&fname, uri);
            if (ArxAcquireMtds(scio, 1, &fname.s, NIL, &mtd))
                AfxThrowError();

            if (_ArxParseXmlMtd(xml.root, mtd)) AfxThrowError();
            else
            {

            }

            *pMtd = mtd;
        }

        AfxCleanUpXml(&xml);
    }
    return err;
}
