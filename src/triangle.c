#include "triangle.h"
#include <stdint.h>
#include "display.h"

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{

    // find the two slops from triangle legs
    float inverseSlop1 = (x1 - x0) / (float)(y1 - y0);
    float inverseSlop2 = (x2 - x0) / (float)(y2 - y0);

    // x_start x_end from the top vertex(x0, y0)
    float x_start = x0;
    float x_end = x0;

    // loop all the scan line fromtop to bottom
    for (int y = y0; y < y2; y++)
    {

        drawLine((int)x_start, y, (int)x_end, y, color);
        x_start += inverseSlop1;
        x_end += inverseSlop2;
    }
}

void fillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{

    // find the two slops from triangle legs
    float inverseSlop1 = (x2 - x0) / (float)(y2 - y0);
    float inverseSlop2 = (x2 - x1) / (float)(y2 - y1);

    // x_start x_end from the top vertex(x2, y2)
    float x_start = x2;
    float x_end = x2;

    // loop all the scan line from btootm to top
    for (int y = y2; y >= y0; y--)
    {

        drawLine((int)x_start, y, (int)x_end, y, color);
        x_start -= inverseSlop1;
        x_end -= inverseSlop2;
    }
}

void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{

    //  sort the vertices by y-coordinates in ascending order

    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }

    if (y0 > y1)
    {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        fillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
        return;
    }

    if (y0 == y1)
    {
        fillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
        return;
    }

    // calculate the new vertex  (Mx, My) usinging trinagle similarity
    int My = y1;
    int Mx = x0 + ((float)(x2 - x0) * (y1 - y0)) / (float)(y2 - y0);

    // draw fat-bottom trinagle
    fillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);

    // draw flat-top triangle
    // fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, 0xFF0000FF);
    fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
}