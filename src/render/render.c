#include "../includes/minirt.h"
#include <sys/time.h>
#include <pthread.h>

// #define MAX_REFLECTION_DEPTH 16


typedef struct s_render_data {
    mlx_t *mlx;
    mlx_image_t *img;
    t_scene *scene;
    int threads_completed;
    int rendering_finished;  // Add this flag
    pthread_mutex_t mutex;
    int current_row;
    int render_complete;
    struct timeval start_time;
    struct timeval end_time;
} t_render_data;

typedef struct s_thread_data {
    t_render_data *render_data;
    int start_row;
    int end_row;
    int thread_id;
    int num_threads;
} t_thread_data;


// Helper function to calculate reflection ray
t_ray get_reflection_ray(t_vector hit_point, t_vector normal, t_ray incident_ray)
{
    t_ray reflection;
    reflection.origin = hit_point;
    
    // R = I - 2(N·I)N where I is incident direction, N is normal
    double dot_product = dot(incident_ray.direction, normal);
    t_vector reflection_dir = subtract(incident_ray.direction, 
                                     multiply_scalar(normal, 2.0 * dot_product));
    
    reflection.direction = normalize(reflection_dir);
    return reflection;
}
t_color uint32_to_t_color(uint32_t color)
{
    return (t_color){
        .r = (color >> 24) & 0xFF,
        .g = (color >> 16) & 0xFF,
        .b = (color >> 8) & 0xFF
    };
}

t_color blend_colors(t_color original_color, t_color reflected_color, float reflectivity)
{
    return (t_color) {
        .r = reflectivity * reflected_color.r + (1 - reflectivity) * original_color.r,
        .g = reflectivity * reflected_color.g + (1 - reflectivity) * original_color.g,
        .b = reflectivity * reflected_color.b + (1 - reflectivity) * original_color.b
    };
}


t_vector refract(t_vector incident, t_vector normal, float eta_ratio)
{
    double cos_theta = fmin(dot(multiply_scalar(incident, -1.0), normal), 1.0);
    double sin_theta2 = eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta);
    // Check for total internal reflection
    if (sin_theta2 > 1.0) {
        return reflect(incident, normal); // Return reflection if TIR occurs
    }
    
    t_vector r_out_perp = multiply_scalar(add(incident, multiply_scalar(normal, cos_theta)), eta_ratio);
    t_vector r_out_parallel = multiply_scalar(normal, -sqrt(fabs(1.0 - dot(r_out_perp, r_out_perp))));
    return add(r_out_perp, r_out_parallel);
}

t_vector get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder)
{
    // Project hit point onto cylinder axis
    t_vector axis = normalize(cylinder->orientation);
    t_vector cp = subtract(hit_point, cylinder->center);
    double dot_prod = dot(cp, axis);
    
    // Point on axis closest to hit point
    t_vector axis_point = add(cylinder->center, multiply_scalar(axis, dot_prod));
    
    // Normal is from axis to hit point
    t_vector normal = normalize(subtract(hit_point, axis_point));
    
    return normal;
}
t_vector reflect(t_vector direction, t_vector normal)
{
    double dot_product = dot(direction, normal);
    return subtract(direction, multiply_scalar(normal, 2.0 * dot_product));
}


t_color	combine_color(t_color light_color, t_color object_color)
{
	t_color	result;

	result.r = fmin(255, (light_color.r / 255.0) * object_color.r);
	result.g = fmin(255, (light_color.g / 255.0) * object_color.g);
	result.b = fmin(255, (light_color.b / 255.0) * object_color.b);
	return (result);
}

t_color get_cylinder_checkerboard_color(t_vector point, t_cylinder *cylinder, t_color color1, t_color color2, double scale)
{
    if (is_cylinder_checkerboard(point, cylinder, scale))
        return color1;
    return color2;
}

