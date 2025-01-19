/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_sphere.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:59 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	parse_sphere_material(t_sphere *sphere, char **tokens,
	char **colors)
{
	sphere->material.color.r = ft_atoi(colors[0]);
	sphere->material.color.g = ft_atoi(colors[1]);
	sphere->material.color.b = ft_atoi(colors[2]);
	sphere->material.checker = ft_atoi(tokens[4]);
	sphere->material.reflectivity = ft_atof(tokens[5]);
	sphere->material.transparency = ft_atof(tokens[6]);
	sphere->material.refractive_index = ft_atof(tokens[7]);
}

static void	sphere_parse(char **tokens, char **pos, t_sphere sphere,
		t_scene *scene)
{
	char	**colors;

	colors = ft_split(tokens[3], ',');
	if (!colors || ft_arraylen(colors) != 3)
	{
		ft_free_split(tokens);
		ft_free_split(pos);
		exit_with_error("Invalid sphere color format");
	}
	parse_sphere_material(&sphere, tokens, colors);
	if (!validate_color(&sphere.material.color))
	{
		ft_free_split(tokens);
		ft_free_split(pos);
		ft_free_split(colors);
		return ;
	}
	scene->spheres[scene->num_spheres] = sphere;
	scene->num_spheres++;
	ft_free_split(colors);
	ft_free_split(tokens);
	ft_free_split(pos);
}

void	parse_sphere(char *line, t_scene *scene)
{
	static t_sphere	sphere;
	char			**tokens;
	char			**pos;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 8)
		exit_with_error("Invalid sphere format");
	pos = ft_split(tokens[1], ',');
	if (!pos || ft_arraylen(pos) != 3)
	{
		ft_free_split(tokens);
		exit_with_error("Invalid sphere position format");
	}
	sphere.center.x = ft_atof(pos[0]);
	sphere.center.y = ft_atof(pos[1]);
	sphere.center.z = ft_atof(pos[2]);
	sphere.radius = ft_atof(tokens[2]) / 2.0;
	if (sphere.radius <= 0)
	{
		printf("Error: Sphere diameter must be positive\n");
		ft_free_split(tokens);
		ft_free_split(pos);
		exit(EXIT_FAILURE);
	}
	sphere_parse(tokens, pos, sphere, scene);
}

int	validate_ratio(double value, const char *element_name)
{
	if (value < 0.0 || value > 1.0)
	{
		printf("Error: %s ratio must be between 0.0 and 1.0\n", element_name);
		return (0);
	}
	return (1);
}
