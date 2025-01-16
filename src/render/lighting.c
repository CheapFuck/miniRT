#include "../../includes/minirt.h"

t_vector	random_point_on_light(t_light light)
{
	double		theta;
	double		r;
	t_vector	random_point;

	theta = ((double)rand() / RAND_MAX) * 2 * M_PI;
	r = light.radius * sqrt((double)rand() / RAND_MAX);
	random_point.x = light.pos.x + r * cos(theta);
	random_point.y = light.pos.y + r * sin(theta);
	random_point.z = light.pos.z;
	return (random_point);
}

static void	calc_shadow_vars(t_compute_shadow_factor *vars, t_light light,
	t_vector hit_point)
{
	vars->light_point = random_point_on_light(light);
	vars->shadow_ray_dir = normalize(subtract(vars->light_point, hit_point));
	vars->light_distance = sqrt(dot(subtract(vars->light_point, hit_point),
				subtract(vars->light_point, hit_point)));
	vars->shadow_ray.origin = add(hit_point,
			multiply_scalar(vars->shadow_ray_dir, EPSILON));
	vars->shadow_ray.direction = vars->shadow_ray_dir;
	vars->in_shadow = 0;
	vars->j = 0;
}

double	compute_shadow_factor(t_vector hit_point, t_light light,
	t_scene *scene, int num_samples)
{
	t_compute_shadow_factor	vars;

	vars.unblocked_rays = 0;
	vars.i = 0;
	while (vars.i < num_samples)
	{
		calc_shadow_vars(&vars, light, hit_point);
		while (vars.j < scene->num_spheres)
		{
			if (intersect_sphere(&vars.shadow_ray, &scene->spheres[vars.j],
					&vars.t_shadow)
				&& vars.t_shadow < vars.light_distance)
			{
				vars.in_shadow = 1;
				break ;
			}
			vars.j++;
		}
		if (!vars.in_shadow)
			vars.unblocked_rays++;
		vars.i++;
	}
	return ((double)vars.unblocked_rays / num_samples);
}

int	is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
{
	if (fabs(dot((t_vector){0, 1, 0}, cylinder->orientation)) > 0.99)
		return (is_checkerboard_vertical(point, cylinder, scale));
	else
		return (is_checkerboard_horizontal(point, cylinder, scale));
}

int	is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder,
	double scale)
{
	t_checkerboard_horizontal	vars;

	vars.grid_size = scale;
	vars.local_point = subtract(point, cylinder->center);
	vars.height = dot(vars.local_point, cylinder->orientation);
	vars.radial = subtract(vars.local_point,
			multiply_scalar(cylinder->orientation, vars.height));
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, cylinder->orientation)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, cylinder->orientation));
	vars.y_axis = normalize(cross(cylinder->orientation, vars.x_axis));
	vars.proj_x = dot(vars.radial, vars.x_axis);
	vars.proj_y = dot(vars.radial, vars.y_axis);
	vars.angle = atan2(vars.proj_y, vars.proj_x);
	vars.normalized_angle = (vars.angle + M_PI) / (2 * M_PI);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.normalized_angle * (2 * M_PI * cylinder->radius)
			/ vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

int	is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
{
	t_checkerboard_vertical	vars;

	vars.grid_size = scale;
	vars.local_point = subtract(point, cylinder->center);
	vars.height = dot(vars.local_point, cylinder->orientation);
	vars.radial = subtract(vars.local_point,
			multiply_scalar(cylinder->orientation, vars.height));
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, cylinder->orientation)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, cylinder->orientation));
	vars.y_axis = normalize(cross(cylinder->orientation, vars.x_axis));
	vars.proj_x = dot(vars.radial, vars.x_axis);
	vars.proj_y = dot(vars.radial, vars.y_axis);
	vars.angle = atan2(vars.proj_y, vars.proj_x);
	if (vars.angle < 0)
		vars.angle += 2 * M_PI;
	vars.scaled_angle = vars.angle * (cylinder->radius / vars.grid_size);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.scaled_angle);
	return ((vars.u + vars.v) % 2);
}

