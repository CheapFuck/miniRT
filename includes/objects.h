/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   objects.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:57:56 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:57:57 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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

#endif // OBJECTS_H
