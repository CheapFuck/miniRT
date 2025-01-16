#include "../includes/minirt.h"

t_ray	get_reflection_ray(t_vector hit_point, t_vector normal,
	t_ray incident_ray)
{
	t_ray		reflection;
	double		dot_product;
	t_vector	reflection_dir;

	reflection.origin = hit_point;
	dot_product = dot(incident_ray.direction, normal);
	reflection_dir = subtract(incident_ray.direction,
			multiply_scalar(normal, 2.0 * dot_product));
	reflection.direction = normalize(reflection_dir);
	return (reflection);
}

t_color	uint32_to_t_color(uint32_t color)
{
	return ((t_color)
		{
			.r = (color >> 24) & 0xFF,
			.g = (color >> 16) & 0xFF,
			.b = (color >> 8) & 0xFF
		});
}

t_color	blend_colors(t_color original_color, t_color reflected_color,
	float reflectivity)
{
	return ((t_color)
		{
			.r = reflectivity * reflected_color.r + (1 - reflectivity)
			* original_color.r,
			.g = reflectivity * reflected_color.g + (1 - reflectivity)
			* original_color.g,
			.b = reflectivity * reflected_color.b + (1 - reflectivity)
			* original_color.b
		});
}

t_vector	refract(t_vector incident, t_vector normal, float eta_ratio)
{
	double		cos_theta;
	double		sin_theta2;
	t_vector	r_out_perp;
	t_vector	r_out_parallel;

	cos_theta = fmin(dot(multiply_scalar(incident, -1.0), normal), 1.0);
	sin_theta2 = eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta);
	if (sin_theta2 > 1.0)
	{
		return (reflect(incident, normal));
	}
	r_out_perp = multiply_scalar(add(incident, multiply_scalar(normal,
					cos_theta)), eta_ratio);
	r_out_parallel = multiply_scalar(normal, -sqrt(fabs(1.0 - dot(r_out_perp,
						r_out_perp))));
	return (add(r_out_perp, r_out_parallel));
}

t_vector	get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder)
{
	t_vector	axis;
	t_vector	cp;
	t_vector	axis_point;
	t_vector	normal;
	double		dot_prod;

	axis = normalize(cylinder->orientation);
	cp = subtract(hit_point, cylinder->center);
	dot_prod = dot(cp, axis);
	axis_point = add(cylinder->center, multiply_scalar(axis, dot_prod));
	normal = normalize(subtract(hit_point, axis_point));
	return (normal);
}

t_vector	reflect(t_vector direction, t_vector normal)
{
	double	dot_product;

	dot_product = dot(direction, normal);
	return (subtract(direction, multiply_scalar(normal, 2.0 * dot_product)));
}

t_color	combine_color(t_color light_color, t_color object_color)
{
	t_color	result;

	result.r = fmin(255, (light_color.r / 255.0) * object_color.r);
	result.g = fmin(255, (light_color.g / 255.0) * object_color.g);
	result.b = fmin(255, (light_color.b / 255.0) * object_color.b);
	return (result);
}

