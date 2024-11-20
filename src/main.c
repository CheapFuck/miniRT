#include "../includes/minirt.h"

// static void close_window(mlx_t *mlx)
// {
//     if (mlx) mlx_terminate(mlx);
//     exit(0);
// }

int main(int argc, char **argv)
{
    if (argc != 2) {
        exit_with_error("Usage: ./miniRT <scene.rt>");
    }

    // Initialize MLX42
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
    if (!mlx)
        exit_with_error("Error initializing MLX42");

    // Load and parse the scene
      t_scene scene;
    scene.num_spheres = 0;
    scene.num_cylinders = 0;
    scene.num_lights = 0;

    parse_file(argv[1], &scene);
    render_scene(mlx, &scene);
    mlx_loop(mlx);
    return 0;
}
