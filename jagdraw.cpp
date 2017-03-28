#include "jagdraw.hpp"
#include "imgui/imgui.h"
#include <stdint.h>
#include <stdio.h>
#include "fonts.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

static ImVec2 canvas_pos;            // ImDrawList API uses screen coordinates!
static ImVec2 canvas_size;       // Resize canvas to what's available
static ImDrawList* draw_list;
static int pixel_size = 2;

static void* current_animation;
typedef struct anim_params{
    int32_t duration;
    int32_t start_val;
    int32_t end_val;
    int32_t progress;
    anim_params() { duration = 0; start_val = 0; end_val = 0; progress = 0; }
    anim_params(int32_t dur, int32_t start,int32_t end,int32_t prog) { duration = dur; start_val = start; end_val = end; progress = prog; }
} anim_params_t;

anim_params_t ap;

// assumption of animation running 60Hz
// each exectution of animate() function is 1 step in animation
uint16_t animate() {

}

// configure duration, start val, destination val
void setAnimationParams(int32_t dur, int32_t start, int32_t end) {
    ap.duration = dur;
    ap.start_val = start;
    ap.end_val = end;
    ap.progress = 0;
}
void setAnimationParams(anim_params &_ap) {
    ap = _ap;
}

void setAnimationCurve(void * curve) {
    current_animation = curve;
}

float testdata[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,7.0f,7.0f,6.0f,6.0f,5.0f,5.0f,6.0f,6.0f,9.0f};

void animation1() {

}

void ShowAnimationDesignWindow(bool* p_open) {
    static bool first_run = true;

    static anim_params_t params[5];



    if (first_run) {
        params[0] = anim_params(50,0,20,0);
        params[1] = anim_params(100,75,120,0);
        params[2] = anim_params(25,10,20,0);
        params[3] = anim_params(75,40,70,0);
        params[4] = anim_params(50,10,50,0);
        first_run = false;
    }

    ImGui::SetNextWindowPos(ImVec2(35,350), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(450,0), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Animation Design", p_open))
    {
        ImGui::End();
        return;
    }

    for (int32_t dataindex = 0; dataindex < 5; dataindex++) {
        ImGui::PlotLines("Anim", testdata, IM_ARRAYSIZE(testdata), 0, "avg 0.0", -2.0f, 10.0f, ImVec2(0,100));
    }


    ImGui::End();
}


uint8_t screenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};

