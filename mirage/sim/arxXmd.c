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

// This code is part of SIGMA GL/2.
// This file is part of Advanced RenderWare Extensions.

#include "../sim/asxImplementation.h"

ASX afxChar const *shdResTypeNames[];

_ASX afxError AfxAssembleModelFromXml(arxScenario scio, afxXmlNode const* xml, arxSkeleton skl, arxModel* model)
{
    afxError err = { 0 };
    
    afxString32 urn;
    arxModelBlueprint config = { 0 };
    
    afxString const mdlAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("position"),
        AFX_STRING("orientation"),
        AFX_STRING("scale"),
        AFX_STRING("lodType")
    };

    afxXmlAttr const* a = NIL;
    AFX_ITERATE_CHAIN_B2F(a, elem, &xml->attributes)
    {
        afxUnit sIdx;
        if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(mdlAttrNames), mdlAttrNames, &sIdx))
            continue;

        switch (sIdx)
        {
        case 0: // id
        {
            AfxMakeString32(&urn, &a->content);
            break;
        }
        case 1: // position
        {
            afxV3d position = { 0 };
            AfxScanString(&a->content, "%f %f %f", &position[0], &position[1], &position[2]);
            break;
        }
        case 2: // orientation
        {
            afxQuat orientation = { 0 };
            AfxScanString(&a->content, "%f %f %f %f", &orientation[0], &orientation[1], &orientation[2], &orientation[3]);
            break;
        }
        case 3: // scale
        {
            afxV3d scale = { 1, 1, 1 };
            AfxScanString(&a->content, "%f %f %f", &scale[0], &scale[1], &scale[2]);
            break;
        }
        case 4: // lodType
        {
            afxUnit lodType = 0;
            AfxScanString(&a->content, "%u", &lodType);
            break;
        }
        default:
        {
            AfxReportY("Tag '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
        }
    }

    afxString const mdlChildNames[] =
    {
        AFX_STRING("Joint"),
        AFX_STRING("Rig"),
        AFX_STRING("Attachment")
    };

    afxString const jntAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("parent"),
        AFX_STRING("position"),
        AFX_STRING("orientation"),
        AFX_STRING("scale"),
        AFX_STRING("lodError"),
        AFX_STRING("iw")
    };

    afxString const rigAttrNames[] =
    {
        AFX_STRING("uri")
    };

    afxUnit rigCnt = 0;
    afxUnit jointCnt = 0;
    afxString joints[256];
    afxTransform locals[256];
    afxUnit parents[256];
    afxReal lodErrors[256];
    afxM4r iws[256];
    afxUri meshes[256];

    afxXmlNode const* child;
    AFX_ITERATE_CHAIN_B2F(child, parent, &xml->children)
    {
        afxUnit sIdx;
        if (!AfxCompareStrings(&child->name, 0, TRUE, ARRAY_SIZE(mdlChildNames), mdlChildNames, &sIdx))
            continue;

        switch (sIdx)
        {
        case 0: // Joint
        {
            afxV3d position = { 0 };
            afxQuat orientation = { 0, 0, 0, 1 };
            afxV3d scale = { 1, 1, 1 };
            afxReal lodError = -1.0;
            afxUnit parentIdx = AFX_INVALID_INDEX;
            afxM4r iw = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } };
            afxString id = { 0 };

            AFX_ITERATE_CHAIN_B2F_B(a, elem, &xml->attributes)
            {
                afxUnit sIdx;
                if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(jntAttrNames), jntAttrNames, &sIdx))
                    continue;

                switch (sIdx)
                {
                case 0: // id
                {
                    id = a->content;
                    break;
                }
                case 1: // parentIdx
                {
                    AfxScanString(&a->content, "%u", &parentIdx);
                    break;
                }
                case 2: // position
                {
                    AfxScanString(&a->content, "%f %f %f", &position[0], &position[1], &position[2]);
                    break;
                }
                case 3: // orientation
                {
                    AfxScanString(&a->content, "%f %f %f %f", &orientation[0], &orientation[1], &orientation[2], &orientation[3]);
                    break;
                }
                case 4: // scale
                {
                    AfxScanString(&a->content, "%f %f %f", &scale[0], &scale[1], &scale[2]);
                    break;
                }
                case 5: // lodError
                {
                    AfxScanString(&a->content, "%f", &lodError);
                    break;
                }
                case 6: // iw
                {
                    AfxScanString(&a->content, "%f %f %f %f %f %f %f %f %f %f %f %f", &iw[0][0], &iw[0][1], &iw[0][2], &iw[1][0], &iw[1][1], &iw[1][2], &iw[2][0], &iw[2][1], &iw[2][2], &iw[3][0], &iw[3][1], &iw[3][2]);
                    break;
                }
                default: break;
                }
            }

            afxM3d ss;
            AfxM3dScaling(ss, scale);
            AfxMakeTransform(&locals[jointCnt], position, orientation, ss, TRUE);
            AfxM4rCopy(iws[jointCnt], iw);
            lodErrors[jointCnt] = lodError;
            parents[jointCnt] = parentIdx;
            joints[jointCnt] = id;

            ++jointCnt;
            break;
        }
        case 1: // Rig
        {
            afxUri uri = { 0 };

            AFX_ITERATE_CHAIN_B2F_B(a, elem, &xml->attributes)
            {
                afxUnit sIdx;
                if (!AfxCompareStrings(&a->name, 0, TRUE, ARRAY_SIZE(rigAttrNames), rigAttrNames, &sIdx))
                    continue;

                switch (sIdx)
                {
                case 0: // uri
                {
                    AfxWrapUriString(&uri, &a->content);
                    break;
                }
                default: break;
                }
            }

            meshes[rigCnt] = uri;
            ++rigCnt;
            break;
        }
        case 2: // Attachment
        {
            break;
        }
        default: AfxReportY("Node '%.*s' not handled.", AfxPushString(&a->name));
            break;
        }
    }

    arxModel mdl;
    ArxAssembleModels(scio, 1, &urn.s, &config, &mdl);

    

    return err;
}
