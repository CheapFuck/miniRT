/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting1.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:17 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:17 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vector	random_point_on_light(t_light light)
{
	double		theta;
	double		r;
	t_vector	random_point;

	theta = ((double)rand() / RAND_MAX) * 2 * M_PI;
	r = light.radius * sqrt((double)rand() / RAND_MAX);
	random_point.x = light.pos.x + r * cos(theta);
	random_point.y = light.pos.y + r * sin(theta);
	random_point.z = light.pos.z;
	return (random_point);
}

static void	calc_shadow_vars(t_compute_shadow_factor *vars, t_light light,
	t_vector hit_point)
{
	vars->light_point = random_point_on_light(light);
	vars->shadow_ray_dir = normalize(subtract(vars->light_point, hit_point));
	vars->light_distance = sqrt(dot(subtract(vars->light_point, hit_point),
				subtract(vars->light_point, hit_point)));
	vars->shadow_ray.origin = add(hit_point,
			multiply_scalar(vars->shadow_ray_dir, EPSILON));
	vars->shadow_ray.direction = vars->shadow_ray_dir;
	vars->in_shadow = 0;
	vars->j = 0;
}

double	compute_shadow_factor(t_vector hit_point, t_light light,
	t_scene *scene, int num_samples)
{
	t_compute_shadow_factor	vars;

	vars.unblocked_rays = 0;
	vars.i = 0;
	while (vars.i < num_samples)
	{
		calc_shadow_vars(&vars, light, hit_point);
		while (vars.j < scene->num_spheres)
		{
			if (intersect_sphere(&vars.shadow_ray, &scene->spheres[vars.j],
					&vars.t_shadow)
				&& vars.t_shadow < vars.light_distance)
			{
				vars.in_shadow = 1;
				break ;
			}
			vars.j++;
		}
		if (!vars.in_shadow)
			vars.unblocked_rays++;
		vars.i++;
	}
	return ((double)vars.unblocked_rays / num_samples);
}

int	is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
{
	if (fabs(dot((t_vector){0, 1, 0}, cylinder->orientation)) > 0.99)
		return (is_checkerboard_vertical(point, cylinder, scale));
	else
		return (is_checkerboard_horizontal(point, cylinder, scale));
}

int	is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder,
	double scale)
{
	t_checkerboard_horizontal	vars;

	vars.grid_size = scale;
	vars.local_point = subtract(point, cylinder->center);
	vars.height = dot(vars.local_point, cylinder->orientation);
	vars.radial = subtract(vars.local_point,
			multiply_scalar(cylinder->orientation, vars.height));
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, cylinder->orientation)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, cylinder->orientation));
	vars.y_axis = normalize(cross(cylinder->orientation, vars.x_axis));
	vars.proj_x = dot(vars.radial, vars.x_axis);
	vars.proj_y = dot(vars.radial, vars.y_axis);
	vars.angle = atan2(vars.proj_y, vars.proj_x);
	vars.normalized_angle = (vars.angle + M_PI) / (2 * M_PI);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.normalized_angle * (2 * M_PI * cylinder->radius)
			/ vars.grid_size);
	return ((vars.u + vars.v) % 2);
}
