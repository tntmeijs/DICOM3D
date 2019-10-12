#version 460

layout(rgba32f, binding = 0) uniform image2D output_texture;
layout(binding = 0) uniform sampler3D volume_data;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

bool sphereHit(vec3 ray, vec3 position, float radius)
{
    return distance(position, ray) < radius;
}

void main()
{
    // Pixel position in texture pixels
    ivec2 pixel_position = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 image_size = imageSize(output_texture);

    // Normalized screen-space texture coordinates (transformed to the center of the viewport)
    vec2 uv = pixel_position / image_size;
    uv.x -= 0.5;
    uv.y -= 0.5;

    uv.x *= image_size.x / image_size.y;

    vec3 camera_pos = vec3(0.0, 0.0, 0.0);
    vec3 view_dir = vec3(0.0, 0.0, 1.0);
    vec3 sphere_pos = vec3(0.0, 0.0, 2.0);
    float sphere_radius = 1.5;

    // Send a ray
    vec3 ray_origin = camera_pos;

    vec3 output_color = vec3(uv, 0.0);

    for (int i = 0; i < 64; ++i)
    {
        if (sphereHit(ray_origin, sphere_pos, sphere_radius))
        {
            output_color.b = i / 64.0;
            break;
        }

        // March along view direction
        ray_origin = ray_origin + (view_dir * (i * 0.01)) + vec3(uv, 0.0);
    }

    imageStore(output_texture, pixel_position, vec4(output_color, 1.0));
}
