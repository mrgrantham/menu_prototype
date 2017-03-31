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

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 128

typedef struct Point {
    uint32_t x;
    uint32_t y;
    Point() : x(0), y(0) {};
    Point(uint32_t x, uint32_t y) : x(x), y(y) {};
    Point(uint32_t value) : x(value), y(value) {};
    Point &operator=(Point &p) {this->x = p.x; this->y = p.y; return *this;}
} Point;

extern uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH]; 
extern uint8_t * current_font;

void drawPixel(int32_t x,int32_t y, uint8_t color = 1);
void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawRec(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void drawChar(char letter, uint16_t xpos, uint16_t ypos, uint16_t size = 1);
void print(char * line, uint16_t xpos, uint16_t ypos,uint16_t size = 1);
void setFont(uint8_t *font);
void clearScreen();
// void drawBuffer( uint16_t pixel_size);



#endif