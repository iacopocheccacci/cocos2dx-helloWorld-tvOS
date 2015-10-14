//
//  PlayerDeathState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#ifndef __SOP__PLAYER_DEATH_STATE__
#define __SOP__PLAYER_DEATH_STATE__

#include "PlayerState.h"

class PlayerDeathState : public PlayerState
{
public:
    PlayerDeathState(Player* player, eAction input);
    virtual ~PlayerDeathState();
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);
    
private:
    void setGraphics(Player* player, eAction input);
    void stopAnimation(Player* player);
    
    AnimationComponent* _animationBurn;
    SoundComponent* _soundBurn;
};


#endif // __SOP__PLAYER_DEATH_STATE__
