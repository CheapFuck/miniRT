/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/19 17:19:43 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:42:19 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i++;
	}
	return (0);
}

// 	int main () {
//    char str1[15];
//    char str2[15];
//    int ret;

//    ft_memcpy(str1, "abcdef", 6);
//    ft_memcpy(str2, "ABCDEF", 6);

//    ret = ft_memcmp(str1, str2, 5);

//    if(ret > 0) {
//       printf("str2 is less than str1");
//    } else if(ret < 0) {
//       printf("str1 is less than str2");
//    } else {
//       printf("str1 is equal to str2");
//    }

//    return(0);
// }