#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
SDL_DisplayMode displayMode;
SDL_Texture *colorBufferTexture = NULL;
uint32_t *colorBuffer = NULL;

bool initializeWindow(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.w, displayMode.h, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return false;
    }

    setFullScreen();

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

void renderColorBuffer(void)
{
    SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, WINDOW_WIDTH * sizeof(uint32_t));

    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
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

    if (offset == 0 || color == 0 || offset > WINDOW_WIDTH || offset > WINDOW_HEIGHT)
    {
        return;
    }
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

void createDottedGrid(int offset, uint32_t color)
{

    if (offset == 0 || color == 0 || offset > WINDOW_WIDTH || offset > WINDOW_HEIGHT)
    {
        return;
    }
    for (int y = 0; y < WINDOW_HEIGHT; y += offset)
    {
        for (int x = 0; x < WINDOW_WIDTH; x += offset)
        {
            setPixel(x, y, color);
        }
    }
}

void setFullScreen(void)
{

    SDL_GetDesktopDisplayMode(0, &displayMode);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void renderRectangle(int x, int y, int w, int h, uint32_t color)
{

    for (int i = x; i < x + w; i++)
    {

        for (int j = y; j < y + h; j++)
        {
            if (x)
                setPixel(i, j, color);
        }
    }
}