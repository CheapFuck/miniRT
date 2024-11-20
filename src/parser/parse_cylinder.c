#include "../../includes/minirt.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scene.h"

void parse_cylinder(char *line, t_scene *scene) {
        printf("parse_cylinder called, num_cylinders = %d\n", scene->num_cylinders);
    t_cylinder cylinder;
    int num_parsed = sscanf(line, "cy %lf,%lf,%lf %lf,%lf,%lf %lf %lf %d,%d,%d",
                            &cylinder.center.x, &cylinder.center.y, &cylinder.center.z,
                            &cylinder.orientation.x, &cylinder.orientation.y, &cylinder.orientation.z,
                            &cylinder.diameter, &cylinder.height,
                            &cylinder.color.r, &cylinder.color.g, &cylinder.color.b);

    // Validate if all fields were successfully parsed
    if (num_parsed != 11) {
        fprintf(stderr, "Error: Invalid cylinder format\n");
        return;
    }

    // Validate and normalize orientation vector
    double length = sqrt(pow(cylinder.orientation.x, 2) +
                         pow(cylinder.orientation.y, 2) +
                         pow(cylinder.orientation.z, 2));
    if (length == 0.0) {
        fprintf(stderr, "Error: Cylinder orientation vector cannot be zero\n");
        return;
    }
    cylinder.orientation.x /= length;
    cylinder.orientation.y /= length;
    cylinder.orientation.z /= length;

    // Calculate the radius
    cylinder.radius = cylinder.diameter / 2.0;

    // Check if we have space for more cylinders
    if (scene->num_cylinders >= 65536) {
        fprintf(stderr, "Error: Cylinder array is full\n");
        return;
    }
    // int i;

    
    // Add cylinder to the scene
    
    printf("num_cylinders = %d\n", scene->num_cylinders);
    // scene->num_cylinders++;
    scene->cylinders[scene->num_cylinders] =  cylinder;
    scene->num_cylinders++;
    printf("Cylinder added, num_cylinders = %d\n", scene->num_cylinders);



}




// // Parses a cylinder from the input line and stores it in the scene
// void parse_cylinder(char *line, t_scene *scene) {
//     t_cylinder cylinder;
//     sscanf(line, "cy %lf,%lf,%lf %lf,%lf,%lf %lf %lf %d,%d,%d",
//            &cylinder.center.x, &cylinder.center.y, &cylinder.center.z,
//            &cylinder.orientation.x, &cylinder.orientation.y, &cylinder.orientation.z,
//            &cylinder.diameter, &cylinder.height,
//            &cylinder.color.r, &cylinder.color.g, &cylinder.color.b);
//     cylinder.radius = cylinder.diameter / 2.0;
//     scene->cylinders[scene->num_cylinders++] = cylinder;
// }
