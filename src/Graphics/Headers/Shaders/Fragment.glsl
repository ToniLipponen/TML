/** NOT USED FOR ANYTHING. This is here just for reference. */
R"END(
#version 300 es
in mediump vec4 vColor;
in mediump vec2 vUV;
flat in uint vTexID;
flat in uint vType;
layout (location = 0) out mediump vec4 outColor;

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

mediump vec4 SampleTex(uint index)
{
    switch(index)
    {
        case 0u:  return texture(uTexture0,  vUV); break;
        case 1u:  return texture(uTexture1,  vUV); break;
        case 2u:  return texture(uTexture2,  vUV); break;
        case 3u:  return texture(uTexture3,  vUV); break;
        case 4u:  return texture(uTexture4,  vUV); break;
        case 5u:  return texture(uTexture5,  vUV); break;
        case 6u:  return texture(uTexture6,  vUV); break;
        case 7u:  return texture(uTexture7,  vUV); break;
        case 8u:  return texture(uTexture8,  vUV); break;
        case 9u:  return texture(uTexture9,  vUV); break;
        case 10u: return texture(uTexture10, vUV); break;
        case 11u: return texture(uTexture11, vUV); break;
        case 12u: return texture(uTexture12, vUV); break;
        case 13u: return texture(uTexture13, vUV); break;
        case 14u: return texture(uTexture14, vUV); break;
        case 15u: return texture(uTexture15, vUV); break;
        case 16u: return texture(uTexture16, vUV); break;
        case 17u: return texture(uTexture17, vUV); break;
        case 18u: return texture(uTexture18, vUV); break;
        case 19u: return texture(uTexture19, vUV); break;
        case 20u: return texture(uTexture20, vUV); break;
        case 21u: return texture(uTexture21, vUV); break;
        case 22u: return texture(uTexture22, vUV); break;
        case 23u: return texture(uTexture23, vUV); break;
        case 24u: return texture(uTexture24, vUV); break;
        case 25u: return texture(uTexture25, vUV); break;
        case 26u: return texture(uTexture26, vUV); break;
        case 27u: return texture(uTexture27, vUV); break;
        case 28u: return texture(uTexture28, vUV); break;
        case 29u: return texture(uTexture29, vUV); break;
        case 30u: return texture(uTexture30, vUV); break;
        case 31u: return texture(uTexture31, vUV); break;
        default: return vec4(1.0, 0.0, 0.0, 1.0); break;
    }
}

void main()
{
   switch(vType)
   {
       /// Color
       case 0u:
           outColor = vColor;
       break;

       /// Texture
       case 1u:
           outColor = SampleTex(vTexID);
           if(outColor.a < 0.01)
               discard;
       break;

       /// Text
       case 2u:
           mediump vec4 color = SampleTex(vTexID);
           mediump float alpha = color.r * vColor.a;
           if(alpha > 0.01)
           {
               outColor = vColor;
               outColor.a = alpha;
           }
           else
               discard;
       break;

       default:
       {
           discard;
       } break;
   }
}
)END";