#include "../../includes/minirt.h"

// Parses a plane from the input line and stores it in the scene
void parse_plane(char *line, t_scene *scene) {
    t_plane plane;
    sscanf(line, "pl %lf,%lf,%lf %lf,%lf,%lf %d,%d,%d",
           &plane.point.x, &plane.point.y, &plane.point.z,
           &plane.normal.x, &plane.normal.y, &plane.normal.z,
           &plane.color.r, &plane.color.g, &plane.color.b);
    scene->planes[scene->num_planes++] = plane;
}
