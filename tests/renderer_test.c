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

    // Releasing Escape must not be interpreted as pressing Escape.
    SDL_Event event = {0};
    event.type = SDL_KEYUP;
    event.key.keysym.sym = SDLK_ESCAPE;
    isRunning = true;
    assert(SDL_PushEvent(&event) == 1);
    processInput();
    assert(isRunning);

    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_a;
    assert(SDL_PushEvent(&event) == 1);
    processInput();
    assert(isRunning);

    event.key.keysym.sym = SDLK_ESCAPE;
    assert(SDL_PushEvent(&event) == 1);
    processInput();
    assert(!isRunning);

    isRunning = true;
    event.type = SDL_QUIT;
    assert(SDL_PushEvent(&event) == 1);
    processInput();
    assert(!isRunning);
    isRunning = true;

    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
        0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    assert(surface != NULL);
    renderer = SDL_CreateSoftwareRenderer(surface);
    assert(renderer != NULL);
    setUp();
    assert(colorBuffer != NULL);
    assert(colorBufferTexture != NULL);

    // Lines include both endpoints, even when they collapse to one pixel.
    const int lines[][4] = {
        {10, 10, 15, 10}, {15, 10, 10, 10},
        {10, 10, 10, 15}, {10, 15, 10, 10},
        {10, 10, 15, 15}, {15, 15, 10, 10},
        {10, 10, 12, 15}, {12, 15, 10, 10},
        {10, 15, 15, 10}, {15, 10, 10, 15},
        {10, 10, 10, 10},
    };
    for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); i++)
    {
        clearColorBuffer(0xFF000000);
        const int *line = lines[i];
        drawLine(line[0], line[1], line[2], line[3], 0xFFFF0000);
        assert(colorBuffer[line[1] * WINDOW_WIDTH + line[0]] == 0xFFFF0000);
        assert(colorBuffer[line[3] * WINDOW_WIDTH + line[2]] == 0xFFFF0000);
    }

    vect3_t sum = {0};
    for (int i = 0; i < N_MESH_VERTICES; i++)
    {
        vect3_t p = mesh_vertices[i];
        assert(p.x >= -1 && p.x <= 1);
        assert(p.y >= -1 && p.y <= 1);
        assert(p.z >= -1 && p.z <= 1);
        sum.x += p.x;
        sum.y += p.y;
        sum.z += p.z;
        for (int j = 0; j < i; j++)
        {
            assert(p.x != mesh_vertices[j].x || p.y != mesh_vertices[j].y || p.z != mesh_vertices[j].z);
        }

        // Rotation must preserve distance from the origin.
        vect3_t rotated = vect3RotateY(p, 0.73f);
        float before = p.x * p.x + p.y * p.y + p.z * p.z;
        float after = rotated.x * rotated.x + rotated.y * rotated.y + rotated.z * rotated.z;
        assert(fabsf(before - after) < 0.00001f);
    }
    assert(N_MESH_VERTICES == 8);
    assert(sum.x == 0 && sum.y == 0 && sum.z == 0);
    int edgeUses[N_MESH_VERTICES][N_MESH_VERTICES] = {0};
    int surfaceTriangles[3][2] = {0};
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        face_t face = mesh_faces[i];
        assert(face.a >= 1 && face.a <= N_MESH_VERTICES);
        assert(face.b >= 1 && face.b <= N_MESH_VERTICES);
        assert(face.c >= 1 && face.c <= N_MESH_VERTICES);
        assert(face.a != face.b && face.b != face.c && face.c != face.a);

        int indices[] = {face.a - 1, face.b - 1, face.c - 1};
        vect3_t a = mesh_vertices[indices[0]];
        vect3_t b = mesh_vertices[indices[1]];
        vect3_t c = mesh_vertices[indices[2]];
        float coordinates[3][3] = {
            {a.x, b.x, c.x}, {a.y, b.y, c.y}, {a.z, b.z, c.z},
        };
        int surfaces = 0;
        for (int axis = 0; axis < 3; axis++)
        {
            if (coordinates[axis][0] == coordinates[axis][1] &&
                coordinates[axis][1] == coordinates[axis][2])
            {
                surfaceTriangles[axis][coordinates[axis][0] > 0]++;
                surfaces++;
            }
        }
        assert(surfaces == 1); // A triangle must lie on one cube surface.
        for (int j = 0; j < 3; j++)
        {
            int from = indices[j];
            int to = indices[(j + 1) % 3];
            edgeUses[from][to]++;
            edgeUses[to][from]++;
        }
    }
    for (int axis = 0; axis < 3; axis++)
    {
        assert(surfaceTriangles[axis][0] == 2);
        assert(surfaceTriangles[axis][1] == 2);
    }
    for (int a = 0; a < N_MESH_VERTICES; a++)
    {
        for (int b = a + 1; b < N_MESH_VERTICES; b++)
        {
            vect3_t va = mesh_vertices[a];
            vect3_t vb = mesh_vertices[b];
            int differentAxes = (va.x != vb.x) + (va.y != vb.y) + (va.z != vb.z);
            if (differentAxes == 1)
                assert(edgeUses[a][b] == 2); // All twelve cube edges exist.
            else
                assert(edgeUses[a][b] == 0 || edgeUses[a][b] == 2);
        }
    }

    update();
    render();
    // A slow frame must not trap the next update in the frame limiter.
    SDL_Delay(FRAME_TARGET_TIME + 5);
    update();
    for (int i = 0; i < N_MESH_FACES; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vect2_t p = trianglesToRender[i].points[j];
            assert(isfinite(p.x) && isfinite(p.y));
            assert(p.x > 0 && p.x < WINDOW_WIDTH - 3);
            assert(p.y > 0 && p.y < WINDOW_HEIGHT - 3);
        }
    }
    render();
    size_t bufferSize = sizeof(*colorBuffer) * WINDOW_WIDTH * WINDOW_HEIGHT;
    uint32_t *expectedFrame = malloc(bufferSize);
    assert(expectedFrame != NULL);
    memcpy(expectedFrame, colorBuffer, bufferSize);

    // Verify the uploaded image, not just the CPU framebuffer: drawing after
    // upload used to leave the red points absent from the displayed frame.
    uint32_t *displayedFrame = malloc(bufferSize);
    assert(displayedFrame != NULL);
    assert(SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888,
                                displayedFrame, WINDOW_WIDTH * sizeof(uint32_t)) == 0);
    assert(memcmp(expectedFrame, displayedFrame, bufferSize) == 0);
    free(displayedFrame);

    // The same pose must render identically regardless of previous pixels.
    clearColorBuffer(0xFFFF0000);
    render();
    assert(memcmp(expectedFrame, colorBuffer, bufferSize) == 0);
    int redPixels = 0;
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
    {
        redPixels += colorBuffer[i] == 0xFFFF0000;
    }
    assert(redPixels > N_MESH_VERTICES * 3 * 3);

    free(expectedFrame);
    SDL_DestroyTexture(colorBufferTexture);
    destroyWindow();
    SDL_FreeSurface(surface);
    puts("PASS: input events, cube topology, line endpoints, projection, wireframe, texture upload, and fresh frames");
    return 0;
}
