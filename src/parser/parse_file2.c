/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_file2.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:45 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:46 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	parse_scene_element(char *line, t_scene *scene)
{
	if (ft_strncmp(line, "A ", 2) == 0)
		parse_ambient(line, scene);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_camera(line, scene);
	else if (ft_strncmp(line, "L ", 2) == 0)
		parse_light(line, scene);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		parse_sphere(line, scene);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		parse_plane(line, scene);
	else if (ft_strncmp(line, "cy ", 3) == 0)
	{
		parse_cylinder(line, scene);
		parse_discs(line, scene);
	}
	else if (ft_strncmp(line, "#", 1) != 0)
	{
		printf("line is: %s\n", line);
		exit_with_error("Invalid line in scene file");
	}
}

void	parse_file(const char *filename, t_scene *scene)
{
	int		fd;
	char	*line;

	open_file(filename, &fd);
	line = get_next_line_from_file(fd);
	if (line == NULL)
		exit_with_error("Error opening file");
	while (line != NULL)
	{
		parse_scene_element(line, scene);
		free(line);
		line = get_next_line_from_file(fd);
	}
	free(line);
	close(fd);
}
