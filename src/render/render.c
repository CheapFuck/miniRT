#include "../../includes/minirt.h"
#include <sys/time.h>
#include <pthread.h>

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

// Modify t_render_data to include thread management
// New function to handle rendering in threads
void *render_thread(void *arg)
{
    t_thread_data *thread_data = (t_thread_data *)arg;
    t_render_data *data = thread_data->render_data;
    t_color black = {0, 0, 0};
    t_color white = {255, 255, 255};

    for (int y = thread_data->start_row; y < thread_data->end_row; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            t_ray ray = create_ray(x, y, &data->scene->camera);
            double t;
            int hit = 0;
            t_color final_color = {0, 0, 0};

            // Check sphere intersections
            for (int i = 0; i < data->scene->num_spheres; i++)
            {
                if (intersect_sphere(&ray, &data->scene->spheres[i], &t))
                {
                    hit = 1;
                    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                    t_vector normal = normalize(subtract(hit_point, data->scene->spheres[i].center));
                    
                    if (data->scene->spheres[i].checker == 1)
                    {
                        t_vector local_point = subtract(hit_point, data->scene->spheres[i].center);
                        double u = 0.5 + atan2(local_point.z, local_point.x) / (2 * M_PI);
                        double v = 0.5 - asin(local_point.y / data->scene->spheres[i].radius) / M_PI;
                        
                        int check_u = (int)(u * 10.0) % 2;
                        int check_v = (int)(v * 10.0) % 2;
                        
                        t_color object_color = (check_u == check_v) ? white : black;
                        final_color = apply_lighting(hit_point, normal, object_color, data->scene);
                    }
                    else
                    {
                        final_color = apply_lighting(hit_point, normal, data->scene->spheres[i].color, data->scene);
                    }
                    break;
                }
            }
   for (int i = 0; i <= data->scene->num_cylinders; i++)
            {
                double t_cy;
if (intersect_cylinder(&ray, &data->scene->cylinders[i], &t_cy) && (!hit || t_cy < t))
{
    hit = 1;
    t = t_cy;
    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
    t_vector normal = normalize(subtract(hit_point, data->scene->cylinders[i].center));

    if (data->scene->cylinders[i].checker == 1)
    {
        // Transform hit_point to the cylinder's local coordinate system
        t_vector local_point = world_to_local(hit_point, data->scene->cylinders[i].orientation, data->scene->cylinders[i].center);

        // Cylindrical UV mapping in local space
        double theta = atan2(local_point.z, local_point.x);
        double u = 0.5 + theta / (2 * M_PI); // Maps theta [-pi, pi] to [0, 1]
        double v = local_point.y / data->scene->cylinders[i].height; // Scaled along height

        // Scale the checker pattern
        double scale = 10.0; // Adjust scale to fit your scene
        double scaled_u = u * scale;
        double scaled_v = v * scale;

        int check_u = (int)scaled_u % 2;
        int check_v = (int)scaled_v % 2;

        // Checkerboard color
        t_color base_color = (check_u == check_v) ? white : black;

        // Apply lighting
        final_color = apply_lighting(hit_point, normal, base_color, data->scene);
    }
    else
    {
        // Default cylinder color
        final_color = apply_lighting(hit_point, normal, data->scene->cylinders[i].color, data->scene);
    }

    break;
}



            }
            for (int i = 0; i < data->scene->num_planes; i++)
            {
                double t_plane;

                if (intersect_plane(&ray, &data->scene->planes[i], &t_plane) && (!hit || t_plane < t))
                {
                    hit = 1;
                    t = t_plane;
                    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                    t_vector normal = data->scene->planes[i].normal;
                    
                    if(data->scene->planes[i].checker == 1)
                    {
                        t_color object_color = get_checkerboard_color(hit_point, black, white, 1.0);
                        final_color = apply_lighting(hit_point, normal, object_color, data->scene);
                    }
                    else
					{
                        final_color = apply_lighting(hit_point, normal, data->scene->planes[i].color, data->scene);
                    
					}// color = ft_pixel((final_color.r/255)*scene->planes[i].color.r, (final_color.g/255)*scene->planes[i].color.g, (final_color.b/255)*scene->planes[i].color.b,0XFF );
                }
        }

            // Similar checks for cylinders and planes...
            // (Copy your existing intersection checks here)

            if (hit)
            {
                uint32_t color = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
                pthread_mutex_lock(&data->mutex);
                mlx_put_pixel(data->img, x, y, color);
                pthread_mutex_unlock(&data->mutex);
            }
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




// typedef struct s_render_data{
//     mlx_t *mlx;
//     mlx_image_t *img;
//     t_scene *scene;
//     int current_row; // Track the row being rendered
//     int render_complete; // Flag to indicate rendering completion
//     struct timeval start_time; // Start time of rendering
//     struct timeval end_time;   // End time of rendering
// } t_render_data;


int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

// // Function to draw a pixel on the screen
// static void draw_pixel(mlx_image_t *img, int x, int y, uint32_t color) {
//     if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
//         mlx_put_pixel(img, x, y, color);
// }

// Function to create a ray from the camera for a specific pixel
t_ray create_ray(int x, int y, t_camera *camera) {
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


void render_next_row(void *param)
{
    t_color black = {0, 0, 0}; // Black
    t_color white = {255, 255, 255}; // White
    t_render_data *data = (t_render_data *)param;
    int i;
    unsigned char* image_data = malloc(WIDTH * HEIGHT * 4); // Allocate RGBA image buffer
    
    // Render the current row
    // printf("HEIGHT is: %i\ncurrent_row: %i\n", HEIGHT, data->current_row);
    if (data->current_row < HEIGHT)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            t_ray ray = create_ray(x, data->current_row, &data->scene->camera);
            double t;
            int hit = 0;
            t_color final_color = {0, 0, 0};
            for (i = 0; i < data->scene->num_spheres; i++)
            {
  if (intersect_sphere(&ray, &data->scene->spheres[i], &t))
{
    hit = 1;
    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
    t_vector normal = normalize(subtract(hit_point, data->scene->spheres[i].center));
    
    if (data->scene->spheres[i].checker == 1)
    {
        // Calculate UV coordinates for the sphere
        t_vector local_point = subtract(hit_point, data->scene->spheres[i].center);
        double u = 0.5 + atan2(local_point.z, local_point.x) / (2 * M_PI);
        double v = 0.5 - asin(local_point.y / data->scene->spheres[i].radius) / M_PI;

        // Checkerboard pattern scaling
        int check_u = (int)(u * 10.0) % 2;  // Adjust `10.0` to control size of pattern
        int check_v = (int)(v * 10.0) % 2;

        // Determine the checker color
        t_color object_color;
        if (check_u == check_v)
            object_color = white;
        else
            object_color = black;

        // Apply lighting to the checkerboard pattern
        final_color = apply_lighting(hit_point, normal, object_color, data->scene);
    }
    else
    {
        // Default sphere color without checkerboard pattern
        final_color = apply_lighting(hit_point, normal, data->scene->spheres[i].color, data->scene);
    }

    break;
}

            }
            for (i = 0; i <= data->scene->num_cylinders; i++)
            {
                double t_cy;
if (intersect_cylinder(&ray, &data->scene->cylinders[i], &t_cy) && (!hit || t_cy < t))
{
    hit = 1;
    t = t_cy;
    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
    t_vector normal = normalize(subtract(hit_point, data->scene->cylinders[i].center));

    if (data->scene->cylinders[i].checker == 1)
    {
        // Transform hit_point to the cylinder's local coordinate system
        t_vector local_point = world_to_local(hit_point, data->scene->cylinders[i].orientation, data->scene->cylinders[i].center);

        // Cylindrical UV mapping in local space
        double theta = atan2(local_point.z, local_point.x);
        double u = 0.5 + theta / (2 * M_PI); // Maps theta [-pi, pi] to [0, 1]
        double v = local_point.y / data->scene->cylinders[i].height; // Scaled along height

        // Scale the checker pattern
        double scale = 10.0; // Adjust scale to fit your scene
        double scaled_u = u * scale;
        double scaled_v = v * scale;

        int check_u = (int)scaled_u % 2;
        int check_v = (int)scaled_v % 2;

        // Checkerboard color
        t_color base_color = (check_u == check_v) ? white : black;

        // Apply lighting
        final_color = apply_lighting(hit_point, normal, base_color, data->scene);
    }
    else
    {
        // Default cylinder color
        final_color = apply_lighting(hit_point, normal, data->scene->cylinders[i].color, data->scene);
    }

    break;
}



            }
            for (int i = 0; i < data->scene->num_planes; i++)
            {
                double t_plane;

                if (intersect_plane(&ray, &data->scene->planes[i], &t_plane) && (!hit || t_plane < t))
                {
                    hit = 1;
                    t = t_plane;
                    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                    t_vector normal = data->scene->planes[i].normal;
                    
                    if(data->scene->planes[i].checker == 1)
                    {
                        t_color object_color = get_checkerboard_color(hit_point, black, white, 1.0);
                        final_color = apply_lighting(hit_point, normal, object_color, data->scene);
                    }
                    else
					{
                        final_color = apply_lighting(hit_point, normal, data->scene->planes[i].color, data->scene);
                    
					}// color = ft_pixel((final_color.r/255)*scene->planes[i].color.r, (final_color.g/255)*scene->planes[i].color.g, (final_color.b/255)*scene->planes[i].color.b,0XFF );
                }
        }
        if (hit)
        {
            // color = ft_pixel(scene->spheres[i].color.r, scene->spheres[i].color.g, scene->spheres[i].color.b,0XFF );
            // color_temp = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            // color = color * color_temp;
            uint32_t color = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            mlx_put_pixel(data->img, x, data->current_row, color); // Red silhouette
        }
        // else
        // {
        //     uint32_t color = ft_pixel(
        //     rand() % 0xFF, // R
    	//     rand() % 0xFF, // G
		//     rand() % 0xFF, // B
		//     rand() % 0xFF  // A
        //     );
        //     mlx_put_pixel(data->img, x, data->current_row, color);    
        // }
    }
    data->current_row++;
    // Update the window with the current image
        mlx_image_to_window(data->mlx, data->img, 0, 0);
    }
    else
    {
        gettimeofday(&data->end_time, NULL);
        data->render_complete = 1;
        double elapsed_time = (data->end_time.tv_sec - data->start_time.tv_sec) + (data->end_time.tv_usec - data->start_time.tv_usec) / 1e6;
        printf("Rendering took %f seconds\n", elapsed_time);
        // unsigned char* image_data = malloc(WIDTH * HEIGHT * 4); // Allocate RGBA image buffer
        // unsigned char* raw_image_data = (unsigned char*)data->img->pixels;
        save_image_to_file(data->img->pixels, WIDTH, HEIGHT, "output.png");
        // Clean up
        free(image_data);
        mlx_terminate(data->mlx);
        free(data);
        exit(0);
    }
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
