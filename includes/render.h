#ifndef RENDER_H
# define RENDER_H

# include "minirt.h"

# define MAX_REFLECTION_DEPTH 16

void		render_scene(mlx_t *mlx, t_scene *scene);
int			intersect_sphere(t_ray *ray, t_sphere *sphere, double *t);
int			intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t);
int			intersect_plane(t_ray *ray, t_plane *plane, double *t);
t_color		apply_lighting(t_vector hit_point, t_vector normal,
				t_color object_color, t_scene *scene, int depth);
int			is_in_shadow(t_vector hit_point, t_light light, t_scene *scene);
t_vector	random_point_on_light(t_light light);
double		compute_shadow_factor(t_vector hit_point, t_light light,
				t_scene *scene, int num_samples);
int			is_checkerboard(t_vector point, t_cylinder *cylinder, double scale);
t_color		get_checkerboard_color(t_vector point, t_cylinder *cylinder,
				t_color color1, t_color color2, double scale);
t_color		get_cylinder_checkerboard_color(t_vector point,
				t_cylinder *cylinder, t_color color1, t_color color2,
				double scale);
int			is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder,
				double scale);
void		render_next_row(void *param);
t_vector	cross(t_vector a, t_vector b);
int			is_checkerboard_vertical(t_vector point, t_cylinder *cylinder,
				double scale);
int			is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder,
				double scale);
t_vector	world_to_local(t_vector point, t_vector orientation,
				t_vector center);
t_ray		create_ray(int x, int y, t_camera *camera);
t_color		trace_ray(t_ray ray, t_scene *scene, int depth);
t_color		blend_colors(t_color color1, t_color color2, float ratio);
t_vector	refract_ray(t_vector I, t_vector N, float n1, float n2);
t_vector	refract(t_vector incident, t_vector normal, float eta_ratio);
t_color		calculate_transparency(t_vector hit_point, t_vector normal,
				t_ray ray, t_scene *scene, float transparency, float ior);
t_ray		get_reflection_ray(t_vector hit_point, t_vector normal,
				t_ray incident_ray);
t_vector	reflect(t_vector direction, t_vector normal);
t_vector	get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder);
double		calculate_fresnel(t_vector incident, t_vector normal);
t_color		uint32_to_t_color(uint32_t color);
t_color		blend_colors(t_color original_color, t_color reflected_color,
				float reflectivity);
double		schlick_reflection_coefficient(double cos_theta,
				double refractive_index);
int			intersect_disc(t_ray *ray, t_disc *disc, double *t);
t_vector	scale_vector(t_vector v, double s);
double		length_squared(t_vector v);
void		parse_discs(char *line, t_scene *scene);
t_color		combine_color(t_color light_color, t_color object_color);
t_color		get_plane_checkerboard_color(t_vector point, t_color color1,
				t_color color2, t_vector normal, double scale);
t_color		get_disc_checkerboard_color(t_vector point, t_disc *disc,
				t_color color1, t_color color2, double scale);
int			intersect_plane_ray(t_ray *ray, t_plane *plane, double *t_plane);
int			intersect_disc_ray(t_ray *ray, t_disc *disc, double *t_disc);
int			intersect_cylinder_ray(t_ray *ray, t_cylinder *cylinder,
				double *t_cy);
int			intersect_sphere_ray(t_ray *ray, t_sphere *sphere,
				double *t_sphere);
void		check_sphere_intersections(t_ray ray, t_scene *scene,
				t_hit_record *hit);
void		check_cylinder_intersections(t_ray ray, t_scene *scene,
				t_hit_record *hit);
void		check_disc_intersections(t_ray ray, t_scene *scene,
				t_hit_record *hit);
void		check_plane_intersections(t_ray ray, t_scene *scene,
				t_hit_record *hit);
t_color		calculate_object_color(t_hit_record *hit, t_ray ray,
				t_scene *scene, int depth);
t_color		apply_material_effects(t_material_params params);

#endif // RENDER_H
