#include "../../includes/minirt.h"

int	intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
	double		a;
	double		b;
	double		c;
	double		discriminant;
	t_vector	oc;

	oc = subtract(ray->origin, sphere->center);
	a = dot(ray->direction, ray->direction);
	b = 2.0 * dot(oc, ray->direction);
	c = dot(oc, oc) - (sphere->radius * sphere->radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (*t > 0)
		return (1);
	*t = (-b + sqrt(discriminant)) / (2.0 * a);
	return (*t > 0);
}

int	intersect_plane(t_ray *ray, t_plane *plane, double *t)
{
	double		denom;
	t_vector	p0_to_origin;
	double		t_temp;

	denom = dot(plane->normal, ray->direction);
	if (fabs(denom) < 1e-6)
		return (0);
	p0_to_origin = subtract(plane->point, ray->origin);
	t_temp = dot(p0_to_origin, plane->normal) / denom;
	if (t_temp < 0)
		return (0);
	*t = t_temp;
	return (1);
}

t_vector	scale_vector(t_vector v, double s)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}

int	intersect_disc(t_ray *ray, t_disc *disc, double *t)
{
	t_vector	oc;
	double		denom;
	double		t_plane;
	t_vector	p;
	double		distance_squared;

	denom = dot(ray->direction, disc->normal);
	if (fabs(denom) < 1e-6)
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

static double	calc_a(t_ray *ray, t_cylinder *cylinder)
{
	double	a;

	a = dot(ray->direction, ray->direction) - dot(ray->direction,
			cylinder->orientation) * dot(ray->direction,
			cylinder->orientation);
	return (a);
}

static double	calc_b(t_ray *ray, t_cylinder *cylinder, t_vector oc)
{
	double	b;

	b = 2.0 * (dot(ray->direction, oc) - dot(ray->direction,
				cylinder->orientation) * dot(oc, cylinder->orientation));
	return (b);
}

static double	calc_c(t_cylinder *cylinder, t_vector oc)
{
	double	c;

	c = dot(oc, oc) - dot(oc, cylinder->orientation) * dot(oc,
			cylinder->orientation) - cylinder->radius * cylinder->radius;
	return (c);
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
	double		a;
	double		b;
	double		c;
	double		discriminant;
	t_vector	oc;

	oc = subtract(ray->origin, cylinder->center);
	a = calc_a(ray, cylinder);
	b = calc_b(ray, cylinder, oc);
	c = calc_c(cylinder, oc);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	*t = (-b - sqrt(discriminant)) / (2 * a);
	if (*t > 0 && fabs(dot(subtract(add(ray->origin,
						multiply_scalar(ray->direction, *t)), cylinder->center),
				cylinder->orientation)) <= cylinder->height / 2)
		return (1);
	*t = (-b + sqrt(discriminant)) / (2 * a);
	if (*t > 0 && fabs(dot(subtract(add(ray->origin,
						multiply_scalar(ray->direction, *t)), cylinder->center),
				cylinder->orientation)) <= cylinder->height / 2)
		return (1);
	return (0);
}
