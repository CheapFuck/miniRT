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

typedef struct s_compute_shadow_factor
{
	int			unblocked_rays;
	int			i;
	t_vector	light_point;
	t_vector	shadow_ray_dir;
	double		light_distance;
	t_ray		shadow_ray;
	int			in_shadow;
	int			j;
	double		t_shadow;
}	t_compute_shadow_factor;

typedef struct s_in_shadow
{
	int			i;
	t_vector	light_dir;
	double		light_distance;
	t_ray		shadow_ray;
	double		t_shadow;
}	t_in_shadow;

typedef struct s_apply_lighting
{
	int			i;
	t_color		light_contribution;
	t_vector	view_dir;
	t_light		light;
	double		shadow_factor;
	t_vector	light_dir;
	double		diffuse_intensity;
	t_vector	reflect_dir;
	double		specular_intensity;
}	t_apply_lighting;

typedef struct s_disc_checkerboard
{
	double		grid_size;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	int			u;
	int			v;
}	t_disc_checkerboard;

typedef struct s_plane_checkerboard
{
	double		grid_size;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	int			u;
	int			v;
}	t_plane_checkerboard;

typedef struct s_checkerboard_horizontal
{
	t_vector	local_point;
	double		height;
	double		angle;
	double		grid_size;
	t_vector	radial;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	double		normalized_angle;
	int			u;
	int			v;
}	t_checkerboard_horizontal;

typedef struct s_checkerboard_vertical
{
	t_vector	local_point;
	double		height;
	double		angle;
	double		grid_size;
	t_vector	radial;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	double		scaled_angle;
	int			u;
	int			v;
}	t_checkerboard_vertical;

typedef struct s_intersect_cylider
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
}	t_intersect_cylinder;

typedef struct s_create_ray
{
	t_ray		ray;
	double		aspect_ratio;
	double		fov_scale;
	double		norm_x;
	double		norm_y;
	t_vector	right;
	t_vector	up;
}	t_create_ray;

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
	int			dept;
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

typedef struct s_cylinder_checkerboard
{
	t_vector	local_point;
	double		height;
	t_vector	projection;
	t_vector	radial_vector;
	double		angle;
	double		u;
	double		v;
	double		scaled_u;
	double		scaled_v;
	int			u_check;
	int			v_check;
}	t_cylinder_checkerboard;

typedef struct s_get_refraction_ray
{
	t_ray		refraction_ray;
	double		cos_i;
	double		eta_i;
	double		eta_t;
	double		eta;
	double		k;
	t_vector	refracted_dir;
	double		temp;
}	t_get_refraction_ray;

typedef struct s_thread_data
{
	t_render_data	*render_data;
	int				start_row;
	int				end_row;
	int				thread_id;
	int				num_threads;
}	t_thread_data;

typedef struct s_render_thread
{
	t_thread_data	*thread_data;
	t_render_data	*data;
	int				x;
	int				y;
	int				thread_id;
	int				num_threads;
	int				pixel_index;
	t_ray			ray;
	t_color			final_color;
	uint32_t		color;
	double			elapsed_time;
}	t_render_thread;

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
