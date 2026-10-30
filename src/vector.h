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


// opertatin on 2dvectors
vect2_t vect2_add(vect2_t v1, vect2_t v2);



// operations on 3dvectors
vect3_t vect3_add(vect3_t v1, vect3_t v2);
vect3_t vect3_subtract(vect3_t v1, vect3_t v2);
float vect3_dot(vect3_t v1, vect3_t v2);
vect3_t vect3_cross(vect3_t v1, vect3_t v2);
vect3_t vect3_normalize(vect3_t v);
float vect3_length(vect3_t v);
vect3_t vect3RotateX(vect3_t v, float angle);
vect3_t vect3RotateY(vect3_t v, float angle);
vect3_t vect3RotateZ(vect3_t v, float angle);


#endif