//
//  Brazier.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 03/12/14.
//
//

#ifndef __SOP_BRAZIER__
#define __SOP_BRAZIER__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"
#include "Observer.h"

USING_NS_CC;

namespace brazierDefs
{
    static const char* CLASS_NAME = "Brazier";
    static const char* BRAZIER_SHAPE = "brazierShape";
    static const char* FIRE_NODE = "fireNode";
    static const char* FIRE_SPRITE = "fireSprite";
    static const char* PARTICLE_SMOKE = "particle_brazier_smoke.plist";
}

enum class eBrazierStatus :int
{
    BRAZIER_ON = 0,
    BRAZIER_OFF,
};

enum class eBrazierType;

class Goatskin;

class AnimationComponent;
class SoundComponent;

class Brazier : public LHShape
{
public:
    
    virtual ~Brazier();
    NODEWITHDICT_FUNC(Brazier);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    eBrazierStatus getBrazierStatus()               { return _brazierStatus; }
    void blockEffectOnFire();
    void TurnOffIfInWindRange(Goatskin* source);
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
    void initAudio(LHNode * player);
    void initAnimation();
    
    void setUnderEoloEffect();
    
private:
    
    void initParticleSmoke(Node * parentNode);
    
    void turnOffFire();
    void update(float dt);
    void updateAudio(float dt);
    void updateParticleSmoke(float dt);
    
    void removeAllObservers();
    void notify(eEventType event);
    
    void handleTurnOffByWind(LHSprite* sprite);
    void handleTurnOffByBlock(LHSprite* sprite);
    
    eBrazierType getType();
    
    eBrazierStatus _brazierStatus;
    std::vector<Observer*> _observers;
    
    AnimationComponent * _onAnimation;
    AnimationComponent * _turnOffByWindAnimation;
    AnimationComponent * _smokeAnimation;
    
    SoundComponent * _soundFire;
    SoundComponent * _soundFireTurnOff;
    
    ParticleSystemQuad * _particleSmoke;
};

#endif // __SOP_BRAZIER__
