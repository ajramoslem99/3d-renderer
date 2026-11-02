#include "mesh.h"
#include "array.h"
#include <stdio.h>

mesh_t mesh = {.vertices = NULL, .faces = NULL, .rotation = {0, 0, 0}};
vect3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1},
    {.x = -1, .y = -1, .z = 1},
    {.x = -1, .y = 1, .z = -1},
    {.x = -1, .y = 1, .z = 1},
    {.x = 1, .y = -1, .z = -1},
    {.x = 1, .y = -1, .z = 1},
    {.x = 1, .y = 1, .z = -1},
    {.x = 1, .y = 1, .z = 1},
};

face_t cube_faces[N_CUBE_FACES] = {

    // x = -1
    {1, 2, 4},
    {1, 4, 3},

    // x = +1
    {5, 7, 8},
    {5, 8, 6},

    // y = -1
    {1, 5, 6},
    {1, 6, 2},

    // y = +1
    {3, 4, 8},
    {3, 8, 7},

    // z = -1
    {1, 3, 7},
    {1, 7, 5},

    // z = +1
    {2, 6, 8},
    {2, 8, 4},

};

void loadCubeMeshData(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; i++)
    {
        array_push(mesh.vertices, cube_vertices[i]);
    }

    for (int i = 0; i < N_CUBE_FACES; i++)
    {

        array_push(mesh.faces, cube_faces[i]);
    }
}