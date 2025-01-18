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
# include "parser.h"
# include "render.h"
# include "../MLX42/include/lodepng/lodepng.h"

# define WIDTH  640
# define HEIGHT 480
# define NUM_THREADS 1
# define EPSILON	1e-6
# define SAMPLES    8

void	render_scene(mlx_t *mlx, t_scene *scene);
int		main(int argc, char **argv);
void	ft_hook(void *param);

#endif // MINIRT_H
