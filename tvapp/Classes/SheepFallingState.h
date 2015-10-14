//
//  SheepFallingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#ifndef __SOP__SHEEP_FALLING_STATE__
#define __SOP__SHEEP_FALLING_STATE__

#include "SheepState.h"

class SheepFallingState : public SheepState
{
public:
    SheepFallingState(Sheep* sheep, eAction direction);
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep, float dt);
    virtual void stopAnimation();
    
private:
    void setFallingGraphics(Sheep* sheep);
    void setRightFallingGraphics(Sheep* sheep);
    void setLeftFallingGraphics(Sheep* sheep);
    void createLandingAnimation(Sheep* sheep);
    
    void handleDirection(Sheep* sheep, eAction input);
    SheepState* handleStopFall(Sheep* sheep);
    SheepState* handleDeath(Sheep* sheep, eAction input);
    SheepState* handleFlyInput(Sheep* sheep, eAction input);
    
    bool _isRightPressed;
    bool _isLeftPressed;
    
    AnimationComponent* _animationFall;
    SoundComponent* _soundLanding;
};


#endif // __SOP__SHEEP_FALLING_STATE__
