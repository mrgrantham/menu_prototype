#include "Screen.hpp"
#include "jagdraw.hpp"
#include <stdio.h>

void Screen::drawPixel(int32_t x,int32_t y, uint8_t color) {
    if(x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0) {
#ifndef BITBUFFER
        screenBuffer[y][x] = color;
#else
        static int16_t whichBit;
        static int16_t whichByte;
        static int16_t whichSubBit;

        whichBit = (y * SCREEN_HEIGHT) + x;
        whichByte = whichBit / 32; // whichBit << 5 might  be faster depending on compiler optimizations
        whichSubBit = whichBit % 32;


        screenBuffer[whichByte] = color ? (screenBuffer[whichByte] | (1<<whichSubBit)) :(screenBuffer[whichByte] & ~(1<<whichSubBit));
#endif
    }
}

void Screen::drawBuffer()
{
    static int32_t pxrow;
    static int32_t pxcol;
    for (pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
#ifndef BITBUFFER
            if (screenBuffer[pxrow][pxcol])
            {
                drawDev(pxrow, pxcol, true);
            }
            else
            {
                drawDev(pxrow, pxcol, false);
            }
#else
            static int16_t whichBit;
            static int16_t whichByte;
            static int16_t whichSubBit;

            whichBit = (pxrow * SCREEN_HEIGHT) + pxcol;
            whichByte = whichBit / 32; // whichBit << 5 might  be faster depending on compiler optimizations
            whichSubBit = whichBit % 32;
            printf("buffer: %d x: %3d y: %3d \n",sizeof(screenBuffer)),pxrow,pxcol;
            if ((screenBuffer[whichByte] >> whichSubBit) & 0x00000001)
            {
                drawDev(pxrow, pxcol, true);
            }
            else
            {
                drawDev(pxrow, pxcol, false);
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
