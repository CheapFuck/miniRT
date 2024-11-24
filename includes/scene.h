#ifndef SCENE_H
#define SCENE_H

#include "objects.h" // Include this to access t_sphere and other types

typedef struct s_camera {
    t_vector pos;
    t_vector orientation;
    double fov;
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
    t_color color;
    t_material material;     // Material properties (transparency, refraction)
    bool checker;
} t_plane;

typedef struct s_hit_record {
    t_vector point;          // Intersection point
    t_vector normal;         // Surface normal at the intersection
    double t;                // Distance from ray origin to intersection
    t_color color;           // Color of the object hit
    t_material material;     // Material properties (transparency, refractive index, etc.)
    int hit;                 // Boolean to indicate if an intersection occurred
} t_hit_record;


typedef struct s_cylinder {
    t_vector center;
    t_vector orientation;
    double diameter;
    double height;
    double radius;
    t_color color;
    bool checker;
    t_material material;     // Material properties (transparency, refraction)
} t_cylinder;

typedef struct s_sphere {
    
    double shininess;
    bool checker;
    t_vector center;
    t_vector position;       // Center of the sphere
    double radius;           // Sphere radius
    t_color color;           // Sphere color
    t_material material;     // Material properties (transparency, refraction)
} t_sphere;

typedef struct s_scene {
    
    t_ambient ambient;       // Add this field for ambient lighting
    t_camera camera;
    t_light lights[10];
    t_plane planes[10]; // Add planes here
    t_sphere *spheres; // Adjust size as needed
    t_cylinder cylinders[10];
    // t_sphere *spheres;       // Array of spheres
    int num_spheres;
    int num_planes;
    int num_cylinders;
    int num_lights;
} t_scene;



#endif // SCENE_H
