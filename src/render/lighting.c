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

static t_ray	generate_shadow_ray(t_vector hit_point, t_light light)
{
	t_ray		shadow_ray;
	t_vector	light_point;
	t_vector	shadow_ray_dir;

	light_point = random_point_on_light(light);
	shadow_ray_dir = normalize(subtract(light_point, hit_point));
	shadow_ray.origin = add(hit_point, multiply_scalar(shadow_ray_dir, 1e-4));
	shadow_ray.direction = shadow_ray_dir;
	return (shadow_ray);
}

static double	compute_distance(t_vector a, t_vector b)
{
	t_vector	diff;

	diff = subtract(a, b);
	return (sqrt(dot(diff, diff)));
}

// Compute the shadow factor for a given point
double	compute_shadow_factor(t_vector hit_point, t_light light,
			t_scene *scene, int num_samples)
{
	int			unblocked_rays;
	int			i;
	t_ray		shadow_ray;
	double		light_distance;

	unblocked_rays = 0;
	i = 0;
	while (i < num_samples)
	{
		shadow_ray = generate_shadow_ray(hit_point, light);
		light_distance = compute_distance(random_point_on_light(light),
				hit_point);
		if (!is_in_shadow(&shadow_ray, scene, light_distance))
			unblocked_rays++;
		i++;
	}
	return ((double)unblocked_rays / num_samples);
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

	grid_size = scale;
	local_point = subtract(point, cylinder->center);
	height = dot(local_point, cylinder->orientation);
	radial = subtract(local_point,
			multiply_scalar(cylinder->orientation, height));
	up = (t_vector){0, 1, 0};
	if (fabs(dot(up, cylinder->orientation)) > 0.99)
		up = (t_vector){1, 0, 0};
	x_axis = normalize(cross(up, cylinder->orientation));
	y_axis = normalize(cross(cylinder->orientation, x_axis));
	proj_x = dot(radial, x_axis);
	proj_y = dot(radial, y_axis);
	angle = atan2(proj_y, proj_x);
	normalized_angle = (angle + M_PI) / (2 * M_PI);
	u = (int)floor(height / grid_size);
	v = (int)floor(normalized_angle * (2 * M_PI * cylinder->radius)
			/ grid_size);
	return ((u + v) % 2);
}

int	is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
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

	grid_size = scale;
	local_point = subtract(point, cylinder->center);
	height = dot(local_point, cylinder->orientation);
	radial = subtract(local_point,
			multiply_scalar(cylinder->orientation, height));
	up = (t_vector){0, 1, 0};
	if (fabs(dot(up, cylinder->orientation)) > 0.99)
		up = (t_vector){1, 0, 0};
	x_axis = normalize(cross(up, cylinder->orientation));
	y_axis = normalize(cross(cylinder->orientation, x_axis));
	proj_x = dot(radial, x_axis);
	proj_y = dot(radial, y_axis);
	angle = atan2(proj_y, proj_x);
	if (angle < 0)
		angle += 2 * M_PI;
	scaled_angle = angle * (cylinder->radius / grid_size);
	u = (int)floor(height / grid_size);
	v = (int)floor(scaled_angle);
	return ((u + v) % 2);
}

int	is_plane_checkerboard(t_vector point, t_vector plane_normal, double scale)
{
	double		grid_size;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	int			u;
	int			v;

	grid_size = scale;
	up = (t_vector){0, 1, 0};
	if (fabs(dot(up, plane_normal)) > 0.99)
		up = (t_vector){1, 0, 0};
	x_axis = normalize(cross(up, plane_normal));
	y_axis = normalize(cross(plane_normal, x_axis));
	proj_x = dot(point, x_axis);
	proj_y = dot(point, y_axis);
	u = (int)floor(proj_x / grid_size);
	v = (int)floor(proj_y / grid_size);
	return ((u + v) % 2);
}

