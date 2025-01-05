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
    // int j;

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

#include <math.h>

    // int is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
    // {
    //     t_vector local_point;
    //     double height;
    //     double angle;
    //     double grid_size = scale;

    //     // Translate point to local space (relative to cylinder center)
    //     local_point = subtract(point, cylinder->center);

    //     // Project the point onto the cylinder's orientation axis to get height
    //     height = dot(local_point, cylinder->orientation);

    //     // Remove the height component to get the point on the cylinder's "circle"
    //     t_vector radial = subtract(local_point,
    //                         multiply_scalar(cylinder->orientation, height));

    //     // Create an orthonormal basis perpendicular to the cylinder's orientation
    //     t_vector up = {0, 1, 0}; // Arbitrary "up" vector
    //     if (fabs(dot(up, cylinder->orientation)) > 0.99) // Avoid parallel vectors
    //         up = (t_vector){1, 0, 0}; 

    //     t_vector x_axis = normalize(cross(up, cylinder->orientation));
    //     t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

    //     // Project radial vector onto the local X-Y plane
    //     double proj_x = dot(radial, x_axis);
    //     double proj_y = dot(radial, y_axis);

    //     // Calculate the angle around the cylinder using atan2
    //     angle = atan2(proj_y, proj_x);

    //     // Map angle to a range [0, 2*pi]
    //     if (angle < 0)
    //         angle += 2 * M_PI;

    //     // Scale height and angle to grid size
    //     int u = (int)floor(height / grid_size);
    //     int v = (int)floor((angle * cylinder->radius) / grid_size);

    //     return (u + v) % 2;
    // }

// int is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
// {
//     t_vector local_point;
//     double height;
//     double angle;
//     double grid_size = scale;

//     // Translate point to local space (relative to cylinder center)
//     local_point = subtract(point, cylinder->center);

//     // Project the point onto the cylinder's orientation axis to get height
//     height = dot(local_point, cylinder->orientation);

//     // Remove the height component to get the point on the cylinder's "circle"
//     t_vector radial = subtract(local_point,
//                         multiply_scalar(cylinder->orientation, height));

//     // Create an orthonormal basis perpendicular to the cylinder's orientation
//     t_vector up = {0, 1, 0}; // Arbitrary "up" vector
//     if (fabs(dot(up, cylinder->orientation)) > 0.99) // Avoid parallel vectors
//         up = (t_vector){1, 0, 0};

//     t_vector x_axis = normalize(cross(up, cylinder->orientation));
//     t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

//     // Project radial vector onto the local X-Y plane
//     double proj_x = dot(radial, x_axis);
//     double proj_y = dot(radial, y_axis);

//     // Calculate the angle around the cylinder using atan2
//     angle = atan2(proj_y, proj_x);

//     // Normalize angle to [0, 2*pi]
//     if (angle < 0)
//         angle += 2 * M_PI;

//     // Map angle to a range scaled to grid size relative to circumference
//     double circumference = 2 * M_PI * cylinder->radius;
//     double scaled_angle = angle / (2 * M_PI) * circumference;

//     // Scale height and angle to grid size
//     int u = (int)floor(height / grid_size);       // Vertical coordinate
//     int v = (int)floor(scaled_angle / grid_size); // Horizontal (angular) coordinate

//     // Return the alternating checkerboard pattern
//     return (u + v) % 2;
// }

// int is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
// {
//     t_vector local_point;
//     double height;
//     double angle;
//     double grid_size = scale;

//     // Translate point to local space (relative to cylinder center)
//     local_point = subtract(point, cylinder->center);

//     // Project the point onto the cylinder's orientation axis to get height
//     height = dot(local_point, cylinder->orientation);

//     // Remove the height component to get the point on the cylinder's "circle"
//     t_vector radial = subtract(local_point,
//                         multiply_scalar(cylinder->orientation, height));

//     // Create an orthonormal basis perpendicular to the cylinder's orientation
//     t_vector up = {0, 1, 0};
//     if (fabs(dot(up, cylinder->orientation)) > 0.99)
//         up = (t_vector){1, 0, 0};

//     t_vector x_axis = normalize(cross(up, cylinder->orientation));
//     t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

