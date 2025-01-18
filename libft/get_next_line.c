/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 13:52:57 by thivan-d      #+#    #+#                 */
/*   Updated: 2024/11/14 14:18:38 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*trim_temp(char *tijdelijk)
{
	char	*newtemp;
	int		j;
	int		i;

	i = 0;
	j = 0;
	while (tijdelijk[i] != '\n')
	{
		if (tijdelijk[i] == '\0')
			return (free(tijdelijk), NULL);
		i++;
	}
	i++;
	newtemp = (char *)(malloc(ft_strlen(&tijdelijk[i]) + 1));
	if (!newtemp)
		return (free(tijdelijk), NULL);
	while (tijdelijk[i] != '\0')
		newtemp[j++] = tijdelijk[i++];
	newtemp[j] = '\0';
	free(tijdelijk);
	return (newtemp);
}

char	*bladiebla(char **temp, char **cut)
{
	int	i;

	i = 0;
	while (temp[0][i] != '\n' && temp[0][i])
		i++;
	if (temp[0][i] == '\n')
		i++;
	*cut = malloc((i + 1) * sizeof(char));
	if (!*cut)
	{
		free(*temp);
		*temp = NULL;
		return (NULL);
	}
	i = -1;
	while (temp[0][++i] != '\n' && temp[0][i] != '\0')
		cut[0][i] = temp[0][i];
	if (temp[0][i] == '\n')
		cut[0][i++] = '\n';
	cut[0][i] = '\0';
	if (!cut || !cut[0])
		return (free(temp), NULL);
	*temp = trim_temp(*temp);
	return (*cut);
}

char	*read_line(int fd, char *temp)
{
	int		bytes;
	char	from[BUFFER_SIZE + 1];

	if (!temp)
		return (NULL);
	bytes = 1;
	from[0] = '\0';
	while (bytes != 0 && !ft_strchr(temp, '\n'))
	{
		bytes = read(fd, from, BUFFER_SIZE);
		if (bytes < 0)
			return (NULL);
		from[bytes] = '\0';
		temp = ft_strjoin(temp, from);
		if (!temp)
			return (NULL);
	}
	if (!temp[0])
		return (free(temp), NULL);
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*temp[OPEN_MAX];
	char		*cut;

	cut = NULL;
	if (!temp[fd])
	{
		temp[fd] = malloc(1 * sizeof(char));
		if (!temp[fd])
			return (free (temp[fd]), NULL);
		if (temp[fd])
			temp[fd][0] = '\0';
	}
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0 || fd > OPEN_MAX)
	{
		free(temp[fd]);
		temp[fd] = NULL;
		return (NULL);
	}
	temp[fd] = read_line(fd, temp[fd]);
	if (!temp[fd])
		return (NULL);
	bladiebla(&temp[fd], &cut);
	if (!cut)
		return (NULL);
	return (cut);
}

/*   special thanks to bmajor, abba, jvan-hal, nsterk, */
/*   lpraca-l, jbouma, evoronin, lpraca-l, jbouma,     */
/*   scristia, pskrucha, cbijman, jmolenaa, cschuijt,  */
/*   evoronin, jovieira and ybakker                    */