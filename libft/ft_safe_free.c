/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_safe_free.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 17:12:00 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 17:12:03 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
