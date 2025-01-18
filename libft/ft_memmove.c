/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:16:01 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 12:34:47 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*s;
	char	*d;

	s = (char *)src;
	d = (char *)dst;
	if (d > s)
	{
		while (len--)
			d[len] = s[len];
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}

// int main()
// {
// const char src[] = "http://www.codam.nl";
//    char dest[] = "Heloooo!!        !";
//    printf("Before ft_memcpy dest = %s\n", dest);
//    ft_memmove(dest, src, ft_strlen(src)+1);
//    printf("After memcpy dest = %s\n", dest);
//    return(0);
// }
