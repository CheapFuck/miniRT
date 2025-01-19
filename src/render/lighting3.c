/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting3.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:23 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:24 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	get_checkerboard_color(t_vector point, t_cylinder *cylinder,
	double scale)
{
	if (is_checkerboard(point, cylinder, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

static void	calc_in_shadow_vars(t_in_shadow *vars, t_light light,
t_vector hit_point)
{
	vars->light_dir = normalize(subtract(light.pos, hit_point));
	vars->light_distance = sqrt(dot(subtract(light.pos, hit_point),
				subtract(light.pos, hit_point)));
	vars->shadow_ray.origin = add(hit_point, multiply_scalar(vars->light_dir,
				EPSILON));
	vars->shadow_ray.direction = vars->light_dir;
	vars->i = 0;
}

int	is_in_shadow(t_vector hit_point, t_light light, t_scene *scene)
{
	t_in_shadow	vars;

	calc_in_shadow_vars(&vars, light, hit_point);
	while (vars.i < scene->num_spheres)
	{
		if (intersect_sphere(&vars.shadow_ray, &scene->spheres[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
		vars.i++;
	}
	vars.i = 0;
	while (vars.i < scene->num_cylinders)
	{
		if (intersect_cylinder(&vars.shadow_ray, &scene->cylinders[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
		vars.i++;
	}
	while (vars.i < scene->num_planes)
		if (intersect_plane(&vars.shadow_ray, &scene->planes[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
	vars.i++;
	return (0);
}

static void	apply_lighting_loop(t_apply_lighting *vars, t_scene *scene,
	t_vector hit_point, t_vector normal)
{
	while (vars->i < scene->num_lights)
	{
		vars->light = scene->lights[vars->i];
		vars->shadow_factor = compute_shadow_factor(hit_point, vars->light,
				scene, 8);
		if (vars->shadow_factor > 0)
		{
			vars->light_dir = normalize(subtract(vars->light.pos, hit_point));
			vars->diffuse_intensity = fmax(0.0, dot(normal, vars->light_dir))
				* vars->light.brightness * vars->shadow_factor;
			vars->reflect_dir = normalize(subtract(multiply_scalar(normal, 2.0
							* dot(normal, vars->light_dir)), vars->light_dir));
			vars->specular_intensity = pow(fmax(0.0, dot(vars->reflect_dir,
							vars->view_dir)), 50) * vars->light.brightness
				* vars->shadow_factor * 1;
			vars->light_contribution.r += vars->light.color.r
				* (vars->diffuse_intensity + vars->specular_intensity);
			vars->light_contribution.g += vars->light.color.g
				* (vars->diffuse_intensity + vars->specular_intensity);
			vars->light_contribution.b += vars->light.color.b
				* (vars->diffuse_intensity + vars->specular_intensity);
		}
		vars->i++;
	}
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
	t_color object_color, t_scene *scene)
{
	t_apply_lighting	vars;

	if (scene->dept > MAX_REFLECTION_DEPTH)
		return ((t_color){0, 0, 0});
	vars.light_contribution = (t_color){0, 0, 0};
	vars.light_contribution.r += 255 * scene->ambient.ratio;
	vars.light_contribution.g += 255 * scene->ambient.ratio;
	vars.light_contribution.b += 255 * scene->ambient.ratio;
	vars.view_dir = normalize(subtract(scene->camera.pos, hit_point));
	vars.i = 0;
	apply_lighting_loop(&vars, scene, hit_point, normal);
	return (combine_color(vars.light_contribution, object_color));
}