int is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
{
    // Step 1: Move the point to the cylinder's local space
    t_vector local_point = subtract(point, cylinder->center);

    // Step 2: Project the point onto the cylinder's axis (orientation)
    double height = dot(local_point, cylinder->orientation); // Projection length
    t_vector projection = multiply_scalar(cylinder->orientation, height);

    // Step 3: Find the radial position (2D point around the cylinder)
    t_vector radial_vector = subtract(local_point, projection);

    // Step 4: Calculate the angle around the cylinder (normalized to [0, 1])
    double angle = atan2(radial_vector.z, radial_vector.x); // Angle in [-π, π]
    double u = (angle / (2.0 * M_PI)) + 0.5;               // Normalize to [0, 1]

    // Step 5: Calculate the height ratio (v-coordinate)
    double v = (height + (cylinder->height / 2.0)) / cylinder->height; // Normalize to [0, 1]

    // Step 6: Apply scaling for the checkerboard pattern
    double scaled_u = u * scale;
    double scaled_v = v * scale;

    // Step 7: Create checkerboard pattern
    int u_check = (int)floor(scaled_u) % 2;
    int v_check = (int)floor(scaled_v) % 2;

    // Step 8: Return the pattern value (1 for black, 0 for white)
    return (u_check + v_check) % 2;
}












// Modified color calculation function with reflection support
// t_color trace_ray(t_ray ray, t_scene *scene, int depth)
// {
//     t_hit_record hit;

