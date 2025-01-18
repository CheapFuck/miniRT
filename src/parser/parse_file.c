#include "../../includes/minirt.h"

static int	fill_buffer(int fd, char *buffer)
{
	int		i;
	char	c;
	int		read_result;

	i = 0;
	read_result = read(fd, &c, 1);
	while (read_result > 0)
	{
		if (c == '\n')
			break ;
		buffer[i++] = c;
		read_result = read(fd, &c, 1);
	}
	buffer[i] = '\0';
	if (read_result < 0)
		return (-1);
	if (read_result == 0 && i == 0)
		return (0);
	return (1);
}

static char	*handle_read_result(char *buffer, int result)
{
	if (result < 0 || result == 0)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

char	*read_line(int fd)
{
	char	*buffer;
	int		result;

	buffer = malloc(10000);
	if (!buffer)
		return (NULL);
	while (1)
	{
		result = fill_buffer(fd, buffer);
		if (result != 1)
			return (handle_read_result(buffer, result));
		if (buffer[0] != '\0')
			return (buffer);
	}
}

void	open_file(const char *filename, int *fd)
{
	*fd = open(filename, O_RDONLY);
	if (*fd < 0)
		exit_with_error("Error opening file");
}

char	*get_next_line_from_file(int fd)
{
	char	*line;

	line = read_line(fd);
	return (line);
}

void	parse_scene_element(char *line, t_scene *scene)
{
	if (ft_strncmp(line, "A ", 2) == 0)
		parse_ambient(line, scene);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_camera(line, scene);
	else if (ft_strncmp(line, "L ", 2) == 0)
		parse_light(line, scene);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		parse_sphere(line, scene);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		parse_plane(line, scene);
	else if (ft_strncmp(line, "cy ", 3) == 0)
	{
		parse_cylinder(line, scene);
		parse_discs(line, scene);
	}
	else if (ft_strncmp(line, "#", 1) != 0)
	{
		printf("line is: %s\n", line);
		exit_with_error("Invalid line in scene file");
	}
}

void	parse_file(const char *filename, t_scene *scene)
{
	int		fd;
	char	*line;

	open_file(filename, &fd);
	line = get_next_line_from_file(fd);
	if (line == NULL)
		exit_with_error("Error opening file");
	while (line != NULL)
	{
		parse_scene_element(line, scene);
		free(line);
		line = get_next_line_from_file(fd);
	}
	free(line);
	close(fd);
}
