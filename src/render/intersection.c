#include "../../includes/minirt.h"

int	intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
	t_vector	oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		sqrt_discriminant;
	double		t1;
	double		t2;

	oc = subtract(ray->origin, sphere->center);
	a = dot(ray->direction, ray->direction);
	b = 2.0 * dot(oc, ray->direction);
	c = dot(oc, oc) - (sphere->radius * sphere->radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	sqrt_discriminant = sqrt(discriminant);
	t1 = (-b - sqrt_discriminant) / (2.0 * a);
	t2 = (-b + sqrt_discriminant) / (2.0 * a);
	if (t1 > 0 && t2 > 0)
		*t = fmin(t1, t2);
	else if (t1 > 0)
		*t = t1;
	else if (t2 > 0)
		*t = t2;
	else
		return (0);
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

int	intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
	t_vector	oc;
	t_vector	axis;
	double		dot_dir_axis;
	double		dot_oc_axis;
	t_vector	d;
	t_vector	o;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		sqrt_discriminant;
	double		t1;
	double		t2;
	t_vector	p1;
	double		height1;
	t_vector	p2;
	double		height2;
	int			hit;

	oc = subtract(ray->origin, cylinder->center);
	axis = normalize(cylinder->orientation);
	dot_dir_axis = dot(ray->direction, axis);
	dot_oc_axis = dot(oc, axis);
	d = subtract(ray->direction, multiply_scalar(axis, dot_dir_axis));
	o = subtract(oc, multiply_scalar(axis, dot_oc_axis));
	a = dot(d, d);
	b = 2.0 * dot(d, o);
	c = dot(o, o) - (cylinder->radius * cylinder->radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);
	sqrt_discriminant = sqrt(discriminant);
	t1 = (-b - sqrt_discriminant) / (2.0 * a);
	t2 = (-b + sqrt_discriminant) / (2.0 * a);
	p1 = add(ray->origin, multiply_scalar(ray->direction, t1));
	height1 = dot(subtract(p1, cylinder->center), axis);
	p2 = add(ray->origin, multiply_scalar(ray->direction, t2));
	height2 = dot(subtract(p2, cylinder->center), axis);
	hit = 0;
	if (t1 > 0 && fabs(height1) <= cylinder->height / 2)
	{
		*t = t1;
		hit = 1;
	}
	if (t2 > 0 && fabs(height2) <= cylinder->height / 2 && (!hit || t2 < *t))
	{
		*t = t2;
		hit = 1;
	}
	return (hit);
}
