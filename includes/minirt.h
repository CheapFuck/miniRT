#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "scene.h"
# include "objects.h"
# include "parser.h"
# include "utils.h"
# include "render.h"


#define WIDTH  1024
#define HEIGHT 768

void    render_scene(mlx_t *mlx, t_scene *scene);
void    exit_with_error(const char *msg);
int     main(int argc, char **argv);
#endif // MINIRT_H
