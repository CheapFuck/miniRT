/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:58:01 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 16:58:02 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/time.h>
# include "scene.h"
# include "objects.h"
# include "render.h"
# include "../MLX42/include/lodepng/lodepng.h"

# define WIDTH  640
# define HEIGHT 480
# define NUM_THREADS 1
# define EPSILON	1e-6
# define SAMPLES    8
# define MAX_REFLECTION_DEPTH 16

#endif // MINIRT_H
