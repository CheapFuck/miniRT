/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder1.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:29 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:30 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	handle_pars_error(char **tokens, const char *error_message)
{
	printf("%s\n", error_message);
	clean_2d_array(tokens);
	exit(EXIT_FAILURE);
}

char	**split_and_validate(char *str, int expected_parts)
{
	char	**tokens;
	int		i;

	tokens = ft_split(str, ',');
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
		i++;
	if (i != expected_parts)
	{
		handle_pars_error(tokens, "Error: Wrong number of values");
		return (NULL);
	}
	i = 0;
	while (i < expected_parts)
	{
		if (!tokens[i] || !is_valid_number(tokens[i]))
		{
			handle_pars_error(tokens, "Error: Invalid number format");
			return (NULL);
		}
		i++;
	}
	return (tokens);
}

int	normalize_orientation(t_cylinder *cylinder)
{
	double	length;

	length = sqrt(pow(cylinder->orientation.x, 2)
			+ pow(cylinder->orientation.y, 2)
			+ pow(cylinder->orientation.z, 2));
	if (length == 0.0)
		exit_with_error("Error: Cylinder orientation vector cannot be zero");
	cylinder->orientation.x /= length;
	cylinder->orientation.y /= length;
	cylinder->orientation.z /= length;
	return (1);
}

int	normalize_orientation_disc(t_disc *disc)
{
	double	length;

	length = sqrt(pow(disc->orientation.x, 2)
			+ pow(disc->orientation.y, 2)
			+ pow(disc->orientation.z, 2));
	if (length == 0.0)
		exit_with_error("Error: Cylinder orientation vector cannot be zero");
	disc->orientation.x /= length;
	disc->orientation.y /= length;
	disc->orientation.z /= length;
	return (1);
}

int	parse_cylinder_properties(char **tokens, t_cylinder *cylinder)
{
	char	**center_tokens;
	char	**orientation_tokens;

	center_tokens = split_and_validate(tokens[1], 3);
	if (!center_tokens)
		return (0);
	cylinder->center.x = ft_atof(center_tokens[0]);
	cylinder->center.y = ft_atof(center_tokens[1]);
	cylinder->center.z = ft_atof(center_tokens[2]);
	clean_2d_array(center_tokens);
	orientation_tokens = split_and_validate(tokens[2], 3);
	if (!orientation_tokens)
		return (0);
	cylinder->orientation.x = ft_atof(orientation_tokens[0]);
	cylinder->orientation.y = ft_atof(orientation_tokens[1]);
	cylinder->orientation.z = ft_atof(orientation_tokens[2]);
	clean_2d_array(orientation_tokens);
	return (normalize_orientation(cylinder));
}
