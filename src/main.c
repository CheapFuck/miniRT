#include "../includes/minirt.h"

// static void close_window(mlx_t *mlx)
// {
//     if (mlx) mlx_terminate(mlx);
//     exit(0);
// }


void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(mlx);
		exit(EXIT_SUCCESS);
	}
}


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
    scene.num_cylinders = 0;
    scene.num_planes = 0;
    // scene.spheres->shininess = 1000;

    parse_file(argv[1], &scene);
   	mlx_loop_hook(mlx, ft_hook, mlx);
    render_scene(mlx, &scene);
    mlx_loop(mlx);
    return 0;
}
