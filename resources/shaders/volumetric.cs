#version 460

layout(rgba32f, binding = 0) uniform image2D output_texture;
layout(binding = 0) uniform sampler3D volume_data;

layout(location = 0) uniform vec3 camera_position;
layout(location = 1) uniform vec3 camera_view_direction;
layout(location = 3) uniform vec3 volume_box_minimum;
layout(location = 4) uniform vec3 volume_box_maximum;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

float sdf_sphere(vec3 position, float radius)
{
    return length(position) - radius;
}

float sdf_box( vec3 p, vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

vec3 camera_target = vec3(0.0, 0.0, 0.0);
vec3 camera_up_dir = vec3(0.0, 1.0, 0.0);

vec3 camera_direction = normalize(camera_target - camera_position);
vec3 camera_right = normalize(cross(camera_up_dir, camera_position));
vec3 camera_up = cross(camera_direction, camera_right);

const int MAX_ITERATION = 100;
const float MAX_DISTANCE = 20.0;
const float EPSILON = 0.001;

void main()
{
    // Pixel position in texture pixels
    ivec2 pixel_position = ivec2(gl_GlobalInvocationID.xy);

    // Size of the output texture in pixels
    vec2 image_size = imageSize(output_texture);

    // Screen position ranges from -1 to 1
    vec2 screen_position = (2.0 * pixel_position / image_size) - 1.0;

    // Aspect ratio correction
    screen_position.x *= image_size.x / image_size.y;

    vec3 ray_direction = normalize(camera_right * screen_position.x + camera_up * screen_position.y + camera_direction);

    float total_distance = 0.0;
    vec3 position = camera_position;
    float distance = EPSILON;
    vec3 output_color = vec3(0, 0, 0);

    for (int i = 0; i < MAX_ITERATION; ++i)
    {
        if (distance < EPSILON || total_distance > MAX_DISTANCE)
        {
            break;
        }

        distance = sdf_box(position, vec3(0.5, 0.5, 0.5));
        total_distance += distance;
        position += distance * ray_direction;
    }

    if (distance < EPSILON)
    {
        vec2 eps = vec2(0.0, EPSILON);
        vec3 normal = normalize(vec3(
            sdf_box(position + eps.yxx, vec3(0.5, 0.5, 0.5)) - sdf_box(position - eps.yxx, vec3(0.5, 0.5, 0.5)),
            sdf_box(position + eps.xyx, vec3(0.5, 0.5, 0.5)) - sdf_box(position - eps.xyx, vec3(0.5, 0.5, 0.5)),
            sdf_box(position + eps.xxy, vec3(0.5, 0.5, 0.5)) - sdf_box(position - eps.xxy, vec3(0.5, 0.5, 0.5))
        ));

        float diffuse = max(0.0, dot(-ray_direction, normal));

        output_color = vec3(diffuse);
    }

    imageStore(output_texture, pixel_position, vec4(output_color, 1.0));
}
