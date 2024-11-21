#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct s_vector {
    double x, y, z;
} t_vector;

typedef struct s_color {
    int r;
    int g;
    int b;
} t_color;

typedef struct s_ambient {
    double ratio;
    t_color color;
} t_ambient;


typedef struct s_ray {
    t_vector origin;
    t_vector direction;
} t_ray;

typedef struct s_sphere {
    t_vector center;
    double radius;
    t_color color; // Add this field to store the sphere's color
    double shininess;
} t_sphere;

// Vector operations
t_vector subtract(t_vector a, t_vector b);
double dot(t_vector a, t_vector b);
t_vector normalize(t_vector v);
t_vector multiply_scalar(t_vector v, double scalar);
t_vector add(t_vector a, t_vector b);

#endif // OBJECTS_H
