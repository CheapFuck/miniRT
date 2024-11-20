/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/19 16:55:19 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:37:59 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ch;
	unsigned char	k;

	ch = (unsigned char *)s;
	k = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (*ch == k)
			return (ch);
		ch++;
		i++;
	}
	return (NULL);
}
