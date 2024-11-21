#include "../../includes/minirt.h"
#include <sys/time.h>

typedef struct s_render_data{
    mlx_t *mlx;
    mlx_image_t *img;
    t_scene *scene;
    int current_row; // Track the row being rendered
    int render_complete; // Flag to indicate rendering completion
    struct timeval start_time; // Start time of rendering
    struct timeval end_time;   // End time of rendering
} t_render_data;


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


void render_next_row(void *param)
{
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
                    final_color = apply_lighting(hit_point, normal, data->scene->spheres[i].color, data->scene);
                    // color = ft_pixel((final_color.r/255)*scene->spheres[i].color.r, (final_color.g/255)*scene->spheres[i].color.g, (final_color.b/255)*scene->spheres[i].color.b,0XFF );
                    // printf("Hit sphere at pixel (%d, %d)\n", x, y);
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
                    final_color = apply_lighting(hit_point, normal, data->scene->cylinders[i].color, data->scene);
                    // color = ft_pixel((final_color.r/255)*scene->cylinders[i].color.r, (final_color.g/255)*scene->cylinders[i].color.g, (final_color.b/255)*scene->cylinders[i].color.b,0XFF );
                    // printf("Hit cylinder at pixel (%d, %d)\n", x, y);
                    break;
                }
            }
            for (int i = 0; i < data->scene->num_planes; i++)
            {
                t_color black = {0, 0, 0}; // Black
                t_color white = {255, 255, 255}; // White
                double t_plane;

                if (intersect_plane(&ray, &data->scene->planes[i], &t_plane) && (!hit || t_plane < t))
                {
                    hit = 1;
                    t = t_plane;
                    t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                    t_vector normal = data->scene->planes[i].normal;
                    t_color object_color = get_checkerboard_color(hit_point, black, white, 1.0);
                    final_color = apply_lighting(hit_point, normal, object_color, data->scene);
                    // final_color = apply_lighting(hit_point, normal, scene->planes[i].color, scene);
                    // color = ft_pixel((final_color.r/255)*scene->planes[i].color.r, (final_color.g/255)*scene->planes[i].color.g, (final_color.b/255)*scene->planes[i].color.b,0XFF );
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
        else
        {
            uint32_t color = ft_pixel(
            rand() % 0xFF, // R
    	    rand() % 0xFF, // G
		    rand() % 0xFF, // B
		    rand() % 0xFF  // A
            );
            mlx_put_pixel(data->img, x, data->current_row, color);    
        }
    }
    data->current_row++;
    // Update the window with the current image
        mlx_image_to_window(data->mlx, data->img, 0, 0);
    }
    else  //dit veroorzaakt segfaults
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


void render_scene(mlx_t *mlx, t_scene *scene)
{
    // uint32_t color;

    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
        exit_with_error("Error creating image");
    // ft_memset(img->pixels, 255, img->width * img->height * BPP);
 t_render_data *data = malloc(sizeof(t_render_data));
    if (!data) {
        perror("Error allocating render data");
        exit(EXIT_FAILURE);
    }
    data->mlx = mlx;
    data->img = img;
    data->scene = scene;
    data->current_row = 0;
    data->render_complete = 0;

    // Register the rendering function to run in the event loop

    gettimeofday(&data->start_time, NULL);
    mlx_loop_hook(mlx, render_next_row, data);

    // Start the MLX event loop
    mlx_loop(mlx);

    // Clean up
    free(data);
}