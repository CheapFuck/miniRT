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
    t_vector normal;
    t_color black = {255, 0, 0};
    t_color white = {0, 0, 255};
    int i;
    // t_object_type hit_type;
    hit.index = -1;

    // Check sphere intersections
   i = 0;
while (i < scene->num_spheres)
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
    i++;
}

    // Check cylinder intersections
i = 0;
while (i < scene->num_cylinders)
{
    double t_cy;
    if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cy) && t_cy < hit.t)
    {
        hit.hit = 1;
        hit.t = t_cy;
        hit.type = CYLINDER;
        hit.index = i;
    }
    i++;
}
i = 0;
while (i < scene->num_discs)
{
    double t_disc;
    if (intersect_disc(&ray, &scene->discs[i], &t_disc) && t_disc < hit.t)
    {
        hit.hit = 1;
        hit.t = t_disc;
        hit.type = DISC;
        hit.index = i;
    }
    i++;
}


    // Check plane intersections
i = 0;
while (i < scene->num_planes)
{
    double t_plane;
    if (intersect_plane(&ray, &scene->planes[i], &t_plane) && t_plane < hit.t)
    {
        hit.hit = 1;
        hit.t = t_plane;
        hit.type = PLANE;
        hit.index = i;
    }
    i++;
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
                    double u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
                    double v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
                    
                    int check_u = (int)(u * 20.0) % 2;
                    int check_v = (int)(v * 20.0) % 2;
                    
                    t_color object_color = (check_u == check_v) ? white : black;
                    final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
                    // final_color = apply_lighting(hit.point, normal, object_color, scene);
                }
                else
                {
                    final_color = apply_lighting(hit.point, normal, sphere->material.color, scene, depth + 1);
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
                    
                    // t_color object_color = get_checkerboard_color(hit.point, cylinder, black, white, 1.0);
                    // t_color object_color = get_checkerboard_color(hit.point, &scene->cylinders[i], black, white, .5);
                    // t_color     object_color = get_cylinder_checkerboard_color(hit.point, &scene->cylinders[i], black, white, 10);
                    int is_black = is_checkerboard(hit.point, cylinder, 0.5); // Scale = 1.0 for fine squares
                    t_color object_color = is_black ? black : white;
                    final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
                    // final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
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
                normal = plane->normal;
    			if (dot(ray.direction, normal) > 0)
	    			normal = multiply_scalar(normal, -1); // Flip the normal

                // printf("plane->material.checker: %i\n", plane->material.checker);
                if (plane->material.checker == 1)
                {
                    // t_color object_color = get_checkerboard_color(hit.point, plane, black, white, 1.0);
                    t_color object_color = get_plane_checkerboard_color(hit.point, black, white, normal, 0.05);
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
            case DISC:
            {
                // printf("doe ik dit?\n");
                double t;
               	t = INFINITY;
                t_disc *disc = &scene->discs[hit.index];
                normal = disc->normal;
        			// normal = normalize(subtract(hit_point, scene->discs[i].center));

                	t_color single;
	        t_color gradient;
            // hit.point = add(ray.origin, multiply_scalar(ray.direction, t));
            // normal = disc->normal;
			// normal = normalize(subtract(hit.point, disc->center));
            // gradient = apply_lighting(hit_point, normal, scene->discs[i].color, scene, depth + 1);
            
			// single = apply_lighting(hit_point, scene->discs[i].normal, scene->discs[i].color, scene, depth + 1);
			// final_color = combine_color(single, gradient);


                if (disc->material.checker == 1)
                {
                    // printf("waaaat!?\n");
                    // t_color object_color = get_checkerboard_color(hit.point, disc, black, white, 1.0);
                    t_color object_color = get_disc_checkerboard_color(hit.point, disc, black, white, 0.5);
                    final_color = apply_lighting(hit.point, normal, object_color, scene, depth + 1);
                }
                else
                {
                    // printf("jatoc!?\n");
                    // hit_point = add(ray.origin, ray.direction);
                    gradient = apply_lighting(hit.point, normal, disc->color, scene, depth + 1);
                    // gradient = apply_lighting(hit_point, normal, scene->discs[i].color, scene, depth + 1);
			        
                    gradient = apply_lighting(hit.point, normal, disc->color, scene, depth + 1);
            
            // single = apply_lighting(hit_point, scene->discs[i].normal, scene->discs[i].color, scene, depth + 1);
    			    // single = apply_lighting(hit_point, disc->normal, disc->color, scene, depth + 1);
	    		    // final_color = combine_color(single, gradient);
            		// final_color = single;
              		final_color = gradient;
                    // final_color = apply_lighting(hit.point, normal, disc->material.color, scene, depth + 1);

                }
                hit.material.reflectivity = disc->material.reflectivity;
                hit.material.transparency = disc->material.transparency;
                hit.material.refractive_index = disc->material.refractive_index;
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
  if (hit.material.transparency > 0.0) {
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
            double t;
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
t_ray	create_ray(int x, int y, t_camera *camera)
{
	t_ray	ray;
	t_vector	forward, right, up;
	t_vector	image_point;
    t_vector    r_vector;

    r_vector.x = 0;
    r_vector.y = 1;
    r_vector.z = 0;
	// Step 1: Set up camera basis vectors
	forward = normalize(camera->orientation); // Camera view direction
	right = normalize(cross(r_vector, forward)); // Right vector
	up = cross(forward, right); // Up vector

	// Step 2: Map pixel coordinates to normalized device coordinates
	double aspect_ratio = (double)WIDTH / HEIGHT;
	double fov_scale = tan((camera->fov * M_PI / 180) / 2);

	image_point.x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_scale;
	image_point.y = (1 - 2 * (y + 0.5) / HEIGHT) * fov_scale;
	image_point.z = 1;

	// Step 3: Transform image point to world space
	ray.origin = camera->pos;
	ray.direction = normalize(add(add(scale_vector(right, image_point.x),
									scale_vector(up, image_point.y)),
									scale_vector(forward, image_point.z)));
	return (ray);
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
    int i;

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
    i = 0;
    while (i < num_threads)
{
    t_thread_data *thread_data = malloc(sizeof(t_thread_data));
    thread_data->render_data = data;
    thread_data->thread_id = i;           // Assign thread ID
    thread_data->num_threads = num_threads; // Total number of threads
    pthread_create(&threads[i], NULL, render_thread, thread_data);
    i++;
}


    // Start the MLX loop
    mlx_loop(mlx);
}
