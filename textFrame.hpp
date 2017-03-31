
#ifndef TEXTFRAME_HPP
#define TEXTFRAME_HPP

#include "imgui/imgui.h"
#include "draw.hpp"

typedef uint32_t frame_id;

class TextFrame {
    frame_id id;
    Point size;
    Point position;
    bool visible;
    char text[32];
    anim_id anim;
public:
    TextFrame();
    TextFrame(Point _size, Point _position,const char *text);
    void setSize(Point &_size);
    void setPosition(Point &_position);
    void setPosition(int32_t x, int32_t y);
    void setX(int32_t x);
    void setY(int32_t y);
    void setText(char *_text);
    Point& getSize();
    Point& getPosition();
    ~TextFrame();
};


#endif 