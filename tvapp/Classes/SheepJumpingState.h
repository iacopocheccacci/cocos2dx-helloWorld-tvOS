//
//  SheepJumpingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#ifndef __SOP__SHEEP_JUMPING_STATE__
#define __SOP__SHEEP_JUMPING_STATE__

#include "SheepState.h"

class SheepJumpingState : public SheepState
{
public:
    SheepJumpingState(Sheep* sheep, eAction input);
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep, float dt);
    virtual void stopAnimation();
    
private:
    void setJumpingRightGraphics(Sheep* sheep);
    void setJumpingLeftGraphics(Sheep* sheep);
    void setJumpingGraphics(Sheep* sheep);
    void createJumpAnimation(LHSprite* sprite);
    void createLandingAnimation(Sheep* sheep);
    
    SheepState* handleJump(Sheep* sheep, eAction input);
    void handleDirection(Sheep* sheep, eAction input);
    SheepState* handleStopJump(Sheep* sheep);
    SheepState* handleStopFall(Sheep* sheep);
    SheepState* handleDeath(Sheep* sheep, eAction input);
    SheepState* handleFlyInput(Sheep* sheep, eAction input);
    
    AnimationComponent* _animationJump;
    AnimationComponent* _animationFall;
    
    SoundComponent* _soundJump;
    SoundComponent* _soundLanding;
    
    bool _isFalling;
};

#endif // __SOP__SHEEP_JUMPING_STATE__
