#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES 12 // 6 cube faces two triangles per face

void loadCubeMeshData(void);
extern vect3_t mesh_vertices[N_CUBE_VERTICES];

extern face_t mesh_faces[N_CUBE_FACES];

typedef struct
{

    vect3_t *vertices;
    face_t *faces;
    vect3_t rotation;
    vect3_t position;

} mesh_t;

// Destination must be zero-initialized or own arrays from this implementation.
// Replaces geometry on success; preserves it and the transform on failure.
// Loads positions and convex faces via fan triangulation. Lines must be < 4095 characters.
void loadObjFileData(const char *fileName, mesh_t *destination);

extern mesh_t mesh;
#endif
