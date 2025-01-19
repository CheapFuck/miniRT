/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render1.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:26 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:27 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_ray	get_reflection_ray(t_vector hit_point, t_vector normal,
	t_ray incident_ray)
{
	t_ray		reflection;
	double		dot_product;
	t_vector	reflection_dir;

	reflection.origin = hit_point;
	dot_product = dot(incident_ray.direction, normal);
	reflection_dir = subtract(incident_ray.direction,
			multiply_scalar(normal, 2.0 * dot_product));
	reflection.direction = normalize(reflection_dir);
	return (reflection);
}

t_color	uint32_to_t_color(uint32_t color)
{
	return ((t_color)
		{
			.r = (color >> 24) & 0xFF,
			.g = (color >> 16) & 0xFF,
			.b = (color >> 8) & 0xFF
		});
}

t_color	blend_colors(t_color original_color, t_color reflected_color,
	float reflectivity)
{
	return ((t_color)
		{
			.r = reflectivity * reflected_color.r + (1 - reflectivity)
			* original_color.r,
			.g = reflectivity * reflected_color.g + (1 - reflectivity)
			* original_color.g,
			.b = reflectivity * reflected_color.b + (1 - reflectivity)
			* original_color.b
		});
}

t_vector	refract(t_vector incident, t_vector normal, float eta_ratio)
{
	double		cos_theta;
	double		sin_theta2;
	t_vector	r_out_perp;
	t_vector	r_out_parallel;

	cos_theta = fmin(dot(multiply_scalar(incident, -1.0), normal), 1.0);
	sin_theta2 = eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta);
	if (sin_theta2 > 1.0)
	{
		return (reflect(incident, normal));
	}
	r_out_perp = multiply_scalar(add(incident, multiply_scalar(normal,
					cos_theta)), eta_ratio);
	r_out_parallel = multiply_scalar(normal, -sqrt(fabs(1.0 - dot(r_out_perp,
						r_out_perp))));
	return (add(r_out_perp, r_out_parallel));
}

t_vector	get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder)
{
	t_vector	axis;
	t_vector	cp;
	t_vector	axis_point;
	t_vector	normal;
	double		dot_prod;

	axis = normalize(cylinder->orientation);
	cp = subtract(hit_point, cylinder->center);
	dot_prod = dot(cp, axis);
	axis_point = add(cylinder->center, multiply_scalar(axis, dot_prod));
	normal = normalize(subtract(hit_point, axis_point));
	return (normal);
}