//     if (depth > MAX_REFLECTION_DEPTH)
//         return ((t_color){0, 0, 0});
//     t_color final_color = {0, 0, 0};
//     t_vector normal;
//     t_color black = {255, 255, 255};
//     t_color white = {0, 0, 0};
//     int i;
//     hit.index = -1;
//     hit.t = INFINITY;
//     hit.hit = 0;
//     hit.hit_from_inside = 0;
//     i = 0;
//     while (i < scene->num_spheres)
//     {
//         double t_sphere;
//         ray.direction = normalize(ray.direction);
//         if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere) && t_sphere < hit.t)
//         {
//             hit.hit = 1;
//             hit.t = t_sphere;
//             hit.type = SPHERE;
//             hit.index = i;
//         }
//         i++;
//     }
//     i = 0;
//     while (i < scene->num_cylinders)
//     {
//         double t_cy;
//         if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cy) && t_cy < hit.t)
//         {
//             hit.hit = 1;
//             hit.t = t_cy;
//             hit.type = CYLINDER;
//             hit.index = i;
//         }
//         i++;
//     }
//     i = 0;
//     while (i < scene->num_discs)
//     {
//         double t_disc;
//         if (intersect_disc(&ray, &scene->discs[i], &t_disc) && t_disc < hit.t)
//         {
//             hit.hit = 1;
//             hit.t = t_disc;
//             hit.type = DISC;
//             hit.index = i;
//         }
//         i++;
//     }
//     i = 0;
//     while (i < scene->num_planes)
//     {
//         double t_plane;
//         if (intersect_plane(&ray, &scene->planes[i], &t_plane) && t_plane < hit.t)
//         {
//             hit.hit = 1;
//             hit.t = t_plane;
//             hit.type = PLANE;
//             hit.index = i;
//         }
//         i++;
//     }
//     if (hit.hit)
//     {
//         hit.point = add(ray.origin, multiply_scalar(ray.direction, hit.t));
//         switch (hit.type)
//         {
//             case SPHERE:
//             {
//                 t_sphere *sphere = &scene->spheres[hit.index];
//                 normal = normalize(subtract(hit.point, sphere->center));
//                 if (sphere->material.checker == 1)
//                 {
//                     t_vector local_point = subtract(hit.point, sphere->center);
//                     double u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
//                     double v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
//                     int check_u = (int)(u * 20.0) % 2;
//                     int check_v = (int)(v * 20.0) % 2;
//                     t_color object_color = (check_u == check_v) ? white : black;
//                     final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
//                 }
//                 else
//                     final_color = apply_lighting(hit.point, normal, sphere->material.color, scene, depth + 1);
//                 hit.material.reflectivity = sphere->material.reflectivity;
//                 hit.material.transparency = sphere->material.transparency;
//                 hit.material.refractive_index = sphere->material.refractive_index;
//                 break;
//             }
//             case CYLINDER:
//             {
//                 t_cylinder *cylinder = &scene->cylinders[hit.index];
//                 normal = get_cylinder_normal(hit.point, cylinder);
//                 if (cylinder->material.checker == 1)
//                 {
//                     int is_black = is_checkerboard(hit.point, cylinder, 0.5); // Scale = 1.0 for fine squares
//                     t_color object_color = is_black ? black : white;
//                     final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
//                 }
//                 else
//                     final_color = apply_lighting(hit.point, normal, cylinder->material.color, scene, depth + 1);
//                 hit.material.reflectivity = cylinder->material.reflectivity;
//                 hit.material.transparency = cylinder->material.transparency;
//                 hit.material.refractive_index = cylinder->material.refractive_index;
//                 break;
//             }
//             case PLANE:
//             {
//                 t_plane *plane = &scene->planes[hit.index];
//                 normal = plane->normal;
//                 if (dot(ray.direction, normal) > 0)
// 	    			normal = multiply_scalar(normal, -1); // Flip the normal
//                 if (plane->material.checker == 1)
//                 {
//                     t_color object_color = get_plane_checkerboard_color(hit.point, black, white, normal, 0.5);
//                     final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
//                 }
//                 else
//                     final_color = apply_lighting(hit.point, normal, plane->material.color, scene, depth + 1);
//                 hit.material.reflectivity = plane->material.reflectivity;
//                 hit.material.transparency = plane->material.transparency;
//                 hit.material.refractive_index = plane->material.refractive_index;
//                 break;
//             }
//             case DISC:
//             {
//                 t_disc *disc = &scene->discs[hit.index];
//                 normal = disc->normal;
//     	        t_color gradient;
//                 if (disc->material.checker == 1)
//                 {
//                     t_color object_color = get_disc_checkerboard_color(hit.point, disc, black, white, 0.5);
//                     final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
//                 }
//                 else
//                 {
//                     gradient = apply_lighting(hit.point, normal, disc->color, scene, depth + 1);
//                     gradient = apply_lighting(hit.point, normal, disc->color, scene, depth + 1);
//               		final_color = gradient;
//                 }
//                 hit.material.reflectivity = disc->material.reflectivity;
//                 hit.material.transparency = disc->material.transparency;
//                 hit.material.refractive_index = disc->material.refractive_index;
//                 break;
//             }
//         }
//         if (hit.material.reflectivity > 0.0)
//         {
//             t_ray reflection_ray = get_reflection_ray(hit.point, normal, ray);
//             reflection_ray.origin = add(reflection_ray.origin,
//                                       multiply_scalar(reflection_ray.direction, 0.001));
//             t_color reflected_color = trace_ray(reflection_ray, scene, depth + 1);
//             final_color = blend_colors(final_color, reflected_color, hit.material.reflectivity);
//         }
//         if (hit.material.transparency > 0.0)
//         {
//             float eta_ratio = (hit.hit_from_inside) ? hit.material.refractive_index : (1.0 / hit.material.refractive_index);
//             if (eta_ratio > 1.0)
//             {
//                 t_vector reflected_dir = reflect(ray.direction, hit.normal);
//                 t_ray reflected_ray = {hit.point, reflected_dir};
//                 reflected_ray.origin = add(reflected_ray.origin, 
//                                            multiply_scalar(reflected_ray.direction, 0.001));
//                 t_color reflected_color = trace_ray(reflected_ray, scene, depth + 1);
//                 final_color = blend_colors(final_color, reflected_color, hit.material.transparency);
//             }
//             else
//             {
//                 t_vector refracted_dir = refract(ray.direction, hit.normal, eta_ratio);
//                 t_ray refracted_ray = {hit.point, refracted_dir};
//                 refracted_ray.origin = add(refracted_ray.origin, 
//                                            multiply_scalar(refracted_ray.direction, 0.001));
//                 t_color refracted_color = trace_ray(refracted_ray, scene, depth + 1);
//                 final_color = blend_colors(final_color, refracted_color, hit.material.transparency);
//             }
//         }
//     }
//     return final_color;
// }

