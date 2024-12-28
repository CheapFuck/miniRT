#include "../includes/minirt.h"

void exit_with_error(const char *msg)
{
    if (msg)
        fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

t_vector add(t_vector a, t_vector b)
{
    t_vector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

t_vector subtract(t_vector a, t_vector b)
{
    t_vector result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

t_vector multiply_scalar(t_vector v, double scalar)
{
    t_vector result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

double dot(t_vector a, t_vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vector normalize(t_vector v)
{
    double length;
    t_vector result;

    // Check if any component of the vector is NaN or invalid
    if (isnan(v.x) || isnan(v.y) || isnan(v.z)) {
        // Handle the invalid case
        result.x = 0;
        result.y = 0;
        result.z = 0;
        return result;
    }
    // Calculate the length of the vector
    length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    // Check for zero-length vector
    if (length == 0) {
        // Handle zero-length vector
        result.x = 0;
        result.y = 0;
        result.z = 0;
        return result;
    }
    // Proceed with normalization
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
    return result;
}

double length_squared(t_vector v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}