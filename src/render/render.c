#include "../includes/minirt.h"
#include <sys/time.h>
#include <pthread.h>

t_ray get_reflection_ray(t_vector hit_point, t_vector normal, t_ray incident_ray)
{
    t_ray reflection;
	double dot_product;
	t_vector reflection_dir;

    reflection.origin = hit_point;
    
    dot_product = dot(incident_ray.direction, normal);
    reflection_dir = subtract(incident_ray.direction, 
					multiply_scalar(normal, 2.0 * dot_product));
    reflection.direction = normalize(reflection_dir);
    return (reflection);
}

t_color uint32_to_t_color(uint32_t color)
{
    return ((t_color)
	{
        .r = (color >> 24) & 0xFF,
        .g = (color >> 16) & 0xFF,
        .b = (color >> 8) & 0xFF
    });
}

t_color blend_colors(t_color original_color, t_color reflected_color, float reflectivity)
{
    return ((t_color)
	{
        .r = reflectivity * reflected_color.r + (1 - reflectivity) * original_color.r,
        .g = reflectivity * reflected_color.g + (1 - reflectivity) * original_color.g,
        .b = reflectivity * reflected_color.b + (1 - reflectivity) * original_color.b
    });
}

t_vector refract(t_vector incident, t_vector normal, float eta_ratio)
{
	double cos_theta;
	double sin_theta2;
	t_vector r_out_perp;
	t_vector r_out_parallel;

    cos_theta = fmin(dot(multiply_scalar(incident, -1.0), normal), 1.0);
    sin_theta2 = eta_ratio * eta_ratio * (1.0 - cos_theta * cos_theta);
    if (sin_theta2 > 1.0) {
        return (reflect(incident, normal));
    }
	r_out_perp = multiply_scalar(add(incident, multiply_scalar(normal, cos_theta)), eta_ratio);
    r_out_parallel = multiply_scalar(normal, -sqrt(fabs(1.0 - dot(r_out_perp, r_out_perp))));
    return (add(r_out_perp, r_out_parallel));
}

