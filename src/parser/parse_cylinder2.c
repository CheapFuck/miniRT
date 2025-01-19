/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder2.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:33 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:34 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	parse_disc_properties(char **tokens, t_disc *disc)
{
	char	**center_tokens;
	char	**orientation_tokens;

	center_tokens = split_and_validate(tokens[1], 3);
	if (!center_tokens)
		return (0);
	disc->center.x = ft_atof(center_tokens[0]);
	disc->center.y = ft_atof(center_tokens[1]);
	disc->center.z = ft_atof(center_tokens[2]);
	clean_2d_array(center_tokens);
	orientation_tokens = split_and_validate(tokens[2], 3);
	if (!orientation_tokens)
		return (0);
	disc->orientation.x = ft_atof(orientation_tokens[0]);
	disc->orientation.y = ft_atof(orientation_tokens[1]);
	disc->orientation.z = ft_atof(orientation_tokens[2]);
	clean_2d_array(orientation_tokens);
	return (normalize_orientation_disc(disc));
}

int	parse_color(char *color_str, t_color *color)
{
	char	**color_tokens;

	color_tokens = split_and_validate(color_str, 3);
	if (!color_tokens)
		return (0);
	color->r = ft_atoi(color_tokens[0]);
	color->g = ft_atoi(color_tokens[1]);
	color->b = ft_atoi(color_tokens[2]);
	clean_2d_array(color_tokens);
	return (1);
}

void	parse(char *error, char **tokens)
{
	printf("%s\n", error);
	ft_free_split(tokens);
	exit(EXIT_FAILURE);
}

static void	parse_cylinder_material(t_cylinder *cylinder, char **tokens)
{
	cylinder->material.checker = ft_atoi(tokens[6]);
	cylinder->material.reflectivity = ft_atof(tokens[7]);
	cylinder->material.transparency = ft_atof(tokens[8]);
	cylinder->material.refractive_index = ft_atof(tokens[9]);
	cylinder->radius = cylinder->diameter / 2.0;
}

void	parse_cylinder(char *line, t_scene *scene)
{
	t_cylinder	cylinder;
	char		**tokens;

	cylinder = (t_cylinder){0};
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 10)
		return (parse("Error: Invalid cylinder format", tokens));
	if (!parse_cylinder_properties(tokens, &cylinder))
		return (ft_free_split(tokens));
	if (!validate_nrmlzd_vector(&cylinder.orientation, "Cylinder orientation"))
		return (ft_free_split(tokens));
	cylinder.diameter = ft_atof(tokens[3]);
	if (cylinder.diameter <= 0)
		return (parse("Error: Cylinder diameter must be positive", tokens));
	cylinder.height = ft_atof(tokens[4]);
	if (cylinder.height <= 0)
		return (parse("Error: Cylinder height must be positive", tokens));
	if (!parse_color(tokens[5], &cylinder.material.color)
		|| !validate_color(&cylinder.material.color))
		return (ft_free_split(tokens));
	parse_cylinder_material(&cylinder, tokens);
	if (scene->num_cylinders >= 65536)
		return (parse("Error: Maximum number of cylinders exceeded", tokens));
	scene->cylinders[scene->num_cylinders++] = cylinder;
	ft_free_split(tokens);
}
