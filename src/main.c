#include <stdio.h>
#include "display.h"
#include "vector.h"

// macro

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define POINTS_PER_AXIS 9
#define N_POINTS (POINTS_PER_AXIS * POINTS_PER_AXIS * POINTS_PER_AXIS)

bool isRunning = true;
vect3_t cube[N_POINTS];
vect2_t projectedPoints[N_POINTS];
float fov = 640; // field of view
vect3_t cameraPosition = {.x = 0, .y = 0, .z = -5};
vect3_t cubeRotation = {0};
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

        else if ((event.type = SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
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

    for (int i = 0; i < N_POINTS; i++)
    {

        renderRectangle(projectedPoints[i].x + WINDOW_WIDTH / 2, projectedPoints[i].y + WINDOW_HEIGHT / 2, 2, 2, 0xFF00FF00);
    }
    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

void update(void)
{

    Uint32 currentTime = SDL_GetTicks();
    Uint32 frameTime = currentTime - previousFrameTime;
    if (frameTime < FRAME_TARGET_TIME)
    {
        SDL_Delay(FRAME_TARGET_TIME - frameTime);
    }
     
    previousFrameTime = SDL_GetTicks();

    cubeRotation.x += 0.01;
    cubeRotation.y += 0.01;
    cubeRotation.z += 0.01;
    for (int i = 0; i < N_POINTS; i++)
    {
        vect3_t cubePoint = cube[i];

        // rotate the cube
        vect3_t rotatedPoint = vect3RotateY(cubePoint, cubeRotation.y);
        rotatedPoint = vect3RotateX(rotatedPoint, cubeRotation.x);
        rotatedPoint = vect3RotateZ(rotatedPoint, cubeRotation.z);

        // translate the point away from the camera
        rotatedPoint.z -= cameraPosition.z;

        // project the point
        vect2_t projectedPoint = project(rotatedPoint);

        // save projected point
        projectedPoints[i] = projectedPoint;
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

    // load array of points
    int i = 0;
    for (int x = 0; x < POINTS_PER_AXIS; x++)
    {

        for (int y = 0; y < POINTS_PER_AXIS; y++)
        {

            for (int z = 0; z < POINTS_PER_AXIS; z++)
            {
                // Include both endpoints: -1, -0.75, ..., 0.75, 1.
                vect3_t p = {
                    -1.0f + 2.0f * x / (POINTS_PER_AXIS - 1),
                    -1.0f + 2.0f * y / (POINTS_PER_AXIS - 1),
                    -1.0f + 2.0f * z / (POINTS_PER_AXIS - 1)};
                cube[i] = p;
                i++;
            }
        }
    }
}

vect2_t project(vect3_t point)
{
    vect2_t p = {(fov * point.x) / point.z, (fov * point.y) / point.z};
    return p;
}
