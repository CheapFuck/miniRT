#include "../../includes/minirt.h"

void parse_cylinder(char *line, t_scene *scene)
{
    t_cylinder cylinder;
    char **tokens = ft_split(line, ' '); // Split the line into tokens based on spaces
    if (!tokens) {
        perror("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Validate the first token (should be "cy")
    if (!tokens[0] || ft_strncmp(tokens[0], "cy", 2) != 0) {
        printf("Error: Invalid cylinder format\n");
        clean_2d_array(tokens);
        return;
    }

    // Parse center coordinates
    char **center_tokens = ft_split(tokens[1], ',');
    if (!center_tokens || !center_tokens[0] || !center_tokens[1] || !center_tokens[2]) {
        printf("Error: Invalid cylinder center format\n");
        clean_2d_array(center_tokens);
        clean_2d_array(tokens);
        return;
    }
    cylinder.center.x = ft_atof(center_tokens[0]);
    cylinder.center.y = ft_atof(center_tokens[1]);
    cylinder.center.z = ft_atof(center_tokens[2]);
    clean_2d_array(center_tokens);

    // Parse orientation vector
    char **orientation_tokens = ft_split(tokens[2], ',');
    if (!orientation_tokens || !orientation_tokens[0] || !orientation_tokens[1] || !orientation_tokens[2]) {
        printf("Error: Invalid cylinder orientation format\n");
        clean_2d_array(orientation_tokens);
        clean_2d_array(tokens);
        return;
    }
    cylinder.orientation.x = ft_atof(orientation_tokens[0]);
    cylinder.orientation.y = ft_atof(orientation_tokens[1]);
    cylinder.orientation.z = ft_atof(orientation_tokens[2]);
    clean_2d_array(orientation_tokens);

    // Normalize orientation vector
    double length = sqrt(pow(cylinder.orientation.x, 2) +
                         pow(cylinder.orientation.y, 2) +
                         pow(cylinder.orientation.z, 2));
    if (length == 0.0) {
        printf("Error: Cylinder orientation vector cannot be zero\n");
        clean_2d_array(tokens);
        return;
    }
    cylinder.orientation.x /= length;
    cylinder.orientation.y /= length;
    cylinder.orientation.z /= length;

    // Parse diameter and height
    if (!tokens[3] || !tokens[4]) {
        printf("Error: Missing cylinder diameter or height\n");
        clean_2d_array(tokens);
        return;
    }
    cylinder.diameter = ft_atof(tokens[3]);
    cylinder.height = ft_atof(tokens[4]);

    // Parse color
    char **color_tokens = ft_split(tokens[5], ',');
    if (!color_tokens || !color_tokens[0] || !color_tokens[1] || !color_tokens[2]) {
        printf("Error: Invalid cylinder color format\n");
        clean_2d_array(color_tokens);
        clean_2d_array(tokens);
        return;
    }
    cylinder.material.color.r = ft_atoi(color_tokens[0]);
    cylinder.material.color.g = ft_atoi(color_tokens[1]);
    cylinder.material.color.b = ft_atoi(color_tokens[2]);
    clean_2d_array(color_tokens);

    cylinder.material.checker = ft_atoi(tokens[6]);
    cylinder.material.reflectivity = ft_atof(tokens[7]);

    cylinder.material.transparency = ft_atof(tokens[8]);
    cylinder.material.refractive_index = ft_atof(tokens[9]);


    // Calculate the radius
    cylinder.radius = cylinder.diameter / 2.0;

    // Check if we can add more cylinders
    if (scene->num_cylinders >= 65536) {
        printf("Error: Cylinder array is full\n");
        clean_2d_array(tokens);
        return;
    }
printf("cylinder\n");
    // Add the cylinder to the scene
    scene->cylinders[scene->num_cylinders] = cylinder;
    scene->num_cylinders++;

    // Clean up
    clean_2d_array(tokens);
}


int normalize_orientation_disc(t_disc *disc)
{
	double length;

	length = sqrt(pow(disc->orientation.x, 2) + pow(disc->orientation.y, 2) + pow(disc->orientation.z, 2));
	if (length == 0.0)
	{
		printf("Error: Cylinder orientation vector cannot be zero\n");
		return (0);
	}
	disc->orientation.x /= length;
	disc->orientation.y /= length;
	disc->orientation.z /= length;
	return (1);
}


void handle_parse_error(char **tokens, const char *error_message)
{
	printf("%s\n", error_message);
	clean_2d_array(tokens);
}

char **split_and_validate(char *str, int expected_parts)
{
	char **tokens;
	int i;

	tokens = ft_split(str, ',');
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < expected_parts)
	{
		if (!tokens[i])
		{
			handle_parse_error(tokens, "Error: Invalid token format");
			return (NULL);
		}
		i++;
	}
	return (tokens);
}

int parse_disc_properties(char **tokens, t_disc *disc)
{
	char **center_tokens;
	char **orientation_tokens;

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

int parse_color(char *color_str, t_color *color)
{
	char **color_tokens;

	color_tokens = split_and_validate(color_str, 3);
	if (!color_tokens)
		return (0);
	color->r = ft_atoi(color_tokens[0]);
	color->g = ft_atoi(color_tokens[1]);
	color->b = ft_atoi(color_tokens[2]);
	clean_2d_array(color_tokens);
	return (1);
}


void parse_discs(char *line, t_scene *scene)
{
    t_disc disc;

    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_arraylen(tokens) < 6) {
        fprintf(stderr, "Error: Invalid cylinder format for discs\n");
        ft_free_split(tokens);
        return;
    }
    if (!parse_disc_properties(tokens, &disc)) {
        fprintf(stderr, "Error: Failed to parse cylinder properties\n");
        ft_free_split(tokens);
        return;
    }
    disc.material.checker = ft_atoi(tokens[6]);
    disc.material.reflectivity = ft_atof(tokens[7]);

    disc.material.transparency = ft_atof(tokens[8]);
    disc.material.refractive_index = ft_atof(tokens[9]);

    disc.diameter = ft_atof(tokens[3]);
    disc.radius = disc.diameter / 2.0;
    disc.height = ft_atof(tokens[4]);
    if (!parse_color(tokens[5], &disc.color))
		return (handle_parse_error(tokens,
				"Error: Invalid disc color format"));
    t_disc bottom_disc;
    t_vector bottom_offset = multiply_scalar(disc.orientation, -0.5 * disc.height);
    bottom_disc.center = add(disc.center, bottom_offset);
    bottom_disc.normal = disc.orientation;
    bottom_disc.radius = disc.radius;
    bottom_disc.color = disc.color;
    t_disc top_disc;
    t_vector top_offset = multiply_scalar(disc.orientation, 0.5 * disc.height);
    top_disc.center = add(disc.center, top_offset);
    top_disc.normal = disc.orientation;
    top_disc.radius = disc.radius;
    top_disc.color = disc.color;
    if (scene->num_discs + 2 >= 65535) {
        fprintf(stderr, "Error: Maximum number of discs exceeded\n");
        ft_free_split(tokens);
        return;
    }
    scene->discs[scene->num_discs++] = bottom_disc;
    scene->discs[scene->num_discs++] = top_disc;

    printf("Parsed two discs: bottom at (%f, %f, %f), top at (%f, %f, %f)\n",
           bottom_disc.center.x, bottom_disc.center.y, bottom_disc.center.z,
           top_disc.center.x, top_disc.center.y, top_disc.center.z);

    ft_free_split(tokens);
}


