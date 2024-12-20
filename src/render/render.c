// #include "../../includes/minirt.h"


// int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
// {
//     return (r << 24 | g << 16 | b << 8 | a);
// }

// // // Function to draw a pixel on the screen
// // static void draw_pixel(mlx_image_t *img, int x, int y, uint32_t color) {
// //     if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
// //         mlx_put_pixel(img, x, y, color);
// // }

// // Function to create a ray from the camera for a specific pixel
// t_ray create_ray(int x, int y, t_camera *camera) {
//     t_ray ray;
//     ray.origin = camera->pos;

//     // Calculate direction from camera to pixel (simple perspective projection)
//     ray.direction.x = (2 * (x + 0.5) / (double)WIDTH - 1) * tan(camera->fov / 2 * M_PI / 180);
//     ray.direction.y = (1 - 2 * (y + 0.5) / (double)HEIGHT) * tan(camera->fov / 2 * M_PI / 180);
//     ray.direction.z = 1; // Assume camera is looking along the positive z-axis
//     ray.direction = normalize(ray.direction);

//     return ray;
// }



// // void render_scene(mlx_t *mlx, t_scene *scene)
// // {
// //     (void)scene;
// //     mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
// //     if (!img) exit_with_error("Error creating image");

// //     // Example rendering: Set all pixels to black
// //     for (int y = 0; y < HEIGHT; y++) {
// //         for (int x = 0; x < WIDTH; x++) {
// //             // uint32_t color = 0x000000FF; // Black color
// //         t_ray ray = create_ray(x, y, &scene->camera);
// //         double t;
// //         int hit = 0;

// //         for (int i = 0; i < scene->num_spheres; i++)
// //         {
// //             printf("D\n");
// //             if (intersect_sphere(&ray, &scene->spheres[i], &t))
// //             {
// //                 hit = 1;
// //                 printf("Hit sphere at pixel (%d, %d)\n", x, y);
// //                 break;
// //             }
// //         }

// //         if (hit)
// //     {
// //             mlx_put_pixel(img, x, y, 0xFF0000FF); // Red silhouette

// //     }


// //             uint32_t color = ft_pixel(
// // 				rand() % 0xFF, // R
// // 				rand() % 0xFF, // G
// // 				rand() % 0xFF, // B
// // 				rand() % 0xFF  // A
// //                 );
// //             mlx_put_pixel(img, x, y, color);
// //         }
// //     }
// //     mlx_image_to_window(mlx, img, 0, 0);
// // }



// void render_scene(mlx_t *mlx, t_scene *scene)
// {
//     uint32_t color;
//     int i;

//     mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
//     if (!img)
//         exit_with_error("Error creating image");
//     // ft_memset(img->pixels, 255, img->width * img->height * BPP);
 
//  for (int y = 0; y < HEIGHT; y++)
//  {
//     for (int x = 0; x < WIDTH; x++)
//     {
//         t_ray ray = create_ray(x, y, &scene->camera);
//         double t;
//         int hit = 0;
//         t_color final_color = {0, 0, 0};
        
//         for (i = 0; i < scene->num_spheres; i++)
//         {
//             if (intersect_sphere(&ray, &scene->spheres[i], &t))
//             {
//                 hit = 1;
         
//                 t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
//                 t_vector normal = normalize(subtract(hit_point, scene->spheres[i].center));
//                 final_color = apply_lighting(hit_point, normal, scene->spheres[i].color, scene);
//           color = ft_pixel(scene->spheres[i].color.r, scene->spheres[i].color.g, scene->spheres[i].color.b,0XFF );
//                 // printf("Hit sphere at pixel (%d, %d)\n", x, y);
//                 break;
//             }
//         }
//         for (i = 0; i <= scene->num_cylinders; i++)
//         {
//             double t_cy;
//             if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cy) && (!hit || t_cy < t)) {
//                 hit = 1;
//                 t = t_cy;
//                 t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
//                 t_vector normal = normalize(subtract(hit_point, scene->spheres[i].center));
//                 final_color = apply_lighting(hit_point, normal, scene->spheres[i].color, scene);
               
//                 color = ft_pixel(scene->cylinders[i].color.r, scene->cylinders[i].color.g, scene->cylinders[i].color.b,0XFF );
//                 // printf("Hit cylinder at pixel (%d, %d)\n", x, y);
//                 break;
//             }
//         }
// for (int i = 0; i < scene->num_planes; i++) {
//     double t_plane;
//     if (intersect_plane(&ray, &scene->planes[i], &t_plane) && (!hit || t_plane < t)) {
//         hit = 1;
//         t = t_plane;
//         t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
//         t_vector normal = normalize(subtract(hit_point, scene->spheres[i].center));
//         final_color = apply_lighting(hit_point, normal, scene->spheres[i].color, scene);
         
