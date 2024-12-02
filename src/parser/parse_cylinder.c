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

