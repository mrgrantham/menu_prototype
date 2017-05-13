///
/// draw.hpp
/// created by james grantham
/// 
/// this code contains simple implementation of several draw functions for use in a memory constrained
/// microcontroller driven LCD display

#ifndef DRAW_HPP
#define DRAW_HPP

#include <stdint.h>
#include "imgui/imgui.h"
#include "fonts.h"
#include "stdint.h"
#include "Screen.hpp"


typedef struct Point {
    uint32_t x;
    uint32_t y;
    Point() : x(0), y(0) {};
    Point(uint32_t x, uint32_t y) : x(x), y(y) {};
    Point(uint32_t value) : x(value), y(value) {};
    Point &operator=(Point &p) {this->x = p.x; this->y = p.y; return *this;}
} Point;

// extern uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH]; 
// extern uint8_t * current_font;

void drawLine( Screen *screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawRec( Screen *screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawChar( Screen *screen, char letter, uint16_t xpos, uint16_t ypos, uint16_t size = 1);
void print( Screen *screen, char * line, uint16_t xpos, uint16_t ypos,uint16_t size = 1);
void setFont( Screen *screen, uint8_t *font);
void clearScreen( Screen *screen);
// void drawBuffer( uint16_t pixel_size);



#endif