t_color	get_cylinder_checkerboard_color(t_vector point, t_cylinder *cylinder,
	double scale)
{
	if (is_cylinder_checkerboard(point, cylinder, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

int	is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder,
	double scale)
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

	local_point = subtract(point, cylinder->center);
	height = dot(local_point, cylinder->orientation);
	projection = multiply_scalar(cylinder->orientation, height);
	radial_vector = subtract(local_point, projection);
	angle = atan2(radial_vector.z, radial_vector.x);
	u = (angle / (2.0 * M_PI)) + 0.5;
	v = (height + (cylinder->height / 2.0)) / cylinder->height;
	scaled_u = u * scale;
	scaled_v = v * scale;
	u_check = (int)floor(scaled_u) % 2;
	v_check = (int)floor(scaled_v) % 2;
	return ((u_check + v_check) % 2);
}

static t_ray	get_refraction_ray(t_vector point, t_vector normal,
	t_ray incoming_ray, double refractive_index)
{
	t_ray		refraction_ray;
	double		cos_i;
	double		eta_i;
	double		eta_t;
	double		eta;
	double		k;
	t_vector	refracted_dir;
	double		temp;

	cos_i = -dot(normal, incoming_ray.direction);
	eta_i = 1.0;
	eta_t = refractive_index;
	if (cos_i < 0)
	{
		cos_i = -cos_i;
		temp = eta_i;
		eta_i = eta_t;
		eta_t = temp;
		normal = multiply_scalar(normal, -1);
	}
	eta = eta_i / eta_t;
	k = 1 - eta * eta * (1 - cos_i * cos_i);
	if (k < 0)
	{
		refraction_ray.origin = point;
		refraction_ray.direction = reflect(incoming_ray.direction, normal);
	}
	else
	{
		refracted_dir = add(
				multiply_scalar(incoming_ray.direction, eta),
				multiply_scalar(normal, (eta * cos_i - sqrt(k))));
		refraction_ray.origin = point;
		refraction_ray.direction = normalize(refracted_dir);
	}
	return (refraction_ray);
}

t_hit_record	find_closest_intersection(t_ray ray, t_scene *scene)
{
	t_hit_record	hit;

	hit.index = -1;
	hit.t = INFINITY;
	hit.hit = 0;
	hit.hit_from_inside = 0;
	check_sphere_intersections(ray, scene, &hit);
	check_cylinder_intersections(ray, scene, &hit);
	check_disc_intersections(ray, scene, &hit);
	check_plane_intersections(ray, scene, &hit);
	return (hit);
}

static void	get_hit_normal_sphere(t_hit_record *hit, t_vector *normal,
	t_scene *scene)
{
	t_sphere	*sphere;

	if (hit->type == SPHERE)
	{
		sphere = &scene->spheres[hit->index];
		*normal = normalize(subtract(hit->point, sphere->center));
		hit->material = sphere->material;
	}
}

static void	get_hit_normal_cylinder(t_hit_record *hit, t_vector *normal,
	t_scene *scene)
{
	t_cylinder	*cylinder;

	if (hit->type == CYLINDER)
	{
		cylinder = &scene->cylinders[hit->index];
		*normal = get_cylinder_normal(hit->point, cylinder);
		hit->material = cylinder->material;
	}
}

static void	get_hit_normal_plane(t_hit_record *hit, t_vector *normal,
	t_scene *scene, t_ray ray)
{
	t_plane	*plane;

	if (hit->type == PLANE)
	{
		plane = &scene->planes[hit->index];
		*normal = plane->normal;
		if (dot(ray.direction, plane->normal) > 0)
			*normal = multiply_scalar(plane->normal, -1);
		hit->material = plane->material;
	}
}

static void	get_hit_normal_disc(t_hit_record *hit, t_vector *normal,
	t_scene *scene, t_ray ray)
{
	t_disc	*disc;

	if (hit->type == DISC)
	{
		disc = &scene->discs[hit->index];
		*normal = disc->normal;
		if (dot(ray.direction, disc->normal) > 0)
			*normal = multiply_scalar(disc->normal, -1);
		hit->material = disc->material;
	}
}

void	get_hit_normal(t_hit_record *hit, t_ray ray, t_vector *normal,
	t_scene *scene)
{
	get_hit_normal_sphere(hit, normal, scene);
	get_hit_normal_cylinder(hit, normal, scene);
	get_hit_normal_plane(hit, normal, scene, ray);
	get_hit_normal_disc(hit, normal, scene, ray);
}

static t_color	get_surface_color_sphere(t_scene *scene, t_hit_record *hit)
{
	t_sphere	*sphere;
	t_vector	local_point;
	double		u;
	double		v;

	sphere = &scene->spheres[hit->index];
	if (sphere->material.checker == 1)
	{
		local_point = subtract(hit->point, sphere->center);
		u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
		v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
		if ((int)(u * 20.0) % 2 == (int)(v * 20.0) % 2)
			return ((t_color){255, 255, 255});
		else
			return ((t_color){0, 0, 0});
	}
	else
		return (sphere->material.color);
}

static t_color	get_surface_color_cylinder(t_scene *scene, t_hit_record *hit)
{
	t_cylinder	*cylinder;
	t_color		black;
	t_color		white;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	cylinder = &scene->cylinders[hit->index];
	if (cylinder->material.checker == 1)
	{
		if (is_checkerboard(hit->point, cylinder, 0.5))
		{
			return (black);
		}
		else
		{
			return (white);
		}
	}
	else
		return (cylinder->material.color);
}

static t_color	get_surface_color_plane(t_scene *scene, t_hit_record *hit,
	t_vector normal)
{
	t_color	black;
	t_color	white;
	t_plane	*plane;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	plane = &scene->planes[hit->index];
	if (plane->material.checker == 1)
		return (get_plane_checkerboard_color(hit->point,
				black, white, normal, 0.5));
	else
		return (plane->material.color);
}

static t_color	get_surface_color_disc(t_scene *scene, t_hit_record *hit,
	t_vector normal)
{
	t_color	black;
	t_color	white;
	t_disc	*disc;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	disc = &scene->discs[hit->index];
	if (disc->material.checker == 1)
		return (get_disc_checkerboard_color(hit->point,
				disc, black, white, 0.5));
	else
		return (disc->material.color);
}

t_color	get_surface_color(t_hit_record *hit, t_vector normal,
	t_scene *scene, int depth)
{
	t_color	black;
	t_color	white;
	t_color	object_color;

	black = (t_color){255, 255, 255};
	white = (t_color){0, 0, 0};
	if (hit->type == SPHERE)
		object_color = get_surface_color_sphere(scene, hit);
	else if (hit->type == CYLINDER)
		object_color = get_surface_color_cylinder(scene, hit);
	else if (hit->type == PLANE)
		object_color = get_surface_color_plane(scene, hit, normal);
	else if (hit->type == DISC)
		object_color = get_surface_color_disc(scene, hit, normal);
	return (apply_lighting(hit->point, normal, object_color, scene, depth + 1));
}

t_color	calculate_object_color(t_hit_record *hit, t_ray ray,
	t_scene *scene, int depth)
{
	t_vector			normal;
	t_color				final_color;
	t_material_params	params;

	get_hit_normal(hit, ray, &normal, scene);
	final_color = get_surface_color(hit, normal, scene, depth);
	if (hit->material.reflectivity > 0.0 || hit->material.transparency > 0.0)
	{
		params.hit = hit;
		params.ray = ray;
		params.normal = normal;
		params.scene = scene;
		params.base_color = final_color;
		params.depth = depth;
		final_color = apply_material_effects(params);
	}
	return (final_color);
}

t_color	trace_ray(t_ray ray, t_scene *scene, int depth)
{
	t_hit_record	hit;
	t_color			black;

	black = (t_color){0, 0, 0};
	if (depth > MAX_REFLECTION_DEPTH)
		return (black);
	hit = find_closest_intersection(ray, scene);
	if (hit.hit)
	{
		hit.point = add(ray.origin,
				multiply_scalar(ray.direction, hit.t));
		return (calculate_object_color(&hit, ray, scene, depth));
	}
	return (black);
}

void	check_sphere_intersections(t_ray ray, t_scene *scene,
	t_hit_record *hit)
{
	int		i;
	double	t_sphere;

	i = 0;
	while (i < scene->num_spheres)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere)
			&& t_sphere < hit->t)
		{
			hit->hit = 1;
			hit->t = t_sphere;
			hit->type = SPHERE;
			hit->index = i;
		}
		i++;
	}
}

