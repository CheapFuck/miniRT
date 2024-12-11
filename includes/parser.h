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
int parse_disc_properties(char **tokens, t_disc *disc);
char **split_and_validate(char *str, int expected_parts);
void handle_parse_error(char **tokens, const char *error_message);
int normalize_orientation_disc(t_disc *disc);
int parse_color(char *color_str, t_color *color);



// Utility functions for parsing
t_vector parse_vector(const char *str);
// t_color parse_color(const char *str);
double parse_double(const char *str);
int parse_int(const char *str);

// Error handling
void exit_with_error(const char *msg);
void save_image_to_file(const unsigned char* raw_image_data, unsigned width, unsigned height, const char* filename);

#endif // PARSER_H
