#include "../../includes/minirt.h"

t_color combine_color(t_color light_color, t_color object_color) {
    t_color result;

    result.r = fmin(255, (light_color.r / 255.0) * object_color.r);
    result.g = fmin(255, (light_color.g / 255.0) * object_color.g);
    result.b = fmin(255, (light_color.b / 255.0) * object_color.b);

    return result;
}

t_vector random_point_on_light(t_light light) {
    double theta = ((double)rand() / RAND_MAX) * 2 * M_PI; // Random angle
    double r = light.radius * sqrt((double)rand() / RAND_MAX); // Random radius (square root for uniform distribution)

    t_vector random_point;
    random_point.x = light.pos.x + r * cos(theta);
    random_point.y = light.pos.y + r * sin(theta);
    random_point.z = light.pos.z;

    return random_point;
}

double compute_shadow_factor(t_vector hit_point, t_light light, t_scene *scene, int num_samples) {
    int unblocked_rays = 0;

    for (int i = 0; i < num_samples; i++) {
        t_vector light_point = random_point_on_light(light);
        t_vector shadow_ray_dir = normalize(subtract(light_point, hit_point));
        double light_distance = sqrt(dot(subtract(light_point, hit_point), subtract(light_point, hit_point)));

        // Create the shadow ray
        t_ray shadow_ray;
        shadow_ray.origin = add(hit_point, multiply_scalar(shadow_ray_dir, 1e-4)); // Offset to avoid self-intersection
        shadow_ray.direction = shadow_ray_dir;

        int in_shadow = 0;

        // Check intersection with all objects
        for (int j = 0; j < scene->num_spheres; j++) {
            double t_shadow;
            if (intersect_sphere(&shadow_ray, &scene->spheres[j], &t_shadow) && t_shadow < light_distance) {
                in_shadow = 1;
                break;
            }
        }

        if (!in_shadow)
            unblocked_rays++;
    }

    return (double)unblocked_rays / num_samples; // Fraction of rays that reach the light
}


int is_in_shadow(t_vector hit_point, t_light light, t_scene *scene) {
    // Direction from the hit point to the light source
    t_vector light_dir = normalize(subtract(light.pos, hit_point));
    double light_distance = sqrt(dot(subtract(light.pos, hit_point), subtract(light.pos, hit_point)));

    // Create the shadow ray
    t_ray shadow_ray;
    shadow_ray.origin = add(hit_point, multiply_scalar(light_dir, 1e-4)); // Offset to avoid self-intersection
    shadow_ray.direction = light_dir;

    // Check intersection with all objects
    for (int i = 0; i < scene->num_spheres; i++) {
        double t_shadow;
        if (intersect_sphere(&shadow_ray, &scene->spheres[i], &t_shadow) && t_shadow < light_distance) {
            return 1; // In shadow
        }
    }

    for (int i = 0; i < scene->num_cylinders; i++) {
        double t_shadow;
        if (intersect_cylinder(&shadow_ray, &scene->cylinders[i], &t_shadow) && t_shadow < light_distance) {
            return 1; // In shadow
        }
    }

    // TODO: Add checks for planes or other objects

    return 0; // Not in shadow
}


// t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene) {
//     t_color result = {0, 0, 0};
    
//     t_color light_contribution = {0, 0, 0};
//     // Ambient lighting
//   light_contribution.r += 255 * scene->ambient.ratio;
//     light_contribution.g += 255 * scene->ambient.ratio;
//     light_contribution.b += 255 * scene->ambient.ratio;

//     // Diffuse lighting
//     for (int i = 0; i < scene->num_lights; i++) {
//         t_light light = scene->lights[i];
//         t_vector light_dir = normalize(subtract(light.pos, hit_point));
//         double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness;

//         light_contribution.r += 255 * diffuse_intensity;
//         light_contribution.g += 255 * diffuse_intensity;
//         light_contribution.b += 255 * diffuse_intensity;
//     }
    
//     // Iterate over all lights in the scene
//     for (int i = 0; i < scene->num_lights; i++) {
//         t_light light = scene->lights[i];
//         t_vector light_dir = normalize(subtract(light.pos, hit_point));
//         // double light_distance = sqrt(dot(light_dir, light_dir));

//         // Shadow check (optional): Skip light if point is in shadow
//         // ...

//         // Diffuse lighting
//         double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness;

