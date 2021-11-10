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

// uniform sampler2D uTextures[32];
// Ditching sampler array for separate samplers.
// There are some issues with sampler arrays.

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;
uniform sampler2D uTexture8;
uniform sampler2D uTexture9;
uniform sampler2D uTexture10;
uniform sampler2D uTexture11;
uniform sampler2D uTexture12;
uniform sampler2D uTexture13;
uniform sampler2D uTexture14;
uniform sampler2D uTexture15;
uniform sampler2D uTexture16;
uniform sampler2D uTexture17;
uniform sampler2D uTexture18;
uniform sampler2D uTexture19;
uniform sampler2D uTexture20;
uniform sampler2D uTexture21;
uniform sampler2D uTexture22;
uniform sampler2D uTexture23;
uniform sampler2D uTexture24;
uniform sampler2D uTexture25;
uniform sampler2D uTexture26;
uniform sampler2D uTexture27;
uniform sampler2D uTexture28;
uniform sampler2D uTexture29;
uniform sampler2D uTexture30;
uniform sampler2D uTexture31;

mat4 bt601 = mat4(
  1.16438,  0.00000,  1.59603, -0.87079,
  1.16438, -0.39176, -0.81297,  0.52959,
  1.16438,  2.01723,  0.00000, -1.08139,
  0, 0, 0, 1
);

vec4 SampleTex()
{
    switch(vTexID)
    {
        case 0u:  return texture(uTexture0,   vUV); break;
        case 1u:  return texture(uTexture1,   vUV); break;
        case 2u:  return texture(uTexture2,   vUV); break;
        case 3u:  return texture(uTexture3,   vUV); break;
        case 4u:  return texture(uTexture4,   vUV); break;
        case 5u:  return texture(uTexture5,   vUV); break;
        case 6u:  return texture(uTexture6,   vUV); break;
        case 7u:  return texture(uTexture7,   vUV); break;
        case 8u:  return texture(uTexture8,   vUV); break;
        case 9u:  return texture(uTexture9,   vUV); break;
        case 10u: return texture(uTexture10,  vUV); break;
        case 11u: return texture(uTexture11,  vUV); break;
        case 12u: return texture(uTexture12,  vUV); break;
        case 13u: return texture(uTexture13,  vUV); break;
        case 14u: return texture(uTexture14,  vUV); break;
        case 15u: return texture(uTexture15,  vUV); break;
        case 16u: return texture(uTexture16,  vUV); break;
        case 17u: return texture(uTexture17,  vUV); break;
        case 18u: return texture(uTexture18,  vUV); break;
        case 19u: return texture(uTexture19,  vUV); break;
        case 20u: return texture(uTexture20,  vUV); break;
        case 21u: return texture(uTexture21,  vUV); break;
        case 22u: return texture(uTexture22,  vUV); break;
        case 23u: return texture(uTexture23,  vUV); break;
        case 24u: return texture(uTexture24,  vUV); break;
        case 25u: return texture(uTexture25,  vUV); break;
        case 26u: return texture(uTexture26,  vUV); break;
        case 27u: return texture(uTexture27,  vUV); break;
        case 28u: return texture(uTexture28,  vUV); break;
        case 29u: return texture(uTexture29,  vUV); break;
        case 30u: return texture(uTexture30,  vUV); break;
        case 31u: return texture(uTexture31,  vUV); break;
        default:  return vec4(1.0, 0.0, 0.0, 1.0); break;
    }
}
void main()
{
   vec4 color = vec4(0.0);
   switch(vType)
   {
       case 1:
           outColor = vColor;
       break;
       case 2:
           outColor = SampleTex();
           if(outColor.a < 0.01)
           {
               discard;
           }
       break;
       case 0:
       case 3:
           color = SampleTex();
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
           outColor = SampleTex() * bt601;
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

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;

mediump mat4 bt601 = mat4(
  1.16438,  0.00000,  1.59603, -0.87079,
  1.16438, -0.39176, -0.81297,  0.52959,
  1.16438,  2.01723,  0.00000, -1.08139,
  0, 0, 0, 1
);

mediump vec4 SampleTex()
{
    mediump vec4 color = vec4(1.0);
    switch(vTexID)
    {
        case 0u: color = texture(uTexture0, vUV); break;;
        case 1u: color = texture(uTexture1, vUV); break;;
        case 2u: color = texture(uTexture2, vUV); break;;
        case 3u: color = texture(uTexture3, vUV); break;;
        case 4u: color = texture(uTexture4, vUV); break;;
        case 5u: color = texture(uTexture5, vUV); break;;
        case 6u: color = texture(uTexture6, vUV); break;;
        case 7u: color = texture(uTexture7, vUV); break;;
        default: color = vec4(1.0, 0.0, 0.0, 1.0); break;
    }
    return color;
}

void main()
{
   switch(vType)
   {
       case 1u:
           outColor = vColor;
       break;
       case 2u:
           outColor = SampleTex();
           if(outColor.a < 0.01)
           {
               discard;
           }
       break;
       case 0u:
       case 3u:
           mediump vec4 color = SampleTex();
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
           outColor = SampleTex() * bt601;
       break;
       default:
           discard;
       break;
   }
}
)END";
#endif