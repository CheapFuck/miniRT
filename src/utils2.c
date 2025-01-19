#include "../includes/minirt.h"

t_vector	normalize(t_vector v)
{
	double		length;
	t_vector	result;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	return (result);
}

double	length_squared(t_vector v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

t_vector	scale_vector(t_vector v, double s)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}