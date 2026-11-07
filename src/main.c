#include <stdio.h>
#include <math.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

// macros
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

/* Global Variables */
mesh_t *meshesToRender = NULL;
triangle_t *trianglesToRender = NULL;
bool isRunning = true;
float fov = 628; // field of view
vect3_t cameraPosition = {.x = 0, .y = 0, .z = -7};
// vect3_t cameraPosition = {.x = 0, .y = 0, .z = 0};
static Uint32 previousFrameTime = 0;

const char *files[] = {
    "./src/assets/f22.obj",
    "./src/assets/torus.obj",
    "./src/assets/cube.obj"};

/* function prototypes*/
void processInput(void);
void render(void);
void update(void);
void setUp(void);
vect2_t project(vect3_t point);
mesh_t *createMeshes(int count, const char *paths[]);
static void normalizeMesh(mesh_t *mesh);
void freeResources(void);

int main()
{

    /* Create SDL Window */
    isRunning = initializeWindow();
    if (!isRunning)
    {
        destroyWindow();
        return 1;
    }

    setUp();
    previousFrameTime = SDL_GetTicks();

    /* ** Rendering Loop ***/

    while (isRunning)
    {
        processInput();
        update();
        render();
    }

    freeResources();
    destroyWindow();

    return 0;
}

void processInput(void)
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }

        else if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
        {
            isRunning = false;
        }
    }
}

void render(void)
{
    // Rebuild the framebuffer so previous cube positions do not leave trails.
    clearColorBuffer(0xFF000000);
    // createDottedGrid(10, 0xFF008080);

    // loop over all prijected triangles and render them
    int num_triangles = array_length(trianglesToRender);
    for (int i = 0; i < num_triangles; i++)
    {
        triangle_t triangle = trianglesToRender[i];

        //  use renderRectangle function from display.c to draw individual points of the triangle
        // renderRectangle(triangle.points[0].x, triangle.points[0].y, 2, 2, 0xFFFF00);
        // renderRectangle(triangle.points[1].x, triangle.points[1].y, 2, 2, 0xFFF00);
        // renderRectangle(triangle.points[2].x, triangle.points[2].y, 2, 2, 0xFFFF0000);

        // use drawTriangle function from display.c to draw the triangle
        // drawTriangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xff00FF00);

        drawFilledTriangle(300, 50, 50, 400, 300, 500, 0xFF00FF00);
    }

    array_free(trianglesToRender);
    trianglesToRender = NULL;
    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

void update(void)
{

    // Discard any previous frame, including an update that was not rendered.
    array_free(trianglesToRender);
    trianglesToRender = NULL;

    Uint32 currentTime = SDL_GetTicks();
    Uint32 frameTime = currentTime - previousFrameTime;
    if (frameTime < FRAME_TARGET_TIME)
    {
        SDL_Delay(FRAME_TARGET_TIME - frameTime);
    }

    previousFrameTime = SDL_GetTicks();

    // loop over all faces

    for (int k = 0; k < array_length(meshesToRender); k++)
    {
        mesh_t *current = &meshesToRender[k];
        current->rotation.x += 0.01f;
        // current->rotation.y += 0.01f;
        // current->rotation.z += 0.01f;

        int num_mesh_faces = array_length(current->faces);
        triangle_t projectedTriangle;
        for (int i = 0; i < num_mesh_faces; i++)
        {

            face_t face = current->faces[i];
            vect3_t faceVertices[3];
            faceVertices[0] = current->vertices[face.a - 1];
            faceVertices[1] = current->vertices[face.b - 1];
            faceVertices[2] = current->vertices[face.c - 1];

            // create an array to hold the transformed vertices
            vect3_t transformedVertices[3];

            // loop all three vertices of the current face and apply tranformation
            for (int j = 0; j < 3; j++)
            {
                vect3_t transformedvertex = faceVertices[j];

                transformedvertex = vect3RotateX(transformedvertex, current->rotation.x);
                transformedvertex = vect3RotateY(transformedvertex, current->rotation.y);
                transformedvertex = vect3RotateZ(transformedvertex, current->rotation.z);

                // Place the rotated model in the world before applying the camera.
                transformedvertex.x += current->position.x;
                transformedvertex.y += current->position.y;
                transformedvertex.z += current->position.z;

                // Express the vertex relative to the fixed, forward-facing camera.
                transformedvertex.x -= cameraPosition.x;
                transformedvertex.y -= cameraPosition.y;
                transformedvertex.z -= cameraPosition.z;

                // save the transformed vertex
                transformedVertices[j] = transformedvertex;
            }

            // check back face culling (clock wise)
            vect3_t vector_A = transformedVertices[0]; /*   A */
            vect3_t vector_B = transformedVertices[1]; /* /  \*/
            vect3_t vector_C = transformedVertices[2]; /*C---B */

            vect3_t vector_AB = vect3_subtract(vector_B, vector_A);
            vect3_t vector_AC = vect3_subtract(vector_C, vector_A);
            vect3_t face_normal = vect3_cross(vector_AC, vector_AB);

            // normalize the face normal vector
            vect3_normalize(&face_normal);

            vect3_t camera_ray = vect3_subtract(vector_A, cameraPosition);
            float dot_product = vect3_dot(face_normal, camera_ray);

            if (dot_product < 0)
            {
                continue;
            }

            // loop all three vertices of the current face and project them
            for (int j = 0; j < 3; j++)
            {

                // project the curret vertex
                vect2_t projectedVertex = project(transformedVertices[j]);

                // Projection gives offsets from the center, not screen coordinates.
                projectedVertex.x += WINDOW_WIDTH / 2;
                projectedVertex.y += WINDOW_HEIGHT / 2;

                projectedTriangle.points[j] = projectedVertex;
            }

            array_push(trianglesToRender, projectedTriangle);
        }
    }
}

void setUp(void)
{

    // allocate the necessary memory
    colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

    // give each pixel red color
    clearColorBuffer(0xFF000000);

    // create SDL texture
    colorBufferTexture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);

    // Load and arrange the selected models for comparison.
    int count = (int)(sizeof(files) / sizeof(files[0]));
    meshesToRender = createMeshes(count, files);
}

