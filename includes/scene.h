#ifndef SCENE_H
#define SCENE_H

#include "objects.h" // Include this to access t_sphere and other types



typedef struct s_camera {
    t_vector pos;
    t_vector orientation;
    double fov;
    double fov_scale_x; // Precomputed scale for X
    double fov_scale_y; // Precomputed scale for Y
} t_camera;

typedef struct s_light {
    t_vector pos;
    double brightness;
    t_color color;
    double radius;       // Radius of the area light
} t_light;

typedef struct s_plane {
    t_vector point;
    t_vector normal;
    // t_color color;
    t_material  material;
    // bool checker;
    // double reflectivity;  // 0.0 to 1.0, where 1.0 is perfect mirror
} t_plane;


typedef struct s_cylinder {
    t_vector center;
    t_vector orientation;
    double diameter;
    double height;
    double radius;
    t_material  material;
} t_cylinder;

typedef struct s_disc
{
	double		diameter;
	double		height;
    t_material  material;
	t_vector	orientation;
    t_vector center;    // Center of the disc
    t_vector normal;    // Normalized normal vector of the disc
    double radius;      // Radius of the disc
    t_color color;      // Color of the disc
}	t_disc;



typedef struct s_sphere {
    t_vector center;
    double radius;
    t_material  material;
    // t_color color; // Add this field to store the sphere's color
    // double shininess;
    // bool checker;
    // double reflectivity;  // 0.0 to 1.0, where 1.0 is perfect mirror
} t_sphere;

typedef struct s_scene {
    t_ambient ambient;       // Add this field for ambient lighting
    t_camera camera;
    t_light lights[10];
    t_plane planes[10]; // Add planes here
    t_sphere spheres[10]; // Adjust size as needed
    t_cylinder cylinders[10];
   	t_disc	    discs[20];
    int num_spheres;
    int num_planes;
   	int			num_cylinders;
   	int			num_discs;
    int num_lights;
} t_scene;


typedef struct s_render_data {
    mlx_t *mlx;
    mlx_image_t *img;
    t_scene *scene;
    int threads_completed;
    int rendering_finished;  // Add this flag
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


#endif // SCENE_H
