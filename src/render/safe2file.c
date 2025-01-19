/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   safe2file.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:53 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:56:54 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	save_image_to_file(const unsigned char *raw_image_data, unsigned width,
	unsigned height, const char *filename)
{
	unsigned char	*png_data;
	size_t			png_size;
	unsigned int	error;

	png_data = NULL;
	png_size = 0;
	error = lodepng_encode32(&png_data, &png_size, raw_image_data, width,
			height);
	if (error)
	{
		printf("Error %u: %s\n", error, lodepng_error_text(error));
		return ;
	}
	error = lodepng_save_file(png_data, png_size, filename);
	if (error)
	{
		printf("Error %u: %s\n", error, lodepng_error_text(error));
	}
	else
	{
		printf("Image saved to %s\n", filename);
	}
	free(png_data);
}
