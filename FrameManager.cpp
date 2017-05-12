
#include "FrameManager.hpp"


    FrameManager::FrameManager() {

    }    
    
    frame_id FrameManager::addFrame(ViewFrame *frame) {
        frame_id id = next_frame_id;
        frames[id] = frame;
        return id;
    }

    bool FrameManager::delFrame(frame_id id) {
        if (id < next_frame_id) {
            if(frames[id] != NULL) {
                delete frames[id];
                frames[id] = NULL;
                return true;
            } else {
                printf("NULL frame\n");
                return false;
            }
        }
        return false;
    }

    bool FrameManager::activateFrame(frame_id id) {
        frames[id]->active = true;
        return true;
    }
    bool FrameManager::deactivateFrame(frame_id id) {
        frames[id]->active = true;  
        return true;
    }

    void FrameManager::drawFrames(void) {
        static uint32_t frame_index;
        for (frame_index = 0; frame_index < next_frame_id; frame_index++) {
            if (frames[frame_index]->active) {
                frames[frame_index]->draw();
            }
        }

    }