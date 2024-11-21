#ifndef PARSER_H
#define PARSER_H

#include "minirt.h"

// Parsing functions
void parse_file(const char *filename, t_scene *scene);
void parse_ambient(char *line, t_scene *scene);
void parse_camera(char *line, t_scene *scene);
void parse_light(char *line, t_scene *scene);
void parse_sphere(char *line, t_scene *scene);
void parse_plane(char *line, t_scene *scene);
void parse_cylinder(char *line, t_scene *scene);

// Utility functions for parsing
t_vector parse_vector(const char *str);
t_color parse_color(const char *str);
double parse_double(const char *str);
int parse_int(const char *str);

// Error handling
void exit_with_error(const char *msg);
void save_image_to_file(const unsigned char* raw_image_data, unsigned width, unsigned height, const char* filename);

#endif // PARSER_H
