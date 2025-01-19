/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render5.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:38 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:39 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static t_color	get_surface_color_disc(t_scene *scene, t_hit_record *hit)
{
	t_disc	*disc;

	disc = &scene->discs[hit->index];
	if (disc->material.checker == 1)
		return (get_disc_checkerboard_color(hit->point, disc, 0.5));
	else
		return (disc->material.color);
}

t_color	get_surface_color(t_hit_record *hit, t_vector normal,
	t_scene *scene, int depth)
{
	t_color	black;
	t_color	object_color;

	scene->dept = depth;
	black = (t_color){0, 0, 0};
	object_color = black;
	if (hit->type == SPHERE)
		object_color = get_surface_color_sphere(scene, hit);
	else if (hit->type == CYLINDER)
		object_color = get_surface_color_cylinder(scene, hit);
	else if (hit->type == PLANE)
		object_color = get_surface_color_plane(scene, hit, normal);
	else if (hit->type == DISC)
		object_color = get_surface_color_disc(scene, hit);
	return (apply_lighting(hit->point, normal, object_color, scene));
}

t_color	calculate_object_color(t_hit_record *hit, t_ray ray,
	t_scene *scene, int depth)
{
	t_vector			normal;
	t_color				final_color;
	t_material_params	params;

	normal = (t_vector){0, 0, 0};
	get_hit_normal(hit, ray, &normal, scene);
	final_color = get_surface_color(hit, normal, scene, depth);
	if (hit->material.reflectivity > 0.0 || hit->material.transparency > 0.0)
	{
		params.hit = hit;
		params.ray = ray;
		params.normal = normal;
		params.scene = scene;
		params.base_color = final_color;
		params.depth = depth;
		final_color = apply_material_effects(params);
	}
	return (final_color);
}

t_color	trace_ray(t_ray ray, t_scene *scene, int depth)
{
	t_hit_record	hit;
	t_color			black;

	black = (t_color){0, 0, 0};
	if (depth > MAX_REFLECTION_DEPTH)
		return (black);
	hit = find_closest_intersection(ray, scene);
	if (hit.hit)
	{
		hit.point = add(ray.origin,
				multiply_scalar(ray.direction, hit.t));
		return (calculate_object_color(&hit, ray, scene, depth));
	}
	return (black);
}

void	check_sphere_intersections(t_ray ray, t_scene *scene,
	t_hit_record *hit)
{
	int		i;
	double	t_sphere;

	i = 0;
	while (i < scene->num_spheres)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere)
			&& t_sphere < hit->t)
		{
			hit->hit = 1;
			hit->t = t_sphere;
			hit->type = SPHERE;
			hit->index = i;
		}
		i++;
	}
}
