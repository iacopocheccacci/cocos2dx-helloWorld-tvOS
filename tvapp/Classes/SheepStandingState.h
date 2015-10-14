//
//  SheepStandingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#ifndef __SOP__SHEEP_STANDING_STATE__
#define __SOP__SHEEP_STANDING_STATE__

#include "SheepState.h"

class SoundComponent;

class SheepStandingState : public SheepState
{
public:
    SheepStandingState(Sheep* sheep);
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep, float dt);
    virtual void stopAnimation();
    
private:
    void setGraphics(Sheep* sheep);
    void createRandomAnimation(Sheep * sheep);
    eAction getDirection(Sheep* sheep);
    void createCatchAnimation(Sheep* sheep);
    
    SheepState* handleMove(Sheep* sheep, eAction input);
    SheepState* handleJump(Sheep* sheep, eAction input);
    SheepState* handleFall(Sheep* sheep, eAction input);
    SheepState* handleDeath(Sheep* sheep, eAction input);
    SheepState* handleFlyInput(Sheep* sheep, eAction input);
    
    AnimationComponent* _animationIdle1;
    AnimationComponent* _animationIdle2;
    AnimationComponent* _animationIdle3;
    AnimationComponent* _animationCatch;
    
    SoundComponent * _soundExcited;
    
    float _animationDuration;
    float _timer;
};

#endif // __SOP__SHEEP_STANDING_STATE__
