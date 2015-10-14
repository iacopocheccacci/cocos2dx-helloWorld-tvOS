//
//  Wolf.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#ifndef __SOP__WOLF__
#define __SOP__WOLF__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "WolfState.h"
#include "Definitions.h"
#include "Observer.h"

USING_NS_CC;

namespace wolfDefs
{
    static const char* CLASS_NAME = "Wolf";
    static const char* SPRITE_NAME = "WolfSprite";
    static const char* SPRITE_FILE_W4 = "lupo_Pan.png";
    static const char* SPRITE_FILE_W5 = "lupo_GROTTA_w5.png";
}

class SoundComponent;
class AnimationComponent;

class Wolf : public LHNode, public Observer
{
public:
    NODEWITHDICT_FUNC(Wolf);
    virtual ~Wolf();
    
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    void updateStatus(LHNodeProtocol* node, bool begin);
    
    // Observer method
    virtual void update(eEventType event, Ref * sender, void * options);
    
    int getActiveBrazierCounter()                   { return _activeBrazierCounter; }
    void incrementActiveBrazierCounter()            { ++_activeBrazierCounter; }
    void decrementActiveBrazierCounter()            { --_activeBrazierCounter; }

    size_t getSheepCounter()                           { return _sheepArray.count(); }
    
    void catchSheep();
    
    SoundComponent * getSoundWolfDiscovered()       { return _soundWolfDiscovered; }
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
    void deleteSheepFromArrayIfExist(LHNode* sheep);
    
private:
    
    void initAudio();
    void initAnimation();
    
    void startAnimationCatch(LHNode * catchedSheep);
    void playCatchSound();
    
    void updateSheepArray(LHNodeProtocol* node, bool begin);
    
    void safelyRemoveSheep(LHNode* sheep);
    
    WolfState* _currentState;
    int _activeBrazierCounter;
//    int _sheepCounter;
    __Array _sheepArray;
    
    bool _catching;
    
    SoundComponent * _soundWolfDiscovered;
    SoundComponent * _soundWolfCatch;

    AnimationComponent * _animationCatchFar;
    AnimationComponent * _animationCatchNear;
//    AnimationComponent * _animationCatch;
    
    Sprite * _spriteCatch;
    Sprite * _spriteCatchAnimation;
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
};

#endif // __SOP__WOLF__
