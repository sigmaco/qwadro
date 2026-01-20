
struct MatDefStruct
{
    vec3 albedo;
    float roughness;
    float metallic;
    int albedoTexIdx;
};

UNIFORM(2, 20, std140, sgl_mtl)
{
    MatDefStruct mtlInfos[512];
};
FETCH(2, 21, u_mtlMap);
