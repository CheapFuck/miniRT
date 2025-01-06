#include "../../includes/minirt.h"

// Parses a plane from the input line and stores it in the scene

// void parse_plane(char *line, t_scene *scene) {
//     t_plane plane;
//     char **tokens = ft_split(line, ' '); // Split the line into tokens
//     if (!tokens) {
//         perror("Error: Memory allocation failed");
//         exit(EXIT_FAILURE);
//     }

//     // Validate the first token (should be "pl")
//     if (!tokens[0] || ft_strncmp(tokens[0], "pl", 2) != 0) {
//         printf("Error: Invalid plane format\n");
//         clean_2d_array(tokens);
//         return;
//     }

//     // Parse point coordinates
//     char **point_tokens = ft_split(tokens[1], ',');
//     if (!point_tokens || !point_tokens[0] || !point_tokens[1] || !point_tokens[2]) {
//         printf("Error: Invalid plane point format\n");
//         clean_2d_array(point_tokens);
//         clean_2d_array(tokens);
//         return;
//     }
//     plane.point.x = ft_atof(point_tokens[0]);
//     plane.point.y = ft_atof(point_tokens[1]);
//     plane.point.z = ft_atof(point_tokens[2]);
//     clean_2d_array(point_tokens);

//     // Parse normal vector
//     char **normal_tokens = ft_split(tokens[2], ',');
//     if (!normal_tokens || !normal_tokens[0] || !normal_tokens[1] || !normal_tokens[2]) {
//         printf("Error: Invalid plane normal format\n");
//         clean_2d_array(normal_tokens);
//         clean_2d_array(tokens);
//         return;
//     }
//     plane.normal.x = ft_atof(normal_tokens[0]);
//     plane.normal.y = ft_atof(normal_tokens[1]);
//     plane.normal.z = ft_atof(normal_tokens[2]);
//     clean_2d_array(normal_tokens);

//     // Normalize the normal vector
//     double length = sqrt(pow(plane.normal.x, 2) +
//                          pow(plane.normal.y, 2) +
//                          pow(plane.normal.z, 2));
//     if (length == 0.0) {
//         printf("Error: Plane normal vector cannot be zero\n");
//         clean_2d_array(tokens);
//         return;
//     }
//     plane.normal.x /= length;
//     plane.normal.y /= length;
//     plane.normal.z /= length;

//     // Parse color
//     char **color_tokens = ft_split(tokens[3], ',');
//     if (!color_tokens || !color_tokens[0] || !color_tokens[1] || !color_tokens[2]) {
//         printf("Error: Invalid plane color format\n");
//         clean_2d_array(color_tokens);
//         clean_2d_array(tokens);
//         return;
//     }
//     plane.material.color.r = ft_atoi(color_tokens[0]);
//     plane.material.color.g = ft_atoi(color_tokens[1]);
//     plane.material.color.b = ft_atoi(color_tokens[2]);
//     clean_2d_array(color_tokens);

//     // Parse checker and reflectivity
//     plane.material.checker = ft_atoi(tokens[4]);
//     plane.material.reflectivity = ft_atof(tokens[5]);
// printf("plane\n");
//     // Add the plane to the scene
//     if (scene->num_planes >= 65536) {
//         printf("Error: Plane array is full\n");
//         clean_2d_array(tokens);
//         return;
//     }
//     scene->planes[scene->num_planes] = plane;
//     scene->num_planes++;

//     // Clean up
//     clean_2d_array(tokens);
// }


void	handle_plane_parse_error(char **tokens, char **split_tokens,
	const char *error_message)
{
	ft_free_split(split_tokens);
	ft_free_split(tokens);
	exit_with_error(error_message);
}

int	parse_plane_tokens(char *token, t_vector *vector)
{
	char	**vector_tokens;

	vector_tokens = split_and_validate(token, 3);
	if (!vector_tokens)
		return (0);
	vector->x = ft_atof(vector_tokens[0]);
	vector->y = ft_atof(vector_tokens[1]);
	vector->z = ft_atof(vector_tokens[2]);
	ft_free_split(vector_tokens);
	return (1);
}

int	parse_plane_vector(char *token, t_vector *normal, char **tokens)
{
	double	length;

	if (!parse_plane_tokens(token, normal))
		handle_plane_parse_error(tokens, NULL, "Invalid plane normal format");
	length = sqrt(pow(normal->x, 2) + pow(normal->y, 2) + pow(normal->z, 2));
	if (length == 0.0)
		handle_plane_parse_error(tokens, NULL,
			"Plane normal vector cannot be zero");
	normal->x /= length;
	normal->y /= length;
	normal->z /= length;
	return (1);
}

int	parse_plane_color(char *token, t_color *color, char **tokens)
{
	char	**color_tokens;

	color_tokens = split_and_validate(token, 3);
	if (!color_tokens)
		handle_plane_parse_error(tokens, color_tokens,
			"Invalid plane color format");
	color->r = ft_atoi(color_tokens[0]);
	color->g = ft_atoi(color_tokens[1]);
	color->b = ft_atoi(color_tokens[2]);
	ft_free_split(color_tokens);
	if (color->r < 0 || color->r > 255 || color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		handle_plane_parse_error(tokens, NULL,
			"Plane color out of range (0 to 255)");
	return (1);
}

void	parse_plane(char *line, t_scene *scene)
{
	char	**tokens;
	t_plane	plane;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 6)
		handle_plane_parse_error(tokens, NULL, "Invalid plane format");
	if (!parse_plane_tokens(tokens[1], &plane.point))
		handle_plane_parse_error(tokens, NULL, "Invalid plane point format");
	if (!parse_plane_tokens(tokens[2], &plane.normal))
		handle_plane_parse_error(tokens, NULL, "Invalid plane normal format");
	if (!validate_nrmlzd_vector(&plane.normal, "Plane normal"))
		return (ft_free_split(tokens));
	if (!parse_plane_vector(tokens[2], &plane.normal, tokens))
		return ;
	if (!parse_plane_color(tokens[3], &plane.material.color, tokens))
		return ;
	if (!validate_color(&plane.material.color))
		return (ft_free_split(tokens));
    // Parse checker and reflectivity
    plane.material.checker = ft_atoi(tokens[4]);
    plane.material.reflectivity = ft_atof(tokens[5]);
	if (scene->num_planes >= 65536)
		handle_plane_parse_error(tokens, NULL, "Plane array is full");
	scene->planes[scene->num_planes] = plane;
	scene->num_planes++;
	ft_free_split(tokens);
}
