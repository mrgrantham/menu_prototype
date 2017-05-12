


#ifndef JAGDRAW_HPP
#define JAGDRAW_HPP

#include <stdint.h>


#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class Screen {

    public:
    uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};
    uint8_t * current_font = 0;
    void drawBuffer(uint16_t pixel_size);
};

void ShowAnimationDesignWindow(bool* p_open);
void ShowMenuPrototypeWindow(bool* p_open);
void ShowScrollTestWindow(bool * p_open);
    
#endif