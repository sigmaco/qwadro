
// Intel issue: array without size supported only in GS.

UNIFORM(3, 30, std140, sgl_obj)
{
    mat4 m[1024];
};
FETCH(3, 31, u_biasToJntMap);


