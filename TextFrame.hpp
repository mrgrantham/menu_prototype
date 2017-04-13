
#ifndef TEXTFRAME_HPP
#define TEXTFRAME_HPP

#include "imgui/imgui.h"
#include "draw.hpp"
#include "ViewFrame.hpp"


class TextFrame : public ViewFrame {
    char text[32];
public:
    TextFrame();
    TextFrame(Point _size, Point _position);
    TextFrame(Point _size, Point _position,const char *_text);
    void setSize(Point &_size);
    void setPosition(Point &_position);
    void setPosition(int32_t x, int32_t y);
    void setX(int32_t x);
    void setY(int32_t y);
    void setText(const char *_text);
    Point& getSize();
    Point& getPosition();
    ~TextFrame();
};


#endif 