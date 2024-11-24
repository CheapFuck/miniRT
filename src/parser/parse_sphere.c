#include "../../includes/minirt.h"


// Parses a sphere from the input line and stores it in the scene
// void parse_sphere(char *line, t_scene *scene)
// {
//     t_sphere sphere;
//     char **tokens = ft_split(line, ' ');
//     printf("1pling plong\n");
//     if (!tokens || ft_arraylen(tokens) != 5)
//         exit_with_error("Invalid sphere format");
//     printf("2pling plong\n");
//     char **pos = ft_split(tokens[1], ',');
//     sphere.center.x = ft_atof(pos[0]);
//     sphere.center.y = ft_atof(pos[1]);
//     sphere.center.z = ft_atof(pos[2]);
//     printf("3pling plong\n");
//     sphere.radius = ft_atof(tokens[2]) / 2.0;
//     printf("4pling plong\n");
//     char **colors = ft_split(tokens[3], ',');
//     sphere.color.r = ft_atoi(colors[0]);
//     sphere.color.g = ft_atoi(colors[1]);
//     sphere.color.b = ft_atoi(colors[2]);
//     printf("5pling plong\n");
//     sphere.checker = ft_atoi(tokens[4]);
//     printf("6pling plong\n");
//     // scene->spheres[scene->num_spheres++] = sphere;
//     printf("7num_speres = %d\n", scene->num_spheres);
//     scene->spheres[scene->num_spheres] = sphere;
//     scene->num_spheres++;
//     printf("Sphere added, num_spheres = %d\n", scene->num_spheres);

//     ft_free_split(tokens);
//     ft_free_split(pos);
//     ft_free_split(colors);
// }

// void parse_sphere(char *line, t_scene *scene)
// {
//     t_sphere sphere;
//     char **tokens = ft_split(line, ' ');

//     if (ft_arraylen(tokens) != 9) {
//         fprintf(stderr, "Error: Invalid sphere format\n");
//         ft_free_split(tokens);
//         exit(1);
//     }


//     // Parse position
//     char **position = ft_split(tokens[1], ',');
//     if (ft_arraylen(position) != 3) {
//         fprintf(stderr, "Error: Invalid sphere position\n");
//         ft_free_split(position);
//         ft_free_split(tokens);
//         exit(1);
//     }
//     sphere.position.x = ft_atof(position[0]);
//     sphere.position.y = ft_atof(position[1]);
//     sphere.position.z = ft_atof(position[2]);
//     ft_free_split(position);

//     // Parse radius
//     sphere.radius = ft_atof(tokens[2]);

//     // Parse color
//     char **color = ft_split(tokens[3], ',');
//     if (ft_arraylen(color) != 3) {
//         fprintf(stderr, "Error: Invalid sphere color\n");
//         ft_free_split(color);
//         ft_free_split(tokens);
//         exit(1);
//     }
//     sphere.color.r = ft_atoi(color[0]);
//     sphere.color.g = ft_atoi(color[1]);
//     sphere.color.b = ft_atoi(color[2]);
//     ft_free_split(color);

//     // Parse transparency
//     sphere.material.transparency = ft_atof(tokens[4]);

//     // Parse refractive index
//     sphere.material.refractive_index = ft_atof(tokens[5]);

//     // Validate transparency and refractive index
//     if (sphere.material.transparency < 0.0 || sphere.material.transparency > 1.0) {
//         fprintf(stderr, "Error: Transparency must be between 0.0 and 1.0\n");
//         ft_free_split(tokens);
//         exit(1);
//     }
//     if (sphere.material.refractive_index < 1.0) {
//         fprintf(stderr, "Error: Refractive index must be >= 1.0\n");
//         ft_free_split(tokens);
//         exit(1);
//     }

//     // Add the sphere to the scene
//     scene->objects[scene->object_count++] = sphere;

//     ft_free_split(tokens);
// }


void parse_sphere(char *line, t_scene *scene) {
    char **tokens = ft_split(line, ' ');
    if (ft_arraylen(tokens) != 7) {
        fprintf(stderr, "Error: Invalid sphere format\n");
        ft_free_split(tokens);
        exit(1);
    }

    t_sphere sphere;
    

    // Parse position
    char **position = ft_split(tokens[1], ',');
    sphere.position.x = ft_atof(position[0]);
    sphere.position.y = ft_atof(position[1]);
    sphere.position.z = ft_atof(position[2]);
    ft_free_split(position);

    // Parse radius
    sphere.radius = ft_atof(tokens[2]);

    // Parse color
    char **color = ft_split(tokens[3], ',');
    sphere.color.r = ft_atoi(color[0]);
    sphere.color.g = ft_atoi(color[1]);
    sphere.color.b = ft_atoi(color[2]);
    ft_free_split(color);
    
    // Parse reflectivity
    sphere.material.reflectivity = ft_atof(tokens[6]);  // Assuming reflectivity is in position 6
    // Validate reflectivity
    if (sphere.material.reflectivity < 0.0 || sphere.material.reflectivity > 1.0) {
        fprintf(stderr, "Error: Reflectivity must be between 0.0 and 1.0\n");
        ft_free_split(tokens);
        exit(1);
    }
    // Parse material
    sphere.material.transparency = ft_atof(tokens[4]);
    sphere.material.refractive_index = ft_atof(tokens[5]);

    // Validate transparency and refractive index
    if (sphere.material.transparency < 0.0 || sphere.material.transparency > 1.0) {
        fprintf(stderr, "Error: Transparency must be between 0.0 and 1.0\n");
        ft_free_split(tokens);
        exit(1);
    }
    if (sphere.material.refractive_index < 1.0) {
        fprintf(stderr, "Error: Refractive index must be >= 1.0\n");
        ft_free_split(tokens);
        exit(1);
    }

    // Add the sphere to the spheres array in the scene
    scene->spheres[scene->num_spheres++] = sphere;

    ft_free_split(tokens);
}
