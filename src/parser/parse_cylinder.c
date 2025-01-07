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

static void	parse(char *error, char **tokens)
{
	printf("%s\n", error);
	ft_free_split(tokens);
	exit(EXIT_FAILURE);
}

static void	parse_cylinder_atributes(t_cylinder *cylinder, char		**tokens)
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
	if (!parse_color(tokens[5], &cylinder.material.color))
		return (ft_free_split(tokens));
	if (!validate_color(&cylinder.material.color))
		return (ft_free_split(tokens));
	parse_cylinder_atributes(&cylinder, tokens);
	if (scene->num_cylinders >= 65536)
		return (parse("Error: Maximum number of cylinders exceeded", tokens));
	scene->cylinders[scene->num_cylinders++] = cylinder;
	ft_free_split(tokens);
}

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
