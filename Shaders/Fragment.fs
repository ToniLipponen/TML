#version 460 core

in vec4 vColor;
in vec2 vUV;
in flat uint vTexID;

out vec4 outColor;
uniform sampler2D uTextures[16];

void main()
{
    if(vTexID == 0)
    {
        outColor = vec4(vColor);
    }
    else if(vTexID == 1)
    {
        vec4 color = texture(uTextures[vTexID], vUV);
        if(color.a > 0.1)
        {
            outColor = vColor;
            outColor.a = color.a;
        }
        else
        {
            discard;
        }
    }
    else
    {
        outColor = texture(uTextures[vTexID], vUV);
    }
}