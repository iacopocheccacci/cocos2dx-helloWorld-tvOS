//
//  Eolo.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 29/05/15.
//
//

#ifndef __SOP__EOLO__
#define __SOP__EOLO__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"
#include "Observer.h"

namespace eoloDefs
{
    static const char* CLASS_NAME = "Eolo";
    static const char* NODE_NAME = "EoloNode";
    static const char* SPRITE_NAME = "EoloSprite";
    static const char* AIR_SPRITE_NAME = "AriaSprite";
    static const char* LOAD_ANIMATION = "load";
    static const char* BLOW_ANIMATION = "blow";
    static const char* STOP_ANIMATION = "stop";
    static const char* UNLOAD_ANIMATION = "unload";
    static const char* IDLE_ANIMATION = "idle";
    static const char* INFLATE_ANIMATION = "start";
//    static const char* BLOW_PARTICLE = "particles/particle_eolo_blow_b.plist";
}

class SoundComponent;
class AnimationComponent;

class Eolo : public LHNode, public Observer
{
public:
    NODEWITHDICT_FUNC(Eolo);
    virtual ~Eolo() {};
    
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    // Observer method
    virtual void update(eEventType event, Ref * sender, void * options);
    
private:
    void initAudio();
    
    void setIdleState();
    void setLoadingState();
    void setActiveState();
    void setUnloadState();
    
    void startUpDownMovement();
    void stopUpDownMovement();
    
    void startInflate();
    void stopInflate();
    
    SoundComponent * _soundLoad;
    SoundComponent * _soundBlow;
};

#endif // __SOP__EOLO__
