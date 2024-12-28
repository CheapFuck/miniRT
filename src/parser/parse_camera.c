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
	ft_free_split(tokens);
	ft_free_split(pos);
	ft_free_split(orient);
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

int	validate_color(t_color *color)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		exit_with_error("Error: Color values must be between 0 and 255");
	return (1);
}

int	validate_nrmlzd_vector(t_vector *vector, const char *element_name)
{
	if (vector->x < -1.0 || vector->x > 1.0
		|| vector->y < -1.0 || vector->y > 1.0
		|| vector->z < -1.0 || vector->z > 1.0)
	{
		printf("Error: %s vector components must be between -1 and 1\n",
			element_name);
		exit_with_error("");
	}
	return (1);
}

int	validate_fov(int fov)
{
	if (fov < 0 || fov >= 180)
	{
		return (0);
	}
	return (1);
}

// Function to track unique elements
int	validate_unique_element(t_scene *scene, char type)
{
	if (type == 'A')
	{
		if (scene->has_ambient)
			exit_with_error("Error: Ambient (A) can only be declared once");
		scene->has_ambient = 1;
	}
	else if (type == 'C')
	{
		if (scene->has_camera)
			exit_with_error("Error: Camera (C) can only be declared once");
		scene->has_camera = 1;
	}
	else if (type == 'L')
	{
		if (scene->has_light)
			exit_with_error("Error: Light (L) can only be declared once");
		scene->has_light = 1;
	}
	return (1);
}

int	is_valid_number(const char *str)
{
	int	i;
	int	has_digit;
	int	decimal_points;

	i = 0;
	has_digit = 0;
	decimal_points = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			has_digit = 1;
		else if (str[i] == '.')
		{
			decimal_points++;
			if (decimal_points > 1)
				return (0);
		}
		else
			return (0);
		i++;
	}
	return (i > 0 && has_digit);
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
