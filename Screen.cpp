#include "Screen.hpp"
#include "jagdraw.hpp"

void Screen::drawBuffer()
{
    static int32_t pxrow;
    static int32_t pxcol;
    for (pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
#ifndef BITBUFFER

            //printf("       \n----------------\ndraw start x:%4f y:%4f \ncanvas size x:%4f y:%4f \ncanvas pos x:%3f y:%3f  \ndpxrow: %3d pxcol: %d\n", draw_start.x,draw_start.y,canvas_size.x,canvas_size.y,canvas_pos.x, canvas_pos.y,pxrow,pxcol);
            if (screenBuffer[pxrow][pxcol])
            {
                drawPixel(pxrow, pxcol, true);
                // printf("white pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            }
            else
            {
                drawPixel(pxrow, pxcol, false);
                // printf("black pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            }
#else
            static int16_t whichBit;
            static int16_t whichByte;
            static int16_t whichSubBit;

            whichBit = (pxrow * SCREEN_HEIGHT) + pxcol;
            whichByte = whichBit / 32; // whichBit << 5 might  be faster depending on compiler optimizations
            whichSubBit = whichBit % 32;

            if ((screenBuffer[whichByte] >> whichSubBit) & 0x00000001)
            {
                drawPixel(pxrow, pxcol, true);
                // printf("white pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            }
            else
            {
                drawPixel(pxrow, pxcol, false);
                // printf("black pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            }

#endif
        }
    }
}

void Screen::clear()
{
#ifndef BITBUFFER

    static int32_t pxrow;
    static int32_t pxcol;
    for (pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
            screenBuffer[pxrow][pxcol] = 0;
        }
    }
#else
    static int16_t bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT / 32;
    static int32_t pxrow;
    for (pxrow = 0; pxrow < bufferSize; pxrow++)
    {
        screenBuffer[pxrow] = 0;
    }

#endif
}
