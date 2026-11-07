#include "vector.h"
#include <math.h>

/*=================================== 2d Vectors =================================*/
// implementations
/*=================================================================================*/
float vect2_length(vect2_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

vect2_t vect2_add(vect2_t v1, vect2_t v2)
{
    vect2_t result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

vect2_t vect2_subtract(vect2_t v1, vect2_t v2)
{
    vect2_t result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

vect2_t vect2_mul(vect2_t v, float scalar)
{
    vect2_t result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

vect2_t vect2_divide(vect2_t v, float scalar)
{
    vect2_t result;
    result.x = v.x / scalar;
    result.y = v.y / scalar;
    return result;
}

float vect2_dot(vect2_t v1, vect2_t v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

void vect2_normalize(vect2_t *v)
{
    float length = vect2_length(*v);
    v->x /= length;
    v->y /= length;
}

/*=================================== 3d Vectors =================================*/
// implementations
/*=================================================================================*/

float vect3_length(vect3_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vect3_t vect3_add(vect3_t v1, vect3_t v2)
{
    vect3_t result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

vect3_t vect3_subtract(vect3_t v1, vect3_t v2)
{
    vect3_t result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

vect3_t vect3_mul(vect3_t v, float scalar)
{
    vect3_t result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

float vect3_dot(vect3_t v1, vect3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vect3_t vect3_divide(vect3_t v, float scalar)
{
    vect3_t result;
    result.x = v.x / scalar;
    result.y = v.y / scalar;
    result.z = v.z / scalar;
    return result;
}

vect3_t vect3_cross(vect3_t v1, vect3_t v2)
{
    vect3_t result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

void vect3_normalize(vect3_t *v)
{

    float length = vect3_length(*v);

    v->x /= length;
    v->y /= length;
    v->z /= length;
}
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
