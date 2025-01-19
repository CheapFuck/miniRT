/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_plane.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:54 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:55 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	handle_plane_parse_error(char **tokens, char **split_tokens,
	char *error_message)
{
	ft_free_split(split_tokens);
	ft_free_split(tokens);
	exit_with_error(error_message);
}

int	parse_plane_tokens(char *token, t_vector *vector)
{
	char	**vector_tokens;

	vector_tokens = split_and_validate(token, 3);
	if (!vector_tokens)
		return (0);
	vector->x = ft_atof(vector_tokens[0]);
	vector->y = ft_atof(vector_tokens[1]);
	vector->z = ft_atof(vector_tokens[2]);
	ft_free_split(vector_tokens);
	return (1);
}

int	parse_plane_vector(char *token, t_vector *normal, char **tokens)
{
	double	length;

	if (!parse_plane_tokens(token, normal))
		handle_plane_parse_error(tokens, NULL, "Invalid plane normal format");
	length = sqrt(pow(normal->x, 2) + pow(normal->y, 2) + pow(normal->z, 2));
	if (length == 0.0)
		handle_plane_parse_error(tokens, NULL,
			"Plane normal vector cannot be zero");
	normal->x /= length;
	normal->y /= length;
	normal->z /= length;
	return (1);
}

int	parse_plane_color(char *token, t_color *color, char **tokens)
{
	char	**color_tokens;

	color_tokens = split_and_validate(token, 3);
	if (!color_tokens)
		handle_plane_parse_error(tokens, color_tokens,
			"Invalid plane color format");
	color->r = ft_atoi(color_tokens[0]);
	color->g = ft_atoi(color_tokens[1]);
	color->b = ft_atoi(color_tokens[2]);
	ft_free_split(color_tokens);
	if (color->r < 0 || color->r > 255 || color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		handle_plane_parse_error(tokens, NULL,
			"Plane color out of range (0 to 255)");
	return (1);
}

void	parse_plane(char *line, t_scene *scene)
{
	char	**tokens;
	t_plane	plane;

	plane = (t_plane){0};
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 8)
		handle_plane_parse_error(tokens, NULL, "Invalid plane format");
	if (!parse_plane_tokens(tokens[1], &plane.point))
		handle_plane_parse_error(tokens, NULL, "Invalid plane point format");
	if (!parse_plane_tokens(tokens[2], &plane.normal))
		handle_plane_parse_error(tokens, NULL, "Invalid plane normal format");
	if (!validate_nrmlzd_vector(&plane.normal, "Plane normal"))
		return (ft_free_split(tokens));
	if (!parse_plane_vector(tokens[2], &plane.normal, tokens)
		|| !parse_plane_color(tokens[3], &plane.material.color, tokens))
		return ;
	if (!validate_color(&plane.material.color))
		return (ft_free_split(tokens));
	plane.material.checker = ft_atoi(tokens[4]);
	plane.material.reflectivity = ft_atof(tokens[5]);
	if (scene->num_planes >= 65536)
		handle_plane_parse_error(tokens, NULL, "Plane array is full");
	scene->planes[scene->num_planes] = plane;
	scene->num_planes++;
	ft_free_split(tokens);
}
