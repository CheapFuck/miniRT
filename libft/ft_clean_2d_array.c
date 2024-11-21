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
