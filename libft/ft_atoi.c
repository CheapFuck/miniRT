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
