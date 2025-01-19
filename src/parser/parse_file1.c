/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_file1.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:41 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:55:42 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
