#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

// global variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool isRunning = true;
uint32_t *colorBuffer = NULL;
void clearColorBuffer(uint32_t color);
SDL_Texture *colorBufferTexture = NULL;
void setPixel(int x, int y, uint32_t color);
void destroyWindow(void);

/* function prototypes*/
bool initializeWindow(void);
void processInput(void);
void render(void);
void update(void);
void setUp(void);
void clearColorBuffer(uint32_t color);
void createGrid(int offset, uint32_t color);

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

bool initializeWindow(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        false;
    }

    SDL_SetRenderDrawColor(renderer, 66, 0xcc, 33, 10);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return true;
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
    clearColorBuffer(0xFFDAA520);

    createGrid(50, 0xFF000000);

    // create SDL texture
    colorBufferTexture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, WINDOW_WIDTH * sizeof(uint32_t));

    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void clearColorBuffer(uint32_t color)
{

    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            colorBuffer[y * WINDOW_WIDTH + x] = color;
        }
    }
}

void setPixel(int x, int y, uint32_t color)
{

    if (x < 0 || x >= WINDOW_WIDTH ||
        y < 0 || y >= WINDOW_HEIGHT)
    {
        return;
    }
    int index = y * WINDOW_WIDTH + x;
    colorBuffer[index] = color;
}

void destroyWindow(void)
{
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    colorBufferTexture = NULL;
}

void createGrid(int offset, uint32_t color)
{

    for (int y = 0; y < WINDOW_HEIGHT; y += offset)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            setPixel(x, y, color);
        }
    }

    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x += offset)
        {
            setPixel(x, y, color);
        }
    }
}