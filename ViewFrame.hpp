
#ifndef VIEW_FRAME_HPP
#define VIEW_FRAME_HPP

#include "draw.hpp"
#include "animate.hpp"

typedef uint32_t frame_id;

class ViewFrame {
    frame_id id;
    Point size;
    Point position;
    bool visible;
    char text[32];
    anim_id anim;
    
    public:
    ViewFrame();
    ViewFrame(Point _size, Point _position);
    //virtual void draw(void); // every frame must be able to draw itself

};

#endif