static t_ray get_refraction_ray(t_vector point, t_vector normal, t_ray incoming_ray, double refractive_index)
{
    t_ray refraction_ray;
    double cos_i = -dot(normal, incoming_ray.direction); // Cosine of the angle of incidence
    double eta_i = 1.0; // Assume ray is coming from air (refractive index ~1.0)
    double eta_t = refractive_index; // Material's refractive index
    
    if (cos_i < 0) // Ray is exiting the material
    {
        cos_i = -cos_i;
        double temp = eta_i;
        eta_i = eta_t;
        eta_t = temp;
        normal = multiply_scalar(normal, -1); // Invert normal
    }
    
    double eta = eta_i / eta_t;
    double k = 1 - eta * eta * (1 - cos_i * cos_i);
    
    if (k < 0)
    {
        // Total internal reflection, return a reflection ray instead
        refraction_ray.origin = point;
        refraction_ray.direction = reflect(incoming_ray.direction, normal);
    }
    else
    {
        t_vector refracted_dir = add(
            multiply_scalar(incoming_ray.direction, eta),
            multiply_scalar(normal, (eta * cos_i - sqrt(k)))
        );
        refraction_ray.origin = point;
        refraction_ray.direction = normalize(refracted_dir);
    }
    
    return refraction_ray;
}

// trace_ray.c
t_hit_record find_closest_intersection(t_ray ray, t_scene *scene)
{
    t_hit_record hit;
    
    hit.index = -1;
    hit.t = INFINITY;
    hit.hit = 0;
    hit.hit_from_inside = 0;
    
    check_sphere_intersections(ray, scene, &hit);
    check_cylinder_intersections(ray, scene, &hit);
    check_disc_intersections(ray, scene, &hit);
    check_plane_intersections(ray, scene, &hit);
    
    return (hit);
}

static get_hit_normal_sphere(t_hit_record *hit, t_vector *normal, t_scene *scene)
{
      if (hit->type == SPHERE)
    {
        t_sphere *sphere;
        
        sphere = &scene->spheres[hit->index];
        *normal = normalize(subtract(hit->point, sphere->center));
        hit->material = sphere->material;
    }
}

static get_hit_normal_cylinder(t_hit_record *hit, t_vector *normal, t_scene *scene)
{
   if (hit->type == CYLINDER)
    {
        t_cylinder *cylinder;
        
        cylinder = &scene->cylinders[hit->index];
        *normal = get_cylinder_normal(hit->point, cylinder);
        hit->material = cylinder->material;
    }
}

static get_hit_normal_plane(t_hit_record *hit, t_vector *normal, t_scene *scene, t_ray ray)
{
  if (hit->type == PLANE)
    {
        t_plane *plane;
        
        plane = &scene->planes[hit->index];
        *normal = plane->normal;
        if (dot(ray.direction, plane->normal) > 0)
            *normal = multiply_scalar(plane->normal, -1);
        hit->material = plane->material;
    }
}


static get_hit_normal_disc(t_hit_record *hit, t_vector *normal, t_scene *scene, t_ray ray)
{
   if (hit->type == DISC)
    {
        t_disc *disc;
        
        disc = &scene->discs[hit->index];
        *normal = disc->normal;
        if (dot(ray.direction, disc->normal) > 0)
            *normal = multiply_scalar(disc->normal, -1);
        hit->material = disc->material;
    }
}


void get_hit_normal(t_hit_record *hit, t_ray ray, t_vector *normal, t_scene *scene)
{
    get_hit_normal_sphere(hit, normal, scene);
    get_hit_normal_cylinder(hit, normal, scene);
    get_hit_normal_plane(hit, normal, scene, ray);
    get_hit_normal_disc(hit, normal, scene, ray);
}
static t_color get_surface_color_sphere(t_scene *scene, t_hit_record *hit)
{
    t_sphere *sphere;
    t_color black;
    t_color white;
    black = (t_color){255, 255, 255};
    white = (t_color){0, 0, 0};
        sphere = &scene->spheres[hit->index];
        if (sphere->material.checker == 1)
        {
            t_vector local_point;
            double u;
            double v;
            
            local_point = subtract(hit->point, sphere->center);
            u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
            v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
            return(((int)(u * 20.0) % 2 == (int)(v * 20.0) % 2) 
                ? white : black);
        }
        else
            return(sphere->material.color);
    
}

static t_color get_surface_color_cylinder(t_scene *scene, t_hit_record *hit)
{
    t_cylinder *cylinder;
    t_color black;
    t_color white;
    black = (t_color){255, 255, 255};
    white = (t_color){0, 0, 0};
 
        cylinder = &scene->cylinders[hit->index];
        if (cylinder->material.checker == 1)
            return(is_checkerboard(hit->point, cylinder, 0.5) 
                ? black : white);
        else
            return(cylinder->material.color);
}


