#include "minirt.h"

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

typedef struct s_material
{
    double transparency;      // 0.0 (opaque) to 1.0 (fully transparent)
    double refractive_index;  // Index of refraction (e.g., 1.0 for air, 1.5 for glass)
    double reflectivity;      // 0.0 (no reflection) to 1.0 (fully reflective)
    t_color color;
    bool checker;
} t_material;

// Add the object type enum
typedef enum e_object_type {
    SPHERE,
    CYLINDER,
    PLANE,
    DISC
} t_object_type;

typedef struct s_hit_record {
    t_vector        point;          // Intersection point
    t_vector        normal;         // Surface normal at the intersection
    double          t;                // Distance from ray origin to intersection
    t_color         color;           // Color of the object hit
    t_material      material;     // Material properties (transparency, refractive index, etc.)
    int             hit;                 // Boolean to indicate if an intersection occurred
    int             index;
    t_object_type   type;
    bool          hit_from_inside;
} t_hit_record;


// Vector operations
t_vector subtract(t_vector a, t_vector b);
double dot(t_vector a, t_vector b);
t_vector normalize(t_vector v);
t_vector multiply_scalar(t_vector v, double scalar);
t_vector add(t_vector a, t_vector b);



#endif // OBJECTS_H
