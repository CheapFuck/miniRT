#include "../../includes/minirt.h"


void parse_light(char *line, t_scene *scene) {
    // Split the line into tokens
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_arraylen(tokens) != 4) {
        ft_free_split(tokens);
        exit_with_error("Invalid light format");
    }

    // Parse the position (x, y, z)
    char **pos = ft_split(tokens[1], ',');
    if (!pos || ft_arraylen(pos) != 3) {
        ft_free_split(pos);
        ft_free_split(tokens);
        exit_with_error("Invalid light position format");
    }
    t_light light;
    light.pos.x = ft_atof(pos[0]);
    light.pos.y = ft_atof(pos[1]);
    light.pos.z = ft_atof(pos[2]);
    ft_free_split(pos);

    // Parse the brightness
    light.brightness = ft_atof(tokens[2]);
    if (light.brightness < 0 || light.brightness > 1) {
        ft_free_split(tokens);
        exit_with_error("Light brightness out of range (0 to 1)");
    }

    // Parse the color (R, G, B)
    char **color = ft_split(tokens[3], ',');
    if (!color || ft_arraylen(color) != 3) {
        ft_free_split(color);
        ft_free_split(tokens);
        exit_with_error("Invalid light color format");
    }
    light.color.r = ft_atoi(color[0]);
    light.color.g = ft_atoi(color[1]);
    light.color.b = ft_atoi(color[2]);
    if (light.color.r < 0 || light.color.r > 255 ||
        light.color.g < 0 || light.color.g > 255 ||
        light.color.b < 0 || light.color.b > 255) {
        ft_free_split(color);
        ft_free_split(tokens);
        exit_with_error("Light color out of range (0 to 255)");
    }
    ft_free_split(color);

    // Add the light to the scene
    // printf("num_lights = %d\n", scene->num_lights);
    scene->lights[scene->num_lights] = light;
    scene->num_lights++;
    // printf("Light added, num_lights = %d\n", scene->num_lights);

    // Free the token array
    ft_free_split(tokens);
}

void parse_ambient(char *line, t_scene *scene) {
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_arraylen(tokens) != 3)
        exit_with_error("Invalid ambient light format");

    scene->ambient.ratio = ft_atof(tokens[1]);
    
    char **colors = ft_split(tokens[2], ',');
    if (!colors || ft_arraylen(colors) != 3)
        exit_with_error("Invalid ambient color format");

    scene->ambient.color.r = ft_atoi(colors[0]);
    scene->ambient.color.g = ft_atoi(colors[1]);
    scene->ambient.color.b = ft_atoi(colors[2]);

    ft_free_split(tokens);
    ft_free_split(colors);
}
