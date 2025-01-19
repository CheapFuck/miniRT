/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render6.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:41 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:42 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	check_cylinder_intersections(t_ray ray, t_scene *scene,
	t_hit_record *hit)
{
	int		i;
	double	t_cylinder;

	i = 0;
	while (i < scene->num_cylinders)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cylinder)
			&& t_cylinder < hit->t)
		{
			hit->hit = 1;
			hit->t = t_cylinder;
			hit->type = CYLINDER;
			hit->index = i;
		}
		i++;
	}
}

void	check_disc_intersections(t_ray ray, t_scene *scene, t_hit_record *hit)
{
	int		i;
	double	t_disc;

	i = 0;
	while (i < scene->num_discs)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_disc(&ray, &scene->discs[i], &t_disc)
			&& t_disc < hit->t)
		{
			hit->hit = 1;
			hit->t = t_disc;
			hit->type = DISC;
			hit->index = i;
		}
		i++;
	}
}

void	check_plane_intersections(t_ray ray, t_scene *scene, t_hit_record *hit)
{
	int		i;
	double	t_plane;

	i = 0;
	while (i < scene->num_planes)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_plane(&ray, &scene->planes[i], &t_plane)
			&& t_plane < hit->t)
		{
			hit->hit = 1;
			hit->t = t_plane;
			hit->type = PLANE;
			hit->index = i;
		}
		i++;
	}
}

t_color	apply_reflection(t_material_params params, t_color base_color)
{
	t_ray	reflection_ray;
	t_color	reflected_color;

	reflection_ray = get_reflection_ray(params.hit->point,
			params.normal, params.ray);
	reflection_ray.origin = add(reflection_ray.origin,
			multiply_scalar(reflection_ray.direction, 0.001));
	reflected_color = trace_ray(reflection_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, reflected_color,
			params.hit->material.reflectivity));
}

t_color	apply_refraction(t_material_params params, t_color base_color)
{
	t_ray	refraction_ray;
	t_color	refracted_color;

	refraction_ray = get_refraction_ray(params.hit->point, params.normal,
			params.ray, params.hit->material.refractive_index);
	refraction_ray.origin = add(refraction_ray.origin,
			multiply_scalar(refraction_ray.direction, 0.001));
	refracted_color = trace_ray(refraction_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, refracted_color,
			params.hit->material.transparency));
}
