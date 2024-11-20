/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 13:34:09 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/24 11:54:09 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	char	*newstr;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	if (s == NULL || s_len < start)
		return (ft_strdup(""));
	if (start + len < s_len)
		substr = malloc((len + 1) * sizeof(char));
	else
		substr = malloc((s_len - start + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = start;
	newstr = substr;
	while (i < (start + len) && s[i])
	{
		*newstr = s[i];
		i++;
		newstr++;
	}
	*newstr = '\0';
	return (substr);
}