static t_color get_surface_color_plane(t_scene *scene, t_hit_record *hit, t_vector normal)
{
    t_color black;
    t_color white;
    black = (t_color){255, 255, 255};
    white = (t_color){0, 0, 0};
 
      t_plane *plane;
        
        plane = &scene->planes[hit->index];
        if (plane->material.checker == 1)
            return(get_plane_checkerboard_color(hit->point, 
                black, white, normal, 0.5));
        else
            return(plane->material.color);
}

static t_color get_surface_color_disc(t_scene *scene, t_hit_record *hit, t_vector normal)
{
    t_color black;
    t_color white;
    black = (t_color){255, 255, 255};
    white = (t_color){0, 0, 0};
 
       t_disc *disc;
        
        disc = &scene->discs[hit->index];
        if (disc->material.checker == 1)
            return(get_disc_checkerboard_color(hit->point, 
                disc, black, white, 0.5));
        else
            return(disc->material.color);
}


t_color get_surface_color(t_hit_record *hit, t_vector normal, 
    t_scene *scene, int depth)
{
    t_color black;
    t_color white;
    t_color object_color;
    
    black = (t_color){255, 255, 255};
    white = (t_color){0, 0, 0};
    
    if (hit->type == SPHERE)
		object_color = get_surface_color_sphere(scene, hit);
    else if (hit->type == CYLINDER)
        object_color = get_surface_color_cylinder(scene, hit);
    else if (hit->type == PLANE)
    	object_color = get_surface_color_plane(scene, hit, normal);
    else if (hit->type == DISC)
    	object_color = get_surface_color_disc(scene, hit, normal);
    return (apply_lighting(hit->point, normal, object_color, scene, depth + 1));
}

t_color calculate_object_color(t_hit_record *hit, t_ray ray, t_scene *scene, int depth)
{
    t_vector normal;
    t_color final_color;
    
    get_hit_normal(hit, ray, &normal, scene);  // Added scene parameter here
    final_color = get_surface_color(hit, normal, scene, depth);
    if (hit->material.reflectivity > 0.0 || hit->material.transparency > 0.0)
        final_color = apply_material_effects(hit, ray, normal, 
            scene, final_color, depth);
    return (final_color);
}

t_color trace_ray(t_ray ray, t_scene *scene, int depth)
{
    t_hit_record hit;
    t_color black = {0, 0, 0};

    if (depth > MAX_REFLECTION_DEPTH)
        return (black);
    hit = find_closest_intersection(ray, scene);
    if (hit.hit)
    {
        hit.point = add(ray.origin, 
            multiply_scalar(ray.direction, hit.t));
        return (calculate_object_color(&hit, ray, scene, depth));
    }
    return (black);
}
// trace_ray_utils.c
void check_sphere_intersections(t_ray ray, t_scene *scene,     t_hit_record *hit)
{
    int i;
    double t_sphere;

    i = 0;
    while (i < scene->num_spheres)
    {
        ray.direction = normalize(ray.direction);
        if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere) 
            && t_sphere < hit->t)
        {
            hit->hit = 1;
            hit->t = t_sphere;
            hit->type = SPHERE;
            hit->index = i;
        }
        i++;
    }
}

void check_cylinder_intersections(t_ray ray, t_scene *scene,     t_hit_record *hit)
{
    int i;
    double t_cylinder;

    i = 0;
    while (i < scene->num_cylinders)
    {
        ray.direction = normalize(ray.direction);
        if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cylinder) 
            && t_cylinder < hit->t)
        {
            hit->hit = 1;
            hit->t = t_cylinder;
            hit->type = CYLINDER;
            hit->index = i;
        }
        i++;
    }
}

void check_disc_intersections(t_ray ray, t_scene *scene,     t_hit_record *hit)
{
    int i;
    double t_disc;

    i = 0;
    while (i < scene->num_discs)
    {
        ray.direction = normalize(ray.direction);
        if (intersect_disc(&ray, &scene->discs[i], &t_disc) 
            && t_disc < hit->t)
        {
            hit->hit = 1;
            hit->t = t_disc;
            hit->type = DISC;
            hit->index = i;
        }
        i++;
    }
}

