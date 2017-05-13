#include "jagdraw.hpp"
#include "imgui/imgui.h"
#include <stdint.h>
#include <stdio.h>
#include "fonts.h"
#include <math.h>
#include "animate.hpp"
#include "draw.hpp"
#include <stdlib.h>

#include "FrameManager.hpp"
#include "TextFrame.hpp"
#include "DrawFrame.hpp"
#include "Screen.hpp"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


static ImDrawList* draw_list;
int32_t font_size=3;

static Animation *anim_obj;
d_array_t testdata[5];


ImVec2 canvas_pos;            // ImDrawList current_API uses screen coordinates!
ImVec2 canvas_size;       // Resize canvas to what's available
int pixel_size = 2;
static ImVec2 draw_start;

void drawPixel(int16_t x, int16_t y,bool pixel) {
    // draw border
    static ImVec2 border_upper_left;
    static ImVec2 border_bottom_right;
    static bool first_run = true;
    draw_list = ImGui::GetWindowDrawList();
    if (first_run) {


        border_upper_left.x = draw_start.x - 1;
        border_upper_left.y = draw_start.y - 1; 
        border_bottom_right.x = draw_start.x + (pixel_size * SCREEN_WIDTH) + 1;
        border_bottom_right.y = draw_start.y + (pixel_size * SCREEN_HEIGHT) + 1; 

        draw_list->AddRect(border_upper_left, border_bottom_right, ImColor(0,0,250), 0.0f, ~0, 1.0f);
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
    upper_left = ImVec2(draw_start.x + (y * pixel_size),draw_start.y + (x * pixel_size));
    bottom_right = ImVec2(draw_start.x + ((y+1) * pixel_size) ,draw_start.y + ((x+1) * pixel_size)  );

    if (pixel_size == 1)
    {
        draw_list->AddLine(upper_left, bottom_right, px_color, 1.0f);
    }
    else
    {
        draw_list->AddRectFilled(upper_left, bottom_right, px_color, 0.0f, ~0);
    }
}

void ShowAnimationDesignWindow(bool* p_open) {
    static bool first_run = true;
    static anim_id params[5];
    if (first_run) {
        anim_obj = Animation::getInstance();
        params[0] = anim_obj->setAnimationState(50,0,100, &Animation::animation1);
        params[1] = anim_obj->setAnimationState(50,0,100, &Animation::animation2);
        params[2] = anim_obj->setAnimationState(50,0,100, &Animation::animation3);
        params[3] = anim_obj->setAnimationState(50,0,100, &Animation::animation4);
        params[4] = anim_obj->setAnimationState(50,0,100, &Animation::animation5);
        for (uint16_t anim_index = 0; anim_index < 5; anim_index++) {
            anim_params_t * animation_state = anim_obj->getAnimationState(params[anim_index]);
            testdata[anim_index].data = (float*)malloc(sizeof(float) * animation_state->duration);
            testdata[anim_index].size = animation_state->duration;
            for (uint16_t step = 0; step <  animation_state->duration; step++) {
                void (*anim)(anim_params_t &params);
                anim = animation_state->curve;
                anim(*animation_state);
                testdata[anim_index].data[step] = (float)animation_state->output;
            }
        }
        // get id for 
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
        float diff = anim_obj->getAnimationState(params[dataindex])->end_val - anim_obj->getAnimationState(params[dataindex])->start_val;
        ImGui::PlotLines("Anim", testdata[dataindex].data, testdata[dataindex].size, 1.0f, "-----", (float)anim_obj->getAnimationState(params[dataindex])->start_val - (diff/3.0f), (float)anim_obj->getAnimationState(params[dataindex])->end_val + (diff/3.0f), ImVec2(0,110));

    }


    ImGui::End();
}



// Demonstrate using the low-level ImDrawList to draw custom shcurrent_apes. 
void ShowMenuPrototypeWindow(bool* p_open)
{
    static bool first_run = true;
    static anim_id test_animation;
    static anim_params_t *scroller_state;
    if (first_run) {
        anim_obj = Animation::getInstance();
        test_animation = anim_obj->setAnimationState(50,0,150, &Animation::animation2);
        first_run = false;
        scroller_state = anim_obj->getAnimationState(test_animation);

    }
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
            anim_obj->resetAnimation(test_animation);
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

        draw_start.x = canvas_pos.x + ( (canvas_size.x - (pixel_size * SCREEN_WIDTH)) / 2 );
        draw_start.y = canvas_pos.y + ( (canvas_size.y - (pixel_size * SCREEN_HEIGHT)) / 2 );
        
        // ---------- BEGIN Screen Simulation Draw Routines ------------ //

        static Screen *mainScreen = new Screen();

        mainScreen->clear();
        setFont(mainScreen,(uint8_t*)&homespun_font);

        const char * test = "--TESTING--";
        static int16_t font_width = 6;
        if(!scroller_state->complete) {
            printf("EIO: prog: %3d dur: %3d st: %3d end: %3d output: %3d",  scroller_state->progress,
                                                                            scroller_state->duration,
                                                                            scroller_state->start_val,
                                                                            scroller_state->end_val,
                                                                            scroller_state->output);
            printf(" scroller: %3d\n",scroller);
            anim_obj->animate(test_animation);
            scroller = scroller_state->output;
        }
        print(mainScreen,(char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

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
        drawRec(mainScreen,margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 - (rec_height/2)+3);
        drawRec(mainScreen,margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3,SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));

        //drawRec(170,scroller,scroller,250);
        mainScreen->drawBuffer();

        // ---------- END Screen Simulation Draw Routines ------------ //


        if (adding_preview)
            points.pop_back();
    }
    ImGui::End();
}

void ShowScrollTestWindow(bool * p_open) {

    // sets vertical positioning of items in  frame
    static int32_t scroller = 150;

    static bool first_run = true;
    static anim_id test_animation;
    static anim_params_t *scroller_state;
    if (first_run) {
        anim_obj = Animation::getInstance();
        test_animation = anim_obj->setAnimationState(50,0,150, &Animation::animation2);
        first_run = false;
        scroller_state = anim_obj->getAnimationState(test_animation);

    }

    ImGui::SetNextWindowPos(ImVec2(530,35), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(700,800), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Menu Scroll", p_open))
    {
        ImGui::End();
        return;
    }

    draw_list = ImGui::GetWindowDrawList();

    ImGui::Text("MENU DESIGN");
    if (ImGui::Button("Reset")) {
        scroller = 150;
        anim_obj->resetAnimation(test_animation);
    }
    ImGui::SameLine;
    ImGui::SliderInt("POS SLIDER", &scroller, 0, 300);

    canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList current_API uses screen coordinates!
    canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available

    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f; // ensure the canvas size is at least 50 wide by 50 tall
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
    draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0));
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255,255,255));

    draw_start.x = canvas_pos.x + ( (canvas_size.x - (pixel_size * SCREEN_WIDTH)) / 2 );
    draw_start.y = canvas_pos.y + ( (canvas_size.y - (pixel_size * SCREEN_HEIGHT)) / 2 );

    // ----------------------------
    // Platform Independant Drawing
    // ----------------------------
    {
        static FrameManager *manager;
        static Screen *mainScreen;
        static bool first_time = true;

        if (first_time) {
            mainScreen = new Screen();
            manager = new FrameManager(mainScreen);
            setFont(mainScreen,(uint8_t*)&homespun_font);
            DrawFrame *rec1 = new DrawFrame();
            DrawFrame *rec2 = new DrawFrame();
            TextFrame *txt = new TextFrame(Point(200,50),Point(20,20),"Frame");
            manager->addFrame((ViewFrame*)rec1);
            manager->addFrame((ViewFrame*)rec2);
            manager->addFrame((ViewFrame*)txt);

            first_time = false;
        }

        mainScreen->clear();

        static const char * test = "--TESTING--";
        static int16_t font_width = 6;

        print(mainScreen,(char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

        static int32_t margin = 15;
        static int32_t rec_height = 50;
        drawRec(mainScreen, margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 - (rec_height/2)+3);
        drawRec(mainScreen,margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3,SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));

        mainScreen->drawBuffer();

    }
    // ----------------------------
    // End  Drawing
    // ----------------------------



    ImGui::End();


}