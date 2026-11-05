#ifndef VECTOR_H
#define VECTOR_H

/*=================================== 2d Vectors =================================*/
typedef struct
{

    float x;
    float y;

} vect2_t;

float vect2_length(vect2_t v);
vect2_t vect2_add(vect2_t v1, vect2_t v2);
vect2_t vect2_subtract(vect2_t v1, vect2_t v2);
vect2_t vect2_mul(vect2_t v, float scalar);
vect2_t vect2_divide(vect2_t v, float scalar);
float vect2_dot(vect2_t v1, vect2_t v2);

/*=================================== 3d Vectors =================================*/
typedef struct
{
    float x;
    float y;
    float z;

} vect3_t;

float vect3_length(vect3_t v);
vect3_t vect3_add(vect3_t v1, vect3_t v2);
vect3_t vect3_subtract(vect3_t v1, vect3_t v2);
vect3_t vect3_mul(vect3_t v, float scalar);
vect3_t vect3_divide(vect3_t v, float scalar);
float vect3_dot(vect3_t v1, vect3_t v2);
vect3_t vect3_cross(vect3_t v1, vect3_t v2);
vect3_t vect3_normalize(vect3_t v);
vect3_t vect3RotateX(vect3_t v, float angle);
vect3_t vect3RotateY(vect3_t v, float angle);
vect3_t vect3RotateZ(vect3_t v, float angle);

#endif