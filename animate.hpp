
#ifndef ANIMATE_HPP
#define ANIMATE_HPP

#include <iostream>

using namespace std;

typedef struct anim_params{
    int32_t duration;
    int32_t start_val;
    int32_t end_val;
    int32_t progress;
    int32_t output;
    void (* curve)(anim_params &params);
    anim_params() { duration = 0; start_val = 0; end_val = 0; progress = 0; curve = NULL;}
    anim_params(int32_t dur, int32_t start,int32_t end,void (* crv)(anim_params &params)) { duration = dur; start_val = start; end_val = end; progress = start; curve = crv; }
} anim_params_t;

typedef struct d_array {
    float *data;
    uint16_t size;
} d_array_t;


class Animation {
    static Animation *singleton;
    Animation() {
        cout << "animated" << endl;
    }
    anim_params_t animation_list[16];

    public:

    anim_params_t current_ap;
    static Animation *getInstance();
    void setAnimationParams(int32_t dur, int32_t start, int32_t end, void (*curve)(anim_params_t &params));
    void setAnimationParams(anim_params &_current_ap);
    void resetAnimation(uint32_t animID);
    static void animation1(anim_params_t &params);
    static void animation2(anim_params_t &params);
    static void animation3(anim_params_t &params);
    static void animation4(anim_params_t &params);
    static void animation5(anim_params_t &params);
    void animate();


};


#endif
