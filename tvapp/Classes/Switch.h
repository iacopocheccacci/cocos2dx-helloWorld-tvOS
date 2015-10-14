//
//  Switch.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/12/14.
//
//

#ifndef __SOP_SWITCH__
#define __SOP_SWITCH__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Observer.h"
#include "Definitions.h"

USING_NS_CC;

namespace switchDefs
{
    static const char* CLASS_NAME = "Switch";
}

class SoundComponent;

class Switch : public LHNode
{
public:
    NODEWITHDICT_FUNC(Switch);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void updateStatus(bool begin);
    
private:
    
    void initAudio();
    
    void notify(eEventType event);
    void moveSwitch();
    
    std::vector<Observer*> _observers;
    bool _switchOn;
    unsigned int _itemBelowCounter;
    
    SoundComponent * _soundPress;
    SoundComponent * _soundRelease;
};

#endif // __SOP_SWITCH__