int	is_plane_checkerboard(t_vector point, t_vector plane_normal, double scale)
{
	t_plane_checkerboard	vars;

	vars.grid_size = scale;
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, plane_normal)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, plane_normal));
	vars.y_axis = normalize(cross(plane_normal, vars.x_axis));
	vars.proj_x = dot(point, vars.x_axis);
	vars.proj_y = dot(point, vars.y_axis);
	vars.u = (int)floor(vars.proj_x / vars.grid_size);
	vars.v = (int)floor(vars.proj_y / vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

t_color	get_plane_checkerboard_color(t_vector point, t_vector normal,
	double scale)
{
	if (is_plane_checkerboard(point, normal, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

int	is_disc_checkerboard(t_vector point, t_disc *disc, double scale)
{
	t_disc_checkerboard	vars;

	vars.grid_size = scale;
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, disc->normal)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, disc->normal));
	vars.y_axis = normalize(cross(disc->normal, vars.x_axis));
	vars.proj_x = dot(point, vars.x_axis);
	vars.proj_y = dot(point, vars.y_axis);
	vars.u = (int)floor(vars.proj_x / vars.grid_size);
	vars.v = (int)floor(vars.proj_y / vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

t_color	get_disc_checkerboard_color(t_vector point, t_disc *disc, double scale)
{
	if (is_disc_checkerboard(point, disc, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

t_color	get_checkerboard_color(t_vector point, t_cylinder *cylinder,
	double scale)
{
	if (is_checkerboard(point, cylinder, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

static void	calc_in_shadow_vars(t_in_shadow *vars, t_light light,
t_vector hit_point)
{
	vars->light_dir = normalize(subtract(light.pos, hit_point));
	vars->light_distance = sqrt(dot(subtract(light.pos, hit_point),
				subtract(light.pos, hit_point)));
	vars->shadow_ray.origin = add(hit_point, multiply_scalar(vars->light_dir,
				EPSILON));
	vars->shadow_ray.direction = vars->light_dir;
	vars->i = 0;
}

int	is_in_shadow(t_vector hit_point, t_light light, t_scene *scene)
{
	t_in_shadow	vars;

	calc_in_shadow_vars(&vars, light, hit_point);
	while (vars.i < scene->num_spheres)
	{
		if (intersect_sphere(&vars.shadow_ray, &scene->spheres[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
		vars.i++;
	}
	vars.i = 0;
	while (vars.i < scene->num_cylinders)
	{
		if (intersect_cylinder(&vars.shadow_ray, &scene->cylinders[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
		vars.i++;
	}
	while (vars.i < scene->num_planes)
		if (intersect_plane(&vars.shadow_ray, &scene->planes[vars.i],
				&vars.t_shadow) && vars.t_shadow < vars.light_distance)
			return (1);
	vars.i++;
	return (0);
}

static void	apply_lighting_loop(t_apply_lighting *vars, t_scene *scene,
	t_vector hit_point, t_vector normal)
{
	while (vars->i < scene->num_lights)
	{
		vars->light = scene->lights[vars->i];
		vars->shadow_factor = compute_shadow_factor(hit_point, vars->light,
				scene, 8);
		if (vars->shadow_factor > 0)
		{
			vars->light_dir = normalize(subtract(vars->light.pos, hit_point));
			vars->diffuse_intensity = fmax(0.0, dot(normal, vars->light_dir))
				* vars->light.brightness * vars->shadow_factor;
			vars->reflect_dir = normalize(subtract(multiply_scalar(normal, 2.0
							* dot(normal, vars->light_dir)), vars->light_dir));
			vars->specular_intensity = pow(fmax(0.0, dot(vars->reflect_dir,
							vars->view_dir)), 50) * vars->light.brightness
				* vars->shadow_factor * 1;
			vars->light_contribution.r += vars->light.color.r
				* (vars->diffuse_intensity + vars->specular_intensity);
			vars->light_contribution.g += vars->light.color.g
				* (vars->diffuse_intensity + vars->specular_intensity);
			vars->light_contribution.b += vars->light.color.b
				* (vars->diffuse_intensity + vars->specular_intensity);
		}
		vars->i++;
	}
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
	t_color object_color, t_scene *scene)
{
	t_apply_lighting	vars;

	if (scene->dept > MAX_REFLECTION_DEPTH)
		return ((t_color){0, 0, 0});
	vars.light_contribution = (t_color){0, 0, 0};
	vars.light_contribution.r += 255 * scene->ambient.ratio;
	vars.light_contribution.g += 255 * scene->ambient.ratio;
	vars.light_contribution.b += 255 * scene->ambient.ratio;
	vars.view_dir = normalize(subtract(scene->camera.pos, hit_point));
	vars.i = 0;
	apply_lighting_loop(&vars, scene, hit_point, normal);
	return (combine_color(vars.light_contribution, object_color));
}

t_vector	compute_reflection(t_vector light_dir, t_vector normal)
{
	t_vector	scaled_normal;

	scaled_normal = multiply_scalar(normal, 2.0 * dot(normal, light_dir));
	return (subtract(scaled_normal, light_dir));
}
