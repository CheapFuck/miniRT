#include "../../includes/minirt.h"

// Parses a plane from the input line and stores it in the scene

void parse_plane(char *line, t_scene *scene) {
    t_plane plane;
    char **tokens = ft_split(line, ' '); // Split the line into tokens
    if (!tokens) {
        perror("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Validate the first token (should be "pl")
    if (!tokens[0] || ft_strncmp(tokens[0], "pl", 2) != 0) {
        printf("Error: Invalid plane format\n");
        clean_2d_array(tokens);
        return;
    }

    // Parse point coordinates
    char **point_tokens = ft_split(tokens[1], ',');
    if (!point_tokens || !point_tokens[0] || !point_tokens[1] || !point_tokens[2]) {
        printf("Error: Invalid plane point format\n");
        clean_2d_array(point_tokens);
        clean_2d_array(tokens);
        return;
    }
    plane.point.x = ft_atof(point_tokens[0]);
    plane.point.y = ft_atof(point_tokens[1]);
    plane.point.z = ft_atof(point_tokens[2]);
    clean_2d_array(point_tokens);

    // Parse normal vector
    char **normal_tokens = ft_split(tokens[2], ',');
    if (!normal_tokens || !normal_tokens[0] || !normal_tokens[1] || !normal_tokens[2]) {
        printf("Error: Invalid plane normal format\n");
        clean_2d_array(normal_tokens);
        clean_2d_array(tokens);
        return;
    }
    plane.normal.x = ft_atof(normal_tokens[0]);
    plane.normal.y = ft_atof(normal_tokens[1]);
    plane.normal.z = ft_atof(normal_tokens[2]);
    clean_2d_array(normal_tokens);

    // Normalize the normal vector
    double length = sqrt(pow(plane.normal.x, 2) +
                         pow(plane.normal.y, 2) +
                         pow(plane.normal.z, 2));
    if (length == 0.0) {
        printf("Error: Plane normal vector cannot be zero\n");
        clean_2d_array(tokens);
        return;
    }
    plane.normal.x /= length;
    plane.normal.y /= length;
    plane.normal.z /= length;

    // Parse color
    char **color_tokens = ft_split(tokens[3], ',');
    if (!color_tokens || !color_tokens[0] || !color_tokens[1] || !color_tokens[2]) {
        printf("Error: Invalid plane color format\n");
        clean_2d_array(color_tokens);
        clean_2d_array(tokens);
        return;
    }
    plane.material.color.r = ft_atoi(color_tokens[0]);
    plane.material.color.g = ft_atoi(color_tokens[1]);
    plane.material.color.b = ft_atoi(color_tokens[2]);
    clean_2d_array(color_tokens);

    // Parse checker and reflectivity
    plane.material.checker = ft_atoi(tokens[4]);
    plane.material.reflectivity = ft_atof(tokens[5]);

    // Add the plane to the scene
    if (scene->num_planes >= 65536) {
        printf("Error: Plane array is full\n");
        clean_2d_array(tokens);
        return;
    }
    scene->planes[scene->num_planes] = plane;
    scene->num_planes++;

    // Clean up
    clean_2d_array(tokens);
}