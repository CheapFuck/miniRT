/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:18:57 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:16:31 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	len_d;

	if (dst == NULL && dstsize == 0)
		return (ft_strlen(src));
	i = ft_strlen(dst);
	j = 0;
	len_d = ft_strlen(dst);
	if (dstsize < len_d + 1)
		return (ft_strlen(src) + dstsize);
	while (src[j] && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = '\0';
	if (dstsize < len_d)
		return (ft_strlen(src) + dstsize);
	else
		return (len_d + ft_strlen(src));
}
