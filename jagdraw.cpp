#include "jagdraw.hpp"
#include "imgui/imgui.h"
#include <stdint.h>
#include <stdio.h>
#include "fonts.h"
#include <math.h>
#include "animate.hpp"
#include "draw.hpp"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

static ImVec2 canvas_pos;            // ImDrawList current_API uses screen coordinates!
static ImVec2 canvas_size;       // Resize canvas to what's available
static ImDrawList* draw_list;
static int pixel_size = 2;

int32_t font_size=3;

static Animation *anim_obj;
d_array_t testdata[5];

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


void ShowAnimationDesignWindow(bool* p_open) {
    static bool first_run = true;

    static anim_params_t params[5];

    if (first_run) {
        anim_obj = Animation::getInstance();
        params[0] = anim_params(50, 0,  20, &Animation::animation1);
        params[1] = anim_params(50,0,20, &Animation::animation2);
        params[2] = anim_params(50,10,100, &Animation::animation3);
        params[3] = anim_params(50,0,100, &Animation::animation4);
        params[4] = anim_params(50,0,100, &Animation::animation5);
        for (uint16_t anim_index = 0; anim_index < 5; anim_index++) {
            testdata[anim_index].data = (float*)malloc(sizeof(float) * params[anim_index].duration);
            testdata[anim_index].size = params[anim_index].duration;
           anim_obj->setAnimationParams(params[anim_index]);
            for (uint16_t step = 0; step <  anim_obj->current_ap.duration; step++) {
                void (*anim)(anim_params_t &params);
                anim = anim_obj->current_ap.curve;
                anim(anim_obj->current_ap);
                testdata[anim_index].data[step] = (float)anim_obj->current_ap.output;
            }
        }
        first_run = false;
    }

    ImGui::SetNextWindowPos(ImVec2(35,350), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(450,0), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Animation Design", p_open))
    {
        ImGui::End();
        return;
    }


    // ImGui::Text("%3d ",testdata[3].size);
    // for (int x = 0; x < testdata[3].size; x++) {
    //     ImGui::Text("%3.2f ",testdata[0].data[x]); ImGui::SameLine();
    // }
    // ImGui::Text("---");
    for (int32_t dataindex = 0; dataindex < 5; dataindex++) {
        float diff = params[dataindex].end_val - params[dataindex].start_val;
        ImGui::PlotLines("Anim", testdata[dataindex].data, testdata[dataindex].size, 1.0f, "-----", (float)params[dataindex].start_val - (diff/10.0f), (float)params[dataindex].end_val + (diff/10.0f), ImVec2(0,100));

    }


    ImGui::End();
}



// Demonstrate using the low-level ImDrawList to draw custom shcurrent_apes. 
void ShowMenuPrototypeWindow(bool* p_open)
{
    static int32_t scroller = 150;
    static int32_t l1x1=0,l1y1=0,l1x2=0,l1y2=0,l2x1=0,l2y1=0,l2x2=0,l2y2=0;
    l1x1 = 80;
    l1y1 = scroller;
    l1x2 = 32;
    l1y2 = 47;
    
    l2x1 = 100;
    l2y1 = 200;
    l2x2 = 105;
    l2y2 = scroller;   

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
        if (ImGui::Button("Reset")) {
            points.clear();
            scroller = 150;
        }
        ImGui::SameLine();
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
        ImGui::SliderInt("POS SLIDER", &scroller, 0, 300);
        ImGui::SliderInt("FONT SIZE SLIDER", &font_size, 0, 10);

        ImGui::Text("scroller: %3d",scroller);

        // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
        // However you can draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
        // If you only use the ImDrawList current_API, you can notify the owner window of its extends by using SetCursorPos(max).
        canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList current_API uses screen coordinates!
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

        setFont((uint8_t*)&homespun_font);

        const char * test = "--TESTING--";
        static int16_t font_width = 6;
        print((char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

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
        // drawRec(margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));
        drawRec(margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 - (rec_height/2)+3);
        drawRec(margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3,SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));

        //drawRec(170,scroller,scroller,250);

        drawBuffer(pixel_size);

        if (adding_preview)
            points.pop_back();
    }
    ImGui::End();
}