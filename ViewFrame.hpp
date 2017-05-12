
#ifndef VIEW_FRAME_HPP
#define VIEW_FRAME_HPP

#include "draw.hpp"
#include "animate.hpp"

typedef int32_t frame_id;

class ViewFrame {
    frame_id id;
    Point size;
    Point position;
    anim_id anim;
    
    public:
    bool active;
    ViewFrame();
    ViewFrame(Point _size, Point _position);
    void setScreen(Screen *screen);
    virtual void draw(void) = 0; // every frame must be able to draw itself

};

#endif
