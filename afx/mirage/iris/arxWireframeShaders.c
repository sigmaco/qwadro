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
#include "../qwadro_afx/coree/draw/avxIcd.h"

#define AFX_R(_raw_string_literal_) "\n" #_raw_string_literal_
//#define AFX_R(_raw_string_literal_) _Pragma (#_raw_string_literal_)

afxChar const wireBaryVshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Vertex shader. Basic pass-through for barycentric-based wireframe.

#include <../gfx/stdView.inc>

PUSH(pushes)
{
    mat4 uM; // transform matrix
    vec3 uWireColor; // = vec3(1.0, 1.0, 1.0);
    float uWireThickness; // = 0.02;
    vec3 uFillColor; // = vec3(0.1, 0.1, 0.1);
};
    
IN(0, vec3, aPos);
IN(1, vec3, aBary); // barycentric coordinates
OUT(1, vec3, vBary);
    
void main()
{
    gl_Position = p * v * uM * vec4(aPos, 1.0);
    vBary = aBary;
}
)";

afxChar const wireBaryFshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Fragment Shader. Barycentric-based wireframe.

PUSH(pushes)
{
mat4 uM; // transform matrix
vec3 uWireColor; // = vec3(1.0, 1.0, 1.0);
float uWireThickness; // = 0.02;
vec3 uFillColor; // = vec3(0.1, 0.1, 0.1);
};
    
IN(1, vec3, vBary);
OUT(0, vec4, FragColor);
    
float edgeFactor()
{
    vec3 d = fwidth(vBary);
    vec3 a3 = smoothstep(vec3(0.0), d * uWireThickness, vBary);
    return min(min(a3.x, a3.y), a3.z);
}
    
void main()
{
    float e = edgeFactor();
    vec3 color = mix(uWireColor, uFillColor, e);
    FragColor = vec4(color, 1.0);
}
)";

avxPipelineConfig const wireBaryPipc =
{
    .cullMode = avxCullMode_NONE,
    .primTop = avxTopology_TRI_LIST,
    .fillMode = avxFillMode_FACE,
    .depthTestEnabled = TRUE,
    .depthCompareOp = avxCompareOp_LESS
};

afxChar const wireVshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Vertex shader. Basic pass-through for wireframe.
    
#include <../gfx/stdEnv.inc>
#include <../gfx/stdView.inc>

PUSH(pushes)
{
    mat4 uM; // transform matrix
    vec3 uWireColor; // = vec3(1.0, 1.0, 1.0);
    float uWireThickness; // = 0.02;
    vec3 uFillColor; // = vec3(0.1, 0.1, 0.1);
};
    
IN(0, vec3, aPos);
    
void main()
{
    gl_Position = p * v * uM * vec4(aPos, 1.0);
}
)";

afxChar const wireFshCode[] = R"(
// EMOTION ENGINE (c) 2017 SIGMA TECHNOLOGY GROUP
// Fragment Shader. Wireframe.
    
#include <../gfx/stdEnv.inc>
    
PUSH(pushes)
{
mat4 uM; // transform matrix
vec3 uWireColor; // = vec3(1.0, 1.0, 1.0);
float uWireThickness; // = 0.02;
vec3 uFillColor; // = vec3(0.1, 0.1, 0.1);
};

OUT(0, vec4, FragColor);

void main()
{
    vec3 color = uWireColor;
    FragColor = vec4(color, 1.0);
}
)";

avxPipelineConfig const wirePipc =
{
    .cullMode = avxCullMode_NONE,
    .primTop = avxTopology_LINE_LIST,
    .fillMode = avxFillMode_FACE,
    .depthTestEnabled = TRUE,
    .depthCompareOp = avxCompareOp_LESS
};
