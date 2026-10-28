#include <stdio.h>
#include "display.h"

bool isRunning = true;

/* function prototypes*/
void processInput(void);
void render(void);
void update(void);
void setUp(void);


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
    renderColorBuffer();
    createDottedGrid(10, 0xFF008080);
    renderRectangle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 50, 50, 0xFFFF0000);
    SDL_RenderPresent(renderer);
}

void update(void)
{
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
}
