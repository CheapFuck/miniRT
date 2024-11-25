#ifndef RENDER_H
#define RENDER_H

#include "minirt.h"

// Function to render the scene
void render_scene(mlx_t *mlx, t_scene *scene);

// Ray-Object intersection functions
int intersect_sphere(t_ray *ray, t_sphere *sphere, double *t);
int intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t);
int intersect_plane(t_ray *ray, t_plane *plane, double *t);
t_color apply_lighting(t_vector hit_point, t_vector normal, t_color object_color, t_scene *scene);
int is_in_shadow(t_vector hit_point, t_light light, t_scene *scene);
t_vector random_point_on_light(t_light light);
double compute_shadow_factor(t_vector hit_point, t_light light, t_scene *scene, int num_samples);
int is_checkerboard(t_vector point, double scale);
t_color get_checkerboard_color(t_vector point, t_color color1, t_color color2, double scale);
void render_next_row(void *param);
t_vector cross(t_vector a, t_vector b);
t_vector world_to_local(t_vector point, t_vector orientation, t_vector center);
t_ray create_ray(int x, int y, t_camera *camera);
// uint32_t trace_ray(t_ray ray, t_scene *scene, int depth);
t_color trace_ray(t_ray ray, t_scene *scene, int depth);
t_color blend_colors(t_color color1, t_color color2, float ratio);
t_vector refract_ray(t_vector I, t_vector N, float n1, float n2);
t_color calculate_transparency(t_vector hit_point, t_vector normal, t_ray ray, t_scene *scene, float transparency, float ior);
t_ray get_reflection_ray(t_vector hit_point, t_vector normal, t_ray incident_ray);
t_vector get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder);

#endif // RENDER_H
