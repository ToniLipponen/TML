/**
 * @brief Simple vignette compute shader
 */

#pragma once

const char* shaderSource = R"END(
#version 450 core
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (rgba8, binding = 0) uniform image2D image;

float vignette(vec2 uv, float radius, float smoothness)
{
    float diff = radius - distance(uv, vec2(0.5, 0.5));
    return smoothstep(-smoothness, smoothness, diff);
}

void main(void)
{
    ivec2 uv = ivec2(gl_GlobalInvocationID.xy);
    vec2 size = imageSize(image);

    vec4 color = imageLoad(image, uv) * vignette(vec2(uv)/size, 0.3, 0.3);
    color.a = 1;
    imageStore(image, uv, color);
}
)END";