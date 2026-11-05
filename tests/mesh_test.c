#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/array.h"
#include "../src/mesh.h"

static void loadText(const char *text)
{
    char path[] = "/tmp/renderer-obj-XXXXXX";
    int fd = mkstemp(path);
    assert(fd >= 0);
    FILE *file = fdopen(fd, "w");
    assert(file != NULL);
    assert(fputs(text, file) >= 0);
    assert(fclose(file) == 0);
    loadObjFileData(path, &mesh);
    assert(unlink(path) == 0);
}

static void assertValidIndices(void)
{
    int vertexCount = array_length(mesh.vertices);
    for (int i = 0; i < array_length(mesh.faces); i++)
    {
        face_t face = mesh.faces[i];
        assert(face.a >= 1 && face.a <= vertexCount);
        assert(face.b >= 1 && face.b <= vertexCount);
        assert(face.c >= 1 && face.c <= vertexCount);
    }
}

int main(void)
{
    loadObjFileData("src/assets/cube.obj", &mesh);
    assert(array_length(mesh.vertices) == 8);
    assert(array_length(mesh.faces) == 12);
    assert(mesh.vertices[0].x == 1 && mesh.vertices[0].y == 1 && mesh.vertices[0].z == -1);
    assert(mesh.faces[0].a == 1 && mesh.faces[0].b == 5 && mesh.faces[0].c == 7);
    assert(mesh.faces[1].a == 1 && mesh.faces[1].b == 7 && mesh.faces[1].c == 3);
    assertValidIndices();

    loadObjFileData("src/assets/torus.obj", &mesh);
    assert(array_length(mesh.vertices) == 576);
    assert(array_length(mesh.faces) == 1152);
    assertValidIndices();

    loadText("  # whitespace, all face formats, negative indices, inline comments\n"
             "\tv\t0 0 0\n v 1 0 0\nv 1 1 0\nv 0 1 0\n"
             "vt 0 0\nvn 0 0 1\n"
             "f 1 2 3\nf 1/1 2/1 3/1\nf 1//1 2//1 3//1\n"
             "f 1/1/1 2/1/1 3/1/1\nf -4 -3 -2 -1 # quad\n");
    assert(array_length(mesh.vertices) == 4);
    assert(array_length(mesh.faces) == 6);
    assert(mesh.faces[4].a == 1 && mesh.faces[4].b == 2 && mesh.faces[4].c == 3);
    assert(mesh.faces[5].a == 1 && mesh.faces[5].b == 3 && mesh.faces[5].c == 4);
    assertValidIndices();

    // Every rejected file must preserve the previously loaded geometry.
    const char *invalid[] = {
        "v 0 0\n",
        "v nope 0 0\n",
        "v nan 0 0\n",
        "v 0 0 0\nf 0 1 1\n",
        "v 0 0 0\nf 1 1 2\n",
        "v 0 0 0\nf -2 1 1\n",
        "v 0 0 0\nf 1 1\n",
        "v 0 0 0\nf 1x 1 1\n",
        "v 0 0 0\nf 9999999999999999999999 1 1\n",
        "# empty geometry\n",
    };
    vect3_t *vertices = mesh.vertices;
    face_t *faces = mesh.faces;
    for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++)
    {
        loadText(invalid[i]);
        assert(mesh.vertices == vertices && mesh.faces == faces);
        assert(array_length(mesh.vertices) == 4 && array_length(mesh.faces) == 6);
    }
    loadObjFileData("/dev/null/nonexistent.obj", &mesh);
    loadObjFileData(NULL, &mesh);
    assert(mesh.vertices == vertices && mesh.faces == faces);

    // A final line without a newline is valid.
    loadText("v 0 0 0\nv 1 0 0\nv 0 1 0\nf 1 2 3");
    assert(array_length(mesh.vertices) == 3 && array_length(mesh.faces) == 1);

    array_free(mesh.vertices);
    array_free(mesh.faces);
    puts("PASS: OBJ assets, face formats, triangulation, indices, replacement, and failed loads");
    return 0;
}