vect2_t project(vect3_t point)
{
    vect2_t p = {(fov * point.x) / point.z, (fov * point.y) / point.z};
    return p;
}

void freeResources(void)
{
    for (int i = 0; i < array_length(meshesToRender); i++)
    {
        array_free(meshesToRender[i].vertices);
        array_free(meshesToRender[i].faces);
    }
    array_free(meshesToRender);
    meshesToRender = NULL;
    array_free(trianglesToRender);
    trianglesToRender = NULL;
    array_free(mesh.faces);
    array_free(mesh.vertices);
    mesh.faces = NULL;
    mesh.vertices = NULL;
    free(colorBuffer);
    colorBuffer = NULL;
    SDL_DestroyTexture(colorBufferTexture);
    colorBufferTexture = NULL;
}

mesh_t *createMeshes(int count, const char *paths[])
{

    mesh_t *meshes = NULL;
    if (count <= 0 || paths == NULL)
        return NULL;

    for (int i = 0; i < count; i++)
    {

        mesh_t next = {0};
        loadObjFileData(paths[i], &next);

        if (array_length(next.vertices) == 0 || array_length(next.faces) == 0)
        {

            array_free(next.faces);
            array_free(next.vertices);
            continue;
        }

        normalizeMesh(&next);
        array_push(meshes, next);
    }

    int loadedCount = array_length(meshes);
    for (int i = 0; i < loadedCount; i++)
        meshes[i].position.x = (i - (loadedCount - 1) * 0.5f) * 2.6f;

    return meshes;
}

// This comparison view centers each model and gives it a radius of one unit.
// Uniform scaling preserves proportions but intentionally removes imported size differences.
static void normalizeMesh(mesh_t *mesh)
{
    vect3_t min = mesh->vertices[0];
    vect3_t max = min;
    int count = array_length(mesh->vertices);
    for (int i = 1; i < count; i++)
    {
        vect3_t v = mesh->vertices[i];
        min.x = fminf(min.x, v.x);
        min.y = fminf(min.y, v.y);
        min.z = fminf(min.z, v.z);
        max.x = fmaxf(max.x, v.x);
        max.y = fmaxf(max.y, v.y);
        max.z = fmaxf(max.z, v.z);
    }
    vect3_t center = {(min.x + max.x) * 0.5f,
                      (min.y + max.y) * 0.5f,
                      (min.z + max.z) * 0.5f};
    float radius = 0;
    for (int i = 0; i < count; i++)
    {
        mesh->vertices[i].x -= center.x;
        mesh->vertices[i].y -= center.y;
        mesh->vertices[i].z -= center.z;
        vect3_t v = mesh->vertices[i];
        radius = fmaxf(radius, sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
    }
    if (radius > 0)
    {
        for (int i = 0; i < count; i++)
        {
            mesh->vertices[i].x /= radius;
            mesh->vertices[i].y /= radius;
            mesh->vertices[i].z /= radius;
        }
    }
}