//     // Project radial vector onto the local X-Y plane
//     double proj_x = dot(radial, x_axis);
//     double proj_y = dot(radial, y_axis);

//     // Calculate the angle around the cylinder using atan2
//     angle = atan2(proj_y, proj_x);

//     // Normalize angle to [0, 1) range
//     double normalized_angle = (angle + M_PI) / (2 * M_PI);

//     // Scale height and angle into grid coordinates
//     int u = (int)floor(height / grid_size);
//     int v = (int)floor(normalized_angle * (2 * M_PI * cylinder->radius) / grid_size);

//     // Return alternating checkerboard pattern
//     return (u + v) % 2;
// }




// int is_checkerboard(t_vector point, t_cylinder *cylinder, double scale) {
//     t_vector local_point;
//     double height;
//     double angle;
//     double grid_size = scale;

//     // Translate point to local space (relative to cylinder center)
//     local_point = subtract(point, cylinder->center);

//     // Project the point onto the cylinder's orientation axis to get height
//     height = dot(local_point, cylinder->orientation);

//     // Remove the height component to get the point on the cylinder's "circle"
//     t_vector radial = subtract(local_point, multiply_scalar(cylinder->orientation, height));

//     // Handle vertical cylinders properly
//     t_vector up = {0, 1, 0}; // Arbitrary "up" vector
//     if (fabs(dot(up, cylinder->orientation)) > 0.99)
//         up = (t_vector){1, 0, 0}; // Switch "up" vector for vertical alignment

//     // Create orthonormal basis perpendicular to cylinder's orientation
//     t_vector x_axis = normalize(cross(up, cylinder->orientation));
//     t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

//     // Project radial vector onto the local X-Y plane
//     double proj_x = dot(radial, x_axis);
//     double proj_y = dot(radial, y_axis);

//     // Calculate the angle around the cylinder using atan2
//     angle = atan2(proj_y, proj_x);

//     // Normalize angle to [0, 1) range
//     double normalized_angle = (angle + M_PI) / (2 * M_PI);

//     // Scale height and angle into grid coordinates
//     int u = (int)floor(height / grid_size);
//     int v = (int)floor(normalized_angle * (cylinder->radius * 2 * M_PI) / grid_size);

//     // Return alternating checkerboard pattern
//     return (u + v) % 2;
// }

int is_checkerboard(t_vector point, t_cylinder *cylinder, double scale) {
    if (fabs(dot((t_vector){0, 1, 0}, cylinder->orientation)) > 0.99) {
        // Vertical cylinder
        return is_checkerboard_vertical(point, cylinder, scale);
    } else {
        // Horizontal cylinder
        return is_checkerboard_horizontal(point, cylinder, scale);
    }
}


