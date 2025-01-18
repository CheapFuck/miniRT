/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/08 11:59:48 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/14 14:48:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	num_strings(char const*string, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (string[i])
	{
		while (string[i] == c)
			i++;
		if (string[i])
		{
			while (string[i] && string[i] != c)
				i++;
			j++;
		}
	}
	return (j);
}

static void	free_array(char **array, size_t i)
{
	while (i > 0)
	{
		i--;
		free(array[i]);
	}
	free(array);
}

static int	fill_array(char const *s, char c, const size_t tel, char **array)
{
	size_t	i;
	size_t	start;
	size_t	end;

	i = 0;
	start = 0;
	end = 0;
	while (i < tel)
	{
		while (s[start] == c)
			start++;
		end = start;
		while (s[end] && s[end] != c)
			end++;
		array[i] = ft_substr(s, start, end - start);
		if (!array[i])
		{
			free_array(array, i);
			return (1);
		}
		i++;
		start = end;
	}
	array[i] = 0;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char			**array;
	const size_t	count = num_strings(s, c);

	array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array)
		return (0);
	if (fill_array(s, c, count, array) == 0)
		return (array);
	return (0);
}
