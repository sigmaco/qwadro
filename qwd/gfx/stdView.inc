
UNIFORM(0, 0, std140, sgl_view)
{
    mat4 p;
    mat4 ip;
    mat4 vp;
    mat4 ivp;
    mat4 v;
    mat4 iv;
    
    vec4 viewPos; // view point
    vec2 viewExtent;

    vec3 starDir;
    vec4 starPos;
    vec4 starKd;
};