t_vector get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder)
{
    t_vector	axis;
    t_vector	cp;
    double		dot_prod;
	t_vector	axis_point;
	t_vector	normal;

	axis = normalize(cylinder->orientation);
	cp = subtract(hit_point, cylinder->center);
	dot_prod = dot(cp, axis);
	axis_point = add(cylinder->center, multiply_scalar(axis, dot_prod));
    normal = normalize(subtract(hit_point, axis_point));
    return (normal);
}
t_vector reflect(t_vector direction, t_vector normal)
{
    double dot_product;

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

t_color get_cylinder_checkerboard_color(t_vector point, t_cylinder *cylinder,
		t_color color1, t_color color2, double scale)
{
    if (is_cylinder_checkerboard(point, cylinder, scale))
        return (color1);
    return (color2);
}

int	is_cylinder_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
{
    t_vector    vectors[4];
    double      doubles[6];
	int			u_check;
	int			v_check;

	vectors[0] = subtract(point, cylinder->center);
    doubles[0] = dot(vectors[0], cylinder->orientation);
    vectors[1] = multiply_scalar(cylinder->orientation, doubles[0]);
	vectors[2] = subtract(vectors[0], vectors[1]);
    doubles[1] = atan2(vectors[2].z, vectors[2].x);
	doubles[2] = (doubles[1] / (2.0 * M_PI)) + 0.5;
	doubles[3] = (doubles[0] + (cylinder->height / 2.0)) / cylinder->height;
    doubles[4] = doubles[2] * scale;
    doubles[5] = doubles[3] * scale;
    u_check = (int)floor(doubles[4]) % 2;
    v_check = (int)floor(doubles[5]) % 2;
    return ((u_check + v_check) % 2);
}

static void	adjust_refraction_params(t_vector *normal, double *cos_i, double *eta_i, double *eta_t)
{
	double temp;

	if (*cos_i < 0)
	{
		*cos_i = -*cos_i;
		temp = *eta_i;
		*eta_i = *eta_t;
		*eta_t = temp;
		*normal = multiply_scalar(*normal, -1);
	}
}

static t_vector	calculate_refracted_dir(t_vector incoming_dir, t_vector normal, double eta, double cos_i, double k)
{
	return (add(multiply_scalar(incoming_dir, eta),
			multiply_scalar(normal, (eta * cos_i - sqrt(k)))));
}

static t_ray	create_refraction_ray(t_vector point, t_vector direction)
{
	t_ray ray;

	ray.origin = point;
	ray.direction = normalize(direction);
	return (ray);
}

t_ray	get_refraction_ray(t_vector point, t_vector normal, t_ray incoming_ray, double refractive_index)
{
	double		cos_i;
	double		eta_i;
	double		eta_t;
	double		eta;
	double		k;
	t_vector	refracted_dir;

	cos_i = -dot(normal, incoming_ray.direction);
	eta_i = 1.0;
	eta_t = refractive_index;
	adjust_refraction_params(&normal, &cos_i, &eta_i, &eta_t);
	eta = eta_i / eta_t;
	k = 1 - eta * eta * (1 - cos_i * cos_i);
	if (k < 0)
		return (create_refraction_ray(point, reflect(incoming_ray.direction, normal)));
	refracted_dir = calculate_refracted_dir(incoming_ray.direction, normal, eta, cos_i, k);
	return (create_refraction_ray(point, refracted_dir));
}


static void init_hit_record(t_hit_record *record)
{
    record->point =(t_vector){0 , 0, 0};
	record->normal =(t_vector){0 , 0, 0};
	record->t = 0.0;
	record->color = (t_color){0,0,0};
	record->material = (t_material){0.0, 0.0, 0.0, (t_color){0,0,0}, 0};
	record->hit = 0;
	record->index = 0;
	// record->type = (t_object_type)NULL;
	record->hit_from_inside = 0;
}

void hit_processing(t_hit_record *hit, t_object_type type, double object, int i)
{
    hit->hit = 1;
    hit->t = object;
    hit->type = type;
    hit->index = i;

}

static void loop_intersect_sphere(t_scene *scene, t_ray *ray, t_hit_record *hit)
{
    int i;

    i = 0;
    while (i < scene->num_spheres)
    {
        double sphere;
        ray->direction = normalize(ray->direction);
        if (intersect_sphere(ray, &scene->spheres[i], &sphere) && sphere < hit->t)
            hit_processing(hit, SPHERE, sphere, i);
        i++;
    }
}

static void loop_intersect_cylinder(t_scene *scene, t_ray *ray, t_hit_record *hit)
{
    int i;

    i = 0;
    while (i < scene->num_cylinders)
    {
        double cylinder;
        ray->direction = normalize(ray->direction);
        if (intersect_cylinder(ray, &scene->cylinders[i], &cylinder) && cylinder < hit->t)
            hit_processing(hit, CYLINDER, cylinder, i);
        i++;
    }
}

static void loop_intersect_disc(t_scene *scene, t_ray *ray, t_hit_record *hit)
{
    int i;

    i = 0;
    while (i < scene->num_discs)
    {
        double disc;
        ray->direction = normalize(ray->direction);
        if (intersect_disc(ray, &scene->discs[i], &disc) && disc < hit->t)
            hit_processing(hit, DISC, disc, i);
        i++;
    }
}

static void loop_intersect_plane(t_scene *scene, t_ray *ray, t_hit_record *hit)
{
    int i;

    i = 0;
    while (i < scene->num_planes)
    {
        double plane;
        ray->direction = normalize(ray->direction);
        if (intersect_plane(ray, &scene->planes[i], &plane) && plane < hit->t)
            hit_processing(hit, PLANE, plane, i);
        i++;
    }
}
static void	init_colors(t_color *final_color)
{
	*final_color = (t_color){0, 0, 0};
}

void	init_hit(t_hit_record *hit)
{
	hit->index = -1;
	hit->t = INFINITY;
	hit->hit = 0;
	hit->hit_from_inside = 0;
}
static void	initialize_and_intersect(t_scene *scene, t_ray *ray,
	t_color *final_color, t_hit_record *hit)
{
    init_hit_record(hit);
	init_colors(final_color);
	init_hit(hit);
	loop_intersect_sphere(scene, ray, hit);
	loop_intersect_cylinder(scene, ray, hit);
	loop_intersect_disc(scene, ray, hit);
	loop_intersect_plane(scene, ray, hit);
}

static t_color	process_sphere_hit(t_hit_record *hit, t_scene *scene, t_vector *normal)
{
    t_sphere	*sphere;
	t_vector	local_point;
	double		u;
	double		v;
	int			check_u;
	int			check_v;
	t_color		object_color;

	sphere = &scene->spheres[hit->index];
	*normal = normalize(subtract(hit->point, sphere->center));
	hit->material = scene->spheres[hit->index].material;
    if (sphere->material.checker == 1)
	{
		local_point = subtract(hit->point, sphere->center);
		u = 2.0 + atan2(local_point.z, local_point.x) / (2 * M_PI);
		v = 2.0 - asin(local_point.y / sphere->radius) / M_PI;
		check_u = (int)(u * 20.0) % 2;
		check_v = (int)(v * 20.0) % 2;
		object_color = (check_u == check_v) ? (t_color){0, 0, 0} : (t_color){255, 255, 255};
		return (apply_lighting(hit->point, *normal, object_color, scene));
	}
	
    return (apply_lighting(hit->point, *normal, sphere->material.color, scene));
    
}



static t_color	process_cylinder_hit(t_hit_record *hit, t_scene *scene, t_vector *normal)
{
    t_cylinder *cylinder = &scene->cylinders[hit->index];
    *normal = get_cylinder_normal(hit->point, cylinder);
    hit->material = cylinder->material;
    if (cylinder->material.checker == 1)
    {
        int is_black = is_checkerboard(hit->point, cylinder, 0.5);
        t_color object_color = is_black ? (t_color){255, 255, 255} : (t_color){0, 0, 0};
        return (apply_lighting(hit->point, *normal, object_color, scene));
    }
    else
        return(apply_lighting(hit->point, *normal, cylinder->material.color, scene));
    
}

static t_color	process_disc_hit(t_hit_record *hit, t_scene *scene, t_ray ray, t_vector *normal)
{
    t_disc *disc = &scene->discs[hit->index];
    *normal = disc->normal;
    hit->material = disc->material;
    if (dot(ray.direction, *normal) > 0)
        *normal = multiply_scalar(*normal, -1);
    if (disc->material.checker == 1)
    {
        t_color object_color = get_disc_checkerboard_color(hit->point, disc, 0.5);
        return(apply_lighting(hit->point, *normal, object_color, scene));
    }
    else
        return(apply_lighting(hit->point, *normal, disc->material.color, scene));
}


static t_color	process_plane_hit(t_hit_record *hit, t_scene *scene, t_ray ray, t_vector *normal)
{
    t_plane *plane = &scene->planes[hit->index];
    *normal = plane->normal;
    hit->material = plane->material;
    if (dot(ray.direction, *normal) > 0)
        *normal = multiply_scalar(*normal, -1);
    if (plane->material.checker == 1)
    {
        t_color object_color = get_plane_checkerboard_color(hit->point, *normal, 0.5);
        return(apply_lighting(hit->point, *normal, object_color, scene));
    }
    else
        return(apply_lighting(hit->point, *normal, plane->material.color, scene));
    
    
}

static t_color paint_object(t_hit_record *hit, t_scene *scene, t_ray ray, t_vector *normal)
{
       if (hit->type == SPHERE)
            return( process_sphere_hit(hit, scene, normal));
        else if (hit->type == CYLINDER)
            return( process_cylinder_hit(hit, scene, normal));
        else if (hit->type == PLANE)
            return(process_plane_hit(hit, scene, ray, normal));
        else if (hit->type == DISC)
            return(process_disc_hit(hit, scene, ray, normal));
}

t_color	trace_ray(t_ray ray, t_scene *scene)
{
    t_hit_record	hit;
	t_color			final_color;
	t_vector		normal;

    normal = (t_vector){0,0,0,};
    initialize_and_intersect(scene, &ray, &final_color, &hit);
    if (hit.hit)
    {
        hit.point = add(ray.origin, multiply_scalar(ray.direction, hit.t));
        final_color = paint_object(&hit, scene, ray, &normal);
        if (hit.material.reflectivity > 0.0 || hit.material.transparency > 0.0)
        {
            t_ray reflection_ray = get_reflection_ray(hit.point, normal, ray);
            reflection_ray.origin = add(reflection_ray.origin, multiply_scalar(reflection_ray.direction, 0.001));
            t_color reflected_color = trace_ray(reflection_ray, scene);
            final_color = blend_colors(final_color, reflected_color, hit.material.reflectivity);
            t_ray refraction_ray = get_refraction_ray(hit.point, normal, ray, hit.material.refractive_index);
            refraction_ray.origin = add(refraction_ray.origin, multiply_scalar(refraction_ray.direction, 0.001));
            t_color refracted_color = trace_ray(refraction_ray, scene);
            final_color = blend_colors(final_color, refracted_color, hit.material.transparency);
        }
    }
    return (final_color);
}

double	schlick_reflection_coefficient(double cos_theta, double refractive_index)
{
    double r0 = pow((1 - refractive_index) / (1 + refractive_index), 2);
    return r0 + (1 - r0) * pow((1 - cos_theta), 5);
}

static void	process_pixel(int x, int y, t_render_data *data, int thread_id, int num_threads)
{
	int		pixel_index;
	t_ray	ray;
	t_color	final_color;
	uint32_t color;

	pixel_index = y * WIDTH + x;
	if (pixel_index % num_threads == thread_id)
	{
		ray = create_ray(x, y, &data->scene->camera);
		final_color = trace_ray(ray, data->scene);
		color = (final_color.r << 24) | (final_color.g << 16) 
				| (final_color.b << 8) | 0xFF;
		pthread_mutex_lock(&data->mutex);
		mlx_put_pixel(data->img, x, y, color);
		pthread_mutex_unlock(&data->mutex);
	}
}

static void	update_render_progress(t_render_data *data)
{
	double	elapsed_time;

	pthread_mutex_lock(&data->mutex);
	data->threads_completed++;
	if (data->threads_completed == NUM_THREADS)
	{
		gettimeofday(&data->end_time, NULL);
		elapsed_time = (data->end_time.tv_sec - data->start_time.tv_sec) + 
					   (data->end_time.tv_usec - data->start_time.tv_usec) / 1e6;
		printf("Rendering took %f seconds\n", elapsed_time);
		save_image_to_file(data->img->pixels, WIDTH, HEIGHT, "output.png");
		data->rendering_finished = 1;
	}
	pthread_mutex_unlock(&data->mutex);
}

void	*render_thread(void *arg)
{
	t_thread_data	*thread_data;
	t_render_data	*data;
	int				x;
	int				y;

	thread_data = (t_thread_data *)arg;
	data = thread_data->render_data;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			process_pixel(x, y, data, thread_data->thread_id, thread_data->num_threads);
			x++;
		}
		y++;
	}
	update_render_progress(data);
	free(thread_data);
	return (NULL);
}


