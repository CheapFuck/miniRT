#include "../../includes/minirt.h"



void parse_camera(char *line, t_scene *scene) {
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_arraylen(tokens) != 4)
        exit_with_error("Invalid camera format");

    char **pos = ft_split(tokens[1], ',');
    scene->camera.pos.x = ft_atof(pos[0]);
    scene->camera.pos.y = ft_atof(pos[1]);
    scene->camera.pos.z = ft_atof(pos[2]);

    char **orient = ft_split(tokens[2], ',');
    scene->camera.orientation.x = ft_atof(orient[0]);
    scene->camera.orientation.y = ft_atof(orient[1]);
    scene->camera.orientation.z = ft_atof(orient[2]);

    scene->camera.fov = ft_atoi(tokens[3]);

    ft_free_split(tokens);
    ft_free_split(pos);
    ft_free_split(orient);
}
