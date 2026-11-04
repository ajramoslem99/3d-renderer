#include "mesh.h"
#include "array.h"
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void loadObjFileData(const char *fileName)
{
    if (fileName == NULL)
    {
        fprintf(stderr, "OBJ filename is NULL\n");
        return;
    }

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        perror(fileName);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {

        if (strncmp(line, "v ", 2) == 0)
        {

            // vertext data
            vect3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh.vertices, vertex);
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            // face data
            face_t face;
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                   &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                   &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                   &vertex_indices[2], &texture_indices[2], &normal_indices[2]);

            face.a = vertex_indices[0];
            face.b = vertex_indices[1];
            face.c = vertex_indices[2];
            array_push(mesh.faces, face);
        }
    }


    fclose(fp);
    
}
