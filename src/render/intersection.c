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
      // Ensure that the intersection is in front of the camera
    return (*t > 0);
}

int intersect_plane(t_ray *ray, t_plane *plane, double *t)
{
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


t_vector	scale_vector(t_vector v, double s)
{
	t_vector result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}
int intersect_disc(t_ray *ray, t_disc *disc, double *t)
{
    t_vector oc;             // Vector from ray origin to disc center
    double denom;            // Dot product of ray direction and disc normal
    double t_plane;          // Distance to intersection point on plane
    t_vector p;              // Intersection point
    double distance_squared; // Squared distance from center to intersection point

    // Step 1: Calculate the denominator (ray direction · disc normal)
    denom = dot(ray->direction, disc->normal);
    if (fabs(denom) < 1e-6) // Avoid division by near-zero; ray is parallel to disc
        return (0);

    // Step 2: Find intersection distance to the plane containing the disc
    oc = subtract(disc->center, ray->origin); // Vector from ray origin to disc center
    t_plane = dot(oc, disc->normal) / denom;

    if (t_plane <= 0) // Intersection is behind the ray's origin
        return (0);

    // Step 3: Check if the intersection point is within the disc's radius
    p = add(ray->origin, scale_vector(ray->direction, t_plane)); // Intersection point
    distance_squared = length_squared(subtract(p, disc->center));
    if (distance_squared > disc->radius * disc->radius)
        return (0); // Intersection is outside the disc

    // Valid intersection
    *t = t_plane;
    return (1);
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

    if (t1 > 0 && fabs(height1) <= cylinder->height / 2)
    {
        *t = t1;
        hit = 1;
    }
    if (t2 > 0 && fabs(height2) <= cylinder->height / 2 && (!hit || t2 < *t))
    {
        *t = t2;
        hit = 1;
    }

    return hit;
}
