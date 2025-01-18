/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 13:13:10 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/24 11:41:37 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	i = 0;
	while (i < len && haystack[i])
	{
		j = 0;
		while ((i + j) < len && haystack[i + j] == needle[j])
		{
			if (!needle[j + 1])
				return ((char *)haystack + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

// int main () {
//    const char haystack[20] = "TutorialsPoint";
//    const char needle[10] = "Point";
//    char *ret;
//    ret = ft_strnstr(haystack, needle, 15);
//    printf("The substring is: %s\n", ret);
//    return(0);
// }