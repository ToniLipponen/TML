R"END(
#version 300 es
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 UV;
layout (location = 2) in uint Color;
layout (location = 3) in uint TTI; // Type, tex, index

out vec4 vColor;
out vec2 vUV;
flat out uint vTexID;
flat out uint vType;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 r = uView * vec4(Pos, 0, 1);
    gl_Position = uProj * vec4(r.xy, 0, 1);

    vColor = vec4(
        ((Color & uint(0xff000000)) >> 24),
        ((Color & uint(0x00ff0000)) >> 16),
        ((Color & uint(0x0000ff00)) >> 8),
        ((Color & uint(0x000000ff)))) * 0.003921568;
    vUV = UV;
    vTexID = (TTI & uint(0x00ff0000)) >> 16;
    vType = (TTI & uint(0xff000000)) >> 24;
    gl_Position.z += float((TTI & uint(0x0000ff00)) >> 8) * 0.0001;
}
)END";