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

void loadObjFileData(const char *fileName, mesh_t *destination)
{

    if (destination == NULL)
    {
        fprintf(stderr, "Destination mesh is NULL\n");
        return;
    }

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

    mesh_t loaded = {0};
    char line[4096];
    int lineNumber = 0;
    const char *error = NULL;
    while (fgets(line, sizeof(line), fp))
    {
        lineNumber++;
        if (strchr(line, '\n') == NULL && !feof(fp))
        {
            error = "OBJ line is too long";
            goto failure;
        }
        char *comment = strchr(line, '#');
        if (comment != NULL)
            *comment = '\0';
        const char *whitespace = " \t\r\n";
        char *record = strtok(line, whitespace);
        if (record == NULL)
            continue;
        if (strcmp(record, "v") == 0)
        {
            vect3_t vertex;
            float *coordinates[] = {&vertex.x, &vertex.y, &vertex.z};
            for (int i = 0; i < 3; i++)
            {
                char *token = strtok(NULL, whitespace);
                char *end;
                errno = 0;
                if (token == NULL)
                {
                    error = "vertex needs three coordinates";
                    goto failure;
                }
                *coordinates[i] = strtof(token, &end);
                if (end == token || *end != '\0' || errno == ERANGE || !isfinite(*coordinates[i]))
                {
                    error = "invalid vertex coordinate";
                    goto failure;
                }
            }
            array_push(loaded.vertices, vertex);
        }
        else if (strcmp(record, "f") == 0)
        {
            int first = 0, previous = 0, count = 0;
            char *token;
            while ((token = strtok(NULL, whitespace)) != NULL)
            {
                // Only positions are used: accept v, v/vt, v//vn and v/vt/vn.
                char *end;
                errno = 0;
                long index = strtol(token, &end, 10);
                int vertexCount = array_length(loaded.vertices);
                if (end == token || (*end != '\0' && *end != '/') ||
                    errno == ERANGE || index == 0 || index > INT_MAX ||
                    index < -(long)vertexCount)
                {
                    error = "invalid face vertex index";
                    goto failure;
                }
                if (index < 0)
                    index += vertexCount + 1L;

                // Fan triangulation: (a,b,c,d) becomes (a,b,c), (a,c,d).
                if (count == 0)
                    first = (int)index;
                else if (count >= 2)
                {
                    face_t face = {first, previous, (int)index};
                    array_push(loaded.faces, face);
                }
                previous = (int)index;
                count++;
            }
            if (count < 3)
            {
                error = "face needs at least three vertices";
                goto failure;
            }
        }
    }
    if (ferror(fp) || array_length(loaded.vertices) == 0 || array_length(loaded.faces) == 0)
    {
        error = "could not read mesh geometry";
        goto failure;
    }
    int vertexCount = array_length(loaded.vertices);
    for (int i = 0; i < array_length(loaded.faces); i++)
    {
        face_t f = loaded.faces[i];
        if (f.a > vertexCount || f.b > vertexCount || f.c > vertexCount)
        {
            error = "face references a missing vertex";
            goto failure;
        }
    }
    fclose(fp);
    array_free(destination->vertices);
    array_free(destination->faces);
    destination->vertices = loaded.vertices;
    destination->faces = loaded.faces;
    return;

failure:
    fprintf(stderr, "%s:%d: %s\n", fileName, lineNumber, error);
    fclose(fp);
    array_free(loaded.vertices);
    array_free(loaded.faces);
}
