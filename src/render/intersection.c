/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   intersection.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:11 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:12 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
	t_vector	oc;
	double		discriminant;
	double		t1;
	double		t2;

	oc = subtract(ray->origin, sphere->center);
	discriminant = pow(dot(oc, ray->direction), 2) - dot(oc, oc)
		+ sphere->radius * sphere->radius;
	if (discriminant < 0)
		return (0);
	discriminant = sqrt(discriminant);
	t1 = (-dot(oc, ray->direction) - discriminant) / dot(ray->direction,
			ray->direction);
	t2 = (-dot(oc, ray->direction) + discriminant) / dot(ray->direction,
			ray->direction);
	if (t1 > 0 && t2 > 0)
		*t = fmin(t1, t2);
	else if (t1 > 0)
		*t = t1;
	else if (t2 > 0)
		*t = t2;
	else
		return (0);
	return (1);
}

int	intersect_plane(t_ray *ray, t_plane *plane, double *t)
{
	double		denom;
	t_vector	p0_to_origin;
	double		t_temp;

	denom = dot(plane->normal, ray->direction);
	if (fabs(denom) < EPSILON)
		return (0);
	p0_to_origin = subtract(plane->point, ray->origin);
	t_temp = dot(p0_to_origin, plane->normal) / denom;
	if (t_temp < 0)
		return (0);
	*t = t_temp;
	return (1);
}

int	intersect_disc(t_ray *ray, t_disc *disc, double *t)
{
	t_vector	oc;
	double		denom;
	double		t_plane;
	t_vector	p;
	double		distance_squared;

	denom = dot(ray->direction, disc->normal);
	if (fabs(denom) < EPSILON)
		return (0);
	oc = subtract(disc->center, ray->origin);
	t_plane = dot(oc, disc->normal) / denom;
	if (t_plane <= 0)
		return (0);
	p = add(ray->origin, scale_vector(ray->direction, t_plane));
	distance_squared = length_squared(subtract(p, disc->center));
	if (distance_squared > disc->radius * disc->radius)
		return (0);
	*t = t_plane;
	return (1);
}

static void	calc_vars(t_intersect_cylinder *vars, t_cylinder *cylinder,
	t_ray *ray)
{
	vars->oc = subtract(ray->origin, cylinder->center);
	vars->axis = normalize(cylinder->orientation);
	vars->dot_dir_axis = dot(ray->direction, vars->axis);
	vars->dot_oc_axis = dot(vars->oc, vars->axis);
	vars->d = subtract(ray->direction, multiply_scalar(vars->axis,
				vars->dot_dir_axis));
	vars->o = subtract(vars->oc, multiply_scalar(vars->axis,
				vars->dot_oc_axis));
	vars->a = dot(vars->d, vars->d);
	vars->b = 2.0 * dot(vars->d, vars->o);
	vars->c = dot(vars->o, vars->o) - (cylinder->radius * cylinder->radius);
	vars->discriminant = vars->b * vars->b - 4 * vars->a * vars->c;
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
	t_intersect_cylinder	vars;

	calc_vars(&vars, cylinder, ray);
	if (vars.discriminant < 0)
		return (0);
	vars.sqrt_discriminant = sqrt(vars.discriminant);
	vars.t1 = (-vars.b - vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.t2 = (-vars.b + vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.p1 = add(ray->origin, multiply_scalar(ray->direction, vars.t1));
	vars.height1 = dot(subtract(vars.p1, cylinder->center), vars.axis);
	vars.p2 = add(ray->origin, multiply_scalar(ray->direction, vars.t2));
	vars.height2 = dot(subtract(vars.p2, cylinder->center), vars.axis);
	vars.hit = 0;
	if (vars.t1 > 0 && fabs(vars.height1) <= cylinder->height / 2)
	{
		*t = vars.t1;
		vars.hit = 1;
	}
	if (vars.t2 > 0 && fabs(vars.height2) <= cylinder->height / 2
		&& (!vars.hit || vars.t2 < *t))
	{
		*t = vars.t2;
		vars.hit = 1;
	}
	return (vars.hit);
}
