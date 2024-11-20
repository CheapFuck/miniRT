/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/19 13:06:37 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:34:35 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	int		i;

	i = 0;
	str = (char *)s;
	while (str[i] != '\0')
	{
		if (str[i] == (char)c)
			return (&str[i]);
		i++;
	}
	if (str[i] == (char)c)
		return (&str[i]);
	return (NULL);
}
// int main () {
// 	const char str[] = "http://www.tutorialspoint.com";
// 	const char ch = 't';
// 	char *ret;

// 	ret = ft_strchr(str, ch);
// 	printf("%s\n",ft_strchr(str, ch));

// 	printf("String after |%c| is -. |%s|\n", ch, ret);
// 	//system("leaks a.out");
// 	return(0);
// }