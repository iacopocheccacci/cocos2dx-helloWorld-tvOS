//
//  Goatskin.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/12/14.
//
//

#ifndef __SOP_GOATSKIN__
#define __SOP_GOATSKIN__

#include "InteractiveBlock.h"
#include "Definitions.h"

class AnimationComponent;
class SoundComponent;

enum class eGoatskinStatus : int
{
    GOATSKIN_IDLE = 0,
    GOATSKIN_ACTIVE,
};
    
namespace goatskinDefs
{
    static const char* CLASS_NAME = "Goatskin";
    static const char* SPRITE_NAME = "GoatskinSprite";
    
    static const float THRESHOLD_TURN_ON = 11.0f;
    static const float THRESHOLD_SHAKE = 3.0f;
};

class Goatskin : public InteractiveBlock
{
public:
    NODEWITHDICT_FUNC(Goatskin);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    bool turnOn(float impulse, bool begin);
    void setStatus(eGoatskinStatus status)          { _status = status; }
    eGoatskinStatus getStatus()                     { return _status; }
    void setRange(float range)                      { _range = range; }
    float getRange()                                { return _range; }
    void openAction();
    void blowAction();
    void closeAction()                              { _status = eGoatskinStatus::GOATSKIN_IDLE; }
    
private:
    void initAnimation();
    void initAudio();
    
    eGoatskinStatus _status;
    float _range;
    
    AnimationComponent * _animationOpen;
    AnimationComponent * _animationBlow;
    
    SoundComponent * _soundPressed;
    SoundComponent * _soundBlow;
};

#endif // __SOP_GOATSKIN__
