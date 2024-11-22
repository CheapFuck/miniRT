#include "../../includes/minirt.h"


// Parses a sphere from the input line and stores it in the scene
void parse_sphere(char *line, t_scene *scene) {
    t_sphere sphere;
    char **tokens = ft_split(line, ' ');
    printf("1pling plong\n");
    if (!tokens || ft_arraylen(tokens) != 5)
        exit_with_error("Invalid sphere format");
    printf("2pling plong\n");
    char **pos = ft_split(tokens[1], ',');
    sphere.center.x = ft_atof(pos[0]);
    sphere.center.y = ft_atof(pos[1]);
    sphere.center.z = ft_atof(pos[2]);
    printf("3pling plong\n");
    sphere.radius = ft_atof(tokens[2]) / 2.0;
    printf("4pling plong\n");
    char **colors = ft_split(tokens[3], ',');
    sphere.color.r = ft_atoi(colors[0]);
    sphere.color.g = ft_atoi(colors[1]);
    sphere.color.b = ft_atoi(colors[2]);
    printf("5pling plong\n");
    sphere.checker = ft_atoi(tokens[4]);
    printf("6pling plong\n");
    // scene->spheres[scene->num_spheres++] = sphere;
    printf("7num_speres = %d\n", scene->num_spheres);
    scene->spheres[scene->num_spheres] = sphere;
    scene->num_spheres++;
    printf("Sphere added, num_spheres = %d\n", scene->num_spheres);

    ft_free_split(tokens);
    ft_free_split(pos);
    ft_free_split(colors);
}