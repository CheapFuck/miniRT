/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/19 13:17:40 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/15 14:28:08 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	int		i;

	str = (char *)s;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (str[i] == (char)c)
			return (&str[i]);
		i--;
	}
	if (str[0] == (char)c)
		return (&str[0]);
	return (NULL);
}

// if (i == 0)
// 	return (NULL);
//
// int main () {
// 	const char str[] = "http://www.tutorialspoint.com";
// 	const char ch = 't';
// 	char *ret;
// 	ret = ft_strrchr(str, ch);
//    	printf("%s\n",ft_strrchr(str, ch));
//    	printf("String after |%c| is -. |%s|\n", ch, ret);
//    	return(0);
// }