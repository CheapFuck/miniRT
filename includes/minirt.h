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
# include "../MLX42/include/lodepng/lodepng.h"


#define WIDTH  640
#define HEIGHT 480
#define NUM_THREADS 12

void    render_scene(mlx_t *mlx, t_scene *scene);
void    exit_with_error(const char *msg);
int     main(int argc, char **argv);


// Add the object type enum
typedef enum e_object_type {
    SPHERE,
    CYLINDER,
    PLANE
} t_object_type;


#endif // MINIRT_H