int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

t_ray	create_ray(int x, int y, t_camera *camera)
{
    t_ray		ray;
    double		aspect_ratio;
	double		fov_scale;
	double		norm_x;
	double		norm_y;
	t_vector	right;
	t_vector	up;

    aspect_ratio = (double)WIDTH / HEIGHT;
	fov_scale = tan((camera->fov * M_PI / 180) / 2);
    norm_x = (2 * (x + 0.5) / WIDTH - 1) * aspect_ratio * fov_scale;
    norm_y = (1 - 2 * (y + 0.5) / HEIGHT) * fov_scale;
    right = normalize(cross((t_vector){0, 1, 0}, camera->orientation));
    up = cross(camera->orientation, right);
    ray.origin = camera->pos;
    ray.direction.x = norm_x * right.x + norm_y * up.x + camera->orientation.x;
    ray.direction.y = norm_x * right.y + norm_y * up.y + camera->orientation.y;
    ray.direction.z = norm_x * right.z + norm_y * up.z + camera->orientation.z;
    ray.direction = normalize(ray.direction);
    return (ray);
}

t_vector	cross(t_vector a, t_vector b)
{
    t_vector result;

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

t_vector	world_to_local(t_vector point, t_vector orientation, t_vector center)
{
    t_vector local_point = subtract(point, center);
    t_vector up = orientation;
    t_vector right = normalize(cross(up, (fabs(up.y) < 0.999) ? (t_vector){0, 1, 0} : (t_vector){1, 0, 0}));
    t_vector forward = cross(right, up);

    return (t_vector)
    {
        dot(local_point, right),
        dot(local_point, up),
        dot(local_point, forward)
    };
}

void	update_display(void *param)
{
    t_render_data *data;

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

static void init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene, t_render_data *data)
{
	data->mlx = mlx;
    data->img = img;
    data->scene = scene;
    data->threads_completed = 0;
    data->rendering_finished = 0;
	pthread_mutex_init(&data->mutex, NULL);
    gettimeofday(&data->start_time, NULL);
}

void render_scene(mlx_t *mlx, t_scene *scene)
{
    static int		i;
    const int		num_threads = NUM_THREADS;
	pthread_t		threads[num_threads];
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
    while (i < num_threads)
	{
    	thread_data = malloc(sizeof(t_thread_data));
    	thread_data->render_data = data;
    	thread_data->thread_id = i;
		thread_data->num_threads = num_threads;
    	pthread_create(&threads[i], NULL, render_thread, thread_data);
    	i++;
	}
    mlx_loop(mlx);
}
