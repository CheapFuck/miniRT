#include "../../includes/minirt.h"


t_vector random_point_on_light(t_light light)
{
    double theta = ((double)rand() / RAND_MAX) * 2 * M_PI; // Random angle
    double r = light.radius * sqrt((double)rand() / RAND_MAX); // Random radius (square root for uniform distribution)

    t_vector random_point;
    random_point.x = light.pos.x + r * cos(theta);
    random_point.y = light.pos.y + r * sin(theta);
    random_point.z = light.pos.z;

    return random_point;
}

double compute_shadow_factor(t_vector hit_point, t_light light, t_scene *scene, int num_samples)
{
    int unblocked_rays = 0;
    int i;
    int j;

 i = 0;
while (i < num_samples) {
    t_vector light_point = random_point_on_light(light);
    t_vector shadow_ray_dir = normalize(subtract(light_point, hit_point));
    double light_distance = sqrt(dot(subtract(light_point, hit_point), subtract(light_point, hit_point)));

    // Create the shadow ray
    t_ray shadow_ray;
    shadow_ray.origin = add(hit_point, multiply_scalar(shadow_ray_dir, 1e-4)); // Offset to avoid self-intersection
    shadow_ray.direction = shadow_ray_dir;

    int in_shadow = 0;

    // Check intersection with all objects
    int j = 0;
    while (j < scene->num_spheres) {
        double t_shadow;
        if (intersect_sphere(&shadow_ray, &scene->spheres[j], &t_shadow) && t_shadow < light_distance) {
            in_shadow = 1;
            break;
        }
        j++;
    }

    if (!in_shadow)
        unblocked_rays++;

    i++;
}

    return (double)unblocked_rays / num_samples; // Fraction of rays that reach the light
}

int is_checkerboard(t_vector point, double scale)
{
    float epsilon = 1e-4;
    float grid_size = scale; // Adjust to your checkerboard square size
    float snapped_x = floor((point.x + epsilon)/ grid_size) * grid_size;
    float snapped_y = floor((point.y + epsilon)/ grid_size) * grid_size;
    float snapped_z = floor((point.z + epsilon)/ grid_size) * grid_size;
    return ((int)(snapped_x / grid_size) + 
                       (int)(snapped_y / grid_size) + 
                       (int)(snapped_z / grid_size)) % 2;
}

t_color get_checkerboard_color(t_vector point, t_color color1, t_color color2, double scale){
    if (is_checkerboard(point, scale))
    {
        return color1;
    } else
    {
        return color2;
    }
}


int is_in_shadow(t_vector hit_point, t_light light, t_scene *scene)
{
    int i;

    // Direction from the hit point to the light source
    t_vector light_dir = normalize(subtract(light.pos, hit_point));
    double light_distance = sqrt(dot(subtract(light.pos, hit_point), subtract(light.pos, hit_point)));

    // Create the shadow ray
    t_ray shadow_ray;
    shadow_ray.origin = add(hit_point, multiply_scalar(light_dir, 1e-4)); // Offset to avoid self-intersection
    shadow_ray.direction = light_dir;

    // Check intersection with all objects
i = 0;
while (i < scene->num_spheres) {
    double t_shadow;
    if (intersect_sphere(&shadow_ray, &scene->spheres[i], &t_shadow) && t_shadow < light_distance) {
        return 1; // In shadow
    }
    i++;
}

 i = 0;
while (i < scene->num_cylinders) {
    double t_shadow;
    if (intersect_cylinder(&shadow_ray, &scene->cylinders[i], &t_shadow) && t_shadow < light_distance) {
        return 1; // In shadow
    }
    i++;
}
  i = 0;
while (i < scene->num_planes) {
    double t_shadow;
    if (intersect_plane(&shadow_ray, &scene->planes[i], &t_shadow) && t_shadow < light_distance) {
        return 1; // In shadow
    }
    i++;
}
    return 0; // Not in shadow
}

t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene, int depth)
{
    int i;

    if (depth > MAX_REFLECTION_DEPTH)
        return ((t_color){0, 0, 0});
    t_color light_contribution = {0, 0, 0};

    // Ambient lighting
    light_contribution.r += 255 * scene->ambient.ratio;
    light_contribution.g += 255 * scene->ambient.ratio;
    light_contribution.b += 255 * scene->ambient.ratio;

    // View direction (from hit point to camera)
    t_vector view_dir = normalize(subtract(scene->camera.pos, hit_point));

    // Diffuse and specular lighting
   i = 0;
while (i < scene->num_lights) {
    t_light light = scene->lights[i];
    double shadow_factor = compute_shadow_factor(hit_point, light, scene, 32); // 32 samples for soft shadows

    if (shadow_factor > 0) { // Only compute lighting if not fully in shadow
        // Light direction
        t_vector light_dir = normalize(subtract(light.pos, hit_point));
        
        // Diffuse lighting
        double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness * shadow_factor;

        // Specular lighting
        t_vector reflect_dir = normalize(subtract(multiply_scalar(normal, 2.0 * dot(normal, light_dir)), light_dir));
        double specular_intensity = pow(fmax(0.0, dot(reflect_dir, view_dir)), 50) * light.brightness * shadow_factor * 1; // 50 is the shininess factor

        // Add contributions to light
        light_contribution.r += light.color.r * (diffuse_intensity + specular_intensity);
        light_contribution.g += light.color.g * (diffuse_intensity + specular_intensity);
        light_contribution.b += light.color.b * (diffuse_intensity + specular_intensity);
    }

    i++;
}

    // Combine light contribution with object color
    return combine_color(light_contribution, object_color);
}


t_vector compute_reflection(t_vector light_dir, t_vector normal)
{
    t_vector scaled_normal = multiply_scalar(normal, 2.0 * dot(normal, light_dir));
    return subtract(scaled_normal, light_dir);
}
