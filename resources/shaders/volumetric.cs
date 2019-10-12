#version 460

layout(rgba32f, binding = 0) uniform image2D outputTex;
layout(binding = 0) uniform sampler3D volumeTex;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main()
{
    // Pixel position in texture pixels
    ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 imageSize = imageSize(outputTex);

    // Normalized screen-space texture coordinates (0 to 1)
    vec2 uv = pixelPos / imageSize;

    imageStore(outputTex, pixelPos, vec4(1.0, uv, 1.0));
}
