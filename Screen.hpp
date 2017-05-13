#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <stdint.h>
#include "imgui/imgui.h"

#define BITBUFFER // this is to activate a space saving buffer that only used 1 bit per a 



#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

class Screen {

    public:

#ifndef BITBUFFER
    uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};
#else
    uint32_t screenBuffer[SCREEN_WIDTH * SCREEN_WIDTH / 32] = {0};
#endif
    uint8_t * current_font = 0;
    
    void drawBuffer();
    void clear();
};

#endif