void	check_cylinder_intersections(t_ray ray, t_scene *scene,
	t_hit_record *hit)
{
	int		i;
	double	t_cylinder;

	i = 0;
	while (i < scene->num_cylinders)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_cylinder(&ray, &scene->cylinders[i], &t_cylinder)
			&& t_cylinder < hit->t)
		{
			hit->hit = 1;
			hit->t = t_cylinder;
			hit->type = CYLINDER;
			hit->index = i;
		}
		i++;
	}
}

void	check_disc_intersections(t_ray ray, t_scene *scene, t_hit_record *hit)
{
	int		i;
	double	t_disc;

	i = 0;
	while (i < scene->num_discs)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_disc(&ray, &scene->discs[i], &t_disc)
			&& t_disc < hit->t)
		{
			hit->hit = 1;
			hit->t = t_disc;
			hit->type = DISC;
			hit->index = i;
		}
		i++;
	}
}

void	check_plane_intersections(t_ray ray, t_scene *scene, t_hit_record *hit)
{
	int		i;
	double	t_plane;

	i = 0;
	while (i < scene->num_planes)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_plane(&ray, &scene->planes[i], &t_plane)
			&& t_plane < hit->t)
		{
			hit->hit = 1;
			hit->t = t_plane;
			hit->type = PLANE;
			hit->index = i;
		}
		i++;
	}
}

t_color	apply_reflection(t_material_params params, t_color base_color)
{
	t_ray	reflection_ray;
	t_color	reflected_color;

	reflection_ray = get_reflection_ray(params.hit->point,
			params.normal, params.ray);
	reflection_ray.origin = add(reflection_ray.origin,
			multiply_scalar(reflection_ray.direction, 0.001));
	reflected_color = trace_ray(reflection_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, reflected_color,
			params.hit->material.reflectivity));
}

t_color	apply_refraction(t_material_params params, t_color base_color)
{
	t_ray	refraction_ray;
	t_color	refracted_color;

	refraction_ray = get_refraction_ray(params.hit->point, params.normal,
			params.ray, params.hit->material.refractive_index);
	refraction_ray.origin = add(refraction_ray.origin,
			multiply_scalar(refraction_ray.direction, 0.001));
	refracted_color = trace_ray(refraction_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, refracted_color,
			params.hit->material.transparency));
}

t_color	apply_material_effects(t_material_params params)
{
	t_color	final_color;

	final_color = params.base_color;
	if (params.hit->material.reflectivity > 0.0)
		final_color = apply_reflection(params, final_color);
	if (params.hit->material.transparency > 0.0)
		final_color = apply_refraction(params, final_color);
	return (final_color);
}

double	schlick_reflection_coefficient(double cos_theta,
	double refractive_index)
{
	double	r0;

	r0 = pow((1 - refractive_index) / (1 + refractive_index), 2);
	return (r0 + (1 - r0) * pow((1 - cos_theta), 5));
}

