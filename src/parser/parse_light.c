/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_light.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:50 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:50 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	parse_light_position(char **tokens, t_light *light)
{
	char	**pos;

	pos = split_and_validate(tokens[1], 3);
	if (!pos)
	{
		ft_free_split(pos);
		ft_free_split(tokens);
		exit_with_error("Invalid light position format");
		return (0);
	}
	light->pos.x = ft_atof(pos[0]);
	light->pos.y = ft_atof(pos[1]);
	light->pos.z = ft_atof(pos[2]);
	ft_free_split(pos);
	return (1);
}

int	parse_light_brightness(char **tokens, t_light *light)
{
	light->brightness = ft_atof(tokens[2]);
	if (light->brightness < 0 || light->brightness > 1)
	{
		ft_free_split(tokens);
		exit_with_error("Light brightness out of range (0 to 1)");
		return (0);
	}
	return (1);
}

int	parse_light_color(char **tokens, t_light *light)
{
	char	**color;

	color = split_and_validate(tokens[3], 3);
	if (!color)
	{
		ft_free_split(color);
		ft_free_split(tokens);
		exit_with_error("Invalid light color format");
		return (0);
	}
	light->color.r = ft_atoi(color[0]);
	light->color.g = ft_atoi(color[1]);
	light->color.b = ft_atoi(color[2]);
	if (light->color.r < 0 || light->color.r > 255
		|| light->color.g < 0 || light->color.g > 255
		|| light->color.b < 0 || light->color.b > 255)
	{
		ft_free_split(color);
		ft_free_split(tokens);
		exit_with_error("Light color out of range (0 to 255)");
		return (0);
	}
	ft_free_split(color);
	return (1);
}

void	parse_light(char *line, t_scene *scene)
{
	char		**tokens;

	validate_unique_element(scene, 'L');
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 4)
	{
		ft_free_split(tokens);
		exit_with_error("Invalid light format");
	}
	scene->lights[scene->num_lights].pos.x = 0.0;
	scene->lights[scene->num_lights].pos.y = 0.0;
	scene->lights[scene->num_lights].pos.z = 0.0;
	scene->lights[scene->num_lights].radius = 0.0;
	if (!parse_light_position(tokens, &scene->lights[scene->num_lights]))
		return ;
	scene->lights[scene->num_lights].brightness = ft_atof(tokens[2]);
	if (!validate_ratio(scene->lights[scene->num_lights].brightness,
			"Light brightness"))
		return (ft_free_split(tokens));
	if (!parse_light_color(tokens, &scene->lights[scene->num_lights]))
		return ;
	if (!validate_color(&scene->lights[scene->num_lights].color))
		return (ft_free_split(tokens));
	scene->num_lights++;
	ft_free_split(tokens);
}

void	parse_ambient(char *line, t_scene *scene)
{
	char	**tokens;
	char	**colors;

	validate_unique_element(scene, 'A');
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 3)
		exit_with_error("Invalid ambient light format");
	scene->ambient.ratio = ft_atof(tokens[1]);
	if (!validate_ratio(scene->ambient.ratio, "Ambient light"))
		return (ft_free_split(tokens));
	colors = ft_split(tokens[2], ',');
	if (!colors || ft_arraylen(colors) != 3)
		exit_with_error("Invalid ambient color format");
	scene->ambient.color.r = ft_atoi(colors[0]);
	scene->ambient.color.g = ft_atoi(colors[1]);
	scene->ambient.color.b = ft_atoi(colors[2]);
	if (!validate_color(&scene->ambient.color))
	{
		ft_free_split(tokens);
		ft_free_split(colors);
		return ;
	}
	ft_free_split(tokens);
	ft_free_split(colors);
}