//horizontal
int is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder, double scale)
{
    t_vector local_point;
    double height;
    double angle;
    double grid_size = scale;

    // Translate point to local space (relative to cylinder center)
    local_point = subtract(point, cylinder->center);

    // Project the point onto the cylinder's orientation axis to get height
    height = dot(local_point, cylinder->orientation);

    // Remove the height component to get the point on the cylinder's "circle"
    t_vector radial = subtract(local_point,
                        multiply_scalar(cylinder->orientation, height));

    // Handle vertical cylinders properly
    t_vector up = {0, 1, 0}; // Arbitrary "up" vector
    if (fabs(dot(up, cylinder->orientation)) > 0.99)
        up = (t_vector){1, 0, 0}; // Switch "up" vector for vertical alignment

    // Create orthonormal basis perpendicular to cylinder's orientation
    t_vector x_axis = normalize(cross(up, cylinder->orientation));
    t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

    // Project radial vector onto the local X-Y plane
    double proj_x = dot(radial, x_axis);
    double proj_y = dot(radial, y_axis);

    // Calculate the angle around the cylinder using atan2
    angle = atan2(proj_y, proj_x);

    // Normalize angle to [0, 1) range
    double normalized_angle = (angle + M_PI) / (2 * M_PI);

    // Scale height and angle into grid coordinates
    int u = (int)floor(height / grid_size);
    int v = (int)floor(normalized_angle * (2 * M_PI * cylinder->radius) / grid_size);

    // Return alternating checkerboard pattern
    return (u + v) % 2;
}
// vertical
int is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
{
    t_vector local_point;
    double height;
    double angle;
    double grid_size = scale;

    // Translate point to local space (relative to cylinder center)
    local_point = subtract(point, cylinder->center);

    // Project the point onto the cylinder's orientation axis to get height
    height = dot(local_point, cylinder->orientation);

    // Remove the height component to get the point on the cylinder's "circle"
    t_vector radial = subtract(local_point,
                        multiply_scalar(cylinder->orientation, height));

    // Handle vertical cylinders properly
    t_vector up = {0, 1, 0}; // Arbitrary "up" vector
    if (fabs(dot(up, cylinder->orientation)) > 0.99)
        up = (t_vector){1, 0, 0}; // Switch "up" vector for vertical alignment

    // Create orthonormal basis perpendicular to cylinder's orientation
    t_vector x_axis = normalize(cross(up, cylinder->orientation));
    t_vector y_axis = normalize(cross(cylinder->orientation, x_axis));

    // Project radial vector onto the local X-Y plane
    double proj_x = dot(radial, x_axis);
    double proj_y = dot(radial, y_axis);

    // Calculate the angle around the cylinder using atan2
    angle = atan2(proj_y, proj_x);

    // Normalize angle to [0, 2π] range
    if (angle < 0)
        angle += 2 * M_PI;

    // Circumference-based scaling to match height grid
    // double circumference = 2 * M_PI * cylinder->radius;
    double scaled_angle = angle * (cylinder->radius / grid_size);

    // Scale height into grid coordinates
    int u = (int)floor(height / grid_size);
    int v = (int)floor(scaled_angle);

    // Return alternating checkerboard pattern
    return (u + v) % 2;
}






int is_plane_checkerboard(t_vector point, t_vector plane_normal, double scale)
{
    double grid_size = scale;

    // Create an orthonormal basis for the plane
    t_vector up = {0, 1, 0}; // Arbitrary "up" vector
    if (fabs(dot(up, plane_normal)) > 0.99) // Avoid parallel vectors
        up = (t_vector){1, 0, 0};

    t_vector x_axis = normalize(cross(up, plane_normal));
    t_vector y_axis = normalize(cross(plane_normal, x_axis));

    // Project the point onto the plane's local X-Y axes
    double proj_x = dot(point, x_axis);
    double proj_y = dot(point, y_axis);

    // Calculate grid coordinates
    int u = (int)floor(proj_x / grid_size);
    int v = (int)floor(proj_y / grid_size);

    // Return the checkerboard color
    return (u + v) % 2;
}

t_color get_plane_checkerboard_color(t_vector point, t_color color1, t_color color2, t_vector normal,  double scale)
{
    if (is_plane_checkerboard(point, normal, scale))
        return color1;
    return color2;
}

int is_disc_checkerboard(t_vector point, t_disc *disc, double scale)
{
   double grid_size = scale;

    // Create an orthonormal basis for the plane
    t_vector up = {0, 1, 0}; // Arbitrary "up" vector
    if (fabs(dot(up, disc->normal)) > 0.99) // Avoid parallel vectors
        up = (t_vector){1, 0, 0};

    t_vector x_axis = normalize(cross(up, disc->normal));
    t_vector y_axis = normalize(cross(disc->normal, x_axis));

    // Project the point onto the plane's local X-Y axes
    double proj_x = dot(point, x_axis);
    double proj_y = dot(point, y_axis);

    // Calculate grid coordinates
    int u = (int)floor(proj_x / grid_size);
    int v = (int)floor(proj_y / grid_size);

    // Return the checkerboard color
    return (u + v) % 2;
}

t_color get_disc_checkerboard_color(t_vector point, t_disc *disc, t_color color1, t_color color2, double scale)
{
    if (is_disc_checkerboard(point, disc, scale))
        return color1;
    return color2;
}



t_color get_checkerboard_color(t_vector point, t_cylinder *cylinder,
                               t_color color1, t_color color2, double scale)
{
    if (is_checkerboard(point, cylinder, scale))
        return color1;
    return color2;
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
    double shadow_factor = compute_shadow_factor(hit_point, light, scene, 8); // 32 samples for soft shadows

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
