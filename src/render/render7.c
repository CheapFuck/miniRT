/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render7.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:44 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:45 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	apply_material_effects(t_material_params params)
{
	t_color	final_color;

	final_color = params.base_color;
	if (params.hit->material.reflectivity > 0.0)
		final_color = apply_reflection(params, final_color);
	if (params.hit->material.transparency > 0.0)
		final_color = apply_refraction(params, final_color);
	return (final_color);
}

double	schlick_reflection_coefficient(double cos_theta,
	double refractive_index)
{
	double	r0;

	r0 = pow((1 - refractive_index) / (1 + refractive_index), 2);
	return (r0 + (1 - r0) * pow((1 - cos_theta), 5));
}

static void	render_thread_loop(t_render_thread *vars)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			vars->pixel_index = y * WIDTH + x;
			if (vars->pixel_index % vars->num_threads == vars->thread_id)
			{
				vars->ray = create_ray(x, y, &vars->data->scene->camera);
				vars->final_color = trace_ray(vars->ray, vars->data->scene, 5);
				vars->color = (vars->final_color.r << 24)
					| (vars->final_color.g << 16)
					| (vars->final_color.b << 8) | 0xFF;
				pthread_mutex_lock(&vars->data->mutex);
				mlx_put_pixel(vars->data->img, x, y, vars->color);
				pthread_mutex_unlock(&vars->data->mutex);
			}
			x++;
		}
		y++;
	}
}

void	*render_thread(void *arg)
{
	t_render_thread	vars;

	vars = (t_render_thread){0};
	vars.thread_data = (t_thread_data *)arg;
	vars.data = vars.thread_data->render_data;
	vars.thread_id = vars.thread_data->thread_id;
	vars.num_threads = vars.thread_data->num_threads;
	render_thread_loop(&vars);
	pthread_mutex_lock(&vars.data->mutex);
	vars.data->threads_completed++;
	if (vars.data->threads_completed == NUM_THREADS)
	{
		gettimeofday(&vars.data->end_time, NULL);
		vars.elapsed_time = (vars.data->end_time.tv_sec
				- vars.data->start_time.tv_sec) + (vars.data->end_time.tv_usec
				- vars.data->start_time.tv_usec) / 1e6;
		printf("Rendering took %f seconds\n", vars.elapsed_time);
		save_image_to_file(vars.data->img->pixels, WIDTH, HEIGHT, "output.png");
		vars.data->rendering_finished = 1;
	}
	pthread_mutex_unlock(&vars.data->mutex);
	free(vars.thread_data);
	return (NULL);
}

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
