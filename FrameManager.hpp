
#ifndef FRAMEMANAGER_HPP
#define FRAMEMANAGER_HPP

#include "ViewFrame.hpp"
#include "TextFrame.hpp"
#include "DrawFrame.hpp"
#include "ScrollFrame.hpp"


typedef struct frame {
    enum {View_Frame, Text_Frame, Draw_Frame} type;
    ViewFrame vframe;
} frame_t;

class FrameManager {
    frame_t frames[32]; 
    frame_id next_frame_id;
public:
    FrameManager();
    bool addFrame(ViewFrame *frame);
    bool delFrame(ViewFrame *frame);
    void drawFrames(void);
};

#endif