/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcpy.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/15 16:06:48 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 12:43:04 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	if (!dest && !src)
		return (0);
	i = 0;
	d = (char *)dest;
	s = (char *)src;
	while (i != n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

// int main () {
//    const char src[50] = "http://www.tutorialspoint.com";
//    char dest[50];
//    strcpy(dest,"Heloooo!!");
//    printf("Before ft_memcpy dest = %s\n", dest);
//    ft_memcpy(dest, src, strlen(src)+1);
//    printf("After ft_memcpy dest = %s\n", dest);
//    return(0);
// }
