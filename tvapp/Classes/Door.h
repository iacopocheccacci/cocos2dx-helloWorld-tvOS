//
//  Door.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/12/14.
//
//

#ifndef __SOP_DOOR__
#define __SOP_DOOR__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Observer.h"
#include "Definitions.h"

class SoundComponent;

USING_NS_CC;

namespace doorDefs
{
    static const char* CLASS_NAME = "Door";
    static const char* SHADOW_SPRITE_NAME = "shadow";
    static const char* HORIZONTAL_DOOR_TAG = "HORIZONTAL";
    
    static const float SHAKE_FORCE = 10.0f;
    static const float SHAKE_DURATION = 0.5f;
}

enum class eDoorType : int
{
    OR_DOOR,
    AND_DOOR,
};

enum class eDoorStatus;

class Door : public LHSprite, public Observer
{
public:
    NODEWITHDICT_FUNC(Door);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt) override;
    
    void updateSwitchCounter();
    eDoorStatus getStatus()                 { return _status; }
    eDoorType getType()                     { return _type; }
    
    // Observer method
    void update(eEventType event, cocos2d::Ref * sender, void * options) override;
//    virtual void update(eEventType event, Ref * sender);
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
private:
    
    void initAudio();
    void initBounds();
    
    // scheduler
    void update(float dt) override;
    
    eDoorType checkDoorType();
    void updateActiveSwitch(bool increment);
    bool checkIfUnlocked();
    void openDoor(Ref * doorSwitch);
    void closeDoor(Ref * doorSwitch);
    void shakeDoor(Ref * doorSwitch);
    
    void playShakeEffect();
    void resetShakeEffect();
    
    void playShadowEffectOpen();
    void playShadowEffectClose();
    
    void notify(eEventType event, Ref * doorSwitch);
    
    eDoorStatus _status;
    eDoorType _type;
    int _switchCounter;
    int _activeSwitch;
    int _enterTimer;
    
    SoundComponent * _soundOpen;
    SoundComponent * _soundClose;
    SoundComponent * _soundShake;
    
    std::vector<Observer*> _observers;
    Point _upperBound;
    Point _lowerBound;
    bool _isInverseDirection;
};

#endif // __SOP_DOOR__
