//
//  SheepDeathState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#ifndef __SOP__SHEEP_DEATH_STATE__
#define __SOP__SHEEP_DEATH_STATE__

#include "SheepState.h"

class SheepDeathState : public SheepState
{
public:
    SheepDeathState(Sheep* sheep, eAction input);
    virtual SheepState* handleInput(Sheep* sheep, eAction input);
    virtual void update(Sheep* sheep);
    
private:
    void setGraphics(Sheep* sheep, eAction input);
    void stopAnimation(Sheep* sheep);
    void burnAndRemove(Sheep* sheep);
    
    AnimationComponent* _animationBurn;
    SoundComponent * _soundBurn;
};


#endif // __SOP__SHEEP_DEATH_STATE__
