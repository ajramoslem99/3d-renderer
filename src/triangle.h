#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct
{

    vect2_t points[3];

} triangle_t;

typedef struct
{

    int a;
    int b;
    int c;

} face_t;

#endif