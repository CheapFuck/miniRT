/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render3.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:32 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:33 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_ray	get_refraction_ray(t_vector point, t_vector normal,
	t_ray incoming_ray, double refractive_index)
{
	t_get_refraction_ray	vars;

	vars.cos_i = -dot(normal, incoming_ray.direction);
	vars.eta_i = 1.0;
	vars.eta_t = refractive_index;
	if (vars.cos_i < 0)
		refraction_helper(&vars, &normal);
	vars.eta = vars.eta_i / vars.eta_t;
	vars.k = 1 - vars.eta * vars.eta * (1 - vars.cos_i * vars.cos_i);
	if (vars.k < 0)
	{
		vars.refraction_ray.origin = point;
		vars.refraction_ray.direction = reflect(incoming_ray.direction, normal);
	}
	else
	{
		vars.refracted_dir = add(
				multiply_scalar(incoming_ray.direction, vars.eta),
				multiply_scalar(normal, (vars.eta * vars.cos_i
						- sqrt(vars.k))));
		vars.refraction_ray.origin = point;
		vars.refraction_ray.direction = normalize(vars.refracted_dir);
	}
	return (vars.refraction_ray);
}

t_hit_record	find_closest_intersection(t_ray ray, t_scene *scene)
{
	t_hit_record	hit;

	hit = (t_hit_record){0};
	hit.index = -1;
	hit.t = INFINITY;
	hit.hit = 0;
	hit.hit_from_inside = 0;
	check_sphere_intersections(ray, scene, &hit);
	check_cylinder_intersections(ray, scene, &hit);
	check_disc_intersections(ray, scene, &hit);
	check_plane_intersections(ray, scene, &hit);
	return (hit);
}

void	get_hit_normal_sphere(t_hit_record *hit, t_vector *normal,
	t_scene *scene)
{
	t_sphere	*sphere;

	if (hit->type == SPHERE)
	{
		sphere = &scene->spheres[hit->index];
		*normal = normalize(subtract(hit->point, sphere->center));
		hit->material = sphere->material;
	}
}

void	get_hit_normal_cylinder(t_hit_record *hit, t_vector *normal,
	t_scene *scene)
{
	t_cylinder	*cylinder;

	if (hit->type == CYLINDER)
	{
		cylinder = &scene->cylinders[hit->index];
		*normal = get_cylinder_normal(hit->point, cylinder);
		hit->material = cylinder->material;
	}
}

void	get_hit_normal_plane(t_hit_record *hit, t_vector *normal,
	t_scene *scene, t_ray ray)
{
	t_plane	*plane;

	if (hit->type == PLANE)
	{
		plane = &scene->planes[hit->index];
		*normal = plane->normal;
		if (dot(ray.direction, plane->normal) > 0)
			*normal = multiply_scalar(plane->normal, -1);
		hit->material = plane->material;
	}
}
