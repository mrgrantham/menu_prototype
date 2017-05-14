
#include "ViewFrame.hpp"
    // Screen *_screen;

ViewFrame::ViewFrame() {
    Point p;
    position = p;
}

ViewFrame::ViewFrame(Screen &screen) {
    Point p;
    Point s;

    position = p;
    size = s;
    _screen = &screen;

}

ViewFrame::ViewFrame(Point _size, Point _position) {
    size = _size;
    position = _position;
}

ViewFrame::ViewFrame(Screen &screen, Point _size, Point _position) {
    size = _size;
    position = _position;
    _screen = &screen;
}


void ViewFrame::setCorners() {
    corners[UpperLeft] = position;
    
    Point LL(position.x + size.x, position.y);
    corners[LowerLeft] = LL;

    Point UR(position.x, position.y + size.y);
    corners[UpperRight] = UR;

    Point LR(position.x + size.x, position.y + size.y);
    corners[LowerRight] = LR;

}

void ViewFrame::setMask(Point pos, Point sz) {
    mask[UpperLeft] = pos;

    Point LL(pos.x + sz.x, pos.y);
    mask[LowerLeft] = LL;

    Point UR(pos.x, pos.y + sz.y);
    mask[UpperRight] = UR;

    Point LR(pos.x + sz.x, pos.y + sz.y);
    mask[LowerRight] = LR;
}

void ViewFrame::setPosition(Point p) {
    position = p;
    setCorners();
}

Point &ViewFrame::getPosition() {
    return position;
}

void ViewFrame::setSize(Point s) {
    position = s;
    setCorners();
}

void ViewFrame::setScreen(Screen *screen) {
    _screen = screen;
}
