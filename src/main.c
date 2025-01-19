/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:59 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:57:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	init_scene(t_scene *scene)
{
	scene->num_spheres = 0;
	scene->num_cylinders = 0;
	scene->num_discs = 0;
	scene->num_planes = 0;
	scene->num_lights = 0;
	scene->has_ambient = 0;
	scene->has_camera = 0;
	scene->has_light = 0;
}

void	ft_hook(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(mlx);
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv)
{
	mlx_t	*mlx;
	t_scene	scene;

	scene = (t_scene){0};
	if (argc != 2)
		exit_with_error("Usage: ./miniRT <scene.rt>");
	init_scene(&scene);
	parse_file(argv[1], &scene);
	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	if (!mlx)
		exit_with_error("Error initializing MLX42");
	mlx_loop_hook(mlx, ft_hook, mlx);
	render_scene(mlx, &scene);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
