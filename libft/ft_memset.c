/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/15 15:32:06 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:18:49 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*i;
	unsigned char	*end;

	i = b;
	end = i + len;
	while (1)
	{
		if (i >= end)
			break ;
		*i = (unsigned char) c;
		i++;
	}
	return (b);
}

//  int main()
//  {
//      char str[] = "almost every programmer should know memset!";
//   ft_memset (str,'-',6);
//   printf("%s \n", str);

//     memset (str,'-',6);
//   printf("%s \n", str);
//   return 0;
//  }