/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render4.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:35 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:36 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	get_hit_normal_disc(t_hit_record *hit, t_vector *normal,
	t_scene *scene, t_ray ray)
{
	t_disc	*disc;

	if (hit->type == DISC)
	{
		disc = &scene->discs[hit->index];
		*normal = disc->normal;
		if (dot(ray.direction, disc->normal) > 0)
			*normal = multiply_scalar(disc->normal, -1);
		hit->material = disc->material;
	}
}

void	get_hit_normal(t_hit_record *hit, t_ray ray, t_vector *normal,
	t_scene *scene)
{
	get_hit_normal_sphere(hit, normal, scene);
	get_hit_normal_cylinder(hit, normal, scene);
	get_hit_normal_plane(hit, normal, scene, ray);
	get_hit_normal_disc(hit, normal, scene, ray);
}

t_color	get_surface_color_sphere(t_scene *scene, t_hit_record *hit)
{
	t_sphere	*sphere;
	t_vector	local_point;
	double		u;
	double		v;

	sphere = &scene->spheres[hit->index];
	if (sphere->material.checker == 1)
	{
		local_point = subtract(hit->point, sphere->center);
		u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
		v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
		if ((int)(u * 20.0) % 2 == (int)(v * 20.0) % 2)
			return ((t_color){255, 255, 255});
		else
			return ((t_color){0, 0, 0});
	}
	else
		return (sphere->material.color);
}

t_color	get_surface_color_cylinder(t_scene *scene, t_hit_record *hit)
{
	t_cylinder	*cylinder;
	t_color		black;
	t_color		white;

	white = (t_color){255, 255, 255};
	black = (t_color){0, 0, 0};
	cylinder = &scene->cylinders[hit->index];
	if (cylinder->material.checker == 1)
	{
		if (is_checkerboard(hit->point, cylinder, 0.5))
			return (black);
		else
			return (white);
	}
	else
		return (cylinder->material.color);
}

t_color	get_surface_color_plane(t_scene *scene, t_hit_record *hit,
	t_vector normal)
{
	t_plane	*plane;

	plane = &scene->planes[hit->index];
	if (plane->material.checker == 1)
		return (get_plane_checkerboard_color(hit->point, normal, 0.5));
	else
		return (plane->material.color);
}