void	*render_thread(void *arg)
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

	thread_data = (t_thread_data *)arg;
	data = thread_data->render_data;
	thread_id = thread_data->thread_id;
	num_threads = thread_data->num_threads;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			pixel_index = y * WIDTH + x;
			if (pixel_index % num_threads == thread_id)
			{
				ray = create_ray(x, y, &data->scene->camera);
				final_color = trace_ray(ray, data->scene, 5);
				color = (final_color.r << 24) | (final_color.g << 16)
					| (final_color.b << 8) | 0xFF;
				pthread_mutex_lock(&data->mutex);
				mlx_put_pixel(data->img, x, y, color);
				pthread_mutex_unlock(&data->mutex);
			}
			x++;
		}
		y++;
	}
	pthread_mutex_lock(&data->mutex);
	data->threads_completed++;
	if (data->threads_completed == NUM_THREADS)
	{
		gettimeofday(&data->end_time, NULL);
		elapsed_time = (data->end_time.tv_sec - data->start_time.tv_sec)
			+ (data->end_time.tv_usec - data->start_time.tv_usec) / 1e6;
		printf("Rendering took %f seconds\n", elapsed_time);
		save_image_to_file(data->img->pixels, WIDTH, HEIGHT, "output.png");
		data->rendering_finished = 1;
	}
	pthread_mutex_unlock(&data->mutex);
	free(thread_data);
	return (NULL);
}

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

static t_vector	scale(t_vector v, double s)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}

t_ray	create_ray(int x, int y, t_camera *camera)
{
	t_ray		ray;
	t_vector	r_vector;
	t_vector	dir[3];
	double		aspect_fov_scale;
	t_vector	image_point;

	r_vector.x = 0;
	r_vector.y = 1;
	r_vector.z = 0;
	dir[0] = normalize(camera->orientation);
	dir[1] = normalize(cross(r_vector, dir[0]));
	dir[2] = cross(dir[0], dir[1]);
	aspect_fov_scale = tan((camera->fov * M_PI / 180) / 2) * (double)WIDTH
		/ HEIGHT;
	image_point.x = (2 * (x + 0.5) / WIDTH - 1) * aspect_fov_scale;
	image_point.y = (1 - 2 * (y + 0.5) / HEIGHT) * aspect_fov_scale;
	image_point.z = 1;
	ray.origin = camera->pos;
	ray.direction = normalize(add(add(scale(dir[1], image_point.x),
					scale(dir[2], image_point.y)), scale(dir[0],
					image_point.z)));
	return (ray);
}

t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

t_vector	world_to_local(t_vector point, t_vector orientation,
	t_vector center)
{
	t_vector	local_point;
	t_vector	up;
	t_vector	right;
	t_vector	forward;
	t_vector	temp;

	local_point = subtract(point, center);
	up = orientation;
	if (fabs(up.y) < 0.999)
		temp = (t_vector){0, 1, 0};
	else
		temp = (t_vector){1, 0, 0};
	right = normalize(cross(up, temp));
	forward = cross(right, up);
	return ((t_vector)
		{
			dot(local_point, right),
			dot(local_point, up),
			dot(local_point, forward)
		});
}

void	update_display(void *param)
{
	t_render_data	*data;

	data = (t_render_data *)param;
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	pthread_mutex_lock(&data->mutex);
	if (data->rendering_finished)
	{
		mlx_terminate(data->mlx);
		pthread_mutex_unlock(&data->mutex);
		pthread_mutex_destroy(&data->mutex);
		free(data);
		exit(0);
	}
	pthread_mutex_unlock(&data->mutex);
}

static void	init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene,
	t_render_data *data)
{
	data->mlx = mlx;
	data->img = img;
	data->scene = scene;
	data->threads_completed = 0;
	data->rendering_finished = 0;
	pthread_mutex_init(&data->mutex, NULL);
	gettimeofday(&data->start_time, NULL);
}

void	render_scene(mlx_t *mlx, t_scene *scene)
{
	static int		i;
	pthread_t		threads[NUM_THREADS];
	mlx_image_t		*img;
	t_render_data	*data;
	t_thread_data	*thread_data;

	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
		exit_with_error("Error creating image");
	data = malloc(sizeof(t_render_data));
	if (!data)
		exit_with_error("Error allocating render data");
	init_render_scene(mlx, img, scene, data);
	mlx_loop_hook(mlx, update_display, data);
	while (i < NUM_THREADS)
	{
		thread_data = malloc(sizeof(t_thread_data));
		thread_data->render_data = data;
		thread_data->thread_id = i;
		thread_data->num_threads = NUM_THREADS;
		pthread_create(&threads[i], NULL, render_thread, thread_data);
		i++;
	}
	mlx_loop(mlx);
}
