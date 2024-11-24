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

typedef struct s_material {
    float transparency; // 0 (opaque) to 1 (fully transparent)
    float refractive_index; // Index of refraction (e.g., 1.0 for air, 1.5 for glass)
    float reflectivity;      // 0.0 (no reflection) to 1.0 (perfect mirror)
} t_material;

typedef struct s_object {
    t_vector position;
    t_color color;
    t_material material;
    // Add other object-specific properties like radius for spheres
} t_object;

// Vector operations
t_vector subtract(t_vector a, t_vector b);
double dot(t_vector a, t_vector b);
t_vector normalize(t_vector v);
t_vector multiply_scalar(t_vector v, double scalar);
t_vector add(t_vector a, t_vector b);

#endif // OBJECTS_H
