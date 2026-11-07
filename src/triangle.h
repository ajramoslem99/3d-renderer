#ifndef TRIANGLE_H
#define TRIANGLE_H


#include<stdint.h>
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



void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif