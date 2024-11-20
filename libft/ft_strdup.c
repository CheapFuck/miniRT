/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 13:11:21 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 14:09:52 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*copy;

	len = ft_strlen(s1) + 1;
	copy = (char *)malloc(len * sizeof(char));
	if (copy == NULL)
		return (NULL);
	ft_memmove(copy, s1, len);
	return (copy);
}

// int main()
// {
//     char source[] = "GeeksForGeeks";
//     // A copy of source is created dynamically
//     // and pointer to copy is returned.
//     char* target = ft_strdup(source);
//     printf("%s", target);
//     return 0;
// }