
#ifndef VIEW_FRAME_HPP
#define VIEW_FRAME_HPP

#include "draw.hpp"
#include "animate.hpp"

typedef int32_t frame_id;

typedef enum {
    UpperLeft,
    LowerLeft,
    UpperRight,
    LowerRight
} Corners;

class ViewFrame {
    protected:
    frame_id id;
    Point size;     
    Point position; // absolute coordinates
    Point corners[4] = {0};
    Point mask[4];

    anim_id anim;
    Screen *_screen;
    void setCorners();

    public:
    bool active = true;
    ViewFrame();
    ViewFrame(Screen &screen);
    ViewFrame(Point _size, Point _position);
    ViewFrame(Screen &screen, Point _size, Point _position);
    void setMask(Point pos, Point size);
    void setSize(Point &_size);
    void setX(int32_t x);
    void setY(int32_t y);
    void setPosition(Point p);
    void setPosition(int32_t x, int32_t y);
    Point &getPosition();
    void setSize(Point s);
    void setScreen(Screen *screen);
    virtual void draw(void) = 0; // every frame must be able to draw itself

};

#endif
