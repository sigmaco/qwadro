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

#define _AFX_DRAW_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_MATERIAL_C
#define _ARX_SKELETON_C
#define _ARX_SCENARIO_C
#include "../scene/arxIcd.h"
#include "../qwadro_afx/targa/avxIcd.h"

afxChar const mtlVshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Vertex shader. Basic pass-through.

#include <../gfx/stdView.inc>
#include <../gfx/stdObject.inc>

PUSH(pushes)
{
    uint in_jntMtxStackBase; // per body
    uint in_biasToJntMapBase; // per rig, start index into jntMap
    uint in_mtlStackBase;
    uint in_mtlMapBase;
};

IN(0, vec3, aPos);
IN(3, vec3, aNormal);
IN(6, vec2, aTexCoord);
OUT(0, vec3, fPos);
OUT(1, vec3, fNormal);
OUT(2, vec2, fTexCoord);

void main()
{
    uint jntIdx = uint(texelFetch(u_biasToJntMap, int(in_biasToJntMapBase + 0)).r);
    mat4 m0 = m[in_jntMtxStackBase + jntIdx];
    
    fPos = vec3(m0 * vec4(aPos, 1.0));
    fNormal = mat3(transpose(inverse(m0))) * aNormal;
    fTexCoord = aTexCoord;
    
    gl_Position = p * v * vec4(fPos, 1.0);
}
)";

afxChar const mtlFshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Fragment Shader. PBR lighting with support for both solid color and texture.

#include <../gfx/stdView.inc>
#include <../gfx/stdMaterial.inc>
#include <../gfx/stdObject.inc>

PUSH(pushes)
{
    uint in_jntMtxStackBase; // per body
    uint in_biasToJntMapBase; // per rig, start index into jntMap
    uint in_mtlStackBase;
    uint in_mtlMapBase;
};

#if 0
// Lighting
UNIFORM(1, 10, std140, Lighting)
{
    vec3 lightDir; // Directional light (normalized)
    vec3 lightColor;
    //vec3 viewPos;
};
#endif

TEXTURE(2, 2, sampler2D, albedoMap);
TEXTURE(2, 3, sampler2D, mtlTextures[]);

//const float PI = 3.14159265359;

// Normal distribution function (GGX)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}

// Fresnel term (Schlick's approximation)
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Geometry function (Smith)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

IN(0, vec3, fPos);
IN(1, vec3, fNormal);
IN(2, vec2, fTexCoord);
OUT(0, vec4, FragColor);

void main()
{
    vec3 lightDir = vec3(-1, 1, -1);
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    
    vec3 N = normalize(fNormal);
    vec3 V = normalize(vec3(viewPos) - fPos);
    vec3 L = normalize(-lightDir); // light direction toward surface
    vec3 H = normalize(V + L);
    
    uint mtlIdx = uint(texelFetch(u_mtlMap, int(in_mtlMapBase)).r);
    MatDefStruct mtl = mtlInfos[in_mtlStackBase + mtlIdx];
    int albedoTexIdx = mtl.albedoTexIdx;
    
    vec3 baseColor = /*(albedoTexIdx != -1) ? vec3(texture(mtlTextures[albedoTexIdx], fTexCoord).rgb) :*/ mtl.albedo;
    float perceptualRoughness = clamp(mtl.roughness, 0.04, 1.0);

    // Reflectance at normal incidence
    vec3 F0 = mix(vec3(0.04), baseColor, mtl.metallic);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, perceptualRoughness);
    float G = GeometrySmith(N, V, L, perceptualRoughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    // kS is specular, kD is diffuse
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - mtl.metallic;

    float NdotL = max(dot(N, L), 0.0);
    vec3 irradiance = lightColor * NdotL;

    vec3 Lo = (kD * baseColor / PI + specular) * irradiance;

    // Simple ambient approximation
    vec3 ambient = vec3(0.03) * baseColor;

    vec3 color = ambient + Lo;

    // HDR tonemapping (optional)
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // gamma correction

    FragColor = vec4(color, 1.0);
}
)";

avxPipelineConfig const mtlPipc =
{
    .cullMode = avxCullMode_BACK,
    .primTop = avxTopology_TRI_LIST,
    .fillMode = avxFillMode_FACE,
    .depthTestEnabled = TRUE,
    .depthCompareOp = avxCompareOp_LESS
};
