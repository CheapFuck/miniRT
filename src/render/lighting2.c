/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting2.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:20 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:21 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
{
	t_checkerboard_vertical	vars;

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
	if (vars.angle < 0)
		vars.angle += 2 * M_PI;
	vars.scaled_angle = vars.angle * (cylinder->radius / vars.grid_size);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.scaled_angle);
	return ((vars.u + vars.v) % 2);
}

int	is_plane_checkerboard(t_vector point, t_vector plane_normal, double scale)
{
	t_plane_checkerboard	vars;

	vars.grid_size = scale;
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, plane_normal)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, plane_normal));
	vars.y_axis = normalize(cross(plane_normal, vars.x_axis));
	vars.proj_x = dot(point, vars.x_axis);
	vars.proj_y = dot(point, vars.y_axis);
	vars.u = (int)floor(vars.proj_x / vars.grid_size);
	vars.v = (int)floor(vars.proj_y / vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

t_color	get_plane_checkerboard_color(t_vector point, t_vector normal,
	double scale)
{
	if (is_plane_checkerboard(point, normal, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

int	is_disc_checkerboard(t_vector point, t_disc *disc, double scale)
{
	t_disc_checkerboard	vars;

	vars.grid_size = scale;
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, disc->normal)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, disc->normal));
	vars.y_axis = normalize(cross(disc->normal, vars.x_axis));
	vars.proj_x = dot(point, vars.x_axis);
	vars.proj_y = dot(point, vars.y_axis);
	vars.u = (int)floor(vars.proj_x / vars.grid_size);
	vars.v = (int)floor(vars.proj_y / vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

t_color	get_disc_checkerboard_color(t_vector point, t_disc *disc, double scale)
{
	if (is_disc_checkerboard(point, disc, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}
