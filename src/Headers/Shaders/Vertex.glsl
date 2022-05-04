R"END(

#version 310 es
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 UV;
layout (location = 2) in uint Color;
layout (location = 3) in uint TypeAndTex;

out vec4  vColor;
out vec2  vUV;
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
    vTexID = (TypeAndTex & uint(0x0000ffff));
    vType = (TypeAndTex & uint(0xffff0000)) >> 16;
}
)END";