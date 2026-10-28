#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// global variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool isRunning = true;

SDL_Rect rectangle = {100, 100, 150, 150};

/* function prototypes*/
bool initializeWindow(void);
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

bool initializeWindow(void)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw  white rectangle
    // rectangle = {100, 100, 150, 150};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rectangle);
    SDL_RenderPresent(renderer);
}

void update(void)
{

    rectangle.x += 1;
    rectangle.y += 1;
}

void setUp(void)
{
}