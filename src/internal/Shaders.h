#pragma once


#ifndef TML_USE_GLES
std::string VERTEX_STRING =
R"END(
#version 450 core
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 UV;
layout (location = 2) in uint Color;
layout (location = 3) in uint Tex;
layout (location = 4) in uint Type;

out flat vec4  vColor;
out vec2  vUV;
out vec2 vViewSize;
out flat uint  vTexID;
out flat uint  vType;

uniform vec2 uViewSize;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uScale;

void main()
{
    const vec4 r = uView * vec4(Pos-(uViewSize*0.5), 1, 1);
    gl_Position = uProj * (uScale * vec4(r.xy + (uViewSize*0.5), 0, 1));

    vColor = vec4(
        ((Color & 0xff000000) >> 24),
        ((Color & 0x00ff0000) >> 16),
        ((Color & 0x0000ff00) >> 8),
        ((Color & 0x000000ff))) * 0.003921568;
    vUV = UV;
    vTexID = Tex;
    vType = Type;
    vViewSize = uViewSize;
}
)END";

std::string FRAGMENT_STRING =
R"END(
#version 450 core
in vec4 vColor;
in vec2 vUV;
in vec2 vViewSize;
flat in uint vTexID;
flat in uint vType;
out vec4 outColor;

uniform sampler2D uTextures[32];

mat4 bt601 = mat4(
  1.16438,  0.00000,  1.59603, -0.87079,
  1.16438, -0.39176, -0.81297,  0.52959,
  1.16438,  2.01723,  0.00000, -1.08139,
  0, 0, 0, 1
);

void main()
{
   vec4 color = vec4(0.0);
   switch(vType)
   {
       case 0:
           color = texture(uTextures[vTexID], vUV);
           if(color.r > 0.01)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 1:
           outColor = vColor;
       break;
       case 2:
           outColor = texture(uTextures[vTexID], vUV);
           if(outColor.a < 0.01)
           {
               discard;
           }
       break;
       case 3:
           color = texture(uTextures[vTexID], vUV);
           if(color.r > 0.01)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 4:
           outColor = texture(uTextures[vTexID], vUV) * bt601;
       break;
       default:
           discard;
       break;
   }
}
)END";
#else

std::string VERTEX_STRING =
R"END(
#version 310 es
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 UV;
layout (location = 2) in uint Color;
layout (location = 3) in uint Tex;
layout (location = 4) in uint Type;

out vec4  vColor;
out vec2  vUV;
out vec2 vViewSize;
out flat uint  vTexID;
out flat uint  vType;

uniform vec2 uViewSize;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uScale;

void main()
{
    vec4 r = uView * vec4(Pos-(uViewSize*0.5), 1, 1);
    gl_Position = uProj * (uScale * vec4(r.xy + (uViewSize*0.5), 0, 1));

    vColor = vec4(
        ((Color & uint(0xff000000)) >> 24),
        ((Color & uint(0x00ff0000)) >> 16),
        ((Color & uint(0x0000ff00)) >> 8),
        ((Color & uint(0x000000ff)))) * 0.003921568;
    vUV = UV;
    vTexID = Tex;
    vType = Type;
    vViewSize = uViewSize;
}
)END";

std::string FRAGMENT_STRING =
R"END(
#version 310 es
in mediump vec4 vColor;
in mediump vec2 vUV;
in mediump vec2 vViewSize;
flat in uint vTexID;
flat in uint vType;
out mediump vec4 outColor;

uniform sampler2D uTexture;

mediump mat4 bt601 = mat4(
  1.16438,  0.00000,  1.59603, -0.87079,
  1.16438, -0.39176, -0.81297,  0.52959,
  1.16438,  2.01723,  0.00000, -1.08139,
  0, 0, 0, 1
);

void main()
{
   mediump vec4 color = vec4(0.0);
   switch(vType)
   {
       case 0u:
           color = texture(uTexture, vUV);
           if(color.r > 0.01)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 1u:
           outColor = vColor;
       break;
       case 2u:
           outColor = texture(uTexture, vUV);
           if(outColor.a < 0.01)
           {
               discard;
           }
       break;
       case 3u:
           color = texture(uTexture, vUV);
           if(color.r > 0.01)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 4u:
           outColor = texture(uTexture, vUV) * bt601;
       break;
       default:
           discard;
       break;
   }
}
)END";
#endif