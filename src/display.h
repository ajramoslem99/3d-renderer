#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *colorBufferTexture;
extern SDL_DisplayMode displayMode;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern uint32_t *colorBuffer;

bool initializeWindow(void);
void clearColorBuffer(uint32_t color);
void renderColorBuffer(void);
void createGrid(int offset, uint32_t color);
void createDottedGrid(int offset, uint32_t color);
void clearColorBuffer(uint32_t color);
void setPixel(int x, int y, uint32_t color);
void setFullScreen(void);
void destroyWindow(void);
void renderRectangle(int x, int y, int w, int h, uint32_t color);

#endif
