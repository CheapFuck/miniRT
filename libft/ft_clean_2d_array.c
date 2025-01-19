/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_clean_2d_array.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 17:12:13 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 17:12:14 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	clean_2d_array(char **arr)
{
	int	i;

	i = 0;
	if (arr != NULL)
	{
		while (arr[i])
		{
			safe_free((void **)&arr[i]);
			i++;
		}
		safe_free((void **)&arr);
	}
}
