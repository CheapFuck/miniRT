/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/17 16:20:28 by thivan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:28:20 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	unsigned char	c1;
	unsigned char	c2;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < n -1)
		i++;
	c1 = s1[i];
	c2 = s2[i];
	return (c1 - c2);
}

// int main () {
//    char str1[20];
//    char str2[20];
//    int result;

//    strcpy(str1, "bbb");
//    strcpy(str2, "aaa");

//    //This will compare the first 4 characters
//    result = strncmp(str1, str2, 4);

//    if(result > 0) {
//printf("ASCII value of first unmatched character
// of str1 is greater than str2");
//    } else if(result < 0) {
//printf("ASCII value of first unmatched character of str1 is less than str2");
//    } else {
//       printf("Both the strings str1 and str2 are equal");
//    }

//    return 0;
// }