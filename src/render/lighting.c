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
		j = 0;
		while (j < scene->num_cylinders)
		{
			if (intersect_cylinder(&shadow_ray, &scene->cylinders[j],
					&t_shadow) && t_shadow < light_distance)
			{
				in_shadow = 1;
				break ;
			}
			j++;
		}
		j = 0;
		while (j < scene->num_planes)
		{
			if (intersect_plane(&shadow_ray, &scene->planes[j], &t_shadow)
				&& t_shadow < light_distance)
			{
				in_shadow = 1;
				break ;
			}
			j++;
		}
		j = 0;
		while (j < scene->num_discs)
		{
			if (intersect_disc(&shadow_ray, &scene->discs[j], &t_shadow)
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
		return (is_checkerboard_xy(point, cylinder, scale, 1));
	else
		return (is_checkerboard_xy(point, cylinder, scale, 0));
}

int	is_checkerboard_xy(t_vector point, t_cylinder *cylinder, double scale,
	int mode)
{
	t_vector	vectors[5];
	double		doubles[7];

	vectors[2] = (t_vector){0, 1, 0};
	vectors[0] = subtract(point, cylinder->center);
	doubles[0] = dot(vectors[0], cylinder->orientation);
	vectors[1] = subtract(vectors[0], multiply_scalar(cylinder->orientation,
				doubles[0]));
	if (fabs(dot(vectors[2], cylinder->orientation)) > 0.99)
		vectors[2] = (t_vector){1, 0, 0};
	vectors[3] = normalize(cross(vectors[2], cylinder->orientation));
	vectors[4] = normalize(cross(cylinder->orientation, vectors[3]));
	doubles[2] = dot(vectors[0], vectors[3]);
	doubles[3] = dot(vectors[0], vectors[4]);
	doubles[1] = atan2(doubles[3], doubles[2]);
	if (doubles[1] < 0)
		doubles[1] += 2 * M_PI;
	if (mode == 0)
		doubles[4] = (doubles[1] + M_PI) / (2 * M_PI) * (2 * M_PI
				* cylinder->radius) / scale;
	else
		doubles[4] = doubles[1] * (cylinder->radius / scale);
	doubles[5] = floor(doubles[0] / scale);
	doubles[6] = floor(doubles[4]);
	return ((int)(doubles[5] + doubles[6]) % 2);
}

int	is_plane_checkerboard(t_vector point, t_vector plane_normal, double scale)
{
	t_vector	vectors[3];
	double		doubles[3];
	int			u;
	int			v;

	doubles[0] = scale;
	vectors[0] = (t_vector){0, 1, 0};
	if (fabs(dot(vectors[0], plane_normal)) > 0.99)
		vectors[0] = (t_vector){1, 0, 0};
	vectors[1] = normalize(cross(vectors[0], plane_normal));
	vectors[2] = normalize(cross(plane_normal, vectors[1]));
	doubles[1] = dot(point, vectors[1]);
	doubles[2] = dot(point, vectors[2]);
	u = (int)floor(doubles[1] / doubles[0]);
	v = (int)floor(doubles[2] / doubles[0]);
	return ((u + v) % 2);
}

t_color	get_plane_checkerboard_color(t_vector point, t_vector normal,
	double scale)
{
	t_color			black;
	t_color			white;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	if (is_plane_checkerboard(point, normal, scale))
		return (black);
	return (white);
}

int	is_disc_checkerboard(t_vector point, t_disc *disc, double scale)
{
	t_vector	vectors[3];
	double		doubles[3];
	int			u;
	int			v;

	doubles[0] = scale;
	vectors[0] = (t_vector){0, 1, 0};
	if (fabs(dot(vectors[0], disc->normal)) > 0.99)
		vectors[0] = (t_vector){1, 0, 0};
	vectors[1] = normalize(cross(vectors[0], disc->normal));
	vectors[2] = normalize(cross(disc->normal, vectors[1]));
	doubles[1] = dot(point, vectors[1]);
	doubles[2] = dot(point, vectors[2]);
	u = (int)floor(doubles[1] / doubles[0]);
	v = (int)floor(doubles[2] / doubles[0]);
	return ((u + v) % 2);
}

t_color	get_disc_checkerboard_color(t_vector point, t_disc *disc, double scale)
{
	t_color			black;
	t_color			white;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	if (is_disc_checkerboard(point, disc, scale))
		return (black);
	return (white);
}

static int	intersect_sphere_helper(t_scene *scene, t_ray shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

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

static int	intersect_cylinder_helper(t_scene *scene, t_ray shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

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

static int	intersect_plane_helper(t_scene *scene, t_ray shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

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

static int	intersect_disc_helper(t_scene *scene, t_ray shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_discs)
	{
		if (intersect_disc(&shadow_ray, &scene->discs[i], &t_shadow)
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

static t_color	apply_ambient_lighting(double ambient_ratio)
{
	t_color	ambient;

	ambient.r = 255 * ambient_ratio;
	ambient.g = 255 * ambient_ratio;
	ambient.b = 255 * ambient_ratio;
	return (ambient);
}

static t_color	calc_light(t_light light, t_vector hit_point,
	t_vector normal, t_vector view_dir)
{
	t_color		contribution;
	t_vector	light_dir;
	t_vector	reflect_dir;
	double		diffuse_intensity;
	double		specular_intensity;

	light_dir = normalize(subtract(light.pos, hit_point));
	diffuse_intensity = fmax(0.0, dot(normal, light_dir)) * light.brightness;
	reflect_dir = normalize(subtract(multiply_scalar(normal,
					2.0 * dot(normal, light_dir)), light_dir));
	specular_intensity = pow(fmax(0.0, dot(reflect_dir, view_dir)), 50)
		* light.brightness;
	contribution.r = light.color.r * (diffuse_intensity + specular_intensity);
	contribution.g = light.color.g * (diffuse_intensity + specular_intensity);
	contribution.b = light.color.b * (diffuse_intensity + specular_intensity);
	return (contribution);
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
	t_color object_color, t_scene *scene)
{
	t_color		light_contribution;
	t_vector	view_dir;
	int			i;
	double		shadow_factor;
	t_color		temp;

	if (5 > MAX_REFLECTION_DEPTH)
		return ((t_color){0, 0, 0});
	light_contribution = apply_ambient_lighting(scene->ambient.ratio);
	view_dir = normalize(subtract(scene->camera.pos, hit_point));
	i = 0;
	while (i < scene->num_lights)
	{
		shadow_factor = compute_shadow_factor(hit_point, scene->lights[i],
				scene, 8);
		if (shadow_factor > 0)
		{
			temp = calc_light(scene->lights[i], hit_point, normal, view_dir);
			light_contribution.r += temp.r * shadow_factor;
			light_contribution.g += temp.g * shadow_factor;
			light_contribution.b += temp.b * shadow_factor;
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
