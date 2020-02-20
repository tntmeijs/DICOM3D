#version 460

layout(rgba32f, binding = 0) uniform image2D output_texture;
layout(binding = 0) uniform sampler3D volume_data;

layout(location = 0) uniform vec3 camera_position;
layout(location = 1) uniform vec3 camera_view_direction;
layout(location = 3) uniform vec3 volume_box_minimum;
layout(location = 4) uniform vec3 volume_box_maximum;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main()
{
    // Pixel position in texture pixels
    ivec2 pixel_position = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 image_size = imageSize(output_texture);

    // Normalized screen-space texture coordinates (transformed to the center of the viewport and aspect ratio correction)
    vec2 uv = ((pixel_position / image_size).xy - 0.5);
    uv.x *= image_size.x / image_size.y;

    // Just output the UV coordinates for now
    vec3 output_color = vec3(uv.xy, 0.0);

    imageStore(output_texture, pixel_position, vec4(output_color, 1.0));
}
