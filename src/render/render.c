#include "../../includes/minirt.h"
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
t_color uint32_to_t_color(uint32_t color) {
    return (t_color){
        .r = (color >> 24) & 0xFF,
        .g = (color >> 16) & 0xFF,
        .b = (color >> 8) & 0xFF
    };
}

t_color blend_colors(t_color original_color, t_color reflected_color, float reflectivity) {
    return (t_color) {
        .r = reflectivity * reflected_color.r + (1 - reflectivity) * original_color.r,
        .g = reflectivity * reflected_color.g + (1 - reflectivity) * original_color.g,
        .b = reflectivity * reflected_color.b + (1 - reflectivity) * original_color.b
    };
}


t_vector refract(t_vector incident, t_vector normal, float eta_ratio) {
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
t_vector reflect(t_vector direction, t_vector normal) {
    double dot_product = dot(direction, normal);
    return subtract(direction, multiply_scalar(normal, 2.0 * dot_product));
}


// Modified color calculation function with reflection support
t_color trace_ray(t_ray ray, t_scene *scene, int depth)
{
    if (depth > MAX_REFLECTION_DEPTH)
        return ((t_color){0, 0, 0});



    t_hit_record hit;
    hit.t = INFINITY;
    hit.hit = 0;
    t_color light_contribution = {0, 0, 0};
    // double t = INFINITY;
    // int hit = 0;
    t_color final_color = {0, 0, 0};
    double reflectivity = 0.0;
    double transparency = 0.0;      // 0.0 (opaque) to 1.0 (fully transparent)
    double refractive_index = 0.0;
    t_vector hit_point, normal;
    t_color black = {255, 0, 0};
    t_color white = {0, 255, 0};
    // t_object_type hit_type;
    hit.index = -1;

    // Check sphere intersections
    for (int i = 0; i < scene->num_spheres; i++)
    {
        double t_sphere;
        ray.direction = normalize(ray.direction);
        if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere) && t_sphere < hit.t)
        {
            hit.hit = 1;
            hit.t = t_sphere;
            hit.type = SPHERE;
            hit.index = i;
        }



        
    }

    // Check cylinder intersections
    for (int i = 0; i < scene->num_cylinders; i++)
    {
        double t_cy;
        if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cy) && t_cy < hit.t)
        {
            hit.hit = 1;
            hit.t = t_cy;
            hit.type = CYLINDER;
            hit.index = i;
        }
    }

    // Check plane intersections
    for (int i = 0; i < scene->num_planes; i++)
    {
        // printf("PLEEN\n");
        double t_plane;
        if (intersect_plane(&ray, &scene->planes[i], &t_plane) && t_plane < hit.t)
        {
            // printf("PLEEN HIT\n");
            hit.hit = 1;
            hit.t = t_plane;
            hit.type = PLANE;
            hit.index = i;
        }
    }

    // Calculate color based on closest intersection
    if (hit.hit)
    {
        hit.point = add(ray.origin, multiply_scalar(ray.direction, hit.t));

        switch (hit.type)
        {
            case SPHERE:
            {
                t_sphere *sphere = &scene->spheres[hit.index];
                normal = normalize(subtract(hit.point, sphere->center));
                                        // printf("LALALA\n");

                if (sphere->material.checker == 1)
                {
                        // printf("LBLBLB\n");

                    t_vector local_point = subtract(hit.point, sphere->center);
                    double u = 0.5 + atan2(local_point.z, local_point.x) / (2 * M_PI);
                    double v = 0.5 - asin(local_point.y / sphere->radius) / M_PI;
                    
                    int check_u = (int)(u * 10.0) % 2;
                    int check_v = (int)(v * 10.0) % 2;
                    
                    t_color object_color = (check_u == check_v) ? white : black;
                    
                    final_color = apply_lighting(hit_point, normal, object_color, scene, depth + 1);
                    // final_color = apply_lighting(hit.point, normal, object_color, scene);
                }
                else
                {
                    final_color = apply_lighting(hit_point, normal, sphere->material.color, scene, depth + 1);
                }
                // reflectivity = sphere->material.reflectivity;
                // transparency = sphere->material.transparency;
                // refractive_index = sphere->material.refractive_index;

                hit.material.reflectivity = sphere->material.reflectivity;
                hit.material.transparency = sphere->material.transparency;
                hit.material.refractive_index = sphere->material.refractive_index;

                break;
            }
            case CYLINDER:
            {
                t_cylinder *cylinder = &scene->cylinders[hit.index];
                normal = get_cylinder_normal(hit.point, cylinder);  // Make sure you have this function
                
               if (cylinder->material.checker == 1)
                {
                    t_color object_color = get_checkerboard_color(hit.point, black, white, 1.0);
                    final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
                }
                else
                {
                    final_color = apply_lighting(hit.point, normal, cylinder->material.color, scene, depth + 1);
                }
                hit.material.reflectivity = cylinder->material.reflectivity;
                hit.material.transparency = cylinder->material.transparency;
                hit.material.refractive_index = cylinder->material.refractive_index;
                break;
            }
            case PLANE:
            {
                t_plane *plane = &scene->planes[hit.index];
                normal = plane->normal;
                
                if (plane->material.checker == 1)
                {
                    t_color object_color = get_checkerboard_color(hit.point, black, white, 1.0);
                    final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
                }
                else
                {
                    final_color = apply_lighting(hit.point, normal, plane->material.color, scene, depth + 1);
                }
                hit.material.reflectivity = plane->material.reflectivity;
                hit.material.transparency = plane->material.transparency;
                hit.material.refractive_index = plane->material.refractive_index;
                break;
            }
        }
        
        if (hit.material.reflectivity > 0.0)
        {
            double cos_theta = fmax(dot(normal, multiply_scalar(ray.direction, -1.0)), 0.0);
            // double reflection_coefficient = schlick_reflection_coefficient(cos_theta, hit.material.refractive_index);
            t_ray reflection_ray = get_reflection_ray(hit.point, normal, ray);
            reflection_ray.origin = add(reflection_ray.origin,
                                      multiply_scalar(reflection_ray.direction, 0.001));
            t_color reflected_color = trace_ray(reflection_ray, scene, depth + 1);
            final_color = blend_colors(final_color, reflected_color, hit.material.reflectivity);
            // final_color = blend_colors(final_color, reflected_color, reflection_coefficient * hit.material.reflectivity);
        }
        //    if (hit.material.reflectivity > 0.0) {
        //     t_vector reflected_dir = reflect(ray.direction, hit.normal);
        //     double cos_theta = fmax(dot(normal, multiply_scalar(ray.direction, -1.0)), 0.0);
        //     double reflection_coefficient = schlick_reflection_coefficient(cos_theta, hit.material.refractive_index);
        //     t_ray reflected_ray = {hit.point, reflected_dir};
        //     reflected_ray.origin = add(reflected_ray.origin, 
        //                               multiply_scalar(reflected_ray.direction, 0.001));
        //     t_color reflected_color = trace_ray(reflected_ray, scene, depth + 1);
        //     // final_color = blend_colors(final_color, reflected_color, reflection_coefficient * hit.material.reflectivity);
        //     final_color = blend_colors(final_color, reflected_color, hit.material.reflectivity);
        // }
  if (hit.material.transparency > 0.0) {
            // Check if the ray is entering or exiting the material
            // printf("TRnaS\n");
            // hit.hit_from_inside = dot(ray.direction, hit.normal) > 0;
            // if (hit.hit_from_inside)
            // {
            //     hit.normal = multiply_scalar(hit.normal, -1.0); // Flip normal if inside
            // }           

            // Ensure eta_ratio is valid before using it
            float eta_ratio = (hit.hit_from_inside) ? hit.material.refractive_index : (1.0 / hit.material.refractive_index);
            
            // Check for total internal reflection
            if (eta_ratio > 1.0) {
                // Handle total internal reflection case
                t_vector reflected_dir = reflect(ray.direction, hit.normal);
                t_ray reflected_ray = {hit.point, reflected_dir};
                reflected_ray.origin = add(reflected_ray.origin, 
                                           multiply_scalar(reflected_ray.direction, 0.001));
                t_color reflected_color = trace_ray(reflected_ray, scene, depth + 1);
                final_color = blend_colors(final_color, reflected_color, hit.material.transparency);
            } else {
                // Calculate refraction direction
                t_vector refracted_dir = refract(ray.direction, hit.normal, eta_ratio);
                t_ray refracted_ray = {hit.point, refracted_dir};
                refracted_ray.origin = add(refracted_ray.origin, 
                                           multiply_scalar(refracted_ray.direction, 0.001));
                t_color refracted_color = trace_ray(refracted_ray, scene, depth + 1);
                // Blend refraction with final color
                final_color = blend_colors(final_color, refracted_color, hit.material.transparency);
            }
        
        }
// Handle refraction
// if (hit.material.transparency > 0.0) {

//    hit.hit_from_inside = dot(ray.direction, hit.normal) > 0;
//             if (hit.hit_from_inside)
//             {
//                 hit.normal = multiply_scalar(hit.normal, -1.0); // Flip normal if inside
//             }      

//     float eta_ratio = (hit.hit_from_inside) ? hit.material.refractive_index : (1.0 / hit.material.refractive_index);
//     t_vector refracted_dir = refract(ray.direction, hit.normal, eta_ratio);

//     // Offset the origin to prevent shadow acne (self-intersection)
//          double cos_theta = fmax(dot(normal, multiply_scalar(ray.direction, -1.0)), 0.0);
//             double reflection_coefficient = schlick_reflection_coefficient(cos_theta, hit.material.refractive_index);
    
//     t_ray refracted_ray = {add(hit.point, multiply_scalar(refracted_dir, 0.01)), refracted_dir};
//    refracted_ray.origin = add(refracted_ray.origin, 
//                                       multiply_scalar(refracted_ray.direction, 0.001));
//     // Recursively trace the refracted ray with decreased depth
//     t_color refracted_color = trace_ray(refracted_ray, scene, depth - 1);

//     // Blend refraction with final color
//     final_color = blend_colors(final_color, refracted_color,    hit.material.transparency);
// // printf("Refracted Direction: (%f, %f, %f)\n", refracted_dir.x, refracted_dir.y, refracted_dir.z);
// // printf("Final Color: R=%f, G=%f, B=%f\n", final_color.r, final_color.g, final_color.b);

// }
// }Handle refraction
      

    }

    return final_color;
}

