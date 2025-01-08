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
# include "render.h"
# include "../MLX42/include/lodepng/lodepng.h"

# define WIDTH          1280
# define HEIGHT         1024
# define NUM_THREADS    1

typedef struct s_render_data {
    mlx_t *mlx;
    mlx_image_t *img;
    t_scene *scene;
    int threads_completed;
    int rendering_finished;
    pthread_mutex_t mutex;
    int current_row;
    int render_complete;
    struct timeval start_time;
    struct timeval end_time;
} t_render_data;

typedef struct s_thread_data {
    t_render_data *render_data;
    int start_row;
    int end_row;
    int thread_id;
    int num_threads;
} t_thread_data;

void	render_scene(mlx_t *mlx, t_scene *scene);
void	exit_with_error(const char *msg);
int		main(int argc, char **argv);
void	ft_hook(void *param);

#endif // MINIRT_H
