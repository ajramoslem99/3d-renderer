#include "vector.h"
#include <math.h>

vect3_t vect3RotateX(vect3_t v, float angle)
{
    vect3_t result;
    result.x = v.x;
    result.y = v.y * cos(angle) - v.z * sin(angle);
    result.z = v.y * sin(angle) + v.z * cos(angle);
    return result;
}

vect3_t vect3RotateY(vect3_t v, float angle)
{
    vect3_t result;
    result.x = v.x * cos(angle) + v.z * sin(angle);
    result.y = v.y;
    result.z = -v.x * sin(angle) + v.z * cos(angle);
    return result;
}

vect3_t vect3RotateZ(vect3_t v, float angle)
{
    vect3_t result;
    result.x = v.x * cos(angle) - v.y * sin(angle);
    result.y = v.x * sin(angle) + v.y * cos(angle);
    result.z = v.z;
    return result;
}