double schlick_reflection_coefficient(double cos_theta, double refractive_index) {
    double r0 = pow((1 - refractive_index) / (1 + refractive_index), 2);
    return r0 + (1 - r0) * pow((1 - cos_theta), 5);
}
// Modify t_render_data to include thread management
// New function to handle rendering in threads
void *render_thread(void *arg)
{
    double reflectivity = 0.0;
    t_thread_data *thread_data = (t_thread_data *)arg;
    t_render_data *data = thread_data->render_data;
    t_color black = {0, 0, 0};
    t_color white = {255, 255, 255};
    t_vector hit_point, normal;
	// pthread_detach(pthread_self());

    for (int y = thread_data->start_row; y < thread_data->end_row; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            t_ray ray = create_ray(x, y, &data->scene->camera);
            // t_vector origin = {0, 0, 0};
            // ray.origin = origin;
            t_color final_color = trace_ray(ray, data->scene, 5);
            double t;
            uint32_t color = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            pthread_mutex_lock(&data->mutex);
            mlx_put_pixel(data->img, x, y, color);
            pthread_mutex_unlock(&data->mutex);
        }
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
t_ray create_ray(int x, int y, t_camera *camera)
{
    t_ray ray;
    ray.origin = camera->pos;

    // Calculate direction from camera to pixel (simple perspective projection)
    ray.direction.x = (2 * (x + 0.5) / (double)WIDTH - 1) * tan(camera->fov / 2 * M_PI / 180);
    ray.direction.y = (1 - 2 * (y + 0.5) / (double)HEIGHT) * tan(camera->fov / 2 * M_PI / 180);
    ray.direction.z = 1; // Assume camera is looking along the positive z-axis
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

t_vector world_to_local(t_vector point, t_vector orientation, t_vector center)
{
    t_vector local_point = subtract(point, center);

    // Assume `orientation` is a normalized vector representing the cylinder's axis.
    // Build a local coordinate system using the cylinder's orientation
    t_vector up = orientation;
    t_vector right = normalize(cross(up, (fabs(up.y) < 0.999) ? (t_vector){0, 1, 0} : (t_vector){1, 0, 0}));
    t_vector forward = cross(right, up);

    // Transform the local_point into this coordinate system
    return (t_vector){
        dot(local_point, right),
        dot(local_point, up),
        dot(local_point, forward)
    };
}

void update_display(void *param)
{
    t_render_data *data = (t_render_data *)param;
    
    mlx_image_to_window(data->mlx, data->img, 0, 0);
    
    // Check if rendering is finished
    pthread_mutex_lock(&data->mutex);
    if (data->rendering_finished)
    {
        // Clean up and terminate
        mlx_terminate(data->mlx);
        pthread_mutex_unlock(&data->mutex);
        pthread_mutex_destroy(&data->mutex);
        free(data);
        exit(0);
    }
    pthread_mutex_unlock(&data->mutex);
}


void render_scene(mlx_t *mlx, t_scene *scene)
{
    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
        exit_with_error("Error creating image");

    t_render_data *data = malloc(sizeof(t_render_data));
    if (!data) {
        perror("Error allocating render data");
        exit(EXIT_FAILURE);
    }

    // Initialize render data
    data->mlx = mlx;
    data->img = img;
    data->scene = scene;
    data->threads_completed = 0;
    data->rendering_finished = 0;  // Initialize the flag
    pthread_mutex_init(&data->mutex, NULL);
    gettimeofday(&data->start_time, NULL);

    mlx_loop_hook(mlx, update_display, data);

    // Create threads
    const int num_threads = NUM_THREADS;  // Consider making this a #define NUM_THREADS
    pthread_t threads[num_threads];
    int rows_per_thread = HEIGHT / num_threads;

    for (int i = 0; i < num_threads; i++) {
        t_thread_data *thread_data = malloc(sizeof(t_thread_data));
        thread_data->render_data = data;
        thread_data->start_row = i * rows_per_thread;
        thread_data->end_row = (i == num_threads - 1) ? HEIGHT : (i + 1) * rows_per_thread;
        
        pthread_create(&threads[i], NULL, render_thread, thread_data);
    }

    // Start the MLX loop
    mlx_loop(mlx);
}
