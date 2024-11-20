/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 17:50:06 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:29:26 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// #include <stdio.h>

// int	iswhitespace(char k)
// {
// 	if (k == '\t' || k == '\n' || k == '\r' || k == '\v'
// 		|| k == '\f' || k == ' ')
// 		return (1);
// 	return (0);
// }

static int	protect_overflow(long calc, int plusmin)
{
	if ((plusmin * calc) > 2147483647)
		return (-1);
	if ((plusmin * calc) < -2147483648)
		return (0);
	return (calc * plusmin);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			neg;
	int long	digits;

	i = 0;
	neg = 1;
	digits = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\r' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == ' ')
		i++;
	if (str[i] == '-')
			neg = -neg;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]) && digits < 2147483647 && digits > -2147483648)
		digits = digits * 10 + str[i++] - '0';
	return (protect_overflow(digits, neg));
}

// OLD VERSION UNOPTIMIZED
//
// int	ft_atoi(const char *str)
// 	int			i;
// 	int			neg;
// 	int long	digits;
// 	int			plusmin;
// 	i = 0;
// 	neg = 1;
// 	digits = 0;
// 	plusmin = 0;
// 	while (iswhitespace(str[i]))
// 		i++;
// 	while (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			neg = -neg;
// 		i++;
// 		plusmin++;
// 		if (plusmin > 1)
// 			return (0);
// 	while (ft_isdigit(str[i]) && digits < 2147483647 && digits > -2147483648)
// 		digits = digits * 10 + str[i++] - '0';
// 	return (protect_overflow(digits, neg));
// 	}
// int	main(void)
// {
// 	printf("%d dit is ft_atoi\n", ft_atoi("99999999999999999999999999"));
// 	printf("%d dit is ft_atoi\n", ft_atoi("-99999999999999999999999999"));
// 	printf("%d dit is ft_atoi\n", ft_atoi("elloh256"));
// 	printf("%d dit is ft_atoi\n", ft_atoi("+-256ffeferg"));
// 	printf("%d dit is ft_atoi\n", ft_atoi("    --------+---+1234ab567"));
// 	printf("%d dit is ft_atoi\n", ft_atoi("\t\v\f\r\n \f-06050"));
// 	printf("\n");
// 	printf("\n");
// 	printf("%d dit is atoi\n", atoi("99999999999999999999999999"));
// 	printf("%d dit is atoi\n", atoi("-99999999999999999999999999"));
// 	printf("%d dit is atoi\n", atoi("hello256"));
// 	printf("%d dit is atoi\n", atoi("+-256"));
// 	printf("%d dit is atoi\n", atoi("    --------+---+1234ab567"));
// 	printf("%d dit is atoi\n", atoi("\t\v\f\r\n \f-06050"));
// 	return (0);
// }