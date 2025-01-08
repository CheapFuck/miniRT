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

double	compute_shadow_factor(t_vector hit_point, t_light light, t_scene *scene,
		int num_samples)
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

	unblocked_rays = 0;
	i = 0;
	while (i < num_samples)
	{
		light_point = random_point_on_light(light);
		shadow_ray_dir = normalize(subtract(light_point, hit_point));
		light_distance = sqrt(dot(subtract(light_point, hit_point),
					subtract(light_point, hit_point)));
		shadow_ray.origin = add(hit_point, multiply_scalar(shadow_ray_dir,
					1e-4));
		shadow_ray.direction = shadow_ray_dir;
		in_shadow = 0;
		j = 0;
		while (j < scene->num_spheres)
		{
			if (intersect_sphere(&shadow_ray, &scene->spheres[j], &t_shadow)
				&& t_shadow < light_distance)
			{
				in_shadow = 1;
				break ;
			}
			j++;
		}
		if (!in_shadow)
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
	t_vector	radial;
	t_vector	up;
	t_vector	x_axis;
	t_vector	y_axis;
	double		proj_x;
	double		proj_y;
	double		normalized_angle;
	int			u;
	int			v;

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
	u = (int)floor(height / scale);
	v = (int)floor(normalized_angle * (2 * M_PI * cylinder->radius) / scale);
	return ((u + v) % 2);
}


int	is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
{
	static double 	array[6];
	static t_vector	vectors[6];
	int				u;
	int				v;

	array[2] = scale;
	vectors[5] = subtract(point, cylinder->center);
	array[0] = dot(vectors[5], cylinder->orientation);
	vectors[0] = subtract(vectors[5], multiply_scalar(cylinder->orientation,
				array[0]));
	vectors[1] = (t_vector){0, 1, 0};
	if (fabs(dot(vectors[1], cylinder->orientation)) > 0.99)
		vectors[1] = (t_vector){1, 0, 0};
	vectors[2] = normalize(cross(vectors[1], cylinder->orientation));
	vectors[3] = normalize(cross(cylinder->orientation, vectors[2]));
	array[3] = dot(vectors[0], vectors[2]);
	array[4] = dot(vectors[0], vectors[2]);
	array[1] = atan2(array[4], array[3]);
	if (array[1] < 0)
		array[1] += 2 * M_PI;
	array[5] = array[1] * (cylinder->radius / array[1]);
	u = (int)floor(array[0] / array[1]);
	v = (int)floor(array[5]);
	return (((u + v) % 2));
}

// int	is_checkerboard_vertical(t_vector point, t_cylinder *cylinder, double scale)
// {
// 	t_vector	local_point;
// 	double		height;
// 	double		angle;
// 	double		grid_size;
// 	t_vector	radial;
// 	t_vector	up;
// 	t_vector	x_axis;
// 	t_vector	y_axis;
// 	double		proj_x;
// 	double		proj_y;
// 	double		scaled_angle;
// 	int			u;
// 	int			v;

// 	grid_size = scale;
// 	local_point = subtract(point, cylinder->center);
// 	height = dot(local_point, cylinder->orientation);
// 	radial = subtract(local_point, multiply_scalar(cylinder->orientation,
// 				height));
// 	up = (t_vector){0, 1, 0};
// 	if (fabs(dot(up, cylinder->orientation)) > 0.99)
// 		up = (t_vector){1, 0, 0};
// 	x_axis = normalize(cross(up, cylinder->orientation));
// 	y_axis = normalize(cross(cylinder->orientation, x_axis));
// 	proj_x = dot(radial, x_axis);
// 	proj_y = dot(radial, y_axis);
// 	angle = atan2(proj_y, proj_x);
// 	if (angle < 0)
// 		angle += 2 * M_PI;
// 	scaled_angle = angle * (cylinder->radius / grid_size);
// 	u = (int)floor(height / grid_size);
// 	v = (int)floor(scaled_angle);
// 	return (((u + v) % 2));
// }

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
static int intersect_sphere_helper(t_scene *scene, t_ray shadow_ray, double t_shadow, double light_distance)
{
	int i;

	i = 0;
	while (i < scene->num_spheres)
	{
		if (intersect_sphere(&shadow_ray, &scene->spheres[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

static int intersect_cylinder_helper(t_scene *scene, t_ray shadow_ray, double t_shadow, double light_distance)
{
	int i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_cylinder(&shadow_ray, &scene->cylinders[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

static int intersect_plane_helper(t_scene *scene, t_ray shadow_ray, double t_shadow, double light_distance)
{
	int i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_plane(&shadow_ray, &scene->planes[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}
int	is_in_shadow(t_vector hit_point, t_light light, t_scene *scene)
{
	t_vector	light_dir;
	double		light_distance;
	t_ray		shadow_ray;
	double		t_shadow;

	t_shadow = 0.0;
	light_dir = normalize(subtract(light.pos, hit_point));
	light_distance = sqrt(dot(subtract(light.pos, hit_point),
				subtract(light.pos, hit_point)));
	shadow_ray.origin = add(hit_point, multiply_scalar(light_dir, 1e-4));
	shadow_ray.direction = light_dir;
	if(intersect_sphere_helper(scene, shadow_ray, t_shadow, light_distance))
		return(1);
	if(intersect_cylinder_helper(scene, shadow_ray, t_shadow, light_distance))
		return(1);
	if(intersect_plane_helper(scene, shadow_ray, t_shadow, light_distance))
		return(1);
	return (0);
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
		t_color object_color, t_scene *scene)
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

	if (5 > MAX_REFLECTION_DEPTH)
		return ((t_color){0, 0, 0});
	light_contribution = (t_color){0, 0, 0};
	light_contribution.r += 255 * scene->ambient.ratio;
	light_contribution.g += 255 * scene->ambient.ratio;
	light_contribution.b += 255 * scene->ambient.ratio;
	view_dir = normalize(subtract(scene->camera.pos, hit_point));
	i = 0;
	while (i < scene->num_lights)
	{
		light = scene->lights[i];
		shadow_factor = compute_shadow_factor(hit_point, light, scene, 32);
		if (shadow_factor > 0)
		{
			light_dir = normalize(subtract(light.pos, hit_point));
			diffuse_intensity = fmax(0.0, dot(normal, light_dir))
				* light.brightness * shadow_factor;
			reflect_dir = normalize(subtract(multiply_scalar(normal, 2.0
							* dot(normal, light_dir)), light_dir));
			specular_intensity = pow(fmax(0.0, dot(reflect_dir, view_dir)), 50)
				* light.brightness * shadow_factor * 1;
			light_contribution.r += light.color.r * (diffuse_intensity
					+ specular_intensity);
			light_contribution.g += light.color.g * (diffuse_intensity
					+ specular_intensity);
			light_contribution.b += light.color.b * (diffuse_intensity
					+ specular_intensity);
		}
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
