/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:58:06 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 17:02:44 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "minirt.h"

void			render_scene(mlx_t *mlx, t_scene *scene);
int				intersect_sphere(t_ray *ray, t_sphere *sphere, double *t);
int				intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t);
int				intersect_plane(t_ray *ray, t_plane *plane, double *t);
t_color			apply_lighting(t_vector hit_point, t_vector normal,
					t_color object_color, t_scene *scene);
int				is_in_shadow(t_vector hit_point, t_light light, t_scene *scene);
t_vector		random_point_on_light(t_light light);
double			compute_shadow_factor(t_vector hit_point, t_light light,
					t_scene *scene, int num_samples);
int				is_checkerboard(t_vector point, t_cylinder *cylinder,
					double scale);
t_color			get_checkerboard_color(t_vector point, t_cylinder *cylinder,
					double scale);
t_color			get_cylinder_checkerboard_color(t_vector point,
					t_cylinder *cylinder, double scale);
int				is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder,
					double scale);
void			render_next_row(void *param);
t_vector		cross(t_vector a, t_vector b);
int				is_checkerboard_vertical(t_vector point, t_cylinder *cylinder,
					double scale);
int				is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder,
					double scale);
t_vector		world_to_local(t_vector point, t_vector orientation,
					t_vector center);
t_ray			create_ray(int x, int y, t_camera *camera);
t_color			trace_ray(t_ray ray, t_scene *scene, int depth);
t_color			blend_colors(t_color color1, t_color color2, float ratio);
t_vector		refract_ray(t_vector I, t_vector N, float n1, float n2);
t_vector		refract(t_vector incident, t_vector normal, float eta_ratio);
t_ray			get_reflection_ray(t_vector hit_point, t_vector normal,
					t_ray incident_ray);
t_vector		reflect(t_vector direction, t_vector normal);
t_vector		get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder);
double			calculate_fresnel(t_vector incident, t_vector normal);
t_color			uint32_to_t_color(uint32_t color);
t_color			blend_colors(t_color original_color, t_color reflected_color,
					float reflectivity);
double			schlick_reflection_coefficient(double cos_theta,
					double refractive_index);
int				intersect_disc(t_ray *ray, t_disc *disc, double *t);
t_vector		scale_vector(t_vector v, double s);
double			length_squared(t_vector v);
void			parse_discs(char *line, t_scene *scene);
t_color			combine_color(t_color light_color, t_color object_color);
t_color			get_plane_checkerboard_color(t_vector point, t_vector normal,
					double scale);
t_color			get_disc_checkerboard_color(t_vector point, t_disc *disc,
					double scale);
int				intersect_plane_ray(t_ray *ray, t_plane *plane,
					double *t_plane);
int				intersect_disc_ray(t_ray *ray, t_disc *disc, double *t_disc);
int				intersect_cylinder_ray(t_ray *ray, t_cylinder *cylinder,
					double *t_cy);
int				intersect_sphere_ray(t_ray *ray, t_sphere *sphere,
					double *t_sphere);
void			check_sphere_intersections(t_ray ray, t_scene *scene,
					t_hit_record *hit);
void			check_cylinder_intersections(t_ray ray, t_scene *scene,
					t_hit_record *hit);
void			check_disc_intersections(t_ray ray, t_scene *scene,
					t_hit_record *hit);
void			check_plane_intersections(t_ray ray, t_scene *scene,
					t_hit_record *hit);
t_color			calculate_object_color(t_hit_record *hit, t_ray ray,
					t_scene *scene, int depth);
t_color			apply_material_effects(t_material_params params);
void			render_scene(mlx_t *mlx, t_scene *scene);
int				main(int argc, char **argv);
void			ft_hook(void *param);
void			parse_file(const char *filename, t_scene *scene);
void			parse_ambient(char *line, t_scene *scene);
void			parse_camera(char *line, t_scene *scene);
void			parse_light(char *line, t_scene *scene);
void			parse_sphere(char *line, t_scene *scene);
void			parse_plane(char *line, t_scene *scene);
void			parse_cylinder(char *line, t_scene *scene);
int				parse_disc_properties(char **tokens, t_disc *disc);
char			**split_and_validate(char *str, int expected_parts);
void			handle_parse_error(char **tokens, const char *error_message);
int				normalize_orientation_disc(t_disc *disc);
int				parse_color(char *color_str, t_color *color);
t_vector		parse_vector(const char *str);
double			parse_double(const char *str);
int				parse_int(const char *str);
void			exit_with_error(char *msg);
void			save_image_to_file(const unsigned char *raw_image_data,
					unsigned width, unsigned height, const char *filename);
int				validate_ratio(double value, const char *element_name);
int				validate_color(t_color *color);
int				validate_nrmlzd_vector(t_vector *vec, char *context);
int				validate_fov(int fov);
int				validate_unique_element(t_scene *scene, char type);
int				is_valid_number(const char *str);
t_vector		subtract(t_vector a, t_vector b);
double			dot(t_vector a, t_vector b);
t_vector		normalize(t_vector v);
t_vector		multiply_scalar(t_vector v, double scalar);
t_vector		add(t_vector a, t_vector b);
void			open_file(const char *filename, int *fd);
char			*get_next_line_from_file(int fd);
int				parse_cylinder_properties(char **tokens, t_cylinder *cylinder);
void			parse(char *error, char **tokens);
void			handle_pars_error(char **tokens, const char *error_message);
void			refraction_helper(t_get_refraction_ray	*vars,
					t_vector *normal);
t_ray			get_refraction_ray(t_vector point, t_vector normal,
					t_ray incoming_ray, double refractive_index);
void			get_hit_normal_sphere(t_hit_record *hit, t_vector *normal,
					t_scene *scene);
void			get_hit_normal_cylinder(t_hit_record *hit, t_vector *normal,
					t_scene *scene);
void			get_hit_normal_plane(t_hit_record *hit, t_vector *normal,
					t_scene *scene, t_ray ray);
t_color			get_surface_color_sphere(t_scene *scene, t_hit_record *hit);
t_color			get_surface_color_cylinder(t_scene *scene, t_hit_record *hit);
t_color			get_surface_color_plane(t_scene *scene, t_hit_record *hit,
					t_vector normal);
void			get_hit_normal(t_hit_record *hit, t_ray ray, t_vector *normal,
					t_scene *scene);
t_hit_record	find_closest_intersection(t_ray ray, t_scene *scene);
t_color			apply_reflection(t_material_params params, t_color base_color);
t_color			apply_refraction(t_material_params params, t_color base_color);
void			init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene,
					t_render_data *data);
void			update_display(void *param);
void			*render_thread(void *arg);
t_vector		normalize(t_vector v);


#endif // RENDER_H