void drawPixel(int32_t x,int32_t y, uint8_t color = 1) {
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

uint8_t * current_font;
int32_t font_size=3;

void drawChar(char letter, uint16_t xpos, uint16_t ypos, uint16_t size = 1){
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

void print(char * line, uint16_t xpos, uint16_t ypos,uint16_t size = 1) {
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

void drawBuffer( uint16_t pixel_size) {
    static ImVec2 draw_start;
    draw_start.x = canvas_pos.x + ( (canvas_size.x - (pixel_size * SCREEN_WIDTH)) / 2 );
    draw_start.y = canvas_pos.y + ( (canvas_size.y - (pixel_size * SCREEN_HEIGHT)) / 2 );
    // draw border
    static ImVec2 border_upper_left;
    static ImVec2 border_bottom_right;

    border_upper_left.x = draw_start.x - 1;
    border_upper_left.y = draw_start.y - 1; 
    border_bottom_right.x = draw_start.x + (pixel_size * SCREEN_WIDTH) + 1;
    border_bottom_right.y = draw_start.y + (pixel_size * SCREEN_HEIGHT) + 1; 

    draw_list->AddRect(border_upper_left, border_bottom_right, ImColor(0,0,250), 0.0f, ~0, 1.0f);

    for ( int32_t pxrow = 0; pxrow < SCREEN_HEIGHT; pxrow++ ) {
        for ( int32_t pxcol = 0; pxcol < SCREEN_WIDTH; pxcol++ ) {
            static ImColor px_color = ImColor(IM_COL32_WHITE);
            ImVec2 upper_left = ImVec2(draw_start.x + (pxcol * pixel_size),draw_start.y + (pxrow * pixel_size));
            ImVec2 bottom_right = ImVec2(draw_start.x + ((pxcol+1) * pixel_size) ,draw_start.y + ((pxrow+1) * pixel_size)  );
            //printf("       \n----------------\ndraw start x:%4f y:%4f \ncanvas size x:%4f y:%4f \ncanvas pos x:%3f y:%3f  \ndpxrow: %3d pxcol: %d\n", draw_start.x,draw_start.y,canvas_size.x,canvas_size.y,canvas_pos.x, canvas_pos.y,pxrow,pxcol);      
            if(screenBuffer[pxrow][pxcol]) {
                px_color = ImColor(IM_COL32_WHITE);
                // printf("white pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            } else {
                px_color = ImColor(IM_COL32_BLACK);
                // printf("black pixel at x:%4f y:%4f to x:%4f y:%4f\n",upper_left.x,upper_left.y,bottom_right.x,bottom_right.y);
            }
            if(pixel_size == 1) {
                draw_list->AddLine(upper_left,bottom_right, px_color, 1.0f);
            } else {
                draw_list->AddRectFilled(upper_left,bottom_right, px_color,0.0f, ~0);
            }
           
        }
    }
}

// Demonstrate using the low-level ImDrawList to draw custom shapes. 
void ShowMenuPrototypeWindow(bool* p_open)
{
    static int32_t spinner = 205;
    static int32_t l1x1=0,l1y1=0,l1x2=0,l1y2=0,l2x1=0,l2y1=0,l2x2=0,l2y2=0;
    l1x1 = 80;
    l1y1 = spinner;
    l1x2 = 32;
    l1y2 = 47;
    
    l2x1 = 100;
    l2y1 = 200;
    l2x2 = 105;
    l2y2 = spinner;   

    ImGui::SetNextWindowPos(ImVec2(530,35), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(700,800), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Menu Prototype", p_open))
    {
        ImGui::End();
        return;
    }

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc.
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4.
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types)
    // In this example we are not using the maths operators!
    draw_list = ImGui::GetWindowDrawList();
    {
        static ImVector<ImVec2> points;
        static bool adding_line = false;
        ImGui::Text("MENU DESIGN");
        if (ImGui::Button("Reset")) points.clear();ImGui::SameLine();
        if (pixel_size == 1) {
            if (ImGui::Button("Set 2X")) pixel_size = 2;
            ImGui::SameLine();
            ImGui::Text("\tCurrent Size: 1X");
        } else if (pixel_size == 2) {
            if (ImGui::Button("Set 1X")) pixel_size = 1;
            ImGui::SameLine();
            ImGui::Text("\tCurrent Size: 2X");
        } else {
            ImGui::Button("Huh?");
            ImGui::SameLine();
            ImGui::Text("\tCurrent Size: Dunno");
        }
        ImGui::SliderInt("POS SLIDER", &spinner, 0, 300);
        ImGui::SliderInt("FONT SIZE SLIDER", &font_size, 0, 10);

        ImGui::Text("Spinner: %3d",spinner);

        // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
        // However you can draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
        // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
        canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
        canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
        //printf("       \n----------------\n\ncanvas size x:%4f y:%4f \ncanvas pos x:%3f y:%3f  \n", canvas_size.x,canvas_size.y,canvas_pos.x, canvas_pos.y);      

        if (canvas_size.x < 50.0f) canvas_size.x = 50.0f; // ensure the canvas size is at least 50 wide by 50 tall
        if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
        draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0));
        draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255,255,255));

        bool adding_preview = false;
        ImGui::InvisibleButton("canvas", canvas_size);
        ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);
        if (adding_line)
        {
            adding_preview = true;
            points.push_back(mouse_pos_in_canvas);
            if (!ImGui::GetIO().MouseDown[0])
                adding_line = adding_preview = false;
        }
        if (ImGui::IsItemHovered())
        {
            if (!adding_line && ImGui::IsMouseClicked(0))
            {
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (ImGui::IsMouseClicked(1) && !points.empty())
            {
                adding_line = adding_preview = false;
                points.pop_back();
                points.pop_back();
            }
        }
        draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x+canvas_size.x, canvas_pos.y+canvas_size.y));      // clip lines within the canvas (if we resize it, etc.)
        for (int i = 0; i < points.Size - 1; i += 2)
            draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i+1].x, canvas_pos.y + points[i+1].y), IM_COL32(255,255,0,255), 2.0f);
        draw_list->PopClipRect();
        clearScreen();
        // drawPixel(20,50);
        // drawPixel(120,160);
        setFont((uint8_t*)&homespun_font);
        // drawChar(' ',100,150);
        // drawChar('!',110,150);
        // drawChar('"',120,150);
        // drawChar('#',130,150);
        // drawChar('$',140,150);
        // drawChar('%',150,150);

        static char * test = "--TESTING--";
        static int16_t font_width = 6;
        print(test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),spinner,font_size);

    //    drawLine(40,40,50,50);
    //     drawPixel(l1x1,l1y1);
    //     drawPixel(l1x2,l1y2);
    //     drawLine(l1x1,l1y1,l1x2,l1y2);

    //     drawPixel(l2x1,l2y1);
    //     drawPixel(l2x2,l2y2);
    //     //drawLine(l2x1,l2y1,l2x2,l2y2);

    //     drawLine(l2x2,l2y2,l2x1,l2y1);

        static int32_t margin = 15;
        static int32_t rec_height = 50;
        drawRec(margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));
        //drawRec(170,spinner,spinner,250);

        drawBuffer(pixel_size);

        if (adding_preview)
            points.pop_back();
    }
    ImGui::End();
}