void check_plane_intersections(t_ray ray, t_scene *scene,     t_hit_record *hit)
{
    int i;
    double t_plane;

    i = 0;
    while (i < scene->num_planes)
    {
        ray.direction = normalize(ray.direction);
        if (intersect_plane(&ray, &scene->planes[i], &t_plane) 
            && t_plane < hit->t)
        {
            hit->hit = 1;
            hit->t = t_plane;
            hit->type = PLANE;
            hit->index = i;
        }
        i++;
    }
}


typedef struct s_material_params
{
    t_hit_record    *hit;
    t_ray           ray;
    t_vector        normal;
    t_scene         *scene;
    int             depth;
}   t_material_params;

t_color apply_reflection(t_material_params params, t_color base_color)
{
    t_ray   reflection_ray;
    t_color reflected_color;

    reflection_ray = get_reflection_ray(params.hit->point, 
        params.normal, params.ray);
    reflection_ray.origin = add(reflection_ray.origin,
        multiply_scalar(reflection_ray.direction, 0.001));
    reflected_color = trace_ray(reflection_ray, params.scene, 
        params.depth + 1);
    return (blend_colors(base_color, reflected_color,
        params.hit->material.reflectivity));
}

t_color apply_refraction(t_material_params params, t_color base_color)
{
    t_ray   refraction_ray;
    t_color refracted_color;

    refraction_ray = get_refraction_ray(params.hit->point, params.normal,
        params.ray, params.hit->material.refractive_index);
    refraction_ray.origin = add(refraction_ray.origin,
        multiply_scalar(refraction_ray.direction, 0.001));
    refracted_color = trace_ray(refraction_ray, params.scene, 
        params.depth + 1);
    return (blend_colors(base_color, refracted_color,
        params.hit->material.transparency));
}

t_color apply_material_effects(t_hit_record *hit, t_ray ray,
    t_vector normal, t_scene *scene, t_color base_color, int depth)
{
    t_material_params    params;
    t_color             final_color;

    params.hit = hit;
    params.ray = ray;
    params.normal = normal;
    params.scene = scene;
    params.depth = depth;
    final_color = base_color;
    if (hit->material.reflectivity > 0.0)
        final_color = apply_reflection(params, final_color);
    if (hit->material.transparency > 0.0)
        final_color = apply_refraction(params, final_color);
    return (final_color);
}

double schlick_reflection_coefficient(double cos_theta, double refractive_index)
{
    double r0 = pow((1 - refractive_index) / (1 + refractive_index), 2);
    return r0 + (1 - r0) * pow((1 - cos_theta), 5);
}
// Modify t_render_data to include thread management
// New function to handle rendering in threads
void *render_thread(void *arg)
{
    // double reflectivity = 0.0;
    t_thread_data *thread_data = (t_thread_data *)arg;
    t_render_data *data = thread_data->render_data;
    // t_color black = {0, 0, 0};
    // t_color white = {255, 255, 255};
    // t_vector normal;
	// pthread_detach(pthread_self());
    int x;
    int y;
    int thread_id = thread_data->thread_id;
    int num_threads = thread_data->num_threads;
y = 0;
while (y < HEIGHT) {
    x = 0;
    while (x < WIDTH) {
        int pixel_index = y * WIDTH + x;
        if (pixel_index % num_threads == thread_id)
        {
            t_ray ray = create_ray(x, y, &data->scene->camera);
            t_color final_color = trace_ray(ray, data->scene, 5);
            // double t;
            uint32_t color = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            pthread_mutex_lock(&data->mutex);
            mlx_put_pixel(data->img, x, y, color);
            pthread_mutex_unlock(&data->mutex);
        }
        x++;
    }
    y++;
}

    // Update thread completion count
    pthread_mutex_lock(&data->mutex);
    data->threads_completed++;
    // If this is the last thread to complete
    if (data->threads_completed == NUM_THREADS)
    {
        gettimeofday(&data->end_time, NULL);
        double elapsed_time = (data->end_time.tv_sec - data->start_time.tv_sec) + 
                            (data->end_time.tv_usec - data->start_time.tv_usec) / 1e6;
        printf("Rendering took %f seconds\n", elapsed_time);
        
        // Save the image
        save_image_to_file(data->img->pixels, WIDTH, HEIGHT, "output.png");
        
        // Set the finished flag instead of exiting
        data->rendering_finished = 1;
    }
    pthread_mutex_unlock(&data->mutex);

    free(thread_data);
    return NULL;
}


