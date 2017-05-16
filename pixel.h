
#ifndef PIXEL_H
#define PIXEL_H

#define SIMULATION

#include <stdint.h>
#include "Point.hpp"
#ifdef SIMULATION

#include "imgui/imgui.h"

void setPixelMaskCorners(Point *mc);
void setStart(ImVec2 *draw_start);
void setPx(int *pixelSize) ;
void drawDev(int16_t x, int16_t y,bool pixel) ;
#else
    // SJONE specific code
#endif

#endif