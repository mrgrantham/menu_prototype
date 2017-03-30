///
/// draw.cpp
/// created by james grantham
/// 
/// this code contains simple implementation of several draw functions for use in a memory constrained
/// microcontroller driven LCD display

#include "draw.hpp"

uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};
uint8_t * current_font = 0;

void drawPixel(int32_t x,int32_t y, uint8_t color) {
    if(x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0) {
        screenBuffer[y][x] = color;
    }
}

void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    static int32_t xdiff;
    static int32_t ydiff;

    // get absolute value of x difference
    if (x1 > x2) {
        xdiff   = x1 - x2;
    } else {
        xdiff   = x2 - x1;
    }

    // get absolute value of y difference
    if (y1 > y2) {
        ydiff   = y1 - y2;
    } else {
        ydiff   = y2 - y1;
    }

    // ImGui::Text("XDIFF: %3d YDIFF: %3d",xdiff,ydiff);


    static int32_t yinc = 0;
    static int32_t xinc = 0;

    static int32_t yend = 0;
    static int32_t xend = 0;

    static int32_t ycomp = 0;
    static int32_t xcomp = 0;

    ycomp = 0;
    xcomp = 0;  

    // X is the longer traversal
    if (xdiff > ydiff) {
        //ycomp = ydiff - xdiff; 
        if (x1 < x2) {
            xinc = x1;
            yinc = y1;
            xend = x2;
            yend = y2;
        } else {
            xinc = x2;
            yinc = y2;
            xend = x1;
            yend = y1;            
        }
        // ImGui::Text("Xinc: %3d Yinc: %3d",xinc,yinc);
        // ImGui::Text("Xend: %3d Yend: %3d",xend,yend);       
        for (; xinc <= xend; xinc++ ) {
            ycomp += ydiff;
            // ImGui::Text("xinc: %3d yinc: %3d xdiff: %3d ydiff: %3d xcomp: %3d ycomp: %3d\n",xinc,yinc,xdiff,ydiff,xcomp,ycomp);
            if(ycomp >= xdiff) {
                if (yinc < yend) {
                    yinc++;
                } else {
                    yinc--;
                }
                ycomp -= xdiff;
            }
            drawPixel(xinc,yinc);
        }
    } else { // Y is the longer traversal
        //xcomp = xdiff - ydiff; 
        if (y1 < y2) {
            xinc = x1;
            yinc = y1;
            xend = x2;
            yend = y2;
        } else {
            xinc = x2;
            yinc = y2;
            xend = x1;
            yend = y1;            
        }
        // ImGui::Text("Xinc: %3d Yinc: %3d",xinc,yinc);
        // ImGui::Text("Xend: %3d Yend: %3d",xend,yend);      
        for (; yinc < yend; yinc++ ) {
            xcomp += xdiff;
            // ImGui::Text("xinc: %3d yinc: %3d xdiff: %3d ydiff: %3d xcomp: %3d ycomp: %3d\n",xinc,yinc,xdiff,ydiff,xcomp,ycomp);
            if(xcomp >= ydiff) {
                if (xinc < xend) {
                    xinc++;
                } else {
                    xinc--;
                }                
                xcomp -= ydiff;
            }
            drawPixel(xinc,yinc);
        }

    }

}

void drawRec(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    // x1y1 x2y1
    drawLine( x1, y1, x2, y1);
    // x1y1 x1y2
    drawLine( x1, y1, x1, y2);
    // x1y2 x2y2
    drawLine( x1, y2, x2, y2);
    // x2y1 x2y2
    drawLine( x2, y1, x2, y2);
}



void drawChar(char letter, uint16_t xpos, uint16_t ypos, uint16_t size){
    static uint16_t index;
    static uint16_t font_height = 8;
    static uint16_t font_width = 6;
    index = letter - 0x20;
    static uint16_t row = 0;
    static uint16_t col = 0;
    static uint16_t px = 0;
    static uint16_t py = 0;
    for (col = 0; col < font_width; col++ ) {
        for (row = 0; row < font_height; row++ ) {
            static uint8_t pixel;
            pixel = (current_font[(index * (font_width + 1)) + col] >> row) & 0x01;
            for (px = 0; px < size; px++) {
                for (py = 0; py < size; py++ ) {
                    drawPixel(xpos + (col * size) + px,ypos + (row * size) + py,pixel);
                }
            }
        }
    }
}

void print(char * line, uint16_t xpos, uint16_t ypos,uint16_t size) {
    static uint8_t font_width = 6;
    static uint16_t line_length;
    line_length  = strlen(line);
    for (uint16_t c = 0; c < line_length; c++) {
        drawChar(line[c], xpos + (font_width * c * size),ypos,size);
    }
}

void setFont(uint8_t *font){
    current_font = font;
}

void clearScreen()
{
    for (int32_t pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++)
    {
        for (int32_t pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++)
        {
            screenBuffer[pxrow][pxcol] = 0;
        }
    }
}
