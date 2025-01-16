#ifndef SCENE_H
# define SCENE_H

# include "objects.h"

typedef struct s_camera
{
	t_vector	pos;
	t_vector	orientation;
	double		fov;
}	t_camera;

typedef struct s_light
{
	t_vector	pos;
	double		brightness;
	t_color		color;
	double		radius;
}	t_light;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_material	material;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	orientation;
	double		diameter;
	double		height;
	double		radius;
	t_material	material;
}	t_cylinder;

typedef struct s_disc
{
	double		diameter;
	double		height;
	t_material	material;
	t_vector	orientation;
	t_vector	center;
	t_vector	normal;
	double		radius;
	t_color		color;
}	t_disc;

typedef struct s_sphere
{
	t_vector	center;
	double		radius;
	t_material	material;
	t_vector	oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		sqrt_discriminant;
	double		t1;
	double		t2;
}	t_sphere;
typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		lights[100];
	t_plane		planes[100];
	t_sphere	spheres[100];
	t_cylinder	cylinders[100];
	t_disc		discs[200];
	int			num_spheres;
	int			num_planes;
	int			num_cylinders;
	int			num_discs;
	int			num_lights;
	int			has_ambient;
	int			has_camera;
	int			has_light;
}	t_scene;

typedef struct s_render_data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			*scene;
	int				threads_completed;
	int				rendering_finished;
	pthread_mutex_t	mutex;
	int				current_row;
	int				render_complete;
	struct timeval	start_time;
	struct timeval	end_time;
}	t_render_data;

typedef struct s_thread_data
{
	t_render_data	*render_data;
	int				start_row;
	int				end_row;
	int				thread_id;
	int				num_threads;
}	t_thread_data;

typedef struct s_material_params
{
	t_hit_record	*hit;
	t_ray			ray;
	t_vector		normal;
	t_scene			*scene;
	t_color			base_color;
	int				depth;
}	t_material_params;

#endif // SCENE_H
