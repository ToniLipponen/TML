#version 460 core

in vec4 vColor;
in vec2 vUV;
in flat uint vTexID;
in flat uint vType;

out vec4 outColor;
uniform sampler2D uTextures[32];

void main()
{
    vec4 color = vec4(0.0);
    switch(vType)
    {
        case 0:
            color = texture(uTextures[vTexID], vUV);
            if(color.r > 0.1)
            {
                outColor = vColor;
                outColor.a = color.r;
            }
            else
            {
                discard;
            }
        break;
        case 1:
            outColor = vec4(vColor);
        break;
        case 2:
            outColor = texture(uTextures[vTexID], vUV);
            if(outColor.a < 0.1)
            {
                discard;
            }
        break;
        case 3:
            color = texture(uTextures[vTexID], vUV);
            if(color.r > 0.2)
            {
                outColor = vColor;
                outColor.a = color.r;
            }
            else
            {
                discard;
            }
        break;
        default:
            discard;
        break;
    }
}