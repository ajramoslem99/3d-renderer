#include <stdio.h>
#include "display.h"
#include "vector.h"

// macro
#define N_POINTS (9 * 9 * 9)

bool isRunning = true;
vect3_t cube[N_POINTS];
vect2_t projectedPoints[N_POINTS];
float fov = 640; // field of view
vect3_t cameraPosition = {.x = 0, .y = 0, .z= -5};
vect3_t cubeRotation = {};

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

    /* ** Rendering Loop ***/

    while (isRunning)
    {
        processInput();
        update();
        render();
        SDL_Delay(16);
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
    for (int i = 0; i < N_POINTS; i++)
    {

        renderRectangle(projectedPoints[i].x + WINDOW_WIDTH / 2, projectedPoints[i].y + WINDOW_HEIGHT / 2, 3, 3, 0xFF00FF00);
    }
    renderColorBuffer();
    createDottedGrid(10, 0xFF008080);
    SDL_RenderPresent(renderer);
}

void update(void)
{

    cubeRotation.x += 0.1;
    for (int i = 0; i < N_POINTS; i++)
    {
        vect3_t cubePoint = cube[i];

        // move away from the camera
        cubePoint.z -= cameraPosition.z;
        vect2_t projectedPoint = project(cubePoint);
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
    for (float x = -1.0; x < 1.0; x += 0.25)
    {

        for (float y = -1.0; y < 1.0; y += 0.25)
        {

            for (float z = -1.0; z < 1.0; z += 0.25)
            {
                vect3_t p = {x, y, z};
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
