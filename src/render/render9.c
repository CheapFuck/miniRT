/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render9.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:50 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:57:31 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	render_scene(mlx_t *mlx, t_scene *scene)
{
	int		i;
	pthread_t		threads[NUM_THREADS];
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
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data = malloc(sizeof(t_thread_data));
		thread_data->render_data = data;
		thread_data->thread_id = i;
		thread_data->num_threads = NUM_THREADS;
		pthread_create(&threads[i], NULL, render_thread, thread_data);
		i++;
	}
	mlx_loop(mlx);
}
