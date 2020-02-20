#version 460

layout(rgba32f, binding = 0) uniform image2D output_texture;
layout(binding = 0) uniform sampler3D volume_data;

layout(location = 0) uniform vec3 camera_position;
layout(location = 1) uniform vec3 camera_view_direction;
layout(location = 3) uniform vec3 volume_box_minimum;
layout(location = 4) uniform vec3 volume_box_maximum;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

bool IntersectBox(vec3 orig, vec3 dir, vec3 box_max, vec3 box_min, out vec3 tmin, out vec3 tmax)
{
   tmin = (box_min - orig)/dir;
   tmax = (box_max - orig)/dir;
   
   vec3 real_min = min(tmin, tmax);
   vec3 real_max = max(tmin, tmax);
   
   float minmax = min( min(real_max.x, real_max.y), real_max.z);
   float maxmin = max( max(real_min.x, real_min.y), real_min.z);

   return (minmax >= maxmin);
}

float sdTorus(vec3 p, vec2 t)
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

vec3 MarchVolume(vec3 ray_origin, vec3 ray_direction)
{
    vec3 color = vec3(0.0, 0.0, 0.0);
    float distance = 0.0;

    const float MAX_DIST = 1500.0;
    const float STEP_SIZE = 0.1;

    while (distance < MAX_DIST)
    {
        vec3 ray_end = ray_origin + (ray_direction * distance);

        color.r = sdTorus(ray_end, vec2(100.0, 25)) / MAX_DIST;

        if (sdTorus(ray_end, vec2(100.0, 25.0)) < 0.001)
        {
            // Hit!
            color.g = distance / MAX_DIST * 0.5f;
            color.b = distance / MAX_DIST * 0.5f;
            break;
        }

        // March along the ray
        distance += STEP_SIZE;
    }

    return color;
}

void main()
{
    // Pixel position in texture pixels
    ivec2 pixel_position = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 image_size = imageSize(output_texture);

    // Normalized screen-space texture coordinates (transformed to the center of the viewport and aspect ratio correction)
    vec2 uv = ((pixel_position / image_size).xy - 0.5);
    uv.x *= image_size.x / image_size.y;

    // Send a ray
    vec3 ray_origin = camera_position;
    vec3 ray_direction = normalize(camera_view_direction + vec3(uv, 0.0));

    // Pixel color
    vec3 output_color = vec3(0.0, 0.0, 0.0);

    // Ray-box intersection entry
    vec3 ray_entry;

    // Ray-box intersection exit
    vec3 ray_exit;

    // Only march when the ray is actually going to hit the volume
    if (IntersectBox(ray_origin, ray_direction, volume_box_minimum, volume_box_maximum, ray_entry, ray_exit))
    {
        // Start marching right at the start of the box

        // Start marching along the ray
        output_color = MarchVolume(ray_origin, ray_direction);

        // For debugging purposes, visualize the box in which the volume will be rendered
        output_color.rgb += vec3(0.15f, 0.15f, 0.15f);
    }

    imageStore(output_texture, pixel_position, vec4(output_color, 1.0));
}
