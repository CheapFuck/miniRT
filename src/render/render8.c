/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render8.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:47 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:57:16 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_ray	create_ray(int x, int y, t_camera *camera)
{
	t_create_ray	vars;

	vars.aspect_ratio = (double)WIDTH / HEIGHT;
	vars.fov_scale = tan((camera->fov * M_PI / 180) / 2);
	vars.norm_x = (2 * (x + 0.5) / WIDTH - 1) * vars.aspect_ratio
		* vars.fov_scale;
	vars.norm_y = (1 - 2 * (y + 0.5) / HEIGHT) * vars.fov_scale;
	vars.right = normalize(cross((t_vector){0, 1, 0}, camera->orientation));
	vars.up = cross(camera->orientation, vars.right);
	vars.ray.origin = camera->pos;
	vars.ray.direction.x = vars.norm_x * vars.right.x + vars.norm_y * vars.up.x
		+ camera->orientation.x;
	vars.ray.direction.y = vars.norm_x * vars.right.y + vars.norm_y * vars.up.y
		+ camera->orientation.y;
	vars.ray.direction.z = vars.norm_x * vars.right.z + vars.norm_y * vars.up.z
		+ camera->orientation.z;
	vars.ray.direction = normalize(vars.ray.direction);
	return (vars.ray);
}

t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_vector	world_to_local(t_vector point, t_vector orientation,
	t_vector center)
{
	t_vector	local_point;
	t_vector	up;
	t_vector	right;
	t_vector	forward;
	t_vector	temp_vector;

	local_point = subtract(point, center);
	up = orientation;
	if (fabs(up.y) < 0.999)
		temp_vector = (t_vector){0, 1, 0};
	else
		temp_vector = (t_vector){1, 0, 0};
	right = normalize(cross(up, temp_vector));
	forward = cross(right, up);
	return ((t_vector)
		{
			dot(local_point, right),
			dot(local_point, up),
			dot(local_point, forward)
		});
}

void	update_display(void *param)
{
	t_render_data	*data;

	data = (t_render_data *)param;
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	pthread_mutex_lock(&data->mutex);
	if (data->rendering_finished)
	{
		mlx_terminate(data->mlx);
		pthread_mutex_unlock(&data->mutex);
		pthread_mutex_destroy(&data->mutex);
		free(data);
		exit(0);
	}
	pthread_mutex_unlock(&data->mutex);
}

void	init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene,
	t_render_data *data)
{
	data->mlx = mlx;
	data->img = img;
	data->scene = scene;
	data->threads_completed = 0;
	data->rendering_finished = 0;
	pthread_mutex_init(&data->mutex, NULL);
	gettimeofday(&data->start_time, NULL);
}
