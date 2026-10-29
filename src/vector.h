#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{

    float x;
    float y;

} vect2_t;

typedef struct
{
    float x;
    float y;
    float z;

} vect3_t;


// opertatin on 2d and 3d vectors
vect2_t vect2_add(vect2_t v1, vect2_t v2);
vect3_t vect3_add(vect3_t v1, vect3_t v2);

#endif