//         result.r += object_color.r * diffuse_intensity;
//         result.g += object_color.g * diffuse_intensity;
//         result.b += object_color.b * diffuse_intensity;
//     }

//     // Clamp the result to [0, 255]
//     result.r = fmin(255, result.r);
//     result.g = fmin(255, result.g);
//     result.b = fmin(255, result.b);

//     // return result;
//     return combine_color(light_contribution, object_color);
// }


// t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene) {
//     t_color light_contribution = {0, 0, 0};

//     // Ambient lighting
//     light_contribution.r += 255 * scene->ambient.ratio;
//     light_contribution.g += 255 * scene->ambient.ratio;
//     light_contribution.b += 255 * scene->ambient.ratio;

//     // Diffuse lighting
//     for (int i = 0; i < scene->num_lights; i++) {
//         t_light light = scene->lights[i];

//         // Check if the point is in shadow
//         if (is_in_shadow(hit_point, light, scene))
//             continue; // Skip this light if the point is in shadow

//         t_vector light_dir = normalize(subtract(light.pos, hit_point));
//         double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness;

//         light_contribution.r += 255 * diffuse_intensity;
//         light_contribution.g += 255 * diffuse_intensity;
//         light_contribution.b += 255 * diffuse_intensity;
//     }

//     // Combine light contribution with object color
//     return combine_color(light_contribution, object_color);
// }


t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene) {
    t_color light_contribution = {0, 0, 0};

    // Ambient lighting
    light_contribution.r += 255 * scene->ambient.ratio;
    light_contribution.g += 255 * scene->ambient.ratio;
    light_contribution.b += 255 * scene->ambient.ratio;

    // Diffuse lighting
    for (int i = 0; i < scene->num_lights; i++) {
        t_light light = scene->lights[i];
        double shadow_factor = compute_shadow_factor(hit_point, light, scene, 32); // 16 samples for soft shadows

        if (shadow_factor > 0) { // Only compute lighting if not fully in shadow
            t_vector light_dir = normalize(subtract(light.pos, hit_point));
            double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness * shadow_factor;

            light_contribution.r += 255 * diffuse_intensity;
            light_contribution.g += 255 * diffuse_intensity;
            light_contribution.b += 255 * diffuse_intensity;
        }
    }

    // Combine light contribution with object color
    return combine_color(light_contribution, object_color);
}

t_vector compute_reflection(t_vector light_dir, t_vector normal)
{
    t_vector scaled_normal = multiply_scalar(normal, 2.0 * dot(normal, light_dir));
    return subtract(scaled_normal, light_dir);
}


// t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene)
// {
//     t_color light_contribution = {0, 0, 0};

//     // Ambient lighting
//     light_contribution.r += 255 * scene->ambient.ratio;
//     light_contribution.g += 255 * scene->ambient.ratio;
//     light_contribution.b += 255 * scene->ambient.ratio;

//     t_vector view_dir = normalize(subtract(scene->camera.pos, hit_point)); // View vector

//     // Diffuse and specular lighting
//     for (int i = 0; i < scene->num_lights; i++) {
//         t_light light = scene->lights[i];

//         t_vector light_dir = normalize(subtract(light.pos, hit_point));
//         double diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness;

//         // Compute reflection vector
//         t_vector reflect_dir = subtract(multiply_scalar(normal, 2.0 * dot(normal, light_dir)), light_dir);

//         // Compute specular intensity
//         double specular_intensity = pow(fmax(0.0, dot(reflect_dir, view_dir)), 25) * light.brightness; // 32 = shininess factor

//         // Shadow check (optional)
//         // double shadow_factor = 1.0;
//         double shadow_factor = compute_shadow_factor(hit_point, light, scene, 16); // 16 samples for soft shadows

//         if (is_in_shadow(hit_point, light, scene))
//             shadow_factor = 0.0;

//         // Combine lighting contributions
//         // if(specular_intensity)
//         //    printf("specular intensity is: %f\n", specular_intensity);
//         light_contribution.r += shadow_factor * (255 * diffuse_intensity + 255 * specular_intensity);
//         light_contribution.g += shadow_factor * (255 * diffuse_intensity + 255 * specular_intensity);
//         light_contribution.b += shadow_factor * (255 * diffuse_intensity + 255 * specular_intensity);
//     }

//     // Combine light contribution with object color
//     return combine_color(light_contribution, object_color);
// }
