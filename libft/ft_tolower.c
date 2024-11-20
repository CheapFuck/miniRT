/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_tolower.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:24:05 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:21:49 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		c = c + 32;
	return (c);
}

// int main ()
// {
//     char a;
//     char b;
//     a = 'Z';

//     printf("converted to lowercase = %i \n",ft_tolower(a));
//     a = ft_tolower(a);

//     b = ft_tolower(a);
//      printf("%c\n", a);
//     printf("%c\n", b);

// }