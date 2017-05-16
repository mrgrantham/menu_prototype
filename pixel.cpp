
#include "pixel.h"

#ifdef SIMULATION


void setStart(ImVec2 *draw_start) {
    ds = draw_start;
}

void setPx(int *pixelSize) {
    px = pixelSize;
}


void drawDev(int16_t x, int16_t y,bool pixel) {

    static ImDrawList* draw_list;
    static bool first_run = true;
    if (first_run) {
        draw_list = ImGui::GetWindowDrawList();
    }
    
    static ImColor px_white = ImColor(IM_COL32_WHITE);
    static ImColor px_black = ImColor(IM_COL32_BLACK);
    static ImColor px_color;

    if (pixel) {
        px_color = px_white;
    } else {
        px_color = px_black;
    }

    static ImVec2 upper_left;
    static ImVec2 bottom_right;
    upper_left = ImVec2(ds->x + (x * (*px)),ds->y + (y * (*px)));
    bottom_right = ImVec2(ds->x + ((x+1) * (*px)) ,ds->y + ((y+1) * (*px))  );

    if ((*px) == 1)
    {
        draw_list->AddLine(upper_left, bottom_right, px_color, 1.0f);
    }
    else
    {
        draw_list->AddRectFilled(upper_left, bottom_right, px_color, 0.0f, ~0);
    }
}
#else
    // SJONE specific code
#endif