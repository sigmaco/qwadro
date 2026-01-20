/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   R E N D E R I Z A T I O N   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.
// This file is part of Advanced RenderWare Extensions.

#ifndef ARX_MATERIAL_DDK_H
#define ARX_MATERIAL_DDK_H

#include "qwadro/scene/arxScenario.h"

AFX_DEFINE_STRUCT(_arxDdiMtl);
AFX_DEFINE_STRUCT(_arxIddMtl);

#ifdef _ARX_MATERIAL_C

AFX_DEFINE_STRUCT(arxMaterialMap)
{
    afxString     usage; // usage 16
    arxMaterial     sub; // arxMaterial
};

#ifdef _ARX_MATERIAL_IMPL
AFX_OBJECT(_arxMaterial)
#else
AFX_OBJECT(arxMaterial)
#endif
{
    _arxIddMtl*         idd;
    _arxDdiMtl const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // Used by rendering and shading:
    // PBR-compatible parameters
    afxV3d      albedo; // base color --- (0.7, 0.7, 0.75)
    afxReal     metallic; // [ 0, 1 ] --- (ex.: metal = 1.0)
    afxReal     roughness; // [ 0, 1 ] --- (ex.: metal = 0.25)
    // Reflectivity for real-time ray-traced reflections or environment mapping
    afxReal     reflectivity; // [ 0, 1 ] --- (ex.: metal = 0.9)
    // Texture maps (albedo, normal, specular, etc.)
    afxUnit     albedoMapId;
    afxUnit     normalMapId;
    afxUnit     specularMapId;

    // Used by physics and collision systems:
    // mass per cubic meter (affects inertia, buoyancy).
    afxReal     density; // kg/m³ or unitless (ex.: metal = 7850)
    // affects motion resistance.
    afxReal     staticFriction; // [ 0, 1 ] (ex.: metal = 0.6)
    afxReal     dynFriction; // [ 0, 1 ] (ex.: metal = 0.4)
    // bounce response.
    afxReal     restitution; // [ 0, 1 ] (ex.: metal = 0.2)
    // used for deformation, penetration, and wear simulation
    afxReal     hardness; // (ex.: metal = 7.5)

    // Used by audio generation systems:
    // supports sound propagation through solids.
    afxReal     sndSpeed; // (ex.: metal = 5960)
    // Impact sound variations for different intensity ranges
    afxUnit     impactSndId[3]; // (ex.: metal = "sounds/scrape/[metal_light,metal_medium,metal_heavy].ogg")
    // Scrape / drag sounds
    afxUnit     scrapeSndId; // (ex.: metal = "sounds/scrape/metal.ogg")
    // Resonance frequency (for ringing sounds on impacts)
    afxUnit     acousticResonanceFreq; // (ex.: metal = 440)
    // Damping factor --- how fast a vibration dies out.
    afxReal     acousticDampingFactor; // (ex.: metal = 0.3)

    // Thermal
    // heat simulation
    afxReal     thermalCondutivity; // (ex.: metal = 50.2)
    afxUnit heatCapacity; // (ex.: metal = 450)
    afxReal emissivity; // (ex.: metal = 0.8)
    afxReal ThermalExpansion; // (ex.: metal = 0.00001)
    // Thermal reactivity
    afxReal meltTemperature; // (ex.: metal = 1000)
    afxReal colorChangeTempThreshold; // (ex.: metal = 500)

    // Electrical
    afxReal electricalConductivity; // (ex.: metal = 5.8e7)
    afxReal electricalResistance; // (ex.: metal = 0.005)
    // Magnetic
    afxReal magneticPermeability; // (ex.: metal = 1.26e-6)
    afxReal magneticRemanence; // (ex.: metal = 1.2)
    afxUnit curieTemperature; // (ex.: metal = 800)

    // Damage
    afxReal crackGrowthRate; // (ex.: metal = 0.01)
    afxReal fractureToughness; // (ex.: metal = 300)
    afxReal wearFactor; // (ex.: metal = 0.2)

    // Metadata (could help for batching)
    afxUnit32       hash; // hashed signature of the entire material
    afxAabb         usageField;
    afxString       urn; // 128
};

//AFX_STATIC_ASSERT(offsetof(arxMaterial, color) % AFX_MTL_ALIGN == 0, "");

#ifdef _ARX_MTD_IMPL
AFX_OBJECT(_arxMtd)
#else
AFX_OBJECT(arxMtd)
#endif
{
    _arxIddMtl*         idd;
    _arxDdiMtl const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxString   urn; // 32
    afxChain    classes;
    afxClass    mtlCls;
    avxTxd      txd;
};

#endif//_ARX_MATERIAL_C


ARX afxClassConfig const _ARX_MTL_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MTD_CLASS_CONFIG;

#endif//ARX_MATERIAL_DDK_H
