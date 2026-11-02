#include <stdio.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

// macros
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

/* Global Variables */

triangle_t *trianglesToRender = NULL;
bool isRunning = true;
float fov = 524; // field of view
vect3_t cameraPosition = {.x = 0, .y = 0, .z = -5};
vect3_t meshRotation = {.x = 0, .y = 0, .z = 0};
static Uint32 previousFrameTime = 0;

/* function prototypes*/
void processInput(void);
void render(void);
void update(void);
void setUp(void);
vect2_t project(vect3_t point);

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
        renderRectangle(triangle.points[0].x, triangle.points[0].y, 2, 2, 0xFFFF00);
        renderRectangle(triangle.points[1].x, triangle.points[1].y, 2, 2, 0xFFF00);
        renderRectangle(triangle.points[2].x, triangle.points[2].y, 2, 2, 0xFFFF0000);

        // use drawTriangle function from display.c to draw the triangle
        drawTriangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFFFF0000);
    }

    // clear array of triangles  to be rendered every frame loop
    array_free(trianglesToRender);
    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

void update(void)
{

    // intialize trianglesToRender array
    trianglesToRender = NULL;

    meshRotation.x += 0.01;
    meshRotation.y += 0.01;
    meshRotation.z += 0.01;

    Uint32 currentTime = SDL_GetTicks();
    Uint32 frameTime = currentTime - previousFrameTime;
    if (frameTime < FRAME_TARGET_TIME)
    {
        SDL_Delay(FRAME_TARGET_TIME - frameTime);
    }

    previousFrameTime = SDL_GetTicks();

    // loop over all faces
    int num_mesh_faces = array_length(mesh.faces);
    for (int i = 0; i < num_mesh_faces; i++)
    {

        //face_t face = mesh_faces[i];
        face_t face = mesh.faces[i];
        vect3_t faceVertices[3];
        faceVertices[0] =  mesh.vertices[face.a - 1]; 
        faceVertices[1] =  mesh.vertices[face.b - 1];
        faceVertices[2] = mesh.vertices[face.c - 1]; 

        triangle_t projectedTriangle;

        // loop all three vertices of the current face and apply tranformation
        for (int j = 0; j < 3; j++)
        {
            vect3_t transformedvertex = faceVertices[j];

            transformedvertex = vect3RotateX(transformedvertex, meshRotation.x);
            transformedvertex = vect3RotateY(transformedvertex, meshRotation.y);
            transformedvertex = vect3RotateZ(transformedvertex, meshRotation.z);

            // Express the vertex relative to the fixed, forward-facing camera.
            transformedvertex.x -= cameraPosition.x;
            transformedvertex.y -= cameraPosition.y;
            transformedvertex.z -= cameraPosition.z;

            // project the curret vertex
            vect2_t projectedVertex = project(transformedvertex);

            // Projection gives offsets from the center, not screen coordinates.
            projectedVertex.x += WINDOW_WIDTH / 2;
            projectedVertex.y += WINDOW_HEIGHT / 2;

            projectedTriangle.points[j] = projectedVertex;
        }

        array_push(trianglesToRender, projectedTriangle);
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

    loadCubeMeshData();
}

vect2_t project(vect3_t point)
{
    vect2_t p = {(fov * point.x) / point.z, (fov * point.y) / point.z};
    return p;
}
