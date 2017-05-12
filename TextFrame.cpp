
#include "TextFrame.hpp"
#include "string.h"

TextFrame::TextFrame() : ViewFrame() {

}

TextFrame::TextFrame(Point _size, Point _position) : ViewFrame(_size,_position) {
    setText("?");
}

TextFrame::TextFrame(Point _size, Point _position,const char *_text) : ViewFrame(_size,_position) {
    setText(_text);
}

void TextFrame::setSize(Point &_size) {

}

void TextFrame::setPosition(Point &_position) {

}

void TextFrame::setText(const char *_text) {
    static uint16_t size;
    size = strlen(_text) + 1;
    if (size < 32) {
        strncpy(text,_text,size);
    } else {
        strncpy(text,_text,31);
        text[31] = '\0';
    }
}

void TextFrame::draw() {
    
}

Point& TextFrame::getSize() {

}

Point& TextFrame::getPosition() {

}

TextFrame::~TextFrame() {

}