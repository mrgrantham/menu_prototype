
#include "ViewFrame.hpp"
    // Screen *_screen;

ViewFrame::ViewFrame() {

}

ViewFrame::ViewFrame(Point _size, Point _position) {
    size = _size;
    position = _position;
}

void ViewFrame::setScreen(Screen *screen) {
    _screen = screen;
}
