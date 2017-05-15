
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

ViewFrame::ViewFrame(Point _size, Point _position) : position(), size(), corners(),maskSize(),maskPosition() {
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
    maskCorners[UpperLeft] = pos;

    Point LL(pos.x + sz.x, pos.y);
    maskCorners[LowerLeft] = LL;

    Point UR(pos.x, pos.y + sz.y);
    maskCorners[UpperRight] = UR;

    Point LR(pos.x + sz.x, pos.y + sz.y);
    maskCorners[LowerRight] = LR;

    maskPosition = pos;
    maskSize = sz;
}

Point& ViewFrame::getMaskPosition() {
    return maskPosition;
}

Point& ViewFrame::getMaskSize() {
    return maskSize;
}

Point * ViewFrame::getMaskCorners() {
    return maskCorners;
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