int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

// Function to create a ray from the camera for a specific pixel
// t_ray create_ray(int x, int y, t_camera *camera)
// {
//     t_ray ray;
//     ray.origin = camera->pos;

//     // Calculate direction from camera to pixel (simple perspective projection)
//     ray.direction.x = (2 * (x + 0.5) / (double)WIDTH - 1) * tan(camera->fov / 2 * M_PI / 180);
//     ray.direction.y = (1 - 2 * (y + 0.5) / (double)HEIGHT) * tan(camera->fov / 2 * M_PI / 180);
//     ray.direction.z = 1; // Assume camera is looking along the positive z-axis
//     ray.direction = normalize(ray.direction);

//     return ray;
// }
t_ray create_ray(int x, int y, t_camera *camera)
{
    t_ray ray;
    
    // Step 1: Calculate aspect ratio and field of view scale factor
    double aspect_ratio = (double)WIDTH / HEIGHT;
    double fov_scale = tan((camera->fov * M_PI / 180) / 2);

    // Step 2: Compute the normalized device coordinates of the image point
    double norm_x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_scale;
    double norm_y = (1 - 2 * (y + 0.5) / HEIGHT) * fov_scale;

    // Step 3: Compute the right and up vectors on the fly
    t_vector right = normalize(cross((t_vector){0, 1, 0}, camera->orientation)); // Right vector
    t_vector up = cross(camera->orientation, right); // Up vector

    // Step 4: Compute the ray direction in world space
    ray.origin = camera->pos; // The ray starts at the camera's position
    ray.direction.x = norm_x * right.x + norm_y * up.x + camera->orientation.x;
    ray.direction.y = norm_x * right.y + norm_y * up.y + camera->orientation.y;
    ray.direction.z = norm_x * right.z + norm_y * up.z + camera->orientation.z;

    // Normalize the direction to ensure the ray is unit-length
    ray.direction = normalize(ray.direction);

    return ray;
}


t_vector cross(t_vector a, t_vector b)
{
    t_vector result;

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}

t_vector	world_to_local(t_vector point, t_vector orientation, t_vector center)
{
    t_vector local_point = subtract(point, center);
    t_vector up = orientation;
    t_vector right = normalize(cross(up, (fabs(up.y) < 0.999) ? (t_vector){0, 1, 0} : (t_vector){1, 0, 0}));
    t_vector forward = cross(right, up);

    return (t_vector)
    {
        dot(local_point, right),
        dot(local_point, up),
        dot(local_point, forward)
    };
}

void	update_display(void *param)
{
    t_render_data *data;

	data = (t_render_data *)param;
    mlx_image_to_window(data->mlx, data->img, 0, 0);
    pthread_mutex_lock(&data->mutex);
    if (data->rendering_finished)
    {
        mlx_terminate(data->mlx);
        pthread_mutex_unlock(&data->mutex);
        pthread_mutex_destroy(&data->mutex);
        free(data);
        exit(0);
    }
    pthread_mutex_unlock(&data->mutex);
}

static void init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene, t_render_data *data)
{
	data->mlx = mlx;
    data->img = img;
    data->scene = scene;
    data->threads_completed = 0;
    data->rendering_finished = 0;
	pthread_mutex_init(&data->mutex, NULL);
    gettimeofday(&data->start_time, NULL);
}

void render_scene(mlx_t *mlx, t_scene *scene)
{
    static int		i;
    const int		num_threads = NUM_THREADS;
	pthread_t		threads[num_threads];
	mlx_image_t		*img;
	t_render_data	*data;
	t_thread_data	*thread_data;

	img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
        exit_with_error("Error creating image");
    data = malloc(sizeof(t_render_data));
    if (!data)
		exit_with_error("Error allocating render data");
	init_render_scene(mlx, img, scene, data);
    mlx_loop_hook(mlx, update_display, data);
    while (i < num_threads)
	{
    	thread_data = malloc(sizeof(t_thread_data));
    	thread_data->render_data = data;
    	thread_data->thread_id = i;
		thread_data->num_threads = num_threads;
    	pthread_create(&threads[i], NULL, render_thread, thread_data);
    	i++;
	}
    mlx_loop(mlx);
}
