#version 460

layout(rgba32f, binding = 0) uniform image2D output_texture;
layout(binding = 0) uniform sampler3D volume_data;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main()
{
    // Pixel position in texture pixels
    ivec2 pixel_position = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 image_size = image_size(output_texture);

    // Normalized screen-space texture coordinates (0 to 1)
    vec2 uv = pixel_position / image_size;

    imageStore(output_texture, pixel_position, vec4(1.0, uv, 1.0));
}
