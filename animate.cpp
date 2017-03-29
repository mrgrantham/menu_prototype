
#include "animate.hpp"
#include <math.h>


Animation *Animation::singleton = NULL;


Animation *Animation::getInstance() {
    if (!singleton) {   
        singleton = new Animation();
    }
    return singleton;
}


// assumption of animation running 60Hz
// each exectution of animate() function is 1 step in animation

// configure duration, start val, destination val
void Animation::setAnimationParams(int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &params)) {
    current_ap.duration = dur;
    current_ap.start_val = start;
    current_ap.end_val = end;
    current_ap.progress = start;
    current_ap.output = 0;
    current_ap.curve = curve;
}
void Animation::setAnimationParams(anim_params &_current_ap) {
    current_ap = _current_ap;
}

void Animation::resetAnimation(uint32_t animID) {
    
}

void Animation::animation1(anim_params_t &params) {
    params.output = 5.0f * sin(params.progress);
    if (params.progress != params.duration) {
        params.progress++;
    }
}

void Animation::animation2(anim_params_t &params) {
    params.output = pow(2,-params.progress) * 100.0f * (sin((float)params.progress * M_PI/(float)params.duration));
    if (params.progress != params.duration) {
        params.progress++;
    }
}

void Animation::animation3(anim_params_t &params) {
    params.output = 5.0f * sin(params.progress);
    if (params.progress != params.duration) {
        params.progress++;
    }
}

// cubic ease in
void Animation::animation4(anim_params_t &params) {
    static float anim_percent;    
    static float base_anim;

    anim_percent = (float)params.progress/(float)params.duration;
    base_anim = pow(anim_percent,3.0f);
    params.output  = params.start_val + base_anim * ( params.end_val - params.start_val );
    if (params.progress != params.duration) {
        params.progress++;
    }
}

void Animation::animation5(anim_params_t &params) {
    params.output = 5.0f * sin(params.progress);
    if (params.progress != params.duration) {
        params.progress++;
    }
}

void Animation::animate() {
    // increment all animation curves by 1 and marke those that are done as complete;
}