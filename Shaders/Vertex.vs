#version 460 core

layout (location = 0) in vec2 Pos;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 UV;
layout (location = 3) in uint TexID;
layout (location = 4) in float Rotation;
layout (location = 5) in uint type;

out vec4  vColor;
out vec2  vUV;
out uint  vTexID;
out uint  vType;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    gl_Position = uView * (uProjection * vec4(Pos, 1.0, 1.0));
    vColor = Color;
    vUV = UV;
    vTexID = TexID;
    vType = type;
}