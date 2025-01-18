#include "libft.h"

int	ft_arraylen(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}
