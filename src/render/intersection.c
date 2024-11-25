#include "../../includes/minirt.h"

// Check if a ray intersects a sphere
int intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
    t_vector oc = subtract(ray->origin, sphere->center);
    double a = dot(ray->direction, ray->direction);
    double b = 2.0 * dot(oc, ray->direction);
    double c = dot(oc, oc) - (sphere->radius * sphere->radius);
    double discriminant = b * b - 4 * a * c;
    // If the discriminant is negative, there's no intersection
    if (discriminant < 0)
        return 0;

    // Calculate the two possible intersection distances
    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    // Find the nearest positive intersection
    if (t1 > 0 && t2 > 0)
        *t = fmin(t1, t2);
    else if (t1 > 0)
        *t = t1;
    else if (t2 > 0)
        *t = t2;
    else
        return 0;
    // printf("a: %f, b: %f, c: %f, discriminant: %f\n", a, b, c, discriminant);
    // printf("Ray: Origin (%f, %f, %f), Direction (%f, %f, %f)\n", 
    // ray->origin.x, ray->origin.y, ray->origin.z, 
    //    ray->direction.x, ray->direction.y, ray->direction.z);

    // printf("Sphere: Center (%f, %f, %f), Radius %f\n", sphere->center.x, sphere->center.y, sphere->center.z, sphere->radius);

    // Ensure that the intersection is in front of the camera
    return (*t > 0);
}

int intersect_plane(t_ray *ray, t_plane *plane, double *t) {
    // Calculate the dot product of the plane normal and the ray direction
    double denom = dot(plane->normal, ray->direction);

    // If denom is close to 0, the ray is parallel to the plane
    if (fabs(denom) < 1e-6)
        return 0;

    // Calculate t (intersection distance)
    t_vector p0_to_origin = subtract(plane->point, ray->origin);
    double t_temp = dot(p0_to_origin, plane->normal) / denom;

    // If t is negative, the intersection is behind the ray origin
    if (t_temp < 0)
        return 0;

    *t = t_temp;
    return 1; // Intersection found
}


int intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
    // Step 1: Calculate the vector from the ray origin to the cylinder center
    t_vector oc = subtract(ray->origin, cylinder->center);

    // Step 2: Project ray direction and oc onto the cylinder's axis
    t_vector axis = normalize(cylinder->orientation);
    double dot_dir_axis = dot(ray->direction, axis);
    double dot_oc_axis = dot(oc, axis);

    // Step 3: Calculate components for the quadratic equation
    t_vector d = subtract(ray->direction, multiply_scalar(axis, dot_dir_axis));
    t_vector o = subtract(oc, multiply_scalar(axis, dot_oc_axis));

    double a = dot(d, d);
    double b = 2.0 * dot(d, o);
    double c = dot(o, o) - (cylinder->radius * cylinder->radius);
    double discriminant = b * b - 4 * a * c;

    // If the discriminant is negative, there's no intersection
    if (discriminant < 0)
        return 0;

    // Calculate the two possible intersection distances
    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    // Step 4: Check if the intersection points are within the cylinder's height
    t_vector p1 = add(ray->origin, multiply_scalar(ray->direction, t1));
    double height1 = dot(subtract(p1, cylinder->center), axis);

    t_vector p2 = add(ray->origin, multiply_scalar(ray->direction, t2));
    double height2 = dot(subtract(p2, cylinder->center), axis);

    int hit = 0;

    if (t1 > 0 && fabs(height1) <= cylinder->height / 2) {
        *t = t1;
        hit = 1;
    }
    if (t2 > 0 && fabs(height2) <= cylinder->height / 2 && (!hit || t2 < *t)) {
        *t = t2;
        hit = 1;
    }

    return hit;
}