//         color = ft_pixel(scene->planes[i].color.r, scene->planes[i].color.g, scene->planes[i].color.b,0XFF );
//     }
// }



//     uint32_t color_temp;

//         if (hit)
//         {
//             // color = ft_pixel(scene->spheres[i].color.r, scene->spheres[i].color.g, scene->spheres[i].color.b,0XFF );
//             color_temp = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
//             // color = color + color_temp;
//             mlx_put_pixel(img, x, y, color_temp); // Red silhouette
//         }
//         else
//         {
//             uint32_t color = ft_pixel(
// 			rand() % 0xFF, // R
// 			rand() % 0xFF, // G
// 			rand() % 0xFF, // B
// 			rand() % 0xFF  // A
//             );
//             mlx_put_pixel(img, x, y, color);
//         }
//     }
// }

//     mlx_image_to_window(mlx, img, 0, 0);
// }





// WIP:

#include "../../includes/minirt.h"


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

void render_scene(mlx_t *mlx, t_scene *scene)
{
    // uint32_t color;
    int i;

    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
        exit_with_error("Error creating image");
    // ft_memset(img->pixels, 255, img->width * img->height * BPP);
 
 for (int y = 0; y < HEIGHT; y++)
 {
    for (int x = 0; x < WIDTH; x++)
    {
        t_ray ray = create_ray(x, y, &scene->camera);
        double t;
        int hit = 0;
        t_color final_color = {0, 0, 0};
        
        for (i = 0; i < scene->num_spheres; i++)
        {
            if (intersect_sphere(&ray, &scene->spheres[i], &t))
            {
                hit = 1;
         
                t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                t_vector normal = normalize(subtract(hit_point, scene->spheres[i].center));
                final_color = apply_lighting(hit_point, normal, scene->spheres[i].color, scene);
                // color = ft_pixel((final_color.r/255)*scene->spheres[i].color.r, (final_color.g/255)*scene->spheres[i].color.g, (final_color.b/255)*scene->spheres[i].color.b,0XFF );
                // printf("Hit sphere at pixel (%d, %d)\n", x, y);
                break;
            }
        }
        for (i = 0; i <= scene->num_cylinders; i++)
        {
            double t_cy;
            if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cy) && (!hit || t_cy < t)) {
                hit = 1;
                t = t_cy;
                t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
                t_vector normal = normalize(subtract(hit_point, scene->cylinders[i].center));
                final_color = apply_lighting(hit_point, normal, scene->cylinders[i].color, scene);
               
                // color = ft_pixel((final_color.r/255)*scene->cylinders[i].color.r, (final_color.g/255)*scene->cylinders[i].color.g, (final_color.b/255)*scene->cylinders[i].color.b,0XFF );
                // printf("Hit cylinder at pixel (%d, %d)\n", x, y);
                break;
            }
        }
for (int i = 0; i < scene->num_planes; i++) {
    double t_plane;
    if (intersect_plane(&ray, &scene->planes[i], &t_plane) && (!hit || t_plane < t)) {
        hit = 1;
        t = t_plane;
        t_vector hit_point = add(ray.origin, multiply_scalar(ray.direction, t));
        t_vector normal = scene->planes[i].normal;
        final_color = apply_lighting(hit_point, normal, scene->planes[i].color, scene);
         
        // color = ft_pixel((final_color.r/255)*scene->planes[i].color.r, (final_color.g/255)*scene->planes[i].color.g, (final_color.b/255)*scene->planes[i].color.b,0XFF );
    }
}



    // uint32_t color_temp;

        if (hit)
        {
            // color = ft_pixel(scene->spheres[i].color.r, scene->spheres[i].color.g, scene->spheres[i].color.b,0XFF );
            // color_temp = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            // color = color * color_temp;
            uint32_t color = (final_color.r << 24) | (final_color.g << 16) | (final_color.b << 8) | 0xFF;
            mlx_put_pixel(img, x, y, color); // Red silhouette
        }
        else
        {
            uint32_t color = ft_pixel(
			rand() % 0xFF, // R
			rand() % 0xFF, // G
			rand() % 0xFF, // B
			rand() % 0xFF  // A
            );
            mlx_put_pixel(img, x, y, color);
        }
    }
}

    mlx_image_to_window(mlx, img, 0, 0);
}