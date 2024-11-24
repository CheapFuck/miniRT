#include "../includes/minirt.h"

// static void close_window(mlx_t *mlx)
// {
//     if (mlx) mlx_terminate(mlx);
//     exit(0);
// }

int main(int argc, char **argv) {
    if (argc != 2) {
        exit_with_error("Usage: ./miniRT <scene.rt>");
    }

    // Initialize MLX42
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
    if (!mlx)
        exit_with_error("Error initializing MLX42");

    // Create image buffer
    mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img) {
        exit_with_error("Error creating image buffer");
    }
    mlx_image_to_window(mlx, img, 0, 0); // Attach image to window

    // Load and parse the scene
    t_scene scene = {0};
    scene.num_spheres = 0;
    scene.num_cylinders = 0;
    scene.num_lights = 0;

    parse_file(argv[1], &scene);

    // Prepare render data
    t_render_data render_data = {0};
    render_data.img = img;
    render_data.scene = &scene;

    // Render the scene
    render_scene(&render_data);

    // Start the MLX42 event loop
    mlx_loop(mlx);

    // Clean up resources (if needed)
    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);

    return 0;
}
