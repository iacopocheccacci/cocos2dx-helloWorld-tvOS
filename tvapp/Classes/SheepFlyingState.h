//
//  SheepFlyingState.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 28/05/15.
//
//

#ifndef __SOP__SHEEP_FLYING_STATE__
#define __SOP__SHEEP_FLYING_STATE__

#include "SheepState.h"
#include "Definitions.h"

class AnimationComponent;
class SoundComponent;

class SheepFlyingState : public SheepState
{
public:
    SheepFlyingState(Sheep* sheep, eAction input);
    virtual ~SheepFlyingState();
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep, float dt);

private:
    void setGraphics(Sheep* sheep, eAction input);
    SheepState* handleStopFlyInput(Sheep* sheep);
    void startAnimation(Sheep* sheep);
    void stopAnimation(Sheep* sheep);
    
    AnimationComponent* _animationFly;
    SoundComponent* _soundComponent;
    
    Action * _actionLoop;
    Point _originalSpritePosition;
};

#endif // __SOP__SHEEP_FLYING_STATE__
