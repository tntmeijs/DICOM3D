#version 460

layout(rgba32f, binding = 0) uniform image2D outputTex;
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main()
{
    ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);

    imageStore(outputTex, pixelPos, vec4(gl_WorkGroupID.xyz, 1.0));
}
