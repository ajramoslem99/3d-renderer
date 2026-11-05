// Exercise the actual setup, update and render paths using an in-memory SDL surface.
#include <assert.h>
#include <math.h>
#include <stdio.h>

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
    assert(array_length(meshesToRender) == 3);
    const int expectedVertices[] = {102, 576, 8};
    const int expectedFaces[] = {200, 1152, 12};
    int totalFaces = 0;
    for (int m = 0; m < 3; m++)
    {
        mesh_t *current = &meshesToRender[m];
        assert(array_length(current->vertices) == expectedVertices[m]);
        assert(array_length(current->faces) == expectedFaces[m]);
        totalFaces += expectedFaces[m];
        float radius = 0;
        for (int v = 0; v < expectedVertices[m]; v++)
        {
            vect3_t p = current->vertices[v];
            radius = fmaxf(radius, sqrtf(p.x * p.x + p.y * p.y + p.z * p.z));
        }
        assert(fabsf(radius - 1) < 0.00001f);
        if (m > 0)
        {
            assert(current->vertices != meshesToRender[m - 1].vertices);
            assert(current->faces != meshesToRender[m - 1].faces);
            assert(current->position.x > meshesToRender[m - 1].position.x);
        }
    }

    // Sample a full turn: every model must remain on screen and separated.
    for (int pose = 0; pose < 12; pose++)
    {
        for (int m = 0; m < 3; m++)
        {
            meshesToRender[m].rotation.x = pose * 0.53f;
            meshesToRender[m].rotation.y = pose * 0.37f;
        }
        update();
        assert(array_length(trianglesToRender) == totalFaces);
        int offset = 0;
        float previousMaxX = -1;
        for (int m = 0; m < 3; m++)
        {
            float minX = WINDOW_WIDTH, maxX = 0;
            for (int f = 0; f < expectedFaces[m]; f++)
            {
                for (int j = 0; j < 3; j++)
                {
                    vect2_t p = trianglesToRender[offset + f].points[j];
                    assert(isfinite(p.x) && isfinite(p.y));
                    assert(p.x > 0 && p.x < WINDOW_WIDTH - 2);
                    assert(p.y > 0 && p.y < WINDOW_HEIGHT - 2);
                    minX = fminf(minX, p.x);
                    maxX = fmaxf(maxX, p.x);
                }
            }
            assert(minX > previousMaxX);
            previousMaxX = maxX;
            offset += expectedFaces[m];
        }
        render();
        assert(trianglesToRender == NULL);

        // All three display regions must actually contain drawn pixels.
        for (int region = 0; region < 3; region++)
        {
            int pixels = 0;
            for (int y = 0; y < WINDOW_HEIGHT; y++)
                for (int x = region * WINDOW_WIDTH / 3; x < (region + 1) * WINDOW_WIDTH / 3; x++)
                    pixels += colorBuffer[y * WINDOW_WIDTH + x] != 0xFF000000;
            assert(pixels > 20);
        }
        if (pose == 1)
            assert(SDL_SaveBMP(surface, "/tmp/renderer-three-meshes.bmp") == 0);
    }
    freeResources();
    assert(meshesToRender == NULL && trianglesToRender == NULL);
    destroyWindow();
    SDL_FreeSurface(surface);
    puts("PASS: three independent meshes, full quad geometry, fitting, separation, projection, pixels, and cleanup");
    return 0;
}
