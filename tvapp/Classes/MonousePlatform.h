//
//  MonousePlatform.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 02/12/14.
//
//

#ifndef __SOP_MONOUSE_PLATFORM__
#define __SOP_MONOUSE_PLATFORM__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"

class AnimationComponent;
class SoundComponent;

namespace monousePlatformDefs
{
    static const char* CLASS_NAME = "MonousePlatform";
    static const char* SPRITE_0_NAME = "MonouseSprite";
    static const char* SPRITE_1_NAME = "MonouseSprite1";
    static const char* SPRITE_2_NAME = "MonouseSprite2";
    static const char* SPRITE_3_NAME = "MonouseSprite3";
    static const char* SPRITE_4_NAME = "MonouseSprite4";
}

USING_NS_CC;

class MonousePlatform : public LHShape
{
public:
    NODEWITHDICT_FUNC(MonousePlatform);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void startLifeTimer();
    
    void initAnimation(int worldNumber);
    
private:
    void initAudio();
    
    void lifeTimeScheduler(float dt);
    void animationTimeScheduler(float dt);
    void stopDropAnimation();
    void stopDropAnimationWithDelay();
        
    unsigned int _lifeTimer;
    unsigned int _animationTimer;
    
    bool _timersStarted;
    
    AnimationComponent * _animationFall;
    
    SoundComponent * _soundFall;

};

#endif // __SOP_MONOUSE_PLATFORM__
