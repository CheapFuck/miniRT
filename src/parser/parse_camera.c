/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_camera.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:15:04 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:15:06 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	parse_camera_position_and_orientation(char *line, t_scene *scene)
{
	char	**tokens;
	char	**pos;
	char	**orient;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 4)
		exit_with_error("Invalid camera format");
	pos = ft_split(tokens[1], ',');
	if (!pos || ft_arraylen(pos) != 3)
		exit_with_error("Invalid camera position format");
	scene->camera.pos.x = ft_atof(pos[0]);
	scene->camera.pos.y = ft_atof(pos[1]);
	scene->camera.pos.z = ft_atof(pos[2]);
	orient = ft_split(tokens[2], ',');
	if (!orient || ft_arraylen(orient) != 3)
		exit_with_error("Invalid camera orientation format");
	scene->camera.orientation.x = ft_atof(orient[0]);
	scene->camera.orientation.y = ft_atof(orient[1]);
	scene->camera.orientation.z = ft_atof(orient[2]);
	normalize(scene->camera.orientation);
	ft_free_split(tokens);
	ft_free_split(pos);
	ft_free_split(orient);
}

static void	validate_camera_orientation(t_scene *scene, char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 4)
		exit_with_error("Invalid camera format");
	if (!validate_nrmlzd_vector(&scene->camera.orientation, "Camera"))
	{
		ft_free_split(tokens);
		exit_with_error("Invalid camera orientation vector");
	}
	scene->camera.fov = ft_atoi(tokens[3]);
	if (!validate_fov(scene->camera.fov))
	{
		ft_free_split(tokens);
		exit_with_error("FOV must be between 0 and 180 degrees");
	}
	scene->has_camera = 1;
	ft_free_split(tokens);
}

void	parse_camera(char *line, t_scene *scene)
{
	validate_unique_element(scene, 'C');
	parse_camera_position_and_orientation(line, scene);
	validate_camera_orientation(scene, line);
}
