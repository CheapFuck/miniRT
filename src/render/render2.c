/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render2.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:29 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:29 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vector	reflect(t_vector direction, t_vector normal)
{
	double	dot_product;

	dot_product = dot(direction, normal);
	return (subtract(direction, multiply_scalar(normal, 2.0 * dot_product)));
}

t_color	combine_color(t_color light_color, t_color object_color)
{
	t_color	result;

	result.r = fmin(255, (light_color.r / 255.0) * object_color.r);
	result.g = fmin(255, (light_color.g / 255.0) * object_color.g);
	result.b = fmin(255, (light_color.b / 255.0) * object_color.b);
	return (result);
}

t_color	get_cylinder_checkerboard_color(t_vector point, t_cylinder *cylinder,
	double scale)
{
	if (is_cylinder_checkerboard(point, cylinder, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

int	is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder,
	double scale)
{
	t_cylinder_checkerboard	vars;

	vars.local_point = subtract(point, cylinder->center);
	vars.height = dot(vars.local_point, cylinder->orientation);
	vars.projection = multiply_scalar(cylinder->orientation, vars.height);
	vars.radial_vector = subtract(vars.local_point, vars.projection);
	vars.angle = atan2(vars.radial_vector.z, vars.radial_vector.x);
	vars.u = (vars.angle / (2.0 * M_PI)) + 0.5;
	vars.v = (vars.height + (cylinder->height / 2.0)) / cylinder->height;
	vars.scaled_u = vars.u * scale;
	vars.scaled_v = vars.v * scale;
	vars.u_check = (int)floor(vars.scaled_u) % 2;
	vars.v_check = (int)floor(vars.scaled_v) % 2;
	return ((vars.u_check + vars.v_check) % 2);
}

void	refraction_helper(t_get_refraction_ray	*vars, t_vector *normal)
{
	vars->cos_i = -vars->cos_i;
	vars->temp = vars->eta_i;
	vars->eta_i = vars->eta_t;
	vars->eta_t = vars->temp;
	*normal = multiply_scalar(*normal, -1);
}
