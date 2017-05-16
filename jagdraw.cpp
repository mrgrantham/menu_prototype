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
#include "NumberFrame.hpp"
#include "Screen.hpp"
#include "pixel.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


static ImDrawList* draw_list;
int32_t font_size=3;

static Animation *anim_obj;
d_array_t testdata[5];


ImVec2 canvas_pos;            // ImDrawList current_API uses screen coordinates!
ImVec2 canvas_size;       // Resize canvas to what's available
int pixel_size = 2;
static ImVec2 draw_start;

void ShowAnimationDesignWindow(bool* p_open) {
    static bool first_run = true;
    static anim_id params[5];

    setStart(&draw_start);

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
    // draw border
    static ImVec2 border_upper_left;
    static ImVec2 border_bottom_right;



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
    // static int32_t l1x1=0,l1y1=0,l1x2=0,l1y2=0,l2x1=0,l2y1=0,l2x2=0,l2y2=0;
    // l1x1 = 80;
    // l1y1 = scroller;
    // l1x2 = 32;
    // l1y2 = 47;
    
    // l2x1 = 100;
    // l2y1 = 200;
    // l2x2 = 105;
    // l2y2 = scroller;   

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
        
        border_upper_left.x = draw_start.x - 1;
        border_upper_left.y = draw_start.y - 1; 
        border_bottom_right.x = draw_start.x + (pixel_size * SCREEN_WIDTH) + 1;
        border_bottom_right.y = draw_start.y + (pixel_size * SCREEN_HEIGHT) + 1; 
        draw_list->AddRect(border_upper_left, border_bottom_right, ImColor(0,0,250), 0.0f, ~0, 1.0f);

        // ---------- BEGIN Screen Simulation Draw Routines ------------ //

        static Screen *mainScreen = new Screen();

        mainScreen->clear();
        setFont(mainScreen,(uint8_t*)&homespun_font);

        const char * test = "--TESTING--";
        static int16_t font_width = 6;
        if(!scroller_state->complete) {
            // printf("EIO: prog: %3d dur: %3d st: %3d end: %3d output: %3d",  scroller_state->progress,
            //                                                                 scroller_state->duration,
            //                                                                 scroller_state->start_val,
            //                                                                 scroller_state->end_val,
            //                                                                 scroller_state->output);
            // printf(" scroller: %3d\n",scroller);
            anim_obj->animate(test_animation);
            scroller = scroller_state->output;
        }
        print(mainScreen,(char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

    //    drawLine(40,40,50,50);
    //     drawDev(l1x1,l1y1);
    //     drawDev(l1x2,l1y2);
    //     drawLine(l1x1,l1y1,l1x2,l1y2);

    //     drawDev(l2x1,l2y1);
    //     drawDev(l2x2,l2y2);
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

    setStart(&draw_start);
    setPx(&pixel_size);
    // draw border
    static ImVec2 border_upper_left;
    static ImVec2 border_bottom_right;
    
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
    ImGui::SameLine();
    ImGui::SliderInt("POS SLIDER", &scroller, 0, 300);

    canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList current_API uses screen coordinates!
    canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available

    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f; // ensure the canvas size is at least 50 wide by 50 tall
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
    draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0), ImColor(0,0,0));
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255,255,255));

    draw_start.x = canvas_pos.x + ( (canvas_size.x - (pixel_size * SCREEN_WIDTH)) / 2 );
    draw_start.y = canvas_pos.y + ( (canvas_size.y - (pixel_size * SCREEN_HEIGHT)) / 2 );
    border_upper_left.x = draw_start.x - 1;
    border_upper_left.y = draw_start.y - 1; 
    border_bottom_right.x = draw_start.x + (pixel_size * SCREEN_WIDTH) + 1;
    border_bottom_right.y = draw_start.y + (pixel_size * SCREEN_HEIGHT) + 1; 
    draw_list->AddRect(border_upper_left, border_bottom_right, ImColor(0,0,250), 0.0f, ~0, 1.0f);
    // ----------------------------
    // Platform Independant Drawing
    // ----------------------------
    {
        static FrameManager *manager;
        static Screen *mainScreen;
        static bool first_time = true;

        static TextFrame *nameFrame;
        static TextFrame *testFrame;
        static DrawFrame *rec1;
        static int32_t *border1params;
        static int32_t *border2params;

        static NumberFrame *numFrame;


        static const char * bname = "<eBoard>";


        static DrawFrame *battery;
        static int32_t *tip;
        static int32_t *bl1;
        static int32_t *bl2;
        static int32_t *bl3;
        static int32_t *bl4;
        static int32_t *box;

        static const char * test = "--TESTING--";
        static int16_t font_width = 6;
        static Point *testFramePos;

        static int32_t margin = 15;
        static int32_t rec_height = 50;

        if (first_time) {
            mainScreen = new Screen();
            manager = new FrameManager(mainScreen);
            setFont(mainScreen,(uint8_t*)&homespun_font);

            border1params = (int32_t*)malloc(4 * sizeof(int32_t));
            border1params[0] = margin;
            border1params[1] = 0;
            border1params[2] = SCREEN_WIDTH - margin;
            border1params[3] = 3;

            border2params = (int32_t*)malloc(4 * sizeof(int32_t));
            border2params[0] = margin;
            border2params[1] = rec_height - 3;
            border2params[2] = SCREEN_WIDTH - margin;
            border2params[3] = rec_height;

            rec1 = new DrawFrame(Point(240,3),Point(0,SCREEN_HEIGHT/2 - (rec_height/2)));
            rec1->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

            rec1->addDrawing(d_rect,border1params);
            rec1->addDrawing(d_rect,border2params);

            numFrame = new NumberFrame(Point(50,50),Point(SCREEN_WIDTH/2 + 70,150),6);

            battery = new DrawFrame(Point(40,20),Point(200,10));

            tip = (int32_t*)malloc(4 * sizeof(int32_t));
            tip[0] = 1;
            tip[1] = 17;
            tip[2] = 3;
            tip[3] = 22;

            bl1 = (int32_t*)malloc(4 * sizeof(int32_t));
            bl1[0] = 5;
            bl1[1] = 14;
            bl1[2] = 7;
            bl1[3] = 25;


            bl2 = (int32_t*)malloc(4 * sizeof(int32_t));
            bl2[0] = 9;
            bl2[1] = 14;
            bl2[2] = 11;
            bl2[3] = 25;

            bl3 = (int32_t*)malloc(4 * sizeof(int32_t));
            bl3[0] = 13;
            bl3[1] = 14;
            bl3[2] = 15;
            bl3[3] = 25;

            bl4 = (int32_t*)malloc(4 * sizeof(int32_t));
            bl4[0] = 17;
            bl4[1] = 14;
            bl4[2] = 19;
            bl4[3] = 25;

            box = (int32_t*)malloc(4 * sizeof(int32_t));
            box[0] = 3;
            box[1] = 12;
            box[2] = 21;
            box[3] = 27;

            battery->addDrawing(d_rect,tip);
            battery->addDrawing(d_rect,bl1);
            battery->addDrawing(d_rect,bl2);
            battery->addDrawing(d_rect,bl3);
            battery->addDrawing(d_rect,bl4);
            battery->addDrawing(d_rect,box);

            // rec2 = new DrawFrame(Point(240,3),Point(margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3));
            // rec2->setMask(Point(0,0),Point(SCREEN_WIDTH,SCREEN_HEIGHT));

            testFramePos = new Point();
            testFrame = new TextFrame(Point(200,50),*testFramePos,test,3);
            testFrame->setMask(Point(0,135),Point(240,50));
            nameFrame = new TextFrame(Point(200,50),Point(30,280),bname,4);
            TextFrame *txt = new TextFrame(Point(200,50),Point(20,20),"BOARD CONNECTION: ACTIVE",1);
            TextFrame *txt2 = new TextFrame(Point(200,50),Point(20,40),"OP. TIME ELAPSED: XX:XX",1);
            TextFrame *txt3 = new TextFrame(Point(200,50),Point(20,60),"EST. TIME REMAINING: XX:XX",1);

            manager->addFrame(nameFrame);
            manager->addFrame(testFrame);
            manager->addFrame((ViewFrame*)rec1);
            manager->addFrame((ViewFrame*)battery);
            manager->addFrame((ViewFrame*)txt);
            manager->addFrame((ViewFrame*)txt2);
            manager->addFrame((ViewFrame*)txt3);

            first_time = false;
        }

        mainScreen->clear();

        testFramePos->x = SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width);
        testFramePos->y = scroller;
        testFrame->setPosition(*testFramePos);
        // Point currentPOS;
        // currentPOS = testFrame->getPosition();
        // printf("testFrame position: %d %d\n", currentPOS.x, currentPOS.y);
        // print(mainScreen,(char *)test,SCREEN_WIDTH/2 - (strlen(test)* font_size/2 * font_width),scroller,font_size);

        // drawRec(mainScreen, margin,SCREEN_HEIGHT/2 - (rec_height/2),SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 - (rec_height/2)+3);
        // drawRec(mainScreen,margin, SCREEN_HEIGHT/2 + (rec_height/2) - 3,SCREEN_WIDTH - margin,SCREEN_HEIGHT/2 + (rec_height/2));

        manager->drawFrames();
        // mainScreen->drawBuffer();

    }
    // ----------------------------
    // End  Drawing
    // ----------------------------



    ImGui::End();


}