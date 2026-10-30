// Exercise the current renderer without opening a desktop window.
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define main renderer_main
#include "../src/main.c"
#undef main

int main(void)
{
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
        0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    assert(surface != NULL);
    renderer = SDL_CreateSoftwareRenderer(surface);
    assert(renderer != NULL);
    setUp();
    assert(colorBuffer != NULL);
    assert(colorBufferTexture != NULL);

    vect3_t sum = {0};
    int originCount = 0;
    for (int i = 0; i < N_POINTS; i++)
    {
        vect3_t p = cube[i];
        assert(p.x >= -1 && p.x <= 1);
        assert(p.y >= -1 && p.y <= 1);
        assert(p.z >= -1 && p.z <= 1);
        sum.x += p.x;
        sum.y += p.y;
        sum.z += p.z;
        originCount += p.x == 0 && p.y == 0 && p.z == 0;
        for (int j = 0; j < i; j++)
        {
            assert(p.x != cube[j].x || p.y != cube[j].y || p.z != cube[j].z);
        }

        // Rotation must preserve distance from the origin.
        vect3_t rotated = vect3RotateY(p, 0.73f);
        float before = p.x * p.x + p.y * p.y + p.z * p.z;
        float after = rotated.x * rotated.x + rotated.y * rotated.y + rotated.z * rotated.z;
        assert(fabsf(before - after) < 0.00001f);
    }
    assert(N_POINTS == 729);
    assert(originCount == 1);
    assert(sum.x == 0 && sum.y == 0 && sum.z == 0);
    assert(cube[0].x == -1 && cube[0].y == -1 && cube[0].z == -1);
    assert(cube[N_POINTS - 1].x == 1 && cube[N_POINTS - 1].y == 1 && cube[N_POINTS - 1].z == 1);

    update();
    render();
    update();
    render();
    size_t bufferSize = sizeof(*colorBuffer) * WINDOW_WIDTH * WINDOW_HEIGHT;
    uint32_t *expectedFrame = malloc(bufferSize);
    assert(expectedFrame != NULL);
    memcpy(expectedFrame, colorBuffer, bufferSize);

    // The same pose must render identically regardless of previous pixels.
    clearColorBuffer(0xFFFF0000);
    render();
    assert(memcmp(expectedFrame, colorBuffer, bufferSize) == 0);
    assert(colorBuffer[0] == 0xFF008080);

    free(expectedFrame);
    SDL_DestroyTexture(colorBufferTexture);
    destroyWindow();
    SDL_FreeSurface(surface);
    puts("PASS: centered cube, unique points, rotation, and fresh frames");
    return 0;
}
