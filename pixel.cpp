
#include "pixel.h"
#include <stdio.h>

static Point *maskCorners;

void setPixelMaskCorners(Point *mc) {
    maskCorners = mc;
}

bool inBounds(int16_t x, int16_t y) {
    if(x < SCREEN_WIDTH && x >= 0 && y < SCREEN_HEIGHT && y >= 0) {
        if (maskCorners == NULL || (x >= maskCorners[UpperLeft].x && y >= maskCorners[UpperLeft].y && x <= maskCorners[LowerRight].x && y <= maskCorners[LowerRight].y  )) {
            return true;
        }
    }
    static int32_t v=0;
    printf("out of bounds %d\n",v++);
    return false;
}

#ifdef SIMULATION

static ImVec2 *ds;
static int * px;

void setStart(ImVec2 *draw_start) {
    ds = draw_start;
}

void setPx(int *pixelSize) {
    px = pixelSize;
}


void drawDev(int16_t x, int16_t y,bool pixel) {
    if(inBounds( x, y )) {
        static ImDrawList* draw_list;
        static bool first_run = true;
        if (first_run) {
            draw_list = ImGui::GetWindowDrawList();
            first_run = false;
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


}
#else
    // SJONE specific code
#endif
