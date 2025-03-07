#ifndef OBJECTS_H
# define OBJECTS_H

# include "minirt.h"

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_ambient
{
	double	ratio;
	t_color	color;
}	t_ambient;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
}	t_ray;

typedef struct s_material
{
	double	transparency;
	double	refractive_index;
	double	reflectivity;
	t_color	color;
	bool	checker;
}	t_material;

typedef enum e_object_type
{
	SPHERE,
	CYLINDER,
	PLANE,
	DISC
}	t_object_type;

typedef struct s_hit_record
{
	t_vector		point;
	t_vector		normal;
	double			t;
	t_color			color;
	t_material		material;
	int				hit;
	int				index;
	t_object_type	type;
	bool			hit_from_inside;
}	t_hit_record;

t_vector	subtract(t_vector a, t_vector b);
double		dot(t_vector a, t_vector b);
t_vector	normalize(t_vector v);
t_vector	multiply_scalar(t_vector v, double scalar);
t_vector	add(t_vector a, t_vector b);

#endif // OBJECTS_H