t_color	get_plane_checkerboard_color(t_vector point, t_color color1,
	t_color color2, t_vector normal, double scale)
{
	if (is_plane_checkerboard(point, normal, scale))
		return (color1);
	return (color2);
}

int	is_disc_checkerboard(t_vector point, t_disc *disc, double scale)
{
	double		grid_size;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	int			u;
	int			v;

	grid_size = scale;
	up = (t_vector){0, 1, 0};
	if (fabs(dot(up, disc->normal)) > 0.99)
		up = (t_vector){1, 0, 0};
	x_axis = normalize(cross(up, disc->normal));
	y_axis = normalize(cross(disc->normal, x_axis));
	proj_x = dot(point, x_axis);
	proj_y = dot(point, y_axis);
	u = (int)floor(proj_x / grid_size);
	v = (int)floor(proj_y / grid_size);
	return ((u + v) % 2);
}

t_color	get_disc_checkerboard_color(t_vector point, t_disc *disc,
	t_color color1, t_color color2, double scale)
{
	if (is_disc_checkerboard(point, disc, scale))
		return (color1);
	return (color2);
}

t_color	get_checkerboard_color(t_vector point, t_cylinder *cylinder,
	t_color color1, t_color color2, double scale)
{
	if (is_checkerboard(point, cylinder, scale))
		return (color1);
	return (color2);
}

static int	intersect_sphere_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_spheres)
	{
		if (intersect_sphere(shadow_ray, &scene->spheres[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

static int	intersect_cylinder_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_cylinder(shadow_ray, &scene->cylinders[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

static int	intersect_plane_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_plane(shadow_ray, &scene->planes[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

static int	intersect_disc_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_discs)
	{
		if (intersect_disc(shadow_ray, &scene->discs[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

int	is_in_shadow(t_ray *shadow_ray, t_scene *scene,
	double light_distance)
{
	double		t_shadow;

	t_shadow = 0.0;
	if (intersect_sphere_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_cylinder_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_plane_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_disc_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	return (0);
}

static t_color	compute_ambient_light(t_color light_contribution,
	double ambient_ratio)
{
	light_contribution.r = light_contribution.r * ambient_ratio;
	light_contribution.g = light_contribution.g * ambient_ratio;
	light_contribution.b = light_contribution.b * ambient_ratio;
	return (light_contribution);
}

static t_color	compute_light_contribution(t_vector hit_point, t_vector normal,
	t_light light, t_scene *scene)
{
	t_vector	light_dir;
	double		shadow_factor;
	double		diffuse_intensity;
	t_color		contribution;

	contribution = (t_color){0, 0, 0};
	shadow_factor = compute_shadow_factor(hit_point, light, scene, SAMPLES);
	if (shadow_factor > 0)
	{
		light_dir = normalize(subtract(light.pos, hit_point));
		diffuse_intensity = fmax(0.0, dot(normal, light_dir))
			* light.brightness * shadow_factor;
		contribution.r = light.color.r * diffuse_intensity;
		contribution.g = light.color.g * diffuse_intensity;
		contribution.b = light.color.b * diffuse_intensity;
	}
	return (contribution);
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
	t_color object_color, t_scene *scene)
{
	int			i;
	t_color		light_contribution;
	t_color		current_contribution;

	light_contribution = (t_color){0, 0, 0};
	light_contribution = compute_ambient_light(scene->ambient.color,
			scene->ambient.ratio);
	i = 0;
	while (i < scene->num_lights)
	{
		current_contribution = compute_light_contribution(hit_point, normal,
				scene->lights[i], scene);
		light_contribution.r += current_contribution.r;
		light_contribution.g += current_contribution.g;
		light_contribution.b += current_contribution.b;
		i++;
	}
	return (combine_color(light_contribution, object_color));
}

t_vector	compute_reflection(t_vector light_dir, t_vector normal)
{
	t_vector	scaled_normal;

	scaled_normal = multiply_scalar(normal, 2.0 * dot(normal, light_dir));
	return (subtract(scaled_normal, light_dir));
}
