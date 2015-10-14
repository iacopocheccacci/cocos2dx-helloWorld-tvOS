//
//  SheepWalkingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#ifndef __SOP__SHEEP_WALKING_STATE__
#define __SOP__SHEEP_WALKING_STATE__

#include "SheepState.h"

class SheepWalkingState : public SheepState
{
public:
    SheepWalkingState(Sheep* sheep, eAction input);
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep, float dt);
    virtual void stopAnimation();
    
private:
    void setWalkingRightGraphics(Sheep* sheep, bool begin);
    void setWalkingLeftGraphics(Sheep* sheep, bool begin);
    
    SheepState* handleStopMove(Sheep* sheep);
    SheepState* handleJump(Sheep* sheep);
    SheepState* handleFall(Sheep* sheep, eAction input);
    SheepState* handleBurn(Sheep* sheep, eAction input);
    SheepState* handleFlyInput(Sheep* sheep, eAction input);
    
    void createStartWalkAnimation(LHSprite* sprite);
    
    eAction _direction;
    AnimationComponent* _animationWalk;
    AnimationComponent* _animationCatch;
    
    SoundComponent* _soundExcited;
    SoundComponent* _soundWalk;
    
    bool _isWalkingRight;
    bool _isWalkingLeft;
};

#endif // __SOP__SHEEP_WALKING_STATE__
