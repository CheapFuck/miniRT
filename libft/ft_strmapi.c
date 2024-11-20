/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strmapi.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 13:12:44 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/15 14:28:26 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*newstring;

	if (!s || !f)
		return (NULL);
	i = 0;
	newstring = (char *)malloc(sizeof(char) * (ft_strlen(s)) + 1);
	if (newstring == NULL)
		return (NULL);
	while (s[i])
	{
		newstring[i] = f(i, s[i]);
		i++;
	}
	newstring[i] = '\0';
	return (newstring);
}
