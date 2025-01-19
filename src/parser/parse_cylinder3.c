/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder3.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:36 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:37 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	disc_parse(t_scene *scene, t_disc disc, char **tokens)
{
	t_disc		bottom_disc;
	t_vector	bottom_offset;
	t_disc		top_disc;
	t_vector	top_offset;

	bottom_offset = multiply_scalar(disc.orientation, -0.5 * disc.height);
	bottom_disc.center = add(disc.center, bottom_offset);
	bottom_disc.normal = disc.orientation;
	bottom_disc.radius = disc.radius;
	bottom_disc.color = disc.color;
	top_offset = multiply_scalar(disc.orientation, 0.5 * disc.height);
	top_disc.center = add(disc.center, top_offset);
	top_disc.normal = disc.orientation;
	top_disc.radius = disc.radius;
	top_disc.color = disc.color;
	if (scene->num_discs + 2 >= 65535)
		return (parse("Error: Maximum number of discs exceeded", tokens));
	scene->discs[scene->num_discs++] = bottom_disc;
	scene->discs[scene->num_discs++] = top_disc;
}

void	parse_discs(char *line, t_scene *scene)
{
	t_disc	disc;
	char	**tokens;

	disc = (t_disc){0};
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) < 6)
		return (parse("Error: Invalid cylinder format for discs", tokens));
	if (!parse_disc_properties(tokens, &disc))
		return (parse("Error: Failed to parse cylinder properties", tokens));
	disc.diameter = ft_atof(tokens[3]);
	disc.radius = disc.diameter / 2.0;
	disc.height = ft_atof(tokens[4]);
	if (!parse_color(tokens[5], &disc.color))
		return (handle_pars_error(tokens, "Error: Invalid disc color format"));
	disc_parse(scene, disc, tokens);
	ft_free_split(tokens);
}
