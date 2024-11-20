/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/27 12:00:33 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/15 14:13:56 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	ft_digit_count(long int i)
{
	int	tel;

	tel = 0;
	if (i == 0)
	{
		tel++;
		return (tel);
	}
	if (i < 0)
	{
		i = i * -1;
		tel++;
	}
	while (i > 0)
	{
		i = i / 10;
		tel++;
	}
	return (tel);
}

char	*prrrt(long int n, int i, char *str)
{
	while (n > 0)
	{
		i--;
		str[i] = 48 + (n % 10);
		n = n / 10;
	}
	return (&*str);
}

char	*ft_itoa(int nb)
{
	char	*str;
	long	n;
	int		i;

	n = nb;
	i = ft_digit_count(n);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (0);
	str[i] = '\0';
	if (n == 0)
	{
		str[0] = 48;
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n = n * -1;
	}
	return (prrrt(n, i, str));
}

// #include <stdio.h>
// int		main(void)
// {
// 	char *i;
// 	i = ft_itoa(12345);
// 	printf("\n aantal cijfers: %d\n", ft_digit_count(12345));
// 	printf("\n%s\n", i);
// 	//printf("\n%s\n", ft_itoa(10050003));
// 	//printf("\n%s\n", ft_itoa(0));
// 	//system("leaks a.out");
// }
