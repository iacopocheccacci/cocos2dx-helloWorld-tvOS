//
//  SoundFrame.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 30/07/15.
//
//

#ifndef __SongOfPan__SoundFrame__
#define __SongOfPan__SoundFrame__

#include "SoundComponent.h"

class SoundVideo : public Node
{
public:
    
    static SoundVideo * createFromWorld(int world);
    bool init(int world);
    
    void playForFrame(int frame);
    void fadeOutAllEffects();
    
    void pauseAllEffects();
    void resumeAllEffects();
    
private:
    
    std::unordered_map<int, SoundComponent *> _effectsByFrame;
    
};

#endif /* defined(__SongOfPan__SoundFrame__) */
